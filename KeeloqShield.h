/* KeeloqShield -  */
/* Library for using the KeeloqShield with the ChipKit */

#ifndef __KEELOQ_SHIELD_H
#define __KEELOQ_SHIELD_H

//#define DBG 1

#include <plib.h>
#include <Time.h>
#include "utility/SPI.h"
#include "KeeloqShield.h"
#include "sxspi.h"
#include "Global.h"
#include "KeeLoq_RX.h"
#include "KeeLoqAlg.h"
#include "Keygen.h"
#include "eepromhandler.h"
#include "keeloq.h"
#include "sx1239.h"

#define KBTN_1		  0x1
#define KBTN_2		  0x2
#define KBTN_3		  0x4
#define KBTN_4		  0x8

#define PIN_SW2        30
#define PIN_SW1        31

#define PIN_LED_BLUE   32
#define PIN_LED_RED    33

#define PIN_LED_TEST   43

#define SX_RESET       34
#define SX_DIO0        35
#define RAW_DATA       36
#define RAW_CLOCK      37
#define SX_NSS         10
#define SX_MOSI        11
#define SX_MISO        12
#define SX_SCK         13

enum KeeloqMsg
{
	KMSG_NEWPAIR     = 1,
	KMSG_PAIRTIMEOUT = 2,
    KMSG_ERASEALL    = 3,
	KMSG_BTN		 = 10
};

extern uint8_t Buffer[22];
extern void (*keeloq_callback)(KeeloqMsg messageType, uint32_t serialNumber, uint8_t btn );

void flash_leds(uint8_t t);
void setupKeeloqShield(void (*callback)(KeeloqMsg messageType, uint32_t serialNumber, uint8_t btn));
void loopKeeloqShield();
uint32_t KeeloqUtilLoop(uint32_t currentTime);

#endif