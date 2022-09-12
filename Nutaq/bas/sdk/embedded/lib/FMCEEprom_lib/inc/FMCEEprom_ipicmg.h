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
 * @file       FMCEEprom_ipicmg.h
 * @brief      fmceeprom library Kontron definitions.
 *
 * $Date: 2015/04/10 14:08:11 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup FMCEEPROMLIB
 *
 * This file is part of Nutaq’s BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq’s BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq’s BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq’s BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq’s BAS Software Suite under the GPL requires that your work based on
 * Nutaq’s BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq’s BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq’s BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/
/*
	(C) Kontron
 */

#ifndef _IPMI_PICMG_H_
#define _IPMI_PICMG_H_

#define  uint8_t   unsigned char
#define  uint16_t  unsigned short
#define  uint32_t  unsigned int

#define BMC_SA     0x20
#define PUBLIC_BUS 0
#define BMC_LUN    0

/* PICMG version */
#define PICMG_CPCI_MAJOR_VERSION                   1
#define PICMG_ATCA_MAJOR_VERSION                   2
#define PICMG_AMC_MAJOR_VERSION                    4

#define FRU_PICMGEXT_LINK_TYPE_BASE                     0x01
#define FRU_PICMGEXT_LINK_TYPE_FABRIC_ETHERNET          0x02
#define FRU_PICMGEXT_LINK_TYPE_FABRIC_INFINIBAND        0x03
#define FRU_PICMGEXT_LINK_TYPE_FABRIC_STAR              0x04
#define FRU_PICMGEXT_LINK_TYPE_PCIE                     0x05

#ifndef HAVE_FRU_PICMG_EXT
#define HAVE_FRU_PICMG_EXT
#pragma pack(1)
struct fru_picmgext_link_desc {
#ifndef WORDS_BIGENDIAN
        unsigned int desig_channel:6;
        unsigned int desig_if:2;
        unsigned int desig_port:4;
        unsigned int type:8;
        unsigned int ext:4;
        unsigned int grouping:8;
#else
        unsigned int grouping:8;
        unsigned int ext:4;
        unsigned int type:8;
        unsigned int desig_port:4;
        unsigned int desig_if:2;
        unsigned int desig_channel:6;
#endif
}; /* __attribute__ ((packed)); */
struct fru_picmgext_amc_link_info {
   unsigned char linkInfo[3];
}; /* __attribute__ ((packed)); */
#pragma pack()
#endif

#define OEM_PICMG  12634
//  IPMI_OEM_PICMG      = 12634,

#define FRU_PICMG_BACKPLANE_P2P                 0x04
#define FRU_PICMG_ADDRESS_TABLE                 0x10
#define FRU_PICMG_SHELF_POWER_DIST              0x11
#define FRU_PICMG_SHELF_ACTIVATION              0x12
#define FRU_PICMG_SHMC_IP_CONN                  0x13
#define FRU_PICMG_BOARD_P2P                     0x14
#define FRU_AMC_CURRENT                         0x16
#define FRU_AMC_ACTIVATION                      0x17
#define FRU_AMC_CARRIER_P2P                     0x18
#define FRU_AMC_P2P                             0x19
#define FRU_AMC_CARRIER_INFO                    0x1a
#define FRU_UTCA_FRU_INFO_TABLE                 0x20
#define FRU_UTCA_CARRIER_MNG_IP                 0x21
#define FRU_UTCA_CARRIER_INFO                   0x22
#define FRU_UTCA_CARRIER_LOCATION               0x23
#define FRU_UTCA_SHMC_IP_LINK                   0x24
#define FRU_UTCA_POWER_POLICY                   0x25
#define FRU_UTCA_ACTIVATION                     0x26
#define FRU_UTCA_PM_CAPABILTY                   0x27
#define FRU_UTCA_FAN_GEOGRAPHY                  0x28
#define FRU_UTCA_CLOCK_MAPPING                  0x29
#define FRU_UTCA_MSG_BRIDGE_POLICY              0x2A
#define FRU_UTCA_OEM_MODULE_DESC                0x2B
#define FRU_PICMG_CLK_CARRIER_P2P               0x2C
#define FRU_PICMG_CLK_CONFIG                    0x2D

