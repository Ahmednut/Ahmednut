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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 * 
 * @file    Cdce62005_defines.h
 * @brief   Ccdce62005 PLL data and structure definitions
 *
 * $Date: 2015/10/16 14:31:21 $
 * $Revision: 1.3 $
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
#ifndef CDCE62005_DEFINES_H__
#define CDCE62005_DEFINES_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

#include "Mo1000DevCom_defines.h"

#include "carrier_service.h"
#include "carrier_service_def.h"

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

#define CDCE62005_NREALPLL 5                                 ///< Number of device real pll outputs (do not change this)
#define CDCE62005_NPLL (CDCE62005_NREALPLL + 1)              ///< Number of pll outputs including virtual one (do not change this)


// Device possible compare frequency range (pfd)
#define CDCE62005_PFDMAX    (40000000)                       ///< Maximum pfd frequency possible
#define CDCE62005_PFDMINEXT (40000)                          ///< Minimum pfd frequency possible with external loop filter
#define CDCE62005_PFDMININT (500000)                         ///< Minimum pfd frequency possible with internal loop filter


/** @name Cdce62005 module instance structure
 * 
 *  Hold all information regarding cdce62005 instance used.
 */
typedef struct Cdce62005_StructCore {
    Mo1000DevCom_stComCore * m_pCom;                               ///< Communication info
    Carrier_handle_t hCarrier;                               ///< Carrier handle
}Cdce62005_stCore;


/** @name Cdce62005 Pll lock status state
 * 
 *  Possible lock status state.   
 */
typedef enum Cdce62005_EnumPllLockStatus {
    eCdce62005PllUnlocked=0,                                 ///< Pll is not locked
    eCdce62005PllLocked=1                                    ///< Pll is locked
} Cdce62005_ePllLockStatus;


/** @name Cdce62005 Pll lock mode
 * 
 *  Possible pll lock mode.   
 */
typedef enum Cdce62005_EnumPllLockMode {
    eCdce62005DigitalPllLock=0,                              ///< Digital pll lock pin mode
    eCdce62005AnalogPllLock=1                                ///< Analog pll lock pin mode
} Cdce62005_ePllLockMode;


/** @name Cdce62005 Pll lock detect count
 * 
 *  Possible pll lock detect count.   
 */
typedef enum Cdce62005_EnumPllLockCount {
    eCdce62005PllLockCount1Pfd=0,                            ///< Lock triggers after 1 coherent lock event (1 pfd period)
    eCdce62005PllLockCount64Pfd=1                            ///< Lock triggers after 64 coherent lock event (1 pfd period)
} Cdce62005_ePllLockCount;


/** @name Cdce62005 digital Pll lock width
 * 
 *  Possible digital pll lock width.   
 */
typedef enum Cdce62005_EnumPllLockWidth {
    eCdce62005PllLockW1D5Ns=0,                               ///< Digital lock detect window of 1.5ns 
    eCdce62005PllLockW3D4Ns=4,                               ///< Digital lock detect window of 3.4ns
    eCdce62005PllLockW5D4Ns=8,                               ///< Digital lock detect window of 5.4ns
    eCdce62005PllLockW5D8Ns=1,                               ///< Digital lock detect window of 5.8ns
    eCdce62005PllLockW7D7Ns=5,                               ///< Digital lock detect window of 7.7ns
    eCdce62005PllLockW9D7Ns=9,                               ///< Digital lock detect window of 9.7ns
    eCdce62005PllLockW15D0Ns=12,                             ///< Digital lock detect window of 15.0ns
    eCdce62005PllLockW15D1Ns=2,                              ///< Digital lock detect window of 15.1ns
    eCdce62005PllLockW17D0Ns=6,                              ///< Digital lock detect window of 17.0ns
    eCdce62005PllLockW19D0Ns=10,                             ///< Digital lock detect window of 19.0ns
    eCdce62005PllLockW19D3Ns=13,                             ///< Digital lock detect window of 19.3ns
    eCdce62005PllLockW28D6Ns=14                              ///< Digital lock detect window of 28.6ns
} Cdce62005_ePllLockWidth;


/** @name Cdce62005 Input buffer Vbb type
 * 
 *  Possible input buffer vbb type.   
 */
typedef enum Cdce62005_EnumInputVbb {
    eCdce62005InputVbbNormal=0,                              ///< Normal input signal
    eCdce62005InputVbbInverted=1                             ///< Inverted input signal
} Cdce62005_eInputVbb;


/** @name Cdce62005 Input buffer termination state
 * 
 *  Possible input buffer termination state.   
 */
typedef enum Cdce62005_EnumInputTermination {
    eCdce62005InputTermEnabled=0,                            ///< Internal termination enabled
    eCdce62005InputTermDisabled=1                            ///< Internal termination disabled (external termination)
} Cdce62005_eInputTermination;


/** @name Cdce62005 Input buffer termination type
 * 
 *  Possible input buffer termination type.   
 */
typedef enum Cdce62005_EnumInputTermType {
    eCdce62005InputAcTermination=0,                          ///< Ac internal termination
    eCdce62005InputDcTermination=1                           ///< Dc internal termination
} Cdce62005_eInputTermType;


/** @name Cdce62005 Input buffer type
 * 
 *  Possible input buffer type.   
 */
