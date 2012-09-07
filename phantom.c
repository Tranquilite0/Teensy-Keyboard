#include "phantom.h"
#include "teensy.h"
#include "usb_keyboard.h"

/* Specify which switch is the function key. E.g. SW0_13 which would be right control on a standard ansiwin layout phantom */
const uint8_t fun_sw = SW0_13;

/* Specify which switch will toggle which layer when combined with function key */ 
const uint8_t layer_sw[NFUN] = {SW4_9, SW4_0, SW4_1, SW4_2, SW4_3, SW4_4, SW4_5};

/* Specify which LEDs handle which function. Options are: SCRL, CAPS, NUM, CMPS (compose), KANA */
uint8_t leds[2] = {
	CAPS, //LED_A (the left one)
	SCRL, //LED_B (the right one)
};

const uint16_t layout0[NKEY] = {
//ROW 0            ROW 1            ROW 2            ROW 3            ROW 4
  KEY_LEFT_CTRL,   KEY_LEFT_SHIFT,  KEY_CAPS_LOCK,   KEY_TAB,         KEY_1,              KEY_ESC,        // COL  0
  KEY_LEFT_GUI,    KEY_PIPE,        KEY_A,           KEY_Q,           KEY_2,              KEY_TILDE,      // COL  1
  KEY_LEFT_ALT,    KEY_Z,           KEY_S,           KEY_W,           KEY_3,              KEY_F1,         // COL  2
  NA,              KEY_X,           KEY_D,           KEY_E,           KEY_4,              KEY_F2,         // COL  3
  NA,              KEY_C,           KEY_F,           KEY_R,           KEY_5,              KEY_F3,         // COL  4
  NA,              KEY_V,           KEY_G,           KEY_T,           KEY_6,              KEY_F4,         // COL  5
  NA,              KEY_B,           KEY_H,           KEY_Y,           KEY_7,              KEY_F5,         // COL  6
  KEY_SPACE,       KEY_N,           KEY_J,           KEY_U,           KEY_8,              KEY_F6,         // COL  7
  NA,              KEY_M,           KEY_K,           KEY_I,           KEY_9,              KEY_F7,         // COL  8
  NA,              KEY_COMMA,       KEY_L,           KEY_O,           KEY_0,              KEY_F8,         // COL  9
  KEY_RIGHT_ALT,   KEY_PERIOD,      KEY_SEMICOLON,   KEY_P,           KEY_MINUS,          KEY_F9,         // COL 10
  KEY_RIGHT_GUI,   KEY_SLASH,       KEY_QUOTE,       KEY_LEFT_BRACE,  KEY_EQUAL,          KEY_F10,        // COL 11
  KEY_APPLICATION, NA,              KEY_BACKSLASH,   KEY_RIGHT_BRACE, NA,                 KEY_F11,        // COL 12
  NA,              KEY_RIGHT_SHIFT, KEY_ENTER,       KEY_BACKSLASH,   KEY_BACKSPACE,      KEY_F12,        // COL 13

  KEY_LEFT,        NA,              NA,              KEY_DELETE,      KEY_INSERT,         KEY_PRINTSCREEN,// COL 14
  KEY_DOWN,        KEY_UP,          NA,              KEY_END,         KEY_HOME,           KEY_SCROLL_LOCK,// COL 15
  KEY_RIGHT,       NA,              NA,              KEY_PAGE_DOWN,   KEY_PAGE_UP,        KEY_PAUSE,      // COL 16
};