#define FRU_PICMGEXT_LINK_TYPE_BASE                  0x01
#define FRU_PICMGEXT_LINK_TYPE_FABRIC_ETHERNET       0x02
#define FRU_PICMGEXT_LINK_TYPE_FABRIC_INFINIBAND     0x03
#define FRU_PICMGEXT_LINK_TYPE_FABRIC_STAR           0x04
#define FRU_PICMGEXT_LINK_TYPE_PCIE                  0x05
#define FRU_PICMGEXT_LINK_TYPE_BASE                  0x01
#define FRU_PICMGEXT_LINK_TYPE_FABRIC_ETHERNET       0x02
#define FRU_PICMGEXT_LINK_TYPE_FABRIC_INFINIBAND     0x03
#define FRU_PICMGEXT_LINK_TYPE_FABRIC_STAR           0x04
#define FRU_PICMGEXT_LINK_TYPE_PCIE                  0x05
#define FRU_PICMGEXT_AMC_LINK_TYPE_RESERVED 		0x00
#define FRU_PICMGEXT_AMC_LINK_TYPE_RESERVED1            0x01
#define FRU_PICMGEXT_AMC_LINK_TYPE_PCI_EXPRESS          0x02
#define FRU_PICMGEXT_AMC_LINK_TYPE_ADVANCED_SWITCHING1  0x03
#define FRU_PICMGEXT_AMC_LINK_TYPE_ADVANCED_SWITCHING2  0x04
#define FRU_PICMGEXT_AMC_LINK_TYPE_ETHERNET             0x05
#define FRU_PICMGEXT_AMC_LINK_TYPE_RAPIDIO              0x06
#define FRU_PICMGEXT_AMC_LINK_TYPE_STORAGE              0x07
#define FRU_PICMGEXT_OEM_SWFW 			     0x03
#define FRU_PICMGEXT_DESIGN_IF_BASE                  0x00
#define FRU_PICMGEXT_DESIGN_IF_FABRIC                0x01
#define FRU_PICMGEXT_DESIGN_IF_UPDATE_CHANNEL        0x02
#define FRU_PICMGEXT_DESIGN_IF_RESERVED              0x03


     #define FRU_PICMGEXT_AMC_LINK_TYPE_PCIE         0x02
     #define FRU_PICMGEXT_AMC_LINK_TYPE_PCIE_AS1     0x03
     #define FRU_PICMGEXT_AMC_LINK_TYPE_PCIE_AS2     0x04
     #define FRU_PICMGEXT_AMC_LINK_TYPE_ETHERNET     0x05
     #define FRU_PICMGEXT_AMC_LINK_TYPE_RAPIDIO      0x06
     #define FRU_PICMGEXT_AMC_LINK_TYPE_STORAGE      0x07

     #define AMC_LINK_TYPE_EXT_PCIE_G1_NSSC  0x00
     #define AMC_LINK_TYPE_EXT_PCIE_G1_SSC   0x01
     #define AMC_LINK_TYPE_EXT_PCIE_G2_NSSC  0x02
     #define AMC_LINK_TYPE_EXT_PCIE_G2_SSC   0x03
     #define AMC_LINK_TYPE_EXT_ETH_1000_BX   0x00
     #define AMC_LINK_TYPE_EXT_ETH_10G_XAUI  0x01
     #define AMC_LINK_TYPE_EXT_STORAGE_FC    0x00
     #define AMC_LINK_TYPE_EXT_STORAGE_SATA  0x01
     #define AMC_LINK_TYPE_EXT_STORAGE_SAS   0x02

#define IPMI_NETFN_PICMG         0x2C

