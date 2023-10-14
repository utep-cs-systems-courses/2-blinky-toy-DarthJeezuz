#include <msp430.h>
#include "led.h"
#include "libTimer.h"
#include "switches.h"
#define SW1 BIT3
#define SW2 BIT4
#define SW3 BIT5
#define SWITCHES SW1
#define RED_LED BIT0
#define GREEN_LED BIT6

int main(void){
  configureClocks();
  led_init();
  switch_init();
  enableWDTInterrupts();

  or_sr(0x18);
}

