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
 * @file    vita49.h
 * @brief   Vita 49 Field Definitions. 
 *
 * $Date: 2015/08/10 17:10:41 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup RTDEXSYNC
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
#ifndef VITA49_H__
#define VITA49_H__

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *                            Private constants                             * 
 ****************************************************************************/ 

/**
 * VRL size in 32-bit words (excluding the VRT payload)
 */
#define VITA49_VRL_SIZE             (3)

/**
 * Uplink VRT data packet size in 32-bit words (with only but all the supported fields)
 */
#define VITA49_UL_VRT_DATA_SIZE(PL) (6 + (PL) + 1)

/**
 * Downlink VRT data packet size in 32-bit words (with only but all the supported fields)
 */
#define VITA49_DL_VRT_DATA_SIZE(PL) (7 + (PL))

/**
 * VRT context packet size in 32-bit words (with only but all the supported fields)
 */
#define VITA49_VRT_CTX_SIZE         (7)

/**
 * VRT extension context packet size in 32-bit words (with only but all the supported fields)
 */
#define VITA49_VRT_ECTX_SIZE        (13)



/*****************************************************************************
 *  VITA 49.1 VRL Frame Definitions
 */

/*
 * VRL Frame Alignment Word (VRLP)
 */
#define VITA49_VRL_FAW_VRLP_MASK                    (0xFFFFFFFF)            ///< FAW 'VRLP' field mask
#define VITA49_VRL_FAW_VRLP_SHIFT                   (0)                     ///< FAW 'VRLP' field shift
#define VITA49_VRL_FAW_VRLP_VAL                     'VRLP'                      ///< FAW 'VRLP' value
#define VITA49_VRL_FAW_V_MASK                       (0x000000FF)            ///< FAW 'V' field mask
#define VITA49_VRL_FAW_V_SHIFT                      (24)                    ///< FAW 'V' field shift
#define VITA49_VRL_FAW_R_MASK                       (0x000000FF)            ///< FAW 'R' field mask
#define VITA49_VRL_FAW_R_SHIFT                      (16)                    ///< FAW 'R' field shift
#define VITA49_VRL_FAW_L_MASK                       (0x000000FF)            ///< FAW 'L' field mask
#define VITA49_VRL_FAW_L_SHIFT                      (8)                     ///< FAW 'L' field shift
#define VITA49_VRL_FAW_P_MASK                       (0x000000FF)            ///< FAW 'P' field mask
#define VITA49_VRL_FAW_P_SHIFT                      (0)                     ///< FAW 'P' field shift

/*
 * VRL Frame Count and Size Word 
 */
#define VITA49_VRL_HEADER_FRCOUNT_MASK              (0x00000FFF)            ///< VRL frame count field mask
#define VITA49_VRL_HEADER_FRCOUNT_SHIFT             (20)                    ///< VRL frame count field shift
#define VITA49_VRL_HEADER_FRSIZE_MASK               (0x000FFFFF)            ///< VRL frame size field mask
#define VITA49_VRL_HEADER_FRSIZE_SHIFT              (0)                     ///< VRL frame size field shift

/*
 * VRL Trailer Word
 */
#define VITA49_VRL_TRAILER_CRC_MASK                 (0xFFFFFFFF)            ///< VRL frame count field mask
#define VITA49_VRL_TRAILER_CRC_SHIFT                (0)                     ///< VRL frame count field shift
#define VITA49_VRL_TRAILER_VEND_MASK                (0xFFFFFFFF)            ///< VRL 'VEND' field mask
#define VITA49_VRL_TRAILER_VEND_SHIFT               (0)                     ///< VRL 'VEND' field shift
#define VITA49_VRL_TRAILER_VEND_VAL                 'VEND'                      ///< VRL 'VEND' value
#define VITA49_VRL_TRAILER_V_MASK                   (0x000000FF)            ///< VRL 'V' field mask
#define VITA49_VRL_TRAILER_V_SHIFT                  (24)                    ///< VRL 'V' field shift
#define VITA49_VRL_TRAILER_E_MASK                   (0x000000FF)            ///< VRL 'E' field mask
#define VITA49_VRL_TRAILER_E_SHIFT                  (16)                    ///< VRL 'E' field shift
#define VITA49_VRL_TRAILER_N_MASK                   (0x000000FF)            ///< VRL 'N' field mask
#define VITA49_VRL_TRAILER_N_SHIFT                  (8)                     ///< VRL 'N' field shift
#define VITA49_VRL_TRAILER_D_MASK                   (0x000000FF)            ///< VRL 'D' field mask
#define VITA49_VRL_TRAILER_D_SHIFT                  (0)                     ///< VRL 'D' field shift


/*****************************************************************************
 *  VITA 49 Common Field Definitions
 */

/*
 * VRT IF Data Packet Header (1 Word)
 */
