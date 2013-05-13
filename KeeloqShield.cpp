/* KeeloqShield  */
/*  */

#include "keeloqshield.h"

//This is where the received bytes will be saved into from the ISR
//----------------- flags defines ------------------------------------
/** Receiver state */
uint8_t RFstate;
/** Timer counter */
int8_t RFcount;
/** Receive buffer pointer */
uint8_t Bptr;
/** Received bits counter */
uint8_t BitCount;
/** Determines what type of encrypted signal was received */
uint8_t RFtype;


//----------------- flags defines ------------------------------------

/** Hopping code verified OK */
boolean FHopOK;
/** Same code as previous */
boolean FSame;
/** State bit that indicates if learn mode is active */
boolean FLearn;
/** Resync required */
boolean F2Chance;
/** Flag indicating that the receive buffer is full */
boolean RFFull;
/** Sampled RF signal */
boolean RFBit;
/** Incoming trasnmission Synch level detect flag */
boolean RFsynch;

//----------------- Encrypted Reception buffer -----------------------
/** Decryption Work Buffer */
uint8_t Buffer[22];
/** RF Receive buffer */
uint8_t RX_Buffer[22];

/** seed value = serial number */
uint8_t SEED[4];
/** resync value for 2 Chance */
uint16_t NextHop;
/** output timer */
uint8_t COut;
/** Function codes and upper nibble of serial number */
uint8_t FCode;
/** Learn timers and counter */
uint8_t CLearn, CTLearn;
/** Led flashing timer and counter */
uint8_t CFlash, CTFlash;
/** Temp storage for read and write to mem. */
uint16_t Dato;
/** Address pointer to record in mem. */
uint16_t Ind;
/** Hopping code sync counter */
uint16_t Hop;
uint16_t Hop1;
/** Last value of sync counter (from EEPROM) */
uint16_t EHop;
uint16_t EHop1;
/** Second copy of sync counter */
uint16_t ETemp;
uint16_t ETemp1;
/** Flag set in the ISR to indicate to mainline code that the heartbeat time has elapsed */
//volatile boolean Continue_Count;

/** Temporary buffer for sprintf/LCD operations */
uint8_t sbuf[22];

int BTN_SW1;
int BTN_SW2;

void (*keeloq_callback)(KeeloqMsg messageType, uint32_t serialNumber, uint8_t btn) = 0;

void setupKeeloqShield(void (*callback)(KeeloqMsg messageType, uint32_t serialNumber, uint8_t btn))
{
#ifdef DBG
  Serial.begin(115200);
#endif

  keeloq_callback = callback;

  BTN_SW1 = 1;
  BTN_SW2 = 1;

  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  digitalWrite(PIN_LED_BLUE, HIGH);
  digitalWrite(PIN_LED_BLUE, HIGH);
  pinMode(PIN_SW1, INPUT);
  pinMode(PIN_SW2, INPUT);
  pinMode(SX_DIO0, INPUT);
  pinMode(RAW_DATA, INPUT);  
  pinMode(RAW_CLOCK, INPUT);

  // set the SX_NSS as an output:
  pinMode (SX_NSS, OUTPUT);
  // initialize SPI:
  SPI.begin(); 
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  
  // Configure for KeeLoq data polling
  InitReceiver();

  // Configure the sx1239
  sx1239_init();

#ifdef DBG
  uint8_t tmp;
  tmp = sx1239_getVersion();  
  Serial.print("SX1239 VER: ");
  Serial.println(tmp, HEX);

  tmp = read_spi(0x06 + READ);
  Serial.print("SX1239 RES: ");
  Serial.println(tmp, HEX);
#endif

   attachCoreTimerService(KeeloqUtilLoop);

   ConfigIntTimer3(T3_ON | T3_INT_PRIOR_3);
   OpenTimer3( T3_ON | T3_PS_1_1 | T3_SOURCE_INT, 4800);

   // Flash LED to indicate that master is ready and entering the main loop
   flash_leds(2);   
}

#ifdef __cplusplus
extern "C" {
#endif
void __ISR(_TIMER_3_VECTOR,IPL3AUTO) rxi_handler(void)
{
  mT3ClearIntFlag();  // Clear interrupt flag
  rxi();

}
#ifdef __cplusplus
}
#endif

