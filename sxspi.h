/* KeeloqShield - sxspi header */
/*  */

#ifndef __SXSPI_H
#define __SXSPI_H

#include <plib.h>
#include <WProgram.h>
#include "keeloq.h"
#include "keeloqalg.h"
#include "global.h"
#include "KeeloqShield.h"
#include "utility/SPI.h"

//SPI Read/Write commands
#define 	WRITE		0x80
#define		READ		0x00

uint8_t write_spi(uint8_t address, uint8_t value);
uint8_t read_spi(uint8_t address);

#endif