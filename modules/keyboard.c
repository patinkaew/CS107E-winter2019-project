/*
 * Keyboard driver
 * This program manages input from keyboard.
 * File: keyboard.c
 * Written by Patin Inkaew (Feb 2019)
 * For CS 107E Winter 2019
 */

#include "gpio.h"
#include "gpioextra.h"
#include "keyboard.h"
#include "ps2.h"
#include "timer.h"
#include "interrupts.h"
#include "ringbuffer.h"
#include <stdbool.h>

static unsigned int CLK, DATA;
static unsigned int time_out = 3000; //microseconds
static bool use_interrupts = false;

//initiate keyboard
void keyboard_init(unsigned int clock_gpio, unsigned int data_gpio){
    CLK = clock_gpio;
    gpio_set_input(CLK);
    gpio_set_pullup(CLK);

    DATA = data_gpio;
    gpio_set_input(DATA);
    gpio_set_pullup(DATA);
}

/* functions for reading scancode when not using interrupts */

/*
 * Wait for clock line to read LOW (0)
 */
static int wait_for_falling_clock_edge(void) {
    unsigned int stamp = timer_get_ticks();
    while (gpio_read(CLK) == 0) {
        if(timer_get_ticks() - stamp > time_out){ return -1; } //time out
    }
    stamp = timer_get_ticks();
    while (gpio_read(CLK) == 1) {
        if(timer_get_ticks() - stamp > time_out){ return -1; } //time_out
    }
    return 0;
}

/*
 * Read scancode when not using interrupts
 * Collecting 11-bit package from keyboard to contruct a scancode
 * This function will be called when read scancode is called and not using interrupts
 */
static unsigned char keyboard_read_raw_without_interrupts(void){
    while(1){
        unsigned char result = 0b0;
        if(wait_for_falling_clock_edge() == -1) { return 0; }; //time out
        if(gpio_read(DATA) != 0) { continue; }; //wrong start bit
        int one_count = 0;
        for(int i = 0; i < 8; i++){
            if(wait_for_falling_clock_edge() == -1) { return 0; }; //time out
            unsigned char bit = gpio_read(DATA);
            if(bit == 1) one_count++; //parity check
            bit = bit << i; //data is LSB first, need to do some bitwise operator
            result = result | bit;
        }
        if(wait_for_falling_clock_edge() == -1) { return 0; }; //time out
        if(gpio_read(DATA) == 1) one_count++; //now read parity
        if(one_count % 2 == 0){ continue; } //wrong parity check
        if(wait_for_falling_clock_edge() == -1) { return 0; }; //time out
        if(gpio_read(DATA) != 1) { continue; } //wronf stop bit
        return result;
    }
}

/* functions for reading scancode when using interrupts */
static rb_t *scancode_queue; //ringbuffer storing scancode
/*
 * Read raw scancode function wheb using interrupts
 * This function has similar functionality to read raw scancode when not using interrupts
 * This function is a interrupt handler for interrupt_vector table
.*/
static bool keyboard_read_raw_with_interrupts(unsigned int pc){
    static int bit_index = 0;
    static int parity_check = 0;
    static unsigned char result = 0;

    if(gpio_check_and_clear_event(CLK)){
        switch(bit_index){
            case 0:{ //start bit
                if(gpio_read(DATA) == 0) bit_index++; //correct start bit
                break;
            }

            case 9:{ //parity bit
                if(gpio_read(DATA) == 1) parity_check++;
                bit_index++;
                if(parity_check % 2 == 0){ //incorrect parity check
                    bit_index = 0;
                    parity_check = 0;
                    result = 0;
                }
                break;
            }

            case 10:{ //stop bit
                if(gpio_read(DATA) == 1) rb_enqueue(scancode_queue, result); //correct stop bit

                //reset to wait for next scancode
                bit_index = 0;
                parity_check = 0;
                result = 0;
                break;
            }

            default:{ //data bit
                unsigned char bit = gpio_read(DATA);
                if(bit == 1) parity_check++; //parity check
                result |= (bit << (bit_index - 1)); //data is LSB first, do some bitwise operations
                bit_index++;
                break;
            }
        }
        return true;
    }
    return false;
}

/*
 * Enable reading scancode through interrupts
 * need to call this function if want to use interrupts
 */
void keyboard_use_interrupts(void){
    use_interrupts = true;

    //initilize interrupts
    gpio_enable_event_detection(CLK, GPIO_DETECT_FALLING_EDGE);
    interrupts_attach_handler(keyboard_read_raw_with_interrupts);
    interrupts_enable_source(INTERRUPTS_GPIO3);

    //initilize ringbuffer
    scancode_queue = rb_new();
}

// public read scancode function

bool keyboard_has_char(){
    return !rb_empty(scancode_queue);
}

