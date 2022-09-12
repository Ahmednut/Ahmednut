#ifndef __CCE_RTDEXSYNC__
#define __CCE_RTDEXSYNC__

#include "connection_state.h"


/*****************************************************************************
 *  RTDEx Core Register Definitions
 */

// INFO
typedef struct sRTDExSync_CoreVerId {
#ifdef _BIG_ENDIAN
    unsigned coreid:16, coreversion:16;
#else
    unsigned coreversion:16, coreid:16;
#endif
} sRTDExSync_CoreVerId_t;

// CONTROL
typedef struct sRTDExSync_Ctrl{
#ifdef _BIG_ENDIAN
	unsigned  rsvd3:20, ChIdx:4, RegIdx:4, rsvd2:1, WrEn:1, rsvd1:1, CoreReset:1;
#else
	unsigned CoreReset:1, rsvd1:1, WrEn:1, rsvd2:1, RegIdx:4, ChIdx:4, rsvd3:20;
#endif
} sRTDExSync_Ctrl_t;

typedef struct sRTDExSync_Core {
    sRTDExSync_CoreVerId_t CoreVerId;
    sRTDExSync_Ctrl_t Ctrl;
    unsigned int u32WrData;
    unsigned int u32RdData;
} sRTDExSync_Core_t;

/*****************************************************************************
 *  RTDEx Channel Register Definitions
 */

/**
 * RTDEx Channel Registers.
 * These are accessed using indirect addressing.
 */
typedef enum RTDExSync_ChReg
{
    RTDExSync_ChReg_Ctrl        = 0x0,
    RTDExSync_ChReg_RxCtrl      = 0x1,
    RTDExSync_ChReg_TxCtrl      = 0x2,
    RTDExSync_ChReg_Stat        = 0x3,
    RTDExSync_ChReg_TxCntxCtrl  = 0x4
} RTDExSync_ChReg_t;

/*
 * RTDExSync Channel Control Register Field definitions
 */
#define RTDExSync_CHREG_CTRL_PRESENT_MASK           (0x00000001)            ///< Core present flag field mask
#define RTDExSync_CHREG_CTRL_PRESENT_SHIFT          (3)                     ///< Core present flag field shift
#define RTDExSync_CHREG_CTRL_DIR_MASK               (0x00000001)            ///< Channel direction field mask
#define RTDExSync_CHREG_CTRL_DIR_SHIFT              (2)                     ///< Channel direction field shift
#define RTDExSync_CHREG_CTRL_ENABLE_MASK            (0x00000001)            ///< Channel enable field mask
#define RTDExSync_CHREG_CTRL_ENABLE_SHIFT           (1)                     ///< Channel enable field shift
#define RTDExSync_CHREG_CTRL_RESET_MASK             (0x00000001)            ///< Channel reset field mask
#define RTDExSync_CHREG_CTRL_RESET_SHIFT            (0)                     ///< Channel reset field shift

/*
 * RTDExSync RX Channel Control Register Field definitions (Host -> RTDExSync code)
 */
#define RTDExSync_CHREG_RXCTRL_FRAMESIZE_MASK       (0x000FFFFF)            ///< Receive frame size field mask
#define RTDExSync_CHREG_RXCTRL_FRAMESIZE_SHIFT      (0)                     ///< Receive frame size field shift

/*
 * RTDExSync TX Channel Control Register Field definitions (RTDExSync code -> Host)
 */
#define RTDExSync_CHREG_TXCTRL_FRAMESIZE_MASK       (0x000FFFFF)            ///< Transmit frame size field mask
#define RTDExSync_CHREG_TXCTRL_FRAMESIZE_SHIFT      (0)                     ///< Transmit frame size field shift

/*
 * RTDExSync Channel Status Register Field definitions
 */
#define RTDExSync_CHREG_STAT_ENCFIFOFULL_MASK       (0x00000001)            ///< Encoder output FIFO full field mask
#define RTDExSync_CHREG_STAT_ENCFIFOFULL_SHIFT      (19)                    ///< Encoder output FIFO full field shift
#define RTDExSync_CHREG_STAT_ENCFIFOEMPTY_MASK      (0x00000001)            ///< Encoder output FIFO empty field mask
#define RTDExSync_CHREG_STAT_ENCFIFOEMPTY_SHIFT     (18)                    ///< Encoder output FIFO empty field shift
#define RTDExSync_CHREG_STAT_DECFIFOFULL_MASK       (0x00000001)            ///< Decoder output FIFO full field mask
#define RTDExSync_CHREG_STAT_DECFIFOFULL_SHIFT      (17)                    ///< Decoder output FIFO full field shift
#define RTDExSync_CHREG_STAT_DECFIFOEMPTY_MASK      (0x00000001)            ///< Decoder output FIFO empty field mask
#define RTDExSync_CHREG_STAT_DECFIFOEMPTY_SHIFT     (16)                    ///< Decoder output FIFO empty field shift
#define RTDExSync_CHREG_STAT_TRIGERRCODE_MASK       (0x000000FF)            ///< Trigger error code field mask
#define RTDExSync_CHREG_STAT_TRIGERRCODE_SHIFT      (18)                    ///< Trigger error code field shift
#define RTDExSync_CHREG_STAT_DECERRCODE_MASK        (0x000000FF)            ///< Decoder error code field mask
#define RTDExSync_CHREG_STAT_DECERRCODE_SHIFT       (0)                     ///< Decoder error code field shift



/*****************************************************************************
 *  RTDEx Sync public functions
 */

Result RTDExSync_Presence_recv(connection_state * state); 
Result RTDExSync_CoreReset_recv(connection_state * state);
Result RTDExSync_ChannelReset_recv(connection_state * state);
Result RTDExSync_ChannelStart_recv(connection_state * state);
Result RTDExSync_ChannelStop_recv(connection_state * state);
Result RTDExSync_ChannelInfo_recv(connection_state * state);

#endif
