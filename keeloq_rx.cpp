/* KeeloqShield - KeeLoq_RX */
/* Keeloq rf encoding rx interrupt bit packer */

#include "keeloq_rx.h"

/**
 * KeeLoq interrupt-based reception state machine.
 */
void rxi(void)
{

    if (RFFull) // avoid overrun
    {
            digitalWrite(PIN_LED_BLUE, LOW);
            Remote(); // Get Message
            digitalWrite(PIN_LED_BLUE, HIGH);
            RFFull = 0;
	
        return;
    }

    RFBit = digitalRead(RAW_DATA);

    switch (RFstate) // state machine main switch
    {
        case TRFUNO: // Have detected a high level waiting for falling edge
            
            if (RFBit == 0) // Read RF level
            {   // falling edge detected  ----+
                //                            |
                //   	                      +-------

                RFstate = TRFZERO; // Falling edge detected, go back to state TRFZERO
            } else { // still high
                RFcount--;
                if (RFcount >= HIGH_TO) // Check for timeouot
                { // If timeout, then check for what type of transmission detected.
                    if (BitCount < KNBIT) // Not a complete transmission so reset state machine
                    {
                        RFtype = 0;
                        RFstate = TRFSYNC;
                        Bptr = 0;
                        BitCount = 0;
                        
                        break;
                    }
                    if ((BitCount >= KNBIT) & (BitCount < XNBIT)) // Is this standard Keeloq?
                    {
                        RFtype = KeeLoq;
                    }

                    if ((BitCount >= XNBIT) & (BitCount < ANBIT)) // Is this XTEA?
                    {
                        RFtype = XTEA;
                    }
                    if (BitCount >= ANBIT) // Is this AES?
                    {
                        RFtype = AES;
                    }

                    RFstate = TRFreset; // Reset State machine for next transmission
                    RFFull = 0x01; // Flag to let the main routine know we have a complete transmission

                }//if RFcount
            }//else
            break;

        case TRFZERO: // We've detected a low level, look for rising edge
            if (RFBit) // Read RF bit.
            {   // rising edge detected     +----
                //                          |
                //                      ----+

                RFstate = TRFUNO;
                RX_Buffer[Bptr] >>= 1; // Rotate
                if (RFcount >= 0) {
                    RX_Buffer[Bptr] += 0x80; // Shift in bit
                }
                RFcount = 0; // Reset length counter

                if ((++BitCount & 7) == 0)
                    Bptr++; // Advance one byte
                if (BitCount == (ANBIT + 10)) // maximum number of bits
                {
                    RFstate = TRFreset; // Finished receiving
                    RFFull = 0x1;
                    RFtype = AES;
                }
            } else { // still low
                RFcount++;
                if (RFcount >= LOW_TO) // too long low
                { // If timeout, then check for what type of transmission detected.

                    
                    RX_Buffer[Bptr] >>= 1; // Rotate
                    if (RFcount >= 0) //shift in last receive bit
                    {
                        RX_Buffer[Bptr] += 0x80; // Shift in bit
                    }

                    if (BitCount < KNBIT) // Not a valid complete transmission so reset state machine
                    {
                        RFtype = 0;
                        RFstate = TRFSYNC;
                        Bptr = 0;
                        BitCount = 0;
                        break;
                    }
                    if (BitCount >= KNBIT & BitCount < XNBIT) // Is this standard Keeloq?
                    {
                        RFtype = KeeLoq;
                    }

                    if (BitCount >= XNBIT & BitCount < ANBIT) // Is this AES?
                    {
                        RFtype = XTEA;
                    }
                    if (BitCount >= ANBIT) // Is this XTEA?
                    {
                        RFtype = AES;
                    }

                    RFstate = TRFreset; // Reset State machine for next transmission
                    RFFull = 0x01; // Flag to let the main routine know we have a complete transmission
//                    NOP();
                }
            }

            break;

        case TRFSYNC: // Expect header before looking for bits
            if (RFBit) 
            {   // rising edge detected  	+---+                  +---..
                //                              |   |  <-Theader-> |
                //                              +------------------+

                if ((RFcount < SHORT_HEAD) || (RFcount >= LONG_HEAD)) {
                    RFstate = TRFreset;
                    RFcount = 0;
                    break; // too short/long, no header
                } else {
                    RFcount = 0; // restart counter
                    RFstate = TRFUNO; // Header detected, start reading bits
                }
            } else { // still low
                RFcount++;
            }
            break;


        case TRFSYNC0: // Wait for high level before looking for synch header
            if (!RFsynch) // High level detected?
            {
                if (RFBit) // High level detected reset counters and wait for sync header
                {
                    RFsynch = 1;
                } //RFbit
            }//RFsynch
            else {
                if (!RFBit) // low level synch pulse is probably detected
                {
                    RFstate = TRFSYNC;
                    RFsynch = 0;
                }
            }
            break;


        case TRFreset: // Reset state machine
        default:
            RFstate = TRFSYNC0; // reset state machine in all other cases
            RFcount = 0;
            Bptr = 0;
            BitCount = 0;
            RFsynch = 0;
            break;

    } // switch    
} // rxi

/**
 * Initialize KeeLoq bit-banged reception routines.
 * Configures TMR0 and reception state machine variables.
 */
void InitReceiver(void)
{
    RFstate = TRFreset; // reset state machine in all other cases
    RFFull = 0; // start with buffer empty
    RFtype = 0; // Type of incoming data (Keeloq, AES, XTEA)
}

