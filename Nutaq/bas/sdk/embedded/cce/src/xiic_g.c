/* $Id: xiic_g.c,v 1.1 2015/04/10 14:08:11 sebastian.jupin-lang Exp $ */
/******************************************************************************
*
* (c) Copyright 2002-2011 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xiic_g.c
*
* This file contains a configuration table that specifies the configuration of
* IIC devices in the system. Each IIC device should have an entry in this table.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- --- ------- -----------------------------------------------
* 1.01a rfp  10/19/01 release
* 1.01c ecm  12/05/02 new rev
* 1.01d jhl  10/08/03 Added general purpose output feature
* 1.13a wgr  03/22/07 Converted to new coding style.
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xiic.h"
#include "xparameters.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/


/************************** Variable Definitions *****************************/

/**
 * The IIC configuration table, sized by the number of instances
 * defined in xparameters.h.
 */
XIic_Config XIic_ConfigTable[XPAR_XIIC_NUM_INSTANCES] = {
	{
	 XPAR_IIC_0_DEVICE_ID,	/* Device ID for instance */
	 XPAR_IIC_0_BASEADDR,	/* Base address */
	 XPAR_IIC_0_TEN_BIT_ADR,/* Uses 10 bit addressing */
	 XPAR_IIC_0_GPO_WIDTH,	/* Number of bits in GPO register */
	}
	,
	{
	 XPAR_IIC_1_DEVICE_ID,	/* Device ID for instance */
	 XPAR_IIC_1_BASEADDR,	/* Base address */
	 XPAR_IIC_1_TEN_BIT_ADR	/* Uses 10 bit addressing */
	 XPAR_IIC_1_GPO_WIDTH,	/* Number of bits in GPO register */
	}
};
