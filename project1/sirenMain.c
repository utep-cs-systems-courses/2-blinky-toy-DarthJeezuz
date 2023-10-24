#include <msp430.h>
#include "led.h"
#include "buzzer.h"
#include "libTimer.h"
#include "switches.h"
const float G = 5102;
const float A = 4546;
const float B = 4050;
const float dS = 3214;
const float e = 3034;
const float f = 2863;
const float fS = 2703;
const float g = 2551;
const float gS = 2408;
const float a = 2273;
const float aS = 2145;
const float b = 2025;
const float c = 1912;
const float cS = 1804;
const float d = 1702.5;
const float eN = 1517.0;
const float fN = 1431.5;
const float cN = 956;
const float dN = 851.25;

const float w = 0.0;

volatile int song_index = 0;
volatile int song_playing = 0;
char ss = 0;
char mulan = 0;
int melody[] = {dS, e, f, fS, g, gS, a, aS, b, c, cS, d};
int song[] = {};
int melody_length = sizeof(melody);
const int sallys_song[] = {e, 0, g, 0, a, 0, b, 0, e, 0, g, 0, e, 1, f, 2, f, 0, w, 0, w, 0, f, 0, a, 0, b, 0, c, 0, a, 0, fS, 0, dS, 0, e, 2, e, 0, w, 0, w, 0, b, 0, b, 0, c, 0, d, 0, g, 0, a, 0, b, 0, a, 2, a, 0, w, 0, w, 0, a, 0, a, 0, b, 0, c, 0, a, 0, fS, 0, dS, 0, e, 2, e, 0, w, 0, w, 0, b, 0, b, 0, c, 0, d, 0, g, 0, a, 0, b, 0, a, 2, a, 0, w, 0, w, 0, a, 0, b, 0, c, 0, c, 0, b, 0, w, 0, w, 0, fS, 0, g, 0, a, 0, a, 2, g, 2, g, 0, w, 0, w, 0, w, 0, c, 0, a, 0, f, 0, e, 0, w, 0, w, 0, e, 0, dS, 0, e, 0, fS, 0, g, 0, e, 2, e, 0, w, 0, w, 0, e, 0, dS, 0, e, 0, fS, 0, g, 0, e, 2, eN, 0, w, 0, w, 0, w, 0,};
const int reflection_mulan[] = {B, 0, c, 0, d, 2, w, 0, w, 0, B, 0, c, 0, d, 3, d, 0, g, 2,        B, 0, d, 0, c, 3, B, 0, A, 2, c, 0, c, 0, c, 3, d, 0, B, 0, A, 2,                                  B, 0, c, 0, d, 2, B, 0, c, 0, d, 0, g, 0, d, 0, g, 0, a, 2, w, 0, w, 0,                            B, 0, c, 0, d, 2, w, 0, B, 0, c, 0, d, 0, d, 2, g, 0, B, 0, d, 0, c, 0, B, 3, A, 2, w, 0, w, 0,    c, 0, c, 0, c, 0, d, 0, B, 0, A, 0, G, 2, w, 0,                                                    a, 2, a, 2, g, 0, a, 2, b, 0, g, 2,                                                                fS, 2, g, 0, e, 2, d, 2, g, 2, c, 2, w, 0, w, 0,                                                   B, 2, d, 0, g, 0, b, 3, b, 3, cN, 0, a, 0, g, 0, a, 2, a, 2, a, 2, w, 0, w, 0,                     a, 2, a, 0, g, 0, a, 0, b, 2, g, 2,                                                                fS, 0, g, 0, e, 2, d, 0, g, 0, c, 2,                                                               B, 2, d, 2, g, 0, b, 3, b, 3, cN, 0, A, 2,                                                         a, 2, b, 2, g, 2, a, 3, b, 2, w, 0, w, 0,                                                          B, 2, d, 0, g, 0, b, 3, b, 3, cN, 0, a, 2,                                                         a, 2, b, 2, g, 2, fS, 3, g, 2, g, 2};
// intended for songs @ 120 bmp
void
__interrupt_vec(WDT_VECTOR) WDT(){ /* 250 interrupts / sec */
  static char beat_count = 0;
  static char ext = 0; // flag for extending note
  static char shortPause = 0;
  
  if(beat_count == 124 && song_playing && ext == 0){
    buzzer_set_period(0);
  }
  if((++beat_count == 125 && song_playing) || (++beat_count == 63 && shortPause == 1 && song_playing)){  /* typical target_tempo = 125  play beat at every 0.5 seconds */
    float freq = 0;
    int duration = 0;
    if(shortPause == 1){
      shortPause = 0;
      buzzer_set_period(0);
      __delay_cycles(500000);
    }
    if(ss == 1){
      freq = sallys_song[song_index];
      duration = sallys_song[song_index + 1];
    }
    else if(mulan == 1){
      freq = reflection_mulan[song_index];
      duration = reflection_mulan[song_index + 1];
    }
    ext = 0;
    if(duration == 0){  // typical tempo
      buzzer_set_period(freq);
      
    }
    else if(duration == 1){  // 8th note (small pause just b4 note)
      __delay_cycles(500000);
      buzzer_set_period(freq);
    }
    else if(duration == 2){  // note must be extended
      buzzer_set_period(freq);
      __delay_cycles(10000000);
      ext = 1;
    }
    else if(duration == 3){ // small rest between notes
      buzzer_set_period(freq);
      shortPause = 1;
    }
    beat_count = 0;
    song_index += 2;    
  }
}

void play_s(int track){
  if(track == 1){
    ss = 1;
  }
  else if(track == 2){
    mulan = 1;
  }
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
void play_melody(){
  buzzer_set_volume(100);
  for(int i = 0; i < melody_length; i++){
    buzzer_set_period(melody[i]);
    __delay_cycles(100000000);
    buzzer_set_period(0);
    __delay_cycles(1000000);
  }
}
