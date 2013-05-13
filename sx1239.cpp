/* KeeloqShield - sx1239 */
/* sx1239 chip driver */

#include "sx1239.h"
#include "sx1239_config.h"

/**
 * @brief   Initilizies the SX1239 receiver in either packet or continous mode
 *      -Continous mode provides raw, demodulated data on the GPIO pins
 *      -Packet mode is for production where PAYLOADREADY is used as interrupt
 *       to determine when the transmission is ready to be emptied
 * @ref sx1239_config.h
 * @ref sx1239.h
 */


void sx1239_init(void)
{
#ifdef DBG
    Serial.println("sx1239 init");
#endif
  
    pinMode(SX_RESET, OUTPUT);
    digitalWrite(SX_RESET, HIGH);
    delay(1);
    pinMode(SX_RESET, INPUT);
    delay(10);
    
    // Set RegOpMode register
    write_spi(REGOPMODE_ADDRESS + WRITE, SX1239_OPMODE_ANS);

    // Set RegDataModul register
    write_spi(REGDATAMODUL_ADDRESS + WRITE, SX1239_DATAMODE_ANS);

    // Set RegFrfMsb, RegFrfMid, and RegFrfLsb registers
    write_spi(REGFRFMSB_ADDRESS + WRITE, (SX1239_FREQ_ANS & 0xFF0000) >> 16);
    write_spi(REGFRFMID_ADDRESS + WRITE, (SX1239_FREQ_ANS & 0x00FF00) >> 8);
    write_spi(REGFRFLSB_ADDRESS + WRITE, (SX1239_FREQ_ANS & 0x0000FF));

    // Set RegSyncConfig register
    write_spi(REGSYNCCONFIG_ADDRESS + WRITE, SX1239_SYNC_ANS);

    // Set the RegRxBw register
    write_spi(REGRXBW_ADDRESS + WRITE, SX1239_RXBW_ANS);

    // Set the RegRxThresh register
    write_spi(REGRSSITHRESH_ADDRESS + WRITE, SX1239_RSSI_THRESH_PEAK_ANS);

    // Set the RegOokPeak register
    write_spi(REGOOKPEAK_ADDRESS + WRITE, SX1239_OOK_PEAK_ANS);

    // Set the RegOokAvg register
    write_spi(REGOOKAVG_ADDRESS + WRITE, SX1239_THRESH_AVG_ANS);

    //GPIO_0 = SyncAddress, GPIO_1 = DCLK, GPIO_2 = DATA    --ONLY IN CONTINUOUS MODE!
    // Warning: must be defined after REGMODE/MODULATION
    write_spi(REGDIOMAPPING1_ADDRESS + WRITE, 0b00000000);

    while ( (read_spi( REGIRQFLAGS1_ADDRESS ) & 0x80 ) == 0x00 )
    {
#ifdef DBG
    Serial.println("sx1239 waiting for ready");
#endif
      delayMicroseconds(500);
    }
    
#ifdef DBG
    Serial.println("sx1239 init done");
#endif

}
    
uint8_t sx1239_getVersion(void)
{
    return write_spi(REGVERSION_ADDRESS + READ, 0xAA); //get revision
}

/**
 * Optional CRC routine. Must pass in 1 byte at a time for a recurring
 * calculation. Uses CTCC-16 algorithm which the SX1239 uses as well
 */
int16_t crc_1021(uint8_t data, uint8_t crc)
{
    int16_t x;

    x = ((crc >> 8) ^ data) & 0xff;
    x ^= x >> 4;
    crc = (crc << 8) ^ (x << 12) ^ (x << 5) ^ x;
    crc &= 0xffff;
    return crc;
}

