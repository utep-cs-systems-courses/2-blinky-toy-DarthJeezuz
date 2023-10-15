#include <msp430.h>
//#include "led.h"
#include "libTimer.h"
//#include "switches.h"
#define SW1 BIT3
#define RED BIT0
#define GREEN BIT6
#define LEDS (BIT0 | BIT6)
#define SW1 BIT3
#define SWITCHES SW1

int main(void){
  configureClocks();
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;
  P1REN |= SWITCHES;
  P1IE |= SWITCHES;
  P1OUT |= SWITCHES;
  P1DIR &= ~SWITCHES;

  or_sr(0x18);
}
void euroSiren(){
  P1OUT |= RED;
  while(1){
    if(RED){
      P1OUT ^= RED;
      P1OUT ^= GREEN;
    }
    else{
      P1OUT ^= GREEN;
      P1OUT ^= RED;
    }
    __delay_cycles(7000000);
  }
}
void switch_interrupt_handler(){
  char p1val = P1IN;

  P1IES |= (p1val & SWITCHES);
  P1IES &= (p1val | ~SWITCHES);
  if(p1val & SW1){
    euroSiren();
  }
}
void __interrupt_vec(PORT1_VECTOR) Port_1(){
  if(P1IFG & SWITCHES){
    P1IFG &= ~SWITCHES;
    switch_interrupt_handler();
  }
}

