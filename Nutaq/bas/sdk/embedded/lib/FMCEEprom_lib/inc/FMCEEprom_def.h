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
 * @file       FMCEEprom_def.h
 * @brief      fmceeprom library related data and structure definitions.
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

#ifndef __FMCEEPROM_DEF_H__
#define __FMCEEPROM_DEF_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

#define FMCEEPROM_IEEMAX_SIZE        1272    ///< Maximum possible internal eeprom section size 
#define FMCEEPROM_MAXFIELD_SIZE      128     ///< Maximum possible typelen field size when decoded
#define FMCEEPROM_MAXFIELD_USIZE     64      ///< Maximum possible typelen field size when undecoded
#define FMCEEPROM_MAX_CHASSIS_FIELDS 7       ///< Maximum supported typelen possible fields in chassis section
#define FMCEEPROM_MAX_BOARD_FIELDS   10      ///< Maximum supported typelen possible fields in board section 
#define FMCEEPROM_MAX_PROD_FIELDS    12      ///< Maximum supported typelen possible fields in product section 
#define FMCEEPROM_MAX_MREC_POWER     16      ///< Maximum supported mutirecord load and dc outputs record
#define FMCEEPROM_MANUF_DATE_STRLEN  256     ///< Maximum string length of manufacturer date 
#define FMCEEPROM_MAX_I2C_DEVICES    84      ///< Maximum number of vita i2c strings devices 

#define FMCEEPROM_IVERSION 1          ///< Internal custom version current version


/** @name FMCEEPROM section field presence info
 * 
 *  Possible fmceeprom section and fields presence status information.   
 */
typedef enum FMCEEPROM_E_presence {
    FMCEEPROM_PRE_ABSENT=0,             ///< Field/section is absent
    FMCEEPROM_PRE_PRESENT=1,            ///< Field/section is present without error
    FMCEEPROM_PRE_CHKERROR=2,           ///< Field/section is present with checksum error
    FMCEEPROM_PRE_OVRERROR=3,           ///< Field/section is present with overflow error
    FMCEEPROM_PRE_PARERROR=4,           ///< Field/section is present but it is partial (incomplete)
    FMCEEPROM_PRE_TLFERROR=5,           ///< Field/section is present but type len error
    FMCEEPROM_PRE_OTHERROR=6            ///< Field/section is present with other general error
}FMCEEPROM_presence;


/** @name FMCEEPROM type len bit fields encoding
 * 
 *  Possible fmceeprom type len fields.   
 */
typedef enum FMCEEPROM_E_typelen {
    FMCEEPROM_BINARY=0,               ///< Binary type data (unpacked)
    FMCEEPROM_BCDP=1,                 ///< BCD plus type packed data (2 chars every byte)
    FMCEEPROM_6ASCII=2,               ///< 6 bits ascii packed data (4 chars every 3 chars)
    FMCEEPROM_ASCII=3                 ///< ASCII (or Unicode 2bytes/char) characters, decoding related to language
}FMCEEPROM_typelen;


/** @name FMCEEPROM chassis type len fields list
 * 
 *  Possible fmceeprom chassis section type len fields.
 *  Must be lower than FMCEEPROM_MAX_CHASSIS_FIELDS    
 */
typedef enum FMCEEPROM_E_chassis_typelenf {
    FMCEEPROM_CHASSIS_FIRST=0,        ///< Chassis first field indicator
    FMCEEPROM_CHASSIS_PARTNUM=0,      ///< Chassis part number field
    FMCEEPROM_CHASSIS_SERIALNUM=1,    ///< Chassis serial number field
    FMCEEPROM_CHASSIS_CUSTOM1=2,      ///< Chassis custom field #1
    FMCEEPROM_CHASSIS_CUSTOM2=3,      ///< Chassis custom field #2
    FMCEEPROM_CHASSIS_CUSTOM3=4,      ///< Chassis custom field #3
    FMCEEPROM_CHASSIS_CUSTOM4=5,      ///< Chassis custom field #4
    FMCEEPROM_CHASSIS_CUSTOM5=6,      ///< Chassis custom field #5
    FMCEEPROM_CHASSIS_LAST=FMCEEPROM_MAX_CHASSIS_FIELDS ///< Chassis last field indicator
}FMCEEPROM_chassis_typelenf;


/** @name FMCEEPROM board type len fields list
 * 
 *  Possible fmceeprom board section type len fields.
 *  Must be lower than FMCEEPROM_MAX_BOARD_FIELDS    
 */