#define VITA49_VRT_DATAPKT_HEADER_PKTTYPE_MASK      (0x0000000F)            ///< VRT packet type field mask
#define VITA49_VRT_DATAPKT_HEADER_PKTTYPE_SHIFT     (28)                    ///< VRT packet type field shift
#define VITA49_VRT_DATAPKT_HEADER_PKTTYPE_ULDATA    (0x1)                       ///< Uplink data packet
#define VITA49_VRT_DATAPKT_HEADER_PKTTYPE_DLDATA    (0x9)                       ///< Downlink data packet
#define VITA49_VRT_DATAPKT_HEADER_C_MASK            (0x00000001)            ///< Class identifier presence field mask
#define VITA49_VRT_DATAPKT_HEADER_C_SHIFT           (27)                    ///< Class identifier presence field shift
#define VITA49_VRT_DATAPKT_HEADER_C_ABSENT          (0x0)                       ///< Class identifier absent
#define VITA49_VRT_DATAPKT_HEADER_C_PRESENT         (0x1)                       ///< Class identifier present
#define VITA49_VRT_DATAPKT_HEADER_T_MASK            (0x00000001)            ///< Trailer word presence field mask
#define VITA49_VRT_DATAPKT_HEADER_T_SHIFT           (26)                    ///< Trailer word presence field shift
#define VITA49_VRT_DATAPKT_HEADER_T_ABSENT          (0x0)                       ///< Trailer word absent
#define VITA49_VRT_DATAPKT_HEADER_T_PRESENT         (0x1)                       ///< Trailer word present
#define VITA49_VRT_DATAPKT_HEADER_RR_MASK           (0x00000003)            ///< Reserved field mask
#define VITA49_VRT_DATAPKT_HEADER_RR_SHIFT          (24)                    ///< Reserved field shift
#define VITA49_VRT_DATAPKT_HEADER_RR_VAL            (0x3)                       ///< Reserved default value
#define VITA49_VRT_DATAPKT_HEADER_TSI_MASK          (0x00000003)            ///< Integer-seconds timestamp mode field mask
#define VITA49_VRT_DATAPKT_HEADER_TSI_SHIFT         (22)                    ///< Integer-seconds timestamp mode field shift
#define VITA49_VRT_DATAPKT_HEADER_TSI_NONE          (0x0)                       ///< No Integer-seconds Timestamp field included
#define VITA49_VRT_DATAPKT_HEADER_TSI_UTC           (0x1)                       ///< Coordinated Universal Time (UTC)
#define VITA49_VRT_DATAPKT_HEADER_TSI_GPS           (0x2)                       ///< GPS time
#define VITA49_VRT_DATAPKT_HEADER_TSI_SAMPCNT       (0x3)                       ///< Sample count
#define VITA49_VRT_DATAPKT_HEADER_TSF_MASK          (0x00000003)            ///< Fractional-seconds timestamp mode field mask
#define VITA49_VRT_DATAPKT_HEADER_TSF_SHIFT         (20)                    ///< Fractional-seconds timestamp mode field shift
#define VITA49_VRT_DATAPKT_HEADER_TSF_NONE          (0x0)                       ///< No Fractional-seconds Timestamp field included
#define VITA49_VRT_DATAPKT_HEADER_TSF_SAMPCNT       (0x1)                       ///< Sample Count Timestamp
#define VITA49_VRT_DATAPKT_HEADER_TSF_REALTIME      (0x2)                       ///< Real Time (Picoseconds) Timestamp
#define VITA49_VRT_DATAPKT_HEADER_TSF_FREERUNNING   (0x3)                       ///< Free Running Count Timestamp
#define VITA49_VRT_DATAPKT_HEADER_PKTCNT_MASK       (0x0000000F)            ///< Packet count field mask
#define VITA49_VRT_DATAPKT_HEADER_PKTCNT_SHIFT      (16)                    ///< Packet count field shift
#define VITA49_VRT_DATAPKT_HEADER_PKTSIZE_MASK      (0x0000FFFF)            ///< Packet size field mask
#define VITA49_VRT_DATAPKT_HEADER_PKTSIZE_SHIFT     (0)                     ///< Packet size field shift

/*
 * VRT Context and Extension Context Packet Header (1 Word)
 */
#define VITA49_VRT_CTXPKT_HEADER_PKTTYPE_MASK       (0x0000000F)            ///< VRT packet type field mask
#define VITA49_VRT_CTXPKT_HEADER_PKTTYPE_SHIFT      (28)                    ///< VRT packet type field shift
#define VITA49_VRT_CTXPKT_HEADER_PKTTYPE_CTX        (0x4)                       ///< context packet
#define VITA49_VRT_CTXPKT_HEADER_PKTTYPE_ECTX       (0x5)                       ///< extension context packet
#define VITA49_VRT_CTXPKT_HEADER_C_MASK             (0x00000001)            ///< Class identifier presence field mask
#define VITA49_VRT_CTXPKT_HEADER_C_SHIFT            (27)                    ///< Class identifier presence field shift
#define VITA49_VRT_CTXPKT_HEADER_C_ABSENT           (0x0)                       ///< Class identifier absent
#define VITA49_VRT_CTXPKT_HEADER_C_PRESENT          (0x1)                       ///< Class identifier present
#define VITA49_VRT_CTXPKT_HEADER_RR_MASK            (0x00000003)            ///< Reserved field mask
#define VITA49_VRT_CTXPKT_HEADER_RR_SHIFT           (25)                    ///< Reserved field shift
#define VITA49_VRT_CTXPKT_HEADER_RR_VAL             (0x3)                       ///< Reserved default value
#define VITA49_VRT_CTXPKT_HEADER_TSM_MASK           (0x00000001)            ///< Precise event timing presence field mask
#define VITA49_VRT_CTXPKT_HEADER_TSM_SHIFT          (24)                    ///< Precise event timing presence field shift
#define VITA49_VRT_CTXPKT_HEADER_TSM_ABSENT         (0x0)                       ///< Precise event timing absent
#define VITA49_VRT_CTXPKT_HEADER_TSM_PRESENT        (0x1)                       ///< Precise event timing present
#define VITA49_VRT_CTXPKT_HEADER_TSI_MASK           (0x00000003)            ///< Integerseconds timestamp mode field mask
#define VITA49_VRT_CTXPKT_HEADER_TSI_SHIFT          (22)                    ///< Integerseconds timestamp mode field shift
#define VITA49_VRT_CTXPKT_HEADER_TSI_NONE           (0x0)                       ///< No Integer-seconds Timestamp field included
#define VITA49_VRT_CTXPKT_HEADER_TSI_UTC            (0x1)                       ///< Coordinated Universal Time (UTC)
#define VITA49_VRT_CTXPKT_HEADER_TSI_GPS            (0x2)                       ///< GPS time
#define VITA49_VRT_CTXPKT_HEADER_TSI_SAMPCNT        (0x3)                       ///< Sample count
#define VITA49_VRT_CTXPKT_HEADER_TSF_MASK           (0x00000003)            ///< Fractional-seconds timestamp mode field mask
#define VITA49_VRT_CTXPKT_HEADER_TSF_SHIFT          (20)                    ///< Fractional-seconds timestamp mode field shift
#define VITA49_VRT_CTXPKT_HEADER_TSF_NONE           (0x0)                       ///< No Fractional-seconds Timestamp field included
#define VITA49_VRT_CTXPKT_HEADER_TSF_SAMPCNT        (0x1)                       ///< Sample Count Timestamp
#define VITA49_VRT_CTXPKT_HEADER_TSF_REALTIME       (0x2)                       ///< Real Time (Picoseconds) Timestamp
#define VITA49_VRT_CTXPKT_HEADER_TSF_FREERUNNING    (0x3)                       ///< Free Running Count Timestamp
#define VITA49_VRT_CTXPKT_HEADER_PKTCNT_MASK        (0x0000000F)            ///< Packet count field mask
#define VITA49_VRT_CTXPKT_HEADER_PKTCNT_SHIFT       (16)                    ///< Packet count field shift
#define VITA49_VRT_CTXPKT_HEADER_PKTSIZE_MASK       (0x0000FFFF)            ///< Packet size field mask
#define VITA49_VRT_CTXPKT_HEADER_PKTSIZE_SHIFT      (0)                     ///< Packet size field shift 

