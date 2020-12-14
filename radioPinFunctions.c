/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* Please define your platform spesific functions in this file ...
* -----------------------------------------------------------------------------
*/

#include <avr/io.h>

#define set_bit(reg,bit) reg |= (1<<bit)
#define clr_bit(reg,bit) reg &= ~(1<<bit)
#define check_bit(reg,bit) (reg&(1<<bit))

/* ------------------------------------------------------------------------- */
void nrf24_setupPins()
{
//	 set_bit(DDRD,0); // CE output
//	 set_bit(DDRD,1); // CSN output
//	 set_bit(DDRD,2); // SCK output
//	 set_bit(DDRD,3); // MOSI output
//	 clr_bit(DDRD,4); // MISO input
//	 clr_bit(DDRD,5); // Interrupt

    set_bit(DDRD,0); // CSN output
    set_bit(DDRD,1); // MOSI output
    set_bit(DDRD,4); // SCK output
    set_bit(DDRB,0); // CE output
    clr_bit(DDRD,2); // Interrupt
    clr_bit(DDRD,3); // MISO input
  //  DDRD = (1 << PD0) | (1 << PD1) | (1 << PD4);
  //  DDRB = (1 << PB0);
}
/* ------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(PORTB,0);//5
    }
    else
    {
        clr_bit(PORTB,0);
    }
}
/* ------------------------------------------------------------------------ */
void nrf24_csn_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(PORTD,0);//0
    }
    else
    {
        clr_bit(PORTD,0);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_sck_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(PORTD,4);//4
    }
    else
    {
        clr_bit(PORTD,4);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_mosi_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(PORTD,1);//1
    }
    else
    {
        clr_bit(PORTD,1);
    }
}
/* ------------------------------------------------------------------------- */
uint8_t nrf24_miso_digitalRead()
{
    return check_bit(PIND,3);//3
}
/* ------------------------------------------------------------------------- */
uint8_t nrf24_intr_digitalRead() {
	return check_bit(PIND,2); //2
}