typedef enum FMCEEPROM_E_board_typelenf {
    FMCEEPROM_BOARD_FIRST=0,        ///< Board first field indicator
    FMCEEPROM_BOARD_MANUFACTURER=0, ///< Board manufacturer field
    FMCEEPROM_BOARD_PRODUCTNAME=1,  ///< Board product name field
    FMCEEPROM_BOARD_SERIALNUM=2,    ///< Board serial number field
    FMCEEPROM_BOARD_PARTNUM=3,      ///< Board part number field
    FMCEEPROM_BOARD_FRUFILE=4,      ///< Board fru file id field
    FMCEEPROM_BOARD_CUSTOM1=5,      ///< Board custom field #1
    FMCEEPROM_BOARD_CUSTOM2=6,      ///< Board custom field #2
    FMCEEPROM_BOARD_CUSTOM3=7,      ///< Board custom field #3
    FMCEEPROM_BOARD_CUSTOM4=8,      ///< Board custom field #4
    FMCEEPROM_BOARD_CUSTOM5=9,      ///< Board custom field #5
    FMCEEPROM_BOARD_LAST=FMCEEPROM_MAX_BOARD_FIELDS ///< Board last field indicator
}FMCEEPROM_board_typelenf;


/** @name FMCEEPROM product type len fields list
 * 
 *  Possible fmceeprom product section type len fields.
 *  Must be lower than FMCEEPROM_MAX_PROD_FIELDS    
 */
typedef enum FMCEEPROM_E_product_typelenf {
    FMCEEPROM_PROD_FIRST=0,        ///< Product first field indicator
    FMCEEPROM_PROD_MANUFACTURER=0, ///< Product manufacturer field
    FMCEEPROM_PROD_PRODUCTNAME=1,  ///< Product product name field
    FMCEEPROM_PROD_PARTNUM=2,      ///< Product part/model number field
    FMCEEPROM_PROD_PRODVERSION=3,  ///< Product version field
    FMCEEPROM_PROD_SERIALNUM=4,    ///< Product serial number field
    FMCEEPROM_PROD_ASSETTAG=5,     ///< Product asset tag field
    FMCEEPROM_PROD_FRUFILE=6,      ///< Product fru file id field
    FMCEEPROM_PROD_CUSTOM1=7,      ///< Product custom field #1
    FMCEEPROM_PROD_CUSTOM2=8,      ///< Product custom field #2
    FMCEEPROM_PROD_CUSTOM3=9,      ///< Product custom field #3
    FMCEEPROM_PROD_CUSTOM4=10,     ///< Product custom field #4
    FMCEEPROM_PROD_CUSTOM5=11,     ///< Product custom field #5
    FMCEEPROM_PROD_LAST=FMCEEPROM_MAX_PROD_FIELDS ///< Product last field indicator
}FMCEEPROM_product_typelenf;


/** @name FMCEEPROM mutirecord power (vita 57.1) records list
 * 
 *  Possible fmceeprom multirecord dc load and dc output section records.
 *  Must be lower than FMCEEPROM_MAX_MREC_POWER    
 */
typedef enum FMCEEPROM_E_mrecord_power {
    FMCEEPROM_POWER_FIRST=0,          ///< Power multirecord first type indicator
    FMCEEPROM_POWER_P1_VADJ_DCLD=0,   ///< Connector P1 Vadjust DC load type
    FMCEEPROM_POWER_P1_3P3V_DCLD=1,   ///< Connector P1 3.3V DC load type
    FMCEEPROM_POWER_P1_12P0V_DCLD=2,  ///< Connector P1 12.0V DC load type
    FMCEEPROM_POWER_P1_VIOBM2C_DCO=3, ///< Connector P1 VIO_B_M2C DC output type
    FMCEEPROM_POWER_P1_VRFAM2C_DCO=4, ///< Connector P1 VREF_A_M2C DC output type
    FMCEEPROM_POWER_P1_VRFBM2C_DCO=5, ///< Connector P1 VREF_B_M2C DC output type
    FMCEEPROM_POWER_P2_VADJ_DCLD=6,   ///< Connector P2 Vadjust DC load type
    FMCEEPROM_POWER_P2_3P3V_DCLD=7,   ///< Connector P2 3.3V DC load type
    FMCEEPROM_POWER_P2_12P0V_DCLD=8,  ///< Connector P2 12.0V DC load type
    FMCEEPROM_POWER_P2_VIOBM2C_DCO=9, ///< Connector P2 VIO_B_M2C DC output type
    FMCEEPROM_POWER_P2_VRFAM2C_DCO=10,///< Connector P2 VREF_A_M2C DC output type
    FMCEEPROM_POWER_P2_VRFBM2C_DCO=11,///< Connector P2 VREF_B_M2C DC output type
    FMCEEPROM_POWER_UNKNOWN1=12,      ///< Unknown vita power supply #1
    FMCEEPROM_POWER_UNKNOWN2=13,      ///< Unknown vita power supply #2
    FMCEEPROM_POWER_UNKNOWN3=14,      ///< Unknown vita power supply #3
    FMCEEPROM_POWER_UNKNOWN4=15,      ///< Unknown vita power supply #4
    FMCEEPROM_POWER_LAST=FMCEEPROM_MAX_MREC_POWER ///< Power multirecord last type indicator
}FMCEEPROM_mrecord_power;


