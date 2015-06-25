/************************************************************************/
/*																		*/
/*	IOXP.h		--		Definition for IOXP library      	    		*/
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
#if !defined(IOXP_H)
#define IOXP_H

/* -------------------------------------------------------------------- */
/*				Include File Definitions						        */
/* -------------------------------------------------------------------- */
#include <inttypes.h>
#include <WProgram.h>
/* -------------------------------------------------------------------- */
/*					Definitions									        */
/* -------------------------------------------------------------------- */
#define IOXP_I2C_ADDR		0x34	// ADP5589 IIC Address
#define IOXP_KB_ROWS		8
#define IOXP_KB_COLS		11
#define IOXP_GPIOS			IOXP_KB_ROWS + IOXP_KB_COLS
#define IOXP_NO_LOGIC		2


/* -------------------------------------------------------------------- */
/*		Register addresses Definitions							        */
/* -------------------------------------------------------------------- */

// Register address
#define IOXP_ADDR_ID               	0x00
#define IOXP_ADDR_INT_STATUS       	0x01
#define IOXP_ADDR_STATUS           	0x02                            
#define IOXP_ADDR_FIFO1            	0x03
#define IOXP_ADDR_FIFO2            	0x04
#define IOXP_ADDR_FIFO3            	0x05
#define IOXP_ADDR_FIFO4            	0x06
#define IOXP_ADDR_FIFO5            	0x07
#define IOXP_ADDR_FIFO6            	0x08
#define IOXP_ADDR_FIFO7            	0x09
#define IOXP_ADDR_FIFO8            	0x0A
#define IOXP_ADDR_FIFO9            	0x0B
#define IOXP_ADDR_FIFO10           	0x0C
#define IOXP_ADDR_FIFO11           	0x0D
#define IOXP_ADDR_FIFO12           	0x0E
#define IOXP_ADDR_FIFO13           	0x0F
#define IOXP_ADDR_FIFO14           	0x10
#define IOXP_ADDR_FIFO15           	0x11
#define IOXP_ADDR_FIFO16           	0x12
#define IOXP_ADDR_GPI_INT_STATUS_A   	0x13
#define IOXP_ADDR_GPI_INT_STATUS_B   	0x14
#define IOXP_ADDR_GPI_INT_STATUS_C   	0x15
#define IOXP_ADDR_GPI_STATUS_A       	0x16
#define IOXP_ADDR_GPI_STATUS_B       	0x17
#define IOXP_ADDR_GPI_STATUS_C       	0x18
#define IOXP_ADDR_RPULL_CONFIG_A   	0x19
#define IOXP_ADDR_RPULL_CONFIG_B   	0x1A
#define IOXP_ADDR_RPULL_CONFIG_C   	0x1B
#define IOXP_ADDR_RPULL_CONFIG_D   	0x1C
#define IOXP_ADDR_RPULL_CONFIG_E		0x1D
#define IOXP_ADDR_GPI_INT_LEVEL_A	0x1E
#define IOXP_ADDR_GPI_INT_LEVEL_B	0x1F
#define IOXP_ADDR_GPI_INT_LEVEL_C    0x20
#define IOXP_ADDR_GPI_EVENT_EN_A     0x21
#define IOXP_ADDR_GPI_EVENT_EN_B     0x22
#define IOXP_ADDR_GPI_EVENT_EN_C     0x23
#define IOXP_ADDR_GPI_INTERRUPT_EN_A 0x24
#define IOXP_ADDR_GPI_INTERRUPT_EN_B 0x25
#define IOXP_ADDR_GPI_INTERRUPT_EN_C 0x26
#define IOXP_ADDR_DEBOUNCE_DIS_A     0x27
#define IOXP_ADDR_DEBOUNCE_DIS_B     0x28
#define IOXP_ADDR_DEBOUNCE_DIS_C     0x29
#define IOXP_ADDR_GPO_DATA_OUT_A     0x2A
#define IOXP_ADDR_GPO_DATA_OUT_B     0x2B
#define IOXP_ADDR_GPO_DATA_OUT_C     0x2C
#define IOXP_ADDR_GPO_OUT_MODE_A     0x2D
#define IOXP_ADDR_GPO_OUT_MODE_B     0x2E
#define IOXP_ADDR_GPO_OUT_MODE_C     0x2F
#define IOXP_ADDR_GPIO_DIRECTION_A 	0x30
#define IOXP_ADDR_GPIO_DIRECTION_B   0x31
#define IOXP_ADDR_GPIO_DIRECTION_C	0x32
#define IOXP_ADDR_UNLOCK1          	0x33
#define IOXP_ADDR_UNLOCK2          	0x34
#define IOXP_ADDR_EXT_LOCK_EVENT   	0x35
#define IOXP_ADDR_UNLOCK_TIMERS		0x36
#define IOXP_ADDR_LOCK_CFG          	0x37
#define IOXP_ADDR_RESET1_EVENT_A     0x38
#define IOXP_ADDR_RESET1_EVENT_B     0x39
#define IOXP_ADDR_RESET1_EVENT_C     0x3A
#define IOXP_ADDR_RESET2_EVENT_A     0x3B
#define IOXP_ADDR_RESET2_EVENT_B     0x3C
#define IOXP_ADDR_RESET_CFG         	0x3D
#define IOXP_ADDR_PWM_OFFT_LOW       0x3E
#define IOXP_ADDR_PWM_OFFT_HIGH      0x3F
#define IOXP_ADDR_PWM_ONT_LOW        0x40
#define IOXP_ADDR_PWM_ONT_HIGH       0x41
#define IOXP_ADDR_PWM_CFG           	0x42
#define IOXP_ADDR_CLOCK_DIV_CFG      0x43
#define IOXP_ADDR_LOGIC_1_CFG        0x44
#define IOXP_ADDR_LOGIC_2_CFG        0x45
#define IOXP_ADDR_LOGIC_FF_CFG       0x46
#define IOXP_ADDR_LOGIC_INT_EVENT    0x47
#define IOXP_ADDR_POLL_TIME_CFG      0x48
#define IOXP_ADDR_PIN_CONFIG_A       0x49
#define IOXP_ADDR_PIN_CONFIG_B       0x4A
#define IOXP_ADDR_PIN_CONFIG_C       0x4B
#define IOXP_ADDR_PIN_CONFIG_D       0x4C
#define IOXP_ADDR_GENERAL_CFG_B      0x4D     
#define IOXP_ADDR_INT_EN            	0x4E 

