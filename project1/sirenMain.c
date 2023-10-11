#include <msp430.h>
#include "led.h"
#include "libTimer.h"
#define SW1 BIT3
#define SW2 BIT4
#define SW3 BIT5
#define RED_LED BIT0
#define GREEN_LED BIT6

int main(void){
  configureClocks();
  led_init();
  enableWTDInterrupts();

  or_sr(0x18);
}