/** @name FMCEEPROM vita main module size
 * 
 *  Possible fmceeprom multirecord vita main volume size information.
 */
typedef enum FMCEEPROM_E_vita_modsize {
    FMCEEPROM_VITAMODSZ_SNGWIDTH=0,      ///< Single width module
    FMCEEPROM_VITAMODSZ_DBLWIDTH=1,      ///< Double width module
    FMCEEPROM_VITAMODSZ_UNKNOWN1=2,      ///< Unknown module size
    FMCEEPROM_VITAMODSZ_UNKNOWN2=3,      ///< Unknown module size
}FMCEEPROM_vita_modsize;


/** @name FMCEEPROM vita main module connector type
 * 
 *  Possible fmceeprom multirecord vita main volume connector type information.
 */
typedef enum FMCEEPROM_E_vita_conntype {
    FMCEEPROM_VITACONNT_LPC=0,           ///< Single width module
    FMCEEPROM_VITACONNT_HPC=1,           ///< Double width module
    FMCEEPROM_VITACONNT_UNKNOWN=2,       ///< Unknown module size
    FMCEEPROM_VITACONNT_NOTFITTED=3,     ///< Unknown module size
}FMCEEPROM_vita_conntype;


/** @name FMCEEPROM vita main module connector type
 * 
 *  Possible fmceeprom multirecord vita main volume connector type information.
 */
typedef enum FMCEEPROM_E_vita_clkdir {
    FMCEEPROM_VITACLKDIR_MEZZ2CAR=0,     ///< Clock from mezzanine to carrier
    FMCEEPROM_VITACLKDIR_CAR2MEZZ=1,     ///< Clock from carrier to mezzanine
}FMCEEPROM_vita_clkdir;


/** @name FMCEEPROM mutirecord power types
 * 
 *  Possible fmceeprom multirecord dc load and dc output type indentification.
 */
typedef enum FMCEEPROM_E_power_type {
    FMCEEPROM_POWER_UNKNOWN=0,           ///< Unspecified power record type
    FMCEEPROM_POWER_DCOUTPUT=1,          ///< Multirecord dc output record type
    FMCEEPROM_POWER_DCLOAD=2             ///< Multirecord dc load record type
}FMCEEPROM_power_type;


/** @name FMCEEPROM LPC data pair pins definition for LPC connector
 * 
 *  Possible fmceeprom LPC LA data pairs.   
 */