/*
 * VRT Stream ID (1 Word)
 */
#define VITA49_VRT_STREAMID_MASK                    (0xFFFFFFFF)            ///< Stream ID field mask
#define VITA49_VRT_STREAMID_SHIFT                   (0)                     ///< Stream ID field shift

/*
 * VRT Class Identifier (2 Words, optional)
 */
#define VITA49_VRT_CLASSID0_RR_MASK                 (0x000000FF)            ///< Reserved field mask
#define VITA49_VRT_CLASSID0_RR_SHIFT                (24)                    ///< Reserved field shift
#define VITA49_VRT_CLASSID0_RR_VAL                  (0x000000FF)                ///< Reserved field default value
#define VITA49_VRT_CLASSID0_OUI_MASK                (0x00FFFFFF)            ///< Class specifier OUI field mask
#define VITA49_VRT_CLASSID0_OUI_SHIFT               (0)                     ///< Class specifier OUI field shift

#define VITA49_VRT_CLASSID1_INFOCC_MASK             (0x0000FFFF)            ///< Information class code field mask
#define VITA49_VRT_CLASSID1_INFOCC_SHIFT            (16)                    ///< Information class code field shift
#define VITA49_VRT_CLASSID1_INFOCC_UNSPECIFIED      (0x0000)                    ///< Unspecified information class code
#define VITA49_VRT_CLASSID1_PKTCC_MASK              (0x0000FFFF)            ///< Packet class code field mask
#define VITA49_VRT_CLASSID1_PKTCC_SHIFT             (0)                     ///< Packet class code field shift
#define VITA49_VRT_CLASSID1_PKTCC_UNSPECIFIED       (0x0000)                    ///< Unspecified packet class code

/*
 * VRT Trailer (1 Word)
 */
#define VITA49_VRT_TRAILER_ENABLES_MASK             (0x00000FFF)            ///< Event indicator enable field mask
#define VITA49_VRT_TRAILER_ENABLES_SHIFT            (20)                    ///< Event indicator enable field mask
#define VITA49_VRT_TRAILER_ENABLES_CALTIMIND        (0x800)                     ///< Calibrated time indicator enabled
#define VITA49_VRT_TRAILER_ENABLES_VALIDDATAIND     (0x400)                     ///< Valid data indicator enabled
#define VITA49_VRT_TRAILER_ENABLES_REFLOCKIND       (0x200)                     ///< Reference lock indicator enabled
#define VITA49_VRT_TRAILER_ENABLES_SAMPLOSSIND      (0x010)                     ///< Sample loss indicator enabled
#define VITA49_VRT_TRAILER_ENABLES_CHSTATIND        (0x001)                     ///< Channel state indicator enabled
#define VITA49_VRT_TRAILER_CALTIMIND_MASK           (0x00000001)            ///< Calibrated time indicator field mask
#define VITA49_VRT_TRAILER_CALTIMIND_SHIFT          (19)                    ///< Calibrated time indicator field shift
#define VITA49_VRT_TRAILER_CALTIMIND_VALID          (1)                         ///< Timestamp is calibrated to some external reference
#define VITA49_VRT_TRAILER_VALIDDATAIND_MASK        (0x00000001)            ///< Valid data indicator field mask
#define VITA49_VRT_TRAILER_VALIDDATAIND_SHIFT       (18)                    ///< Valid data indicator field shift
#define VITA49_VRT_TRAILER_VALIDDATAIND_VALID       (1)                         ///< Data in the packet is valid
#define VITA49_VRT_TRAILER_REFLOCKIND_MASK          (0x00000001)            ///< Reference lock indicator field mask
#define VITA49_VRT_TRAILER_REFLOCKIND_SHIFT         (17)                    ///< Reference lock indicator field shift
#define VITA49_VRT_TRAILER_REFLOCKIND_LOCKED        (1)                         ///< All PLL are locked and stable
#define VITA49_VRT_TRAILER_SAMPLOSSIND_MASK         (0x00000001)            ///< Sample loss indicator field mask
#define VITA49_VRT_TRAILER_SAMPLOSSIND_SHIFT        (12)                    ///< Sample loss indicator field shift
#define VITA49_VRT_TRAILER_SAMPLOSSIND_LOSS         (1)                         ///< Packet contains at least one sample discontinuity
#define VITA49_VRT_TRAILER_CHSTATIND_MASK           (0x00000003)            ///< Channel state indicator field mask
#define VITA49_VRT_TRAILER_CHSTATIND_SHIFT          (8)                     ///< Channel state indicator field shift
#define VITA49_VRT_TRAILER_CHSTATIND_IDLE           (0)                         ///< Channel is idle
#define VITA49_VRT_TRAILER_CHSTATIND_STARTPEND      (1)                         ///< Channel start is pending
#define VITA49_VRT_TRAILER_CHSTATIND_RUNNING        (2)                         ///< Channel is running
#define VITA49_VRT_TRAILER_CHSTATIND_STOPPEND       (3)                         ///< Channel stop is pending
#define VITA49_VRT_TRAILER_E_MASK                   (0x00000001)            ///< Associate context packet count presence field mask
#define VITA49_VRT_TRAILER_E_SHIFT                  (7)                     ///< Associate context packet count presence field shift
#define VITA49_VRT_TRAILER_E_PRESENT                (0x01)                      ///< Associate context packet count present
#define VITA49_VRT_TRAILER_ECTX_PKTCNT_MASK         (0x0000007F)            ///< Associate context packet count field mask
#define VITA49_VRT_TRAILER_ECTX_PKTCNT_SHIFT        (0)                     ///< Associate context packet count field shift

/*
 * User-defined control (1 Word)
 */
