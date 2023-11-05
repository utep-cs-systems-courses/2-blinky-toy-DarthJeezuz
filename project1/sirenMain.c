#include <msp430.h>
#include "led.h"
#include "buzzer.h"
#include "libTimer.h"
#include "switches.h"

const float ASS = 8580;
const float GSS = 9632;
const float DS = 6428;
const float E = 6068;
const float F = 5726;
const float FS = 5406;
const float G = 5102;
const float GS = 4816;
const float AS = 4290; // B-flat equivalent
const float A = 4546;
const float B = 4050;
const float C_ = 3824;
const float CS = 3608;
const float D = 3404;
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
const float aN = 1136.5;
const float cN = 956;
const float dN = 851.25;

const float w = 0.0;

volatile int song_index = 0;
volatile int song_playing = 0;
volatile char usmc = 0;
volatile char ff = 0;
volatile char ss = 0;
volatile char mulan = 0;
const int ffVictory[] = {a, 1, a, 1, a, 1, a, 2, fS, 0, gS, 0, aN, 0, gS, 1, aN, 2};

const int mcHymn[] = {F, 0, A, 0, C_, 0, w, 0, C_, 0, w, 0, C_, 0, w, 0, C_, 0, w, 0, C_, 1, f, 0, C_, 0, w, 0, A, 0, AS, 0, C_, 0, w, 0, C_, 0, w, 0, AS, 0, G, 1, w, 0, F, 2, F, 0, w, 0, F, 0, A, 0, C_, 0, w, 0, C_, 0, w, 0, C_, 0, w, 0, C_, 0, w, 0, C_, 1, w, 0, f, 0, C_, 0, w, 0, A, 0, AS, 0, C_, 0, w, 0, C_, 0, w, 0, AS, 0, G, 1, w, 0, F, 2}; 

// 120 bpm
const int sallys_song[] = {e, 0, g, 0, a, 0, b, 0, e, 0, g, 0, e, 1, f, 2, f, 0, w, 0, f, 0, a, 0, b, 0, c, 0, a, 0, fS, 0, dS, 0, e, 2, e, 0, w, 0, b, 0, b, 0, c, 0, d, 0, g, 0, a, 0, b, 0, a, 2, a, 0, w, 0, a, 0, a, 0, b, 0, c, 0, a, 0, fS, 0, dS, 0, e, 2, e, 0, w, 0, b, 0, b, 0, c, 0, d, 0, g, 0, a, 0, b, 0, a, 2, a, 0, w, 0, a, 0, b, 0, c, 0, c, 2, b, 0, w, 0, fS, 0, g, 0, a, 0, a, 2, g, 2, w, 0, c, 0, a, 0, f, 0, e, 0, w, 0, e, 0, dS, 0, e, 0, fS, 0, g, 0, e, 2, e, 0, w, 0, e, 0, dS, 0, e, 0, fS, 0, g, 0, e, 2, eN, 0, w, 0, w, 0, w, 0,};
// 95 bpm
const int reflection_mulan[] = {C_, 0, CS, 0, DS, 2, w, 0, w, 0, C_, 0, DS, 0, DS, 2, DS, 0, GS, 2,  C_, 0, DS, 0, CS, 3, C_, 0, ASS, 2, CS, 0, CS, 0, CS, 3, DS, 0, C_, 0, ASS, 2,                                  C_, 0, CS, 0, DS, 2, C_, 0, CS, 0, DS, 0, GSS, 0, DS, 0, GSS, 0, AS, 2, w, 0, w, 0,                            C_, 0, CS, 0, DS, 2, w, 0, C_, 0, CS, 0, DS, 0, DS, 2, GSS, 0, C_, 0, DS, 0, CS, 0, C_, 3, ASS, 2, w, 0, w, 0,    CS, 0, CS, 0, CS, 0, DS, 0, C_, 0, ASS, 0, G, 2, w, 0,                                                    AS, 2, AS, 2, GSS, 0, AS, 2, c, 0, GSS, 2,                                                                fS, 2, GSS, 0, e, 2, DS, 2, GSS, 2, CS, 2, w, 0, w, 0,                                                   C_, 2, DS, 0, GSS, 0, c, 3, c, 3, cN, 0, AS, 0, GSS, 0, AS, 2, AS, 2, AS, 2, w, 0, w, 0,                     AS, 2, AS, 0, GSS, 0, AS, 0, c, 2, GSS, 2,                                                                fS, 0, GSS, 0, e, 2, DS, 0, GSS, 0, CS, 2,                                                               C_, 2, DS, 2, GSS, 0, c, 3, c, 3, cN, 0, ASS, 2,                                                         AS, 2, c, 2, GSS, 2, AS, 3, c, 2, w, 0, w, 0,                                                          C_, 2, DS, 0, GSS, 0, c, 3, c, 3, cN, 0, AS, 2,                                                         AS, 2, c, 2, GSS, 2, fS, 3, GSS, 2, GSS, 2};

