#ifndef iso_h__
#define iso_h__

//#define F_CPU 16000000

void update_leds(void);
void setup_leds(void);
void setup_bounce_timer(void);
void toggle_leds(void);

/* NROW number of rows
   NCOL number of columns
   NKEY = NROW*NCOL */
#define NA    0
#define NROW  6
#define NCOL  22
#define NKEY  132

#define INTERRUPT_FUNCTION TIMER0_COMPA_vect


#endif