/* PICMG commands */
#define PICMG_GET_PICMG_PROPERTIES_CMD             0x00
#define PICMG_GET_ADDRESS_INFO_CMD                 0x01
#define PICMG_GET_SHELF_ADDRESS_INFO_CMD           0x02
#define PICMG_SET_SHELF_ADDRESS_INFO_CMD           0x03
#define PICMG_FRU_CONTROL_CMD                      0x04
#define PICMG_GET_FRU_LED_PROPERTIES_CMD           0x05
#define PICMG_GET_LED_COLOR_CAPABILITIES_CMD       0x06
#define PICMG_SET_FRU_LED_STATE_CMD                0x07
#define PICMG_GET_FRU_LED_STATE_CMD                0x08
#define PICMG_SET_IPMB_CMD                         0x09
#define PICMG_SET_FRU_POLICY_CMD                   0x0A
#define PICMG_GET_FRU_POLICY_CMD                   0x0B
#define PICMG_FRU_ACTIVATION_CMD                   0x0C
#define PICMG_GET_DEVICE_LOCATOR_RECORD_CMD        0x0D
#define PICMG_SET_PORT_STATE_CMD                   0x0E
#define PICMG_GET_PORT_STATE_CMD                   0x0F
#define PICMG_COMPUTE_POWER_PROPERTIES_CMD         0x10
#define PICMG_SET_POWER_LEVEL_CMD                  0x11
#define PICMG_GET_POWER_LEVEL_CMD                  0x12
#define PICMG_RENEGOTIATE_POWER_CMD                0x13
#define PICMG_GET_FAN_SPEED_PROPERTIES_CMD         0x14
#define PICMG_SET_FAN_LEVEL_CMD                    0x15
#define PICMG_GET_FAN_LEVEL_CMD                    0x16
#define PICMG_BUSED_RESOURCE_CMD                   0x17

/* AMC.0 commands */
#define PICMG_AMC_SET_PORT_STATE_CMD			0x19
#define PICMG_AMC_GET_PORT_STATE_CMD			0x1A
/* AMC.0 R2.0 commands */
#define PICMG_AMC_SET_CLK_STATE_CMD				0x2C
#define PICMG_AMC_GET_CLK_STATE_CMD				0x2D

/* Site Types */
#define PICMG_ATCA_BOARD                           0x00
#define PICMG_POWER_ENTRY                          0x01
#define PICMG_SHELF_FRU                            0x02
#define PICMG_DEDICATED_SHMC                       0x03
#define PICMG_FAN_TRAY                             0x04
#define PICMG_FAN_FILTER_TRAY                      0x05
#define PICMG_ALARM                                0x06
#define PICMG_AMC                                  0x07
#define PICMG_PMC                                  0x08
#define PICMG_RTM                                  0x09

#pragma pack(1)
struct picmg_set_fru_activation_cmd {
   unsigned char  picmg_id;      /* always 0*/
   unsigned char  fru_id;        /* threshold setting mask */
   unsigned char  fru_state;     /* fru activation/deactivation */
};  // __attribute__ ((packed));
#pragma pack()

typedef enum picmg_busres_board_cmd_types {
        PICMG_BUSRES_BOARD_CMD_QUERY =0,
        PICMG_BUSRES_BOARD_CMD_RELEASE,
        PICMG_BUSRES_BOARD_CMD_FORCE,
        PICMG_BUSRES_BOARD_CMD_BUS_FREE
} t_picmg_busres_board_cmd_types ;

typedef enum picmg_busres_shmc_cmd_types {
        PICMG_BUSRES_SHMC_CMD_REQUEST =0,
        PICMG_BUSRES_SHMC_CMD_RELINQUISH,
        PICMG_BUSRES_SHMC_CMD_NOTIFY
} t_picmg_busres_shmc_cmd_types ;

typedef enum picmg_busres_resource_id {
        PICMG_BUSRES_METAL_TEST_BUS_1=0,
        PICMG_BUSRES_METAL_TEST_BUS_2,
        PICMG_BUSRES_SYNC_CLOCK_GROUP_1,
        PICMG_BUSRES_SYNC_CLOCK_GROUP_2,
        PICMG_BUSRES_SYNC_CLOCK_GROUP_3
} t_picmg_busres_resource_id;

