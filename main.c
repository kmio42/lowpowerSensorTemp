#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

//#define DEBUG 1
//#define DEBUG_PRINT
#include "i2clcd.h"
#include <avr/interrupt.h>
#include  <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/sleep.h>
#include "adc.h"
#include "htu21d.h"
#include "nrf24.h"
#include "capmeas.h"

volatile uint8_t ss = 0;
uint8_t retransmissionCount;
ISR (TIMER2_OVF_vect) {
	++ss;
}
char buffer[25];
uint8_t data[7];
uint8_t count = 0;
uint8_t tx_address[5] = {0xB4,'k','m','i','o'};

int main(void)
{
	_delay_ms(1000);

	//init RTC
	ASSR = (1 << AS2);
	TCCR2A = 0;
//	TCCR2B = (1 << CS22) | (1 << CS21); //128 Hz clock
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); //32Hz clock
	TIMSK2 = (1 << TOIE2);
	DDRB |= (1 << PB2);

//	DDRC |= (1 << PC2);
//
//	PORTC |= (1 << PC2);
	//init i2c
	i2c_init();

#ifdef DEBUG_PRINT
	//debug print
	lcd_init();
	lcd_gotolr(1,1);
    lcd_print((uint8_t *) "Hallo Test");
#endif

    //init nrf24
	nrf24_init();
	nrf24_config(100,7);	// Channel #2 , payload length: 7
	nrf24_tx_address(tx_address);
	nrf24_powerDown();

     //configure sleepmode
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);

	while(ASSR & ((1 << TCN2UB) | (1 << OCR2AUB) | (1 << TCR2AUB)));
	
	sei();
	ss=38;

	while(1) {

    	/*in DEBUG mode transmit data in every interrupt, in normal mode only each 5min*/
#ifdef DEBUG
    	if(1) {
#else
    	//ca. 5min timer ss>37
		if(ss > 37) {
		ss = 0;
#endif
			PORTB |= (1 << PB2);
			ADC_Init();
			ADC_Read(0x06);
			uint16_t voltage = ADC_Read(0x06) >> 2;//1250;
			ADC_disable();
			PORTB &= ~(1 << PB2);

			htu21d_setResolution(HTU21DF_RES_RH8_T12);
			int temperature =  (int) (htu21d_readTemperature()*100);
			int humidity =  (int) (htu21d_readHumidity()*100);

			//int capacitance = read_senskey();
			data[0] = temperature & 0xFF;
			data[1] = temperature >> 8;
			data[2] = humidity & 0xFF;
			data[3] = humidity >> 8;
			data[4] = voltage & 0xFF;
			data[5] = ++count;
			data[6] = retransmissionCount;

			nrf24_powerUp();
			_delay_ms(4);	//wait until powered up and quartz stable
			nrf24_send(data);
			while(nrf24_intr_digitalRead());
//			//		uint8_t status = nrf24_lastMessageStatus();
			retransmissionCount += nrf24_retransmissionCount();
			nrf24_configRegister(STATUS,(1<<MAX_RT) | (1 << TX_DS) | (1 << RX_DR)); //clear interrupt flags
			nrf24_powerDown();

#ifdef DEBUG_PRINT
			/* Debug Ausgaben */
			lcd_gotolr(2,1);
			sprintf(buffer,"%d V  %d ", (voltage*5)/2,ss);
			lcd_print((uint8_t *) buffer);
			lcd_gotolr(1,1);
			sprintf(buffer,"%d %d %d   ",temperature, humidity,retransmissionCount);
			lcd_print((uint8_t *) buffer);
#endif
		}
		else {
			//if nothing to do goto sleep as soon as possible, but not to early
			/* Write dummy value to control register */
			TCCR2A = 0;
			/* Wait until TC2 is updated */
			while(ASSR & ((1 << TCN2UB) | (1 << OCR2AUB) | (1 << TCR2AUB)));
		}
		sleep_mode();
	} /* end while */
}