/* -------------------------------------------------------------------- */
/*		Register Bit Mask Definitions - single bits				        */
/* -------------------------------------------------------------------- */

#define div8(a) (a >> 3)
#define mod8(a) (a - (div8(a) << 3))
#define div4(a) (a >> 2)
#define mod4(a) (a - (div4(a) << 2))

#define IOXP_INT_STATUS_EVENT_INT   		(0x0101)	// EVENT_INT bit of the INT_STATUS register
#define IOXP_INT_STATUS_GPI_INT     		(0x0102)	// GPI_INT bit of the INT_STATUS register
#define IOXP_INT_STATUS_OVERFLOW_INT 	 	(0x0104)	// OVERFLOW_INT bit of the INT_STATUS register
#define IOXP_INT_STATUS_LOCK_INT   		  	(0x0108)	// LOCK_INT bit of the INT_STATUS register
#define IOXP_INT_STATUS_LOGIC1_INT  		(0x0110)	// LOGIC1_INT bit of the INT_STATUS register
#define IOXP_INT_STATUS_LOGIC2_INT  		(0x0120)	// LOGIC2_INT bit of the INT_STATUS register
#define IOXP_STATUS_LOCK_STAT  				(0x0220)	// LOCK_STAT bit of the Status register
#define IOXP_STATUS_LOGIC1_STAT  			(0x0240)	// LOGIC1_STAT bit of the Status register
#define IOXP_STATUS_LOGIC2_STAT  	 		(0x0280)	// LOGIC2_STAT bit of the Status register
#define IOXP_UNLOCK1_UNLOCK1_STATE	  		(0x3340)	// UNLOCK1_STATE bit of the UNLOCK1 register
#define IOXP_UNLOCK2_UNLOCK2_STATE	  		(0x3440)	// UNLOCK2_STATE bit of the UNLOCK2 register
#define IOXP_EXT_LOCK_EVENT_EXT_LOCK_STATE	(0x3540)	// EXT_LOCK_STATE bit of the EXT_LOCK register
#define IOXP_LOCK_CFG_LOCK_EN	  			(0x3701)	// LOCK_EN bit of the LOCK_CFG register			
#define IOXP_RESET1_EVENT_A_RESET1_EVENT_A_LEVEL	(0x3880)	// RESET1_EVENT_A_LEVEL bit of the RESET1_EVENT_A register	
#define IOXP_RESET1_EVENT_B_RESET1_EVENT_B_LEVEL	(0x3980)	// RESET1_EVENT_B_LEVEL bit of the RESET1_EVENT_B register
#define IOXP_RESET1_EVENT_C_RESET1_EVENT_C_LEVEL	(0x3A80)	// RESET1_EVENT_C_LEVEL bit of the RESET1_EVENT_C register
#define IOXP_RESET2_EVENT_A_RESET2_EVENT_A_LEVEL	(0x3B80)	// RESET2_EVENT_A_LEVEL bit of the RESET2_EVENT_A register
#define IOXP_RESET2_EVENT_B_RESET2_EVENT_B_LEVEL	(0x3C80)	// RESET2_EVENT_B_LEVEL bit of the RESET2_EVENT_B register
#define IOXP_RESET_CFG_RST_PASSTHRU_EN		(0x3D20)	// RST_PASSTHRU_EN bit of the RESET_CFG register
#define IOXP_RESET_CFG_RESET1_POL			(0x3D40)	// RESET1_POL bit of the RESET_CFG register
#define IOXP_RESET_CFG_RESET2_POL			(0x3D80)	// RESET2_POL bit of the RESET_CFG register
#define IOXP_PWM_CFG_PWM_EN					(0x4201)	// PWM_EN bit of the PWM_CFG register
#define IOXP_PWM_CFG_PWM_MODE				(0x4202)	// PWM_MODE bit of the PWM_CFG register
#define IOXP_PWM_CFG_PWM_IN_AND				(0x4204)	// PWM_IN_AND bit of the PWM_CFG register
#define IOXP_CLOCK_DIV_CFG_CLK_INV			(0X4340)	// CLK_INV bit of the CLOCK_DIV_CFG register
#define IOXP_CLOCK_DIV_CFG_CLK_DIV_EN		(0X4301)	// DIV_EN bit of the CLOCK_DIV_CFG register
#define IOXP_LOGIC_1_LA1_INV				(0x4408)	// LA1_INV bit of the LOGIC_1 register
#define IOXP_LOGIC_1_LB1_INV				(0x4410)	// LB1_INV bit of the LOGIC_1 register
#define IOXP_LOGIC_1_LC1_INV				(0x4420)	// LC1_INV bit of the LOGIC_1 register
#define IOXP_LOGIC_1_LY1_INV				(0x4440)	// LY1_INV bit of the LOGIC_1 register
#define IOXP_LOGIC_2_LA2_INV				(0x4508)	// LA2_INV bit of the LOGIC_2 register
#define IOXP_LOGIC_2_LB2_INV				(0x4510)	// LB2_INV bit of the LOGIC_2 register
#define IOXP_LOGIC_2_LC2_INV				(0x4520)	// LC2_INV bit of the LOGIC_2 register
#define IOXP_LOGIC_2_LY2_INV				(0x4540)	// LY2_INV bit of the LOGIC_2 register
#define IOXP_LOGIC_2_LY1_CASCADE			(0x4540)	// LY1_CASCADE bit of the LOGIC_2 register
#define IOXP_LOGIC_FF_CFG_FF1_CLR			(0x4601)	// FF1_CLR bit of the LOGIC_FF_CFG register
#define IOXP_LOGIC_FF_CFG_FF1_SET			(0x4602)	// FF1_SET bit of the LOGIC_FF_CFG register
#define IOXP_LOGIC_FF_CFG_FF2_CLR			(0x4604)	// FF2_CLR bit of the LOGIC_FF_CFG register
#define IOXP_LOGIC_FF_CFG_FF2_SET			(0x4608)	// FF2_SET bit of the LOGIC_FF_CFG register
#define IOXP_LOGIC_INT_EVENT_EN_LOGIC1_INT_LEVEL	(0x4701)	// LOGIC1_INT_LEVEL bit of the LOGIC_INT_EVENT_EN register
#define IOXP_LOGIC_INT_EVENT_EN_LOGIC1_EVENT_EN		(0x4702)	// LOGIC1_EVENT_EN bit of the LOGIC_INT_EVENT_EN register
#define IOXP_LOGIC_INT_EVENT_EN_LY1_DBNC_DIS		(0x4704)	// LY1_DBNC_DIS bit of the LOGIC_INT_EVENT_EN register
#define IOXP_LOGIC_INT_EVENT_EN_LOGIC2_INT_LEVEL	(0x4708)	// LOGIC2_INT_LEVEL bit of the LOGIC_INT_EVENT_EN register
#define IOXP_LOGIC_INT_EVENT_EN_LOGIC2_EVENT_EN		(0x4710)	// LOGIC2_EVENT_EN bit of the LOGIC_INT_EVENT_EN register
#define IOXP_LOGIC_INT_EVENT_EN_LY2_DBNC_DIS		(0x4720)	// LY2_DBNC_DIS bit of the LOGIC_INT_EVENT_EN register
#define IOXP_PIN_CONFIG_D_R0_EXTEND  		(0x4C01)	// R0_EXTEND bit of the PIN_CONFIG_D register
#define IOXP_PIN_CONFIG_D_C9_EXTEND  		(0x4C02)	// C9_EXTEND bit of the PIN_CONFIG_D register
#define IOXP_PIN_CONFIG_D_R3_EXTEND_0	  	(0x4C04)	// R3_EXTEND_0 bit of the PIN_CONFIG_D register
#define IOXP_PIN_CONFIG_D_R3_EXTEND_1	  	(0x4C08)	// R3_EXTEND_1 bit of the PIN_CONFIG_D register
#define IOXP_PIN_CONFIG_D_C6_EXTEND	  		(0x4C10)	// C6_EXTEND bit of the PIN_CONFIG_D register
#define IOXP_PIN_CONFIG_D_R4_EXTEND	  		(0x4C20)	// R4_EXTEND bit of the PIN_CONFIG_D register
#define IOXP_PIN_CONFIG_D_C4_EXTEND	  		(0x4C40)	// C4_EXTEND bit of the PIN_CONFIG_D register
#define IOXP_PIN_CONFIG_D_PULL_SELECT  		(0x4C80)	// PULL_SELECT bit of the PIN_CONFIG_D register
#define IOXP_GENERAL_CFG_B_RST_CFG   		(0x4D01)	// RST_CFG bit of the GENERAL_CFG_B register
#define IOXP_GENERAL_CFG_B_INT_CFG     		(0x4D02)	// INT_CFG bit of the GENERAL_CFG_B register
#define IOXP_GENERAL_CFG_B_LCK_TRK_GPI   	(0x4D08)	// RST_CFG bit of the GENERAL_CFG_B register
#define IOXP_GENERAL_CFG_B_LCK_TRK_LOGIC  	(0x4D10)	// TRK_GPI bit of the GENERAL_CFG_B register
#define IOXP_GENERAL_CFG_B_CORE_FREQ0  		(0x4D20)	// CORE_FREQ0 bit of the GENERAL_CFG_B register
#define IOXP_GENERAL_CFG_B_CORE_FREQ1  		(0x4D40)	// CORE_FREQ1 bit of the GENERAL_CFG_B register
#define IOXP_GENERAL_CFG_B_OSC_EN  			(0x4D80)	// OSC_EN bit of the GENERAL_CFG_B register
#define IOXP_INT_EN_EVENT_IEN   	  		(0x4E01)	// EVENT_IEN bit of the INT_EN register
#define IOXP_INT_EN_GPI_IEN     			(0x4E02)	// GPI_IEN bit of the INT_EN register
#define IOXP_INT_EN_OVERFLOW_IEN 		 	(0x4E04)	// OVERFLOW_IEN bit of the INT_EN register
#define IOXP_INT_EN_LOCK_IEN   			  	(0x4E08)	// LOCK_IEN bit of the INT_EN register
#define IOXP_INT_EN_LOGIC1_INT  			(0x4E10)	// LOGIC1_IEN bit of the INT_EN register
#define IOXP_INT_EN_LOGIC2_INT  			(0x4E20)	// LOGIC2_IEN bit of the INT_EN register
#define IOXP_GPI_EVENT_EN_GPI_EVENT_EN(x)	((((0x21 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	// GPI_EVENT_EN bit of the GPI_EVENT_EN registers corresponding to GPIO no x (x between 1 and 19)
#define IOXP_GPI_INT_STAT_GPI_INT(x)		((((0x13 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	// GPI_INT bit of the GPI_INT_STAT registers corresponding to GPIO no x  (x between 1 and 19)
#define IOXP_GPI_STATUS_GPI_STAT(x)		 	((((0x16 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	// GPI_STAT bit of the GPI_STATUS registers corresponding to GPIO no x  (x between 1 and 19)
#define IOXP_INT_LEVEL_GPI_INT_LEVEL(x)		((((0x38 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	// GPI_INT_LEVEL bit of GPI_INT_LEVEL registers corresponding to GPIO no x  (x between 1 and 19)
#define IOXP_DEBOUNCE_DIS_GPI_DEB_DIS(x)	((((0x27 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	// GPI_DEB_DIS bit of the DEBOUNCE_DIS registers corresponding to GPIO no x  (x between 1 and 19)
#define IOXP_GPO_DATA_OUT_GPO_DATA(x)		((((0x2A + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	// GPO_DATA bit of the GPO_DATA_OUT registers corresponding to GPIO no x  (x between 1 and 19)
#define IOXP_GPO_OUT_MODE_GPO_OUT_MODE(x)	((((0x2D + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	// GPO_OUT_MODE bit of the GPO_OUT_MODE registers corresponding to GPIO no x (x between 1 and 19)
#define IOXP_GPIO_DIRECTION_GPIO_DIR(x)		((((0x30 + div8(x - 1))) << 8) | (1 << mod8(x - 1)))	// GPIO_DIR bit of the GPIO_DIRECTION registers corresponding to GPIO no x (x between 1 and 19)
/* -------------------------------------------------------------------- */
/*		Register Bit Mask Definitions - bits groups				        */
/* -------------------------------------------------------------------- */
#define IOXP_ID_MAN_ID  					(0x00F0)	// MAN_ID[3:0] field of the ID register
#define IOXP_ID_REV_ID  					(0x000F)	// REV_ID[3:0] field of the ID register
#define IOXP_INT_STATUS_ALL   				(0x01FF)	// all bits [7:0] of INT_STATUS register 
#define IOXP_STATUS_EC  					(0x020F)	// EC[4:0] field of the Status register
#define IOXP_UNLOCK1_UNLOCK1  				(0x333F)	// UNLOCK1[6:0] field of the UNLOCK1 register
#define IOXP_UNLOCK2_UNLOCK2  				(0x343F)	// UNLOCK2[6:0] field of the UNLOCK2 register
#define IOXP_EXT_LOCK_EVENT_EXT_LOCK_EVENT	(0x353F)	// EXT_LOCK_EVENT[6:0] field of the EXT_LOCK_EVENT register			
#define IOXP_UNLOCK_TIMERS_INT_MASK_TIMER  	(0x36F0)	// INT_MASK_TIMER[4:0] field of the UNLOCK_TIMERS register
#define IOXP_UNLOCK_TIMERS_UNLOCK_TIMER  	(0x360F)	// UNLOCK_TIMER[2:0] field of the UNLOCK_TIMERS register
#define IOXP_RESET1_EVENT_A_RESET1_EVENT_A  (0x383F)	// RESET1_EVENT_A[6:0] field of the RESET1_EVENT_A register
#define IOXP_RESET1_EVENT_B_RESET1_EVENT_B  (0x393F)	// RESET1_EVENT_B[6:0] field of the RESET1_EVENT_B register
#define IOXP_RESET1_EVENT_C_RESET1_EVENT_C  (0x3A3F)	// RESET1_EVENT_C[6:0] field of the RESET1_EVENT_C register
#define IOXP_RESET2_EVENT_A_RESET2_EVENT_A  (0x3B3F)	// RESET2_EVENT_A[6:0] field of the RESET2_EVENT_A register
#define IOXP_RESET2_EVENT_B_RESET2_EVENT_B  (0x3C3F)	// RESET2_EVENT_B[6:0] field of the RESET2_EVENT_B register			
#define IOXP_RESET_CFG_RESET_PULSE_WIDTH	(0x3D03)	// RESET_PULSE_WIDTH[1:0] field of the RESET_PULSE_WIDTH register
#define IOXP_RESET_CFG_RESET_TRIGGER_TIME	(0x3D1C)	// TRIGGER_TIME[2:0] field of the RESET_PULSE_WIDTH register
#define IOXP_CLOCK_DIV_CFG_CLK_DIV			(0X433E)	// CFG_CLK_DIV[4:0] field of the CLOCK_DIV register
#define IOXP_LOGIC_1_CFG_LOGIC1_SEL			(0x4407)	// LOGIC1_SEL[1:0] field of the LOGIC_1_CFG register	
#define IOXP_LOGIC_2_CFG_LOGIC2_SEL			(0x4507)	// LOGIC2_SEL[2:0] field of the LOGIC_2_CFG register
#define IOXP_POLL_TIME_CFG_KEY_POLL_TIME	(0x4803)	// KEY_POLL_TIME[1:0] field of the POLL_TIME_CFG register
#define IOXP_PIN_CONFIG_D_R3_EXTEND		  	(0x4C0C)	// R3_EXTEND[1:0] field of the PIN_CONFIG_D register
#define IOXP_GENERAL_CFG_B_CORE_FREQ  		(0x4D60)	// CORE_FREQ[1:0] field of the GENERAL_CFG_B register
#define IOXP_RPULL_CONFIG_R_PULL_CFG(x)		((((0x19 + div4(x))) << 8) | (3 << (mod4(x)<<1)))		// R_PULL_CFG field of the RPULL_CONFIG registers corresponding to Row x (x between 0 and 7)
#define IOXP_RPULL_CONFIG_C_PULL_CFG(x)		((((0x1B + div4(x))) << 8) | (3 << (mod4(x)<<1)))		// C_PULL_CFG field of the RPULL_CONFIG registers corresponding to Column x (x between 0 and 10)
/* ------------------------------------------------------------------- */
/*			Parameters Definitions  							       */
/* ------------------------------------------------------------------- */



