/****************************************************************************
 *
 *    ****                              I
 *   ******                            ***
 *   *******                           ****
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    ****I  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       perseus601x_def.h
 * @brief      Perseus601x carrier related data and structure definitions.
 *
 * $Date: 2015/11/13 19:22:40 $
 * $Revision: 1.4 $
 *
 * @ingroup PERSEUS601X
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * This file is part of Nutaq's BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq's BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq's BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq's BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq's BAS Software Suite under the GPL requires that your work based on
 * Nutaq's BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq's BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq's BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/

#ifndef __PERSEUS601X_DEF_H__
#define __PERSEUS601X_DEF_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "carrier_service_def.h"

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

#define PERSEUS601X_CORE_ID     0xC601

// This define matches for each vadj voltage setting from Perseus601X_eVadjValue_t enum, the related voltage
// value in 10mV from Perseus601X_eVadjValueLevels_t enum. Each item is in the order of Perseus601X_eVadjValue_t enum.
// Do not change this list content and order (see related E_VADJ_VALUE and Perseus601X_eVadjValueLevels)
#define PERSEUS601X_VADJ_VOLTAGE_LEVELS_DEFINITION { \
  VADJ_1_192V_LEVEL, VADJ_1_490V_LEVEL, VADJ_1_803V_LEVEL, \
  VADJ_2_503V_LEVEL, VADJ_DISABLE_LEVEL, VADJ_UNKNOWN_LEVEL \
}

#define PERSEUS601X_VADJ_VALUE_DEFINITION { \
  VADJ_1_192V, VADJ_1_490V, VADJ_1_803V, \
  VADJ_2_503V, VADJ_DISABLE, VADJ_UNKNOWN \
}


// FPGA address spaces
#define PERSEUS601X_USER_SPACE_BASE_ADDR    0x60000000
#define PERSEUS601X_USER_SPACE_END_ADDR     0x7FFFFFFF
#define PERSEUS601X_USER_SPACE_SIZE         (PERSEUS601X_USER_SPACE_END_ADDR - PERSEUS601X_USER_SPACE_BASE_ADDR + 1)

#define PERSEUS601X_FMC_SPACE_BASE_ADDR     0x85000000
#define PERSEUS601X_FMC_SPACE_END_ADDR      0x85FFFFFF
#define PERSEUS601X_FMC_SPACE_SIZE          (PERSEUS601X_FMC_SPACE_END_ADDR - PERSEUS601X_FMC_SPACE_BASE_ADDR + 1)

// FPGA core addresses
#define PERSEUS601X_FMC_BASE_ADDR           PERSEUS601X_FMC_SPACE_BASE_ADDR
#define PERSEUS601X_FMC_MAX_NUM             2
#define PERSEUS601X_FMC_SIZE                0X10000

#define PERSEUS601X_I2C_BASE_ADDR           0x81600000
#define PERSEUS601X_I2C_END_ADDR            0x8160FFFF
#define PERSEUS601X_I2C_SIZE                (PERSEUS601X_I2C_END_ADDR - PERSEUS601X_I2C_BASE_ADDR + 1)
#define PERSEUS601X_I2C_MAX_NUM             1

#define PERSEUS601X_DDR3_BASE_ADDR          0x80100000
#define PERSEUS601X_DDR3_END_ADDR           0x80101FFF
#define PERSEUS601X_DDR3_SIZE               (PERSEUS601X_DDR3_END_ADDR - PERSEUS601X_DDR3_BASE_ADDR + 1)
#define PERSEUS601X_DDR3_MAX_NUM            1

#define PERSEUS601X_DDR3_I2C_BASE_ADDR      0x74000000
#define PERSEUS601X_DDR3_I2C_END_ADDR       0x7400FFFF
#define PERSEUS601X_DDR3_I2C_SIZE           (PERSEUS601X_DDR3_I2C_END_ADDR - PERSEUS601X_DDR3_I2C_BASE_ADDR +1)
#define PERSEUS601X_DDR3_I2C_MAX_NUM        1

#define PERSEUS601X_SYSMON_BASE_ADDR        0x75000000
#define PERSEUS601X_SYSMON_END_ADDR         0x75000FFF
#define PERSEUS601X_SYSMON_SIZE             (PERSEUS601X_SYSMON_END_ADDR - PERSEUS601X_SYSMON_BASE_ADDR + 1)
#define PERSEUS601X_SYSMON_MAX_NUM          1

#define PERSEUS601X_ETH_BASE_ADDR           0x81C00000
#define PERSEUS601X_ETH_END_ADDR            0x81C3FFFF
#define PERSEUS601X_ETH_SIZE                (PERSEUS601X_ETH_END_ADDR - PERSEUS601X_ETH_BASE_ADDR + 1)
#define PERSEUS601X_ETH_MAX_NUM             1

#define PERSEUS601X_DPRAM_BASE_ADDR         0x80100000
#define PERSEUS601X_DPRAM_END_ADDR          0x80101FFF
#define PERSEUS601X_DPRAM_SIZE              (PERSEUS601X_DPRAM_END_ADDR - PERSEUS601X_DPRAM_BASE_ADDR + 1)
#define PERSEUS601X_DPRAM_MAX_NUM           1

#define PERSEUS601X_RTDEX_GIGE_BASE_ADDR    0x71000000
#define PERSEUS601X_RTDEX_GIGE_MAX_NUM      1
#define PERSEUS601X_RTDEX_GIGE_SIZE         0X10000

#define PERSEUS601X_RECPLAY_BASE_ADDR       0x73000000
#define PERSEUS601X_RECPLAY_MAX_NUM         1
#define PERSEUS601X_RECPLAY_SIZE            0X10000

#define PERSEUS601X_REG_BASE_ADDR           CARRIER_REG_BASE_ADDR
#define PERSEUS601X_REG_MAX_NUM             CARRIER_REG_MAX_NUM
#define PERSEUS601X_REG_SIZE                CARRIER_REG_SIZE

// Internal platform registers
#define PERSEUS601X_LED_MAX_NUM             16

#define PERSEUS601X_MMC_CTRL_BASE_ADDR      (PERSEUS601X_REG_BASE_ADDR + 0x14)

#define PERSEUS601X_CUSTOMREG_BASE_ADDR     (PERSEUS601X_REG_BASE_ADDR + 0x18)
#define PERSEUS601X_CUSTOMREG_MAX_NUM       32
#define PERSEUS601X_CUSTOMREG_SIZE          (1 * 0x4)

// External FPGA cores
#define PERSEUS601X_AURORA_BASE_ADDR        (PERSEUS601X_REG_BASE_ADDR + 0x98)
#define PERSEUS601X_AURORA_MAX_NUM          3
#define PERSEUS601X_AURORA_SIZE             (2 * 0x4)

#define PERSEUS601X_PPSSYNC_BASE_ADDR       (PERSEUS601X_REG_BASE_ADDR + 0xb0)
#define PERSEUS601X_PPSSYNC_MAX_NUM         1
#define PERSEUS601X_PPSSYNC_SIZE            (6 * 0x4)

#define PERSEUS601X_LVDS_BASE_ADDR          (PERSEUS601X_REG_BASE_ADDR + 0xcc)
#define PERSEUS601X_LVDS_MAX_NUM            4
#define PERSEUS601X_LVDS_SIZE               (4 * 0x4)

#define PERSEUS601X_RTDEXSYNC_BASE_ADDR     (PERSEUS601X_REG_BASE_ADDR + 0x10c)
#define PERSEUS601X_RTDEXSYNC_MAX_NUM       1
#define PERSEUS601X_RTDEXSYNC_SIZE          (4 * 0x4)

#define PERSEUS601X_TIMESTAMP_BASE_ADDR     (PERSEUS601X_REG_BASE_ADDR + 0x11c)
#define PERSEUS601X_TIMESTAMP_MAX_NUM       1
#define PERSEUS601X_TIMESTAMP_SIZE          (4 * 0x4)

typedef enum Perseus601x_eI2cZone {
    Perseus601x_eI2cZoneCarrier=0, ///< I2C zone that contains all I2C devices on the carrier
    Perseus601x_eI2cZoneFmc=1,     ///< I2C zone that contains all I2C devices on the FMC
}Perseus601x_eI2cZone_t;

#ifdef __cplusplus
}
#endif  // extern "C"
#endif