typedef enum Cdce62005_EnumInputType {
    eCdce62005InputLvCmos=0,                                 ///< Lvcmos inputs type
    eCdce62005InputLvPecl=2,                                 ///< Lvpecl inputs type
    eCdce62005InputLvds=3                                    ///< Lvds inputs type
} Cdce62005_eInputType;


/** @name Cdce62005 Lvds Input failsafe state
 * 
 *  Possible lvds input failsafe state.   
 */
typedef enum Cdce62005_EnumLvdsInputFailsafe {
    eCdce62005LvdsInputFailDisable=0,                        ///< lvds failsafe disable for all inputs
    eCdce62005LvdsInputFailEnable=1,                         ///< lvds failsafe enable for all inputs
} Cdce62005_eLvdsInputFailsafe;


/** @name Cdce62005 Pll reference input configuration
 * 
 *  Possible pll reference input configuration.   
 */
typedef struct Cdce62005_StructReferenceInput {
    Cdce62005_eInputTermination InputTerm;                   ///< input termination state
    Cdce62005_eInputVbb InputVbb;                            ///< input vbb inversion state
    unsigned uInputFrequency;                                ///< input frequency
    unsigned char ucForceDiv2;                               ///< when !=0, force pll ref (smartmux) to get this reference divided by 2
} Cdce62005_stReferenceInput;


/** @name Cdce62005 Pll lock parameters
 * 
 *  Possible pll lock paramaters.   
 */
typedef struct Cdce62005_StructPllLockParams {
    Cdce62005_ePllLockMode PllLockMode;                      ///< Pll lock mode (analog/digit)
    Cdce62005_ePllLockWidth PllLockWidth;                    ///< Digital pll lock window
    Cdce62005_ePllLockCount PllLockCount;                    ///< Pll event lock count
} Cdce62005_stPllLockParams;


/** @name Cdce62005 Pll reference inputs global configuration
 * 
 *  Possible pll reference inputs global configuration.   
 */
typedef struct Cdce62005_StructGlobalReferenceInput {
    Cdce62005_eInputType InputsType;                         ///< Inputs type
    Cdce62005_eInputTermType InputsTermination;              ///< Input termination config
    Cdce62005_stReferenceInput PrimaryInput;                 ///< Specific primary input configuration
    Cdce62005_stReferenceInput SecondaryInput;               ///< Specific secondary input configuration
    Cdce62005_eLvdsInputFailsafe LvdsFailSafe;               ///< Lvds general failsafe configuration
    unsigned uAuxInputFrequency;                             ///< Auxiliary input frequency
} Cdce62005_stGlobalReferenceInput;


/** @name Cdce62005 output buffer mode
 * 
 *  Possible output buffer mode.   
 */
typedef enum Cdce62005_EnumOutBufferMode {
    eCdce62005OutModeLvCmos=0,                               ///< Output in lvds mode
    eCdce62005OutModeDisabled=1,                             ///< Output disabled
    eCdce62005OutModeLvPecl=2,                               ///< Output in lvpecl mode
    eCdce62005OutModeLvds=3,                                 ///< Output in lvds mode
    eCdce62005OutModeLvPeclHighSwing=6                       ///< Output in lvpecl high swing mode
} Cdce62005_eOutBufferMode;


/** @name Cdce62005 lvcmos output pin mode
 * 
 *  Possible lvds output pin mode.   
 */
typedef enum Cdce62005_EnumLvCmosOutPinMode {
    eCdce62005LvCmosOutPinActive=0,                          ///< Lvcmos pin in normal active state
    eCdce62005LvCmosOutPinInverted=1,                        ///< Lvcmos pin in inverted active state
    eCdce62005LvCmosOutPin3State=2,                          ///< Lvcmos pin in 3 state (HiZ)
    eCdce62005LvCmosOutPinLow=3                              ///< Lvcmos pin Low
} Cdce62005_eLvCmosOutPinMode;


/** @name Cdce62005 output reference mode
 * 
 *  Possible output reference mode.   
 */
typedef enum Cdce62005_EnumOutRefMode {
    eCdce62005OutRefModePrimary=0,                           ///< Output divider reference is primary input
    eCdce62005OutRefModeSecondary=1,                         ///< Output divider reference is secondary input
    eCdce62005OutRefModeSmartmux=2,                          ///< Output divider reference is smartmux output
    eCdce62005OutRefModeSynth=3                              ///< Output divider reference is synthetizer output (from pll)
} Cdce62005_eOutRefMode;


/** @name Cdce62005 output sync mode structure
 * 
 *  Hold all output sync modes.
 */
typedef enum Cdce62005_EnumSyncMode {
    Cdce62005_eSync6UsNextRefAndOutput=0,                    ///< Sync 6 uS delay and next rising edge of ref clock and selected output clock
    Cdce62005_eSync6UsNextOutput=1,                          ///< Sync 6 uS delay and next rising edge of selected output clock
    Cdce62005_eSyncNextRefAndOutput=2,                       ///< Sync next rising edge of ref clock and selected output clock
    Cdce62005_eSyncNextOutput=3                              ///< Sync next rising edge of selected output clock
}Cdce62005_eSyncMode;


/** @name Cdce62005 output parameters
 * 
 *  Possible output parameters.   
 */
