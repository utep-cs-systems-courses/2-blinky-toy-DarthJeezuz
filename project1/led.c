#include <msp430.h>
#include "led.h"
#include "switches.h"

unsigned char red_on = 0, green_on = 0;
unsigned char led_changed = 0;
//static char redVal[] = {0, red_on}, greenVal[] = {0, green_on};

void led_init()
{
  P1DIR |= LEDS;// bits attached to leds are output
  led_changed = 1;
  led_update();
}
void led_update(){
  if(switch_state_changed){
    char ledFlags = 0;

    ledFlags |= switch_state_down ? GREEN : 0;
    ledFlags |= switch_state_down ? 0 : RED;

    P1OUT &= (0xff -LEDS) | ledFlags;
    P1OUT |= ledFlags;
  }
  switch_state_changed = 0;
}
