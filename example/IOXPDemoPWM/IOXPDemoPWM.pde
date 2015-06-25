/************************************************************************/
/*				                        		*/
/*	  IOXPDemoPWMProject 		                                */
/*						                  	*/
/*					                        	*/
/************************************************************************/
/*	Author: Cristian Fatu						*/
/*	Copyright 2012, Digilent Inc.					*/
/************************************************************************/
/*  File Description: 			             		        */
/*					                        	*/
/* This file implements a simple demo application that demonstrates     */
/* how to use the IOXP library to generate PWM		                */
/*									*/
/*	Functionality:							*/
/*									*/
/* In the setup() function, the application initializes                 */
/* the IOXP library object and PWM module.                              */
/*					                        	*/
/*	Required Hardware:						*/
/*	  1. Cerebot MX4cK                  			        */
/*	  2. PmodIOXP - plugged into I2C#2 interface	                */
/* The PWM is generated on pin 4 of J1 (R3) of PmodIOXP.                */
/*					                        	*/
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
  
  // initialize PmodIOXP on I2C #1
  myIOXP.begin(); 
  
  myIOXP.SetRegisterBit(IOXP_GENERAL_CFG_B_OSC_EN,  1);  // enable internal 1 MHz oscillator.

  myIOXP.SetCoreFreq(IOXP_GENERAL_CFG_B_CORE_FREQ_500K);  // core freq. set to 500 kHz
  myIOXP.SetRegisterBit(IOXP_PWM_CFG_PWM_EN, 1);  // PWM_EN bit set to 1

  myIOXP.SetRegisterBitsGroup(IOXP_PIN_CONFIG_D_R3_EXTEND, 2);  // R3_EXTEND_CFG[1:0] set to 10 (R3 reconfigured as PWM)

  // set time off, on (this must be the last)
  myIOXP.SetPWMOffTimeUS(20000);  // 20 ms
  myIOXP.SetPWMOnTimeUS(40000);   // 40 ms
  
  Serial.println("IOXP Demo PWM initialized, 20 ms OFF, 40 ms ON");  // display text on serial monitor
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
**        This function processes the variables iKeyVal, bRow, bCol, bGPI, bEventState, bLock, bUnlock set by keyProcess() function and displays on serial terminal the event details.
**        The function also ouputs to GPO pins a pattern. For key pressed and GPI events, the output pattern is shifted.
/*** ---------------------------------------------------------- ***/
void loop()
{  
}