typedef enum FMCEEPROM_E_LPC_definitions {
    FMCEEPROM_LPC_LA00=0,                  ///< LA00
    FMCEEPROM_LPC_LA01=1,                  ///< LA01
    FMCEEPROM_LPC_LA02=2,                  ///< LA02
    FMCEEPROM_LPC_LA03=3,                  ///< LA03
    FMCEEPROM_LPC_LA04=4,                  ///< LA04
    FMCEEPROM_LPC_LA05=5,                  ///< LA05
    FMCEEPROM_LPC_LA06=6,                  ///< LA06
    FMCEEPROM_LPC_LA07=7,                  ///< LA07
    FMCEEPROM_LPC_LA08=8,                  ///< LA08
    FMCEEPROM_LPC_LA09=9,                  ///< LA09
    FMCEEPROM_LPC_LA10=10,                 ///< LA10
    FMCEEPROM_LPC_LA11=11,                 ///< LA11
    FMCEEPROM_LPC_LA12=12,                 ///< LA12
    FMCEEPROM_LPC_LA13=13,                 ///< LA13
    FMCEEPROM_LPC_LA14=14,                 ///< LA14
    FMCEEPROM_LPC_LA15=15,                 ///< LA15
    FMCEEPROM_LPC_LA16=16,                 ///< LA16
    FMCEEPROM_LPC_LA17=17,                 ///< LA17
    FMCEEPROM_LPC_LA18=18,                 ///< LA18
    FMCEEPROM_LPC_LA19=19,                 ///< LA19
    FMCEEPROM_LPC_LA20=20,                 ///< LA20
    FMCEEPROM_LPC_LA21=21,                 ///< LA21
    FMCEEPROM_LPC_LA22=22,                 ///< LA22
    FMCEEPROM_LPC_LA23=23,                 ///< LA23
    FMCEEPROM_LPC_LA24=24,                 ///< LA24
    FMCEEPROM_LPC_LA25=25,                 ///< LA25
    FMCEEPROM_LPC_LA26=26,                 ///< LA26
    FMCEEPROM_LPC_LA27=27,                 ///< LA27
    FMCEEPROM_LPC_LA28=28,                 ///< LA28
    FMCEEPROM_LPC_LA29=29,                 ///< LA29
    FMCEEPROM_LPC_LA30=30,                 ///< LA30
    FMCEEPROM_LPC_LA31=31,                 ///< LA31
    FMCEEPROM_LPC_LA32=32,                 ///< LA32
    FMCEEPROM_LPC_LA33=33,                 ///< LA33
    FMCEEPROM_LPC_LAST=FMCEEPROM_LPC_LA33+1 ///< Last LPC definition
}FMCEEPROM_LPC_definitions;


/** @name FMCEEPROM HPC data pair pins definition for HPC connector
 * 
 *  Possible fmceeprom HPC HA+HB+LA data pairs.   
 */