#define VITA49_VRT_USERCTRL_CMD_MASK                (0x00000003)            ///< User-defined commmand field mask
#define VITA49_VRT_USERCTRL_CMD_SHIFT               (4)                     ///< User-defined commmand field shift
#define VITA49_VRT_USERCTRL_CMD_NONE                (0x0)                       ///< No command, keep going
#define VITA49_VRT_USERCTRL_CMD_KEEP                (0x0)                       ///< No command, keep going
#define VITA49_VRT_USERCTRL_CMD_START               (0x1)                       ///< Start command
#define VITA49_VRT_USERCTRL_CMD_STOP                (0x2)                       ///< Stop command
#define VITA49_VRT_USERCTRL_SYNCMODE_MASK           (0x00000003)            ///< User-defined synchronisation mode field mask
#define VITA49_VRT_USERCTRL_SYNCMODE_SHIFT          (2)                     ///< User-defined synchronisation mode field shift
#define VITA49_VRT_USERCTRL_SYNCMODE_NOW            (0x0)                       ///< Sync now
#define VITA49_VRT_USERCTRL_SYNCMODE_ONTRIGGER      (0x1)                       ///< Sync on trigger
#define VITA49_VRT_USERCTRL_SYNCMODE_ATTIME         (0x2)                       ///< Sync at time
#define VITA49_VRT_USERCTRL_TRIGSEL_MASK            (0x00000003)            ///< User-defined trigger field mask
#define VITA49_VRT_USERCTRL_TRIGSEL_SHIFT           (0)                     ///< User-defined trigger field shift
#define VITA49_VRT_USERCTRL_TRIGSEL_NA              (0x0)                       ///< No trigger
#define VITA49_VRT_USERCTRL_TRIGSEL_EXTTRIG0        (0x0)                       ///< External trigger 0
#define VITA49_VRT_USERCTRL_TRIGSEL_EXTTRIG1        (0x1)                       ///< External trigger 1
#define VITA49_VRT_USERCTRL_TRIGSEL_EXTTRIG2        (0x2)                       ///< External trigger 2
#define VITA49_VRT_USERCTRL_TRIGSEL_EXTTRIG3        (0x2)                       ///< External trigger 3

/*
 * Context Indicator
 */
#define VITA49_VRT_CTXIND_I_MASK                    (0x00000001)            ///< Context field change indicator mask
#define VITA49_VRT_CTXIND_I_SHIFT                   (31)                    ///< Context field change indicator shift
#define VITA49_VRT_CTXIND_RP_MASK                   (0x00000001)            ///< Reference point presence field mask
#define VITA49_VRT_CTXIND_RP_SHIFT                  (30)                    ///< Reference point presence field shift
#define VITA49_VRT_CTXIND_RP_PRESENT                (0x1)                       ///< Reference point present
#define VITA49_VRT_CTXIND_RFBW_MASK                 (0x00000001)            ///< RF Bandwidth presence field mask
#define VITA49_VRT_CTXIND_RFBW_SHIFT                (29)                    ///< RF Bandwidth presence field shift
#define VITA49_VRT_CTXIND_RFBW_PRESENT              (0x1)                       ///< RF Bandwidth present
#define VITA49_VRT_CTXIND_IFREFFREQ_MASK            (0x00000001)            ///< IF reference frequency presence field mask
#define VITA49_VRT_CTXIND_IFREFFREQ_SHIFT           (28)                    ///< IF reference frequency presence field 
#define VITA49_VRT_CTXIND_IFREFFREQ_PRESENT         (0x1)                       ///< IF Reference frequency present
#define VITA49_VRT_CTXIND_RFREFFREQ_MASK            (0x00000001)            ///< RF reference frequency presence field mask
#define VITA49_VRT_CTXIND_RFREFFREQ_SHIFT           (27)                    ///< RF reference frequency presence field 
#define VITA49_VRT_CTXIND_RFREFFREQ_PRESENT         (0x1)                       ///< RF Reference frequency present
#define VITA49_VRT_CTXIND_RFREFFREQOFST_MASK        (0x00000001)            ///< RF reference frequency offset presence field mask
#define VITA49_VRT_CTXIND_RFREFFREQOFST_SHIFT       (26)                    ///< RF reference frequency offset presence field 
#define VITA49_VRT_CTXIND_RFREFFREQOFST_PRESENT     (0x1)                       ///< RF Reference frequency offset present
#define VITA49_VRT_CTXIND_IFBANDOFST_MASK           (0x00000001)            ///< IF band frequency offset presence field mask
#define VITA49_VRT_CTXIND_IFBANDOFST_SHIFT          (25)                    ///< IF band frequency offset presence field 
#define VITA49_VRT_CTXIND_IFBANDOFST_PRESENT        (0x1)                       ///< IF band frequency offset present
#define VITA49_VRT_CTXIND_REFLEV_MASK               (0x00000001)            ///< Reference level presence field mask
#define VITA49_VRT_CTXIND_REFLEV_SHIFT              (24)                    ///< Reference level presence field 
#define VITA49_VRT_CTXIND_REFLEV_PRESENT            (0x1)                       ///< Reference level present
#define VITA49_VRT_CTXIND_GAIN_MASK                 (0x00000001)            ///< Gain presence field mask
#define VITA49_VRT_CTXIND_GAIN_SHIFT                (23)                    ///< Gain presence field 
#define VITA49_VRT_CTXIND_GAIN_PRESENT              (0x1)                       ///< Gain present
#define VITA49_VRT_CTXIND_OVCNT_MASK                (0x00000001)            ///< Over-rage count presence field mask
#define VITA49_VRT_CTXIND_OVCNT_SHIFT               (22)                    ///< Over-rage count presence field shift
#define VITA49_VRT_CTXIND_OVCNT_PRESENT             (0x1)                       ///< Over-rage count present
#define VITA49_VRT_CTXIND_SR_MASK                   (0x00000001)            ///< Sample rate presence field mask
#define VITA49_VRT_CTXIND_SR_SHIFT                  (21)                    ///< Sample rate presence field shift
#define VITA49_VRT_CTXIND_SR_PRESENT                (0x1)                       ///< Sample rate present 
#define VITA49_VRT_CTXIND_TA_MASK                   (0x00000001)            ///< Timestamp adjustment presence field mask
#define VITA49_VRT_CTXIND_TA_SHIFT                  (20)                    ///< Timestamp adjustment presence field shift
#define VITA49_VRT_CTXIND_TA_PRESENT                (0x1)                       ///< Timestamp adjustment present 
#define VITA49_VRT_CTXIND_TC_MASK                   (0x00000001)            ///< Timestamp calibration time presence field mask
#define VITA49_VRT_CTXIND_TC_SHIFT                  (19)                    ///< Timestamp calibration time presence field shift
#define VITA49_VRT_CTXIND_TC_PRESENT                (0x1)                       ///< Timestamp calibration time present
#define VITA49_VRT_CTXIND_TEMP_MASK                 (0x00000001)            ///< Temperature presence field mask
#define VITA49_VRT_CTXIND_TEMP_SHIFT                (18)                    ///< Temperature presence field shift
#define VITA49_VRT_CTXIND_TEMP_PRESENT              (0x1)                       ///< Temperature present
#define VITA49_VRT_CTXIND_DEVID_MASK                (0x00000001)            ///< Device identifier presence mask
#define VITA49_VRT_CTXIND_DEVID_SHIFT               (17)                    ///< Device identifier presence shift
#define VITA49_VRT_CTXIND_DEVID_PRESENT             (0x1)                       ///< Device identifier present
#define VITA49_VRT_CTXIND_STEVT_MASK                (0x00000001)            ///< State and event indicators presence mask
#define VITA49_VRT_CTXIND_STEVT_SHIFT               (16)                    ///< State and event indicators presence shift
#define VITA49_VRT_CTXIND_STEVT_PRESENT             (0x1)                       ///< State and event indicators present
#define VITA49_VRT_CTXIND_DPLFMT_MASK               (0x00000001)            ///< Data payload format presence mask
#define VITA49_VRT_CTXIND_DPLFMT_SHIFT              (15)                    ///< Data payload format presence shift
#define VITA49_VRT_CTXIND_DPLFMT_PRESENT            (0x1)                       ///< Data payload format present
#define VITA49_VRT_CTXIND_GPS_MASK                  (0x00000001)            ///< Formatted GPS geolocation presence mask
#define VITA49_VRT_CTXIND_GPS_SHIFT                 (14)                    ///< Formatted GPS geolocation presence shift
#define VITA49_VRT_CTXIND_GPS_PRESENT               (0x1)                       ///< Formatted GPS geolocation present
#define VITA49_VRT_CTXIND_INS_MASK                  (0x00000001)            ///< Formatted INS geolocation presence mask
#define VITA49_VRT_CTXIND_INS_SHIFT                 (13)                    ///< Formatted INS geolocation presence shift
#define VITA49_VRT_CTXIND_INS_PRESENT               (0x1)                       ///< Formatted INS geolocation present
#define VITA49_VRT_CTXIND_ECEFE_MASK                (0x00000001)            ///< ECEF (earth-centered, earth-fixed) ephemeris presence mask
#define VITA49_VRT_CTXIND_ECEFE_SHIFT               (12)                    ///< ECEF (earth-centered, earth-fixed) ephemeris presence shift
#define VITA49_VRT_CTXIND_ECEFE_PRESENT             (0x1)                       ///< ECEF (earth-centered, earth-fixed) ephemeris present
#define VITA49_VRT_CTXIND_RELE_MASK                 (0x00000001)            ///< Relative ephemeris presence mask
#define VITA49_VRT_CTXIND_RELE_SHIFT                (11)                    ///< Relative ephemeris presence shift
#define VITA49_VRT_CTXIND_RELE_PRESENT              (0x1)                       ///< Relative ephemeris present
#define VITA49_VRT_CTXIND_EREFID_MASK               (0x00000001)            ///< Ephemeris reference identifier presence mask
#define VITA49_VRT_CTXIND_EREFID_SHIFT              (10)                    ///< Ephemeris reference identifier presence shift
#define VITA49_VRT_CTXIND_EREFID_PRESENT            (0x1)                       ///< Ephemeris reference identifier present
#define VITA49_VRT_CTXIND_GPSA_MASK                 (0x00000001)            ///< GPS ASCII presence mask
#define VITA49_VRT_CTXIND_GPSA_SHIFT                (9)                     ///< GPS ASCII presence shift
#define VITA49_VRT_CTXIND_GPSA_PRESENT              (0x1)                       ///< GPS ASCII present
#define VITA49_VRT_CTXIND_CA_MASK                   (0x00000001)            ///< Context association lists presence mask
#define VITA49_VRT_CTXIND_CA_SHIFT                  (8)                     ///< Context association lists presence shift
#define VITA49_VRT_CTXIND_CA_PRESENT                (0x1)                       ///< Context association lists present
#define VITA49_VRT_CTXIND_RR_MASK                   (0x000000FF)            ///< Reserved bits mask
#define VITA49_VRT_CTXIND_RR_SHIFT                  (0)                     ///< Reserved bits shift
#define VITA49_VRT_CTXIND_RR_DEFAULT                (0x00)                       ///< Reserved bits default cvalue