#define	PAR_EXT_INT0 0	// External interrupt 0
#define	PAR_EXT_INT1 1	// External interrupt 1
#define	PAR_EXT_INT2 2	// External interrupt 2
#define	PAR_EXT_INT3 3	// External interrupt 3
#define	PAR_EXT_INT4 4	// External interrupt 4

#define IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_DIS		(0)		// disabled
#define IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_1SEC	(1)		//  1 sec
#define IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_2SEC	(2)		//  2 sec
#define IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_30SEC	(0X1E)	// 30 sec
#define IOXP_UNLOCK_TIMERS_INT_MASTER_TIMER_31SEC	(0X1F)	// 31 sec

#define IOXP_UNLOCK_TIMER_DIS	// disabled
#define IOXP_UNLOCK_TIMER_1SEC	// 1 sec
#define IOXP_UNLOCK_TIMER_2SEC	// 2 sec
#define IOXP_UNLOCK_TIMER_3SEC	// 3 sec
#define IOXP_UNLOCK_TIMER_4SEC	// 4 sec
#define IOXP_UNLOCK_TIMER_5SEC	// 5 sec	
#define IOXP_UNLOCK_TIMER_6SEC	// 6 sec	
#define IOXP_UNLOCK_TIMER_7SEC	// 7 sec	


