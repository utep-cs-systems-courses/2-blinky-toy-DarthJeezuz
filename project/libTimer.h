#ifndef libTimer_included
#define libTimer_included

void configureClocks();
void enableWDTInterrupts();
void timerAUpmode();

unsigned char get_sr();
void set_sr(), or_sr(), and_sr();

#endif // included
