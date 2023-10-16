#include <msp430.h>
#include "buzzer.h"
#include "libTimer.h"
void buzzer_init()
{
  /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
  */
  timerAUpmode();/* used to drive speaker */
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL &= ~BIT7;
  P2SEL |= BIT6;
  P2DIR = BIT6;/* enable output to speaker (P2.6) */

  TACTL = TASSEL_2 + MC_1; // use SMCLK up mode
  TACCR0 = 0; // set PWM period
  TACCR1 = 0; // set initial duty cycle
  TACCTL1 = OUTMOD_7; // Reset-set mode (generates PWM)
}
void buzzer_set_period(unsigned int cycles) /* buzzer clock = 2MHz.  (period of 1k results in 2kHz tone) */
{
  CCR0 = cycles;
  CCR1 = cycles >> 1;/* one half cycle */
}
void buzzer_set_frequency(float frequency){
  unsigned int period = (unsigned int)(10000000 / frequency); // calcualte period
  TACCR0 = period; // set period for given frequency
  //buzzer_set_period(period);
}
void buzzer_off(){
  TACCR1 = 0;
}
void buzzer_set_volume(unsigned int duty_cycle){
  if(duty_cycle >= 0 && duty_cycle <= TACCR0){
    TACCR1 = duty_cycle; // set duty cycle to control volume
  }
}