#define IOXP_RESET_CFG_RESET_PULSE_WIDTH_500US		0	// 500 us
#define IOXP_RESET_CFG_RESET_PULSE_WIDTH_1MS		1	// 1 ms
#define IOXP_RESET_CFG_RESET_PULSE_WIDTH_2MS		2	// 2 ms
#define IOXP_RESET_CFG_RESET_PULSE_WIDTH_10MS		3	// 10 ms


#define IOXP_RESET_CFG_RESET_TRIGGER_TIME_IMMED		0	// Immediate
#define IOXP_RESET_CFG_RESET_TRIGGER_TIME_1D0SEC	1	// 1.0 sec
#define IOXP_RESET_CFG_RESET_TRIGGER_TIME_1D5SEC	2	// 1.5 sec
#define IOXP_RESET_CFG_RESET_TRIGGER_TIME_2D0SEC	3	// 2.0 sec
#define IOXP_RESET_CFG_RESET_TRIGGER_TIME_2D5SEC	4	// 2.5 sec
#define IOXP_RESET_CFG_RESET_TRIGGER_TIME_3D0SEC	5	// 3.0 sec
#define IOXP_RESET_CFG_RESET_TRIGGER_TIME_3D5SEC	6	// 3.5 sec
#define IOXP_RESET_CFG_RESET_TRIGGER_TIME_4D0SEC	7	// 4.0 sec

