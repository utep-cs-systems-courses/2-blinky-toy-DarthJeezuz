#ifndef libTimer_included
#define libTimer_included

void configureClocks();
void enableWDTInterrupts();
void timerAUpmode();
void buzzer_set_frequency();
void buzzer_off();
void buzzer_set_volume();

unsigned char gert_sr();
void set_sr(), or_sr(), and_sr();

#endif // included