const uint16_t layout1[NKEY] = {
//ROW 0            ROW 1            ROW 2            ROW 3            ROW 4
  KEY_LEFT_CTRL,   KEY_LEFT_SHIFT,  KEY_CAPS_LOCK,   KEY_TAB,         KEY_1,              KEY_ESC,        // COL  0
  KEY_LEFT_GUI,    KEY_PIPE,        KEY_A,           KEY_Q,           KEY_2,              KEY_TILDE,      // COL  1
  KEY_LEFT_ALT,    KEY_Z,           KEY_S,           KEY_W,           KEY_3,              KEY_F1,         // COL  2
  NA,              KEY_X,           KEY_D,           KEY_E,           KEY_4,              KEY_F2,         // COL  3
  NA,              KEY_C,           KEY_F,           KEY_R,           KEY_5,              KEY_F3,         // COL  4
  NA,              KEY_V,           KEY_G,           KEY_T,           KEY_6,              KEY_F4,         // COL  5
  NA,              KEY_B,           KEY_H,           KEY_Y,           KEY_7,              KEY_F5,         // COL  6
  KEY_SPACE,       KEY_N,           KEY_J,           KEY_U,           KEY_8,              KEY_F6,         // COL  7
  NA,              KEY_M,           KEY_K,           KEY_I,           KEY_9,              KEY_F7,         // COL  8
  NA,              KEY_COMMA,       KEY_L,           KEY_O,           KEY_0,              KEY_F8,         // COL  9
  KEY_RIGHT_ALT,   KEY_PERIOD,      KEY_SEMICOLON,   KEY_P,           KEY_MINUS,          KEY_F9,         // COL 10
  KEY_RIGHT_GUI,   KEY_SLASH,       KEY_QUOTE,       KEY_LEFT_BRACE,  KEY_EQUAL,          KEY_F10,        // COL 11
  KEY_APPLICATION, NA,              KEY_BACKSLASH,   KEY_RIGHT_BRACE, NA,                 KEY_F11,        // COL 12
  NA,              KEY_RIGHT_SHIFT, KEY_ENTER,       KEY_BACKSLASH,   KEY_BACKSPACE,      KEY_F12,        // COL 13

  KEYPAD_0,        KEYPAD_1,        KEYPAD_4,        KEYPAD_7,        KEYPAD_SLASH,       KEY_PRINTSCREEN,// COL 14
  KEYPAD_PERIOD,   KEYPAD_2,        KEYPAD_5,        KEYPAD_8,        KEYPAD_ASTERIX,     KEY_SCROLL_LOCK,// COL 15
  KEYPAD_PLUS,     KEYPAD_3,        KEYPAD_6,        KEYPAD_9,        KEYPAD_MINUS,       KEY_PAUSE,      // COL 16
};

const uint16_t layout2[NKEY] = {
//ROW 0            ROW 1            ROW 2            ROW 3            ROW 4
  KEY_LEFT_CTRL,   KEY_LEFT_SHIFT,  KEY_CAPS_LOCK,   KEY_TAB,         KEY_1,              KEY_ESC,        // COL  0
  KEY_LEFT_GUI,    KEY_PIPE,        KEY_A,           KEY_Q,           KEY_2,              KEY_TILDE,      // COL  1
  KEY_LEFT_ALT,    KEY_Z,           KEY_S,           KEY_W,           KEY_3,              KEY_F1,         // COL  2
  NA,              KEY_X,           KEY_D,           KEY_E,           KEY_4,              KEY_F2,         // COL  3
  NA,              KEY_C,           KEY_F,           KEY_R,           KEY_5,              KEY_F3,         // COL  4
  NA,              KEY_V,           KEY_G,           KEY_T,           KEY_6,              KEY_F4,         // COL  5
  NA,              KEY_B,           KEY_H,           KEY_Y,           KEY_7,              KEY_F5,         // COL  6
  KEY_SPACE,       KEY_N,           KEY_J,           KEY_U,           KEY_8,              KEY_F6,         // COL  7
  NA,              KEY_M,           KEY_K,           KEY_I,           KEY_9,              KEY_F7,         // COL  8
  NA,              KEY_COMMA,       KEY_L,           KEY_O,           KEY_0,              KEY_F8,         // COL  9
  KEY_RIGHT_ALT,   KEY_PERIOD,      KEY_SEMICOLON,   KEY_P,           KEY_MINUS,          KEY_F9,         // COL 10
  KEY_RIGHT_GUI,   KEY_SLASH,       KEY_QUOTE,       KEY_LEFT_BRACE,  KEY_EQUAL,          KEY_F10,        // COL 11
  KEY_APPLICATION, NA,              KEY_BACKSLASH,   KEY_RIGHT_BRACE, NA,                 KEY_F11,        // COL 12
  KEY_RIGHT_CTRL,  KEY_RIGHT_SHIFT, KEY_ENTER,       KEY_BACKSLASH,   KEY_BACKSPACE,      KEY_F12,        // COL 13

  KEY_LEFT,        NA,              NA,              KEY_DELETE,      KEY_INSERT,         KEY_PRINTSCREEN,// COL 14
  KEY_DOWN,        KEY_UP,          NA,              KEY_END,         KEY_HOME,           KEY_SCROLL_LOCK,// COL 15
  KEY_RIGHT,       NA,              NA,              KEY_PAGE_DOWN,   KEY_PAGE_UP,        KEY_PAUSE,      // COL 16
};

