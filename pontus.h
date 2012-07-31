#ifndef pontus_h__
#define pontus_h__

void update_leds(void);
void setup_leds(void);
void setup_bounce_timer(void);
void toggle_leds(void);

/* NROW number of rows
   NCOL number of columns
   NKEY = NROW*NCOL */
#define NA    0
#define NROW  6
#define NCOL  18
#define NKEY  108

#define INTERRUPT_FUNCTION TIMER0_COMPA_vect

#endif