typedef enum FMCEEPROM_E_HPC_definitions {
    FMCEEPROM_HPC_LA00=0,                  ///< LA00
    FMCEEPROM_HPC_LA01=1,                  ///< LA01
    FMCEEPROM_HPC_LA02=2,                  ///< LA02
    FMCEEPROM_HPC_LA03=3,                  ///< LA03
    FMCEEPROM_HPC_LA04=4,                  ///< LA04
    FMCEEPROM_HPC_LA05=5,                  ///< LA05
    FMCEEPROM_HPC_LA06=6,                  ///< LA06
    FMCEEPROM_HPC_LA07=7,                  ///< LA07
    FMCEEPROM_HPC_LA08=8,                  ///< LA08
    FMCEEPROM_HPC_LA09=9,                  ///< LA09
    FMCEEPROM_HPC_LA10=10,                 ///< LA10
    FMCEEPROM_HPC_LA11=11,                 ///< LA11
    FMCEEPROM_HPC_LA12=12,                 ///< LA12
    FMCEEPROM_HPC_LA13=13,                 ///< LA13
    FMCEEPROM_HPC_LA14=14,                 ///< LA14
    FMCEEPROM_HPC_LA15=15,                 ///< LA15
    FMCEEPROM_HPC_LA16=16,                 ///< LA16
    FMCEEPROM_HPC_LA17=17,                 ///< LA17
    FMCEEPROM_HPC_LA18=18,                 ///< LA18
    FMCEEPROM_HPC_LA19=19,                 ///< LA19
    FMCEEPROM_HPC_LA20=20,                 ///< LA20
    FMCEEPROM_HPC_LA21=21,                 ///< LA21
    FMCEEPROM_HPC_LA22=22,                 ///< LA22
    FMCEEPROM_HPC_LA23=23,                 ///< LA23
    FMCEEPROM_HPC_LA24=24,                 ///< LA24
    FMCEEPROM_HPC_LA25=25,                 ///< LA25
    FMCEEPROM_HPC_LA26=26,                 ///< LA26
    FMCEEPROM_HPC_LA27=27,                 ///< LA27
    FMCEEPROM_HPC_LA28=28,                 ///< LA28
    FMCEEPROM_HPC_LA29=29,                 ///< LA29
    FMCEEPROM_HPC_LA30=30,                 ///< LA30
    FMCEEPROM_HPC_LA31=31,                 ///< LA31
    FMCEEPROM_HPC_LA32=32,                 ///< LA32
    FMCEEPROM_HPC_LA33=33,                 ///< LA33
    FMCEEPROM_HPC_HA00=34,                 ///< HA00
    FMCEEPROM_HPC_HA01=35,                 ///< HA01
    FMCEEPROM_HPC_HA02=36,                 ///< HA02
    FMCEEPROM_HPC_HA03=37,                 ///< HA03
    FMCEEPROM_HPC_HA04=38,                 ///< HA04
    FMCEEPROM_HPC_HA05=39,                 ///< HA05
    FMCEEPROM_HPC_HA06=40,                 ///< HA06
    FMCEEPROM_HPC_HA07=41,                 ///< HA07
    FMCEEPROM_HPC_HA08=42,                 ///< HA08
    FMCEEPROM_HPC_HA09=43,                 ///< HA09
    FMCEEPROM_HPC_HA10=44,                 ///< HA10
    FMCEEPROM_HPC_HA11=45,                 ///< HA11
    FMCEEPROM_HPC_HA12=46,                 ///< HA12
    FMCEEPROM_HPC_HA13=47,                 ///< HA13
    FMCEEPROM_HPC_HA14=48,                 ///< HA14
    FMCEEPROM_HPC_HA15=49,                 ///< HA15
    FMCEEPROM_HPC_HA16=50,                 ///< HA16
    FMCEEPROM_HPC_HA17=51,                 ///< HA17
    FMCEEPROM_HPC_HA18=52,                 ///< HA18
    FMCEEPROM_HPC_HA19=53,                 ///< HA19
    FMCEEPROM_HPC_HA20=54,                 ///< HA20
    FMCEEPROM_HPC_HA21=55,                 ///< HA21
    FMCEEPROM_HPC_HA22=56,                 ///< HA22
    FMCEEPROM_HPC_HA23=57,                 ///< HA23
    FMCEEPROM_HPC_HB00=58,                 ///< HB00
    FMCEEPROM_HPC_HB01=59,                 ///< HB01
    FMCEEPROM_HPC_HB02=60,                 ///< HB02
    FMCEEPROM_HPC_HB03=61,                 ///< HB03
    FMCEEPROM_HPC_HB04=62,                 ///< HB04
    FMCEEPROM_HPC_HB05=63,                 ///< HB05
    FMCEEPROM_HPC_HB06=64,                 ///< HB06
    FMCEEPROM_HPC_HB07=65,                 ///< HB07
    FMCEEPROM_HPC_HB08=66,                 ///< HB08
    FMCEEPROM_HPC_HB09=67,                 ///< HB09
    FMCEEPROM_HPC_HB10=68,                 ///< HB10
    FMCEEPROM_HPC_HB11=69,                 ///< HB11
    FMCEEPROM_HPC_HB12=70,                 ///< HB12
    FMCEEPROM_HPC_HB13=71,                 ///< HB13
    FMCEEPROM_HPC_HB14=72,                 ///< HB14
    FMCEEPROM_HPC_HB15=73,                 ///< HB15
    FMCEEPROM_HPC_HB16=74,                 ///< HB16
    FMCEEPROM_HPC_HB17=75,                 ///< HB17
    FMCEEPROM_HPC_HB18=76,                 ///< HB18
    FMCEEPROM_HPC_HB19=77,                 ///< HB19
    FMCEEPROM_HPC_HB20=78,                 ///< HB20
    FMCEEPROM_HPC_HB21=79,                 ///< HB21
    FMCEEPROM_HPC_LAST=FMCEEPROM_HPC_HB21+1 ///< Last HPC pin definition
}FMCEEPROM_HPC_definitions;


/** @name FMCEEPROM multi record DC load and output types definition
 * 
 *  Hold information about a DC load or DC output multirecord type.
 */
typedef struct FMCEEPROM_S_Multi_Power
{
    FMCEEPROM_presence presence;   ///< Record presence indication
    FMCEEPROM_power_type ptype;    ///< Multirecord power voltage record type
    unsigned char standby;         ///< Multirecord dc output standby supply =1 when power is off
    int nominal_volt;              ///< Nominal voltage in 10mV
    int min_volt;                  ///< Spec minimum voltage in 10mV
    int max_volt;                  ///< Spec maximum voltage in 10mV
    unsigned int ripple_pp_volt;   ///< Spec ripple and noise peak-to-peak 10Hz to 30MHz in 1 mV
    unsigned int min_current;      ///< Minimum current load in 1mA
    unsigned int max_current;      ///< Maximum current load in 1mA
} FMCEEPROM_Multi_Power;


/** @name FMCEEPROM type len fields definition
 * 
 *  Hold information about a typelen field (variable size).
 */