const uint16_t layout3[NKEY] = {
//ROW 0            ROW 1            ROW 2            ROW 3            ROW 4
  KEY_LEFT_CTRL,   KEY_LEFT_SHIFT,  KEY_CAPS_LOCK,   KEY_TAB,         KEY_1,              KEY_ESC,        // COL  0
  KEY_LEFT_GUI,    KEY_PIPE,        KEY_A,           KEY_Q,           KEY_2,              KEY_TILDE,      // COL  1
  KEY_LEFT_ALT,    KEY_Z,           KEY_S,           KEY_W,           KEY_3,              KEY_F1,         // COL  2
  NA,              KEY_X,           KEY_D,           KEY_E,           KEY_4,              KEY_F2,         // COL  3
  NA,              KEY_C,           KEY_F,           KEY_R,           KEY_5,              KEY_F3,         // COL  4
  NA,              KEY_V,           KEY_G,           KEY_T,           KEY_6,              KEY_F4,         // COL  5
  NA,              KEY_B,           KEY_H,           KEY_Y,           KEY_7,              KEY_F5,         // COL  6
  KEY_SPACE,       KEY_N,           KEY_J,           KEY_U,           KEY_8,              KEY_F6,         // COL  7
  NA,              KEY_M,           KEY_K,           KEY_I,           KEY_9,              KEY_F7,         // COL  8
  NA,              KEY_COMMA,       KEY_L,           KEY_O,           KEY_0,              KEY_F8,         // COL  9
  KEY_RIGHT_ALT,   KEY_PERIOD,      KEY_SEMICOLON,   KEY_P,           KEY_MINUS,          KEY_F9,         // COL 10
  KEY_RIGHT_GUI,   KEY_SLASH,       KEY_QUOTE,       KEY_LEFT_BRACE,  KEY_EQUAL,          KEY_F10,        // COL 11
  KEY_APPLICATION, NA,              KEY_BACKSLASH,   KEY_RIGHT_BRACE, NA,                 KEY_F11,        // COL 12
  KEY_RIGHT_CTRL,  KEY_RIGHT_SHIFT, KEY_ENTER,       KEY_BACKSLASH,   KEY_BACKSPACE,      KEY_F12,        // COL 13

  KEY_LEFT,        NA,              NA,              KEY_DELETE,      KEY_INSERT,         KEY_PRINTSCREEN,// COL 14
  KEY_DOWN,        KEY_UP,          NA,              KEY_END,         KEY_HOME,           KEY_SCROLL_LOCK,// COL 15
  KEY_RIGHT,       NA,              NA,              KEY_PAGE_DOWN,   KEY_PAGE_UP,        KEY_PAUSE,      // COL 16
};

const uint16_t layout4[NKEY] = {
//ROW 0            ROW 1            ROW 2            ROW 3            ROW 4
  KEY_LEFT_CTRL,   KEY_LEFT_SHIFT,  KEY_CAPS_LOCK,   KEY_TAB,         KEY_1,              KEY_ESC,        // COL  0
  KEY_LEFT_GUI,    KEY_PIPE,        KEY_A,           KEY_Q,           KEY_2,              KEY_TILDE,      // COL  1
  KEY_LEFT_ALT,    KEY_Z,           KEY_S,           KEY_W,           KEY_3,              KEY_F1,         // COL  2
  NA,              KEY_X,           KEY_D,           KEY_E,           KEY_4,              KEY_F2,         // COL  3
  NA,              KEY_C,           KEY_F,           KEY_R,           KEY_5,              KEY_F3,         // COL  4
  NA,              KEY_V,           KEY_G,           KEY_T,           KEY_6,              KEY_F4,         // COL  5
  NA,              KEY_B,           KEY_H,           KEY_Y,           KEY_7,              KEY_F5,         // COL  6
  KEY_SPACE,       KEY_N,           KEY_J,           KEY_U,           KEY_8,              KEY_F6,         // COL  7
  NA,              KEY_M,           KEY_K,           KEY_I,           KEY_9,              KEY_F7,         // COL  8
  NA,              KEY_COMMA,       KEY_L,           KEY_O,           KEY_0,              KEY_F8,         // COL  9
  KEY_RIGHT_ALT,   KEY_PERIOD,      KEY_SEMICOLON,   KEY_P,           KEY_MINUS,          KEY_F9,         // COL 10
  KEY_RIGHT_GUI,   KEY_SLASH,       KEY_QUOTE,       KEY_LEFT_BRACE,  KEY_EQUAL,          KEY_F10,        // COL 11
  KEY_APPLICATION, NA,              KEY_BACKSLASH,   KEY_RIGHT_BRACE, NA,                 KEY_F11,        // COL 12
  KEY_RIGHT_CTRL,  KEY_RIGHT_SHIFT, KEY_ENTER,       KEY_BACKSLASH,   KEY_BACKSPACE,      KEY_F12,        // COL 13

  KEY_LEFT,        NA,              NA,              KEY_DELETE,      KEY_INSERT,         KEY_PRINTSCREEN,// COL 14
  KEY_DOWN,        KEY_UP,          NA,              KEY_END,         KEY_HOME,           KEY_SCROLL_LOCK,// COL 15
  KEY_RIGHT,       NA,              NA,              KEY_PAGE_DOWN,   KEY_PAGE_UP,        KEY_PAUSE,      // COL 16
};

