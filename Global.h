/* KeeloqShield - Global */
/* some global defines used in the different files */

#ifndef __GLOBAL_H
#define __GLOBAL_H

#define TRUE    1
#define FALSE   0

#define S0      5   //  Buffer[3] function codes
#define S1      6   //  Buffer[3] function codes
#define S2      7   //  Buffer[3] function codes
#define S3      4   //  Buffer[3] function codes
#define VFlag   7   //  Buffer[8] low battery flag

// -----------------global variables ---------------------------



#define BIT_TEST( x, y) (( (x) & (1<<(y))) != 0)
#define bitset(var,bitno) ((var) |= 1UL < < (bitno))
#define bitclr(var,bitno) ((var) &= ~(1UL < < (bitno)))

extern unsigned char Buffer[];
extern unsigned char DKEY[];

#endif