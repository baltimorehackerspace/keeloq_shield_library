/* KeeloqShield - KeeLoqAlg */
/* Keeloq decryption algorithm  */

#include "keeloq.h"
#include "keeloqalg.h"
#include "keygen.h"
#include "global.h"

uint8_t aux;
uint8_t DKEY[8];

void Decrypt(void)
{
    unsigned char i, j, key, aux;
    signed char p;

#ifdef DBG
    int pos;
    Serial.print("DKEY ");
    for(pos = 0; pos<8; pos++)
    {
      Serial.print(DKEY[pos], HEX);
      Serial.print(" ");
    }
    Serial.println("");
#endif
	
    p = 1;

    for (j = 66; j > 0; j--)
    {
        key = DKEY[p--];
        if (p < 0)
            p += 8;
        for (i = 8; i > 0; i--) {
            // NLF
            if (BIT_TEST(Buffer[3], 6)) {
                if (!BIT_TEST(Buffer[3], 1))
                    aux = 0b00111010; // 10
                else
                    aux = 0b01011100; // 11
            } else {
                if (!BIT_TEST(Buffer[3], 1))
                    aux = 0b01110100; // 00
                else
                    aux = 0b00101110; // 01
            }

            // move bit in position 7
            if (BIT_TEST(Buffer[2], 3))
                aux = (aux << 4) | (aux >> 4);
            if (BIT_TEST(Buffer[1], 0))
                aux <<= 2;
            if (BIT_TEST(Buffer[0], 0))
                aux <<= 1;

            // xor with Buffer and Dkey
            aux ^= Buffer[1] ^ Buffer[3] ^ key;

            Buffer[3] = (Buffer[3] << 1) | (Buffer[2] >> 7);
            Buffer[2] = (Buffer[2] << 1) | (Buffer[1] >> 7);
            Buffer[1] = (Buffer[1] << 1) | (Buffer[0] >> 7);
            Buffer[0] = (Buffer[0] << 1) | (aux >> 7);

            // rotate Dkey
            key <<= 1;
        } // for i
    } // for j
} // decrypt 

