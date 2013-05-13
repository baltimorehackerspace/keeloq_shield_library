 /* This is a simple sketch to just output the callback info via the serial port
 */

/* Include the KeelogShield header */
#include <KeeloqShield.h>

void my_callback(KeeloqMsg messageType, uint32_t serialNumber, uint8_t btn)
{

  Serial.print("Got message ");
  Serial.println(messageType, DEC;
  Serial.print("serial ");
  Serial.println(serialNumber, HEX);
  Serial.print("btn ");
  Serial.println(btn, HEX);
  
  switch(messageType)
  {
    case KMSG_BTN:
      switch(btn)
      {
        case KBTN_1:
			Serial.println("Button 1 pressed");
        break;
        
        case KBTN_2:
			Serial.println("Button 2 pressed");
        break;
        
        case KBTN_3:
			Serial.println("Button 3 pressed");
        break;
        
        case KBTN_4:
			Serial.println("Button 4 pressed");
        break;
        
        default:
        break;
      }
    break;

	case KMSG_NEWPAIR:
		Serial.print("New fob paired, SN: ");
		Serial.println(serialNumber, HEX);
	break;

	case KMSG_PAIRTIMEOUT:
		Serial.println("Pair fob attempt timed out");
	break;
	
    case KMSG_ERASEALL:
		Serial.println("All paired fobs erased");
	break;

    default:
		Serial.println("Invalid messageType received");
    break;
  }
  
  
}

/* Basic setup function, this function runs once */
void setup()
{
  /* Init the serial port */
  Serial.begin(115200);
  
  /* This call initializes the KeeloqShield library, and registers the callback function */
  setupKeeloqShield(my_callback);
}

/* Your main program loop */
void loop()
{
}