typedef struct Cdce62005_StructOutputParams {
    Cdce62005_eOutRefMode OutputRef;                         ///< Output divider reference config
    Cdce62005_eOutBufferMode OutputMode;                     ///< Pin output mode
    Cdce62005_eLvCmosOutPinMode PLvCmosMode;                 ///< P pin lvcmos options
    Cdce62005_eLvCmosOutPinMode NLvCmosMode;                 ///< N pin lvcmos options
    unsigned uOutputFreq;                                    ///< Output frequency in Hz
    unsigned short usPhaseAdj;                               ///< Output phase adjust in deg (from 0 to 359)
} Cdce62005_stOutputParams;


/** @name Cdce62005 pll reference selection
 * 
 *  Possible pll input reference selection. We do not support automatics
 *  clock selection modes.    
 */
typedef enum Cdce62005_EnumInputRefMode {
    eCdce62005InputRefModePrimary=1,                         ///< smartmux use primary ref for pll synth
    eCdce62005InputRefModeSecondary=2,                       ///< smartmux use secondary ref for pll synth
    eCdce62005InputRefModeAux=4                              ///< smartmux use auxiliary (xtal) ref for pll synth
} Cdce62005_eInputRefMode;


/** @name Cdce62005 auxiliary output state
 * 
 *  Possible auxiliary output state.   
 */
typedef enum Cdce62005_EnumAuxState {
    eCdce62005AuxDisable=0,                                  ///< auxiliary output disable
    eCdce62005AuxEnable=1                                    ///< auxiliary output enable
} Cdce62005_eAuxState;


/** @name Cdce62005 auxiliary output select
 * 
 *  Possible auxiliary output select.   
 */
typedef enum Cdce62005_EnumAuxSelect {
    eCdce62005AuxSelect2=0,                                  ///< auxiliary output select output2 divider
    eCdce62005AuxSelect3=1                                   ///< auxiliary output select output3 divider
} Cdce62005_eAuxSelect;


/** @name Cdce62005 internal C1 values
 * 
 *  Possible internal C1 values.   
 */
typedef enum Cdce62005_EnumC1Values {
    eCdce62005C1Val0D0PF=0,                                  ///< C1 value is 0.0 pF
    eCdce62005C1Val1D5PF=1,                                  ///< C1 value is 1.5 pF
    eCdce62005C1Val6D5PF=2,                                  ///< C1 value is 6.5 pF
    eCdce62005C1Val8D0PF=3,                                  ///< C1 value is 8.0 pF
    eCdce62005C1Val10D0PF=4,                                 ///< C1 value is 10.0 pF
    eCdce62005C1Val11D5PF=5,                                 ///< C1 value is 11.5 pF
    eCdce62005C1Val16D5PF=6,                                 ///< C1 value is 16.5 pF
    eCdce62005C1Val18D0PF=7,                                 ///< C1 value is 18.0 pF
    eCdce62005C1Val21D5PF=8,                                 ///< C1 value is 21.5 pF
    eCdce62005C1Val23D0PF=9,                                 ///< C1 value is 23.0 pF
    eCdce62005C1Val28D0PF=10,                                ///< C1 value is 28.0 pF
    eCdce62005C1Val29D5PF=11,                                ///< C1 value is 29.5 pF
    eCdce62005C1Val31D5PF=12,                                ///< C1 value is 31.5 pF
    eCdce62005C1Val33D0PF=13,                                ///< C1 value is 33.0 pF
    eCdce62005C1Val37D5PF=14,                                ///< C1 value is 37.5 pF
    eCdce62005C1Val38D0PF=15,                                ///< C1 value is 38.0 pF
    eCdce62005C1Val39D0PF=16,                                ///< C1 value is 39.0 pF
    eCdce62005C1Val39D5PF=17,                                ///< C1 value is 39.5 pF
    eCdce62005C1Val44D0PF=18,                                ///< C1 value is 44.0 pF
    eCdce62005C1Val45D5PF=19,                                ///< C1 value is 45.5 pF
    eCdce62005C1Val47D5PF=20,                                ///< C1 value is 47.5 pF
    eCdce62005C1Val49D0PF=21,                                ///< C1 value is 49.0 pF
    eCdce62005C1Val54D0PF=22,                                ///< C1 value is 54.0 pF
    eCdce62005C1Val55D5PF=23,                                ///< C1 value is 55.5 pF
    eCdce62005C1Val59D0PF=24,                                ///< C1 value is 59.0 pF
    eCdce62005C1Val60D5PF=25,                                ///< C1 value is 60.5 pF
    eCdce62005C1Val65D5PF=26,                                ///< C1 value is 65.5 pF
    eCdce62005C1Val67D0PF=27,                                ///< C1 value is 67.0 pF
    eCdce62005C1Val69D0PF=28,                                ///< C1 value is 69.0 pF
    eCdce62005C1Val70D5PF=29,                                ///< C1 value is 70.5 pF
    eCdce62005C1Val75D5PF=30,                                ///< C1 value is 75.5 pF
    eCdce62005C1Val77D0PF=31                                 ///< C1 value is 77.0 pF
} Cdce62005_eC1Values;


/** @name Cdce62005 internal C2 values
 * 
 *  Possible internal C2 values.   
 */