typedef struct FMCEEPROM_S_Typelen_Field
{
    FMCEEPROM_presence presence;
    FMCEEPROM_typelen type;
    unsigned char rsvty:2,len:6;
    unsigned char data[FMCEEPROM_MAXFIELD_SIZE];
    unsigned char undecode[FMCEEPROM_MAXFIELD_USIZE];
} FMCEEPROM_Typelen_Field;


/** @name FMCEEPROM LPC LA pins to Hx pins information
 * 
 *  Hold information about extender board LPC delays in ps for LA data pairs to Hx data pairs.
 */
typedef struct FMCEEPROM_S_LPC_LA_Specs
{
    unsigned char presence;                          ///< Specifies LPC LA information section presence (mandatory for FMC extender boards), from FMCEEPROM_presence
    unsigned char dum_align;                         ///< For structure word alignement purpose
	unsigned short delta[FMCEEPROM_LPC_LAST];        ///< Delay in ps for each LA line on extender board type to the Hx lines on connector
	unsigned char pins[FMCEEPROM_LPC_LAST];          ///< Pins on bottom HPC extender (usually HA, HB) connector where is connected the LPC top LA pins (must be a value from FMCEEPROM_HPC_definitions)
} FMCEEPROM_LPC_LA_Specs;


/** @name FMCEEPROM HPC pins information
 * 
 *  Hold information about board HPC delays in ps for HA+HB+LA data pairs.
 */
typedef struct FMCEEPROM_S_HPC_DATA_Specs
{
    unsigned char presence;                          ///< Specifies HPC data pins information section presence (mandatory for all boards), from FMCEEPROM_presence
    unsigned char dum_align[3];                      ///< For structure word alignement purpose
	unsigned short delta[FMCEEPROM_HPC_LAST];        ///< Delay in ps for each LA line on extender board type to the Hx lines on connector
} FMCEEPROM_HPC_DATA_Specs;


/** @name FMCEEPROM Internal structure custom fields information
 *
 * Hold information about internal section custom fields information for Nutaq boards.
 * !!! WARNING: it is not allowed to remove, move or change any field
 * in IFields structure to keep compatibility between
 * different future and past versions of this eeprom section. The only
 * thing possible is to add new fields at the end of the structure: you
 * must increment iversion by 1 each time new fields
 * are added and the new structure is used in any eeprom. New fields are
 * defined at zero for old eeprom. Software must take into account that if
 * some fields are zero, that could be simply not defined. The structure is stored
 * in the eeprom in little endian format; if eventually a Big endian system is
 * reading this structure, it would need to reverse each field which is not supported now
 * isize must always hold the exact size of IFields structure (=sizeof(IFields))
 */
typedef struct FMCEEPROM_S_IFields
{
    unsigned short iversion;            ///< Current version of the structure
    unsigned short isize;               ///< Current size of the structure
    FMCEEPROM_HPC_DATA_Specs iHpcInfo;  ///< HPC signal data pairs information
	FMCEEPROM_LPC_LA_Specs iLpcLa;      ///< LPC top to bottom LA data pairs information for extender type boards
}FMCEEPROM_IFields;


/** @name FMCEEPROM internal version definition
 * 
 *  Hold eeprom custom internal version.
 */
typedef struct FMCEEPROM_S_Internal_Section
{
    FMCEEPROM_presence presence;
    unsigned char internal_area_version;
    union
    {
        unsigned char InternalZoneRaw[FMCEEPROM_IEEMAX_SIZE];
        FMCEEPROM_IFields IFields;
    };
} FMCEEPROM_Internal_Section;


/** @name FMCEEPROM chassis section definition
 * 
 *  Hold eeprom chassis section information.
 *  This section would not normally appears in a FMC module 
 */
typedef struct FMCEEPROM_S_Chassis_Section
{
    FMCEEPROM_presence presence;
    unsigned char chassis_area_version;
    unsigned char type;
    FMCEEPROM_Typelen_Field chassis_typelen_fields[FMCEEPROM_MAX_CHASSIS_FIELDS];
} FMCEEPROM_Chassis_Section;


/** @name FMCEEPROM board section definition
 * 
 *  Hold eeprom board section information.
 */
typedef struct FMCEEPROM_S_Board_Section
{
    FMCEEPROM_presence presence;
    unsigned char board_area_version;
    unsigned char language;
    unsigned char manuf_date_time[3];
    unsigned char manuf_date_time_str[FMCEEPROM_MANUF_DATE_STRLEN];
    FMCEEPROM_Typelen_Field board_typelen_fields[FMCEEPROM_MAX_BOARD_FIELDS];
} FMCEEPROM_Board_Section;


