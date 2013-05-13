/* KeeloqShield - ex1239 config */
/* config defines for the sx1239 chip */

#ifndef __SX1239_CONFIG_H
#define __SX1239_CONFIG_H

#include "sx1239.h"

/*******************************************************
 *                 RECEIVER MODE
 ******************************************************/
/** Sequencer automatically matches RegOpMode.Mode setting */
#define SX1239_SEQUENCER_FORCED (1 << 7)
/** Sequencer is forced by the user */
#define SX1239_SEQUENCER_AUTO   (0 << 7)

/** Listen Mode enabled*/
#define SX1239_LISTEN_EN (1 << 6)
/** Listen Mode disabled */
#define SX1239_LISTEN_DIS (0 << 6)

/** Abort Listen mode */
#define SX1239_LISTEN_ABORT (1 << 5)

/** Receiver Sleep operating mode */
#define SX1239_MODE_SLEEP (0b000 << 2)
/** Receiver Standby operating mode */
#define SX1239_MODE_STDBY  (0b001 << 2)
/** Receiver Frequency Synthesizer operating mode */
#define SX1239_MODE_FS  (0b010 << 2)
/** Receiver Receive operating mode */
#define SX1239_MODE_RX  (0b100 << 2)

/** Default value for the RegOpMode register */
#define SX1239_OPMODE_ANS (SX1239_SEQUENCER_AUTO + SX1239_LISTEN_DIS + SX1239_MODE_RX)

/*******************************************************
 *                 MODULATION
 ******************************************************/
/** Data processing mode: Packet mode */
#define SX1239_DATAMODE_PACKET (0b00 << 5)
/** Data processing mode: Continuous mode with bit synchronizer */
#define SX1239_DATAMODE_CONTINUOUS_SYNC (0b10 << 5)
/** Data processing mode: Continuous mode without bit synchronizer */
#define SX1239_DATAMODE_CONTINUOUS_NO_SYNC (0b11 << 5)

/** FSK modulation scheme */
#define SX1239_MODULATION_FSK (0b00 << 3)
/** OOK modulation scheme */
#define SX1239_MODULATION_OOK (0b01 << 3)

/** Default value for the RegDataModul register */
#define SX1239_DATAMODE_ANS (SX1239_DATAMODE_CONTINUOUS_SYNC + SX1239_MODULATION_FSK)
//#define SX1239_DATAMODE_ANS (SX1239_DATAMODE_CONTINUOUS_NO_SYNC + SX1239_MODULATION_FSK)
//#define SX1239_DATAMODE_ANS (SX1239_DATAMODE_CONTINUOUS_NO_SYNC + SX1239_MODULATION_OOK) //SX1239_MODULATION_FSK)

/*******************************************************
 *                 CENTER FREQUENCY
 ******************************************************/
#define SX1239_FREQUENCY_31500 0x4EC000
#define SX1239_FREQUENCY_39000 0x618000
#define SX1239_FREQUENCY_43392 0x6C7AE1
#define SX1239_FREQUENCY_86830 0xD91333
#define SX1239_FREQUENCY_86832 0xD9147B
#define SX1239_FREQUENCY_91500 0xE4C000

/** Default value for the RegFrfMsb, RegFrfMid, and RegFrfLsb registers */
#define SX1239_FREQ_ANS (SX1239_FREQUENCY_43392)


/*******************************************************
 *                 SYNC detection
 ******************************************************/
/** Enable sync word detection */
#define SX1239_SYNC_EN      (1 << 7)
/** Disable sync word detection */
#define SX1239_SYNC_DIS     (0 << 7)

/** Default value for RegSyncConfig register */
#define SX1239_SYNC_ANS (SX1239_SYNC_DIS)

/***************************************************;****
 *                 RX BANDWIDTH
 ******************************************************/
/** Cut-off frequency of the DC offset canceller */
#define SX1239_BW_DCCFREQ_DEFAULT (0b010 << 5)

/** Channel filter bandwidth control */
#define SX1239_BW_MANT_16 (0b00 << 3)
/** Channel filter bandwidth control */
#define SX1239_BW_MANT_20 (0b01 << 3)
/** Channel filter bandwidth control */
#define SX1239_BW_MANT_24 (0b10 << 3)

/** Channel filter bandwidth control */
#define SX1239_BW_EXP_0 (0b000)
/** Channel filter bandwidth control */
#define SX1239_BW_EXP_1 (0b001)
/** Channel filter bandwidth control */
#define SX1239_BW_EXP_2 (0b010)
/** Channel filter bandwidth control */
#define SX1239_BW_EXP_3 (0b011)
/** Channel filter bandwidth control */
#define SX1239_BW_EXP_4 (0b100)
/** Channel filter bandwidth control */
#define SX1239_BW_EXP_5 (0b101)
/** Channel filter bandwidth control */
#define SX1239_BW_EXP_6 (0b110)
/** Channel filter bandwidth control */
#define SX1239_BW_EXP_7 (0b111)

/** Default value for RegRxBw Register */
#define SX1239_RXBW_ANS (SX1239_BW_DCCFREQ_DEFAULT + SX1239_BW_MANT_24 + SX1239_BW_EXP_0)

/*******************************************************
 *                 RSSI THRESHOLD
 ******************************************************/
/** RSSI trigger level for Rssi interrupt */
#define SX1239_RSSITHRESH_DEFAULT 0xE4

/** Default value for RegRssiThresh register */
#define SX1239_RSSI_THRESH_PEAK_ANS (SX1239_RSSITHRESH_DEFAULT)

/*******************************************************
 *                 REGOOKPEAK
 ******************************************************/
#define SX1239_THRESH_PEAK_TYPE_FIXED  0
#define SX1239_THRESH_PEAK_TYPE_PEAK  0x40
#define SX1239_THRESH_PEAK_TYPE_AVERAGE  0x80

/** Size of each decrement of the RSSI threshold in the OOK demodulator: 0.5 dB */
#define SX1239_THRESH_PEAK_STEP_05dB   0

/** Period of decrement of the RSSI threshold in the OOK demodulator: once per chip */
#define SX1239_THRESH_PEAK_DEC_0   0

/** Default value for the RegOokPeak register */
#define SX1239_OOK_PEAK_ANS (SX1239_THRESH_PEAK_TYPE_AVERAGE + SX1239_THRESH_PEAK_STEP_05dB + SX1239_THRESH_PEAK_DEC_0)

/*******************************************************
 *                 REGOOKAVG
 ******************************************************/
/** fc ? chip rate � 2? */
#define SX1239_THRESH_AVG_2   (0x03 << 6)
/** fc ? chip rate � 4? */
#define SX1239_THRESH_AVG_4   (0x02 << 6)
/** fc ? chip rate � 8? */
#define SX1239_THRESH_AVG_8   (0x01 << 6)
/** fc ? chip rate � 32? */
#define SX1239_THRESH_AVG_32  (0x00 << 6)

/** Default value for the RegOokAvg register */
#define SX1239_THRESH_AVG_ANS (SX1239_THRESH_AVG_32)

/*******************************************************
 *                 REGDIOMAPPING1
 ******************************************************/
//The mapping depends on RegOpMode.Mode (RX/SLEEP/STDBY/FS)
//as well as MODULATION (CONTINUOUS/PACKET)

#endif