/*
 * State and Event Indicator
 */
#define VITA49_VRT_STEVT_ENABLES_MASK               (0x0FF000F)             ///< Event indicator enable field mask
#define VITA49_VRT_STEVT_ENABLES_SHIFT              (4)                     ///< Event indicator enable field shift
#define VITA49_VRT_STEVT_ENABLES_CALTIMIND          (0x800000)                  ///< Calibrated time indicator enabled
#define VITA49_VRT_STEVT_ENABLES_VALIDDATAIND       (0x400000)                  ///< Valid data indicator enabled
#define VITA49_VRT_STEVT_ENABLES_REFLOCKIND         (0x200000)                  ///< Reference lock indicator enabled
#define VITA49_VRT_STEVT_ENABLES_SAMPLOSSIND        (0x100000)                  ///< Sample loss indicator enabled
#define VITA49_VRT_STEVT_ENABLES_CHSTATIND          (0x000001)                  ///< Channel state indicator enabled
#define VITA49_VRT_STEVT_CALTIMIND_MASK             (0x00000001)            ///< Calibrated time indicator field mask
#define VITA49_VRT_STEVT_CALTIMIND_SHIFT            (19)                    ///< Calibrated time indicator field shift
#define VITA49_VRT_STEVT_CALTIMIND_VALID            (1)                         ///< Timestamp is calibrated to some external reference
#define VITA49_VRT_STEVT_VALIDDATAIND_MASK          (0x00000001)            ///< Valid data indicator field mask
#define VITA49_VRT_STEVT_VALIDDATAIND_SHIFT         (18)                    ///< Valid data indicator field shift
#define VITA49_VRT_STEVT_VALIDDATAIND_VALID         (1)                         ///< Data in the packet is valid
#define VITA49_VRT_STEVT_REFLOCKIND_MASK            (0x00000001)            ///< Reference lock indicator field mask
#define VITA49_VRT_STEVT_REFLOCKIND_SHIFT           (17)                    ///< Reference lock indicator field shift
#define VITA49_VRT_STEVT_REFLOCKIND_LOCKED          (1)                         ///< All PLL are locked and stable
#define VITA49_VRT_STEVT_SAMPLOSSIND_MASK           (0x00000001)            ///< Sample loss indicator field mask
#define VITA49_VRT_STEVT_SAMPLOSSIND_SHIFT          (12)                    ///< Sample loss indicator field shift
#define VITA49_VRT_STEVT_SAMPLOSSIND_LOSS           (1)                         ///< Packet contains at least one sample discontinuity
#define VITA49_VRT_STEVT_CHSTATIND_MASK             (0x00000003)            ///< Channel state indicator field mask
#define VITA49_VRT_STEVT_CHSTATIND_SHIFT            (0)                     ///< Channel state indicator field shift
#define VITA49_VRT_STEVT_CHSTATIND_IDLE             (0)                         ///< Channel is idle
#define VITA49_VRT_STEVT_CHSTATIND_STARTPEND        (1)                         ///< Channel start is pending
#define VITA49_VRT_STEVT_CHSTATIND_RUNNING          (2)                         ///< Channel is running
#define VITA49_VRT_STEVT_CHSTATIND_STOPPEND         (3)                         ///< Channel stop is pending

