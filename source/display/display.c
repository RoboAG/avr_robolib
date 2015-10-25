/******************************************************************************
* source/display/display.c                                                    *
* ========================                                                    *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

//**************************<File version>*************************************
#define SYSTEM_DISPLAY_VERSION \
    "source/display/display.c 27.09.2015 V1.0.0"

//**************************<Included files>***********************************
#include "system/display.h"

#include "system/assembler.h"
#include "system/string.h"
#include "system/types.h"

#include <avr/io.h>

//**************************<Variables>****************************************
volatile uint8_t system_display_enabled;

//**************************<Prototypes>***************************************
void system_display_pin_e_toggle(void);
void system_display_busy_check(void);
void system_display_cmd(uint8_t data);

//**************************<Files>********************************************

//**************************[system_display_pin_e_toggle]********************** 27.09.2015
void system_display_pin_e_toggle(void){

    delay_us(1); SYSTEM_DISPLAY_PIN_SET_E(1);
    delay_us(1); SYSTEM_DISPLAY_PIN_SET_E(0);
    delay_us(1);
}

//**************************[system_display_busy_check]************************ 27.09.2015
void system_display_busy_check(void){

    uint8_t counter = 100;

    if (system_display_enabled == 0x00) {return;}

    SYSTEM_DISPLAY_PIN_SET_RW(1);
    SYSTEM_DISPLAY_PIN_SET_RS(0);
    SYSTEM_DISPLAY_PIN_SET_E(0);

    SYSTEM_DISPLAY_DATA_SET_0();
    SYSTEM_DISPLAY_DATA_DDR_R();
    SYSTEM_DISPLAY_DDR|= _BV(DISPLAY_PIN_RW) |
      _BV(DISPLAY_PIN_RS) | _BV(DISPLAY_PIN_E );

    if (system_display_enabled >= 5) {
        system_display_enabled = 4;
    } else {
        system_display_enabled--;
    }

    delay_us(1);

    while ((system_display_enabled != 0xFF) && (--counter)) {
        SYSTEM_DISPLAY_PIN_SET_E(1);
        delay_us(1);
        if (SYSTEM_DISPLAY_BUSY_GET() == 0x00) {
            system_display_enabled = 0xFF;
        }
        SYSTEM_DISPLAY_PIN_SET_E(0);
        system_display_pin_e_toggle();
    }

    SYSTEM_DISPLAY_PIN_SET_RW(0);
    if (system_display_enabled == 0x00) {
        SYSTEM_DISPLAY_DDR &= (~_BV(DISPLAY_PIN_RW) &
          ~_BV(DISPLAY_PIN_RS) & ~_BV(DISPLAY_PIN_E ));
    }
}

//**************************[system_display_cmd]******************************* 27.09.2015
void system_display_cmd(uint8_t data){

    system_display_busy_check();
    if (system_display_enabled != 0xFF) {return;};

    SYSTEM_DISPLAY_DATA_DDR_W();

    SYSTEM_DISPLAY_DATA_SET_H(data);
    system_display_pin_e_toggle();
    SYSTEM_DISPLAY_DATA_SET_L(data);
    system_display_pin_e_toggle();
}

//**************************[display_print]************************************ 27.09.2015
void display_print(uint8_t data){

    system_display_busy_check();
    if (system_display_enabled != 0xFF) {return;};

    SYSTEM_DISPLAY_PIN_SET_RS(1);
    SYSTEM_DISPLAY_DATA_DDR_W();

    SYSTEM_DISPLAY_DATA_SET_H(data);
    system_display_pin_e_toggle();
    SYSTEM_DISPLAY_DATA_SET_L(data);
    system_display_pin_e_toggle();
}

//**************************[display_clear]************************************ 27.09.2015
void display_clear(void){

    system_display_cmd(0x01);
}

//**************************[display_home]************************************* 27.09.2015
void display_home(void){

    system_display_cmd(0x02);
}

//**************************[display_cursor]*********************************** 27.09.2015
void display_cursor(uint8_t mode){

    system_display_cmd(0x0C | (mode & 0x03));
}

//**************************[display_gotoxy]*********************************** 27.09.2015
void display_gotoxy(uint8_t x,uint8_t y) {

    x&= 0x0F;
    if (y & 0x01) {x|= 0x40;}
    if (y & 0x02) {x|= 0x10;}
    system_display_cmd(0x80 | x);
}

//**************************[display_setchar]********************************** 27.09.2015
void display_setchar(uint8_t adr, uint8_t *data) {

    uint8_t a;
    system_display_cmd(0x40 | ((adr & 0x07) << 3));
    for (a = 0; a < 8; a++) {
        display_print(*(data++));
    }
}

//**************************[display_enable]*********************************** 27.09.2015
void display_enable(void) {

    system_display_enabled = 0xFF;
}

//**************************[display_disable]********************************** 27.09.2015
void display_disable(void) {

    system_display_enabled = 0x00;
}

//**************************[display_is_enabled]******************************* 27.09.2015
uint8_t display_is_enabled(void) {

    return system_display_enabled == 0xFF;
}

//**************************[system_display_print]***************************** 27.09.2015
void system_display_print(void (*out)(uint8_t)) {

    string_from_const(out, "display:"                                  "\r\n");

    string_from_const_length(out, "  enabled" , 15);
                                                  string_from_const(out, ": ");
    string_from_bool(out, display_is_enabled());string_from_const(out, "\r\n");

    string_from_const(out,                                             "\r\n");
}

//**************************[system_display_print_compiled]******************** 27.09.2015
void system_display_print_compiled(void (*out)(uint8_t)) {

    string_from_const(out, SYSTEM_DISPLAY_VERSION                      "\r\n");

    string_from_const_length(out, "  DISPLAY_PORT"     , 20);
                                                  string_from_const(out, ": ");
    string_from_macro(out, DISPLAY_PORT);       string_from_const(out, "\r\n");

    string_from_const_length(out, "  DISPLAY_PIN_E"    , 20);
                                                  string_from_const(out, ": ");
    string_from_macro(out, DISPLAY_PIN_E    );  string_from_const(out, "\r\n");
    string_from_const_length(out, "  DISPLAY_PIN_RS"   , 20);
                                                  string_from_const(out, ": ");
    string_from_macro(out, DISPLAY_PIN_RS   );  string_from_const(out, "\r\n");
    string_from_const_length(out, "  DISPLAY_PIN_RW"   , 20);
                                                  string_from_const(out, ": ");
    string_from_macro(out, DISPLAY_PIN_RW   );  string_from_const(out, "\r\n");

    string_from_const_length(out, "  DISPLAY_DATA_PORT", 20);
                                                  string_from_const(out, ": ");
    string_from_macro(out, DISPLAY_DATA_PORT);  string_from_const(out, "\r\n");

    string_from_const_length(out, "  DISPLAY_DATA_HI"  , 20);
                                                  string_from_const(out, ": ");
    #ifdef DISPLAY_DATA_HI
        string_from_macro(out, DISPLAY_DATA_HI);string_from_const(out, "\r\n");
    #else
        string_from_const(out, "[undefined]");  string_from_const(out, "\r\n");
    #endif

    string_from_const(out,                                             "\r\n");
}

//**************************[system_display_init]****************************** 27.09.2015
void system_display_init(void) {

    system_display_enabled = 0xFF;

    SYSTEM_DISPLAY_PIN_SET_RW(0);
    SYSTEM_DISPLAY_PIN_SET_RS(0);
    SYSTEM_DISPLAY_PIN_SET_E(0);
    SYSTEM_DISPLAY_DATA_SET_0();

    SYSTEM_DISPLAY_DDR    |= _BV(DISPLAY_PIN_RW) |
                            _BV(DISPLAY_PIN_RS) |
                            _BV(DISPLAY_PIN_E );

    SYSTEM_DISPLAY_DATA_DDR_W();
    delay_ms(15);

    SYSTEM_DISPLAY_DATA_SET_L(0x03);
    system_display_pin_e_toggle();
    delay_ms(5);

    system_display_pin_e_toggle();
    delay_us(150);

    system_display_pin_e_toggle();

    SYSTEM_DISPLAY_PIN_SET_RW(0);
    SYSTEM_DISPLAY_DATA_DDR_W();
    SYSTEM_DISPLAY_DATA_SET_L(0x02);
    system_display_pin_e_toggle();

    system_display_cmd(0x28); // System set     : 4-Bit mode; 2 rows; 5x7 dots
    system_display_cmd(0x06); // Entry Mode set : shift cursor to the right
    system_display_cmd(0x0C); // Display on/off : turn on; cursor off; no blinking
    system_display_cmd(0x01); // Display clear
}
