#include <msp430.h>
//#include "led.h"
#include "buzzer.h"
#include "libTimer.h"
//#include "switches.h"
#define RED BIT0
#define GREEN BIT6
#define LEDS (BIT0 | BIT6)
#define SW1 BIT3
#define SWITCHES SW1
#define c 3824
#define cS 3608
#define d 3405
#define dS 3214
#define e 3034
#define f 2863
#define fS 2703
#define g 2551
#define gS 2408
#define a 2273
#define aS 2145
#define b 2025

void tone(int tone, int duration){
  for(int i = 0; i < duration; i++){
    CCR0 = tone; //tone length
    CCR1 = tone/2; // half tone ON and half tone OFF
    __delay_cycles(1000);
  }
}


int main(void){
  configureClocks();
  buzzer_init();
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
    buzzer_set_period(0);
    buzzer_off();
    if(RED){
      P1OUT ^= RED;
      P1OUT ^= GREEN;
//    buzzer_set_frequency(500);
      //buzzer_set_volume(1000);
      buzzer_set_period(500);
        
    }
    else{
      
      P1OUT ^= GREEN;
      P1OUT ^= RED;
//      buzzer_set_frequency(8000);
      //buzzer_set_volume(15000);
      buzzer_set_period(8000);
     
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