typedef enum Cdce62005_EnumC2Values {
    eCdce62005C2Val0D0PF=0,                                  ///< C2 value is 0.0 pF
    eCdce62005C2Val12D5PF=1,                                 ///< C2 value is 12.5 pF
    eCdce62005C2Val25D0PF=2,                                 ///< C2 value is 25.0 pF
    eCdce62005C2Val37D5PF=3,                                 ///< C2 value is 37.5 pF
    eCdce62005C2Val87D0PF=4,                                 ///< C2 value is 87.0 pF
    eCdce62005C2Val95D5PF=5,                                 ///< C2 value is 95.5 pF
    eCdce62005C2Val112D0PF=6,                                ///< C2 value is 112.0 pF
    eCdce62005C2Val123D0PF=7,                                ///< C2 value is 123.0 pF
    eCdce62005C2Val124D5PF=8,                                ///< C2 value is 124.5 pF
    eCdce62005C2Val135D5PF=9,                                ///< C2 value is 135.5 pF
    eCdce62005C2Val148D0PF=10,                               ///< C2 value is 148.0 pF
    eCdce62005C2Val160D5PF=11,                               ///< C2 value is 160.5 pF
    eCdce62005C2Val210D0PF=12,                               ///< C2 value is 210.0 pF
    eCdce62005C2Val222D5PF=13,                               ///< C2 value is 222.5 pF
    eCdce62005C2Val226D0PF=14,                               ///< C2 value is 226.0 pF
    eCdce62005C2Val235D0PF=15,                               ///< C2 value is 235.0 pF
    eCdce62005C2Val238D5PF=16,                               ///< C2 value is 238.5 pF
    eCdce62005C2Val247D5PF=17,                               ///< C2 value is 247.5 pF
    eCdce62005C2Val251D0PF=18,                               ///< C2 value is 251.0 pF
    eCdce62005C2Val263D5PF=19,                               ///< C2 value is 263.5 pF
    eCdce62005C2Val313D0PF=20,                               ///< C2 value is 313.0 pF
    eCdce62005C2Val325D5PF=21,                               ///< C2 value is 325.5 pF
    eCdce62005C2Val338D0PF=22,                               ///< C2 value is 338.0 pF
    eCdce62005C2Val349D0PF=23,                               ///< C2 value is 349.0 pF
    eCdce62005C2Val350D5PF=24,                               ///< C2 value is 350.5 pF
    eCdce62005C2Val361D5PF=25,                               ///< C2 value is 361.5 pF
    eCdce62005C2Val374D0PF=26,                               ///< C2 value is 374.0 pF
    eCdce62005C2Val386D5PF=27,                               ///< C2 value is 386.5 pF
    eCdce62005C2Val436D0PF=28,                               ///< C2 value is 436.0 pF
    eCdce62005C2Val448D5PF=29,                               ///< C2 value is 448.5 pF
    eCdce62005C2Val461D0PF=30,                               ///< C2 value is 461.0 pF
    eCdce62005C2Val473D5PF=31                                ///< C2 value is 473.5 pF
} Cdce62005_eC2Values;


/** @name Cdce62005 internal C3 values
 * 
 *  Possible internal C3 values.   
 */
typedef enum Cdce62005_EnumC3Values {
    eCdce62005C3Val0D0PF=0,                                  ///< C3 value is 0.0 pF
    eCdce62005C3Val2D5PF=1,                                  ///< C3 value is 2.5 pF
    eCdce62005C3Val5D5PF=2,                                  ///< C3 value is 5.5 pF
    eCdce62005C3Val8D0PF=3,                                  ///< C3 value is 8.0 pF
    eCdce62005C3Val19D5PF=4,                                 ///< C3 value is 19.5 pF
    eCdce62005C3Val22D0PF=5,                                 ///< C3 value is 22.0 pF
    eCdce62005C3Val25D0PF=6,                                 ///< C3 value is 25.0 pF
    eCdce62005C3Val27D5PF=7,                                 ///< C3 value is 27.5 pF
    eCdce62005C3Val85D0PF=8,                                 ///< C3 value is 85.0 pF
    eCdce62005C3Val87D5PF=9,                                 ///< C3 value is 87.5 pF
    eCdce62005C3Val90D5PF=10,                                ///< C3 value is 90.5 pF
    eCdce62005C3Val93D0PF=11,                                ///< C3 value is 93.0 pF
    eCdce62005C3Val104D5PF=12,                               ///< C3 value is 104.5 pF
    eCdce62005C3Val107D0PF=13,                               ///< C3 value is 107.0 pF
    eCdce62005C3Val110D0PF=14,                               ///< C3 value is 110.0 pF
    eCdce62005C3Val112D5PF=15                                ///< C3 value is 112.5 pF
} Cdce62005_eC3Values;


/** @name Cdce62005 internal R2 values
 * 
 *  Possible internal R2 values.   
 */