#define IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV1				(0X00)	// divide by 1 (pass-through).
#define IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV2				(0X01)	// divide by 2.
#define IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV3				(0X02)	// divide by 3.
#define IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV4				(0X03)	// divide by 4.
#define IOXP_CLOCK_DIV_CFG_CLK_DIV_DIV32			(0X1F)	// divide by 32.

#define IOXP_LOGIC_CFG_LOGIC_SEL_OFF			(0x00)	// off/disable
#define IOXP_LOGIC_CFG_LOGIC_SEL_AND			(0x01)	// AND
#define IOXP_LOGIC_CFG_LOGIC_SEL_OR				(0x02)	// OR
#define IOXP_LOGIC_CFG_LOGIC_SEL_XOR			(0x03)	// XOR
#define IOXP_LOGIC_CFG_LOGIC_SEL_FF				(0x04)	// FF
#define IOXP_LOGIC_CFG_LOGIC_SEL_IN_LA			(0x05)	// IN_LA
#define IOXP_LOGIC_CFG_LOGIC_SEL_IN_LB			(0x06)	// IN_LB
#define IOXP_LOGIC_CFG_LOGIC_SEL_IN_LC			(0x07)	// IN_LC

#define IOXP_POLL_TIME_CFG_KEY_POLL_TIME_10MS	(0x00)	// 10 ms
#define IOXP_POLL_TIME_CFG_KEY_POLL_TIME_20MS	(0x01)	// 20 ms
#define IOXP_POLL_TIME_CFG_KEY_POLL_TIME_30MS	(0x02)	// 30 ms
#define IOXP_POLL_TIME_CFG_KEY_POLL_TIME_40MS	(0x03)	// 40 ms

