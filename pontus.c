#include "pontus.h"
#include "teensy.h"
#include "usb_keyboard.h"

const uint8_t is_modifier[NKEY] = {
  true,            true,            false,           false,           false,           false,  // COL  0
  NA,              false,           NA,              false,           false,           NA,     // COL  1
  true,            false,           false,           false,           false,           false,  // COL  2
  true,            false,           false,           false,           false,           false,  // COL  3
  NA,              false,           false,           false,           false,           false,  // COL  4
  NA,              false,           false,           false,           false,           false,  // COL  5
  NA,              false,           false,           false,           false,           NA,     // COL  6
  false,           false,           NA,              NA,              false,           false,  // COL  7
  NA,              false,           false,           false,           false,           false,  // COL  8
  true,            false,           false,           false,           false,           false,  // COL  9
  NA,              false,           false,           false,           false,           false,  // COL 10
  true,            false,           false,           false,           false,           false,  // COL 11
  false,           false,           false,           false,           false,           false,  // COL 12
  NA,              true,            false,           false,           false,           false,  // COL 13
  true,            NA,              false,           false,           false,           false,  // COL 14

  false,           NA,              NA,              false,           false,           false,  // COL 15
  false,           false,           NA,              false,           false,           false,  // COL 16
  false,           NA,              NA,              false,           false,           false   // COL 17
};

const uint8_t layout[NKEY] = {
//ROW 0            ROW 1            ROW 2            ROW 3            ROW 4
  KEY_LEFT_CTRL,   KEY_LEFT_SHIFT,  KEY_CAPS_LOCK,   KEY_TAB,         KEY_TILDE,          KEY_ESC,        // COL  0
  NA,              KEY_PIPE,        KEY_A,           KEY_Q,           KEY_1,              NA,             // COL  1
  KEY_LEFT_GUI,    KEY_Z,           KEY_S,           KEY_W,           KEY_2,              KEY_F1,         // COL  2
  KEY_LEFT_ALT,    KEY_X,           KEY_D,           KEY_E,           KEY_3,              KEY_F2,         // COL  3
  NA,              KEY_C,           KEY_F,           KEY_R,           KEY_4,              KEY_F3,         // COL  4
  NA,              KEY_V,           KEY_G,           KEY_T,           KEY_5,              KEY_F4,         // COL  5
  NA,              KEY_B,           KEY_H,           KEY_Y,           KEY_6,              NA,             // COL  6
  KEY_SPACE,       KEY_N,           NA,              NA,              KEY_7,              KEY_F5,         // COL  7
  NA,              KEY_M,           KEY_J,           KEY_U,           KEY_8,              KEY_F6,         // COL  8
  KEY_RIGHT_ALT,   KEY_COMMA,       KEY_K,           KEY_I,           KEY_9,              KEY_F7,         // COL  9
  NA,              KEY_PERIOD,      KEY_L,           KEY_O,           KEY_0,              KEY_F8,         // COL 10
  KEY_RIGHT_GUI,   KEY_SLASH,       KEY_SEMICOLON,   KEY_P,           KEY_MINUS,          KEY_F9,         // COL 11
  KEY_APPLICATION, NA,              KEY_QUOTE,       KEY_LEFT_BRACE,  KEY_EQUAL,          KEY_F10,        // COL 12
  NA,              KEY_RIGHT_SHIFT, KEY_BACKSLASH,   KEY_RIGHT_BRACE, KEY_BACKSPACE,      KEY_F11,        // COL 13
  KEY_RIGHT_CTRL,  KEY_A,           NA,              KEY_ENTER,       NA,                 KEY_F12,        // COL 14

  KEY_LEFT,        NA,              NA,              KEY_DELETE,      KEY_INSERT,         KEY_PRINTSCREEN,// COL 15
  KEY_DOWN,        KEY_UP,          NA,              KEY_END,         KEY_HOME,           KEY_SCROLL_LOCK,// COL 16
  KEY_RIGHT,       NA,              NA,              KEY_PAGE_DOWN,   KEY_PAGE_UP,        KEY_PAUSE       // COL 17
};