/** @name FMCEEPROM multirecord OEM VITA main section definition
 * 
 *  Hold OEM multirecord VITA main information.
 */
typedef struct FMCEEPROM_S_Multi_VitaMain
{
    FMCEEPROM_presence presence;
    unsigned char subtype:4,version:4;
    FMCEEPROM_vita_modsize modsize;
    FMCEEPROM_vita_conntype p1connsize;
    FMCEEPROM_vita_conntype p2connsize;
    FMCEEPROM_vita_clkdir clkdir;
    unsigned char rsv;
    unsigned char p1_banka_sig;
    unsigned char p1_bankb_sig;
    unsigned char p2_banka_sig;
    unsigned char p2_bankb_sig;
    unsigned char p1_gbt_sig:4,p2_gbt_sig:4;
    unsigned char max_tck_clk;
} FMCEEPROM_Multi_VitaMain;


/** @name FMCEEPROM i2c devices list (OEM VITA)
 * 
 *  Hold OEM multirecord VITA i2c devices list information.
 */
typedef struct FMCEEPROM_S_I2CDEVICE
{
    FMCEEPROM_presence presence;
    char devname[FMCEEPROM_MAX_I2C_DEVICES];
    unsigned char devnum;
    unsigned char addrmsb[FMCEEPROM_MAX_I2C_DEVICES];
    char addrmsb_dec[FMCEEPROM_MAX_I2C_DEVICES][20];
} FMCEEPROM_I2CDEVICE;


/** @name FMCEEPROM multirecord OEM VITA i2c section definition
 * 
 *  Hold OEM multirecord VITA i2c information.
 */
typedef struct FMCEEPROM_S_Multi_VitaI2C
{
    FMCEEPROM_presence presence;
    unsigned char subtype:4,version:4;
    FMCEEPROM_Typelen_Field vitai2c_str;
    unsigned char i2cdevtypenum;
    FMCEEPROM_I2CDEVICE i2cdevicetype[FMCEEPROM_MAX_I2C_DEVICES];
} FMCEEPROM_Multi_VitaI2C;


/** @name FMCEEPROM product section definition
 * 
 *  Hold eeprom product (oem) section information.
 */
typedef struct FMCEEPROM_S_Product_Section
{
    FMCEEPROM_presence presence;
    unsigned char product_area_version;
    unsigned char language;
    FMCEEPROM_Typelen_Field prod_typelen_fields[FMCEEPROM_MAX_PROD_FIELDS];
} FMCEEPROM_Product_Section;


/** @name FMCEEPROM eeprom information structure
 * 
 *  Hold all information regarding fmc eeprom.
 */
typedef struct FMCEEPROM_S_content {     
    FMCEEPROM_Internal_Section internal;     ///< EEprom Nutaq custom internal section
    FMCEEPROM_Chassis_Section chassis;       ///< EEprom standard chassis section
    FMCEEPROM_Board_Section board;           ///< EEprom standard board section
    FMCEEPROM_Product_Section product;       ///< EEprom standard product section
    FMCEEPROM_Multi_Power power[FMCEEPROM_MAX_MREC_POWER];
    FMCEEPROM_Multi_VitaMain vitamain;
    FMCEEPROM_Multi_VitaI2C vitai2c; 
} FMCEEPROM_content;

/** @name FMCEEPROM Error codes
 * 
 *  List all runtime related error codes of a FMCEEPROM library.
 */