/**
 * Flashes LEDs
 * \param t Number of times to flash
 */
void flash_leds(uint8_t t)
{
    uint8_t i;
    for (i = 0; i < t; i++) 
    {
        digitalWrite(PIN_LED_BLUE, LOW);
        digitalWrite(PIN_LED_RED, LOW);
        delay(125);
        digitalWrite(PIN_LED_BLUE, HIGH);
        digitalWrite(PIN_LED_RED, HIGH);
        delay(125);
    }
}

void checkButtons()
{
  BTN_SW1 = digitalRead(PIN_SW1);
  BTN_SW2 = digitalRead(PIN_SW2);
}

uint32_t KeeloqUtilLoop(uint32_t currentTime)
{
        checkButtons();

        // This portion of the KeeLoq code expects to be run only every 64 ms.
//        if (Continue_Count)
//        {
            // We perform the Learn button check and the LED check every 64ms.
//          Continue_Count = 0; 

            // Check the Learn button.
            if (!BTN_SW1)
            {   // It's pressed. CLearn tracks how long it has been pressed.
                CLearn++;

                // pressing Learn button for more than 10s -> ERASE ALL
                if (CLearn == 156) // 156 * 64 ms = ~10s
                {
                    digitalWrite(PIN_LED_RED, HIGH);
  
#ifdef DBG                    
                    Serial.println("ERASE MEM");
#endif

					keeloq_callback(KMSG_ERASEALL, 0, 0);

					while (!BTN_SW1)
                    {
                      checkButtons();
                    };      // wait for button release
                    digitalWrite(PIN_LED_RED, LOW);

                    COut = TOUT;        // single long flash pulse time
                    // timer will switch off Led

                    ClearMem();

                    // Reset variables related to learn mode
                    CLearn = 0;
                    CTLearn = 0;
                    FLearn = FALSE;
                    CFlash = 5; // request Led flashing to indicate successful learning
                    CTFlash = TFLASH; // load period timer
				}

                // Require that the learn button be pressed for 1 second before
                // entering learn mode
                if (CLearn == 15) // ~ 1 s
                {   // Learn button has been held for the required minimum time
                    FLearn = TRUE;      // enter learn mode command
                    CTLearn = TLEARN;   // load timout value
                    digitalWrite(PIN_LED_RED, LOW);
#ifdef DBG
                    Serial.println("Learn mode active");
#endif
                }
            }
            else
            {   // The learn button is not pressed, so reset the counter.
                CLearn = 0;
            }

            // Toggle the --- LED
            if (COut > 0) // if timer running
            {
                COut--;
                if (COut == 0) // when it reach 0
                {
                    digitalWrite(PIN_LED_RED, HIGH);
                }
            }

            // Check for Learn Mode Timeout.
            if (CTLearn > 0)
            {
                CTLearn--; // count down
                if (CTLearn == 0)
                {   // Time-out period has elapsed
                    digitalWrite(PIN_LED_RED, HIGH);
                    FLearn = FALSE;

#ifdef DBG					
                    Serial.println("LEARN MODE TIMEOUT");
#endif
					keeloq_callback(KMSG_PAIRTIMEOUT, 0, 0);

					}
            }

            // Led Flashing
            if (CFlash > 0)
            {
                CTFlash--; // count down
                if (CTFlash == 0) // if timed out
                {
                    CTFlash = TFLASH; // reload timer
                    CFlash--; // count one flash
                    digitalWrite(PIN_LED_RED, HIGH);
                    if (CFlash & 1)
                        digitalWrite(PIN_LED_RED, LOW);
                }
            }
//        } // Continue_Count
		
		return currentTime + 2560000; // 0.064 / (1/40000000)
}

void loopKeeloqShield()
{
#ifdef THISISMOVEDINTOTHEINTERRUPTFORNOW
        if (RFFull) // RF receive buffer contains a message
        {
            digitalWrite(PIN_LED_BLUE, LOW);
            Remote(); // Get Message
            digitalWrite(PIN_LED_BLUE, HIGH);
            RFFull = 0;
        }
#endif
}