uint8_t *const  row_ddr[NROW] = { _DDRB,  _DDRB,  _DDRB,  _DDRB,  _DDRB,  _DDRB};
uint8_t *const row_port[NROW] = {_PORTB, _PORTB, _PORTB, _PORTB, _PORTB, _PORTB};
uint8_t *const  row_pin[NROW] = { _PINB,  _PINB,  _PINB,  _PINB,  _PINB,  _PINB};
const uint8_t   row_bit[NROW] = { _BIT1,  _BIT2,  _BIT3,  _BIT4,  _BIT5,  _BIT6};

uint8_t *const  col_ddr[NCOL] = { _DDRC,  _DDRC,  _DDRC,  _DDRC,  _DDRC,  _DDRC,  _DDRC,  _DDRC,
				  _DDRD,  _DDRD,  _DDRD,  _DDRD,  _DDRD,  _DDRD,  _DDRD,  _DDRD,
				  _DDRE,  _DDRE};
uint8_t *const col_port[NCOL] = {_PORTC, _PORTC, _PORTC, _PORTC, _PORTC, _PORTC, _PORTC, _PORTC,
				 _PORTD, _PORTD, _PORTD, _PORTD, _PORTD, _PORTD, _PORTD, _PORTD,
				 _PORTE, _PORTE};
const uint8_t   col_bit[NCOL] = { _BIT0,  _BIT1,  _BIT2,  _BIT3,  _BIT4,  _BIT5,  _BIT6,  _BIT7,
				  _BIT0,  _BIT1,  _BIT2,  _BIT3,  _BIT4,  _BIT5,  _BIT6,  _BIT7,
				  _BIT0,  _BIT1};

/* */
inline void update_leds() {
  /* PORTC = (PORTC & 0x8F) | ((keyboard_leds << 4) & 0x70); */
  /* DDRC  = (DDRC  & 0x8F) | ((keyboard_leds << 4) & 0x70); */
}

/* LEDs are on output compare pins OC1B OC1C
   This activates fast PWM mode on them.
   Prescaler 256 and 8-bit counter results in
   16000000/256/256 = 244 Hz blink frequency.
   LED_A: Caps Lock
   LED_B: Scroll Lock  */
/* Output on PWM pins are turned off when the timer 
   reaches the value in the output compare register,
   and are turned on when it reaches TOP (=256). */
void setup_leds(void) {
  /* TCCR3A |=      // Timer control register 3A */
  /*   (1<<WGM30) | // Fast PWM 8-bit */
  /*   (1<<COM3A1)| // Clear OC3A on match, set at TOP */
  /*   (1<<COM3B1)| // Clear OC3B on match, set at TOP */
  /*   (1<<COM3C1); // Clear OC3C on match, set at TOP */
  /* TCCR3B |=      // Timer control register 3B */
  /*   (1<<WGM32) | // Fast PWM 8-bit */
  /*   (1<<CS32);   // Prescaler 256 */
  /* // Output on PWM pins are turned off when the timer  */
  /* // reaches the value in the output compare register, */
  /* // and turned on when it reaches TOP (=256). */
  /* OCR3A = 10;    // Output compare register 3A */
  /* OCR3B = 10;    // Output compare register 3B */
  /* OCR3C = 10;    // Output compare register 3C */
  /* // LEDs: LED_A -> PORTC4, LED_B -> PORTC5, LED_C -> PORTC6 */
  /* DDRC  &= 0x8F; */
  /* PORTC &= 0x8F; */
}

/* Timer0 is used to call the debounce algorithm
   at regular intervals via an interrupt.
   The interrupt is triggered when the timer reaches 
   the value in the output compare register. In CTC
   mode this also resets the timer. */
void setup_bounce_timer(void) {
  TCCR0A |=      // Timer control register 0A
    (1<<WGM01);  // Set CTC, clear timer on compare
  TCCR0B |=      // Timer control register 0A
//  (1<<CS00) |  // Prescaler 1024, frequency 15.6kHz
    (1<<CS02);   // Prescaler 256, frequency 62.5kHz
  TIMSK0 |=      // Timer interrupt mask register 0
    (1<<OCIE0A); // Enable timer interrupt on compare match with OCR0A
  OCR0A = 32;    // Output compare register 0A
}

/* Simple LED blinker for debugging use */
void toggle_leds(void) {
  //DDRB  = PORTB = ~DDRB & 0b11000000;
}
