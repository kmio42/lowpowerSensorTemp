/*
 * htu21d.h
 *
 *  Created on: 10.12.2017
 *      Author: kcl
 */

#ifndef HTU21D_H_
#define HTU21D_H_


#define HTU21DF_I2CADDR       0x40
#define HTU21DF_READTEMP      0xF3
#define HTU21DF_READHUM       0xF5
#define HTU21DF_WRITEREG       0xE6
#define HTU21DF_READREG       0xE7
#define HTU21DF_RESET       0xFE

#define HTU21DF_OTP_RELOAD_DIS (1 << 1)
enum resolution{ HTU21DF_RES_RH12_T14 = 0, HTU21DF_RES_RH8_T12 = 0x01,
	HTU21DF_RES_RH10_T13 = 0x80, HTU21DF_RES_RH11_T11 = 0x81};

void htu21d_setResolution(enum resolution);
void htu21d_softReset(void);
float htu21d_readTemperature(void);
float htu21d_readHumidity(void);
#endif /* HTU21D_H_ */