const uint16_t layout5[NKEY] = {
//ROW 0            ROW 1            ROW 2            ROW 3            ROW 4
  KEY_LEFT_CTRL,   KEY_LEFT_SHIFT,  KEY_CAPS_LOCK,   KEY_TAB,         KEY_1,              KEY_ESC,        // COL  0
  KEY_LEFT_GUI,    KEY_PIPE,        KEY_A,           KEY_Q,           KEY_2,              KEY_TILDE,      // COL  1
  KEY_LEFT_ALT,    KEY_Z,           KEY_S,           KEY_W,           KEY_3,              KEY_F1,         // COL  2
  NA,              KEY_X,           KEY_D,           KEY_E,           KEY_4,              KEY_F2,         // COL  3
  NA,              KEY_C,           KEY_F,           KEY_R,           KEY_5,              KEY_F3,         // COL  4
  NA,              KEY_V,           KEY_G,           KEY_T,           KEY_6,              KEY_F4,         // COL  5
  NA,              KEY_B,           KEY_H,           KEY_Y,           KEY_7,              KEY_F5,         // COL  6
  KEY_SPACE,       KEY_N,           KEY_J,           KEY_U,           KEY_8,              KEY_F6,         // COL  7
  NA,              KEY_M,           KEY_K,           KEY_I,           KEY_9,              KEY_F7,         // COL  8
  NA,              KEY_COMMA,       KEY_L,           KEY_O,           KEY_0,              KEY_F8,         // COL  9
  KEY_RIGHT_ALT,   KEY_PERIOD,      KEY_SEMICOLON,   KEY_P,           KEY_MINUS,          KEY_F9,         // COL 10
  KEY_RIGHT_GUI,   KEY_SLASH,       KEY_QUOTE,       KEY_LEFT_BRACE,  KEY_EQUAL,          KEY_F10,        // COL 11
  KEY_APPLICATION, NA,              KEY_BACKSLASH,   KEY_RIGHT_BRACE, NA,                 KEY_F11,        // COL 12
  KEY_RIGHT_CTRL,  KEY_RIGHT_SHIFT, KEY_ENTER,       KEY_BACKSLASH,   KEY_BACKSPACE,      KEY_F12,        // COL 13

  KEY_LEFT,        NA,              NA,              KEY_DELETE,      KEY_INSERT,         KEY_PRINTSCREEN,// COL 14
  KEY_DOWN,        KEY_UP,          NA,              KEY_END,         KEY_HOME,           KEY_SCROLL_LOCK,// COL 15
  KEY_RIGHT,       NA,              NA,              KEY_PAGE_DOWN,   KEY_PAGE_UP,        KEY_PAUSE,      // COL 16
};

