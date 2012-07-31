/* USB Keyboard Firmware code for generic Teensy Keyboards
 * Copyright (c) 2012 Fredrik Atmer, Bathroom Epiphanies Inc
 * http://bathroomepiphanies.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usb_keyboard.h"
#include "teensy.h"
#include __INCLUDE_KEYBOARD

/* pressed   keeps track of which keys that are pressed 
   release   keeps track of which keys that may be released
   queue     contains the keys that are sent in the HID packet 
   mod_keys  is the bit pattern corresponding to pressed modifier keys */
uint8_t pressed[NKEY];
volatile uint8_t release[NKEY];
uint8_t queue[7] = {255,255,255,255,255,255,255};
uint8_t mod_keys = 0;

extern const uint8_t is_modifier[];
extern const uint8_t layout[];
extern uint8_t *const row_ddr[];
extern uint8_t *const row_port[];
extern uint8_t *const row_pin[];
extern const uint8_t row_bit[];
extern uint8_t *const col_ddr[];
extern uint8_t *const col_port[];
extern const uint8_t col_bit[];

void init(void);
void send(void);
void poll(void);
void key_press(uint8_t key);
void key_release(uint8_t key);
/* void update_leds(void); */
/* void setup_leds(void); */
void setup_io_pins(void);
/* void setup_bounce_timer(void); */
/* void toggle_leds(void); */

/* Check for keys ready to be released, and 
   advance the release counter on all keys. */
ISR(INTERRUPT_FUNCTION) {
  uint8_t key;
  for(key = 0; key < NKEY; key++) {
    if(release[key] == 0x01)
      key_release(key);
    release[key] >>= 1;
  }
  update_leds();
}

int main(void) {
  uint8_t row, col, key;
  uint8_t this, previous[NKEY];

  init();
  for(key = 0; key < NROW; key++)
    previous[key] = 0;

  for(;;) {
    // Set one column at a time to low output, 
    // register which rows are pulled down.
    for(col = 0, key = 0; col < NCOL; col++) {
      *col_port[col] &= ~col_bit[col];
      _delay_us(1);

      for(row = 0; row < NROW; row++, key++) {

	this = !(*row_pin[row] & row_bit[row]);
	
	// Detect rising and falling edges on key status
	if(this && !previous[key])
	  if(!pressed[key])
	    key_press(key);
	  else
	    release[key] = 0x00; // Abort release
	else if(previous[key] && !this)
	  release[key] = 0x80; // Initiate release
	
	previous[key] = this;
      }

      *col_port[col] |= col_bit[col];
    }
  }
}

void send(void) {
  //return;
  uint8_t i;
  for(i=0; i<6; i++)
    keyboard_keys[i] = queue[i]<255? layout[queue[i]]: 0;
  keyboard_modifier_keys = mod_keys;
  usb_keyboard_send();
}

/* */
void key_press(uint8_t key) {
  uint8_t i;
  pressed[key] = true;
  release[key] = 0x00;
  if(is_modifier[key])
    mod_keys |= layout[key];
  else {
    for(i = 5; i > 0; i--) queue[i] = queue[i-1];
    queue[0] = key;
  }
  send();
}

/* */
void key_release(uint8_t key) {
  uint8_t i;
  pressed[key] = false;
  release[key] = 0x00;
  if(is_modifier[key])
    mod_keys &= ~layout[key];
  else {
    for(i = 0; i < 6; i++) 
      if(queue[i]==key)
	break;
    for(; i < 6; i++)
      queue[i] = queue[i+1];
  }
  send();
}

/* Call initialization functions */
void init(void) {
  CLKPR = 0x80; CLKPR = 0;
  usb_init();
  setup_io_pins();
  setup_leds();
  setup_bounce_timer();
  while(!usb_configured());
  _delay_ms(1000);
  // Enable interrupts
  sei();
}

/* PORTB is set as input with pull-up resistors
   PORTC,D,E,F are set to high output */
void setup_io_pins(void) {
  uint8_t row, col, key;
  for(row = 0; row < NROW; row++) {
    *row_ddr[row]  &= ~row_bit[row];
    *row_port[row] |=  row_bit[row];
  }
  for(col = 0; col < NCOL; col++) {
    *col_ddr[col]  |= col_bit[col];
    *col_port[col] |= col_bit[col];
  }
  for(key = 0; key < NKEY; key++) {
    pressed[key] = false;
    release[key] = 0x00;
  }
}