#define IOXP_GENERAL_CFG_B_CORE_FREQ_50K		(0x00)	//  50 kHz
#define IOXP_GENERAL_CFG_B_CORE_FREQ_100K		(0x01)	// 100 kHz
#define IOXP_GENERAL_CFG_B_CORE_FREQ_200K		(0x02)	// 200 kHz
#define IOXP_GENERAL_CFG_B_CORE_FREQ_500K		(0x03)	// 500 kHz


#define IOXP_RPULL_CONFIG_300KPU		0	// enable 300 kO pull-up
#define IOXP_RPULL_CONFIG_300KPD		1	// enable 300 kO pull-down
#define IOXP_RPULL_CONFIG_100KPD		2	// enable 100 kO pull-up.
#define IOXP_RPULL_CONFIG_300KPD		3	// disable all pull-up/pull-down resistors

/* -------------------------------------------------------------------- */
/*					Procedure Declarations						        */
/* -------------------------------------------------------------------- */

// the default value for the keyMap corresponds to Digilent PmodKYPD plugged into J1 connector of PmodIOXP (R0-3, C0-3).
static int keyMap_KYPD[IOXP_KB_ROWS][IOXP_KB_COLS] = 
					{{ 0xD,0xC,0xB,0xA, -1, -1, -1, -1, -1, -1, -1},    //row 0
                     { 0xE,  9,  6,  3, -1, -1, -1, -1, -1, -1, -1},    //row 1
                     { 0xF,  8,  5,  2, -1, -1, -1, -1, -1, -1, -1},    //row 2
                     {   0,  7,  4,  1, -1, -1, -1, -1, -1, -1, -1},    //row 3
                     {  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},    //row 4
                     {  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},    //row 5
                     {  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},    //row 6
                     {  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},    //row 7
					 };