const uint16_t layout6[NKEY] = {
//ROW 0            ROW 1            ROW 2            ROW 3            ROW 4
  KEY_LEFT_CTRL,   KEY_LEFT_SHIFT,  KEY_CAPS_LOCK,   KEY_TAB,         KEY_1,              KEY_ESC,        // COL  0
  KEY_LEFT_GUI,    KEY_PIPE,        KEY_A,           KEY_Q,           KEY_2,              KEY_TILDE,      // COL  1
  KEY_LEFT_ALT,    KEY_Z,           KEY_S,           KEY_W,           KEY_3,              KEY_F1,         // COL  2
  NA,              KEY_X,           KEY_D,           KEY_E,           KEY_4,              KEY_F2,         // COL  3
  NA,              KEY_C,           KEY_F,           KEY_R,           KEY_5,              KEY_F3,         // COL  4
  NA,              KEY_V,           KEY_G,           KEY_T,           KEY_6,              KEY_F4,         // COL  5
  NA,              KEY_B,           KEY_H,           KEY_Y,           KEY_7,              KEY_F5,         // COL  6
  KEY_SPACE,       KEY_N,           KEY_J,           KEY_U,           KEY_8,              KEY_F6,         // COL  7
  NA,              KEY_M,           KEY_K,           KEY_I,           KEY_9,              KEY_F7,         // COL  8
  NA,              KEY_COMMA,       KEY_L,           KEY_O,           KEY_0,              KEY_F8,         // COL  9
  KEY_RIGHT_ALT,   KEY_PERIOD,      KEY_SEMICOLON,   KEY_P,           KEY_MINUS,          KEY_F9,         // COL 10
  KEY_RIGHT_GUI,   KEY_SLASH,       KEY_QUOTE,       KEY_LEFT_BRACE,  KEY_EQUAL,          KEY_F10,        // COL 11
  KEY_APPLICATION, NA,              KEY_BACKSLASH,   KEY_RIGHT_BRACE, NA,                 KEY_F11,        // COL 12
  KEY_RIGHT_CTRL,  KEY_RIGHT_SHIFT, KEY_ENTER,       KEY_BACKSLASH,   KEY_BACKSPACE,      KEY_F12,        // COL 13

  KEY_LEFT,        NA,              NA,              KEY_DELETE,      KEY_INSERT,         KEY_PRINTSCREEN,// COL 14
  KEY_DOWN,        KEY_UP,          NA,              KEY_END,         KEY_HOME,           KEY_SCROLL_LOCK,// COL 15
  KEY_RIGHT,       NA,              NA,              KEY_PAGE_DOWN,   KEY_PAGE_UP,        KEY_PAUSE,      // COL 16
};

/* Holds pointers to all of the layout arrays*/
uint16_t *const layout_ar[NFUN] = { layout0, layout1, layout2, layout3, layout4, layout5, layout6 };

/* Specifies the ports and pin numbers for the rows */
uint8_t *const  row_ddr[NROW] = { _DDRB,  _DDRB,  _DDRB,  _DDRB,  _DDRB,  _DDRB};
uint8_t *const row_port[NROW] = {_PORTB, _PORTB, _PORTB, _PORTB, _PORTB, _PORTB};
uint8_t *const  row_pin[NROW] = { _PINB,  _PINB,  _PINB,  _PINB,  _PINB,  _PINB};
const uint8_t   row_bit[NROW] = { _BIT0,  _BIT1,  _BIT2,  _BIT3,  _BIT4,  _BIT5};

/* Specifies the ports and pin numbers for the columns */
uint8_t *const  col_ddr[NCOL] = { _DDRD,  _DDRC,  _DDRC,  _DDRD,  _DDRD,  _DDRE,
				  _DDRF,  _DDRF,  _DDRF,  _DDRF,  _DDRF,  _DDRF,
				  _DDRD,  _DDRD,  _DDRD,  _DDRD,  _DDRD};
uint8_t *const col_port[NCOL] = {_PORTD, _PORTC, _PORTC, _PORTD, _PORTD, _PORTE,
				 _PORTF, _PORTF, _PORTF, _PORTF, _PORTF, _PORTF,
				 _PORTD, _PORTD, _PORTD, _PORTD, _PORTD};
const uint8_t   col_bit[NCOL] = { _BIT5,  _BIT7,  _BIT6,  _BIT4,  _BIT0,  _BIT6,
				  _BIT0,  _BIT1,  _BIT4,  _BIT5,  _BIT6,  _BIT7,
				  _BIT7,  _BIT6,  _BIT1,  _BIT2,  _BIT3};

inline void update_leds(void) {
    PORTB = (PORTB & 0x3F) | ((keyboard_leds << leds[1]) & LED_B) | ((keyboard_leds << leds[0] >> 1) & LED_A);
    //DDRB  = (DDRB  & 0x3F) | ((keyboard_leds << 5) & 0xC0); //Not Needed at all?
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
  TCCR1A |=      // Timer control register 1A
    (1<<WGM10) | // Fast PWM 8-bit
    (1<<COM1B1)| // Clear OC1B on match, set at TOP
    (1<<COM1C1); // Clear OC1C on match, set at TOP
  TCCR1B |=      // Timer control register 1B
    (1<<WGM12) | // Fast PWM 8-bit
    (1<<CS12);   // Prescaler 256
  OCR1B = 250;    // Output compare register 1B
  OCR1C = 250;    // Output compare register 1C
  // LEDs: LED_A -> PORTB6, LED_B -> PORTB7
  //DDRB  &= 0x3F; //Not needed at all?
  PORTB &= 0x3F;
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
void toggle_leds(void) {DDRB  = PORTB = ~DDRB & 0b11000000;}