enum FMCEEPROM_error {
    FMCEEPROM_OK                         = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_FMCEEPROM, 0), ///< No error
    FMCEEPROM_ERR_OTHER                  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 1),   ///< Incorrect fru size, cannot decode                                                                         
    FMCEEPROM_ERR_HDR_CHK                = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 2),   ///< Header chksum error                                                                               
    FMCEEPROM_ERR_IA_CHK                 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 3),   ///< Internal section checksum                                                                         
    FMCEEPROM_ERR_BD_CHK                 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 4),   ///< Board chksum error                                                                                
    FMCEEPROM_ERR_PR_CHK                 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 5),   ///< Product chksum error                                                                              
    FMCEEPROM_ERR_CH_CHK                 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 6),   ///< Chassis chksum error                                                                              
    FMCEEPROM_ERR_MLHDR_CHK              = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 7),   ///< Multirecord Header chksum error                                                                   
    FMCEEPROM_ERR_MLDAT_CHK              = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 8),   ///< Multirecord data chksum error                                                                     
    FMCEEPROM_ERR_TYPE                   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 9),   ///< Typelen error in a field                                                                          
    FMCEEPROM_ERR_OVERFLOW               = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 10),  ///< Overflow error                                                                                    
    FMCEEPROM_ERR_TLEN_ASC               = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 11),  ///< Language dependant field (ASCII) cannot have a single character                                   
    FMCEEPROM_ERR_DEC_TYPE               = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 12),  ///< Cannot decode the specified type                                                                  
    FMCEEPROM_VITA_INVALIDOEM_DATALEN    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 13),  ///< Invalid oem data len                                   
    FMCEEPROM_VITA_UNKNOWN_SUBT0_VER     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 14),  ///< Unknown subtype 0 version                              
    FMCEEPROM_VITA_INVALID_OEM_ASCII6    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 15),  ///< Invalid oem ascii 6 bits fielUnknown subtype 0 version 
    FMCEEPROM_VITA_INVALID_I2C_CHAR      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 16),  ///< Invalid oem i2c character                              
    FMCEEPROM_VITA_UNKNOWN_SUBT1_VER     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 17),  ///< Unknown subtype version                                
    FMCEEPROM_VITA_UNKNOWN_SUBT          = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 18),  ///< Unkwonw vita oem subtype                               
    FMCEEPROM_VITA_INVALIDMOD_SIZE       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 19),  ///< Invalid vita oem data module size                      
    FMCEEPROM_VITA_INVALIDP1CON_SIZE     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 20),  ///< Invalid vita oem p1 connector module size              
    FMCEEPROM_VITA_INVALIDP2CON_SIZE     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 21),  ///< Invalid vita oem p2 connector module size              
    FMCEEPROM_VITA_INVALIDP1CON_ASIGN    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 22),  ///< Invalid vita oem p1 bank A number of signals pairs     
    FMCEEPROM_VITA_INVALIDP1CON_BSIGN    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 23),  ///< Invalid vita oem p1 bank B number of signals pairs     
    FMCEEPROM_VITA_INVALIDP2CON_ASIGN    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 24),  ///< Invalid vita oem p2 bank A number of signals pairs     
    FMCEEPROM_VITA_INVALIDP2CON_BSIGN    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 25),  ///< Invalid vita oem p2 bank B number of signals pairs     
    FMCEEPROM_VITA_INVALIDTCK_VAL        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 26),  ///< Invalid TCK clock value                                
    FMCEEPROM_VITA_INVALIDP1CON_GBT      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 27),  ///< Invalid vita oem p1 gbt signals                        
    FMCEEPROM_VITA_INVALIDP2CON_GBT      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 28),  ///< Invalid vita oem p2 gbt signals                        
    FMCEEPROM_VITA_RESERVED1             = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 29),  ///< Invalid vita reserved field 1                          
    FMCEEPROM_SPD_DECODE                 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 30),  ///< EEprom format is non standard fmc vita (spd decode)                          
    FMCEEPROM_ERR_INTERNAL_SIZECHK       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 31),  ///< EEprom internal section size check failed                          
    FMCEEPROM_ERR_INTERNAL_SIZECHK2      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 32),  ///< EEprom internal section size check type 2 failed                          
    FMCEEPROM_ERR_CHASSIS_TYPE           = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 33),  ///< EEprom chassis section unknown chassis type                          
    FMCEEPROM_ERR_CHASSIS_TYPELEN        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 34),  ///< EEprom chassis typelen field error                          
    FMCEEPROM_ERR_CHASSIS_MANYFILEDS     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 35),  ///< EEprom chassis too many typelen fields                          
    FMCEEPROM_ERR_BOARD_TYPELEN          = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 36),  ///< EEprom board typelen field error                          
    FMCEEPROM_ERR_BOARD_MANYFILEDS       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 37),  ///< EEprom board too many typelen fields                          
    FMCEEPROM_ERR_PRODUCT_TYPELEN        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 38),  ///< EEprom product typelen field error                          
    FMCEEPROM_ERR_PRODUCT_MANYFILEDS     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 39),  ///< EEprom product too many typelen fields                          
    FMCEEPROM_ERR_INVALID_VITA_REQ_LEVEL = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM, 40),  ///< Invalid vita requierement level specified                          
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

