#include <msp430.h>
#include "switches.h"
#include "led.h"

char switch_state_down, switch_state_down_xra, switch_state_changed; /* effectively boolean */


static char switch_update_interrupt_sense_port1(){
  char p1val = P1IN;
  /* update switch interrupt to detect changes from current buttons */
  P1IES |= (p1val & SW0);/* if switch up, sense down */
  P1IES &= (p1val | ~SW0);/* if switch down, sense up */
  return p1val;
}
static char switch_update_interrupt_sense_port2(){
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);/* if switch down, sense up */
  return p2val;
}
void
switch_interrupt_handler_port1(){
  char p1val = switch_update_interrupt_sense_port1();
  switch_state_down = (p1val & SW0) ? 0 : 1; /* 0 when any switch is up */
  switch_state_changed = 1;
  led_update();
  if(p1val & SW0){
    euroSiren();
    //play_melody();
    
  }
}
void
switch_interrupt_handler_port2(){
  char p2val = switch_update_interrupt_sense_port2();
  //switch_state_down_xra = (p2val & SWITCHES) ? 0 : 1; /* 0 when any switch is up */
  char sw1 = (p2val & SW1) ? 0 : SW1;
  char sw2 = (p2val & SW2) ? 0 : SW2;
  char sw3 = (p2val & SW3) ? 0 : SW3;
  char sw4 = (p2val & SW4) ? 0 : SW4;
  switch_state_changed = 1;
  led_update();
  if(sw1){
    play_s(1);
  }
  else if(sw2){
    play_s(2);
  }
  else if(sw3){
    play_s(3);
  }
  else if(sw4){
    play_s(4);
  }
}
void
__interrupt_vec(PORT1_VECTOR)port_1(){
  if(P1IFG & SW0){ //Button pressed?
    P1IFG &= ~SW0; //Clear pending switch
    switch_interrupt_handler_port1();
  }
}
void
__interrupt_vec(PORT2_VECTOR)port_2(){
  if(P2IFG & SWITCHES){ //Button pressed?
    P2IFG &= ~SWITCHES; //Clear pending switches
    switch_interrupt_handler_port2();
  }
}
void switch_init(){
  P1REN |= SW0;/* enables resistors for board switch */
  P1IE |= SW0;/* enable interrupts for board switch */
  P1OUT |= SW0;/* pull-ups for board switch */
  P1DIR &= ~SW0;/* set bit for input */

  P2REN |= SWITCHES;/* enables resistors for switches */
  P2IE |= SWITCHES;/* enable interrupts for switches */
  P2OUT |= SWITCHES;/* pull-ups for switches */
  P2DIR &= ~SWITCHES;/* set switches' bits for input */
  switch_update_interrupt_sense_port1();
  switch_update_interrupt_sense_port2();
  led_update();
}
