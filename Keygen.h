/* KeeloqShield - keygen header */
/* used to generate the decryption key / verify encryption decoding */

#ifndef __KEYGEN_H
#define __KEYGEN_H

#include <plib.h>
#include <WProgram.h>
#include "keeloq.h"
#include "keeloq_rx.h"
#include "Global.h"

//--------------------------------------------------------------------
// External Variable Definitions
//--------------------------------------------------------------------


#define HopLo   Buffer[0] 	//sync counter
#define HopHi   Buffer[1]   //
#define DisLo   Buffer[2]   //discrimination bits LSB
#define DOK     Buffer[3]   //Disc. MSB + Ovf + Key
#define IDLo    Buffer[4]   //S/N LSB
#define IDMi    Buffer[5]   //S/N
#define IDHi    Buffer[6]   //S/N MSB


void LoadManufCode(void);
void NormalKeyGen(void);
unsigned char DecCHK(void);
unsigned char ReqResync(void);
unsigned char HopCHK(void);

#endif