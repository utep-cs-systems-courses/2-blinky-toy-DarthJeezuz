#include <msp430.h>
#include "libTimer.h"
/*Clocktimer.c*/

void configureClocks(){
  WDTCTL = WDTPW + WDTHOLD; // Disable WD timer
  BCSCTL1 = CALBC1_16MHZ; // set DCO to 16 MHz
  DCOCTL = CALDCO_16MHZ;

  BCSCTL2 &= ~(SELS);  // SMCLK source = DCO
  BCSCTL2 |= DIVS_3;  // SMCLK = DCO / 8
}

// enable WD timer periodic interrupt
// period = SMCLOCK/32k
void enableWDTInterrupts(){
  WDTCTL = WDTPW |  //password required Else device resets
    WDTTMSEL |      // watchdog interval mode
    WDTCNTCL |     //clear watchdog count
    1;             // divide SMCLK by 8192
  IE1 |= WDTIE;    // Enable watchdog interval timer interrupt
}

void timerAUpmode(){
  TA0CCR0 = 0;
  TA0CCR1 = 0;
  TA0CCTL1 = OUTMOD_3;    // toggle p1.6 when timer = count1

  //timer A control:
  // timer clock source 2: system clock (SMCLK)
  // Mode control 1: continuously 0...CCR0
  TACTL = TASSEL_2 + MC_1;
}