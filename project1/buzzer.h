#ifndef buzzer_included
#define buzzer_included

void buzzer_init();
void buzzer_set_period(unsigned int cycles);
void buzzer_set_frequency(float frequency);
void buzzer_off();
void buzzer_set_volume(unsigned int duty_cycle);

#endif // included
