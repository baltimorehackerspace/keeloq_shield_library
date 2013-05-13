/* KeeloqShield - eepromhandler */
/* handles read/writes to the eeprom for storing keys header */

#ifndef __EEPROMHANDLER_H
#define __EEPROMHANDLER_H

#include <plib.h>
#include <WProgram.h>
#include "KeeloqShield.h"
#include "keeloq.h"
#include "keeloqalg.h"
#include "keeloq_rx.h"

#include "utility/EEPROM.h"



#define MAX_USER     	8         // max number of TX that can be learned
#define EL_SIZE      	8         // single record size in bytes

extern boolean FHopOK;  // Hopping code verified OK
extern boolean FLearn;  // Learn mode active
extern uint8_t FCode; // Function codes and upper nibble of serial number

extern uint16_t Dato; // temp storage for read and write to mem.
extern uint16_t Ind;  // address pointer to record in mem.
extern uint16_t Hop;  // hopping code sync counter
extern uint16_t EHop; // last value of sync counter (from EEPROM)
extern uint16_t ETemp; // second copy of sync counter

/*------------------------------------------------------------
; Table structure definition:
;
;  the EEPROM is filled with an array of MAX_USER user records
;  starting at address 0000
;  each record is EL_SIZE byte large and contains the following fields:
;  EEPROM access is in 16 bit words for efficiency
;
;   DatoHi  DatoLo  offset
;  +-------+--------+
;  | FCode | IDLo   |  0    XF contains the function codes (buttons) used during learning
;  +-------+--------+       and the top 4 bit of Serial Number
;  | IDHi  | IDMi   |  +2   IDHi IDMi IDLo contain the 24 lsb of the Serial Number
;  +-------+--------+
;  | HopHi | HopLo  |  +4   sync counter
;  +-------+--------+
;  | HopHi2| HopLo2 |  +6   second copy of sync counter for integrity checking
;  +-------+--------+
;
; NOTE a function code of 0f0 (seed transmission) is considered
; invalid during learning and is used here to a mark location free
;
;------------------------------------------------------------*/
/** Shortcut to the Buffer byte containing the low byte of the sync counter */
#define     HopLo       Buffer[0]
/** Shortcut to the Buffer byte containing the high byte of the sync counter */
#define     HopHi       Buffer[1]
/** Shortcut to the Buffer byte containing the low byte of the discrimination bits */
#define     DisLo       Buffer[2]
/** Shortcut to the Buffer byte containing the high byte of the discrimination bits */
#define     DOK         Buffer[3]
/** Shortcut to the Buffer byte containing the low byte of the serial number */
#define     IDLo        Buffer[4]
/** Shortcut to the Buffer byte containing the middle byte of the serial number */
#define     IDMi        Buffer[5]
/** Shortcut to the BufAer byte containing the high byte of the serial number */
#define     IDHi        Buffer[6]



uint8_t Find(uint8_t packetType); //Looks for learned transmitter S/N
uint8_t Insert(void); //Inserts new transmitter info in empty EEPROM space
uint8_t IDWrite(void); //Saves S/N, function code and hop counter info
uint8_t HopUpdate(void); // Update sync counter on current transmitter
uint8_t ClearMem(void); // Erases all transmitters' information

#endif