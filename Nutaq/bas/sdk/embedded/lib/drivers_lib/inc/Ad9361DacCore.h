/***************************************************************************//**
 *   @file   Ad9361DacCore.h
 *   @brief  Header file of DAC Core Driver.
 *   @author DBogdan (dragos.bogdan@analog.com)
********************************************************************************
 * Copyright 2013(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/
#ifndef AD9361_DAC_CORE_H_
#define AD9361_DAC_CORE_H_

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "Ad9361.h"
#include "Ad9361_defines.h"
#include "Ad9361DacCore_defines.h"

/******************************************************************************/
/************************ Functions Declarations ******************************/
/******************************************************************************/
void Ad9361DacCore_Init(struct Ad9361_stPhy *phy, uint8_t data_sel);

int32_t Ad9361DacCore_SetDataSel(struct Ad9361_stPhy *phy, int32_t chan, enum Ad9361DacCore_eDataSel sel);
void Ad9361DacCore_GetDataSel(struct Ad9361_stPhy *phy, int32_t chan, enum Ad9361DacCore_eDataSel *sel);

void Ad9361DacCore_Sync(struct Ad9361_stPhy *phy, uint8_t force_on);
                             
void Ad9361DacCore_Read(struct Ad9361_stPhy *phy, uint32_t regAddr, uint32_t *data);
void Ad9361DacCore_Write(struct Ad9361_stPhy *phy, uint32_t regAddr, uint32_t data);
unsigned int Ad9361DacCore_ReadValue(struct Ad9361_stPhy *phy, unsigned long reg);

#endif
