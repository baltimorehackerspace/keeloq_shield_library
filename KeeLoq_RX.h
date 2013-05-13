/* KeeloqShield - KeeLoq_RX */
/* Keeloq rf encoding rx header */

#ifndef _KEELOQ_RX_H
#define _KEELOQ_RX_H

#include <plib.h>
#include <WProgram.h>
#include "keeloq.h"
#include "global.h"
#include "keeloqalg.h"
#include "KeeloqShield.h"
#include "eepromhandler.h"

/** TMR0 Interrrupt routine. Receives incoming trasnmission */
void rxi(void);
/** Initalizes receiver routine */
void InitReceiver(void);

//defines
#define	KeeLoq   1
#define	AES      2
#define	XTEA     3
enum ENCODER
{
	KEELOQ_PACKET   = 1,
	AES_PACKET      = 2,
    XTEA_PACKET     = 3
};

#define KNBIT           65       // Number of bits for Keeloq - 1
#define XNBIT           100       // Number of bits for XTEA -1 104
#define ANBIT           160       // Number of bits for AES - 1 176

#define TRFreset    	0       // State table defines
#define TRFSYNC     	1
#define TRFUNO      	2
#define TRFZERO     	3
#define TRFSYNC0		4

/** Longest high Te */
#define HIGH_TO     	7
/** Longest low Te */
#define LOW_TO       	7
/** Shortest Thead accepted */
#define SHORT_HEAD   	25
/** Longest Thead accepted */
#define LONG_HEAD    	45

//--------------------------------------------------------------------
// External Variable Definitions
//--------------------------------------------------------------------
extern uint8_t  RX_Buffer[]; // Receive buffer
extern uint8_t RFstate; // Receiver state
extern int8_t RFcount; // Timer counter
extern uint8_t Bptr; // Receive buffer pointer
extern uint8_t BitCount; // Received bits counter
extern boolean RFsynch; // High level detect prior to synch pulse
extern boolean RFFull; // Buffer full
extern boolean RFBit; // Sampled RF signal
extern uint8_t RFtype; // Determines what type of encrypted signal

#endif