// intended for songs @ 120 bmp
void
__interrupt_vec(WDT_VECTOR) WDT(){ /* 250 interrupts / sec */
  static char beat_count = 0;
  static char ext = 0; // flag for extending note
  static float freq = 0;
  static int duration = 0;
  if(ss){
    freq = sallys_song[song_index];
    duration = sallys_song[song_index + 1];
  }
  if(mulan){
    freq = reflection_mulan[song_index];
    duration = reflection_mulan[song_index + 1];
  }
  if(ff){
    freq = ffVictory[song_index];
    duration = ffVictory[song_index + 1];
  }
  if(usmc){
    freq = mcHymn[song_index];
    duration = mcHymn[song_index + 1];
  }
  
  //sallys song
  /* typical target_tempo = 125  play beat at every 0.5 seconds */
  if(beat_count == 125 && song_playing && ss){  

    if(ext == 0){ buzzer_set_period(0);} // small break betwixt notes

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
    beat_count = 0;
    song_index += 2;    
  }
  //Reflection song
  if(beat_count == 125 && song_playing && mulan){
    if(ext == 0){ buzzer_set_period(0);} // small break betwixt notes

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
    beat_count = 0;
    song_index += 2;    

  }
  //Final Fantasy Victory tune
  if(beat_count == 70 && song_playing && ff){

    if(ext == 0){ buzzer_set_period(0);} // small break betwixt notes
    ext = 0;
    if(duration == 0){
      buzzer_set_period(freq);
    }
    if(duration == 1){ // rapid play & pause
      buzzer_set_period(freq);
      __delay_cycles(1000000);
      buzzer_set_period(0);
      beat_count = 25;
      
    }
    if(duration == 2){ // extend note
      buzzer_set_period(freq);
      __delay_cycles(1000000);
      //ext = 1;
      beat_count = 20;
    }
    song_index += 2;
  }

  // Marine Corps Hymn
  if(beat_count == 70 && song_playing && usmc){
    buzzer_set_period(0);
    if(duration == 0){ // quarter beat
      buzzer_set_period(freq);
    }
    if(duration == 1){
      buzzer_set_period(freq);
      __delay_cycles(1000000);
    }
    if(duration == 2){
      buzzer_set_period(freq);
      __delay_cycles(5000000);
    }
    beat_count = 0;
    song_index += 2;
  }
  beat_count++;
}

void play_s(int track){
  if(track == 1){
    ss = 1;
    mulan = 0;
    ff = 0;
    usmc = 0;
  }
  else if(track == 2){
    mulan = 1;
    ss = 0;
    ff = 0;
    usmc = 0;
  }
  else if(track == 3){
    ff = 1;
    ss = 0;
    mulan = 0;
    usmc = 0;
  }
  else if(track == 4){
    usmc = 1;
    ss = 0;
    mulan = 0;
    ff = 0;
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
/* void play_melody(){ */
/*   buzzer_set_volume(100); */
/*   for(int i = 0; i < melody_length; i++){ */
/*     buzzer_set_period(melody[i]); */
/*     __delay_cycles(100000000); */
/*     buzzer_set_period(0); */
/*     __delay_cycles(1000000); */
/*   } */
/* } */
