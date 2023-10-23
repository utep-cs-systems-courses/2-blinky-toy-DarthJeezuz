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
void
switch_interrupt_handler(){
  char p1val = switch_update_interrupt_sense_port1();
  char p2val = switch_update_interrupt_sense_port2();
  switch_state_down = (p1val & SW0) ? 0 : 1; /* 0 when any switch is up */
  switch_state_down_xra = (p2val & SWITCHES) ? 0 : 1; /* 0 when any switch is up */
  switch_state_changed = 1;
  led_update();
  if(p1val & SW0){
    //euroSiren();
    //play_melody();
    // sallys_song();
    play_ss();
  }
  else if(p2val & SW1){
    //something
  }
  else if(p2val & SW2){
    //something
  }
  else if(p2val & SW3){
    //something
  }
  else if(p2val & SW4){
    //something
  }
}
void
__interrupt_vec(PORT1_VECTOR)port_1(){
  if(P1IFG & SW0){ //Button pressed?
    P1IFG &= ~SW0; //Clear pending switch
    switch_interrupt_handler();
  }
}
void
__interrupt_vec(PORT2_VECTOR)port_2(){
  if(P2IFG & SWITCHES){ //Button pressed?
    P2IFG &= ~SWITCHES; //Clear pending switches
    switch_interrupt_handler();
  }
}
