#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"
#define SW0 BIT3   /* switch 1 is p1.3 */
#define SW1 BIT0
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3
#define SWITCHES SW0//, SW1, SW2, SW3, SW4 // one switch
#define BSWITCHES SW1 | SW2 | SW3 | SW4

int main() {
    configureClocks();

    P1REN |= SWITCHES; // ENABLE RESISTORS FOR SWITCHES
    P1IE |= SWITCHES;  // ENABLE INTERRUPTS FROM SWITCHES
    P1OUT |= SWITCHES; // PULL-UP FOR SWITCHES
    P1DIR &= ~SWITCHES; // SET SWITCHES' BITS FOR INPUT
    P2REN |= BSWITCHES;
    P2IE |= BSWITCHES;
    P2OUT |= BSWITCHES;
    P2DIR &= ~BSWITCHES;
    
    buzzer_init();
    // buzzer_set_period(3000);	/* start buzzing!!! 2MHz/1000 = 2kHz*/


    or_sr(0x18);          // CPU off, GIE on
}

void
switch_interrupt_handler(){
  char p1val = P1IN; //switch is in P1
  char p2val = P2IN;
  //update switch interrupt sense to detect changes from current buttons
  P1IES |= (p1val & SWITCHES); // if switch up, sense down
  P1IES &= (p1val | ~SWITCHES); // if switch down, sense up
  P2IES |= (p2val & BSWITCHES);
  P2IES &= (p2val & ~BSWITCHES);
  
  if(p1val & SW0){
    siren();
  }
  if(p2val & SW1){
    buzzer_set_period(1000);
  }
  if(p2val & SW2){
    buzzer_set_period(4000);
  }
  if(p2val & SW4){
    buzzer_set_period(100000);
  }
}
// Switch on P1
void
__interrupt_vec(PORT1_VECTOR) port_1(){
  if(P1IFG & SWITCHES){         // did a button cause this interrupt?
    P1IFG &= ~SWITCHES;         // clear pending sw interrupts
    switch_interrupt_handler(); // single handler for all switches
  }
}
void
__interrupt_vec(PORT2_VECTOR) port_2(){
  if(P2IFG & BSWITCHES){ 
    P2IFG &= ~BSWITCHES;
    switch_interrupt_handler();
  }
}
void
siren(){
  for(long int i = 1; i < 10000; i++){
    buzzer_set_period(i);
    
  }
  for(long int i = 10000; i > 1; i--){
    buzzer_set_period(i);
  }
  
}