typedef enum Cdce62005_EnumR2Values {
    eCdce62005R2Val127D6KO=0,                                ///< R2 value is 127.6 KOhms
    eCdce62005R2Val123D6KO=1,                                ///< R2 value is 123.6 KOhms
    eCdce62005R2Val118D6KO=2,                                ///< R2 value is 118.6 KOhms
    eCdce62005R2Val114D6KO=3,                                ///< R2 value is 114.6 KOhms
    eCdce62005R2Val107D6KO=4,                                ///< R2 value is 107.6 KOhms
    eCdce62005R2Val103D6KO=5,                                ///< R2 value is 103.6 KOhms
    eCdce62005R2Val98D6KO=6,                                 ///< R2 value is 98.6 KOhms
    eCdce62005R2Val94D6KO=7,                                 ///< R2 value is 94.6 KOhms
    eCdce62005R2Val89D4KO=8,                                 ///< R2 value is 89.4 KOhms
    eCdce62005R2Val85D4KO=9,                                 ///< R2 value is 85.4 KOhms
    eCdce62005R2Val80D4KO=10,                                ///< R2 value is 80.4 KOhms
    eCdce62005R2Val76D4KO=11,                                ///< R2 value is 76.4 KOhms
    eCdce62005R2Val71D2KO=12,                                ///< R2 value is 71.2 KOhms
    eCdce62005R2Val69D4KO=13,                                ///< R2 value is 69.4 KOhms
    eCdce62005R2Val67D2KO=14,                                ///< R2 value is 67.2 KOhms
    eCdce62005R2Val65D4KO=15,                                ///< R2 value is 65.4 KOhms
    eCdce62005R2Val62D2KO=16,                                ///< R2 value is 62.2 KOhms
    eCdce62005R2Val60D4KO=17,                                ///< R2 value is 60.4 KOhms
    eCdce62005R2Val58D2KO=18,                                ///< R2 value is 58.2 KOhms
    eCdce62005R2Val56D4KO=19,                                ///< R2 value is 56.4 KOhms
    eCdce62005R2Val51D2KO=20,                                ///< R2 value is 51.2 KOhms
    eCdce62005R2Val47D2KO=21,                                ///< R2 value is 47.2 KOhms
    eCdce62005R2Val42D2KO=22,                                ///< R2 value is 42.2 KOhms
    eCdce62005R2Val38D2KO=23,                                ///< R2 value is 38.2 KOhms
    eCdce62005R2Val33D0KO=24,                                ///< R2 value is 33.0 KOhms
    eCdce62005R2Val29D0KO=25,                                ///< R2 value is 29.0 KOhms
    eCdce62005R2Val24D0KO=26,                                ///< R2 value is 24.0 KOhms
    eCdce62005R2Val20D0KO=27,                                ///< R2 value is 20.0 KOhms
    eCdce62005R2Val13D0KO=28,                                ///< R2 value is 13.0 KOhms
    eCdce62005R2Val9D0KO=29,                                 ///< R2 value is 19.0 KOhms
    eCdce62005R2Val4D0KO=30,                                 ///< R2 value is 4.0 KOhms
    eCdce62005R2Val0D1KO=31                                  ///< R2 value is 0.1 KOhms
} Cdce62005_eR2Values;


/** @name Cdce62005 internal R3 values
 * 
 *  Possible internal R3 values.   
 */
typedef enum Cdce62005_EnumR3Values {
    eCdce62005R3Val20D0KO=0,                                 ///< R3 value is 20.0 KOhms
    eCdce62005R3Val15D0KO=1,                                 ///< R3 value is 15.0 KOhms
    eCdce62005R3Val10D0KO=2,                                 ///< R3 value is 10.0 KOhms
    eCdce62005R3Val5D0KO=3                                   ///< R3 value is 5.0 KOhms
} Cdce62005_eR3Values;


/** @name Cdce62005 charge pump current values
 * 
 *  Possible charge pump current values.   
 */
typedef enum Cdce62005_EnumChargePump {
    eCdce62005ChargePump50D0UA=0,                            ///< Charge pump current is 50.0 uA
    eCdce62005ChargePump100D0UA=1,                           ///< Charge pump current is 100.0 uA
    eCdce62005ChargePump150D0UA=2,                           ///< Charge pump current is 150.0 uA
    eCdce62005ChargePump200D0UA=3,                           ///< Charge pump current is 200.0 uA
    eCdce62005ChargePump300D0UA=4,                           ///< Charge pump current is 300.0 uA
    eCdce62005ChargePump400D0UA=5,                           ///< Charge pump current is 400.0 uA
    eCdce62005ChargePump600D0UA=6,                           ///< Charge pump current is 600.0 uA
    eCdce62005ChargePump750D0UA=7,                           ///< Charge pump current is 750.0 uA
    eCdce62005ChargePump1D0MA=8,                             ///< Charge pump current is 1.0 mA
    eCdce62005ChargePump1D25MA=9,                            ///< Charge pump current is 1.25 mA
    eCdce62005ChargePump1D5MA=10,                            ///< Charge pump current is 1.5 mA
    eCdce62005ChargePump2D0MA=11,                            ///< Charge pump current is 2.0 mA
    eCdce62005ChargePump2D5MA=12,                            ///< Charge pump current is 2.5 mA
    eCdce62005ChargePump3D0MA=13,                            ///< Charge pump current is 3.0 mA
    eCdce62005ChargePump3D5MA=14,                            ///< Charge pump current is 3.5 mA
    eCdce62005ChargePump3D75MA=15                            ///< Charge pump current is 3.75 mA
} Cdce62005_eChargePump;


/** @name Cdce62005 pll parameters structure
 * 
 *  Hold all pll parameters information.
 */
typedef struct Cdce62005_StructAuxParam {
    Cdce62005_eAuxSelect AuxSelect;                          ///< Auxiliary output selection
    Cdce62005_eAuxState AuxState;                            ///< Auxiliary output state
}Cdce62005_stAuxParam;


