/* KeeloqShield - sxspi header */
/* sxspi device driver defines */

#ifndef __SX1239_H
#define __SX1239_H

#include <plib.h>
#include <WProgram.h>
#include "keeloq.h"
#include "keeloqalg.h"
#include "global.h"
#include "KeeloqShield.h"
#include "sxspi.h"

//SX1239 addresses
#define REGFIFO_ADDRESS         0x00
#define	REGOPMODE_ADDRESS       0x01
#define	REGDATAMODUL_ADDRESS    0x02
#define	REGBITRATEMSB_ADDRESS   0x03
#define	REGBITRATELSB_ADDRESS   0x04
#define	REGFRFMSB_ADDRESS       0x07
#define	REGFRFMID_ADDRESS       0x08
#define	REGFRFLSB_ADDRESS       0x09
#define REGVERSION_ADDRESS      0x10

//Receiver Registers
#define REGLNA_ADDRESS          0x18
#define REGRXBW_ADDRESS			0x19
#define	REGLISTEN1_ADDRESS		0x0D
#define	REGLISTEN2_ADDRESS		0x0E
#define	REGLISTEN3_ADDRESS		0x0F
/** OOK demodulator selection and control in peak mode */
#define REGOOKPEAK_ADDRESS      0x1B
/** Average threshold control of the OOK demodulator */
#define REGOOKAVG_ADDRESS       0x1C
/** Fixed threshold control of the OOK demodulator */
#define REGOOKFIX_ADDRESS       0x1D
//Packet Engine Registers
/** Sync word configuration */
#define REGSYNCCONFIG_ADDRESS   	0x2E
#define REGSYNCVALUE1_ADDRESS		0x2F
#define REGSYNCVALUE2_ADDRESS		0x30
#define	REGPACKETCONFIG1_ADDRESS	0x37
#define	REGPACKETCONFIG2_ADDRESS	0x3D
#define	REGPAYLOADLENGTH_ADDRESS	0x38
//IRQ and pin mapping Registers
#define REGDIOMAPPING1_ADDRESS		0x25
#define REGDIOMAPPING2_ADDRESS		0x26
#define REGRXTIMEOUT1_ADDRESS           0x2A
#define REGRXTIMEOUT2_ADDRESS           0x2B
//Debugging registers
#define	REGIRQFLAGS1_ADDRESS		0x27
#define	REGIRQFLAGS2_ADDRESS		0x28

#define REGRSSITHRESH_ADDRESS           0x29

//Received packet information
#define     MESSAGE_LENGTH  15 	//total received payload
#define		BUTTON_PACKET	0	//Specifies which byte in the received buffer contains the button information
#define		LEARN_BUTTON	0b00000001 //The "learn button" byte

#define         FIFOMAXSIZE   66

// Some easy-to-read bit name defines
/** Enable sync word recognition */
#define SYNCCONFIG_SYNC_ENABLE (1<<7)
/** Disable sync word recognition */
#define SYNCCONFIG_SYNC_DISABLE (0<<7)

void sx1239_init(void);
void sx1239_setFreq(uint32_t freq);
uint8_t sx1239_getVersion(void);

#endif