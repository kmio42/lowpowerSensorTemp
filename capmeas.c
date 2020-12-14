#include "capmeas.h"
/*
A0      B0      B0_PIN
0       0               full discharge
loop:
Z       Z               gap
Z       1               partial charge over pin A0 and senspad
Z       Z               gab
0       Z               partial charge over pin B0 only
0       Z       1       end
*/

uint16_t read_senskey( void )
{
  uint16_t i = MAX_CYCLE + 1;

  SENSKEY_A0 = 0;
  SENSKEY_A0_DDR = 1;
  SENSKEY_B0 = 0;
  SENSKEY_B0_DDR = 1;           // discharge cap

//  ATOMIC_BLOCK(ATOMIC_FORCEON){
    do{
      SENSKEY_A0_DDR = 0;       // A0 = tristate
      SENSKEY_B0 = 1;           // B0 = weak high
      SENSKEY_B0_DDR = 1;       // B0 = strong high
      SENSKEY_B0_DDR = 0;       // B0 = weak high
      SENSKEY_B0 = 0;           // B0 = tristate
      SENSKEY_A0_DDR = 1;       // A0 = strong low
      if( --i == 0 )
        break;                          // timeout
    }while( SENSKEY_B0_PIN == 0 );      // until charged
//  }
  SENSKEY_B0_DDR = 1;           // discharge cap
  return MAX_CYCLE - i;         // big value = key touched or timeout
}
