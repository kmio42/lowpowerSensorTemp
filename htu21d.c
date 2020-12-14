/*
 * htu21d.c
 *
 *  Created on: 10.12.2017
 *      Author: kcl
 */

#include "i2cmaster.h"
#include "htu21d.h"
#include <util/delay.h>
#include <stdlib.h>

void htu21d_setResolution(enum resolution res) {
	if(i2c_start(HTU21DF_I2CADDR << 1))
		return;
	i2c_write(HTU21DF_WRITEREG);
	i2c_write(res | HTU21DF_OTP_RELOAD_DIS);
	i2c_stop();
}
void htu21d_softReset() {
	if(i2c_start(HTU21DF_I2CADDR << 1))
		return;
	i2c_write(HTU21DF_RESET);
	i2c_stop();
}
float htu21d_readTemperature(void) {

  if(i2c_start(HTU21DF_I2CADDR << 1))
	  return -50;
  i2c_write(HTU21DF_READTEMP);
  i2c_stop();

  _delay_ms(18); // add delay between request and actual read!
  /*
    * minimum delay:
    * 11 bit: 7ms
    * 12 bit: 13ms
    * 13 bit: 25ms
    * 14 bit: 50ms
    */

  if(i2c_start((HTU21DF_I2CADDR << 1) + 1))
	  return -50;


  uint16_t t = i2c_readAck();
  t <<= 8;
  t |= i2c_readAck();

  uint8_t crc = i2c_readNak();
  i2c_stop();

//  t >>=2;
//  t*=175;
//  t-=47;
 float temp = t;
  temp *= 175.72;
  temp /= 65536;
  temp -= 46.85;

  return temp;
}


float htu21d_readHumidity(void) {
  // OK lets ready!

  if(i2c_start(HTU21DF_I2CADDR << 1))
	  return -1;
  i2c_write(HTU21DF_READHUM);
  i2c_stop();

  _delay_ms(8); // add delay between request and actual read!
  /*
   * minimum delay:
   *  8 bit: 3ms
   * 10 bit: 5ms
   * 11 bit: 8ms
   * 12 bit: 16ms
   */

  if(i2c_start((HTU21DF_I2CADDR << 1) + 1))
	  return -1;

  uint16_t h = i2c_readAck();
  h <<= 8;
  h |= i2c_readAck();

  uint8_t crc = i2c_readNak();
  i2c_stop();

  float hum = h;
  hum *= 125;
  hum /= 65536;
  hum -= 6;

  return hum;
}


