/* KeeloqShield - KeeLoq */
/* */

#include "keeloq.h"

#ifdef DBG
/** Buffer for sprintf calls. */
char buffer7[22];
#endif


union
{
	char c[4];
	uint32_t num;
} tmpSN;

/**
 * Decrypts and interprets received codes
 */
void Remote(void)
{
    // a frame was received and is stored in the receive buffer
    // move it to decryption Buffer, and restart receiving again

    // Copy the received data to the work buffer
    memcpy(Buffer, RX_Buffer, sizeof(RX_Buffer));
	
#ifdef DBG
    int i;
    Serial.print("DATA ");
    for(i = 0; i<sizeof(RX_Buffer); i++)
    {
      Serial.print(Buffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");
#endif
    
    switch (RFtype)
    {
        case KEELOQ_PACKET: // Standard Keeloq frame detected
            NormalKeyGen(); // Compute the decryption key
            Decrypt(); // Decrypt the hopping code portion

            // Validate received packet (aka, is it a paired device?)
            if (Find(RFtype) == FALSE)
            {   // Transmitter is not paired. Are we in learn mode?
                if (FLearn)
                {   // look for new space to store record
                    if (!Insert()) 
                    {   // fail if no memory available
#ifdef DBG					
                        Serial.println("INSERT CODE FAIL");
#endif
                        break; 
                    }
                    else
                    {
#ifdef DBG					
                        Serial.println("Paired");
#endif						
						if(keeloq_callback)
						{
							tmpSN.c[0] = SEED[0];
							tmpSN.c[1] = SEED[1];
							tmpSN.c[2] = SEED[2];
							tmpSN.c[3] = Buffer[3] & 0xF;
							keeloq_callback(KMSG_NEWPAIR, tmpSN.num, 0);
						}

                        // ASSERT Ind is pointing to a valid memory location
                        IDWrite(); // write Serial Number in memory
                        FHopOK = TRUE; // enable updating Hopping Code
                        HopUpdate(); // Write Hoping code in memory

                        CFlash = 10; // request Led flashing to indicate successful learning
                        CTFlash = TFLASH; // load period timer
//                        LED1 = TRUE; // start with Led on
                        FLearn = FALSE; // terminate successfully Learn
                        CTLearn = 0;
                    }
                }
                else // not in learning mode
                {
#ifdef DBG				
                  Serial.println("Not learned");
#endif				  
                }
            }             
            else if (DecCHK() == FALSE)
            {   //Transmitter is paired, but message is not valid
#ifdef DBG
				Serial.println("Not valid KLQ");
#endif
            }
            else
            {   // Transmitter is already paired
                // Check Hopping Code integrity; are we within the acceptable window?
                if (!HopCHK()) 
                {
#ifdef DBG
				Serial.println("Hop check fail");
#endif
					break;
                }

				// Visual status confirmation of success
				digitalWrite(PIN_LED_RED, LOW);
                COut = TOUT; // Init LED output timer

				/// Callback

				// hopping code incrementing properly
				HopUpdate(); // update memory

				if(keeloq_callback)
				{					
					tmpSN.c[0] = SEED[0];
					tmpSN.c[1] = SEED[1];
					tmpSN.c[2] = SEED[2];
					tmpSN.c[3] = Buffer[3] & 0xF;
					keeloq_callback(KMSG_BTN, tmpSN.num, Buffer[3] >> 4);
				}
				
#ifdef DBG				 
				if (((Buffer[7] ^ FCode) & 0xf0) == 0) // check against learned function code
				{
					Serial.println("check against");
				}

//                tone(38, 3000, 400);
                sprintf(buffer7, "KLQ: %01X%02X%02X%02X", SEED[3], SEED[2], SEED[1], SEED[0]);
                Serial.println((char*)buffer7);
                sprintf(buffer7, "C:%01X%02X%02X%02X F:%1X", Buffer[3] & 0xF, Buffer[2], Buffer[1], Buffer[0], Buffer[3] >> 4);
                Serial.println((char*)buffer7);
#endif
				}
                
            break;

        default:
#ifdef DBG
			Serial.println("AES/XTEA not supported");
#endif
            break;
    }

    // Ready to receive a new frame
    RFFull = FALSE;
    
} // remote



