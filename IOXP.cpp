/************************************************************************/
/*																		*/
/*	IOXP.cpp		--		Definition for IOXP library 	    		*/
/*																		*/
/************************************************************************/
/*	Author:		Cristian Fatu											*/
/*	Copyright 2012, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*		This file defines functions for IOXP							*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	01/10/2012(Cristian Fatu): created	                                */
/*	06/19/2015(JonP) Revision	                                        */
/************************************************************************/
/*	Needs work:															*/
/*																		*/
/*		Various functions are not fully working or could be added       */
/*      in the library such as:                                         */
/*       - The lock and unlock for the FIFO is not fully working        */
/*		 - Add more functionality with the Logic Blocks                 */
/*		 - Add more functionality with the pull-up and pull-down        */
/*         Resistors                                                    */								
/*		 - Add more functionality with the Clock divider                */
/*																		*/
/************************************************************************/
/*                                                                      */
/*  This library is free software; you can redistribute it and/or       */
/*  modify it under the terms of the GNU Lesser General Public          */
/*  License as published by the Free Software Foundation; either        */
/*  version 2.1 of the License, or (at your option) any later version.  */
/*                                                                      */
/*  This library is distributed in the hope that it will be useful,     */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of      */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU   */
/*  Lesser General Public License for more details.                     */
/*                                                                      */
/*  You should have received a copy of the GNU Lesser General Public    */ 
/*  License along with this library; if not, write to the Free Software */
/*  Foundation, Inc.,51 Franklin St, Fifth Floor, Boston, MA  02110-1301*/ 
/*  USA                                                                 */
/************************************************************************/

/* -------------------------------------------------------------------- */
/*				Include File Definitions						        */
/* -------------------------------------------------------------------- */
#include "IOXP.h"
#include <Wire.h>
#include <sys/attribs.h>


#include <WProgram.h>

/* -------------------------------------------------------------------- */
/*				Procedure Definitions							        */
/* -------------------------------------------------------------------- */

IOXP::IOXP()
{
	SetKeyMap(keyMap_KYPD);
}

/* -------------------------------------------------------------------- */
/*	IOXP::WriteBytesI2C                                                 */
/*	Synopsis:                                                           */
/*		WriteBytesI2C(bRegisterAddress, bCntBytes,  rgbValues);         */
/*	Parameters:                                                         */  
/*		uint8_t bAddress   – the address where the values will be       */
/*                           written                                    */
/*  	uint8_t bCntBytes  – the number of bytes that will be written   */
/*		uint8_t *rgbValues - the array of values to be written          */
/*                                                                      */	                                                     
/*  Return Value:                                                       */
/*		void                                                            */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*		This function writes the values from the buffer to the          */ 
/*  	specified number of registers starting from a specified address */
/*		value. It performs the I2C write cycle for the specified array  */ 
/*  	of values to the specified address.                             */
/* -------------------------------------------------------------------- */

void IOXP::WriteBytesI2C(uint8_t bAddress, uint8_t bCntBytes, uint8_t *rgbValues)
{
    Wire.beginTransmission(IOXP_I2C_ADDR); //start transmission to device 
    Wire.send(bAddress);        // send register address
	int nIdxBytes;
	for(nIdxBytes = 0; nIdxBytes < bCntBytes; nIdxBytes++)
	{
		Wire.send(rgbValues[nIdxBytes]); // send value to write
	}	
	Wire.endTransmission(); //end transmission
}

/* -------------------------------------------------------------------- */
/*	IOXP::ReadBytesI2C                                                  */
/*                                                                      */
/*	Synopsis:                                                           */
/*	   ReadBytesI2C(bRegisterAddress, bCntBytes, rgbValues);            */
/*	Parameters:                                                         */  
/*	   uint8_t bAddress   – the address from where the values will be   */
/*	                        read                                        */
/*	   uint8_t bCntBytes  – the number of bytes that will be read       */
/*	   uint8_t *rgbValues - the array where values will be read         */
/*                                                                      */	                                                     
/*  Return Value:                                                       */
/*		void                                                            */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*		This function will read the specified number of registers       */ 
/*  	starting from a specified address and store their values in the */
/*		buffer. It performs the I2C read cycle from the specified       */ 
/*  	address into the specified array of values.                     */
/* -------------------------------------------------------------------- */

void IOXP::ReadBytesI2C(uint8_t bAddress, uint8_t bCntBytes, uint8_t *rgbValues)
{      

	Wire.beginTransmission(IOXP_I2C_ADDR); 	//start transmission to idDevice 
	Wire.send(bAddress);        			//send address to read from
	Wire.endTransmission(); 				//end transmission
	Wire.beginTransmission(IOXP_I2C_ADDR); 	//start transmission to idDevice (initiate again)
	Wire.requestFrom(IOXP_I2C_ADDR, (int)bCntBytes);// request bCntBytes bytes from IOXP_I2C_ADDR device                		

	uint8_t bIdxBytes = 0;

	while(Wire.available() && bIdxBytes < bCntBytes)
	{ 
		rgbValues[bIdxBytes] = Wire.receive(); // receive a byte
		bIdxBytes++;
	}
	Wire.endTransmission(); 				//end transmission
}

/* -------------------------------------------------------------------- */
/*	IOXP::WriteMaskedRegisterValue                                      */
/*                                                                      */
/*	Synopsis:                                                           */
/* 	  	WriteMaskedRegisterValue(uint8_t bAddress, uint8_t bMask,       */
/*      uint8_t bVal);                                                  */
/*	Parameters:                                                         */  
/*	  	uint8_t bAddress - the register address                         */
/*	  	uint8_t bMask    - the mask controlling the write operation     */
/*	  	uint8_t bVal	   - the value containing bits that will be     */
/*                           written to the register                    */	                                                     
/*  Return Value:                                                       */
/*		void                                                            */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*		Writes the value to the specified register using the specified  */ 
/*  	mask. The bits corresponding to the binary 1 values in the mask */
/*		are copied from the value to the register, while the bits       */ 
/*  	corresponding to binary 0 values in the mask remain unchanged   */
/* 		in the register.                                                */
/* -------------------------------------------------------------------- */

void IOXP::WriteMaskedRegisterValue(uint8_t bAddress, uint8_t bMask, uint8_t bVal)
{
	uint8_t bOldVal;
	ReadBytesI2C(bAddress, 1, &bOldVal);
	uint8_t bNewVal = (bOldVal & ~bMask) | (bVal & bMask); 
	WriteBytesI2C(bAddress, 1, &bNewVal);
}

/* -------------------------------------------------------------------- */
/*	IOXP::ReadMaskedRegisterValue                                       */
/*                                                                      */
/*	Synopsis:                                                           */
/*	   	ReadMaskedRegisterValue(uint8_t bAddress, uint8_t bMask);       */
/*	Parameters:                                                         */  
/*	   	uint8_t bAddress	- the register address                      */
/*	   	uint8_t bMask		- the mask controlling the read operation   */
/*                                                                      */	                                                     
/*  Return Value:                                                       */
/*	   	uint8_t bVal & bMask - the masked value                         */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*	   	Reads the value from the specified register using the specified */ 
/*     	mask. The register value is AND-ed with the mask, thus only the */
/*		bits corresponding to binary 1 values in the mask will be       */ 
/*  	returned.                                                       */
/* -------------------------------------------------------------------- */

uint8_t IOXP::ReadMaskedRegisterValue(uint8_t bAddress, uint8_t bMask)
{
	uint8_t bVal;
	ReadBytesI2C(bAddress, 1, &bVal);
	return bVal & bMask;
}