/*
 * Data Payload Format (2 words)
 */
#define VITA49_VRT_PLFMT0_PACK_MASK                 (0x00000001)            ///< Packing method field mask
#define VITA49_VRT_PLFMT0_PACK_SHIFT                (31)                    ///< Packing method field shift
#define VITA49_VRT_PLFMT0_PACK_PROC                 (0)                         ///< Processing-efficient packing is used 
#define VITA49_VRT_PLFMT0_PACK_LINK                 (1)                         ///< Link-efficient packing is used
#define VITA49_VRT_PLFMT0_DTYP_MASK                 (0x00000003)            ///< Real/Complex type field mask
#define VITA49_VRT_PLFMT0_DTYP_SHIFT                (29)                    ///< Real/Complex type field shift
#define VITA49_VRT_PLFMT0_DTYP_REAL                 (0x0)                       ///< Real
#define VITA49_VRT_PLFMT0_DTYP_COMPCART             (0x1)                       ///< Complex, Cartesian
#define VITA49_VRT_PLFMT0_DTYP_COMPPOL              (0x2)                       ///< Complex, Polar
#define VITA49_VRT_PLFMT0_DMFT_MASK                 (0x0000001F)            ///< Data Item Format field mask
#define VITA49_VRT_PLFMT0_DMFT_SHIFT                (24)                    ///< Data Item Format field shift
#define VITA49_VRT_PLFMT0_DMFT_SFIX                 (0x00)                      ///< Signed Fixed-Point
#define VITA49_VRT_PLFMT0_DMFT_SVRT1E               (0x01)                      ///< Signed VRT, 1-bit exponent
#define VITA49_VRT_PLFMT0_DMFT_SVRT2E               (0x02)                      ///< Signed VRT, 2-bit exponent
#define VITA49_VRT_PLFMT0_DMFT_SVRT3E               (0x03)                      ///< Signed VRT, 3-bit exponent
#define VITA49_VRT_PLFMT0_DMFT_SVRT4E               (0x04)                      ///< Signed VRT, 4-bit exponent
#define VITA49_VRT_PLFMT0_DMFT_SVRT5E               (0x05)                      ///< Signed VRT, 5-bit exponent
#define VITA49_VRT_PLFMT0_DMFT_SVRT6E               (0x06)                      ///< Signed VRT, 6-bit exponent
#define VITA49_VRT_PLFMT0_DMFT_FLOAT                (0x0E)                      ///< IEEE-754 Single-Precision Floating-Point
#define VITA49_VRT_PLFMT0_DMFT_DOUBLE               (0x0F)                      ///< IEEE-754 Double-Precision Floating-Point
#define VITA49_VRT_PLFMT0_DMFT_UFIX                 (0x00)                      ///< Unsigned Fixed-Point
#define VITA49_VRT_PLFMT0_DMFT_UVRT1E               (0x01)                      ///< Unsigned VRT, 1-bit exponent
#define VITA49_VRT_PLFMT0_DMFT_UVRT2E               (0x02)                      ///< Unsigned VRT, 2-bit exponent
#define VITA49_VRT_PLFMT0_DMFT_UVRT3E               (0x03)                      ///< Unsigned VRT, 3-bit exponent
#define VITA49_VRT_PLFMT0_DMFT_UVRT4E               (0x04)                      ///< Unsigned VRT, 4-bit exponent
#define VITA49_VRT_PLFMT0_DMFT_UVRT5E               (0x05)                      ///< Unsigned VRT, 5-bit exponent
#define VITA49_VRT_PLFMT0_DMFT_UVRT6E               (0x06)                      ///< Unsigned VRT, 6-bit exponent
#define VITA49_VRT_PLFMT0_EVTTAGSZ_MASK             (0x00000007)            ///< Event-Tag size field mask
#define VITA49_VRT_PLFMT0_EVTTAGSZ_SHIFT            (20)                    ///< Event-Tag size field shift
#define VITA49_VRT_PLFMT0_EVTTAGSZ_OF(X)            (X)                         ///< Unsigned number equal to the Event-Tag size
#define VITA49_VRT_PLFMT0_CHTAGSZ_MASK              (0x0000000F)            ///< Channel-Tag size field mask
#define VITA49_VRT_PLFMT0_CHTAGSZ_SHIFT             (16)                    ///< Channel-Tag size field shift
#define VITA49_VRT_PLFMT0_CHTAGSZ_OF(X)             (X)                         ///< Unsigned number equal to the Channel-Tag size
#define VITA49_VRT_PLFMT0_IPACKFSZ_MASK             (0x0000003F)            ///< Item packing field size field mask
#define VITA49_VRT_PLFMT0_IPACKFSZ_SHIFT            (6)                     ///< Item packing field size field shift
#define VITA49_VRT_PLFMT0_IPACKFSZ_OF(X)            (X)                         ///< Unsigned number that is one less than the actual Item Packing Field size
#define VITA49_VRT_PLFMT0_DATAISZ_MASK              (0x0000003F)            ///< Data item size field mask
#define VITA49_VRT_PLFMT0_DATAISZ_SHIFT             (0)                     ///< Data item size field shift
#define VITA49_VRT_PLFMT0_DATAISZ_OF(X)             (X)                         ///< unsigned number that is one less than the actual Data Item size

