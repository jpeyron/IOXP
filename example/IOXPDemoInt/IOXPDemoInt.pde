/************************************************************************/
/*				                        		*/
/*	  IOXPDemoProject 		                                */
/*						                  	*/
/*					                        	*/
/************************************************************************/
/*	Author: Cristian Fatu						*/
/*	Copyright 2012, Digilent Inc.					*/
/************************************************************************/
/*  File Description: 			             		        */
/*					                        	*/
/* This file implements a simple demo application that demonstrates     */
/* how to use the IOXP library.				                */
/*									*/
/*	Functionality:							*/
/*									*/
/* This demo uses IOXP library with a PmodKYPD connected on J1  	*/
/* connector of PmodIOXP. 						*/
/* It also shows how to use input / output pins of PmodIOXP.            */
/* Events are visualized on keys (press, release) and                   */
/* changed value of input GPIO 5. (pin 1 of J2 on PmodIOXP)             */
/* Input pins are read, and an output patern is outputed to GPIO 13 - 16*/
/*					                        	*/
/*	Required Hardware:						*/
/*	  1. Cerebot MX4cK                			        */
/*	  2. PmodIOXP - plugged into I2C#2 interface	                */
/*	  3. PmodKYPD - plugged into J1 connector of PmodIOXP           */
/*  INT pin of IOXP is connected to INT2 pin of MX4 (JE.7 - RE9)        */
/*						                     	*/
/************************************************************************/
/*  Revision History:			        			*/
/*					                        	*/
/*	03/20/2012(Cristian Fatu): created	       			*/
/*						                     	*/
/************************************************************************/

/* -------------------------------------------------------------------- */
/*				Include File Definitions  	        */
/* -------------------------------------------------------------------- */



#include <IOXP.h>

#include <Wire.h> // Wire library header must be included in the sketch


/* -------------------------------------------------------------------- */
/*				Global Variables		        */
/* -------------------------------------------------------------------- */
  IOXP myIOXP; // the library object
  
  char sMsg[200];  // character string to keep message that is displayed on serial monitor
  uint8_t bRow, bCol, bGPI, bEventState;  
  int iKeyVal;
  
  volatile int cntInt;

  void keyProcess()
  {
    cntInt++;
  }
/* -------------------------------------------------------------------- */
/*	               Procedure Definitions	                        */
/* -------------------------------------------------------------------- */
/***	setup
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		 Performs basic initialization.		
/*** ---------------------------------------------------------- ***/
void setup()
{ 
  //Create a connection to display the data on the serial monitor.
  Serial.begin(9600);
  // initialize PmodIOXP on default I2C (#2 for MX4cK)
  myIOXP.begin(); 
  // GPIO configiration:
  // R0-R3, C0-C3 used for keyboard
  myIOXP.SetKeyboardPinConfig(0x0F, 0x0F); //R0-R3, C0-C3
  
  // GPIO 16 - 13 outputs (connector J2 10-7, lower row)
  // GPIO  8 -  5  inputs (connector J2  4-1, upper row)  
  myIOXP.SetGPIODirection(0xF000);  

  myIOXP.SetRegisterBit(IOXP_GPI_EVENT_EN_GPI_EVENT_EN(5), 1);   // configure GPI 5 to generate event  
  myIOXP.SetRegisterBit(IOXP_GENERAL_CFG_B_OSC_EN,  1);  // enable internal 1 MHz oscillator.
  
  // configure interrupt:
  //    - event mask : interrupt on event and on lock
  //    - interrupt pin is connected to External interrupt 2 (INT2) pin.
  //    - keyProcess function will handle the interrupt
  myIOXP.ConfigureInterrupt(PAR_EXT_INT2, IOXP_INT_EN_EVENT_IEN, keyProcess);
  
  // display info message 
  sprintf(sMsg, "IOXP Demo Initialized \n\rMan_ID: %X, Rev_ID: %X \n\rPress any key and this will generate interrupt", myIOXP.GetRegisterBitsGroup(IOXP_ID_MAN_ID), myIOXP.GetRegisterBitsGroup(IOXP_ID_REV_ID));
  Serial.println(sMsg);  // display text on serial monitor
  
  // initialize variables
  iKeyVal = -1;
  bRow = 0xFF;
  bCol = 0xFF;   
  bGPI = 0xFF;
  bEventState = 0xFF;
}  

/*** ---------------------------------------------------------- ***/
/***	loop
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**	  Main program module. Enters the main program loop.
**        This function processes the variables iKeyVal, bRow, bCol, bGPI, bEventState, bLock, bUnlock set by keyProcess() function and displays on serial terminal the event details and input pins value.
**        The function also ouputs a pattern value to GPO pins. For key pressed and GPI events, the output pattern is shifted.
/*** ---------------------------------------------------------- ***/
void loop()
{  
  uint8_t cntEvents, bVal;  
  char sKeyVal[10];
  sprintf(sMsg, "Loop, no of interrupts: %d", cntInt);
  Serial.println(sMsg);  // display text on serial monitor
  myIOXP.SetRegisterBitsGroup(IOXP_INT_STATUS_ALL, 0x3F);  // write 1 on all INT_STATUS bits, thus clearing and rearming the interrupt
  while(cntEvents = myIOXP.GetRegisterBitsGroup(IOXP_STATUS_EC))
  {
     // while the count of events is not 0, decode events, one by one
     // events can be caused by keys or GPIO
    myIOXP.ReadFIFO(iKeyVal, bRow, bCol, bGPI, bVal, bEventState);
    if((bRow != 0xFF) && (bCol != 0xFF))
    {
      // keyboard event
      if(iKeyVal != -1)
      {
        sprintf(sKeyVal, "%X", iKeyVal);
      }
      else
      {
        strcpy(sKeyVal, " "); // blank, designates no value defined for the key 
      }
      if(bEventState != 0)
      {
        sprintf(sMsg, "Key pressed : Row %d, Col %d, Val %c", bRow, bCol, sKeyVal[strlen(sKeyVal) - 1]); // format text to be displayed
      }
      else
      {
        sprintf(sMsg, "Key released: Row %d, Col %d, Val %c", bRow, bCol, sKeyVal[strlen(sKeyVal) - 1]); // format text to be displayed
      }    
      Serial.println(sMsg);  // display text on serial monitor
      bRow = 0xFF;  // reset variables
      bCol = 0xFF;  // reset variables
    }
      if(bGPI != 0xFF)
      {
        // GPI event
        uint8_t bVal;
        uint32_t dwInputsVal;
        bVal = myIOXP.GetRegisterBit(IOXP_GPI_STATUS_GPI_STAT(bGPI));
        dwInputsVal = myIOXP.GetGPIStat();
        sprintf(sMsg, "GPI[%d]=%d, Inputs: %X", bGPI, bVal, dwInputsVal); // format text to be displayed    
        Serial.println(sMsg);  // display text on serial monitor
        bGPI = 0xFF;  // reset variables
      }  
  }
  delay(500);
}




