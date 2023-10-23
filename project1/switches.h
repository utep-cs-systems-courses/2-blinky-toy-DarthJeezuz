#ifndef switches_included
#define switches_included
#define SW0 BIT3 /* board switch is p1.3 */
#define SW1 BIT0 // following switches are from expanssion board, respectively
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3
#define SWITCHES SW1 | SW2 | SW3 | SW4
#define ALLSWITCHES SW0 | SWITCHES

void switch_init();
void switch_interrupt_handler();
extern char switch_state_down, switch_state_down_xra, switch_state_changed; /* effectively boolean */
//extern int song_index, song_playing;
#endif // included
