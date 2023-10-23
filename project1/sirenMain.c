#include <msp430.h>
#include "led.h"
#include "buzzer.h"
#include "libTimer.h"
#include "switches.h"
const int c = 3824;
const int cS =  3608;
const int d =  3405;
const int dS = 3214;
const int e = 3034;
const int f = 2863;
const int fS = 2703;
const int g = 2551;
const int gS = 2408;
const int a = 2273;
const int aS = 2145;
const int b = 2025;

volatile int song_index = 0;
volatile int song_playing = 0;
int melody[] = {3824, 3608, 3405, 3214, 3034, 2863, 2703, 2551, 2408, 2273, 2145, 2025};
int melody_length = sizeof(melody);
const int sallys_song[] = {e, 0, g, 0, a, 0, b, 0, e, 0, g, 1, e, 0, f, 1, f, 0, a, 0, b, 0, c, 0, a, 0, fS, 0, dS, 1, e, 1, b, 0, b, 0, c, 0, d, 0, g, 0, a, 1, b, 0, a, 1, a, 0, a, 0, b, 0, c, 0, c, 0, a, 0, fS, 0, dS, 1, e, 1, e, 0, b, 0, b, 0, c, 0, d, 0, g, 0, a, 1, b, 0, a, 1, a, 0, a, 1, b, 0, c, 1, c, 1, b, 1, b, 0, fS, 0, g, 0, a, 0, a, 0, g, 0, c, 0, a, 0, f, 0, e, 1, 0, 0};

// intended for songs @ 120 bmp
void
__interrupt_vec(WDT_VECTOR) WDT(){ /* 250 interrupts / sec */
  static char beat_count = 0;
  if(++beat_count == 125 && song_playing){  /* typical target_tempo = 125  play beat at every 0.5 seconds */
    int freq = sallys_song[song_index];
    int duration = sallys_song[song_index + 1];

    if(freq == 0){
      // stop
      buzzer_set_period(0);
      song_playing = 0;
      song_index = 0;
    }else{
      buzzer_set_period(freq);
      /* if(duration == 0){   // stop the note */
      /* 	__delay_cycles(10000000); */
      /* 	buzzer_set_period(0); */
      /* }  // otherwise hold the note */
    beat_count = 0;
    song_index += 2;
    
  }
}
}
void play_ss(){
  song_playing = 1;
  song_index = 0;
}

int main(void){
  configureClocks();
  buzzer_init();
  led_init();
  switch_init();
  enableWDTInterrupts();
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

/* void sallys_song(){ */
/*   /\* letter = note   number = hold the note(boolean) *\/ */
/*   int ss1[] =  {e, 0, g, 0, a, 0, b, 0, e, 0, g, 1, e, 0, f, 1, f, 0, a, 0}; */
/*   int ss2[] =  {b, 0, c, 0, a, 0, fS, 0, dS, 1, e, 1, b, 0}; */
/*   int ss3[] =  {b, 0, c, 0, d, 0, g, 0, a, 1, b, 0, a, 1, a, 0}; */
/*   int ss4[] =  {a, 0, b, 0, c, 0, c, 0, a, 0, fS, 0, dS, 1, e, 1}; */
/*   int ss5[] =  {e, 0, b, 0, b, 0, c, 0, d, 0, g, 0, a, 1, b, 0, a, 1}; */
/*   int ss6[] =  {a, 0, a, 1, b, 0, c, 1, c, 1, b, 1}; */
/*   int ss7[] =  {b, 0, fS, 0, g, 0, a, 0, a, 0, g, 0 c, 0, a, 0, f, 0, e, 1}; */
/*   int *ssPointer[7] = {ss1, ss2, ss3, ss4, ss5, ss6, ss7}; */
/*   int sallys_song[] = {e, 0, g, 0, a, 0, b, 0, e, 0, g, 1, e, 0, f, 1, f, 0, a, 0, b, 0, c, 0, a, 0, fS, 0, dS, 1, e, 1, b, 0, b, 0, c, 0, d, 0, g, 0, a, 1, b, 0, a, 1, a, 0, a, 0, b, 0, c, 0, c, 0, a, 0, fS, 0, dS, 1, e, 1, e, 0, b, 0, b, 0, c, 0, d, 0, g, 0, a, 1, b, 0, a, 1, a, 0, a, 1, b, 0, c, 1, c, 1, b, 1, b, 0, fS, 0, g, 0, a, 0, a, 0, g, 0 c, 0, a, 0, f, 0, e, 1, 0, 0};  */
/*   int sizeOfEachRow[7] = {12, 9, 10, 9, 10, 9, 10}; */
/*   int z = 0; */
/*   for(int k = 0; k < 2; k++){ */
/*     for(unsigned int i = 0; i < 7; i++){ */
/*       int *ptr = ssPointer[i]; */
/*       for(unsigned int j = 0; j < sizeOfEachRow[z]; j++){ */
/* 	buzzer_set_period((unsigned int) *ptr); */
/* 	__delay_cycles(10000000); // 10 Mil near-perfect beat */
/* 	buzzer_set_period(0); */
/* 	__delay_cycles(1000000); */
/* 	ptr++; */
/*       } */
/*       z++; */
/*     } */
/*   } //end of 1st for loop */
/* } */ 
/* void play_melody(){ */
/*   buzzer_set_volume(100); */
/*   for(int i = 0; i < melody_length; i++){ */
/*     buzzer_set_period(melody[i]); */
/*     __delay_cycles(100000000); */
/*     buzzer_set_period(0); */
/*     __delay_cycles(1000000); */
/*   } */
/* } */