#define VITA49_VRT_PLFMT1_REPCNT_MASK               (0x0000FFFF)            ///< Repeat count field mask
#define VITA49_VRT_PLFMT1_REPCNT_SHIFT              (16)                    ///< Repeat count field shift
#define VITA49_VRT_PLFMT1_REPCNT_OF(X)              (X)                         ///< Unsigned number that is one less than the actual Repeat Count
#define VITA49_VRT_PLFMT1_VECTSZ_MASK               (0x0000FFFF)            ///< Vector size field mask
#define VITA49_VRT_PLFMT1_VECTSZ_SHIFT              (0)                     ///< Vector size field shift
#define VITA49_VRT_PLFMT1_VECTSZ_OF(X)              (X)                         ///< Unsigned number that is one less than the actual Vector size

/*
 * Context Association List Header (2 words)
 */
#define VITA49_VRT_CALHDR0_SRCLISTSZ_MASK           (0x000001FF)            ///< Source list size field mask
#define VITA49_VRT_CALHDR0_SRCLISTSZ_SHIFT          (16)                    ///< Source list size field shift
#define VITA49_VRT_CALHDR0_SRCLISTSZ_OF(X)          (X)                         ///< Number of Stream IDs in the Source Context Association List
#define VITA49_VRT_CALHDR0_SYSLISTSZ_MASK           (0x000001FF)            ///< System list size field mask
#define VITA49_VRT_CALHDR0_SYSLISTSZ_SHIFT          (0)                     ///< System list size field shift
#define VITA49_VRT_CALHDR0_SYSLISTSZ_OF(X)          (X)                         ///< Number of Stream IDs in the System Context Association List

#define VITA49_VRT_CALHDR1_VCLISTSZ_MASK            (0x0000FFFF)            ///< Vector-component list size field mask
#define VITA49_VRT_CALHDR1_VCLISTSZ_SHIFT           (16)                    ///< Vector-component list size field shift
#define VITA49_VRT_CALHDR1_VCLISTSZ_OF(X)           (X)                         ///< Number of Stream IDs in the Vector-component Context Association List
#define VITA49_VRT_CALHDR1_A_MASK                   (0x00000001)            ///< Asynchronous-Channel Tag List presence field mask
#define VITA49_VRT_CALHDR1_A_SHIFT                  (15)                    ///< Asynchronous-Channel Tag List presence field shift
#define VITA49_VRT_CALHDR1_A_PRESENT                1                           ///< Asynchronous-Channel Tag List present
#define VITA49_VRT_CALHDR1_ACLISTSZ_MASK            (0x00007FFF)            ///< Asynchronous-channel list size field mask
#define VITA49_VRT_CALHDR1_ACLISTSZ_SHIFT           (0)                     ///< Asynchronous-channel list size field shift
#define VITA49_VRT_CALHDR1_ACLISTSZ_OF(X)           (X)                         ///< Number of Stream IDs in the Asynchronous-Channel Context Association List

/*
 * Extension Context Packet Error Indicators (1 words)
 */
#define VITA49_VRT_ECTXPKT_ERR_TSI_MASK              (0x00000003)            ///< Integer-seconds miss-timed timestamp mode field mask
#define VITA49_VRT_ECTXPKT_ERR_TSI_SHIFT             (22)                    ///< Integer-seconds miss-timed timestamp mode field shift
#define VITA49_VRT_ECTXPKT_ERR_TSI_NONE              (0x0)                       ///< No Integer-seconds Timestamp field included
#define VITA49_VRT_ECTXPKT_ERR_TSI_UTC               (0x1)                       ///< Coordinated Universal Time (UTC)
#define VITA49_VRT_ECTXPKT_ERR_TSI_GPS               (0x2)                       ///< GPS time
#define VITA49_VRT_ECTXPKT_ERR_TSI_SAMPCNT           (0x3)                       ///< Sample count
#define VITA49_VRT_ECTXPKT_ERR_TSF_MASK              (0x00000003)            ///< Fractional-seconds miss-timed timestamp mode field mask
#define VITA49_VRT_ECTXPKT_ERR_TSF_SHIFT             (20)                    ///< Fractional-seconds miss-timed timestamp mode field shift
#define VITA49_VRT_ECTXPKT_ERR_TSF_NONE              (0x0)                       ///< No Fractional-seconds Timestamp field included
#define VITA49_VRT_ECTXPKT_ERR_TSF_SAMPCNT           (0x1)                       ///< Sample Count Timestamp
#define VITA49_VRT_ECTXPKT_ERR_TSF_REALTIME          (0x2)                       ///< Real Time (Picoseconds) Timestamp
#define VITA49_VRT_ECTXPKT_ERR_TSF_FREERUNNING       (0x3)                       ///< Free Running Count Timestamp
#define VITA49_VRT_ECTXPKT_ERR_ERRIND_MASK           (0x000000C7)            ///< Error indicators field mask
#define VITA49_VRT_ECTXPKT_ERR_ERRIND_SHIFT          (0)                     ///< Error indicators field shift
#define VITA49_VRT_ECTXPKT_ERR_ERRIND_INVCMD         (0x80)                      ///< Invalid command error 
#define VITA49_VRT_ECTXPKT_ERR_ERRIND_INVTS          (0x40)                      ///< Invalid timestamp error 
#define VITA49_VRT_ECTXPKT_ERR_ERRIND_VRTPKTERR      (0x04)                      ///< VRT packet error 
#define VITA49_VRT_ECTXPKT_ERR_ERRIND_VRLFRERR       (0x02)                      ///< VRL frame error
#define VITA49_VRT_ECTXPKT_ERR_ERRIND_VRLSYNCLOST    (0x01)                      ///< VRL synchronisation lost



/**
 * VRT packet definition
 * Note that only the supported fields have been defined. 
 */