#if 0
/* the LED color capabilities */
static const char* led_color_str[] = { //__attribute__((unused)) = {
   "reserved",
   "BLUE",
   "RED",
   "GREEN",
   "AMBER",
   "ORANGE",
   "WHITE",
   "reserved"
};


static const char* amc_link_type_str[] = { // __attribute__((unused)) = {
   "RESERVED",
   "RESERVED1",
   "PCI EXPRESS",
   "ADVANCED SWITCHING1",
   "ADVANCED SWITCHING2",
   "ETHERNET",
   "RAPIDIO",
   "STORAGE",
};

static const char* amc_link_type_ext_str[][16]= { //  __attribute__((unused))
	/* FRU_PICMGEXT_AMC_LINK_TYPE_RESERVED */
	{
		"", "", "", "", "", "", "", "",   "", "", "", "", "", "", "", ""
	},
	/* FRU_PICMGEXT_AMC_LINK_TYPE_RESERVED1 */
	{
		"", "", "", "", "", "", "", "",   "", "", "", "", "", "", "", ""
	},
	/* FRU_PICMGEXT_AMC_LINK_TYPE_PCI_EXPRESS */
	{
		"Gen 1 - NSSC",
		"Gen 1 - SSC",
		"Gen 2 - NSSC",
		"Gen 2 - SSC",
		"", "", "", "",
		"", "", "", "", 
		"", "", "", ""
	},
	/* FRU_PICMGEXT_AMC_LINK_TYPE_ADVANCED_SWITCHING1 */
	{
		"Gen 1 - NSSC",
		"Gen 1 - SSC",
		"Gen 2 - NSSC",
		"Gen 2 - SSC",
		"", "", "", "",
		"", "", "", "", 
		"", "", "", ""
	},
	/* FRU_PICMGEXT_AMC_LINK_TYPE_ADVANCED_SWITCHING2 */
	{
		"Gen 1 - NSSC",
		"Gen 1 - SSC",
		"Gen 2 - NSSC",
		"Gen 2 - SSC",
		"", "", "", "",
		"", "", "", "", 
		"", "", "", ""
	},
	/* FRU_PICMGEXT_AMC_LINK_TYPE_ETHERNET */
	{
   		"1000BASE-BX (SerDES Gigabit)",
   		"10GBASE-BX410 Gigabit XAUI",
   		"", "", 
   		"", "", "", "",
		"", "", "", "", 
		"", "", "", ""
	},
	/* FRU_PICMGEXT_AMC_LINK_TYPE_RAPIDIO */
	{
   		"1.25 Gbaud transmission rate",
   		"2.5 Gbaud transmission rate",
   		"3.125 Gbaud transmission rate",
   		"", "", "", "", "",
		"", "", "", "", "", "", "", ""
	},
	/* FRU_PICMGEXT_AMC_LINK_TYPE_STORAGE */
	{
   		"Fibre Channel", 
   		"Serial ATA", 
   		"Serial Attached SCSI",
   		"", "", "", "", "",
		"", "", "", "", "", "", "", ""
	}
};
#endif

struct sAmcPortState {
#ifndef WORDS_BIGENDIAN
   unsigned short lane0       :  1;
   unsigned short lane1       :  1;
   unsigned short lane2       :  1;
   unsigned short lane3       :  1;
   unsigned short type        :  8;
   unsigned short type_ext    :  4;
   unsigned char  group_id    :  8;
#else
   unsigned char  group_id    :  8;
   unsigned short type_ext    :  4;
   unsigned short type        :  8;
   unsigned short lane3       :  1;
   unsigned short lane2       :  1;
   unsigned short lane1       :  1;
   unsigned short lane0       :  1;
#endif

   unsigned char state;
};


#endif
