/* KeeloqShield - eepromhandler */
/* handles read/writes to the eeprom for storing keys header */

#include "eepromhandler.h"

static int address;

//--------------------------------------------------------------------
// EERead ()
//--------------------------------------------------------------------
uint8_t EERead(uint8_t EE_Address)
{
      address = EE_Address;
      return EEPROM.read(EE_Address);
}

//--------------------------------------------------------------------
// EEWrite ()
//--------------------------------------------------------------------
uint8_t EEWrite(uint8_t EE_Address, uint8_t EE_Data)
{
      EEPROM.write(EE_Address, EE_Data);
      return TRUE;
}

//--------------------------------------------------------------------
// RDWord ()
//--------------------------------------------------------------------
void RDword(uint16_t Ind)
{
      Dato = EERead(Ind);
      Dato += (uint16_t) EERead( Ind+1) <<8;
}

//--------------------------------------------------------------------
// RDnext ()
//--------------------------------------------------------------------
void RDnext(void)
{
    // Continue reading
    address++;
    Dato = EERead(address);
    address++;
    Dato += (uint16_t) EERead( address) <<8;
}

//--------------------------------------------------------------------
// WRword ()
//--------------------------------------------------------------------
void WRword(uint16_t Ind)
{
    EEWrite( Ind, Dato); 
    EEWrite( Ind+1, Dato>>8); 
}

/**
 * Search through the whole table the given a record whose ID match
 *
 * Searches nonvolatile memory for a serial number matching IDHi, IDMi, IDLo.
 * If found, updates Ind with the address of the record, EHop with the sync
 * counter value, and ETemp with a second copy of the sync counter.
 *
 * \pre IDHi, IDMi, IDLo loaded with serial number to search
 *
 * \return TRUE if matching record found
 */
uint8_t Find(uint8_t packetType)
{
    uint8_t Found = FALSE;      // init to not found

    if (packetType == KEELOQ_PACKET)
    {
        for (Ind=0; Ind < (EL_SIZE * MAX_USER); Ind+=EL_SIZE)
        {
            RDword( Ind);       // read first Word
            FCode = (Dato>>8);

            if ( (FCode & 0xf0) == 0xf0)		// check if there is a function code
                continue;   // empty

            if (IDLo != (Dato & 0xff))		// ID match?
                continue;   // fails match

            RDnext();       				// Read Next Word
            if ( ( (Dato & 0xff) == IDMi) && ( (Dato>>8) == IDHi))
            {
                Found = TRUE;     // match
#ifdef DBG
                Serial.println("Key found in eeprom");
#endif
                break;
            }
        }

        if (Found == TRUE)
        {
            RDnext();               // read HopHi/Lo
            EHop = Dato;
            RDnext();               // read HopHi2/Lo2
            ETemp= Dato;
        }
    }
    else
        Found = FALSE;

    return Found;
}

/**
 * Search through the whole table for an empty space. Uses IDHi, IDMi, and IDLo
 * as the serial number to insert and writes to Ind the address of the empty
 * record.
 * @return FALSE if no empty space found
 */
uint8_t Insert(void)
{
    for (Ind=0; Ind < (EL_SIZE * MAX_USER); Ind+=EL_SIZE)
    {
        RDword(Ind);        // read first Word
        FCode = (Dato>>8);
        // check if 1111xxxx
        if ( (FCode & 0xf0) == 0xf0)
            return TRUE;    // insert point found
    } // for        

    return  FALSE;          // could not find any empty slot
} // Insert

/**
 * Writes IDHi, IDMi, IDLo, and function code to the address indicated by Ind.
 * @return Returns FALSE if FLearn indicates the application is not in learning
 * mode.
 */
uint8_t IDWrite(void)
{
    if (!FLearn) 
        return FALSE;           // Guard statement: check if Learn ON

    Dato = Buffer[7];
    Dato = (Dato<<8) + IDLo;
    WRword(Ind);                // write first word

    Dato = IDHi;
    Dato = (Dato<<8) + IDMi;
    WRword(Ind+2);              // write second word
    
    return TRUE;
} // IDWrite
          
/**
 * Update the sync counter pointed to by Ind with the value found in HopHi and
 * HopLo.
 *
 * @return Returns FALSE if FLearn indicates the application is not in learning
 * mode.
 */
uint8_t HopUpdate(void)
{
    if (!FHopOK) 
        return FALSE;           // Guard statement: check if Hop update 

    Hop = ((uint16_t)HopHi<<8) + HopLo;
    Dato = Hop;
    WRword(Ind+4);              // write at offset +4
    Dato = Hop;
    WRword(Ind+6);              // back up copy at offset +6
    FHopOK = FALSE;             // for safety disable updating hopping code
    
    return TRUE;
} // HopUpdate

/**
 * Mark all records free
 * @return Always returns true.
 */
uint8_t ClearMem(void)
{
//    char i;
    for (Ind=0; Ind < (EL_SIZE * MAX_USER); Ind+=EL_SIZE)
    {
        Dato = 0xffff;
        WRword( Ind);
    }
  return TRUE;
} // ClearMem
        