class IOXP {
private:	
	void ReadBytesI2C(uint8_t bAddress, uint8_t bCntBytes, uint8_t *rgbValues);
	void WriteBytesI2C(uint8_t bAddress, uint8_t bCntBytes, uint8_t *rgbValues);
	uint8_t ReadMaskedRegisterValue(uint8_t bAddress, uint8_t bMask);
	void WriteMaskedRegisterValue(uint8_t bAddress, uint8_t bMask, uint8_t bValue);
	void DecodeEvent(uint8_t bEvent, int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);
	// under construction not fully working
	uint8_t EncodeEvent(int iKeyVal, uint8_t bRow = 0xFF, uint8_t bCol = 0xFF, uint8_t bGPI = 0xFF, uint8_t bLogic = 0xFF, uint8_t bEventState = 0);

	int GetKeyVal(uint8_t bRow, uint8_t bCol);
	void GetKeyByVal(int iKeyVal, uint8_t &bRow, uint8_t &bCol);
	uint8_t Mask2Scale(uint8_t bMask);
	void attachCNInterrupt(uint8_t bParCNNo, void (*pfIntHandler)(), unsigned char type);
    int keyMap[IOXP_KB_ROWS][IOXP_KB_COLS];	
public:
	IOXP();
	void begin();
	void SetRegister(uint8_t bAddress, uint8_t bValue);	
	uint8_t GetRegister(uint8_t bAddress);
	void SetRegisterBit(uint16_t wBitDef, uint8_t bBitVal);
	uint8_t GetRegisterBit(uint16_t wBitDef);
	void SetRegisterBitsGroup(uint16_t wBitDef, uint8_t bBitsVal);
	uint8_t GetRegisterBitsGroup(uint16_t wBitDef);

	void SetKeyboardPinConfig(uint8_t bRowCfg, uint16_t wColCfg);
	void ReadFIFO(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);

	void ConfigureInterrupt(uint8_t bParExtIntNo, uint16_t wEventMask, void (*pfIntHandler)());
	
	
	// under construction not fully working
	void SetLockEvent(int iKeyVal, uint8_t bRow = 0xFF, uint8_t bCol = 0xFF, uint8_t bGPI = 0xFF, uint8_t bLogic = 0xFF, uint8_t bEventState = 1);
	
	void GetLockEvent(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);
   