typedef struct VITA49_VrtPacket
{
    uint8_t pktType;                                ///< VRT packet type

    union
    {
        ///< VRT Uplink IF Data Packet
        struct
        {
            uint8_t  pktCnt;                        ///< VRT packet count (modulo 16)
            uint32_t u32StreamID;                   ///< Stream identifier

            ///< Timestamp
            struct
            {
                uint8_t TSI;                        ///< Integer-seconds timestamp mode
                uint32_t u32IntTs;                  ///< Integer-seconds timestamp
                uint8_t TSF;                        ///< Fractional-seconds timestamp mode
                uint64_t u64FracTs;                 ///< Fractional-seconds timestamp
            } timestamp;                         

            ///< Payload struct
            struct
            {
                uint32_t size;                      ///< Payload size (in bytes)
                uint8_t *pu8Data;                   ///< Pointer to the payload data
            } pl;                                   

            ///< Indicators
            struct
            {
                uint32_t calTimeIndEn   : 1;        ///< Calibrated time indicator enable
                uint32_t calTimeInd     : 1;        ///< Calibrated time indicator 
                uint32_t validDataIndEn : 1;        ///< Valid data indicator enable
                uint32_t validDataInd   : 1;        ///< Valid data indicator
                uint32_t refLockIndEn   : 1;        ///< Reference lock indicator enable
                uint32_t refLockInd     : 1;        ///< Reference lock indicator
                uint32_t sampLossIndEn  : 1;        ///< Sample loss indicator enable
                uint32_t sampLossInd    : 1;        ///< Sample loss indicator
                uint32_t chStateIndEn   : 1;        ///< Channel state indicator enable
                uint32_t chStateInd     : 2;        ///< Channel state indicator
            } ind;

            ///< Assiciate countext packet
            struct
            {
                int present;                        ///< Associate context packet count presence flag
                uint8_t count;                      ///< Associate context packet count 
            } aCtxPkt;
        } ulDataPkt;


        ///< VRT Downlink IF Data Packet
        struct
        {
            uint8_t  pktCnt;                        ///< VRT packet count (modulo 16)
            uint32_t u32StreamID;                   ///< Stream identifier

            ///< Timestamp
            struct
            {                
                uint8_t TSI;                        ///< Integer-seconds timestamp mode
                uint32_t u32IntTs;                  ///< Integer-seconds timestamp
                uint8_t TSF;                        ///< Fractional-seconds timestamp mode
                uint64_t u64FracTs;                 ///< Fractional-seconds timestamp
            } timestamp;                         

            ///< Payload struct
            struct
            {
                uint32_t size;                      ///< Payload size in 32-bit words
                void *pData;                        ///< Pointer to the payload data
            } pl;                                   

            ///< Operation Mode
            struct
            {
                uint8_t command;                    ///< Command (0:none, 1:start, 2:stop)
                uint8_t syncMode;                   ///< Synchronization mode (0:Now, 1:On trigger, 2:At time)
                uint8_t trigSel;                    ///< Trigger selection (External trigger [0:3])
            } opmode;
        } dlDataPkt;

        ///< VRT Context Packet
        struct
        {
            uint8_t  pktCnt;                        ///< VRT packet count (modulo 16)
            uint32_t u32StreamID;                   ///< Stream identifier

            ///< Timestamp
            struct
            {
                uint8_t general;                    ///< General timing (0:Precise timing, 1:General timing)
                uint8_t TSI;                        ///< Integer-seconds timestamp mode
                uint32_t u32IntTs;                  ///< Integer-seconds timestamp
                uint8_t TSF;                        ///< Fractional-seconds timestamp mode
                uint64_t u64FracTs;                 ///< Fractional-seconds timestamp
            } timestamp;                         

            ///< Payload format
            struct
            {
                int present;                        ///< Payload format present 
                uint8_t packMethod;                 ///< Packing method
                uint8_t dataType;                   ///< Data type
                uint8_t dataItemFmt;                ///< Data item format
                uint8_t itemPackSize;               ///< Item packing field size 
                uint8_t dataItemSize;               ///< Data item size
            } plfmt;

            ///< Sate and Event Indicators 
            struct
            {
                int present;                        ///< State and event indicator present 
                uint32_t calTimeIndEn   : 1;        ///< Calibrated time indicator enable
                uint32_t calTimeInd     : 1;        ///< Calibrated time indicator 
                uint32_t validDataIndEn : 1;        ///< Valid data indicator enable
                uint32_t validDataInd   : 1;        ///< Valid data indicator
                uint32_t refLockIndEn   : 1;        ///< Reference lock indicator enable
                uint32_t refLockInd     : 1;        ///< Reference lock indicator
                uint32_t sampLossIndEn  : 1;        ///< Sample loss indicator enable
                uint32_t sampLossInd    : 1;        ///< Sample loss indicator
                uint32_t chStateIndEn   : 1;        ///< Channel state indicator enable
                uint32_t chStateInd     : 2;        ///< Channel state indicator
            } stevt;
        } ctkPkt;

        ///< VRT Extension Context Packet
        struct
        {
            uint8_t  pktCnt;                        ///< VRT packet count (modulo 16)
            uint32_t u32StreamID;                   ///< System stream identifier

            ///< Timestamp
            struct
            {
                uint8_t general;                    ///< General timing (0:Precise timing, 1:General timing)
                uint8_t TSI;                        ///< Integer-seconds timestamp mode
                uint32_t u32IntTs;                  ///< Integer-seconds timestamp
                uint8_t TSF;                        ///< Fractional-seconds timestamp mode
                uint64_t u64FracTs;                 ///< Fractional-seconds timestamp
            } timestamp;                         

            ///< Error Indicators 
            struct
            {
                uint32_t vrlSyncLoss    : 1;        ///< VRL sync lost
                uint32_t vrlFrameErr    : 1;        ///< VRL frame error 
                uint32_t vrtPktErr      : 1;        ///< VRT packet error
                uint32_t invTimestamp   : 1;        ///< Invalid timestamp
                uint32_t invCommand     : 1;        ///< Invalid command in this state
            } err;

            // RTDEx stats
            struct
            {
                uint32_t u32LostFrameCnt;           ///< RTDEx lost frame counter
                uint32_t u32DropFrameCnt;           ///< RTDEx drop frame counter
                uint32_t u32RecvFrameCnt;           ///< RTDEx receive frame counter
                uint32_t u32XmitFrameCnt;           ///< RTDEx transmit frame counter
            } RTDEx;

            ///< Invalid Timestamp (used with Invalid timestamp indicator)
            struct
            {
                uint32_t u32IntTs;                  ///< Integer-seconds timestamp
                uint64_t u64FracTs;                 ///< Fractional-seconds timestamp
            } invTimestamp;  
        } eCtkPkt;
    } u;
} RTDExSync_VrtPacket_t;

#ifdef __cplusplus
}
#endif

#endif  //VITA49_H__

