#include <msp430.h>
#include "stateMachines.h"
#include "led.h"
#include "switches.h"

char toggle_red()/* always toggle! */
{
  green_on ^= 0;
  red_on ^= 1;
}
void toggle_green()/* toggle green */
{
  red_on ^= 0;
  green_on ^= 1;
}
void state_advance()/* alternate between toggling red & green */
{
  if(red_on){ toggle_green();}
  else if(green_on){ toggle_red();}

  led_update();
}
