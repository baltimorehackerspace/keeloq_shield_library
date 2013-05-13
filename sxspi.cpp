/* KeeloqShield - sxspi */
/* Wrapper functions to hook spi transactions with chip select to the SPI Library */

#include "sxspi.h"


uint8_t read_spi(uint8_t address)
{
  uint8_t ret;
  // take the SS pin low to select the chip:
  digitalWrite(SX_NSS, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(address & 0x7F);
  ret = SPI.transfer(0x00);
  // take the SS pin high to de-select the chip:
  digitalWrite(SX_NSS, HIGH);
  
  return ret;
}

uint8_t write_spi(uint8_t address, uint8_t value)
{
  uint8_t ret;
  // take the SS pin low to select the chip:
  digitalWrite(SX_NSS, LOW);
  delayMicroseconds(10);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  ret = SPI.transfer(value);
  delayMicroseconds(10);
  // take the SS pin high to de-select the chip:
  digitalWrite(SX_NSS, HIGH); 

  return ret;
}