unsigned char keyboard_read_scancode(void){
    if(use_interrupts){ //interrupt driven read scancode is enabled
        while(1){
            int scancode = 0;
            if(rb_dequeue(scancode_queue, &scancode)){
                return (unsigned char)scancode;
            }
        }
    }else{ //interrupts driven read scancode is disable, use normal function call
        while(1){
            unsigned char scancode = keyboard_read_raw_without_interrupts();
            if(scancode != 0){ //if not timeout, return scancode
                return scancode;
            }
        }
    }
}

//read sequence and return key action
key_action_t keyboard_read_sequence(void){
    key_action_t action;
    unsigned char scancode = keyboard_read_scancode();
    if(scancode == 0xe0){ //extra scancode
        scancode = keyboard_read_scancode(); //read again
    }
    if(scancode == 0xf0){ //release scancode
        action.what = KEY_RELEASE;
        action.keycode = keyboard_read_scancode();
    }else{ //press scancode
        action.what = KEY_PRESS;
        action.keycode = scancode;
    }
    return action;
}

/*
 * Return keyboard modifier corresponding to given character
 * ch is input character
 * return keyboard modifier mask
 */
keyboard_modifiers_t get_modifier_mask(unsigned char ch){
    switch(ch){
        case PS2_KEY_SCROLL_LOCK: return KEYBOARD_MOD_SCROLL_LOCK;
        case PS2_KEY_NUM_LOCK: return KEYBOARD_MOD_NUM_LOCK;
        case PS2_KEY_CAPS_LOCK: return KEYBOARD_MOD_CAPS_LOCK;
        case PS2_KEY_SHIFT: return KEYBOARD_MOD_SHIFT;
        case PS2_KEY_ALT: return KEYBOARD_MOD_ALT;
        case PS2_KEY_CTRL: return KEYBOARD_MOD_CTRL;
    }
    return 0;
}

/*
 * Hold and process global modifers (combination of current applied modifiers)
 * action is a key action used to dintiguish press and release
 * key is a ps2 key which hold ASCII key code
 * return current global modifiers
 */
keyboard_modifiers_t get_modifiers(key_action_t action, ps2_key_t key){
    static keyboard_modifiers_t global_modifiers = 0b0;
    keyboard_modifiers_t mask = get_modifier_mask(key.ch);
    if(key.ch == PS2_KEY_SCROLL_LOCK || key.ch == PS2_KEY_NUM_LOCK
                                     || key.ch == PS2_KEY_CAPS_LOCK){ //toggle keys
        if(action.what == KEY_PRESS){
            if((global_modifiers & mask) != 0){ //scroll/num/caps is currently on
                global_modifiers = global_modifiers & ~mask; //turn off
            }else{ //scroll/num/caps is currently off
                global_modifiers = global_modifiers | mask; //turn on
            }
        }
    }else{ //non toggle keys
        keyboard_modifiers_t mask = get_modifier_mask(key.ch);
        if(action.what == KEY_PRESS){ //apply this key modifier
            global_modifiers = global_modifiers | mask;
        }else{ //action.what == KEY_RELEASE //stop this key modifier
            global_modifiers = global_modifiers & ~mask;
        }
    }
    return global_modifiers;
}

//process key sequence and return key event
key_event_t keyboard_read_event(void){
    key_action_t action = keyboard_read_sequence();
    ps2_key_t key = ps2_keys[action.keycode]; //get corresponding ps2 key
    keyboard_modifiers_t modifiers = get_modifiers(action, key); //get modifiers
    key_event_t event = {action, key, modifiers}; //make processed info into key event
    return event;
}

static bool is_numeric_pad(ps2_key_t key){
    if(('0' <= key.other_ch && key.other_ch <= '9') || key.other_ch == '.'){
        return true;
    }
    return false;
}

//process key event and return suitable ASCII character
unsigned char keyboard_read_next(void){
    while(1){
        key_event_t event = keyboard_read_event();
        if(event.action.what == KEY_PRESS){ //key is pressed
            unsigned char ch = event.key.ch;
            if(ch == PS2_KEY_SCROLL_LOCK || ch == PS2_KEY_NUM_LOCK ||
                ch == PS2_KEY_CAPS_LOCK || ch == PS2_KEY_SHIFT ||
                ch == PS2_KEY_ALT || ch == PS2_KEY_CTRL){ //modifiers
                continue;
            }else{ //not a modifiers
                if(event.key.other_ch != 0 /* do not have key other */
                    && !is_numeric_pad(event.key) /*key changed when numlock is presence */
                    && ((event.modifiers & KEYBOARD_MOD_CAPS_LOCK) != 0
                    || (event.modifiers & KEYBOARD_MOD_SHIFT) != 0)){ //shift/caps is presence
                    return event.key.other_ch;
                }else{
                    if(event.action.keycode == 0x66){ //a backspace
                            return '\b';
                    }
                    return event.key.ch;
                }
            }
        }
    }
}