   // under construction not fully working 
	void SetUnlock1Event(int iKeyVal, uint8_t bRow = 0xFF, uint8_t bCol = 0xFF, uint8_t bGPI = 0xFF, uint8_t bLogic = 0xFF, uint8_t bEventState = 1);

	void GetUnlock1Event(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);
	
	// under construction not fully working
	void SetUnlock2Event(int iKeyVal, uint8_t bRow = 0xFF, uint8_t bCol = 0xFF, uint8_t bGPI = 0xFF, uint8_t bLogic = 0xFF, uint8_t bEventState = 1);

	void GetUnlock2Event(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);
    
	// under construction not fully working
	void SetReset1EventA(int iKeyVal, uint8_t bRow = 0xFF, uint8_t bCol = 0xFF, uint8_t bGPI = 0xFF, uint8_t bLogic = 0xFF, uint8_t bEventState = 1);
	
	void GetReset1EventA(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);
	
	// under construction not fully working
	void SetReset1EventB(int iKeyVal, uint8_t bRow = 0xFF, uint8_t bCol = 0xFF, uint8_t bGPI = 0xFF, uint8_t bLogic = 0xFF, uint8_t bEventState = 1);
	
	
	void GetReset1EventB(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);
	
	// under construction not fully working
	void SetReset1EventC(int iKeyVal, uint8_t bRow = 0xFF, uint8_t bCol = 0xFF, uint8_t bGPI = 0xFF, uint8_t bLogic = 0xFF, uint8_t bEventState = 1);

	void GetReset1EventC(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);
	
	// under construction not fully working
	void SetReset2EventA(int iKeyVal, uint8_t bRow = 0xFF, uint8_t bCol = 0xFF, uint8_t bGPI = 0xFF, uint8_t bLogic = 0xFF, uint8_t bEventState = 1);

	void GetReset2EventA(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);
	
	// under construction not fully working
	void SetReset2EventB(int iKeyVal, uint8_t bRow = 0xFF, uint8_t bCol = 0xFF, uint8_t bGPI = 0xFF, uint8_t bLogic = 0xFF, uint8_t bEventState = 1);
	
	void GetReset2EventB(int &iKeyVal, uint8_t &bRow, uint8_t &bCol, uint8_t &bGPI, uint8_t &bLogic, uint8_t &bEventState);
	

	
	void SetIntMaskTimer(uint8_t bIntMaskTimer);
	uint8_t GetIntMaskTimer();
	void SetUnlockTimer(uint8_t bUnlockTimer);
	uint8_t GetUnlockTimer();
	
	void SetRPullConfig(uint8_t *rgbVals);
	void GetRPullConfig(uint8_t *rgbVals);
	
	uint32_t GetGPIIntStat();
	uint32_t GetGPIStat();

	void SetGPIIntLevel(uint32_t dwBitMap);
	uint32_t GetGPIIntLevel();

	void SetGPIEventEn(uint32_t dwBitMap);
	uint32_t GetGPIEventEn();

	void SetGPIDebounceDis(uint32_t dwBitMap);
	uint32_t GetGPIDebounceDis();

	void SetGPODataOut(uint32_t dwBitMap);
	uint32_t GetGPODataOut();
	
	void SetGPOOutMode(uint32_t dwBitMap);
	uint32_t GetGPOOutMode();
	
	void SetGPIODirection(uint32_t dwBitMap);
	uint32_t GetGPIODirection();
	
	void SetResetCfgResetTriggerTime(uint8_t bResetTriggerTime);
	uint8_t GetResetCfgResetTriggerTime();
	
	void SetResetCfgResetPulseWidth(uint8_t bResetPulseWidth);
	uint8_t GetResetCfgResetPulseWidth(uint8_t bResetPulseWidth);
	void SetPWMOffTimeUS(uint16_t wOffTime);
	uint16_t GetPWMOffTimeUS();
	void SetPWMOnTimeUS(uint16_t wOnTime);
	uint16_t GetPWMOnTimeUS();
	
	void SetClkDivClkDiv(uint8_t bClkDiv);
	uint8_t GetClkDivClkDiv();
	
	void SetLogicSel1(uint8_t bLogicSel);
	uint8_t GetLogicSel1();

	void SetLogicSel2(uint8_t bLogicSel);
	uint8_t GetLogicSel2();

	void SetPollTime(uint8_t bPollTime);
	uint8_t GetPollTime();

	void SetCoreFreq(uint8_t bCoreFreq);
	uint8_t GetCoreFreq();
	
	
	void SetKeyMap(int table[IOXP_KB_ROWS][IOXP_KB_COLS]);
};



#endif