/* -------------------------------------------------------------------- */
/*	IOXP::Mask2Scale                                                   */
/*                                                                      */
/*	Synopsis:                                                           */
/*	   Mask2Scale(uint8_t bMask);                                       */
/*	Parameters:                                                         */  
/*	   uint8_t bMask	- the mask value                                */
/*                                                                      */
/*  Return Value:                                                       */
/*		uint8_t - the scale corresponding to the specified mask value   */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*		Returns the scale corresponding to the mask (the number of bits */ 
/*  	that the mask value must be shifted right so that the LSB is 1) */
/* -------------------------------------------------------------------- */

uint8_t IOXP::Mask2Scale(uint8_t bMask)
{
	uint8_t bResult = 0;
	uint8_t bVal = bMask;
	while((bMask & 1) == 0)
	{	
		bMask >>= 1;
		bResult++;
	}
	return bResult;
}

/* -------------------------------------------------------------------- */
/*	IOXP::GetKeyVal                                                     */
/*                                                                      */
/*	Synopsis:                                                           */
/*	    GetKeyVal(uint8_t bRow, uint8_t bCol);                          */
/*	Parameters:                                                         */  
/*	 	uint8_t bRow	- the key row                                   */
/*		uint8_t bCol	- the key column                                */
/*  Return Value:                                                       */
/*		uint8_t - the key value associated to the specified key         */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*		Returns the key value associated to the key specified by row    */ 
/*  	and column. The row and column correspond to the row and column */
/*      pins.                                                           */
/* -------------------------------------------------------------------- */

int IOXP::GetKeyVal(uint8_t bRow, uint8_t bCol)
{	
	int iKeyVal = -1; 
	if(bRow <= IOXP_KB_ROWS && bCol <= IOXP_KB_COLS)
	{
		iKeyVal = keyMap[bRow][bCol];
	}
	return iKeyVal;
}

/* -------------------------------------------------------------------- */
/*	IOXP::GetKeyByVal                                                   */
/*                                                                      */
/*	Synopsis:                                                           */
/*		GetKeyByVal(int iKeyVal, uint8_t &bRow, uint8_t &bCol);         */
/*	Parameters:                                                         */  
/*	 	int iKeyVal		- the key value                                 */
/*		uint8_t &bRow	- key row - output parameter where the keyboard */
/*      				  row value will be stored.                     */
/*      uint8_t &bCol	- key column - output parameter where the       */
/*                        keyboard column value will be stored.	row     */
/*						  value will be stored.                         */
/*  Return Value:                                                       */
/*		uint8_t - the key value associated to the specified key         */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*		This function detects the key that has a specific value         */ 
/*      associated to it and fills the output parameters with row and   */
/*      column information. If no key corresponds to the specified key  */
/*      value, row and value get 0xFF value. If more keys correspond to */
/*      the specified key value, the first found key will be used.      */ 
/* -------------------------------------------------------------------- */

void IOXP::GetKeyByVal(int iKeyVal, uint8_t &bRow, uint8_t &bCol)
{	
	bRow = 0xFF;
	bCol = 0xFF;
	for(int idxRows = 0; idxRows < IOXP_KB_ROWS; idxRows++)       
	{
		for(int idxCols = 0; idxCols < IOXP_KB_COLS; idxCols++)        
		{
			if(keyMap[idxRows][idxCols] == iKeyVal)
			{
				// first occurrence of iKeyVal in the table
				bRow = idxRows;
				bCol = idxCols;
				idxRows = IOXP_KB_ROWS;	// force out
				idxCols = IOXP_KB_COLS; // force out
			}
		}
	}
}   

/* -------------------------------------------------------------------- */
/*	IOXP::DecodeEvent                                                   */
/*                                                                      */
/*	Synopsis:                                                           */
/*		DecodeEvent(uint8_t bEvent, int &iKeyVal, uint8_t &bRow,        */
/*      uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic,                  */
/*		uint8_t &bEventState);                                          */
/*	Parameters:                                                         */  
/*	 	uint8_t bEvent - the event to be decoded. LSB 7 bits correspond */
/*      				 to the event identifier (96 keys, 19 GPIs, 2   */
/*                       Logics).The MSB bit correspond to the event    */ 
/*      				 state.                                         */
/*     	uint8_t bEvent	- the event to be decoded. LSB 7 bits           */
/*                        correspond to the event identifier (96 keys,  */
/*						  19 GPIs, 2 Logics). The MSB bit correspond    */
/*						  to the event state.                           */
/*		int &iKeyVal -  key value - output parameter where the key      */
/*					    value will be stored. It contains -1 value if   */ 
/*					    event is not a keyboard event or if the         */
/*					    the key map table contains -1 value.            */                                                    
/*		uint8_t &bRow - key row - output parameter where the keyboard   */
/*					    row value will be stored. It contains 0xFF      */
/*						value if the event is not a keyboard event.     */                                                          
/*      uint8_t &bCol - key column - output parameter where the         */
/*						keyboard column value will be stored. It        */
/*						contains 0xFF value if the event is not a       */
/*						keyboard event or if the event corresponds to   */
/*						(Rx, GND)			                            */	                                               
/*                                                                      */
/*      uint8_t &bGPI - GPI no - output parameter where the GPI value   */
/*						will be stored.It contains 0xFF value if the    */
/*						event is not a GPI event.                       */                                                            
/*      uint8_t &bLogic	- Logic no - output parameter where the Logic   */
/*						  value will be stored.	It contains 0xFF value  */
/*						  if the event is not a Logic event.            */
/*                                                                      */
/*      uint8_t &bEventState - Event state - output parameter where	the */
/*							   state of the event will be placed.       */
/*						For keys events:                                */
/*								1 - key is pressed                      */
/*								0 - key is released                     */
/*						For GPI and Logic events:                       */
/*								1 - GPI/logic is active                 */
/*								0 - GPI/logic is inactive               */                                                     
/*						                                                */
/*  Return Value:                                                       */
/*		void 	                                                        */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*		Decodes the information corresponding to a specific event.      */
/*		If the event is a key event, key value, row and column the      */
/*      information corresponding to the key are stored in output       */
/*		parameters while the other parameters corresponding to GPI      */
/*		events (bGPI) and Logic events (bLogic) are set to 0xFF. If     */
/*		the event is a GPI event, the GPI number is stored in bGPI      */
/*		while the other parameters corresponding to key events          */
/*		(keyVal, bRow, bCol) and to Logic events (bLogic) are set       */
/*		to -1 (keyVal) and 0xFF respectively. If the event is a Logic   */
/*		event, the Logic number is stored in bLogic, while the          */
/*		parameters corresponding to key events (keyVal, bRow, bCol)     */
/*		and to Key events are set to -1 (keyVal) and 0xFF respectively. */                                                      
/* -------------------------------------------------------------------- */

void IOXP::DecodeEvent(uint8_t bEvent, int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState)
{
	bEventState = (bEvent & 0x80) != 0 ? 1: 0;
	uint8_t bEventNo = bEvent & 0x7F;
	iKeyVal = -1; // default value
	bRow = 0xFF; // default value
	bCol = 0xFF; // default value
	bGPI = 0xFF; // default value
	bLogic = 0xFF; // default value
	if(bEventNo >= 1 && bEventNo <= IOXP_KB_ROWS * IOXP_KB_COLS)
	{
		// (Row, Column)
		// decode the row and column by the event number
		bRow = (uint8_t)((bEventNo - 1)/IOXP_KB_COLS);
		bCol = bEventNo - 1 - IOXP_KB_COLS * bRow;
		iKeyVal = GetKeyVal(bRow, bCol);
	}
	else
	{
		if(/*bEventNo >= 1 + IOXP_KB_ROWS * IOXP_KB_COLS && */bEventNo <= 1 + IOXP_KB_ROWS * (IOXP_KB_COLS + 1))
		{
			// (Row, GND)
			// bCol remains 0xFF, corresponding to GND
			// decode the row by the event number
			bRow = bEventNo - (1 + IOXP_KB_ROWS * IOXP_KB_COLS);
		}
		else
		{
			if(/*bEventNo >= 1 + IOXP_KB_ROWS * (IOXP_KB_COLS + 1) && */bEventNo <= 1 + IOXP_KB_ROWS * (IOXP_KB_COLS + 1) + IOXP_GPIOS)
			{
				// GPI
				// decode the GPI number by the event number
				bGPI = bEventNo - (IOXP_KB_ROWS * (IOXP_KB_COLS + 1));
			}
			else
			{
				if(/*bEventNo >= 1 + IOXP_KB_ROWS * (IOXP_KB_COLS + 1) + IOXP_GPIOS && */bEventNo <= 1 + IOXP_KB_ROWS * (IOXP_KB_COLS + 1) + IOXP_GPIOS + IOXP_NO_LOGIC)
				{
					// Logic
					// decode the Logic number by the event number
					bGPI = bEventNo - (IOXP_KB_ROWS * (IOXP_KB_COLS + 1) + IOXP_GPIOS);
				}
			}
		}
	}
}

/* -------------------------------------------------------------------- */
/*	IOXP::EncodeEvent                                                   */
/*                                                                      */
/*	Synopsis:                                                           */
/*		EncodeEvent(int iKeyVal, uint8_t bRow, uint8_t bCol,            */
/*		uint8_t bGPI, uint8_t bLogic, uint8_t bEventState);             */
/*	Parameters:                                                         */  
/*		int iKeyVal	- key value. It contains -1 value if the event      */
/*					  is not a keyboard event or if the key is          */
/*					  specified by row and column information.          */
/*      uint8_t bRow - key row value. It contains 0xFF value if the     */
/*                     iKeyVal specifies the key or if the event is not */
/*					   a keyboard event. this parameter has a default   */
/*					   value of 0xFF.				                    */
/*                                            */
/*      uint8_t bCol - key column value. It contains 0xFF value if the  */
/*					   iKeyVal specifies the key or if the event is not */
/*					   a keyboard event or if the event corresponds to  */
/*					   (Rx, GND). This parameter has a default value of */
/*					   0xFF.                                            */
/*		uint8_t bGPI - GPI no - output parameter where the GPI value    */
/*					   will be stored. It contains 0xFF value if the    */
/*					   event is not a GPI event. This parameter has a   */
/*					   default value of 0xFF.				            */
/*      uint8_t bLogic - Logic no - output parameter where the Logic    */
/*						 value will be stored. It contains 0xFF value   */
/*						 if the event is not a Logic event. This        */
/*						 parameter has a default value of 0xFF.         */
/*      uint8_t bEventState - Event state                               */
/*						For keys events:                                */
/*								1 - key is pressed                      */
/*								0 - key is released                     */
/*						For GPI and Logic events:                       */
/*								1 - GPI/logic is active                 */
/*								0 - GPI/logic is inactive               */
/*						This parameter has a default value of 0.        */
/*  Return Value:                                                       */
/*		uint8_t - the event byte corresponding to the specified data    */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*		This function builds the event byte. If the event is a key      */
/*		event,it may be specified by key value or by row and column     */
/*		pair. If both ways are used, row and column pair is further     */
/*		used. If a key value corresponds to more keys, the first found  */
/*		will be used. Its LSB 7 bits correspond to the event identifier */
/*		(96 keys, 19 GPIs, 2 Logics). The MSB bit correspond to the     */
/*		event state. The last five parameters have default values       */ 
/* -------------------------------------------------------------------- */

uint8_t IOXP::EncodeEvent(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState)
{
	uint8_t bEvent = 0xFF;
	if((iKeyVal != -1) || (bRow != 0xFF) || (bCol != 0xFF))
	{	// keyboard event
		if(iKeyVal != -1 && bRow == 0xFF && bCol == 0xFF)
		{
			// if key value is defined and row and columns are not defined
			GetKeyByVal(iKeyVal, bRow, bCol); 
		}
		if(bRow != 0xFF)
		{
			if(bCol == 0xFF)
			{
				// GND
				bEvent = 1 + IOXP_KB_ROWS * IOXP_KB_COLS + bRow;
			}
			else
			{
				bEvent = 1 + bRow * IOXP_KB_COLS + bCol;
			}
		}
	}
	else
	{
		if(bGPI >= 1 && bGPI <= IOXP_GPIOS)
		{	// GPI event
			bEvent = 1 + IOXP_KB_ROWS * (IOXP_KB_COLS + 1) + bGPI;
		}
		else
		{
			if(bLogic >= 0 && bLogic <= IOXP_NO_LOGIC)
			{	// Logic event
				bEvent = 1 + IOXP_KB_ROWS * (IOXP_KB_COLS + 1) + IOXP_GPIOS + bLogic;
			}
		}
	}
	if(bEvent != 0xFF)
	{
		bEvent += bEventState != 0 ? 0x80 : 0;
	}
	return bEvent;
}

/* -------------------------------------------------------------------- */
/*	IOXP::begin                                                         */
/*                                                                      */
/*	Synopsis:                                                           */
/*		myIOXP.begin();                                                 */
/*	Parameters:                                                         */  
/*	 	void                                                            */
/*  Return Value:                                                       */
/*		void                                                            */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*		This function initializes the I2C interface #1 that is used to */
/*		communicate with PmodIOXP.                                      */ 
/* -------------------------------------------------------------------- */

void IOXP::begin()
{
	Wire.begin();
}

/* --------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetRegisterBit                                                                                                       */
/*                                                                                                                             */
/*	Synopsis:                                                                                                                  */
/*		SetRegisterBit(uint16_t wBitDef, uint8_t bBitVal);                                                                     */
/*	Parameters:                                                                                                                */  
/*	 	uint16_t wBitDef - the definition of the bit to be written.This                                                        */
/*						   definition consists of two bytes: high byte                                                         */
/*						   is the register address, low byte is the                                                            */
/*						   mask corresponding to the specified bit.                                                            */
/*		                                                                                                                       */
/*				It can be one of the following:                                                                                */
/*			IOXP_INT_STATUS_EVENT_INT   		(0x0101)	- EVENT_INT bit of the INT_STATUS register                         */
/*			IOXP_INT_STATUS_GPI_INT     		(0x0102)	- GPI_INT bit of the INT_STATUS register                           */
/*			IOXP_INT_STATUS_ALL   				(0x01FF)	- all bits of INT_STATUS register                                  */
/*			IOXP_INT_STATUS_OVERFLOW_INT 	 	(0x0104)	- OVERFLOW_INT bit of the INT_STATUS register                      */
/*			IOXP_INT_STATUS_LOCK_INT   		  	(0x0108)	- LOCK_INT bit of the INT_STATUS register                          */
/*			IOXP_INT_STATUS_LOGIC1_INT  		(0x0110)	- LOGIC1_INT bit of the INT_STATUS register                        */
/*			IOXP_INT_STATUS_LOGIC2_INT  		(0x0120)	- LOGIC2_INT bit of the INT_STATUS register                        */
/*			IOXP_STATUS_LOCK_STAT  				(0x0220)	- LOCK_STAT bit of the Status register                             */
/*			IOXP_STATUS_LOGIC1_STAT  			(0x0240)	- LOGIC1_STAT bit of the Status register                           */
/*			IOXP_STATUS_LOGIC2_STAT  	 		(0x0280)	- LOGIC2_STAT bit of the Status register                           */
/*			IOXP_UNLOCK1_UNLOCK1_STATE	  		(0x3340)	- UNLOCK1_STATE bit of the UNLOCK1 register                        */
/*			IOXP_UNLOCK2_UNLOCK2_STATE	  		(0x3440)	- UNLOCK2_STATE bit of the UNLOCK2 register                        */
/*			IOXP_EXT_LOCK_EVENT_EXT_LOCK_STATE	(0x3540)	- EXT_LOCK_STATE bit of the EXT_LOCK register                      */
/*			IOXP_LOCK_CFG_LOCK_EN	  			(0x3701)	- LOCK_EN bit of the LOCK_CFG register			                   */
/*			IOXP_RESET1_EVENT_A_RESET1_EVENT_A_LEVEL	(0x3880)	- RESET1_EVENT_A_LEVEL bit of the RESET1_EVENT_A register  */
/*			IOXP_RESET1_EVENT_B_RESET1_EVENT_B_LEVEL	(0x3980)	- RESET1_EVENT_B_LEVEL bit of the RESET1_EVENT_B register  */
/*			IOXP_RESET1_EVENT_C_RESET1_EVENT_C_LEVEL	(0x3A80)	- RESET1_EVENT_C_LEVEL bit of the RESET1_EVENT_C register  */
/*			IOXP_RESET2_EVENT_A_RESET2_EVENT_A_LEVEL	(0x3B80)	- RESET2_EVENT_A_LEVEL bit of the RESET2_EVENT_A register  */
/*			IOXP_RESET2_EVENT_B_RESET2_EVENT_B_LEVEL	(0x3C80)	- RESET2_EVENT_B_LEVEL bit of the RESET2_EVENT_B register  */
/*			IOXP_RESET_CFG_RST_PASSTHRU_EN		(0x3D20)	- RST_PASSTHRU_EN bit of the RESET_CFG register                    */
/*			IOXP_RESET_CFG_RESET1_POL			(0x3D40)	- RESET1_POL bit of the RESET_CFG register                         */
/*			IOXP_RESET_CFG_RESET2_POL			(0x3D80)	- RESET2_POL bit of the RESET_CFG register                         */
/*			IOXP_PWM_CFG_PWM_EN					(0x4201)	- PWM_EN bit of the PWM_CFG register                               */
/*			IOXP_PWM_CFG_PWM_MODE				(0x4202)	- PWM_MODE bit of the PWM_CFG register                             */
/*			IOXP_PWM_CFG_PWM_IN_AND				(0x4204)	- PWM_IN_AND bit of the PWM_CFG register                           */
/*			IOXP_CLOCK_DIV_CFG_CLK_INV			(0X4340)	- CLK_INV bit of the CLOCK_DIV_CFG register                        */
/*			IOXP_CLOCK_DIV_CFG_CLK_DIV_EN		(0X4301)	- DIV_EN bit of the CLOCK_DIV_CFG register                         */
/*			IOXP_LOGIC_1_LA1_INV				(0x4408)	- LA1_INV bit of the LOGIC_1 register                              */
/*			IOXP_LOGIC_1_LB1_INV				(0x4410)	- LB1_INV bit of the LOGIC_1 register                              */
/*			IOXP_LOGIC_1_LC1_INV				(0x4420)	- LC1_INV bit of the LOGIC_1 register                              */
/*			IOXP_LOGIC_1_LY1_INV				(0x4440)	- LY1_INV bit of the LOGIC_1 register                              */
/*			IOXP_LOGIC_2_LA2_INV				(0x4508)	- LA2_INV bit of the LOGIC_2 register                              */
/*			IOXP_LOGIC_2_LB2_INV				(0x4510)	- LB2_INV bit of the LOGIC_2 register                              */
/*			IOXP_LOGIC_2_LC2_INV				(0x4520)	- LC2_INV bit of the LOGIC_2 register                              */
/*			IOXP_LOGIC_2_LY2_INV				(0x4540)	- LY2_INV bit of the LOGIC_2 register                              */
/*			IOXP_LOGIC_2_LY1_CASCADE			(0x4540)	- LY1_CASCADE bit of the LOGIC_2 register                          */
/*			IOXP_LOGIC_FF_CFG_FF1_CLR			(0x4601)	- FF1_CLR bit of the LOGIC_FF_CFG register                         */
/*			IOXP_LOGIC_FF_CFG_FF1_SET			(0x4602)	- FF1_SET bit of the LOGIC_FF_CFG register                         */
/*			IOXP_LOGIC_FF_CFG_FF2_CLR			(0x4604)	- FF2_CLR bit of the LOGIC_FF_CFG register                         */
/*			IOXP_LOGIC_FF_CFG_FF2_SET			(0x4608)	- FF2_SET bit of the LOGIC_FF_CFG register                         */
/*			IOXP_LOGIC_INT_EVENT_EN_LOGIC1_INT_LEVEL	(0x4701)	- LOGIC1_INT_LEVEL bit of the LOGIC_INT_EVENT_EN register  */
/*			IOXP_LOGIC_INT_EVENT_EN_LOGIC1_EVENT_EN		(0x4702)	- LOGIC1_EVENT_EN bit of the LOGIC_INT_EVENT_EN register   */
/*			IOXP_LOGIC_INT_EVENT_EN_LY1_DBNC_DIS		(0x4704)	- LY1_DBNC_DIS bit of the LOGIC_INT_EVENT_EN register      */
/*			IOXP_LOGIC_INT_EVENT_EN_LOGIC2_INT_LEVEL	(0x4708)	- LOGIC2_INT_LEVEL bit of the LOGIC_INT_EVENT_EN register  */
/*			IOXP_LOGIC_INT_EVENT_EN_LOGIC2_EVENT_EN		(0x4710)	- LOGIC2_EVENT_EN bit of the LOGIC_INT_EVENT_EN register   */
/*			IOXP_LOGIC_INT_EVENT_EN_LY2_DBNC_DIS		(0x4720)	- LY2_DBNC_DIS bit of the LOGIC_INT_EVENT_EN register      */
/*			IOXP_PIN_CONFIG_D_R0_EXTEND  		(0x4C01)	- R0_EXTEND bit of the PIN_CONFIG_D register                       */
/*			IOXP_PIN_CONFIG_D_C9_EXTEND  		(0x4C02)	- C9_EXTEND bit of the PIN_CONFIG_D register                       */
/*			IOXP_PIN_CONFIG_D_R3_EXTEND_0	  	(0x4C04)	- R3_EXTEND_0 bit of the PIN_CONFIG_D register                     */
/*			IOXP_PIN_CONFIG_D_R3_EXTEND_1	  	(0x4C08)	- R3_EXTEND_1 bit of the PIN_CONFIG_D register                     */
/*			IOXP_PIN_CONFIG_D_C6_EXTEND	  		(0x4C10)	- C6_EXTEND bit of the PIN_CONFIG_D register                       */
/*			IOXP_PIN_CONFIG_D_R4_EXTEND	  		(0x4C20)	- R4_EXTEND bit of the PIN_CONFIG_D register                       */
/*			IOXP_PIN_CONFIG_D_C4_EXTEND	  		(0x4C40)	- C4_EXTEND bit of the PIN_CONFIG_D register                       */
/*			IOXP_PIN_CONFIG_D_PULL_SELECT  		(0x4C80)	- PULL_SELECT bit of the PIN_CONFIG_D register                     */
/*			IOXP_GENERAL_CFG_B_RST_CFG   		(0x4D01)	- RST_CFG bit of the GENERAL_CFG_B register                        */
/*			IOXP_GENERAL_CFG_B_INT_CFG     		(0x4D02)	- INT_CFG bit of the GENERAL_CFG_B register                        */
/*			IOXP_GENERAL_CFG_B_LCK_TRK_GPI   	(0x4D08)	- RST_CFG bit of the GENERAL_CFG_B register                        */
/*			IOXP_GENERAL_CFG_B_LCK_TRK_LOGIC  	(0x4D10)	- TRK_GPI bit of the GENERAL_CFG_B register                        */
/*			IOXP_GENERAL_CFG_B_CORE_FREQ0  		(0x4D20)	- CORE_FREQ0 bit of the GENERAL_CFG_B register                     */
/*			IOXP_GENERAL_CFG_B_CORE_FREQ1  		(0x4D40)	- CORE_FREQ1 bit of the GENERAL_CFG_B register                     */
/*			IOXP_GENERAL_CFG_B_OSC_EN  			(0x4D80)	- OSC_EN bit of the GENERAL_CFG_B register                         */
/*			IOXP_INT_EN_EVENT_IEN   	  		(0x4E01)	- EVENT_IEN bit of the INT_EN register                             */
/*			IOXP_INT_EN_GPI_IEN     			(0x4E02)	- GPI_IEN bit of the INT_EN register                               */
/*			IOXP_INT_EN_OVERFLOW_IEN 		 	(0x4E04)	- OVERFLOW_IEN bit of the INT_EN register                          */
/*			IOXP_INT_EN_LOCK_IEN   			  	(0x4E08)	- LOCK_IEN bit of the INT_EN register                              */
/*			IOXP_INT_EN_LOGIC1_INT  			(0x4E10)	- LOGIC1_IEN bit of the INT_EN register                            */
/*			IOXP_INT_EN_LOGIC2_INT  			(0x4E20)	- LOGIC2_IEN bit of the INT_EN register                            */
/*			IOXP_GPI_EVENT_EN_GPI_EVENT_EN(x)	((((0x21 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPI_EVENT_EN bit of the GPI_EVENT_EN registers corresponding to GPIO no x (x between 1 and 19)   */
/*          IOXP_GPI_INT_STAT_GPI_INT(x)		((((0x13 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPI_INT bit of the GPI_INT_STAT registers corresponding to GPIO no x  (x between 1 and 19)       */
/*          IOXP_GPI_STATUS_GPI_STAT(x)		 	((((0x16 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPI_STAT bit of the GPI_STATUS registers corresponding to GPIO no x  (x between 1 and 19)        */
/*          IOXP_INT_LEVEL_GPI_INT_LEVEL(x)		((((0x38 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPI_INT_LEVEL bit of GPI_INT_LEVEL registers corresponding to GPIO no x  (x between 1 and 19)    */
/*          IOXP_GPO_DATA_OUT_GPO_DATA(x)		((((0x2A + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPO_DATA bit of the GPO_DATA_OUT registers corresponding to GPIO no x  (x between 1 and 19)      */
/*          IOXP_GPO_OUT_MODE_GPO_OUT_MODE(x)	((((0x2D + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPO_OUT_MODE bit of the GPO_OUT_MODE registers corresponding to GPIO no x (x between 1 and 19)   */
/*          IOXP_GPIO_DIRECTION_GPIO_DIR(x)		((((0x30 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPIO_DIR bit of the GPIO_DIRECTION registers corresponding to GPIO no x (x between 1 and 19      */
/*                                                                                                                             */
/*		uint8_t bBitVal	- the value to be written to the specified bit. If this is 0, the bit will be cleared, otherwise       */
/*			              it will be set.     				                                                                   */                                         
/*  Return Value:                                                                                                              */
/*		void                                                                                                                   */
/*                                                                                                                             */
/*	Errors:                                                                                                                    */
/*                                                                      													   */
/*	Description:                                                    														   */
/*		This function sets the value of the specified bit to the specified value. The bit definition is made of two bytes:     */
/*		high byte is the register address, low byte is the mask corresponding to the specified bit.	Apart from the pre-defined */
/*		values listed above, the user may use this function to access any register bit, by providing its bit definition.       */ 
/* --------------------------------------------------------------------------------------------------------------------------- */

void IOXP::SetRegisterBit(uint16_t wBitDef, uint8_t bBitVal)
{
	uint8_t bRegisterAddr = *((uint8_t *)&wBitDef + 1);
	uint8_t bMask = *(uint8_t *)&wBitDef;
	WriteMaskedRegisterValue(bRegisterAddr, bMask, (bBitVal != 0) ? 0xFF: 0);
}

/* --------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetRegisterBit                                                                                                       */
/*                                                                                                                             */
/*	Synopsis:                                                                                                                  */
/*		GetRegisterBit(uint16_t wBitDef);                                                                                      */
/*	Parameters:                                                                                                                */  
/*	 	uint16_t wBitDef - the definition of the bit to be written.This                                                        */
/*						   definition consists of two bytes: high byte                                                         */
/*						   is the register address, low byte is the                                                            */
/*						   mask corresponding to the specified bit.                                                            */
/*		                                                                                                                       */
/*				It can be one of the following:                                                                                */
/*			IOXP_INT_STATUS_EVENT_INT   		(0x0101)	- EVENT_INT bit of the INT_STATUS register                         */
/*			IOXP_INT_STATUS_GPI_INT     		(0x0102)	- GPI_INT bit of the INT_STATUS register                           */
/*			IOXP_INT_STATUS_ALL   				(0x01FF)	- all bits of INT_STATUS register                                  */
/*			IOXP_INT_STATUS_OVERFLOW_INT 	 	(0x0104)	- OVERFLOW_INT bit of the INT_STATUS register                      */
/*			IOXP_INT_STATUS_LOCK_INT   		  	(0x0108)	- LOCK_INT bit of the INT_STATUS register                          */
/*			IOXP_INT_STATUS_LOGIC1_INT  		(0x0110)	- LOGIC1_INT bit of the INT_STATUS register                        */
/*			IOXP_INT_STATUS_LOGIC2_INT  		(0x0120)	- LOGIC2_INT bit of the INT_STATUS register                        */
/*			IOXP_STATUS_LOCK_STAT  				(0x0220)	- LOCK_STAT bit of the Status register                             */
/*			IOXP_STATUS_LOGIC1_STAT  			(0x0240)	- LOGIC1_STAT bit of the Status register                           */
/*			IOXP_STATUS_LOGIC2_STAT  	 		(0x0280)	- LOGIC2_STAT bit of the Status register                           */
/*			IOXP_UNLOCK1_UNLOCK1_STATE	  		(0x3340)	- UNLOCK1_STATE bit of the UNLOCK1 register                        */
/*			IOXP_UNLOCK2_UNLOCK2_STATE	  		(0x3440)	- UNLOCK2_STATE bit of the UNLOCK2 register                        */
/*			IOXP_EXT_LOCK_EVENT_EXT_LOCK_STATE	(0x3540)	- EXT_LOCK_STATE bit of the EXT_LOCK register                      */
/*			IOXP_LOCK_CFG_LOCK_EN	  			(0x3701)	- LOCK_EN bit of the LOCK_CFG register			                   */
/*			IOXP_RESET1_EVENT_A_RESET1_EVENT_A_LEVEL	(0x3880)	- RESET1_EVENT_A_LEVEL bit of the RESET1_EVENT_A register  */
/*			IOXP_RESET1_EVENT_B_RESET1_EVENT_B_LEVEL	(0x3980)	- RESET1_EVENT_B_LEVEL bit of the RESET1_EVENT_B register  */
/*			IOXP_RESET1_EVENT_C_RESET1_EVENT_C_LEVEL	(0x3A80)	- RESET1_EVENT_C_LEVEL bit of the RESET1_EVENT_C register  */
/*			IOXP_RESET2_EVENT_A_RESET2_EVENT_A_LEVEL	(0x3B80)	- RESET2_EVENT_A_LEVEL bit of the RESET2_EVENT_A register  */
/*			IOXP_RESET2_EVENT_B_RESET2_EVENT_B_LEVEL	(0x3C80)	- RESET2_EVENT_B_LEVEL bit of the RESET2_EVENT_B register  */
/*			IOXP_RESET_CFG_RST_PASSTHRU_EN		(0x3D20)	- RST_PASSTHRU_EN bit of the RESET_CFG register                    */
/*			IOXP_RESET_CFG_RESET1_POL			(0x3D40)	- RESET1_POL bit of the RESET_CFG register                         */
/*			IOXP_RESET_CFG_RESET2_POL			(0x3D80)	- RESET2_POL bit of the RESET_CFG register                         */
/*			IOXP_PWM_CFG_PWM_EN					(0x4201)	- PWM_EN bit of the PWM_CFG register                               */
/*			IOXP_PWM_CFG_PWM_MODE				(0x4202)	- PWM_MODE bit of the PWM_CFG register                             */
/*			IOXP_PWM_CFG_PWM_IN_AND				(0x4204)	- PWM_IN_AND bit of the PWM_CFG register                           */
/*			IOXP_CLOCK_DIV_CFG_CLK_INV			(0X4340)	- CLK_INV bit of the CLOCK_DIV_CFG register                        */
/*			IOXP_CLOCK_DIV_CFG_CLK_DIV_EN		(0X4301)	- DIV_EN bit of the CLOCK_DIV_CFG register                         */
/*			IOXP_LOGIC_1_LA1_INV				(0x4408)	- LA1_INV bit of the LOGIC_1 register                              */
/*			IOXP_LOGIC_1_LB1_INV				(0x4410)	- LB1_INV bit of the LOGIC_1 register                              */
/*			IOXP_LOGIC_1_LC1_INV				(0x4420)	- LC1_INV bit of the LOGIC_1 register                              */
/*			IOXP_LOGIC_1_LY1_INV				(0x4440)	- LY1_INV bit of the LOGIC_1 register                              */
/*			IOXP_LOGIC_2_LA2_INV				(0x4508)	- LA2_INV bit of the LOGIC_2 register                              */
/*			IOXP_LOGIC_2_LB2_INV				(0x4510)	- LB2_INV bit of the LOGIC_2 register                              */
/*			IOXP_LOGIC_2_LC2_INV				(0x4520)	- LC2_INV bit of the LOGIC_2 register                              */
/*			IOXP_LOGIC_2_LY2_INV				(0x4540)	- LY2_INV bit of the LOGIC_2 register                              */
/*			IOXP_LOGIC_2_LY1_CASCADE			(0x4540)	- LY1_CASCADE bit of the LOGIC_2 register                          */
/*			IOXP_LOGIC_FF_CFG_FF1_CLR			(0x4601)	- FF1_CLR bit of the LOGIC_FF_CFG register                         */
/*			IOXP_LOGIC_FF_CFG_FF1_SET			(0x4602)	- FF1_SET bit of the LOGIC_FF_CFG register                         */
/*			IOXP_LOGIC_FF_CFG_FF2_CLR			(0x4604)	- FF2_CLR bit of the LOGIC_FF_CFG register                         */
/*			IOXP_LOGIC_FF_CFG_FF2_SET			(0x4608)	- FF2_SET bit of the LOGIC_FF_CFG register                         */
/*			IOXP_LOGIC_INT_EVENT_EN_LOGIC1_INT_LEVEL	(0x4701)	- LOGIC1_INT_LEVEL bit of the LOGIC_INT_EVENT_EN register  */
/*			IOXP_LOGIC_INT_EVENT_EN_LOGIC1_EVENT_EN		(0x4702)	- LOGIC1_EVENT_EN bit of the LOGIC_INT_EVENT_EN register   */
/*			IOXP_LOGIC_INT_EVENT_EN_LY1_DBNC_DIS		(0x4704)	- LY1_DBNC_DIS bit of the LOGIC_INT_EVENT_EN register      */
/*			IOXP_LOGIC_INT_EVENT_EN_LOGIC2_INT_LEVEL	(0x4708)	- LOGIC2_INT_LEVEL bit of the LOGIC_INT_EVENT_EN register  */
/*			IOXP_LOGIC_INT_EVENT_EN_LOGIC2_EVENT_EN		(0x4710)	- LOGIC2_EVENT_EN bit of the LOGIC_INT_EVENT_EN register   */
/*			IOXP_LOGIC_INT_EVENT_EN_LY2_DBNC_DIS		(0x4720)	- LY2_DBNC_DIS bit of the LOGIC_INT_EVENT_EN register      */
/*			IOXP_PIN_CONFIG_D_R0_EXTEND  		(0x4C01)	- R0_EXTEND bit of the PIN_CONFIG_D register                       */
/*			IOXP_PIN_CONFIG_D_C9_EXTEND  		(0x4C02)	- C9_EXTEND bit of the PIN_CONFIG_D register                       */
/*			IOXP_PIN_CONFIG_D_R3_EXTEND_0	  	(0x4C04)	- R3_EXTEND_0 bit of the PIN_CONFIG_D register                     */
/*			IOXP_PIN_CONFIG_D_R3_EXTEND_1	  	(0x4C08)	- R3_EXTEND_1 bit of the PIN_CONFIG_D register                     */
/*			IOXP_PIN_CONFIG_D_C6_EXTEND	  		(0x4C10)	- C6_EXTEND bit of the PIN_CONFIG_D register                       */
/*			IOXP_PIN_CONFIG_D_R4_EXTEND	  		(0x4C20)	- R4_EXTEND bit of the PIN_CONFIG_D register                       */
/*			IOXP_PIN_CONFIG_D_C4_EXTEND	  		(0x4C40)	- C4_EXTEND bit of the PIN_CONFIG_D register                       */
/*			IOXP_PIN_CONFIG_D_PULL_SELECT  		(0x4C80)	- PULL_SELECT bit of the PIN_CONFIG_D register                     */
/*			IOXP_GENERAL_CFG_B_RST_CFG   		(0x4D01)	- RST_CFG bit of the GENERAL_CFG_B register                        */
/*			IOXP_GENERAL_CFG_B_INT_CFG     		(0x4D02)	- INT_CFG bit of the GENERAL_CFG_B register                        */
/*			IOXP_GENERAL_CFG_B_LCK_TRK_GPI   	(0x4D08)	- RST_CFG bit of the GENERAL_CFG_B register                        */
/*			IOXP_GENERAL_CFG_B_LCK_TRK_LOGIC  	(0x4D10)	- TRK_GPI bit of the GENERAL_CFG_B register                        */
/*			IOXP_GENERAL_CFG_B_CORE_FREQ0  		(0x4D20)	- CORE_FREQ0 bit of the GENERAL_CFG_B register                     */
/*			IOXP_GENERAL_CFG_B_CORE_FREQ1  		(0x4D40)	- CORE_FREQ1 bit of the GENERAL_CFG_B register                     */
/*			IOXP_GENERAL_CFG_B_OSC_EN  			(0x4D80)	- OSC_EN bit of the GENERAL_CFG_B register                         */
/*			IOXP_INT_EN_EVENT_IEN   	  		(0x4E01)	- EVENT_IEN bit of the INT_EN register                             */
/*			IOXP_INT_EN_GPI_IEN     			(0x4E02)	- GPI_IEN bit of the INT_EN register                               */
/*			IOXP_INT_EN_OVERFLOW_IEN 		 	(0x4E04)	- OVERFLOW_IEN bit of the INT_EN register                          */
/*			IOXP_INT_EN_LOCK_IEN   			  	(0x4E08)	- LOCK_IEN bit of the INT_EN register                              */
/*			IOXP_INT_EN_LOGIC1_INT  			(0x4E10)	- LOGIC1_IEN bit of the INT_EN register                            */
/*			IOXP_INT_EN_LOGIC2_INT  			(0x4E20)	- LOGIC2_IEN bit of the INT_EN register                            */
/*			IOXP_GPI_EVENT_EN_GPI_EVENT_EN(x)	((((0x21 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPI_EVENT_EN bit of the GPI_EVENT_EN registers corresponding to GPIO no x (x between 1 and 19)   */
/*          IOXP_GPI_INT_STAT_GPI_INT(x)		((((0x13 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPI_INT bit of the GPI_INT_STAT registers corresponding to GPIO no x  (x between 1 and 19)       */
/*          IOXP_GPI_STATUS_GPI_STAT(x)		 	((((0x16 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPI_STAT bit of the GPI_STATUS registers corresponding to GPIO no x  (x between 1 and 19)        */
/*          IOXP_INT_LEVEL_GPI_INT_LEVEL(x)		((((0x38 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPI_INT_LEVEL bit of GPI_INT_LEVEL registers corresponding to GPIO no x  (x between 1 and 19)    */
/*          IOXP_GPO_DATA_OUT_GPO_DATA(x)		((((0x2A + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPO_DATA bit of the GPO_DATA_OUT registers corresponding to GPIO no x  (x between 1 and 19)      */
/*          IOXP_GPO_OUT_MODE_GPO_OUT_MODE(x)	((((0x2D + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPO_OUT_MODE bit of the GPO_OUT_MODE registers corresponding to GPIO no x (x between 1 and 19)   */
/*          IOXP_GPIO_DIRECTION_GPIO_DIR(x)		((((0x30 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	                       */
/*							- GPIO_DIR bit of the GPIO_DIRECTION registers corresponding to GPIO no x (x between 1 and 19      */
/*                                                                                                                             */
/*		                                                                                                                       */
/*  Return Value:                                                                                                              */
/*		 uint8_t - the bit value (0 or 1)                                                                                      */                        
/*                                                                                                                             */
/*	Errors:                                                                                                                    */
/*                                                                      													   */
/*	Description:                                                    														   */
/*		This function returns the value of the specified bit.The bit definition is made of two bytes: high byte is the         */
/*		register address, low byte is the mask corresponding to the specified bit. Apart from pre-defined values listed above, */
/*		user may use this function to access any register bit, by providing its bit definition.                                */ 
/* --------------------------------------------------------------------------------------------------------------------------- */



uint8_t IOXP::GetRegisterBit(uint16_t wBitDef)
{
	uint8_t bRegisterAddr = *((uint8_t *)&wBitDef + 1);
	uint8_t bMask = *(uint8_t *)&wBitDef;
	return ReadMaskedRegisterValue(bRegisterAddr, bMask) != 0;
}

/* --------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetRegisterBitsGroup                                                                                                 */
/*                                                                                                                             */
/*	Synopsis:                                                                                                                  */
/*		GetRegisterBit(uint16_t wBitDef);                                                                                      */
/*	Parameters:                                                                                                                */  
/*	 	uint16_t wBitDef - the definition of the bit to be written.This                                                        */
/*						   definition consists of two bytes: high byte                                                         */
/*						   is the register address, low byte is the                                                            */
/*						   mask corresponding to the specified bit.                                                            */
/*		                                                                                                                       */
/*				It can be one of the following:                                                                                */
/*	IOXP_ID_MAN_ID  					(0x00F0)	- MAN_ID[3:0] field of the ID register                                     */
/*			IOXP_ID_REV_ID  					(0x000F)	- REV_ID[3:0] field of the ID register                             */
/*			IOXP_INT_STATUS_ALL   				(0x01FF)	- all bits [7:0] of INT_STATUS register                            */
/*			IOXP_STATUS_EC  					(0x020F)	- EC[4:0] field of the Status register                             */
/*			IOXP_UNLOCK1_UNLOCK1  				(0x333F)	- UNLOCK1[6:0] field of the UNLOCK1 register                       */
/*			IOXP_UNLOCK2_UNLOCK2  				(0x343F)	- UNLOCK2[6:0] field of the UNLOCK2 register                       */
/*			IOXP_EXT_LOCK_EVENT_EXT_LOCK_EVENT	(0x353F)	- EXT_LOCK_EVENT[6:0] field of the EXT_LOCK_EVENT register         */				
/*			IOXP_UNLOCK_TIMERS_INT_MASK_TIMER  	(0x36F0)	- INT_MASK_TIMER[4:0] field of the UNLOCK_TIMERS register          */
/*			IOXP_UNLOCK_TIMERS_UNLOCK_TIMER  	(0x360F)	- UNLOCK_TIMER[2:0] field of the UNLOCK_TIMERS register            */
/*			IOXP_RESET1_EVENT_A_RESET1_EVENT_A  (0x383F)	- RESET1_EVENT_A[6:0] field of the RESET1_EVENT_A register         */
/*			IOXP_RESET1_EVENT_B_RESET1_EVENT_B  (0x393F)	- RESET1_EVENT_B[6:0] field of the RESET1_EVENT_B register         */
/*			IOXP_RESET1_EVENT_C_RESET1_EVENT_C  (0x3A3F)	- RESET1_EVENT_C[6:0] field of the RESET1_EVENT_C register         */
/*			IOXP_RESET2_EVENT_A_RESET2_EVENT_A  (0x3B3F)	- RESET2_EVENT_A[6:0] field of the RESET2_EVENT_A register         */
/*			IOXP_RESET2_EVENT_B_RESET2_EVENT_B  (0x3C3F)	- RESET2_EVENT_B[6:0] field of the RESET2_EVENT_B register	       */				
/*			IOXP_RESET_CFG_RESET_PULSE_WIDTH	(0x3D03)	- RESET_PULSE_WIDTH[1:0] field of the RESET_PULSE_WIDTH register   */
/*			IOXP_RESET_CFG_RESET_TRIGGER_TIME	(0x3D1C)	- TRIGGER_TIME[2:0] field of the RESET_PULSE_WIDTH register        */
/*			IOXP_CLOCK_DIV_CFG_CLK_DIV			(0X433E)	- CFG_CLK_DIV[4:0] field of the CLOCK_DIV register                 */
/*			IOXP_LOGIC_1_CFG_LOGIC1_SEL			(0x4407)	- LOGIC1_SEL[1:0] field of the LOGIC_1_CFG register                */
/*			IOXP_LOGIC_2_CFG_LOGIC2_SEL			(0x4507)	- LOGIC2_SEL[2:0] field of the LOGIC_2_CFG register                */
/*			IOXP_POLL_TIME_CFG_KEY_POLL_TIME	(0x4803)	- KEY_POLL_TIME[1:0] field of the POLL_TIME_CFG register           */
/*			IOXP_PIN_CONFIG_D_R3_EXTEND		  	(0x4C0C)	- R3_EXTEND[1:0] field of the PIN_CONFIG_D register                */
/*			IOXP_GENERAL_CFG_B_CORE_FREQ  		(0x4D60)	- CORE_FREQ[1:0] field of the GENERAL_CFG_B register               */
/*			IOXP_RPULL_CONFIG_R_PULL_CFG(x)		((((0x19 + div4(x))) << 8) | (3 << (mod4(x)<<1)))	                           */
/*							- R_PULL_CFG field of the RPULL_CONFIG registers corresponding to Row x (x between 0 and 7)        */
/*			IOXP_RPULL_CONFIG_C_PULL_CFG(x)		((((0x1B + div4(x))) << 8) | (3 << (mod4(x)<<1)))	                           */
/*							- C_PULL_CFG field of the RPULL_CONFIG registers corresponding to Column x (x between 0 and 10)    */
/*	                                                                                                                           */
/* 		uint8_t bBitsVal		- the value to be written to the specified bits group.                                         */
/*		                                                                                                                       */
/*  Return Value:                                                                                                              */
/*		 void                                                                                                                  */                        
/*                                                                                                                             */
/*	Errors:                                                                                                                    */
/*                                                                      													   */
/*	Description:                                                    														   */
/*		This function sets the value of the specified group of bits to the specified value.                                    */
/*		A group of bits is a register section that is made of more than one bit, grouped according to their function.          */
/*		The value is right justified, considering the LSb from the group of bits as being on position 0, regardless of the     */
/*		position of the group of bits in the register. The group of bits definition is made of two bytes: high byte is the     */
/*		register address, low byte is the mask corresponding to the specified group of bits. Apart from the pre-defined values */
/*		listed above, the user may use this function to access any register group of bits, by providing its specification.     */ 
/* --------------------------------------------------------------------------------------------------------------------------- */

void IOXP::SetRegisterBitsGroup(uint16_t wBitDef, uint8_t bBitsVal)
{
	uint8_t bRegisterAddr = *((uint8_t *)&wBitDef + 1);
	uint8_t bMask = *(uint8_t *)&wBitDef;
	uint8_t bScale = Mask2Scale(bMask);
	uint8_t bScaledGroupVal = bBitsVal << bScale;
	WriteMaskedRegisterValue(bRegisterAddr, bMask, bScaledGroupVal);
}

/* --------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetRegisterBitsGroup                                                                                                 */
/*                                                                                                                             */
/*	Synopsis:                                                                                                                  */
/*		GetRegisterBitsGroup(uint16_t wBitDef);                                                                                */
/*	Parameters:                                                                                                                */  
/*	 	uint16_t wBitDef - the definition of the bit to be written.This                                                        */
/*						   definition consists of two bytes: high byte                                                         */
/*						   is the register address, low byte is the                                                            */
/*						   mask corresponding to the specified bit.                                                            */
/*		                                                                                                                       */
/*				It can be one of the following:                                                                                */
/*			IOXP_ID_MAN_ID  					(0x00F0)	- MAN_ID[3:0] field of the ID register                             */
/*			IOXP_ID_REV_ID  					(0x000F)	- REV_ID[3:0] field of the ID register                             */
/*			IOXP_INT_STATUS_ALL   				(0x01FF)	- all bits [7:0] of INT_STATUS register                            */
/*			IOXP_STATUS_EC  					(0x020F)	- EC[4:0] field of the Status register                             */
/*			IOXP_UNLOCK1_UNLOCK1  				(0x333F)	- UNLOCK1[6:0] field of the UNLOCK1 register                       */
/*			IOXP_UNLOCK2_UNLOCK2  				(0x343F)	- UNLOCK2[6:0] field of the UNLOCK2 register                       */
/*			IOXP_EXT_LOCK_EVENT_EXT_LOCK_EVENT	(0x353F)	- EXT_LOCK_EVENT[6:0] field of the EXT_LOCK_EVENT register         */				
/*			IOXP_UNLOCK_TIMERS_INT_MASK_TIMER  	(0x36F0)	- INT_MASK_TIMER[4:0] field of the UNLOCK_TIMERS register          */
/*			IOXP_UNLOCK_TIMERS_UNLOCK_TIMER  	(0x360F)	- UNLOCK_TIMER[2:0] field of the UNLOCK_TIMERS register            */
/*			IOXP_RESET1_EVENT_A_RESET1_EVENT_A  (0x383F)	- RESET1_EVENT_A[6:0] field of the RESET1_EVENT_A register         */
/*			IOXP_RESET1_EVENT_B_RESET1_EVENT_B  (0x393F)	- RESET1_EVENT_B[6:0] field of the RESET1_EVENT_B register         */
/*			IOXP_RESET1_EVENT_C_RESET1_EVENT_C  (0x3A3F)	- RESET1_EVENT_C[6:0] field of the RESET1_EVENT_C register         */
/*			IOXP_RESET2_EVENT_A_RESET2_EVENT_A  (0x3B3F)	- RESET2_EVENT_A[6:0] field of the RESET2_EVENT_A register         */
/*			IOXP_RESET2_EVENT_B_RESET2_EVENT_B  (0x3C3F)	- RESET2_EVENT_B[6:0] field of the RESET2_EVENT_B register	       */				
/*			IOXP_RESET_CFG_RESET_PULSE_WIDTH	(0x3D03)	- RESET_PULSE_WIDTH[1:0] field of the RESET_PULSE_WIDTH register   */
/*			IOXP_RESET_CFG_RESET_TRIGGER_TIME	(0x3D1C)	- TRIGGER_TIME[2:0] field of the RESET_PULSE_WIDTH register        */
/*			IOXP_CLOCK_DIV_CFG_CLK_DIV			(0X433E)	- CFG_CLK_DIV[4:0] field of the CLOCK_DIV register                 */
/*			IOXP_LOGIC_1_CFG_LOGIC1_SEL			(0x4407)	- LOGIC1_SEL[1:0] field of the LOGIC_1_CFG register	               */
/*			IOXP_LOGIC_2_CFG_LOGIC2_SEL			(0x4507)	- LOGIC2_SEL[2:0] field of the LOGIC_2_CFG register                */
/*			IOXP_POLL_TIME_CFG_KEY_POLL_TIME	(0x4803)	- KEY_POLL_TIME[1:0] field of the POLL_TIME_CFG register           */
/*			IOXP_PIN_CONFIG_D_R3_EXTEND		  	(0x4C0C)	- R3_EXTEND[1:0] field of the PIN_CONFIG_D register                */
/*			IOXP_GENERAL_CFG_B_CORE_FREQ  		(0x4D60)	- CORE_FREQ[1:0] field of the GENERAL_CFG_B register               */
/*			IOXP_RPULL_CONFIG_R_PULL_CFG(x)		((((0x19 + div4(x))) << 8) | (3 << (mod4(x)<<1)))	                           */
/*								- R_PULL_CFG field of the RPULL_CONFIG registers corresponding to Row x (x between 0 and 7)    */
/*			IOXP_RPULL_CONFIG_C_PULL_CFG(x)		((((0x1B + div4(x))) << 8) | (3 << (mod4(x)<<1)))	                           */
/*								- C_PULL_CFG field of the RPULL_CONFIG registers corresponding to Column x(x between 0 and 10) */
/*                                                                                                                             */
/*  Return Value:                                                                                                              */
/*		    uint8_t - the value of the specified bits group                                                                    */                        
/*                                                                                                                             */
/*	Errors:                                                                                                                    */
/*                                                                      													   */
/*	Description:                                                    														   */
/*		This function gets the value of the specified bits group. A group of bits is a register section that is made of more   */
/*		than one bit, grouped according to their function. The value is right justified, considering the LSb from the group    */
/*		of bits as being on position 0, regardless of the position of the group of bits in the register. The group of bits     */
/*		definition is made of two bytes: high byte is the register address, low byte is the mask corresponding to the          */
/*		specified group of bits. Apart from the pre-defined values listed above, the user may use this function to access any  */
/*		 register group of bits, by providing its specification.                                                               */ 
/* --------------------------------------------------------------------------------------------------------------------------- */

uint8_t IOXP::GetRegisterBitsGroup(uint16_t wBitDef)
{
	uint8_t bRegisterAddr = *((uint8_t *)&wBitDef + 1);
	uint8_t bMask = *(uint8_t *)&wBitDef;
	uint8_t bScale = Mask2Scale(bMask);
	uint8_t bGroupVal = ReadMaskedRegisterValue(bRegisterAddr, bMask) >> bScale;
	return bGroupVal;
}

/* -------------------------------------------------------------------- */
/*	IOXP::SetRegister                                                   */
/*                                                                      */
/*	Synopsis:                                                           */
/*		SetRegister(uint8_t bAddress, uint8_t bVal);                    */
/*	Parameters:                                                         */  
/*		uint8_t bAddress - the register address. The same definitions   */
/*						   for the addresses as the name of the         */
/*						   register (capital letters) can be used,      */
/*						   prefixed with IOXP_ADDR                      */
/*						   (ex. IOXP_ADR_INT_STATUS, for INT_STATUS     */
/*						   register)                                    */
/*		uint8_t bVal - the value to be written to the register.         */
/*                                                                      */
/*  Return Value:                                                       */
/*		 void                                                           */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*		This function writes the specified value to the specified       */
/*		register.                                                       */ 
/* -------------------------------------------------------------------- */

void IOXP::SetRegister(uint8_t bAddress, uint8_t bVal)
{
	WriteBytesI2C(bAddress, 1, &bVal);
}

/* -------------------------------------------------------------------- */
/*	IOXP::GetRegister                                                   */
/*                                                                      */
/*	Synopsis:                                                           */
/*		GetRegister(uint8_t bAddress);                                  */
/*	Parameters:                                                         */  
/*		uint8_t bAddress - the register address. The same definitions   */
/*						   for the addresses as the name of the         */
/*						   register (capital letters) can be used,      */
/*						   prefixed with IOXP_ADDR                      */
/*						   (ex. IOXP_ADR_INT_STATUS,for INT_STATUS      */
/*						   register)                                    */
/*                                                                      */
/*  Return Value:                                                       */
/*		uint8_t - the value read from the register.         	        */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*		This function returns the value read from the specified         */
/*		register.                                                       */ 
/* -------------------------------------------------------------------- */

uint8_t IOXP::GetRegister(uint8_t bAddress)
{
	uint8_t bVal;
	ReadBytesI2C(bAddress, 1, &bVal);
	return bVal;
}

/* -------------------------------------------------------------------- */
/*	IOXP::SetKeyboardPinConfig                                          */
/*                                                                      */
/*	Synopsis:                                                           */
/*		SetKeyboardPinConfig(uint8_t bRowCfg, uint16_t wColCfg);        */
/*	Parameters:                                                         */  
/*	 	uint8_t bRowCfg		  											*/
/*		- one byte containing keyboard rows configuration               */
/*				(Rows 0-7), as a bitmap:                                */
/*			- if bit x from the byte is set, then the pin corresponding */
/*				to row x is used for keyboard.                          */
/*		- if bit x from the byte is not set, then the pin               */
/*				corresponding to row x is not used for keyboard         */
/*				(used as GPIO).                                         */
/*		uint16_t wColCfg	                                            */
/*			- two bytes containing keyboard columns configuration       */
/*				(Columns 0-10), as a bitmap:                            */
/*     		-if bit x of one of the two bytes is set, then the pin      */
/*				corresponding to column x is used for keyboard.         */
/*			- if bit x is not set, then pin corresponding to column     */
/*					x is not used for keyboard (used as GPIO).          */                         
/*		                                                                */
/*  Return Value:                                                       */
/*		void                                                            */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*		This function configures the IO pins to be used for keyboard.   */
/*		The input parameters describe rows and columns configuration,   */
/*		as bitmaps containing 1 value for rows / columns used by the    */
/*		keyboard                                                        */
/* -------------------------------------------------------------------- */

void IOXP::SetKeyboardPinConfig(uint8_t bRowCfg, uint16_t wColCfg)
{
	uint8_t rgbVals[3];
	rgbVals[0] = bRowCfg;	// rows 0 - 7
	rgbVals[1] = uint8_t(wColCfg & 0x0F);	// cols 0 - 7
	rgbVals[2] = uint8_t((wColCfg & 0x700) >> 8);	// cols 8 - 10
	WriteBytesI2C(IOXP_ADDR_PIN_CONFIG_A, 3, rgbVals);
}

/* -------------------------------------------------------------------- */
/*	IOXP::ConfigureInterrupt                                            */
/*                                                                      */
/*	Synopsis:                                                           */
/*		ConfigureInterrupt(uint8_t bParExtIntNo, uint16_t wEventMask,   */
/*		void (*pfIntHandler)());                                        */
/*	Parameters:                                                         */  
/*		uint8_t bParExtIntNo	- The parameter indicating the external */
/*								  interrupt number  where the INT pin   */
/*								  is physically connected. Can be one   */
/*								  of the parameters from the following  */
/*								  list:                                 */
/*					PAR_EXT_INT0 0 - External interrupt 0               */
/*				PAR_EXT_INT1 1 - External interrupt 1                   */
/*					PAR_EXT_INT2 2 - External interrupt 2               */
/*					PAR_EXT_INT3 3 - External interrupt 3               */
/*					PAR_EXT_INT4 4 - External interrupt 4               */
/*                                                                      */
/*		uint8_t bEventMask	- the events that are enabled to trigger    */
/*						      the interrupt. It can be one or more      */
/*							  (OR-ed) parameters from the following     */
/*							  list:                                     */
/*					IOXP_INT_EN_EVENT_IEN   	  		(0x4E01)	    */
/*							- events can trigger interrupt              */
/*					IOXP_INT_EN_GPI_IEN     			(0x4E02)     	*/
/*							- GPI interrupt enabled                     */
/*					IOXP_INT_EN_OVERFLOW_IEN 		 	(0x4E04)        */
/*							- overflow condition can trigger interrupt  */
/*					IOXP_INT_EN_LOCK_IEN   			  	(0x4E08)	    */
/*							- lock and unlock can trigger interrupt     */
/*					IOXP_INT_EN_LOGIC1_INT  			(0x4E10)	    */
/*							- Logic 1 can trigger interrupt             */
/*					IOXP_INT_EN_LOGIC2_INT  			(0x4E20)	    */
/*							- Logic 2 can trigger interrupt             */
/*                                                                      */
/*		void (*pfIntHandler)() - pointer to a function that             */
/*								will serve as interrupt  handler.       */
/*										                                */
/*                                                                      */
/*  Return Value:                                                       */
/*		void                                                            */
/*                                                                      */
/*	Errors:                                                             */
/*                                                                      */
/*	Description:                                                        */
/*	The function configures the interrupt by                            */
/*			- associating it to a set of events                         */
/*			- associating it to an external interrupt number (0-4)      */
/*			- associating it to an interrupt handler                    */
/*			Make sure that the interrupt pin of the PmodIOXP is         */
/*			physically connected to the external interrupt pin          */
/*			number corresponding to bParExtIntNo parameter.             */
/* -------------------------------------------------------------------- */

void IOXP::ConfigureInterrupt(uint8_t bParExtIntNo, uint16_t wEventMask, void (*pfIntHandler)())
{
	int iDummyKeyVal;
	
	attachInterrupt(bParExtIntNo, pfIntHandler, FALLING);

	uint8_t bVal = (uint8_t)(wEventMask & 0xFF);
	WriteBytesI2C(IOXP_ADDR_INT_EN, 1, &bVal);
}

/* --------------------------------------------------------------------------------------------------------------------------- */
/*	 IOXP::ReadFIFO                                                                                                            */
/*                                                                                                                             */
/*	Synopsis:                                                                                                                  */
/*		ReadFIFO(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);            */
/*	Parameters:                                                                                                                */  
/*	 	int &iKeyVal	- key value - output parameter where the key value will be stored.                                     */
/*						It contains -1 value if the event is not a keyboard event or if the key map table contains -1 value.   */
/*		uint8_t &bRow	- key row - output parameter where the keyboard row value will be stored.                              */
/*						It contains 0xFF value if the event is not a keyboard event.                                           */
/*		uint8_t &bCol	- key column - output parameter where the keyboard column value will be stored.                        */ 
/*						It contains 0xFF value if the event is not a keyboard event or if the event corresponds to (Rx, GND)   */
/*		uint8_t &bGPI	- GPI no - output parameter where the GPI value will be stored.                                        */
/*						It contains 0xFF value if the event is not a GPI event.                                                */
/*		uint8_t &bLogic	- Logic no - output parameter where the Logic value will be stored.                                    */ 
/*						It contains 0xFF value if the event is not a Logic event.                                              */
/*		uint8_t &bEventState	- Event state - output parameter where the state of the event will be placed.                  */
/*						For keys events:                                                                                       */
/*								1 - key is pressed                                                                             */
/*								0 - key is released                                                                            */
/*						For GPI and Logic events:                                                                              */
/*								1 - GPI/logic is active                                                                        */
/*								0 - GPI/logic is inactive                                                                      */
/*				                                                                                                               */
/*  Return Value:                                                                                                              */
/*		void                                                                                                                   */
/*                                                                                                                             */
/*	Errors:                                                                                                                    */
/*                                                                                                                             */
/*	Description:                                                                                                               */
/*		   This function reads the last event on FIFO, decodes it and returns the specific information.                        */
/*              If the event is a key event, key value, row and column the information corresponding to the key are stored     */
/*			in output parameters while the other parameters corresponding to GPI events (bGPI) and Logic events (bLogic)       */
/*				are set to 0xFF. If the event is a GPI event, the GPI number is stored in bGPI while the other parameters      */
/*			corresponding to key events (keyVal, bRow, bCol) and to Logic events (bLogic) are set to -1 (keyVal) and 0xFF      */
/*				respectively. If the event is a Logic event, the Logic number is stored in bLogic, while the parameters        */
/*			corresponding to key events (keyVal, bRow, bCol) and to Key events are set to -1 (keyVal) and 0xFF respectively.   */ 
/* --------------------------------------------------------------------------------------------------------------------------- */

void IOXP::ReadFIFO(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState)
{
	uint8_t bEvent;
	ReadBytesI2C(IOXP_ADDR_FIFO1, 1, &bEvent);
	DecodeEvent(bEvent, iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
}

/* ------------------------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetLockEvent                                                                                                                               */
/*                                                                                                                                                   */
/*	Synopsis:                                                                                                                                        */
/*		SetLockEvent(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState);                                    */
/*	Parameters:                                                                                                                                      */  
/*		int iKeyVal	- key value                                                                                                                      */
/*						It contains -1 value if the event is not a keyboard event or if the key is specified by row and column information.          */
/*	    uint8_t bRow	- key row value                                                                                                              */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event.                             */
/*						This parameter has a default value of 0xFF.                                                                                  */
/*		uint8_t bCol	- key column value                                                                                                           */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event or if the event corresponds  */
/*						to (Rx, GND). This parameter has a default value of 0xFF.                                                                    */
/*		uint8_t bGPI	- GPI number.                                                                                                                */
/*						It contains 0xFF value if the event is not a GPI event.                                                                      */
/*						This parameter has a default value of 0xFF.                                                                                  */
/*		uint8_t bLogic	- Logic number.                                                                                                              */
/*						It contains 0xFF value if the event is not a Logic event.                                                                    */
/*						This parameter has a default value of 0xFF.                                                                                  */
/*		uint8_t bEventState - Event state                                                                                                            */
/*						For keys events:                                                                                                             */
/*								1 - key is pressed                                                                                                   */
/*								0 - key is released                                                                                                  */
/*						For GPI and Logic events:                                                                                                    */
/*								1 - GPI/logic is active                                                                                              */
/*								0 - GPI/logic is inactive                                                                                            */
/*						This parameter has a default value of 1. 	                                                                                 */
/*		                                                                                                                                             */
/*  Return Value:                                                                                                                                    */
/*		 void                                                                                                                                        */
/*                                                                                                                                                   */
/*	Errors:                                                                                                                                          */
/*                                                                                                                                                   */
/*	Description:                                                                                                                                     */
/*		This function sets the lock event according to the specified information.It encodes information in the event byte using EncodeEvent function.*/
/*		When more keys have that same value associated, then only the first one of them will be identified with that value.                          */
/*		If no key corresponds to the specified key value, no event will be created.                                                                  */
/*		The last five parameters have default values.                                                                                                */ 
/* ------------------------------------------------------------------------------------------------------------------------------------------------- */

void IOXP::SetLockEvent(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState)
{
	uint8_t bEvent = EncodeEvent(iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
	if(bEvent != 0xFF)
	{
		WriteBytesI2C(IOXP_ADDR_EXT_LOCK_EVENT, 1, &bEvent);	
	}
}

/* ------------------------------------------------------------------------------------------------------------------------------ */
/*	 IOXP::GetLockEvent                                                                                                           */
/*                                                                                                                                */
/*	Synopsis:                                                                                                                     */
/*		GetLockEvent(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);	          */
/*	Parameters:                                                                                                                   */  
/*		int &iKeyVal	- key value - output parameter where the key value will be stored.                                        */
/*						It contains -1 value if the event is not a keyboard event or if the key map table contains -1 value.      */
/*		uint8_t &bRow	- key row - output parameter where the keyboard row value will be stored.                                 */
/*						It contains 0xFF value if the event is not a keyboard event.                                              */
/*		uint8_t &bCol	- key column - output parameter where the keyboard column value will be stored.                           */
/*						It contains 0xFF value if the event is not a keyboard event or if the event corresponds to (Rx, GND)      */
/*		uint8_t &bGPI	- GPI no - output parameter where the GPI value will be stored.                                           */
/*						It contains 0xFF value if the event is not a GPI event.                                                   */
/*		uint8_t &bLogic	- Logic no - output parameter where the Logic value will be stored.                                       */
/*						It contains 0xFF value if the event is not a Logic event.                                                 */
/*		uint8_t &bEventState	- Event state - output parameter where the state of the event will be placed.                     */
/*						For keys events:                                                                                          */
/*								1 - key is pressed                                                                                */
/*								0 - key is released                                                                               */
/*						For GPI and Logic events:                                                                                 */
/*								1 - GPI/logic is active                                                                           */
/*								0 - GPI/logic is inactive                                                                         */
/*  Return Value:                                                                                                                 */
/*		void                                                                                                                      */
/*                                                                                                                                */
/*	Errors:                                                                                                                       */
/*                                                                                                                                */
/*	Description:                                                                                                                  */
/*		This function reads the lock event, decodes it and returns the specific information.  If the event is a key event,        */
/*      key value, row and column the information corresponding to the key are stored in output parameters                        */
/*		while the other parameters corresponding to GPI events (bGPI) and Logic events (bLogic) are set to 0xFF.                  */
/*      If the event is a GPI event, the GPI number is stored in bGPI while the other parameters corresponding to                 */
/*		key events (keyVal, bRow, bCol) and to Logic events (bLogic) are set to -1 (keyVal) and 0xFF respectively.                */
/*      If the event is a Logic event, the Logic number is stored in bLogic, while the parameters corresponding to                */
/*		key events (keyVal, bRow, bCol) and to Key events are set to -1 (keyVal) and 0xFF respectively.	                          */                                                 
/* ------------------------------------------------------------------------------------------------------------------------------ */

void IOXP::GetLockEvent(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState)
{
	uint8_t bEvent;
	ReadBytesI2C(IOXP_ADDR_EXT_LOCK_EVENT, 1, &bEvent);
	DecodeEvent(bEvent, iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
}

/* --------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetUnlock1Event                                                                                                      */
/*  	                                                                                                                       */
/*	Synopsis:                                                                                                                  */
/*		SetUnlock1Event(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState);           */
/*	Parameters:                                                                                                                */  
/*		int iKeyVal	- key value                                                                                                */
/*						It contains -1 value if the event is not a keyboard event or if the key is specified by row and        */
/*						column information.                                                                                    */
/*		uint8_t bRow	- key row value                                                                                        */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event.       */
/*						This parameter has a default value of 0xFF.                                                            */
/*		uint8_t bCol	- key column value                                                                                     */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event        */
/*						or if the event corresponds to (Rx, GND).                                                              */
/*						This parameter has a default value of 0xFF.                                                            */
/*		uint8_t bGPI	- GPI number.                                                                                          */
/*						It contains 0xFF value if the event is not a GPI event.                                                */
/*						This parameter has a default value of 0xFF.                                                            */
/*		uint8_t bLogic	- Logic number.                                                                                        */
/*						It contains 0xFF value if the event is not a Logic event.                                              */
/*						This parameter has a default value of 0xFF.                                                            */
/*		uint8_t bEventState - Event state                                                                                      */
/*						For keys events:                                                                                       */
/*								1 - key is pressed                                                                             */
/*								0 - key is released                                                                            */
/*						For GPI and Logic events:                                                                              */
/*								1 - GPI/logic is active                                                                        */
/*								0 - GPI/logic is inactive                                                                      */
/*  Return Value:                                                                                                              */
/*		void                                                                                                                   */
/*                                                                                                                             */
/*	Errors:                                                                                                                    */
/*                                                                                                                             */
/*	Description:                                                                                                               */
/*		This function sets the Unlock1 event according to the specified information. It encodes information in the event byte  */
/*		using EncodeEvent function.	When more keys have that same value associated, then only the first one of them will be    */
/*		identified with that value. If no key corresponds to the specified key value, no event will be created.                */
/*		The last five parameters have default values.	                                                                       */ 
/* --------------------------------------------------------------------------------------------------------------------------- */


void IOXP::SetUnlock1Event(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState)
{
	uint8_t bEvent = EncodeEvent(iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
	WriteBytesI2C(IOXP_ADDR_UNLOCK1, 1, &bEvent);	
}

/* --------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetUnlock1Event                                                                                                      */
/*                                                                                                                             */
/*	Synopsis:                                                                                                                  */
/*		GetUnlock1Event(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);	   */
/*	Parameters:                                                                                                                */  
/*	 	int &iKeyVal	- key value - output parameter where the key value will be stored.                                     */
/*						It contains -1 value if the event is not a keyboard event or if the key map table contains -1 value.   */
/*		uint8_t &bRow	- key row - output parameter where the keyboard row value will be stored.                              */
/*						It contains 0xFF value if the event is not a keyboard event.                                           */
/*		uint8_t &bCol	- key column - output parameter where the keyboard column value will be stored.                        */
/*						It contains 0xFF value if the event is not a keyboard event or if the event corresponds to (Rx, GND)   */
/*		uint8_t &bGPI	- GPI no - output parameter where the GPI value will be stored.                                        */
/*						It contains 0xFF value if the event is not a GPI event.                                                */
/*		uint8_t &bLogic	- Logic no - output parameter where the Logic value will be stored.                                    */
/*						It contains 0xFF value if the event is not a Logic event.                                              */
/*		uint8_t &bEventState	- Event state - output parameter where the state of the event will be placed.                  */
/*						For keys events:                                                                                       */
/*								1 - key is pressed                                                                             */
/*								0 - key is released                                                                            */
/*						For GPI and Logic events:                                                                              */
/*								1 - GPI/logic is active                                                                        */
/*								0 - GPI/logic is inactive                                                                      */                   
/*		                                                                                                                       */
/*  Return Value:                                                                                                              */
/*		void                                                                                                                   */
/*                                                                                                                             */
/*	Errors:                                                                                                                    */
/*                                                                                                                             */
/*	Description:                                                                                                               */
/*		This function reads the Unlock1 event, decodes it and returns the specific information.                                */
/*      If the event is a key event, key value, row and column the information corresponding to the key are stored in          */
/*		output parameters while the other parameters corresponding to GPI events (bGPI) and Logic events (bLogic) are set to   */
/*		0xFF. If the event is a GPI event, the GPI number is stored in bGPI while the other parameters corresponding to        */
/*		key events (keyVal, bRow, bCol) and to Logic events (bLogic) are set to -1 (keyVal) and 0xFF respectively.             */
/*      If the event is a Logic event, the Logic number is stored in bLogic, while the parameters corresponding to             */
/*		key events (keyVal, bRow, bCol) and to Key events are set to -1 (keyVal) and 0xFF respectively.	                       */ 
/* --------------------------------------------------------------------------------------------------------------------------- */

void IOXP::GetUnlock1Event(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState)
{
	uint8_t bEvent;
	ReadBytesI2C(IOXP_ADDR_UNLOCK1, 1, &bEvent);
	DecodeEvent(bEvent, iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
}

/* -------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetUnlock2Event                                                                                                     */
/*                                                                                                                            */
/*	Synopsis:                                                                                                                 */
/*		SetUnlock2Event(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState);          */
/*	Parameters:                                                                                                               */  
/*	 	int iKeyVal	- key value                                                                                               */
/*						It contains -1 value if the event is not a keyboard event or if the key is specified by row and       */
/*						column information.                                                                                   */
/*		uint8_t bRow	- key row value                                                                                       */
/*					    It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event.      */
/*						This parameter has a default value of 0xFF.                                                           */
/*		uint8_t bCol	- key column value                                                                                    */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event or    */
/*						if the event corresponds to (Rx, GND).                                                                */
/*						This parameter has a default value of 0xFF.                                                           */
/*		uint8_t bGPI	- GPI number.                                                                                         */
/*						It contains 0xFF value if the event is not a GPI event.                                               */
/*						This parameter has a default value of 0xFF.                                                           */
/*		uint8_t bLogic	- Logic number.                                                                                       */
/*						It contains 0xFF value if the event is not a Logic event.                                             */
/*						This parameter has a default value of 0xFF.                                                           */
/*		uint8_t bEventState - Event state                                                                                     */
/*						For keys events:                                                                                      */
/*								1 - key is pressed                                                                            */
/*								0 - key is released                                                                           */
/*						For GPI and Logic events:                                                                             */
/*								1 - GPI/logic is active                                                                       */
/*								0 - GPI/logic is inactive                                                                     */
/*						This parameter has a default value of 1.                                                              */                                                                                                                   
/*  Return Value:                                                                                                             */
/*		void                                                                                                                  */
/*                                                                                                                            */
/*	Errors:                                                                                                                   */
/*                                                                                                                            */
/*	Description:                                                                                                              */
/*		This function sets the Unlock2 event according to the specified information. It encodes information in the event      */
/*		byte using EncodeEvent function. When more keys have that same value associated, then only the first one of them      */
/*		will be identified with that value. If no key corresponds to the specified key value, no event will be created.       */
/*      The last five parameters have default values.                                                                         */ 
/* -------------------------------------------------------------------------------------------------------------------------- */


void IOXP::SetUnlock2Event(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState)
{
	uint8_t bEvent = EncodeEvent(iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
	WriteBytesI2C(IOXP_ADDR_UNLOCK2, 1, &bEvent);	
}

/* -------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetUnlock2Event                                                                                                           */
/*                                                                                                                                  */
/*	Synopsis:                                                                                                                       */
/*		GetUnlock2Event(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);          */
/*	Parameters:                                                                                                                     */  
/*	 	 int &iKeyVal	- key value - output parameter where the key value will be stored.                                          */
/*						It contains -1 value if the event is not a keyboard event or if the key map table contains -1 value.        */
/*		uint8_t &bRow	- key row - output parameter where the keyboard row value will be stored.                                   */
/*						It contains 0xFF value if the event is not a keyboard event.                                                */
/*		uint8_t &bCol	- key column - output parameter where the keyboard column value will be stored.                             */
/*						It contains 0xFF value if the event is not a keyboard event or if the event corresponds to (Rx, GND)        */
/*		uint8_t &bGPI	- GPI no - output parameter where the GPI value will be stored.                                             */
/*						It contains 0xFF value if the event is not a GPI event.                                                     */
/*		uint8_t &bLogic	- Logic no - output parameter where the Logic value will be stored.                                         */
/*						It contains 0xFF value if the event is not a Logic event.                                                   */
/*		uint8_t &bEventState	- Event state - output parameter where the state of the event will be placed.                       */
/*						For keys events:                                                                                            */
/*								1 - key is pressed                                                                                  */
/*								0 - key is released                                                                                 */
/*						For GPI and Logic events:                                                                                   */
/*								1 - GPI/logic is active                                                                             */
/*								0 - GPI/logic is inactive                                                                           */
/*						                                                                                                            */
/*  Return Value:                                                                                                                   */
/*		 void                                                                                                                       */
/*                                                                                                                                  */
/*	Errors:                                                                                                                         */
/*                                                                                                                                  */
/*	Description:                                                                                                                    */
/*		This function reads the Unlock2 event, decodes it and returns the specific information.                                     */
/*      If the event is a key event, key value, row and column the information corresponding to the key are stored in output        */
/*		parameters while the other parameters corresponding to GPI events (bGPI) and Logic events (bLogic) are set to 0xFF.         */
/*      If the event is a GPI event, the GPI number is stored in bGPI while the other parameters corresponding to                   */
/*		key events (keyVal, bRow, bCol) and to Logic events (bLogic) are set to -1 (keyVal) and 0xFF respectively.                  */
/*      If the event is a Logic event, the Logic number is stored in bLogic, while the parameters corresponding to                  */
/*		key events (keyVal, bRow, bCol) and to Key events are set to -1 (keyVal) and 0xFF respectively.                             */ 
/* -------------------------------------------------------------------------------------------------------------------------------- */


void IOXP::GetUnlock2Event(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState)
{
	uint8_t bEvent;
	ReadBytesI2C(IOXP_ADDR_UNLOCK2, 1, &bEvent);
	DecodeEvent(bEvent, iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
}

/* ---------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetReset1EventA                                                                                                       */
/*                                                                                                                              */
/*	Synopsis:                                                                                                                   */
/*		SetReset1EventA(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState);            */
/*	Parameters:                                                                                                                 */  
/*		int iKeyVal	- key value                                                                                                 */
/*						It contains -1 value if the event is not a keyboard event or if the key is specified by row and column  */
/*						information.                                                                                            */
/*		uint8_t bRow	- key row value                                                                                         */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event.        */
/*						This parameter has a default value of 0xFF.                                                             */
/*		uint8_t bCol	- key column value                                                                                      */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event or if   */
/*						the event corresponds to (Rx, GND).                                                                     */
/*						This parameter has a default value of 0xFF.                                                             */
/*		uint8_t bGPI	- GPI number.                                                                                           */
/*						It contains 0xFF value if the event is not a GPI event.                                                 */
/*						This parameter has a default value of 0xFF.                                                             */
/*		uint8_t bLogic	- Logic number.                                                                                         */
/*						It contains 0xFF value if the event is not a Logic event.                                               */
/*						This parameter has a default value of 0xFF.                                                             */
/*		uint8_t bEventState - Event state                                                                                       */
/*						For keys events:                                                                                        */
/*								1 - key is pressed                                                                              */
/*								0 - key is released                                                                             */
/*						For GPI and Logic events:                                                                               */
/*								1 - GPI/logic is active                                                                         */
/*								0 - GPI/logic is inactive                                                                       */
/*						This parameter has a default value of 1.	                                                            */
/*  Return Value:                                                                                                               */
/*		void                                                                                                                    */
/*                                                                                                                              */
/*	Errors:                                                                                                                     */
/*                                                                                                                              */
/*	Description:                                                                                                                */
/*		This function sets the Reset1EventA event according to the specified information. It encodes information in the event   */
/*		byte using EncodeEvent function. When more keys have that same value associated, then only the first one of them will   */
/*		be identified with that value. If no key corresponds to the specified key value, no event will be created.              */
/*      The last five parameters have default values.                                                                           */ 
/* ---------------------------------------------------------------------------------------------------------------------------- */

void IOXP::SetReset1EventA(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState)
{
	uint8_t bEvent = EncodeEvent(iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
	WriteBytesI2C(IOXP_ADDR_RESET1_EVENT_A, 1, &bEvent);	
}

/* ------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetReset1EventA                                                                                                          */
/*                                                                                                                                 */
/*	Synopsis:                                                                                                                      */
/*		GetReset1EventA(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);         */
/*	Parameters:                                                                                                                    */  
/*	 	int &iKeyVal	- key value - output parameter where the key value will be stored.                                         */
/*						It contains -1 value if the event is not a keyboard event or if the key map table contains -1 value.       */
/*		uint8_t &bRow	- key row - output parameter where the keyboard row value will be stored.                                  */
/*						It contains 0xFF value if the event is not a keyboard event.                                               */
/*		uint8_t &bCol	- key column - output parameter where the keyboard column value will be stored.                            */
/*						It contains 0xFF value if the event is not a keyboard event or if the event corresponds to (Rx, GND)       */
/*		uint8_t &bGPI	- GPI no - output parameter where the GPI value will be stored.                                            */
/*						It contains 0xFF value if the event is not a GPI event.                                                    */
/*		uint8_t &bLogic	- Logic no - output parameter where the Logic value will be stored.                                        */
/*						It contains 0xFF value if the event is not a Logic event.                                                  */
/*		uint8_t &bEventState	- Event state - output parameter where the state of the event will be placed.                      */
/*						For keys events:                                                                                           */
/*								1 - key is pressed                                                                                 */
/*								0 - key is released                                                                                */
/*						For GPI and Logic events:                                                                                  */
/*								1 - GPI/logic is active                                                                            */
/*								0 - GPI/logic is inactive                                                                          */
/*  Return Value:                                                                                                                  */
/*		void                                                                                                                       */
/*                                                                                                                                 */
/*	Errors:                                                                                                                        */
/*                                                                                                                                 */
/*	Description:                                                                                                                   */
/*		This function reads the Reset1EventA event, decodes it and returns the specific information.                               */
/*      If the event is a key event, key value, row and column the information corresponding to the key are stored in output       */
/*		parameters while the other parameters corresponding to GPI events (bGPI) and Logic events (bLogic) are set to 0xFF.        */
/*      If the event is a GPI event, the GPI number is stored in bGPI while the other parameters corresponding to                  */
/*		key events (keyVal, bRow, bCol) and to Logic events (bLogic) are set to -1 (keyVal) and 0xFF respectively.                 */
/*      If the event is a Logic event, the Logic number is stored in bLogic, while the parameters corresponding to                 */
/*	    key events (keyVal, bRow, bCol) and to Key events are set to -1 (keyVal) and 0xFF respectively.                            */ 
/* ------------------------------------------------------------------------------------------------------------------------------- */

void IOXP::GetReset1EventA(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState)
{
	uint8_t bEvent;
	ReadBytesI2C(IOXP_ADDR_RESET1_EVENT_A, 1, &bEvent);
	DecodeEvent(bEvent, iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
}

/* -------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetReset1EventB                                                                                                     */
/*                                                                                                                            */
/*	Synopsis:                                                                                                                 */
/*		SetReset1EventB(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState);          */
/*	Parameters:                                                                                                               */  
/*	 	int iKeyVal	- key value                                                                                               */
/*						It contains -1 value if the event is not a keyboard event or if the key is specified by row and       */
/*						column information.                                                                                   */
/*		uint8_t bRow	- key row value                                                                                       */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event.      */
/*						This parameter has a default value of 0xFF.                                                           */
/*		uint8_t bCol	- key column value                                                                                    */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event or    */
/*						if the event corresponds to (Rx, GND).                                                                */
/*						This parameter has a default value of 0xFF.                                                           */
/*		uint8_t bGPI	- GPI number.                                                                                         */
/*						It contains 0xFF value if the event is not a GPI event.                                               */
/*						This parameter has a default value of 0xFF.                                                           */
/*		uint8_t bLogic	- Logic number.                                                                                       */
/*						It contains 0xFF value if the event is not a Logic event.                                             */
/*						This parameter has a default value of 0xFF.                                                           */
/*		uint8_t bEventState - Event state                                                                                     */
/*						For keys events:                                                                                      */
/*								1 - key is pressed                                                                            */
/*								0 - key is released                                                                           */ 
/*						For GPI and Logic events:                                                                             */
/*								1 - GPI/logic is active                                                                       */
/*								0 - GPI/logic is inactive                                                                     */
/*						This parameter has a default value of 1.                                                              */
/*						                                                                                                      */
/*  Return Value:                                                                                                             */
/*		void                                                                                                                  */
/*                                                                                                                            */
/*	Errors:                                                                                                                   */
/*                                                                                                                            */
/*	Description:                                                                                                              */
/*		This function sets the Reset1EventB event according to the specified information. It encodes information in the event */
/*		byte using EncodeEvent function. When more keys have that same value associated, then only the first one of them will */
/*		be identified with that value. If no key corresponds to the specified key value, no event will be created.            */
/*      The last five parameters have default values.	                                                                      */ 
/* -------------------------------------------------------------------------------------------------------------------------- */

void IOXP::SetReset1EventB(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState)
{
	uint8_t bEvent = EncodeEvent(iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
	WriteBytesI2C(IOXP_ADDR_RESET1_EVENT_B, 1, &bEvent);	
}

/* -------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetReset1EventB                                                                                                           */
/*                                                                                                                                  */
/*	Synopsis:                                                                                                                       */
/*		GetReset1EventB(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);          */
/*	Parameters:                                                                                                                     */  
/*	 	int &iKeyVal	- key value - output parameter where the key value will be stored.                                          */
/*						It contains -1 value if the event is not a keyboard event or if the key map table contains -1 value.        */
/*		uint8_t &bRow	- key row - output parameter where the keyboard row value will be stored.                                   */
/*						It contains 0xFF value if the event is not a keyboard event.                                                */
/*		uint8_t &bCol	- key column - output parameter where the keyboard column value will be stored.                             */
/*						It contains 0xFF value if the event is not a keyboard event or if the event corresponds to (Rx, GND)        */
/*		uint8_t &bGPI	- GPI no - output parameter where the GPI value will be stored.                                             */
/*						It contains 0xFF value if the event is not a GPI event.                                                     */
/*		uint8_t &bLogic	- Logic no - output parameter where the Logic value will be stored.                                         */
/*						It contains 0xFF value if the event is not a Logic event.                                                   */
/*		uint8_t &bEventState	- Event state - output parameter where the state of the event will be placed.                       */
/*						For keys events:                                                                                            */
/*								1 - key is pressed                                                                                  */
/*								0 - key is released                                                                                 */
/*						For GPI and Logic events:                                                                                   */
/*								1 - GPI/logic is active                                                                             */
/*								0 - GPI/logic is inactive                                                                           */
/*  Return Value:                                                                                                                   */
/*		void                                                                                                                        */
/*                                                                                                                                  */
/*	Errors:                                                                                                                         */
/*                                                                                                                                  */
/*	Description:                                                                                                                    */
/*		This function reads the Reset1EventB event, decodes it and returns the specific information.                                */
/*      If the event is a key event, key value, row and column the information corresponding to the key are stored in output        */
/*		parameters while the other parameters corresponding to GPI events (bGPI) and Logic events (bLogic) are set to 0xFF.         */
/*      If the event is a GPI event, the GPI number is stored in bGPI while the other parameters corresponding to                   */
/*		key events (keyVal, bRow, bCol) and to Logic events (bLogic) are set to -1 (keyVal) and 0xFF respectively.                  */
/*      If the event is a Logic event, the Logic number is stored in bLogic, while the parameters corresponding to                  */
/*		key events (keyVal, bRow, bCol) and to Key events are set to -1 (keyVal) and 0xFF respectively.                             */ 
/* -------------------------------------------------------------------------------------------------------------------------------- */

void IOXP::GetReset1EventB(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState)
{
	uint8_t bEvent;
	ReadBytesI2C(IOXP_ADDR_RESET1_EVENT_B, 1, &bEvent);
	DecodeEvent(bEvent, iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
}

/* -------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetReset1EventC                                                                                                     */
/*                                                                                                                            */
/*	Synopsis:                                                                                                                 */
/*		SetReset1EventC(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState);          */
/*	Parameters:                                                                                                               */  
/*	 	int iKeyVal	- key value                                                                                               */
/*						It contains -1 value if the event is not a keyboard event or if the key is specified by row and       */
/*						column information.                                                                                   */
/*		uint8_t bRow	- key row value                                                                                       */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event.      */
/*						This parameter has a default value of 0xFF.                                                           */
/*		uint8_t bCol	- key column value                                                                                    */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event       */
/*						or if the event corresponds to (Rx, GND).                                                             */
/*						This parameter has a default value of 0xFF.                                                           */
/*		uint8_t bGPI	- GPI number.                                                                                         */
/*						It contains 0xFF value if the event is not a GPI event.                                               */
/*						This parameter has a default value of 0xFF.                                                           */
/*		uint8_t bLogic	- Logic number.                                                                                       */
/*						It contains 0xFF value if the event is not a Logic event.                                             */
/*						This parameter has a default value of 0xFF.                                                           */
/*		uint8_t bEventState - Event state                                                                                     */
/*						For keys events:                                                                                      */
/*								1 - key is pressed                                                                            */
/*								0 - key is released                                                                           */
/*						For GPI and Logic events:                                                                             */
/*								1 - GPI/logic is active                                                                       */
/*								0 - GPI/logic is inactive                                                                     */
/*						This parameter has a default value of 1.                                                              */
/*  Return Value:                                                                                                             */
/*		 void                                                                                                                 */
/*                                                                                                                            */
/*	Errors:                                                                                                                   */
/*                                                                                                                            */
/*	Description:                                                                                                              */
/*		This function sets the Reset1EventC event according to the specified information. It encodes information in the event */
/*		byte using EncodeEvent function. When more keys have that same value associated, then only the first one of them will */
/*		be identified with that value. If no key corresponds to the specified key value, no event will be created.            */
/*      The last five parameters have default values.                                                                         */ 
/* -------------------------------------------------------------------------------------------------------------------------- */

void IOXP::SetReset1EventC(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState)
{
	uint8_t bEvent = EncodeEvent(iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
	WriteBytesI2C(IOXP_ADDR_RESET1_EVENT_C, 1, &bEvent);	
}

/* -------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetReset1EventC                                                                                                           */
/*                                                                                                                                  */
/*	Synopsis:                                                                                                                       */
/*		GetReset1EventC(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);          */
/*	Parameters:                                                                                                                     */  
/*	 	int &iKeyVal	- key value - output parameter where the key value will be stored.                                          */
/*						It contains -1 value if the event is not a keyboard event or if the key map table contains -1 value.        */
/*		uint8_t &bRow	- key row - output parameter where the keyboard row value will be stored.                                   */
/*						It contains 0xFF value if the event is not a keyboard event.                                                */
/*		uint8_t &bCol	- key column - output parameter where the keyboard column value will be stored.                             */
/*						It contains 0xFF value if the event is not a keyboard event or if the event corresponds to (Rx, GND)        */
/*		uint8_t &bGPI	- GPI no - output parameter where the GPI value will be stored.                                             */
/*						It contains 0xFF value if the event is not a GPI event.                                                     */
/*		uint8_t &bLogic	- Logic no - output parameter where the Logic value will be stored.                                         */
/*						It contains 0xFF value if the event is not a Logic event.                                                   */
/*		uint8_t &bEventState	- Event state - output parameter where the state of the event will be placed.                       */
/*						For keys events:                                                                                            */
/*								1 - key is pressed                                                                                  */
/*								0 - key is released                                                                                 */
/*						For GPI and Logic events:                                                                                   */
/*								1 - GPI/logic is active                                                                             */
/*								0 - GPI/logic is inactive                                                                           */
/*  Return Value:                                                                                                                   */
/*		 void                                                                                                                       */
/*                                                                                                                                  */
/*	Errors:                                                                                                                         */
/*                                                                                                                                  */
/*	Description:                                                                                                                    */
/*		This function reads the Reset1EventC event, decodes it and returns the specific information.                                */
/*      If the event is a key event, key value, row and column the information corresponding to the key are stored in output        */
/*		parameters while the other parameters corresponding to GPI events (bGPI) and Logic events (bLogic) are set to 0xFF.         */
/*      If the event is a GPI event, the GPI number is stored in bGPI while the other parameters corresponding to                   */
/*		key events (keyVal, bRow, bCol) and to Logic events (bLogic) are set to -1 (keyVal) and 0xFF respectively.                  */
/*      If the event is a Logic event, the Logic number is stored in bLogic, while the parameters corresponding to                  */
/*		key events (keyVal, bRow, bCol) and to Key events are set to -1 (keyVal) and 0xFF respectively.                             */ 
/* -------------------------------------------------------------------------------------------------------------------------------- */

void IOXP::GetReset1EventC(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState)
{
	uint8_t bEvent;
	ReadBytesI2C(IOXP_ADDR_RESET1_EVENT_C, 1, &bEvent);
	DecodeEvent(bEvent, iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
}

/* ------------------------------------------------------------------------------------------------------------------------------ */
/*	IOXP::SetReset2EventA                                                                                                         */
/*                                                                                                                                */
/*	Synopsis:                                                                                                                     */
/*		SetReset2EventA(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState);	          */
/*	Parameters:                                                                                                                   */  
/*		int iKeyVal	- key value                                                                                                   */
/*						It contains -1 value if the event is not a keyboard event or if the key is specified by row and column    */
/*						information.                                                                                              */
/*		uint8_t bRow	- key row value                                                                                           */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event.          */
/*						This parameter has a default value of 0xFF.                                                               */
/*		uint8_t bCol	- key column value                                                                                        */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event or if     */
/*						the event corresponds to (Rx, GND).                                                                       */
/*						This parameter has a default value of 0xFF.                                                               */
/*		uint8_t bGPI	- GPI number.                                                                                             */
/*						It contains 0xFF value if the event is not a GPI event.                                                   */
/*						This parameter has a default value of 0xFF.                                                               */
/*		uint8_t bLogic	- Logic number.                                                                                           */
/*						It contains 0xFF value if the event is not a Logic event.                                                 */
/*						This parameter has a default value of 0xFF.                                                               */
/*		uint8_t bEventState - Event state                                                                                         */
/*						For keys events:                                                                                          */
/*								1 - key is pressed                                                                                */
/*								0 - key is released                                                                               */
/*						For GPI and Logic events:                                                                                 */
/*								1 - GPI/logic is active                                                                           */
/*								0 - GPI/logic is inactive                                                                         */
/*						This parameter has a default value of 1.                                                                  */
/*  Return Value:                                                                                                                 */
/*		void                                                                                                                      */
/*                                                                                                                                */
/*	Errors:                                                                                                                       */
/*                                                                                                                                */
/*	Description:                                                                                                                  */
/*		This function sets the Reset2EventA event according to the specified information. It encodes information in the event     */
/*		byte using EncodeEvent function. When more keys have that same value associated, then only the first one of them will     */
/*		be identified with that value. If no key corresponds to the specified key value, no event will be created.                */
/*      The last five parameters have default values.                                                                             */ 
/* ------------------------------------------------------------------------------------------------------------------------------ */

void IOXP::SetReset2EventA(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState)
{
	uint8_t bEvent = EncodeEvent(iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
	WriteBytesI2C(IOXP_ADDR_RESET2_EVENT_A, 1, &bEvent);	
}

/* ------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetReset2EventA                                                                                                          */
/*                                                                                                                                 */
/*	Synopsis:                                                                                                                      */
/*		GetReset2EventA(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);         */
/*	Parameters:                                                                                                                    */  
/*	 	int &iKeyVal	- key value - output parameter where the key value will be stored.                                         */
/*						It contains -1 value if the event is not a keyboard event or if the key map table contains -1 value.       */
/*		uint8_t &bRow	- key row - output parameter where the keyboard row value will be stored.                                  */
/*						It contains 0xFF value if the event is not a keyboard event.                                               */
/*		uint8_t &bCol	- key column - output parameter where the keyboard column value will be stored.                            */
/*						It contains 0xFF value if the event is not a keyboard event or if the event corresponds to (Rx, GND)       */
/*		uint8_t &bGPI	- GPI no - output parameter where the GPI value will be stored.                                            */
/*						It contains 0xFF value if the event is not a GPI event.                                                    */
/*		uint8_t &bLogic	- Logic no - output parameter where the Logic value will be stored.                                        */ 
/*						It contains 0xFF value if the event is not a Logic event.                                                  */
/*		uint8_t &bEventState	- Event state - output parameter where the state of the event will be placed.                      */
/*						For keys events:                                                                                           */
/*								1 - key is pressed                                                                                 */
/*								0 - key is released                                                                                */
/*						For GPI and Logic events:                                                                                  */
/*								1 - GPI/logic is active                                                                            */
/*								0 - GPI/logic is inactive                                                                          */
/*  Return Value:                                                                                                                  */
/*		void                                                                                                                       */
/*                                                                                                                                 */
/*	Errors:                                                                                                                        */
/*                                                                                                                                 */
/*	Description:                                                                                                                   */
/*		This function reads the Reset2EventA event, decodes it and returns the specific information.                               */ 
/*      If the event is a key event, key value, row and column the information corresponding to the key are stored in output       */
/*		parameters while the other parameters corresponding to GPI events (bGPI) and Logic events (bLogic) are set to 0xFF.        */
/*      If the event is a GPI event, the GPI number is stored in bGPI while the other parameters corresponding to                  */
/*		key events (keyVal, bRow, bCol) and to Logic events (bLogic) are set to -1 (keyVal) and 0xFF respectively.                 */
/*      If the event is a Logic event, the Logic number is stored in bLogic, while the parameters corresponding to                 */
/*		key events (keyVal, bRow, bCol) and to Key events are set to -1 (keyVal) and 0xFF respectively.                            */ 
/* ------------------------------------------------------------------------------------------------------------------------------- */

void IOXP::GetReset2EventA(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState)
{
	uint8_t bEvent;
	ReadBytesI2C(IOXP_ADDR_RESET2_EVENT_A, 1, &bEvent);
	DecodeEvent(bEvent, iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
}

/* --------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetReset2EventB                                                                                                      */
/*                                                                                                                             */
/*	Synopsis:                                                                                                                  */
/*		SetReset2EventB(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState);           */
/*	Parameters:                                                                                                                */  
/*	 	int iKeyVal	- key value                                                                                                */
/*						It contains -1 value if the event is not a keyboard event or if the key is specified by row and column */
/*						information.                                                                                           */
/*		uint8_t bRow	- key row value                                                                                        */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event.       */
/*						This parameter has a default value of 0xFF.                                                            */
/*		uint8_t bCol	- key column value                                                                                     */
/*						It contains 0xFF value if the iKeyVal specifies the key or if the event is not a keyboard event or     */
/*						if the event corresponds to (Rx, GND).                                                                 */
/*						This parameter has a default value of 0xFF.                                                            */
/*		uint8_t bGPI	- GPI number.                                                                                          */
/*						It contains 0xFF value if the event is not a GPI event.                                                */
/*						This parameter has a default value of 0xFF.                                                            */
/*		uint8_t bLogic	- Logic number.                                                                                        */
/*						It contains 0xFF value if the event is not a Logic event.                                              */
/*						This parameter has a default value of 0xFF.                                                            */
/*		uint8_t bEventState - Event state                                                                                      */
/*						For keys events:                                                                                       */
/*								1 - key is pressed                                                                             */
/*								0 - key is released                                                                            */
/*						For GPI and Logic events:                                                                              */
/*								1 - GPI/logic is active                                                                        */
/*								0 - GPI/logic is inactive                                                                      */
/*						This parameter has a default value of 1.                                                               */
/*  Return Value:                                                                                                              */
/*		void                                                                                                                   */
/*                                                                                                                             */
/*	Errors:                                                                                                                    */
/*                                                                                                                             */
/*	Description:                                                                                                               */
/*		This function sets the Reset2EventB event according to the specified information. It encodes information in the event  */
/*		byte using EncodeEvent function. When more keys have that same value associated, then only the first one of them will  */
/*		be identified with that value. If no key corresponds to the specified key value, no event will be created.             */
/*      The last five parameters have default values.                                                                          */ 
/* --------------------------------------------------------------------------------------------------------------------------- */

void IOXP::SetReset2EventB(int iKeyVal, uint8_t bRow, uint8_t bCol, uint8_t bGPI, uint8_t bLogic, uint8_t bEventState)
{
	uint8_t bEvent = EncodeEvent(iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
	WriteBytesI2C(IOXP_ADDR_RESET2_EVENT_B, 1, &bEvent);	
}

/* -------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetReset2EventB                                                                                                           */
/*                                                                                                                                  */
/*	Synopsis:                                                                                                                       */
/*		GetReset2EventB(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);          */
/*	Parameters:                                                                                                                     */  
/*	 	uint8_t &bRow	- key row - output parameter where the keyboard row value will be stored.                                   */
/*						It contains 0xFF value if the event is not a keyboard event.                                                */
/*		uint8_t &bCol	- key column - output parameter where the keyboard column value will be stored.                             */
/*						It contains 0xFF value if the event is not a keyboard event or if the event corresponds to (Rx, GND)        */
/*		uint8_t &bGPI	- GPI no - output parameter where the GPI value will be stored.                                             */
/*						It contains 0xFF value if the event is not a GPI event.                                                     */
/*		uint8_t &bLogic	- Logic no - output parameter where the Logic value will be stored.                                         */
/*						It contains 0xFF value if the event is not a Logic event.                                                   */
/*		uint8_t &bEventState	- Event state - output parameter where the state of the event will be placed.                       */
/*						For keys events:                                                                                            */
/*								1 - key is pressed                                                                                  */
/*								0 - key is released                                                                                 */
/*						For GPI and Logic events:                                                                                   */
/*								1 - GPI/logic is active                                                                             */
/*								0 - GPI/logic is inactive                                                                           */
/*  Return Value:                                                                                                                   */
/*		void                                                                                                                        */
/*                                                                                                                                  */
/*	Errors:                                                                                                                         */
/*                                                                                                                                  */
/*	Description:                                                                                                                    */
/*		This function reads the Reset2EventB event, decodes it and returns the specific information.                                */
/*      If the event is a key event, key value, row and column the information corresponding to the key are stored in output        */
/*		parameters while the other parameters corresponding to GPI events (bGPI) and Logic events (bLogic) are set to 0xFF.         */
/*      If the event is a GPI event, the GPI number is stored in bGPI while the other parameters corresponding to                   */
/*		key events (keyVal, bRow, bCol) and to Logic events (bLogic) are set to -1 (keyVal) and 0xFF respectively.                  */
/*      If the event is a Logic event, the Logic number is stored in bLogic, while the parameters corresponding to                  */
/*		key events (keyVal, bRow, bCol) and to Key events are set to -1 (keyVal) and 0xFF respectively.                             */
/* -------------------------------------------------------------------------------------------------------------------------------- */

void IOXP::GetReset2EventB(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState)
{
	uint8_t bEvent;
	ReadBytesI2C(IOXP_ADDR_RESET2_EVENT_B, 1, &bEvent);
	DecodeEvent(bEvent, iKeyVal, bRow, bCol, bGPI, bLogic, bEventState);
}

/* ---------------------------------------------------------------------------------------- */
/*	 IOXP::SetIntMaskTimer                                                                  */
/*                                                                                          */
/*	Synopsis:                                                                               */
/*		SetIntMaskTimer(uint8_t bIntMaskTimer);                                             */
/*	Parameters:                                                                             */  
/*	 	uint8_t bIntMaskTimer - the information to be set for INT_MASK_TIMER bits.          */
/*					It can be one of the following time values:                             */
/*		            IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_DIS		(0)		- disabled          */
/*                  IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_1SEC	(1)		-  1 sec            */
/*                  IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_2SEC	(2)		-  2 sec            */
/*                  IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_30SEC	(0X1E)	- 30 sec            */
/*                  IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_31SEC	(0X1F)	- 31 sec            */
/*  Return Value:                                                                           */
/*		void                                                                                */
/*                                                                                          */
/*	Errors:                                                                                 */
/*                                                                                          */
/*	Description:                                                                            */
/*		This function sets the INT_MASK_TIMER information of the UNLOCK_TIMERS register.    */ 
/* ---------------------------------------------------------------------------------------- */

void IOXP::SetIntMaskTimer(uint8_t bIntMaskTimer)
{
	SetRegisterBitsGroup(IOXP_UNLOCK_TIMERS_INT_MASK_TIMER, bIntMaskTimer);	
}

/* ----------------------------------------------------------------------------------------- */
/*	IOXP::GetIntMaskTimer                                                                    */
/*                                                                                           */
/*	Synopsis:                                                                                */
/*		GetIntMaskTimer();                                                                   */
/*	Parameters:                                                                              */  
/*	 	uint8_t -  the information read from INT_MASK_TIMER bits.                            */
/*					It can be one of the following:                                          */
/*					IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_DIS		(0)		- disabled           */
/*                  IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_1SEC	(1)		-  1 sec             */
/*                  IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_2SEC	(2)		-  2 sec             */
/*                  IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_30SEC	(0X1E)	- 30 sec             */
/*                  IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_31SEC	(0X1F)	- 31 sec             */
/*  Return Value:                                                                            */
/*		 uint8_t  - the INT_MASK_TIMER information of the UNLOCK_TIMERS register             */
/*                                                                                           */
/*	Errors:                                                                                  */
/*                                                                                           */
/*	Description:                                                                             */
/*		This function returns the INT_MASK_TIMER information of the UNLOCK_TIMERS register.  */ 
/* ----------------------------------------------------------------------------------------- */

uint8_t IOXP::GetIntMaskTimer()
{
	return GetRegisterBitsGroup(IOXP_UNLOCK_TIMERS_INT_MASK_TIMER);
}

/* ----------------------------------------------------------------------------------- */
/*	IOXP::SetUnlockTimer                                                               */
/*                                                                                     */
/*	Synopsis:                                                                          */
/*		SetUnlockTimer(uint8_t bUnlockTimer);                                          */
/*	Parameters:                                                                        */  
/*	 	uint8_t bUnlockTimer	- the information to be set for UNLOCK_TIMER bits      */
/*					It can be one of the following time values:                        */
/*                        IOXP_UNLOCK_TIMER_DIS		- disabled                         */
/*                        IOXP_UNLOCK_TIMER_1SEC	- 1 sec                            */
/*                        IOXP_UNLOCK_TIMER_2SEC	- 2 sec                            */
/*                        IOXP_UNLOCK_TIMER_3SEC	- 3 sec                            */
/*                        IOXP_UNLOCK_TIMER_4SEC	- 4 sec                            */
/*                        IOXP_UNLOCK_TIMER_5SEC	- 5 sec	                           */
/*                        IOXP_UNLOCK_TIMER_6SEC	- 6 sec	                           */
/*                        IOXP_UNLOCK_TIMER_7SEC	- 7 sec	                           */
/*	Return Value:                                                                      */
/*		void                                                                           */
/*                                                                                     */
/*	Errors:                                                                            */
/*                                                                                     */
/*	Description:                                                                       */
/*		This function sets the UNLOCK_TIMER information of the UNLOCK_TIMERS register. */ 
/* ----------------------------------------------------------------------------------- */

void IOXP::SetUnlockTimer(uint8_t bUnlockTimer)
{
	SetRegisterBitsGroup(IOXP_UNLOCK_TIMERS_UNLOCK_TIMER, bUnlockTimer);	
}

/* -------------------------------------------------------------------------------------- */
/*	IOXP::GetUnlockTimer                                                                  */
/*                                                                                        */
/*	Synopsis:                                                                             */
/*		GetUnlockTimer();                                                                 */
/*	Parameters:                                                                           */  
/*	                                                                                      */
/*  Return Value:                                                                         */
/*		 uint8_t -  the information read from UNLOCK_TIMER bits                           */
/*					It can be one of the following time values:                           */
/*                        IOXP_UNLOCK_TIMER_DIS		- disabled                            */
/*                        IOXP_UNLOCK_TIMER_1SEC	- 1 sec                               */
/*                        IOXP_UNLOCK_TIMER_2SEC	- 2 sec                               */
/*                        IOXP_UNLOCK_TIMER_3SEC	- 3 sec                               */
/*                        IOXP_UNLOCK_TIMER_4SEC	- 4 sec                               */
/*                        IOXP_UNLOCK_TIMER_5SEC	- 5 sec	                              */
/*                        IOXP_UNLOCK_TIMER_6SEC	- 6 sec	                              */
/*                        IOXP_UNLOCK_TIMER_7SEC	- 7 sec	                              */
/*                                                                                        */
/*	Errors:                                                                               */
/*                                                                                        */
/*	Description:                                                                          */
/*		This function returns the UNLOCK_TIMER information of the UNLOCK_TIMERS register. */ 
/* -------------------------------------------------------------------------------------- */

uint8_t IOXP::GetUnlockTimer()
{
	return GetRegisterBitsGroup(IOXP_UNLOCK_TIMERS_UNLOCK_TIMER);
}

/* ---------------------------------------------------------------------------------------- */
/*	IOXP::SetRPullConfig                                                                    */
/*                                                                                          */
/*	Synopsis:                                                                               */
/*		SetRPullConfig(uint8_t *rgbVals);                                                   */
/*	Parameters:                                                                             */  
/*	 	uint8_t *rgbVals - 	the array containing five bytes corresponding to RPULL_CONFIG   */
/*							registers. Each byte contains PULL UP / DOWN options for four   */
/*							rows or columns, according to ADP5589 documentation.            */
/*  Return Value:                                                                           */
/*		void                                                                                */
/*                                                                                          */
/*	Errors:                                                                                 */
/*                                                                                          */
/*	Description:                                                                            */
/*		This function sets the value for all five RPULL_CONFIG registers by providing an    */
/*		array containing five bytes. Each byte contains PULL UP / DOWN options for four     */
/*		rows or columns, according to ADP5589 documentation.                                */
/*		The possible options for each row or column are:                                    */
/*			IOXP_RPULL_CONFIG_300KPU		0	// enable 300 kΩ pull-up                    */
/*			IOXP_RPULL_CONFIG_300KPD		1	// enable 300 kΩ pull-down                  */
/*          IOXP_RPULL_CONFIG_100KPD		2	// enable 100 kΩ pull-up.                   */
/*          IOXP_RPULL_CONFIG_300KPD		3	// disable all pull-up/pull-down resistors  */
/*		Note that this information can be accessed for each row or column:                  */
/*			- for rows using SetRegisterBitsGroup(IOXP_RPULL_CONFIG_R_PULL_CFG(bRowNo))     */
/*				(bRowNo between 0 and 7).                                                   */
/*			- for columns using SetRegisterBitsGroup(IOXP_RPULL_CONFIG_C_PULL_CFG(bColNo))  */
/*				(bColNo between 0 and 10).                                                  */ 
/* ---------------------------------------------------------------------------------------- */

void IOXP::SetRPullConfig(uint8_t *rgbVals)
{
	WriteBytesI2C(IOXP_ADDR_RPULL_CONFIG_A, 5, rgbVals);
}

/* --------------------------------------------------------------------------------------- */
/*	IOXP::GetRPullConfig                                                                   */
/*                                                                                         */
/*	Synopsis:                                                                              */
/*		GetRPullConfig(uint8_t *rgbVals);                                                  */
/*	Parameters:                                                                            */  
/*	 	uint8_t *rgbVals  - the array containing five bytes corresponding to               */ 
/*							RPULL_CONFIG registers.                                        */
/*							Each byte contains PULL UP / DOWN options for four rows        */
/*							or columns, according to ADP5589 documentation.                */
/*  Return Value:                                                                          */
/*		void                                                                               */
/*                                                                                         */
/*	Errors:                                                                                */
/*                                                                                         */
/*	Description:                                                                           */
/*		This function retrieves the PULL UP / DOWN option for all rows and columns into an */
/*		array containing five bytes. Each byte contains PULL UP / DOWN options for four    */
/*		rows or columns, according to ADP5589 documentation. The possible options for each */
/*		row or column are:                                                                 */
/*			IOXP_RPULL_CONFIG_300KPU		0	// enable 300 kO pull-up                   */
/*			IOXP_RPULL_CONFIG_300KPD		1	// enable 300 kO pull-down                 */
/*          IOXP_RPULL_CONFIG_100KPD		2	// enable 100 kO pull-up.                  */
/*          IOXP_RPULL_CONFIG_300KPD		3	// disable all pull-up/pull-down resistors */
/*		Note that this information can be accessed for each row or column:                 */
/*			- for rows using GetRegisterBitsGroup(IOXP_RPULL_CONFIG_R_PULL_CFG(bRowNo))    */
/*				(bRowNo between 0 and 7).                                                  */
/*			- for columns using GetRegisterBitsGroup(IOXP_RPULL_CONFIG_C_PULL_CFG(bColNo)) */
/*				(bColNo between 0 and 10).                                                 */       
/* --------------------------------------------------------------------------------------- */

void IOXP::GetRPullConfig(uint8_t *rgbVals)
{
	ReadBytesI2C(IOXP_ADDR_RPULL_CONFIG_A, 5, rgbVals);
}

/* ----------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetGPIIntStat                                                                                                                */
/*                                                                                                                                     */
/*	Synopsis:                                                                                                                          */
/*		GetGPIIntStat();                                                                                                               */
/*	Parameters:                                                                                                                        */  
/*      void                                                                                                                           */
/*                                                                                                                                     */
/*  Return Value:                                                                                                                      */
/*	 	uint32_t - the 32-bit value containing in its 19 LSbs the value of the GPI_INT bits, for each GPI pin. These bits are          */
/*					grouped in the three GPI_INT_STAT 8-bit registers corresponding to each of the 19 GPIs.                            */
/*	Errors:                                                                                                                            */
/*                                                                                                                                     */
/*	Description:                                                                                                                       */
/*		This function returns a value containing in its 19 LSbs the value of the GPI_INT bits, for each GPI pin. These bits are        */
/*		grouped in the three GPI_INT_STAT 8-bit registers corresponding to each of the 19 GPIs.	This is the function used to read      */
/*		which of the GPI pin triggered an interrupt. Note that each GPI_INT bit can be accessed using the function                     */
/*		GetRegisterBit(IOXP_GPI_INT_STAT_GPI_INT(bGPIONo)), where bGPIONo is in the 1 - 19 range.                                      */
/* ----------------------------------------------------------------------------------------------------------------------------------- */

uint32_t IOXP::GetGPIIntStat()
{
	uint32_t bResult = 0;
	ReadBytesI2C(IOXP_ADDR_GPI_INT_STATUS_A, 3, (uint8_t *)&bResult);
	return bResult;
}

/* ---------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetGPIStat                                                                                                                  */
/*                                                                                                                                    */
/*	Synopsis:                                                                                                                         */
/*		GetGPIStat();                                                                                                                 */
/*	Parameters:                                                                                                                       */  
/*		void                                                                                                                          */
/*  Return Value:                                                                                                                     */
/*		uint32_t - the 32-bit value containing in its 19 LSbs the value of the GPI_STAT bits, for each GPI pin. These bits are        */
/*		grouped in the three GPI_STATUS 8-bit registers                                                                               */
/*                                                                                                                                    */
/*	Errors:                                                                                                                           */
/*                                                                                                                                    */
/*	Description:                                                                                                                      */
/*		This function returns a value containing in its 19 LSbs the value of the GPI_STAT bits, for each GPI pin. These bits are      */
/*		grouped in the three GPI_STATUS 8-bit registers. This is the function used to read the input value of the GPIOs defined       */
/*		as inputs.                                                                                                                    */
/*		Note that each GPI_STAT bit can be accessed using the function GetRegisterBit(IOXP_GPI_STATUS_GPI_STAT(bGPIONo)),             */
/*		where bGPIONo is in the 1 - 19 range.                                                                                         */
/* ---------------------------------------------------------------------------------------------------------------------------------- */

uint32_t IOXP::GetGPIStat()
{
	uint32_t bResult = 0;
	ReadBytesI2C(IOXP_ADDR_GPI_STATUS_A, 3, (uint8_t *)&bResult);
	return bResult;
}

/* ------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetGPIIntLevel                                                                                                           */
/*                                                                                                                                 */
/*	Synopsis:                                                                                                                      */
/*		SetGPIIntLevel(uint32_t dwBitMap);                                                                                         */
/*	Parameters:                                                                                                                    */  
/*	 	uint32_t dwBitMap	- the 32-bit value containing in its 19 LSbs the value of the GPI_INT_LEVEL bits, for each GPI pin.    */
/*							  These bits are grouped in the three GPI_INT_LEVEL 8-bit registers                                    */
/*                                                                                                                                 */
/*  Return Value:                                                                                                                  */
/*		void                                                                                                                       */
/*                                                                                                                                 */
/*	Errors:                                                                                                                        */
/*                                                                                                                                 */
/*	Description:                                                                                                                   */
/*		This function sets the value of the GPI_INT_LEVEL bits from GPI_INT_LEVEL registers for all 19 pins by providing a 32-bit  */
/*		value containing in its 19 bits the specified values.                                                                      */
/*		Note that each GPI_INT_LEVEL bit can be accessed using the function SetRegisterBit(IOXP_INT_LEVEL_GPI_INT_LEVEL(bGPIONo)), */
/*		where bGPIONo is in the 1 - 19 range.                                                                                      */
/* ------------------------------------------------------------------------------------------------------------------------------- */

void IOXP::SetGPIIntLevel(uint32_t dwBitMap)
{
	WriteBytesI2C(IOXP_ADDR_GPI_INT_LEVEL_A, 3, (uint8_t *)&dwBitMap);
}

/* -------------------------------------------------------------------------------------------------------------------------------------- */
/*	 IOXP::GetGPIIntLevel                                                                                                                 */
/*                                                                                                                                        */
/*	Synopsis:                                                                                                                             */
/*		GetGPIIntLevel();                                                                                                                 */
/*	Parameters:                                                                                                                           */  
/*	 	 void                                                                                                                             */
/*		                                                                                                                                  */
/*	Return Value:																														  */
/*		 uint32_t - the 32-bit value containing in its 19 LSbs the value of the GPI_INT_LEVEL bits, for each GPI pin. These bits are      */
/*					grouped in the three GPI_INT_LEVEL 8-bit registers                                                                    */
/*                                                                                                                                        */
/*	Errors:                                                                                                                               */
/*                                                                                                                                        */
/*	Description:                                                                                                                          */
/*		This function returns a value containing in its 19 LSbs the value of the GPI_INT_LEVEL bits, for each GPI pin. These bits are     */
/*		grouped in the three GPI_INT_LEVEL 8-bit registers.                                                                               */
/*		Note that each GPI_INT_LEVEL bit can be accessed using the function GetRegisterBit(IOXP_INT_LEVEL_GPI_INT_LEVEL(bGPIONo)),        */
/*		where bGPIONo is in the 1 - 19 range.                                                                                             */
/* -------------------------------------------------------------------------------------------------------------------------------------- */

uint32_t IOXP::GetGPIIntLevel()
{
	uint32_t bResult = 0;
	ReadBytesI2C(IOXP_ADDR_GPI_INT_LEVEL_A, 3, (uint8_t *)&bResult);
	return bResult;
}

/* ---------------------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetGPIEventEn                                                                                                                           */
/*                                                                                                                                                */
/*	Synopsis:                                                                                                                                     */
/*		SetGPIEventEn(uint32_t dwBitMap);                                                                                                         */
/*	Parameters:                                                                                                                                   */  
/*	 	uint32_t dwBitMap	- the 32-bit value containing in its 19 LSbs the value of the GPI_EVENT_EN bits, for each GPI pin. These bits are     */
/*							  grouped in the three GPI_EVENT_EN 8-bit registers.                                                                  */
/*                                                                                                                                                */
/*  Return Value:                                                                                                                                 */
/*		void                                                                                                                                      */
/*                                                                                                                                                */
/*	Errors:                                                                                                                                       */
/*                                                                                                                                                */
/*	Description:                                                                                                                                  */
/*		This function sets the value of the GPI_EVENT_EN bits from GPI_EVENT_EN registers for all 19 pins by providing a 32-bit value containing  */
/*		in its 19 bits the specified values.                                                                                                      */
/*		Note that each GPI_EVENT_EN bit can be accessed using the function SetRegisterBit(IOXP_GPI_EVENT_EN_GPI_EVENT_EN(bGPIONo)),               */
/*		where bGPIONo is in the 1 - 19 range.                                                                                                     */
/* ---------------------------------------------------------------------------------------------------------------------------------------------- */

void IOXP::SetGPIEventEn(uint32_t dwBitMap)
{
	WriteBytesI2C(IOXP_ADDR_GPI_EVENT_EN_A, 3, (uint8_t *)&dwBitMap);
}

/* ------------------------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetGPIEventEn                                                                                                                              */
/*                                                                                                                                                   */
/*	Synopsis:                                                                                                                                        */
/*		GetGPIEventEn();                                                                                                                             */
/*	Parameters:                                                                                                                                      */  
/*	 	 void                                                                                                                                        */
/*		                                                                                                                                             */
/*  Return Value:                                                                                                                                    */
/*		 uint32_t - the 32-bit value containing in its 19 LSbs the value of the GPI_EVENT_EN bits, for each GPI pin. These bits are grouped in the   */
/*					three GPI_EVENT_EN 8-bit registers.                                                                                              */
/*                                                                                                                                                   */
/*                                                                                                                                                   */
/*	Errors:                                                                                                                                          */
/*                                                                                                                                                   */
/*	Description:                                                                                                                                     */
/*		This function a value containing in its 19 LSbs the value of the GPI_EVENT_EN bits, for each GPI pin. These bits are grouped in the three    */
/*		GPI_EVENT_EN 8-bit registers.                                                                                                                */
/*		Note that each GPI_EVENT_EN bit can be accessed using the function GetRegisterBit(IOXP_GPI_EVENT_EN_GPI_EVENT_EN(bGPIONo)),                  */
/*		where bGPIONo is in the 1 - 19 range.                                                                                                        */
/* ------------------------------------------------------------------------------------------------------------------------------------------------- */

uint32_t IOXP::GetGPIEventEn()
{
	uint32_t bResult = 0;
	ReadBytesI2C(IOXP_ADDR_GPI_EVENT_EN_A, 3, (uint8_t *)&bResult);
	return bResult;
}

/* ------------------------------------------------------------------------------------------------------------------------------ */
/*	IOXP::SetGPIDebounceDis                                                                                                       */
/*                                                                                                                                */
/*	Synopsis:                                                                                                                     */
/*		SetGPIDebounceDis(uint32_t dwBitMap);                                                                                     */
/*	Parameters:                                                                                                                   */  
/*	 	uint32_t dwBitMap	- the 32-bit value containing in its 19 LSbs the value of the GPI_DEB_DIS bits, for each GPI pin.     */
/*							  These bits are grouped in the three DEBOUNCE_DIS 8-bit registers.                                   */
/*                                                                                                                                */
/*  Return Value:                                                                                                                 */
/*		 void                                                                                                                     */
/*                                                                                                                                */
/*	Errors:                                                                                                                       */
/*                                                                                                                                */
/*	Description:                                                                                                                  */
/*		This function sets the value of the GPI_DEB_DIS bits from DEBOUNCE_DIS registers for all 19 pins by providing a 32-bit    */
/*		value containing in its 19 bits the specified values.                                                                     */
/*		Note that each GPI_DEB_DIS bit can be accessed using the function SetRegisterBit(IOXP_DEBOUNCE_DIS_GPI_DEB_DIS(bGPIONo)), */ 
/*	    where bGPIONo is in the 1 - 19 range.                                                                                     */
/* ------------------------------------------------------------------------------------------------------------------------------ */

void IOXP::SetGPIDebounceDis(uint32_t dwBitMap)
{
	WriteBytesI2C(IOXP_ADDR_DEBOUNCE_DIS_A, 3, (uint8_t *)&dwBitMap);
}

/* -------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetGPIDebounceDis                                                                                                         */
/*                                                                                                                                  */
/*	Synopsis:                                                                                                                       */
/*		GetGPIDebounceDis();                                                                                                        */
/*	Parameters:                                                                                                                     */  
/*	 	 void                                                                                                                       */
/*  Return Value:                                                                                                                   */
/*		 uint32_t - the 32-bit value containing in its 19 LSbs the value of the GPI_DEB_DIS bits, for each GPI pin. These bits      */
/*					are grouped in the three DEBOUNCE_DIS 8-bit registers.                                                          */
/*                                                                                                                                  */
/*                                                                                                                                  */
/*	Errors:                                                                                                                         */
/*                                                                                                                                  */
/*	Description:                                                                                                                    */
/*		This function returns a value containing in its 19 LSbs the value of the GPI_DEB_DIS bits, for each GPI pin. These bits     */
/*		are grouped in the three DEBOUNCE_DIS 8-bit registers.                                                                      */
/*		Note that each GPI_DEB_DIS bit can be accessed using the function GetRegisterBit(IOXP_DEBOUNCE_DIS_GPI_DEB_DIS(bGPIONo)),   */
/*		where bGPIONo is in the 1 - 19 range.                                                                                       */
/* -------------------------------------------------------------------------------------------------------------------------------- */

uint32_t IOXP::GetGPIDebounceDis()
{
	uint32_t bResult = 0;
	ReadBytesI2C(IOXP_ADDR_DEBOUNCE_DIS_A, 3, (uint8_t *)&bResult);
	return bResult;
}

/* ------------------------------------------------------------------------------------------------------------------------------ */
/*	IOXP::SetGPODataOut                                                                                                           */
/*                                                                                                                                */
/*	Synopsis:                                                                                                                     */
/*		SetGPODataOut(uint32_t dwBitMap);                                                                                         */
/*	Parameters:                                                                                                                   */  
/*		uint32_t dwBitMap	- the 32-bit value containing in its 19 LSbs the value of the GPO_DATA bits, for each GPI pin. These  */
/*							  bits are grouped in the three GPO_DATA_OUT 8-bit registers.                                         */
/*                                                                                                                                */
/*  Return Value:                                                                                                                 */
/*		  void                                                                                                                    */
/*                                                                                                                                */
/*	Errors:                                                                                                                       */
/*                                                                                                                                */
/*	Description:                                                                                                                  */
/*		This function sets the value of the GPO_DATA bits from GPO_DATA_OUT registers for all 19 pins by providing a 32-bit value */
/*		containing in its 19 bits the specified values.                                                                           */
/*		Note that each GPO_DATA bit can be accessed using the function SetRegisterBit(IOXP_GPO_DATA_OUT_GPO_DATA(bGPIONo)), where */
/*		bGPIONo is in the 1 - 19 range.                                                                                           */
/* ------------------------------------------------------------------------------------------------------------------------------ */

void IOXP::SetGPODataOut(uint32_t dwBitMap)
{
	WriteBytesI2C(IOXP_ADDR_GPO_DATA_OUT_A, 3, (uint8_t *)&dwBitMap);
}

/* ----------------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetGPODataOut                                                                                                                      */
/*                                                                                                                                           */
/*	Synopsis:                                                                                                                                */
/*		GetGPODataOut()                                                                                                                      */
/*	Parameters:                                                                                                                              */  
/*	 	void                                                                                                                                 */
/*		                                                                                                                                     */
/*  Return Value:                                                                                                                            */
/*		uint32_t - the 32-bit value containing in its 19 LSbs the value of the GPO_DATA bits, for each GPI pin. These bits are grouped in    */
/*				   the three GPO_DATA_OUT 8-bit registers.                                                                                   */
/*                                                                                                                                           */
/*                                                                                                                                           */
/*	Errors:                                                                                                                                  */
/*                                                                                                                                           */
/*	Description:                                                                                                                             */
/*		This function returns a value containing in its 19 LSbs the value of the GPI_DEB_DIS bits, for each GPI pin. These bits are          */
/*		grouped in the three DEBOUNCE_DIS 8-bit registers.                                                                                   */
/*		Note that each GPO_DATA bit can be accessed using the function GetRegisterBit(IOXP_GPO_DATA_OUT_GPO_DATA(bGPIONo)),                  */
/*		where bGPIONo is in the 1 - 19 range.                                                                                                */
/* ----------------------------------------------------------------------------------------------------------------------------------------- */

uint32_t IOXP::GetGPODataOut()
{
	uint32_t bResult = 0;
	ReadBytesI2C(IOXP_ADDR_GPO_DATA_OUT_A, 3, (uint8_t *)&bResult);
	return bResult;
}

/* ---------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetGPOOutMode                                                                                                               */
/*                                                                                                                                    */
/*	Synopsis:                                                                                                                         */
/*		SetGPOOutMode(uint32_t dwBitMap);                                                                                             */
/*	Parameters:                                                                                                                       */  
/*	 	uint32_t dwBitMap	- the 32-bit value containing in its 19 LSbs the value of the GPO_OUT_MODE bits, for each GPI pin.        */
/*								  These bits are grouped in the three GPO_OUT_MODE 8-bit registers.                                   */
/*	                                                                                                                                  */
/*  Return Value:                                                                                                                     */
/*		void                                                                                                                          */
/*                                                                                                                                    */
/*	Errors:                                                                                                                           */
/*                                                                                                                                    */
/*	Description:                                                                                                                      */
/*		This function sets the value of the GPO_OUT_MODE bits from GPO_OUT_MODE registers for all 19 pins by providing a 32-bit value */
/*		containing in its 19 bits the specified values.                                                                               */
/*		Note that each GPO_OUT_MODE bit can be accessed using the function SetRegisterBit(IOXP_GPO_OUT_MODE_GPO_OUT_MODE(bGPIONo)),   */
/*		where bGPIONo is in the 1 - 19 range.                                                                                         */
/* ---------------------------------------------------------------------------------------------------------------------------------- */


void IOXP::SetGPOOutMode(uint32_t dwBitMap)
{
	WriteBytesI2C(IOXP_ADDR_GPO_DATA_OUT_A, 3, (uint8_t *)&dwBitMap);
}

/* ------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetGPOOutMode                                                                                                            */
/*                                                                                                                                 */
/*	Synopsis:                                                                                                                      */
/*		GetGPOOutMode()                                                                                                            */
/*	Parameters:                                                                                                                    */  
/*		void                                                                                                                       */
/*		                                                                                                                           */
/*  Return Value:                                                                                                                  */
/*		 uint32_t - the 32-bit value containing in its 19 LSbs the value of the GPO_OUT_MODE bits, for each GPI pin. These bits    */
/*					are grouped in the three GPO_OUT_MODE 8-bit registers.                                                         */
/*                                                                                                                                 */
/*                                                                                                                                 */
/*	Errors:                                                                                                                        */
/*                                                                                                                                 */
/*	Description:                                                                                                                   */
/*		This function retrieves the value of the GPO_OUT_MODE bits from GPO_OUT_MODE registers for all 19 pins.                    */
/*		Note that each GPO_OUT_MODE bit can be accessed using the function GetRegisterBit(IOXP_GPO_OUT_MODE_GPO_OUT_MODE(bGPIONo)),*/
/*		where bGPIONo is in the 1 - 19 range.                                                                                      */
/* ------------------------------------------------------------------------------------------------------------------------------- */


uint32_t IOXP::GetGPOOutMode()
{
	uint32_t bResult = 0;
	ReadBytesI2C(IOXP_ADDR_GPO_DATA_OUT_A, 3, (uint8_t *)&bResult);
	return bResult;
}

/* ------------------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetGPIODirection                                                                                                                     */
/*                                                                                                                                             */
/*	Synopsis:                                                                                                                                  */
/*		SetGPIODirection(uint32_t dwBitMap);                                                                                                   */
/*	Parameters:                                                                                                                                */  
/*		uint32_t dwBitMap	- the 32-bit value containing in its 19 LSbs the value of the GPIO_DIR bits, for each GPI pin. These bits are      */
/*							  grouped in the three GPIO_DIRECTION 8-bit registers                                                              */
/*                                                                                                                                             */
/*  Return Value:                                                                                                                              */
/*		void                                                                                                                                   */
/*                                                                                                                                             */
/*	Errors:                                                                                                                                    */
/*                                                                                                                                             */
/*	Description:                                                                                                                               */
/*		This function sets the value of the GPIO_DIR bits from GPIO_DIRECTION registers for all 19 pins by providing a 32-bit value containing */
/*		in its 19 bits the specified values.                                                                                                   */
/*		Note that each GPIO_DIR bit can be accessed using the function SetRegisterBit(IOXP_GPIO_DIRECTION_GPIO_DIR(bGPIONo)), where	bGPIONo    */
/*		is in the 1 - 19 range.                                                                                                                */
/* ------------------------------------------------------------------------------------------------------------------------------------------- */

void IOXP::SetGPIODirection(uint32_t dwBitMap)
{
	WriteBytesI2C(IOXP_ADDR_GPIO_DIRECTION_A, 3, (uint8_t *)&dwBitMap);
}

/* ----------------------------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::GetGPIODirection                                                                                                                   */
/*                                                                                                                                           */
/*	Synopsis:                                                                                                                                */
/*		GetGPIODirection();                                                                                                                  */
/*	Parameters:                                                                                                                              */  
/*		void                                                                                                                                 */
/*		                                                                                                                                     */
/*  Return Value:                                                                                                                            */
/*		uint32_t - the 32-bit value containing in its 19 LSbs the value of the GPIO_DIR bits, for each GPI pin. These bits are grouped in    */
/*				   the three GPIO_DIRECTION 8-bit registers                                                                                  */
/*                                                                                                                                           */
/*                                                                                                                                           */
/*	Errors:                                                                                                                                  */
/*                                                                                                                                           */
/*	Description:                                                                                                                             */
/*		This function returns a value containing in its 19 LSbs the value of the GPIO_DIR bits, for each GPI pin. These bits are grouped     */
/*		in the three GPIO_DIRECTION 8-bit registers.                                                                                         */
/*		Note that each GPIO_DIR bit can be accessed using the function GetRegisterBit(IOXP_GPIO_DIRECTION_GPIO_DIR(bGPIONo)), where bGPIONo  */
/*		is in the 1 - 19 range.                                                                                                              */
/* ----------------------------------------------------------------------------------------------------------------------------------------- */

uint32_t IOXP::GetGPIODirection()
{
	uint32_t bResult = 0;
	ReadBytesI2C(IOXP_ADDR_GPIO_DIRECTION_A, 3, (uint8_t *)&bResult);
	return bResult;
}

/* -------------------------------------------------------------------------------------------- */
/*	IOXP::SetResetCfgResetTriggerTime                                                           */
/*                                                                                              */
/*	Synopsis:                                                                                   */
/*		SetResetCfgResetTriggerTime(uint8_t bResetTriggerTime);                                 */
/*	Parameters:                                                                                 */  
/*	 	uint8_t bResetTriggerTime	- the information to be set for RESET_TRIGGER_TIME bits     */
/*					It can be one of the following time values:                                 */
/*					IOXP_RESET_CFG_RESET_TRIGGER_TIME_IMMED		0	// Immediate                */
/*					IOXP_RESET_CFG_RESET_TRIGGER_TIME_1D0SEC	1	// 1.0 sec                  */
/*					IOXP_RESET_CFG_RESET_TRIGGER_TIME_1D5SEC	2	// 1.5 sec                  */
/*					IOXP_RESET_CFG_RESET_TRIGGER_TIME_2D0SEC	3	// 2.0 sec                  */
/*				IOXP_RESET_CFG_RESET_TRIGGER_TIME_2D5SEC	4	// 2.5 sec                      */
/*					IOXP_RESET_CFG_RESET_TRIGGER_TIME_3D0SEC	5	// 3.0 sec                  */
/*					IOXP_RESET_CFG_RESET_TRIGGER_TIME_3D5SEC	6	// 3.5 sec                  */
/*					IOXP_RESET_CFG_RESET_TRIGGER_TIME_4D0SEC	7	// 4.0 sec                  */
/*	                                                                                            */
/*  Return Value:                                                                               */
/*		void                                                                                    */
/*                                                                                              */
/*	Errors:                                                                                     */
/*                                                                                              */
/*	Description:                                                                                */
/*		This function sets the RESET_TRIGGER_TIME information of the RESET_CFG register.        */
/* -------------------------------------------------------------------------------------------- */

void IOXP::SetResetCfgResetTriggerTime(uint8_t bResetTriggerTime)
{
	SetRegisterBitsGroup(IOXP_RESET_CFG_RESET_TRIGGER_TIME, bResetTriggerTime);	
}

/* ----------------------------------------------------------------------------------------- */
/*	IOXP::GetResetCfgResetTriggerTime                                                        */
/*                                                                                           */
/*	Synopsis:                                                                                */
/*		GetResetCfgResetTriggerTime();                                                       */
/*	Parameters:                                                                              */  
/*	 	void                                                                                 */
/*		                                                                                     */
/*  Return Value:                                                                            */
/*		uint8_t -  the information read from RESET_TRIGGER_TIME bits                         */
/* 					It can be one of the following time values:                              */
/* 					IOXP_RESET_CFG_RESET_TRIGGER_TIME_IMMED		0	// Immediate             */
/* 					IOXP_RESET_CFG_RESET_TRIGGER_TIME_1D0SEC	1	// 1.0 sec               */
/* 					IOXP_RESET_CFG_RESET_TRIGGER_TIME_1D5SEC	2	// 1.5 sec               */
/* 					IOXP_RESET_CFG_RESET_TRIGGER_TIME_2D0SEC	3	// 2.0 sec               */
/* 					IOXP_RESET_CFG_RESET_TRIGGER_TIME_2D5SEC	4	// 2.5 sec               */
/* 					IOXP_RESET_CFG_RESET_TRIGGER_TIME_3D0SEC	5	// 3.0 sec               */
/* 					IOXP_RESET_CFG_RESET_TRIGGER_TIME_3D5SEC	6	// 3.5 sec               */
/* 					IOXP_RESET_CFG_RESET_TRIGGER_TIME_4D0SEC	7	// 4.0 sec               */
/*                                                                                           */
/*	Errors:                                                                                  */
/*                                                                                           */
/*	Description:                                                                             */
/*		This function returns the RESET_TRIGGER_TIME information of the RESET_CFG register.  */
/* ----------------------------------------------------------------------------------------- */

uint8_t IOXP::GetResetCfgResetTriggerTime()
{
	return GetRegisterBitsGroup(IOXP_RESET_CFG_RESET_PULSE_WIDTH);	
}

/* --------------------------------------------------------------------------------------- */
/*	IOXP::SetResetCfgResetPulseWidth                                                       */
/*                                                                                         */
/*	Synopsis:                                                                              */
/*		SetResetCfgResetPulseWidth(uint8_t bResetPulseWidth);                              */
/*	Parameters:                                                                            */  
/*	 	uint8_t bResetPulseWidth	- the information to be set for RESET_PULSE_WIDTH bits */
/*					It can be one of the following time values:                            */
/*						IOXP_RESET_CFG_RESET_PULSE_WIDTH_500US		0	- 500 us           */
/*                      IOXP_RESET_CFG_RESET_PULSE_WIDTH_1MS		1	- 1 ms             */
/*                    	 IOXP_RESET_CFG_RESET_PULSE_WIDTH_2MS		2	- 2 ms             */
/*                      IOXP_RESET_CFG_RESET_PULSE_WIDTH_10MS		3	- 10 ms            */
/*		                                                                                   */
/*  Return Value:                                                                          */
/*		void                                                                               */
/*                                                                                         */
/*	Errors:                                                                                */
/*                                                                                         */
/*	Description:                                                                           */
/*		This function sets the RESET_PULSE_WIDTH information of the RESET_CFG register.    */
/* --------------------------------------------------------------------------------------- */

void IOXP::SetResetCfgResetPulseWidth(uint8_t bResetPulseWidth)
{
	SetRegisterBitsGroup(IOXP_RESET_CFG_RESET_PULSE_WIDTH, bResetPulseWidth);	
}

/* ------------------------------------------------------------------------------------------- */
/*	IOXP::GetResetCfgResetPulseWidth                                                           */
/*                                                                                             */
/*	Synopsis:                                                                                  */
/*		GetResetCfgResetPulseWidth(uint8_t bResetPulseWidth)                                   */
/*	Parameters:                                                                                */  
/*	 	 uint8_t bResetPulseWidth	- the information read from RESET_PULSE_WIDTH bits         */
/*					It can be one of the following time values:                                */
/*						IOXP_RESET_CFG_RESET_PULSE_WIDTH_500US		0	- 500 us               */
/*                      IOXP_RESET_CFG_RESET_PULSE_WIDTH_1MS		1	- 1 ms                 */
/*                      IOXP_RESET_CFG_RESET_PULSE_WIDTH_2MS		2	- 2 ms                 */
/*                      IOXP_RESET_CFG_RESET_PULSE_WIDTH_10MS		3	- 10 ms		           */			
/*                                                                                             */
/*  Return Value:                                                                              */
/*		  uint8_t - returns RESET_PULSE_WIDTH information of the RESET_CFG register            */
/*                                                                                             */
/*	Errors:                                                                                    */
/*                                                                                             */
/*	Description:                                                                               */
/*		 This function returns the RESET_PULSE_WIDTH information of the RESET_CFG register.    */
/* ------------------------------------------------------------------------------------------- */

uint8_t IOXP::GetResetCfgResetPulseWidth(uint8_t bResetPulseWidth)
{
	return GetRegisterBitsGroup(IOXP_RESET_CFG_RESET_PULSE_WIDTH);	
}

/* -------------------------------------------------------------------------------------------------------------------- */
/*	IOXP::SetPWMOffTimeUS                                                                                               */
/*                                                                                                                      */
/*	Synopsis:                                                                                                           */
/*		 SetPWMOffTimeUS(uint16_t wOffTime);                                                                            */
/*	Parameters:                                                                                                         */  
/*	 	 uint16_t wOffTime	- the 16 bits value containing the off time (in us)                                         */
/*                                                                                                                      */
/*  Return Value:                                                                                                       */
/*		  void                                                                                                          */
/*                                                                                                                      */
/*	Errors:                                                                                                             */
/*                                                                                                                      */
/*	Description:                                                                                                        */
/*		This function sets the PWM off time (expressed in micro seconds) by providing a 16 bits value corresponding to  */
/*		PWM_OFFT_LOW and PWM_OFFT_HIGH registers                                                                        */
/* -------------------------------------------------------------------------------------------------------------------- */

void IOXP::SetPWMOffTimeUS(uint16_t wOffTime)
{
	WriteBytesI2C(IOXP_ADDR_PWM_OFFT_LOW, 2, (uint8_t *)&wOffTime);
}

/* ------------------------------------------------------------------------------ */
/*	IOXP::GetPWMOffTimeUS                                                         */
/*                                                                                */
/*	Synopsis:                                                                     */
/*		GetPWMOffTimeUS();                                                        */       
/*	Parameters:                                                                   */  
/*		void                                                                      */
/*		                                                                          */
/*  Return Value:                                                                 */
/*		uint16_t - the 16 bits value containing the off time (in us)              */
/*                                                                                */
/*                                                                                */
/*	Errors:                                                                       */
/*                                                                                */
/*	Description:                                                                  */
/*		This function returns the PWM off time (expressed in micro seconds) as a  */
/*		16 bits value contained in PWM_OFFT_LOW and PWM_OFFT_HIGH registers.      */
/* ------------------------------------------------------------------------------ */


uint16_t IOXP::GetPWMOffTimeUS()
{
	uint16_t wResult;
	ReadBytesI2C(IOXP_ADDR_PWM_OFFT_LOW, 2, (uint8_t *)&wResult);
	return wResult;
}

/* -------------------------------------------------------------------------------------------------- */
/*	IOXP::SetPWMOnTimeUS                                                                              */
/*                                                                                                    */
/*	Synopsis:                                                                                         */
/*		SetPWMOnTimeUS(uint16_t wOnTime)                                                              */
/*	Parameters:                                                                                       */  
/*		uint16_t wOnTime	- the 16 bits value containing the on time (in us)                        */
/*                                                                                                    */
/*  Return Value:                                                                                     */
/*		void                                                                                          */
/*                                                                                                    */
/*                                                                                                    */
/*	Errors:                                                                                           */
/*                                                                                                    */
/*	Description:                                                                                      */
/*		This function sets the PWM on time (expressed in micro seconds) by providing a 16 bits value  */
/*		corresponding to PWM_ONT_LOW and PWM_ONT_HIGH registers                                       */
/* -------------------------------------------------------------------------------------------------- */


void IOXP::SetPWMOnTimeUS(uint16_t wOnTime)
{
	WriteBytesI2C(IOXP_ADDR_PWM_ONT_LOW, 2, (uint8_t *)&wOnTime);
}

/* ----------------------------------------------------------------------------- */
/*	IOXP::GetPWMOnTimeUS                                                         */
/*                                                                               */
/*	Synopsis:                                                                    */
/*		GetPWMOnTimeUS()                                                         */
/*	Parameters:                                                                  */  
/*		void                                                                     */
/*		                                                                         */
/*  Return Value:                                                                */
/*		uint16_t - the 16 bits value containing the on time (in us)              */
/*                                                                               */
/*	Errors:                                                                      */
/*                                                                               */
/*	Description:                                                                 */
/*		This function returns the PWM on time (expressed in micro seconds)       */
/*		as a 16 bits value contained in PWM_ONT_LOW and PWM_ONT_HIGH registers.  */
/* ----------------------------------------------------------------------------- */


uint16_t IOXP::GetPWMOnTimeUS()
{
	uint16_t wResult;
	ReadBytesI2C(IOXP_ADDR_PWM_ONT_LOW, 2, (uint8_t *)&wResult);
	return wResult;
}

/* --------------------------------------------------------------------------------------------- */
/*	IOXP::SetClkDivClkDiv                                                                        */
/*                                                                                               */
/*	Synopsis:                                                                                    */
/*		SetClkDivClkDiv(uint8_t bClkDiv);                                                        */
/*	Parameters:                                                                                  */  
/*		uint8_t bClkDiv	- the information to be set for CLK_DIV                                  */
/*				It can be one of the following:                                                  */
/*					IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV1		(0X00)	// divide by 1 (pass-through).   */
/*                  IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV2		(0X01)	// divide by 2.                  */
/*                  IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV3		(0X02)	// divide by 3.                  */
/*                    IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV4		(0X03)	// divide by 4.              */
/*					IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV32	(0X1F)	// divide by 32.                 */
/*                                                                                               */
/*  Return Value:                                                                                */
/*		void                                                                                     */
/*                                                                                               */
/*	Errors:                                                                                      */
/*                                                                                               */
/*	Description:                                                                                 */
/*		This function sets the CLK_DIV information of the CLOCK_DIV_CFG register.                */
/* --------------------------------------------------------------------------------------------- */


void IOXP::SetClkDivClkDiv(uint8_t bClkDiv)
{
	SetRegisterBitsGroup(IOXP_CLOCK_DIV_CFG_CLK_DIV, bClkDiv);	
}


/* ----------------------------------------------------------------------------------------------- */
/*	IOXP::GetClkDivClkDiv                                                                          */
/*                                                                                                 */
/*	Synopsis:                                                                                      */
/*		GetClkDivClkDiv()                                                                          */
/*	Parameters:                                                                                    */  
/*	 	 void                                                                                      */
/*		                                                                                           */
/*		 uint8_t -  the information read from CLK_DIV                                              */
/*					It can be one of the following:                                                */
/*					    IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV1		(0X00)	// divide by 1 (pass-through). */
/*                      IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV2		(0X01)	// divide by 2.                */
/*                      IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV3		(0X02)	// divide by 3.                */
/*                      IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV4		(0X03)	// divide by 4.                */
/*                      IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV32	(0X1F)	// divide by 32.               */
/*                                                                                                 */
/*                                                                                                 */
/*	Errors:                                                                                        */
/*                                                                                                 */
/*	Description:                                                                                   */
/*		This function returns the CLK_DIV information of the CLOCK_DIV_CFG register.               */
/* ----------------------------------------------------------------------------------------------- */


uint8_t IOXP::GetClkDivClkDiv()
{
	return GetRegisterBitsGroup(IOXP_CLOCK_DIV_CFG_CLK_DIV);
}


/* ------------------------------------------------------------------------------ */
/*	IOXP::SetLogicSel1                                                            */
/*                                                                                */
/*	Synopsis:                                                                     */
/*		SetLogicSel1(uint8_t bLogicSel)                                           */
/*	Parameters:                                                                   */  
/*		uint8_t bLogicSel - the information to be set for LOGIC1_SEL              */
/*			It can be one of the following:                                       */
/*				IOXP_LOGIC_CFG_LOGIC_SEL_OFF		(0x00)	// off/disable        */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_AND		(0x01)	// AND                */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_OR			(0x02)	// OR                 */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_XOR		(0x03)	// XOR                */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_FF			(0x04)	// FF                 */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_IN_LA		(0x05)	// IN_LA              */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_IN_LB		(0x06)	// IN_LB              */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_IN_LC		(0x07)	// IN_LC              */
/*                                                                                */
/*  Return Value:                                                                 */
/*		void                                                                      */
/*                                                                                */
/*	Errors:                                                                       */
/*                                                                                */
/*	Description:                                                                  */
/*		This function sets the LOGIC1_SEL information of the LOGIC1_CFG register. */
/* ------------------------------------------------------------------------------ */

void IOXP::SetLogicSel1(uint8_t bLogicSel)
{
	SetRegisterBitsGroup(IOXP_LOGIC_1_CFG_LOGIC1_SEL, bLogicSel);	
}

/* ----------------------------------------------------------------------------------- */
/*	IOXP::GetLogicSel1                                                                 */
/*                                                                                     */
/*	Synopsis:                                                                          */
/*		  GetLogicSel1();                                                              */
/*	Parameters:                                                                        */  
/*	 	  void                                                                         */
/*		                                                                               */
/*  Return Value:                                                                      */
/*		uint8_t -  the information read from LOGIC1_SEL                                */
/*			It can be one of the following:                                            */
/*				IOXP_LOGIC_CFG_LOGIC_SEL_OFF		(0x00)	// off/disable             */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_AND		(0x01)	// AND                     */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_OR			(0x02)	// OR                      */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_XOR		(0x03)	// XOR                     */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_FF			(0x04)	// FF                      */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_IN_LA		(0x05)	// IN_LA                   */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_IN_LB		(0x06)	// IN_LB                   */
/*              IOXP_LOGIC_CFG_LOGIC_SEL_IN_LC		(0x07)	// IN_LC                   */
/*                                                                                     */
/*                                                                                     */
/*	Errors:                                                                            */
/*                                                                                     */
/*	Description:                                                                       */
/*		  This function returns the LOGIC1_SEL information of the LOGIC1_CFG register. */
/* ----------------------------------------------------------------------------------- */

uint8_t IOXP::GetLogicSel1()
{
	return GetRegisterBitsGroup(IOXP_LOGIC_1_CFG_LOGIC1_SEL);
}

/* ------------------------------------------------------------------------------ */
/*	IOXP::SetLogicSel2                                                            */
/*                                                                                */
/*	Synopsis:                                                                     */
/*		SetLogicSel2(uint8_t bLogicSel);                                          */
/*	Parameters:                                                                   */  
/*		uint8_t bLogicSel - the information to be set for LOGIC2_SEL              */
/*				It can be one of the following:                                   */
/*					IOXP_LOGIC_CFG_LOGIC_SEL_OFF		(0x00)	// off/disable    */
/*                  IOXP_LOGIC_CFG_LOGIC_SEL_AND		(0x01)	// AND            */
/*                  IOXP_LOGIC_CFG_LOGIC_SEL_OR			(0x02)	// OR             */
/*                  IOXP_LOGIC_CFG_LOGIC_SEL_XOR		(0x03)	// XOR            */
/*                 IOXP_LOGIC_CFG_LOGIC_SEL_FF			(0x04)	// FF             */
/*                  IOXP_LOGIC_CFG_LOGIC_SEL_IN_LA		(0x05)	// IN_LA          */
/*                  IOXP_LOGIC_CFG_LOGIC_SEL_IN_LB		(0x06)	// IN_LB          */
/*                  IOXP_LOGIC_CFG_LOGIC_SEL_IN_LC		(0x07)	// IN_LC          */
/*                                                                                */
/*  Return Value:                                                                 */
/*		void                                                                      */
/*                                                                                */
/*	Errors:                                                                       */
/*                                                                                */
/*	Description:                                                                  */
/*		This function sets the LOGIC2_SEL information of the LOGIC2_CFG register. */
/* ------------------------------------------------------------------------------ */

void IOXP::SetLogicSel2(uint8_t bLogicSel)
{
	SetRegisterBitsGroup(IOXP_LOGIC_2_CFG_LOGIC2_SEL, bLogicSel);	
}

/* ----------------------------------------------------------------------------------- */
/*	IOXP::GetLogicSel2                                                                 */
/*                                                                                     */
/*	Synopsis:                                                                          */
/*		GetLogicSel2();                                                                */
/*	Parameters:                                                                        */  
/*	 	void                                                                           */
/*		                                                                               */
/*  Return Value:                                                                      */
/*		uint8_t -  the information read from LOGIC2_SEL                                */
/*				It can be one of the following:                                        */
/*		    		IOXP_LOGIC_CFG_LOGIC_SEL_OFF		(0x00)	// off/disable         */
/*                  IOXP_LOGIC_CFG_LOGIC_SEL_AND		(0x01)	// AND                 */
/*                  IOXP_LOGIC_CFG_LOGIC_SEL_OR			(0x02)	// OR                  */
/*                  IOXP_LOGIC_CFG_LOGIC_SEL_XOR		(0x03)	// XOR                 */
/*                  IOXP_LOGIC_CFG_LOGIC_SEL_FF			(0x04)	// FF                  */
/*                  IOXP_LOGIC_CFG_LOGIC_SEL_IN_LA		(0x05)	// IN_LA               */
/*                   IOXP_LOGIC_CFG_LOGIC_SEL_IN_LB		(0x06)	// IN_LB               */
/*                  IOXP_LOGIC_CFG_LOGIC_SEL_IN_LC		(0x07)	// IN_LC               */
/*                                                                                     */
/*	Errors:                                                                            */
/*                                                                                     */
/*	Description:                                                                       */
/*		This function returns the LOGIC2_SEL information of the LOGIC2_CFG register.   */
/* ----------------------------------------------------------------------------------- */

uint8_t IOXP::GetLogicSel2()
{
	return GetRegisterBitsGroup(IOXP_LOGIC_2_CFG_LOGIC2_SEL);
}

/* ---------------------------------------------------------------------------------- */
/* 	IOXP::SetPollTime                                                                 */
/*                                                                                    */
/*	Synopsis:                                                                         */
/*		SetPollTime(uint8_t bPollTime)                                                */
/*	Parameters:                                                                       */  
/*	 	 uint8_t bPollTime	- the information to be set for POLL_TIME bits.           */
/*				It can be one of the following time values::                          */
/*					IOXP_POLL_TIME_CFG_KEY_POLL_TIME_10MS	(0x00)	- 10 ms           */
/*					IOXP_POLL_TIME_CFG_KEY_POLL_TIME_20MS	(0x01)	- 20 ms           */
/*					IOXP_POLL_TIME_CFG_KEY_POLL_TIME_30MS	(0x02)	- 30 ms           */
/*					IOXP_POLL_TIME_CFG_KEY_POLL_TIME_40MS	(0x03)	- 40 ms           */                       
/*		                                                                              */
/*  Return Value:                                                                     */
/*		 void                                                                         */
/*                                                                                    */
/*	Errors:                                                                           */
/*                                                                                    */
/*	Description:                                                                      */
/*		 This function sets the POLL_TIME information of the POLL_TIME_CFG register.  */
/* ---------------------------------------------------------------------------------- */

void IOXP::SetPollTime(uint8_t bPollTime)
{
	SetRegisterBitsGroup(IOXP_POLL_TIME_CFG_KEY_POLL_TIME, bPollTime);	
}

/* ----------------------------------------------------------------------------------- */
/*	IOXP::GetPollTime                                                                  */
/*                                                                                     */
/*	Synopsis:                                                                          */
/*		GetPollTime();                                                                 */
/*	Parameters:                                                                        */  
/*		void                                                                           */
/*		                                                                               */
/*  Return Value:                                                                      */
/*		uint8_t -  the information read from POLL_TIME bits                            */
/*                                                                                     */
/*                                                                                     */
/*	Errors:                                                                            */
/*                                                                                     */
/*	Description:                                                                       */
/*		This function returns the POLL_TIME information of the POLL_TIME_CFG register. */
/* ----------------------------------------------------------------------------------- */

uint8_t IOXP::GetPollTime()
{
	return GetRegisterBitsGroup(IOXP_POLL_TIME_CFG_KEY_POLL_TIME);
}

/* ----------------------------------------------------------------------------------- */
/*	IOXP::SetCoreFreq                                                                  */
/*                                                                                     */
/*	Synopsis:                                                                          */
/*		SetCoreFreq(uint8_t bCoreFreq);                                                */
/*	Parameters:                                                                        */  
/*		uint8_t bCoreFreq	- the information to be set for CORE_FREQ bits             */
/*			It can be one of the following:                                            */
/*				IOXP_GENERAL_CFG_B_CORE_FREQ_50K		(0x00)	-  50 kHz              */
/*				IOXP_GENERAL_CFG_B_CORE_FREQ_100K		(0x01)	- 100 kHz              */
/*				IOXP_GENERAL_CFG_B_CORE_FREQ_200K		(0x02)	- 200 kHz              */
/*				IOXP_GENERAL_CFG_B_CORE_FREQ_500K	    (0x03)	- 500 kHz              */
/*                                                                                     */
/*  Return Value:                                                                      */
/*		  void                                                                         */
/*                                                                                     */
/*	Errors:                                                                            */
/*                                                                                     */
/*	Description:                                                                       */
/*		 This function sets the CORE_FREQ information of the GENERAL_CFG_B register.   */
/* ----------------------------------------------------------------------------------- */

void IOXP::SetCoreFreq(uint8_t bCoreFreq)
{
	SetRegisterBitsGroup(IOXP_GENERAL_CFG_B_CORE_FREQ, bCoreFreq);	
}

/* ------------------------------------------------------------------------------------- */
/*	IOXP::GetCoreFreq                                                                    */
/*                                                                                       */
/*	Synopsis:                                                                            */
/*		GetCoreFreq();                                                                   */
/*	Parameters:                                                                          */  
/*	 	  void                                                                           */
/*		                                                                                 */
/*  Return Value:                                                                        */ 
/*		  uint8_t - the information read from CORE_FREQ bits.                            */
/*				It can be one of the following:                                          */
/*					IOXP_GENERAL_CFG_B_CORE_FREQ_50K		(0x00)	-  50 kHz            */
/*					IOXP_GENERAL_CFG_B_CORE_FREQ_100K		(0x01)	- 100 kHz            */
/*					IOXP_GENERAL_CFG_B_CORE_FREQ_200K		(0x02)	- 200 kHz            */
/*					IOXP_GENERAL_CFG_B_CORE_FREQ_500K		(0x03)	- 500 kHz            */
/*                                                                                       */
/*                                                                                       */
/*	Errors:                                                                              */
/*                                                                                       */
/*	Description:                                                                         */
/*		 This function returns the CORE_FREQ information of the GENERAL_CFG_B register.  */
/* ------------------------------------------------------------------------------------- */

uint8_t IOXP::GetCoreFreq()
{
	return GetRegisterBitsGroup(IOXP_GENERAL_CFG_B_CORE_FREQ);
}

/* ---------------------------------------------------------------------------------------------- */
/*	IOXP::SetKeyMap                                                                               */
/*                                                                                                */
/*	Synopsis:                                                                                     */
/*		SetKeyMap(int table[IOXP_KB_ROWS][IOXP_KB_COLS]);                                         */
/*	Parameters:                                                                                   */  
/*		int table[IOXP_KB_ROWS][IOXP_KB_COLS]	- the table containing key mappings               */
/*						                                                                          */
/*  Return Value:                                                                                 */
/*		void                                                                                      */
/*                                                                                                */
/*	Errors:                                                                                       */
/*                                                                                                */
/*	Description:                                                                                  */
/*		This function sets the value for keys using the values from specified table.              */
/*		Mapping values on keys is optional, user may use keys just by specifying row and column.  */
/* ---------------------------------------------------------------------------------------------- */

void IOXP::SetKeyMap(int table[IOXP_KB_ROWS][IOXP_KB_COLS])
{  
	for(int idxRows = 0 ; idxRows < IOXP_KB_ROWS ; idxRows++)       
	{
		for(int idxCols = 0 ; idxCols < IOXP_KB_COLS ; idxCols++)        
		{
			this->keyMap[idxRows][idxCols] = table[idxRows][idxCols];
		}
	}   
}