/* KeeloqShield - KeeLoqAlg */
/* Keeloq decryption algorithm header */

#ifndef __KEELOQALG_H
#define __KEELOQALG_H

#include <plib.h>
#include <WProgram.h>

/** Decryption key for the KLQ algorithm. */
extern uint8_t DKEY[8];

/**
 * KLQ decryption algorithm.
 * \pre DKEY must contain the encryption key and Buffer must contain the data
 */
void Decrypt(void);

#endif