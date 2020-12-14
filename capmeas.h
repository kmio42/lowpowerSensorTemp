/*
 * capmeas.h
 *
 *  Created on: 24.05.2019
 *      Author: kcl
 */

#ifndef CAPMEAS_H_
#define CAPMEAS_H_
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
//#include <atomic.h>
struct test {
  char b0:1;
  char b1:1;
  char b2:1;
  char b3:1;
  char b4:1;
  char b5:1;
  char b6:1;
  char b7:1;
} __attribute__((__packed__));

#define SBIT(r,n) (((volatile struct test *)&r)->b##n)

#define MAX_CYCLE       1000

#define SENSKEY_A0      SBIT( PORTC, 3 )        // connect to senspad
#define SENSKEY_A0_DDR  SBIT( DDRC,  3 )

#define SENSKEY_B0      SBIT( PORTC, 1 )        // connect to cap to A0
#define SENSKEY_B0_DDR  SBIT( DDRC,  1 )
#define SENSKEY_B0_PIN  SBIT( PINC,  1 )

uint16_t read_senskey( void );

#endif /* CAPMEAS_H_ */
