/* This is a very basic example.
 * It will beep a different tone for each button on the remote when pressed
 * You will need to pair the remote before this example will function properly
 */

/* Include the KeelogShield header */
#include <KeeloqShield.h>

/* The callback function should be treated as an ISR function.  It needs to be quick and not use any delay() statements */
/* Do not do any cpu time intensive functions from within this callback  -- There is another example to illustrate how */
/* To do something more time intensive by saving the data from the callback, then processing it within the loop() */

void my_callback(KeeloqMsg messageType, uint32_t serialNumber, uint8_t btn)
{
  
  switch(messageType)
  {
    case KMSG_BTN:
      switch(btn)
      {
	    // The KBTN_{x} defines refer to the bitmask of each button
        case KBTN_1:
          tone(38, 1000, 400);
        break;
        
        case KBTN_2:
          tone(38, 3000, 400);
        break;
        
        case KBTN_3:
          tone(38, 5000, 400);
        break;
        
        case KBTN_4:
          tone(38, 7000, 400);
        break;
        
        default:
        break;
      }
    break;
    
    default:
    break;
  }
}

/* Basic setup function, this function runs once */
void setup()
{
  /* This call initializes the KeeloqShield library, and registers the callback function */
  setupKeeloqShield(my_callback);
}

/* Your main program loop */
void loop()
{
}