/** @name Cdce62005 loopfilter type
 * 
 *  Possible loopfilter type. This will also change the behavior
 *  of the pll loop filter calculation: when internal is selected
 *  it will returns an error (#eCdce62005NoFilterSolution) to manually
 *  calculates the loop filter only when a synthetized pll solution exist
 *  and the loop filter configuration cannot be found. When external
 *  loop filter is used, it will always returns an error (#eCdce62005NoFilterSolution)
 *  if the solution can be synthetized, to force external loop filter calculation.        
 */
typedef enum Cdce62005_EnumLoopFilterType {
    eCdce62005LoopFilterInt=0,                               ///< Internal (calculated when possible)  loop filter type
    eCdce62005LoopFilterExt=1,                               ///< External (manually calculated with external components)
} Cdce62005_eLoopFilterType;


/** @name Cdce62005 pll parameters structure
 * 
 *  Hold all pll parameters information.
 */
typedef struct Cdce62005_StructParam {
    Cdce62005_stGlobalReferenceInput RefInput;      ///< All reference inputs configuration
    Cdce62005_eInputRefMode SmartMuxRefSelect;      ///< Which reference input is used for the pll reference frequency
    Cdce62005_stOutputParams OutParams[CDCE62005_NPLL]; ///< Output clock parameters
    Cdce62005_eSyncMode SyncMode;                   ///< Output synchronisation mode to be used
    Cdce62005_stAuxParam AuxParams;                 ///< Auxiliary output parameters
    Cdce62005_stPllLockParams PllLockParams;        ///< Pll lock params
    Cdce62005_eLoopFilterType LoopFilterType;       ///< Loop filter type internal or external (external are always manually calculated)
    unsigned uPfdFreqMin;                           ///< Minimum pfd frequency that we wish to used (must be <=uPfdFreqMax ) and inside [CDCE62005_PFDMIN,CDCE62005_PFDMAX]
    unsigned uPfdFreqMax;                           ///< Maximum pfd frequency that we wish to used (must be >=uPfdFreqMin ) and inside [CDCE62005_PFDMIN,CDCE62005_PFDMAX]
    unsigned uRefFreq;                              ///< Calculated Pll reference frequency used for the input divider (smartmux2) for vco operation (when usReferenceDivider=1, if not it is smartmux1 frequency)
    unsigned uPfdFreq;                              ///< Calculated pfd (comparison) frequency
    unsigned uVcoFreq;                              ///< Calculated vco frequency
    unsigned uSynthFreq;                            ///< Calculated synthetized frequency
    unsigned uAuxFreq;                              ///< Calculated auxiliary output frequency
    unsigned short usFeedbackDivider;               ///< Calculated feedback divider
    unsigned char  ucBypassDivider;                 ///< Calculated bypass divider
    unsigned short usCFeedbackDivider;              ///< Calculated combined (fb * bypass) feedback divider value
    unsigned short usCFeedbackDividerIndex;         ///< Calculated combined (fb * bypass) feedback divider value index (in buffer list)
    unsigned short usReferenceDivider;              ///< Calculated reference divider (means divide by 1 to 8)
    unsigned char  ucPrimaryInputDivider;           ///< Calculated primary input pre divider (0 disable, 1: div by 1, 2: div by 2)
    unsigned char  ucSecondaryInputDivider;         ///< Calculated secondary input pre divider (0 disable, 1: div by 1, 2: div by 2)
    unsigned short usInputDivider;                  ///< Calculated input divider
    unsigned char aucOutDivider[CDCE62005_NPLL];    ///< Calculated outputs dividers
    unsigned char aucOutPhaseAdjNdx[CDCE62005_NPLL];///< Calculated outputs phase adjust index (realphase = aucOutPhaseAdjNdx[x]/aucOutDivider[x]*360 deg)
    unsigned char ucPrescaleDivider;                ///< Calculated prescaler divider
    unsigned char ucVcoBypassAll;                   ///< Calculated when !=0 means all outputs bypass the vco (fanout mode)
    unsigned char aucVcoBypass[CDCE62005_NPLL];     ///< Calculated when !=0 means this output bypass the vco (fanout mode) 
    unsigned char ucUsableRefDivider;               ///< Calculated when !=0 means the input reference divider can be used while solutionning
    short sLoopFilterIndex;                         ///< Calculated loopfilter index in the precalculated loop filter solution table
    Cdce62005_eR2Values R2Value;                    ///< Calculated loop filter R2 value
    Cdce62005_eR3Values R3Value;                    ///< Calculated loop filter R3 value
    Cdce62005_eC1Values C1Value;                    ///< Calculated loop filter C1 value
    Cdce62005_eC2Values C2Value;                    ///< Calculated loop filter C2 value
    Cdce62005_eC3Values C3Value;                    ///< Calculated loop filter C3 value
    Cdce62005_eChargePump ChargePump;               ///< Calculated loop filter charge pump current value
} Cdce62005_stParam;

// Internal register address
typedef enum Cdce62005_EnumAddr {
    eCdce62005Addr00=0,
    eCdce62005Addr01=1,
    eCdce62005Addr02=2,
    eCdce62005Addr03=3,
    eCdce62005Addr04=4,
    eCdce62005Addr05=5,
    eCdce62005Addr06=6,
    eCdce62005Addr07=7,
    eCdce62005Addr08=8
} Cdce62005_eAddr;


