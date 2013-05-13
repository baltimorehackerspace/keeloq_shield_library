/* KeeloqShield - KeeLoq */
/*  */

#ifndef __KEELOQ_H
#define __KEELOQ_H

#include <plib.h>
#include <WProgram.h>
#include "keeloq_rx.h"
#include "keygen.h"
#include "keeloqalg.h"
#include "eepromhandler.h"
#include "keeloqshield.h"


//--------------------------------------------------------------------
// LED & LEARN Timings
//--------------------------------------------------------------------
#define     TOUT        4           //   5 * 64ms = 320ms output delay
#define     TFLASH      5           //   4 * 64ms = 256ms half period
#define     TLEARN      255         // 255 * 64ms =  16s  learn timeout

//----------------- flags defines ------------------------------------
extern boolean FHopOK;
extern boolean FSame;
extern boolean FLearn;
extern boolean F2Chance;
extern boolean RFFull;
extern boolean RFBit;
extern boolean RFsynch;

extern uint8_t Buffer[22];
extern uint8_t RX_Buffer[22];
extern uint8_t SEED[4];
extern uint16_t NextHop;
extern uint8_t COut;
extern uint8_t FCode;
extern uint8_t CLearn, CTLearn;
extern uint8_t CFlash, CTFlash;
extern uint16_t Dato;
extern uint16_t Ind;
extern uint16_t Hop;
extern uint16_t Hop1;
extern uint16_t EHop;
extern uint16_t EHop1;
extern uint16_t ETemp;
extern uint16_t ETemp1;

void Remote(void);

#endif