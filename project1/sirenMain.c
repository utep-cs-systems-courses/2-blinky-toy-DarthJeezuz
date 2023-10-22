#include <msp430.h>
#include "led.h"
#include "buzzer.h"
#include "libTimer.h"
#include "switches.h"
int c = 3824;
int cS =  3608;
int d =  3405;
int dS = 3214;
int e = 3034;
int f = 2863;
int fS = 2703;
int g = 2551;
int gS = 2408;
int a = 2273;
int aS = 2145;
int b = 2025;

int melody[] = {3824, 3608, 3405, 3214, 3034, 2863, 2703, 2551, 2408, 2273, 2145, 2025};
int melody_length = sizeof(melody);

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
  led_init();
  switch_init();
  or_sr(0x18);
}
void euroSiren(){
  P1OUT |= RED;
  unsigned int count = 0;
  while(1){

    if(count == 0){
      P1OUT ^= RED;
      P1OUT ^= GREEN;
      //buzzer_set_frequency(100);
      //buzzer_set_volume(15000);
      buzzer_set_period(1000);
      count++;  
    }
    else{
      P1OUT ^= GREEN;
      P1OUT ^= RED;
      //buzzer_set_frequency(7000);
      //buzzer_set_volume(15000);
      buzzer_set_period(10000);
      count = 0;
    }
    __delay_cycles(7000000);    
    buzzer_set_period(0);
    __delay_cycles(350000);
  }
}

void play_melody(){
  buzzer_set_volume(100);
  for(int i = 0; i < melody_length; i++){
    buzzer_set_period(melody[i]);
    __delay_cycles(100000000);
    buzzer_set_period(0);
    __delay_cycles(1000000);
  }
}
void sallys_song(){
  int ss1[] =  {e, g, a, b, e, g, g, e, f, f, f, a };
  int ss2[] =  {b, c, a, fS, dS, dS, e, e, b};
  int ss3[] =  {b, c, d, g, a, a, b, a, a, a};
  int ss4[] =  {a, b, c, c, a, fS, dS, dS, e};
  int ss5[] =  {e, b, b, c, d, g, a, a, b, a};
  int ss6[] =  {a, a, a, b, c, c, c, c, b};
  int ss7[] =  {b, fS, g, a, a, g, c, a, f, e};
  int *ssPointer[7] = {ss1, ss2, ss3, ss4, ss5, ss6, ss7};
  int sizeOfEachRow[7] = {12, 9, 10, 9, 10, 9, 10};
  int z = 0;
  for(int k = 0; k < 2; k++){
    for(unsigned int i = 0; i < 7; i++){
      int *ptr = ssPointer[i];
      for(unsigned int j = 0; j < sizeOfEachRow[z]; j++){
	buzzer_set_period((unsigned int) *ptr);
	__delay_cycles(10000000); // 10 Mil near-perfect beat
	buzzer_set_period(0);
	__delay_cycles(1000000);
	ptr++;
      }
      z++;
    }
  } //end of 1st for loop
}