enum Cdce62005_eError {
    eCdce62005Ok                  = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_CDCE62005P, 0), 
    eCdce62005InvalidAddr01       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 1),    ///< Invalid register address specified location 01
    eCdce62005InvalidAddr02       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 2),    ///< Invalid register address specified location 02
    eCdce62005Ival01              = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 3),    ///< Invalid output divider register value at location 01.
    eCdce62005Divider01           = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 4),    ///< Requested frequencies cannot be generated because of divider limitation at location 01
    eCdce62005OutputFreqOver      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 5),    ///< Output frequency overrange for specified output pin mode
    eCdce62005PfdImpossible       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 6),    ///< Requested frequencies cannot be generated because of phase detector limitation
    eCdce62005NoPrescaler01       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 7),    ///< Requested frequencies cannot be generated because of prescaler limitation at location 01
    eCdce62005InvalidPrescaler    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 8),    ///< Invalid prescaler
    eCdce62005OutputDivImpossible = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 9),    ///< VCO freq is too high
    eCdce62005Overflow01          = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 10),   ///< Overflow during calculation at location 01
    eCdce62005Ival02              = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 11),   ///< Invalid output divider register value at location 02.
    eCdce62005Ival03              = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 12),   ///< Invalid output divider register value at location 03.
    eCdce62005Ival04              = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 13),   ///< Invalid output divider register value at location 04.
    eCdce62005Ival05              = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 14),   ///< Invalid output divider register value at location 05.
    eCdce62005Divider02           = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 15),   ///< Requested frequencies cannot be generated because of divider limitation at location 02
    eCdce62005Divider03           = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 16),   ///< Requested frequencies cannot be generated because of divider limitation at location 03
    eCdce62005NoPrescaler02       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 17),   ///< Requested frequencies cannot be generated because of prescaler limitation at location 02
    eCdce62005Overflow02          = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 18),   ///< Overflow during calculation at location 02
    eCdce62005BadFeedbackIndex    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 19),   ///< Bad feedback divider buffer starting index
    eCdce62005BadTimerFct         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 20),   ///< Bad timer function provided
    eCdce62005PllLockError        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 21),   ///< Pll lock validation problem
    eCdce62005BadPllRefSelect     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 22),   ///< Invalid pll input reference selection choosen
    eCdce62005BadAuxOutState      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 23),   ///< Invalid auxiliary output control state specified
    eCdce62005BadAuxOutSelect     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 24),   ///< Invalid auxiliary output selection specified
    eCdce62005BadInputType        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 25),   ///< Invalid input reference type specified
    eCdce62005BadInputTypeFreq    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 26),   ///< Invalid overrange input reference frequency according to input type specified
    eCdce62005BadInputPllFreq01   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 27),   ///< Invalid overrange pll input (smartmux) reference frequency over 500 MHz at location 01
    eCdce62005BadInputPllFreq02   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 28),   ///< Invalid overrange pll input (smartmux) reference frequency over 500 MHz at location 02
    eCdce62005NoFanoutDivider01   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 29),   ///< Output is in fanout mode, but no divider exist for the specified output frequency at location 01
    eCdce62005NoFanoutDivider02   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 30),   ///< Output is in fanout mode, but no divider exist for the specified output frequency at location 02
    eCdce62005NoFanoutDivider03   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 31),   ///< Output is in fanout mode, but no divider exist for the specified output frequency at location 03
    eCdce62005SmartMuxRefNotSup   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 32),   ///< Unsupported, output configured to use smartmux, but it's clock reference is not auxiliary clock
    eCdce62005BadPfdMinFrequency  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 33),   ///< Bad pfd minimum frequency provided
    eCdce62005BadPfdMaxFrequency  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 34),   ///< Bad pfd maximum frequency provided
    eCdce62005BadInputPreDivider  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 35),   ///< Bad reference input pre divider used for smartmux
    eCdce62005BadPinOutMode01     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 36),   ///< Bad output pin mode type specified at location 01
    eCdce62005NoSynthSolution     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 37),   ///< No synthetized solution can be find
    eCdce62005NoFilterSolution    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 38),   ///< No loop filter solution can be find, a synthetized solution exists that requieres manual loop filter calculation
    eCdce62005BadPinOutMode02     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 39),   ///< Bad output pin mode type specified at location 02
    eCdce62005BadPinOutMode03     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 40),   ///< Bad output pin mode type specified at location 03
    eCdce62005BadPinOutMode04     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 41),   ///< Bad output pin mode type specified at location 04
    eCdce62005BadPinOutMode05     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 42),   ///< Bad output pin mode type specified at location 05
    eCdce62005BadPinOutMode06     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 43),   ///< Bad output pin mode type specified at location 06
    eCdce62005BadChargePump       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 44),   ///< Bad charge pump value specified
    eCdce62005BadC1               = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 45),   ///< Bad C1 value specified
    eCdce62005BadC2               = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 46),   ///< Bad C2 value specified
    eCdce62005BadC3               = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 47),   ///< Bad C3 value specified
    eCdce62005BadR2               = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 48),   ///< Bad R2 value specified
    eCdce62005BadR3               = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 49),   ///< Bad R3 value specified
    eCdce62005BadLoopFilterType   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 50),   ///< Bad loop filter type
    eCdce62005BadPriPreDiv        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 51),   ///< Bad primary input pre divider
    eCdce62005BadSecPreDiv        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 52),   ///< Bad secondary input pre divider
    eCdce62005BadRefDivider       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 53),   ///< Bad reference divider
    eCdce62005BadInputRefMode     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 54),   ///< Bad smartmux (pll) input reference
    eCdce62005BadInputPinsType    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 55),   ///< Bad input pins type (must be lvcmos, lvpecl or lvds)
    eCdce62005BadPriTermCtrl      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 56),   ///< Bad primary input termination control
    eCdce62005BadSecTermCtrl      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 57),   ///< Bad secondary input termination control
    eCdce62005BadLvdsFailSafeMode = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 58),   ///< Bad inputs lvds fail safe mode
    eCdce62005BadPriVbbMode       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 59),   ///< Bad primary input Vbb mode
    eCdce62005BadSecVbbMode       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 60),   ///< Bad secondary input Vbb mode
    eCdce62005BadInputTermMode    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 61),   ///< Bad input termination mode (AC/DC)
    eCdce62005BadVcoFrequency     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 62),   ///< Bad pll vco frequency range
    eCdce62005BadPllLockPinMode   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 63),   ///< Bad pll lock pin mode
    eCdce62005BadPllLockCountMode = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 64),   ///< Bad pll lock count mode
    eCdce62005BadPllLockWidth     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 65),   ///< Bad pll lock witdh
    eCdce62005BadInputDivider     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 66),   ///< Bad input divider
    eCdce62005BadAuxOutputSelect  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 67),   ///< Bad auxiliary output selection
    eCdce62005BadAuxOutputState   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 68),   ///< Bad auxiliary output state
    eCdce62005BadSyncMode         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 69),   ///< Bad output sync mode
    eCdce62005BadOutput0Phase     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 70),   ///< Output #0 as a wrong output phase value
    eCdce62005BadOutput1Phase     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 71),   ///< Output #1 as a wrong output phase value
    eCdce62005BadOutput2Phase     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 72),   ///< Output #2 as a wrong output phase value
    eCdce62005BadOutput3Phase     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 73),   ///< Output #3 as a wrong output phase value
    eCdce62005BadOutput4Phase     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 74),   ///< Output #4 as a wrong output phase value
    eCdce62005BadOutput5Phase     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 75),   ///< Output #5 (virtual) as a wrong output phase value
    eCdce62005BadOutput0Ref       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 76),   ///< Output #0 as a wrong clock reference
    eCdce62005BadOutput1Ref       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 77),   ///< Output #1 as a wrong clock reference
    eCdce62005BadOutput2Ref       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 78),   ///< Output #2 as a wrong clock reference
    eCdce62005BadOutput3Ref       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 79),   ///< Output #3 as a wrong clock reference
    eCdce62005BadOutput4Ref       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 80),   ///< Output #4 as a wrong clock reference
    eCdce62005BadOutput5Ref       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 81),   ///< Output #5 (virtual) as a wrong clock reference
    eCdce62005BadPLvcmosOut0Mode  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 82),   ///< Output #0 as a bad positive lvcmos mode
    eCdce62005BadPLvcmosOut1Mode  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 83),   ///< Output #1 as a bad positive lvcmos mode
    eCdce62005BadPLvcmosOut2Mode  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 84),   ///< Output #2 as a bad positive lvcmos mode
    eCdce62005BadPLvcmosOut3Mode  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 85),   ///< Output #3 as a bad positive lvcmos mode
    eCdce62005BadPLvcmosOut4Mode  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 86),   ///< Output #4 as a bad positive lvcmos mode
    eCdce62005BadPLvcmosOut5Mode  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 87),   ///< Output #5 (virtual) as a bad positive lvcmos mode
    eCdce62005BadNLvcmosOut0Mode  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 88),   ///< Output #0 as a bad negative lvcmos mode
    eCdce62005BadNLvcmosOut1Mode  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 89),   ///< Output #1 as a bad negative lvcmos mode
    eCdce62005BadNLvcmosOut2Mode  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 90),   ///< Output #2 as a bad negative lvcmos mode
    eCdce62005BadNLvcmosOut3Mode  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 91),   ///< Output #3 as a bad negative lvcmos mode
    eCdce62005BadNLvcmosOut4Mode  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 92),   ///< Output #4 as a bad negative lvcmos mode
    eCdce62005BadNLvcmosOut5Mode  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 93),   ///< Output #5 (virtual) as a bad negative lvcmos mode
    eCdce62005BadOutput0PhaseVal  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 94),   ///< Output #0 as a wrong output phase value
    eCdce62005BadOutput1PhaseVal  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 95),   ///< Output #1 as a wrong output phase value
    eCdce62005BadOutput2PhaseVal  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 96),   ///< Output #2 as a wrong output phase value
    eCdce62005BadOutput3PhaseVal  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 97),   ///< Output #3 as a wrong output phase value
    eCdce62005BadOutput4PhaseVal  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 98),   ///< Output #4 as a wrong output phase value
    eCdce62005BadOutput5PhaseVal  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005P, 99)    ///< Output #5 (virtual) as a wrong output phase value
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

