/***************************************************************************//**
 *   @file   Ad9361Util.h
 *   @brief  Header file of Util driver.
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
#ifndef __NO_OS_PORT_H__
#define __NO_OS_PORT_H__

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "Ad9361.h"
#include "bas_error.h"

/******************************************************************************/
/********************** Macros and Constants Definitions **********************/
/******************************************************************************/

#ifdef DEBUG_RADIO640
#define dev_err(dev, format, ...)		printf(format, ## __VA_ARGS__);printf("\n")
#define dev_warn(dev, format, ...)		printf(format, ## __VA_ARGS__);printf("\n")
#define dev_dbg(dev, format, ...)		printf(format, ## __VA_ARGS__);printf("\n")
#define ad9361_debug_spi(...)           debug(__VA_ARGS__)
#else
#define dev_err(dev, format, ...)
#define dev_warn(dev, format, ...)
#define dev_dbg(dev, format, ...)
#define ad9361_debug_spi(...)
#endif //DEBUG_RADIO640


#define ARRAY_SIZE(arr)							(sizeof(arr) / sizeof((arr)[0]))
#define min(x, y)								(((x) < (y)) ? (x) : (y))
#define min_t(type, x, y)						(type)min((type)(x), (type)(y))
#define max(x, y)								(((x) > (y)) ? (x) : (y))
#define max_t(type, x, y)						(type)max((type)(x), (type)(y))
#define clamp(val, min_val, max_val)			(max(min((val), (max_val)), (min_val)))
#define clamp_t(type, val, min_val, max_val)	(type)clamp((type)(val), (type)(min_val), (type)(max_val))
#define DIV_ROUND_UP(x, y)						(((x) + (y) - 1) / (y))
#define DIV_ROUND_CLOSEST(x, divisor)			(((x) + (divisor) / 2) / (divisor))
#define BIT(x)									(1 << (x))
#define CLK_IGNORE_UNUSED						BIT(3)
#define CLK_GET_RATE_NOCACHE					BIT(6)

struct Ad9361Util_Clk {
	const char	*name;
	uint32_t	rate;
	uint8_t	    init_done;
};

/******************************************************************************/
/************************ Functions Declarations ******************************/
/******************************************************************************/
uint32_t Ad9361Util_GetClkRate(struct Ad9361_stPhy *phy,
					  struct refclk_scale *clk_priv);
int32_t Ad9361Util_SetClkRate(struct Ad9361_stPhy *phy,
					 struct refclk_scale *clk_priv,
					 uint32_t rate);
uint32_t Ad9361Util_IntSqrt(uint32_t x);
int32_t Ad9361Util_IntLog2(int32_t x);
uint64_t Ad9361Util_DoDiv(uint64_t* n, uint64_t base);
uint32_t Ad9361Util_Ffs(uint32_t word);

#endif
