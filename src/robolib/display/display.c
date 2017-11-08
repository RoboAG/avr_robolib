/******************************************************************************
* display/display.c                                                           *
* =================                                                           *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

//**************************<File version>*************************************
#define ROBOLIB_DISPLAY_VERSION \
  "robolib/display/display.c 27.09.2015 V1.0.0"

//**************************<Included files>***********************************
#include <robolib/display.h>

#include <robolib/assembler.h>
#include <robolib/string.h>
#include <robolib/types.h>

#include <avr/io.h>

//**************************<Variables>****************************************
volatile uint8_t robolib_display_enabled;

//**************************<Prototypes>***************************************
void robolib_display_pin_e_toggle(void);
void robolib_display_busy_check(void);
void robolib_display_cmd(uint8_t data);

//**************************<Files>********************************************

//**************************[robolib_display_pin_e_toggle]********************** 27.09.2015
void robolib_display_pin_e_toggle(void){

    delay_us(1); ROBOLIB_DISPLAY_PIN_SET_E(1);
    delay_us(1); ROBOLIB_DISPLAY_PIN_SET_E(0);
    delay_us(1);
}

//**************************[robolib_display_busy_check]************************ 27.09.2015
void robolib_display_busy_check(void){

    uint8_t counter = 100;

    if (robolib_display_enabled == 0x00) {return;}

    ROBOLIB_DISPLAY_PIN_SET_RW(1);
    ROBOLIB_DISPLAY_PIN_SET_RS(0);
    ROBOLIB_DISPLAY_PIN_SET_E(0);

    ROBOLIB_DISPLAY_DATA_SET_0();
    ROBOLIB_DISPLAY_DATA_DDR_R();
    ROBOLIB_DISPLAY_DDR|= _BV(DISPLAY_PIN_RW) |
      _BV(DISPLAY_PIN_RS) | _BV(DISPLAY_PIN_E );

    if (robolib_display_enabled >= 5) {
        robolib_display_enabled = 4;
    } else {
        robolib_display_enabled--;
    }

    delay_us(1);

    while ((robolib_display_enabled != 0xFF) && (--counter)) {
        ROBOLIB_DISPLAY_PIN_SET_E(1);
        delay_us(1);
        if (ROBOLIB_DISPLAY_BUSY_GET() == 0x00) {
            robolib_display_enabled = 0xFF;
        }
        ROBOLIB_DISPLAY_PIN_SET_E(0);
        robolib_display_pin_e_toggle();
    }

    ROBOLIB_DISPLAY_PIN_SET_RW(0);
    if (robolib_display_enabled == 0x00) {
        ROBOLIB_DISPLAY_DDR &= (~_BV(DISPLAY_PIN_RW) &
          ~_BV(DISPLAY_PIN_RS) & ~_BV(DISPLAY_PIN_E ));
    }
}

//**************************[robolib_display_cmd]******************************* 27.09.2015
void robolib_display_cmd(uint8_t data){

    robolib_display_busy_check();
    if (robolib_display_enabled != 0xFF) {return;};

    ROBOLIB_DISPLAY_DATA_DDR_W();

    ROBOLIB_DISPLAY_DATA_SET_H(data);
    robolib_display_pin_e_toggle();
    ROBOLIB_DISPLAY_DATA_SET_L(data);
    robolib_display_pin_e_toggle();
}

//**************************[display_print]************************************ 27.09.2015
void display_print(uint8_t data){

    robolib_display_busy_check();
    if (robolib_display_enabled != 0xFF) {return;};

    ROBOLIB_DISPLAY_PIN_SET_RS(1);
    ROBOLIB_DISPLAY_DATA_DDR_W();

    ROBOLIB_DISPLAY_DATA_SET_H(data);
    robolib_display_pin_e_toggle();
    ROBOLIB_DISPLAY_DATA_SET_L(data);
    robolib_display_pin_e_toggle();
}

//**************************[display_clear]************************************ 27.09.2015
void display_clear(void){

    robolib_display_cmd(0x01);
}

//**************************[display_home]************************************* 27.09.2015
void display_home(void){

    robolib_display_cmd(0x02);
}

//**************************[display_cursor]*********************************** 27.09.2015
void display_cursor(uint8_t mode){

    robolib_display_cmd(0x0C | (mode & 0x03));
}

//**************************[display_gotoxy]*********************************** 27.09.2015
void display_gotoxy(uint8_t x,uint8_t y) {

    x&= 0x0F;
    if (y & 0x01) {x|= 0x40;}
    if (y & 0x02) {x|= 0x10;}
    robolib_display_cmd(0x80 | x);
}

//**************************[display_setchar]********************************** 27.09.2015
void display_setchar(uint8_t adr, uint8_t *data) {

    uint8_t a;
    robolib_display_cmd(0x40 | ((adr & 0x07) << 3));
    for (a = 0; a < 8; a++) {
        display_print(*(data++));
    }
}

//**************************[display_enable]*********************************** 27.09.2015
void display_enable(void) {

    robolib_display_enabled = 0xFF;
}

//**************************[display_disable]********************************** 27.09.2015
void display_disable(void) {

    robolib_display_enabled = 0x00;
}

//**************************[display_is_enabled]******************************* 27.09.2015
uint8_t display_is_enabled(void) {

    return robolib_display_enabled == 0xFF;
}

//**************************[robolib_display_print]***************************** 27.09.2015
void robolib_display_print(void (*out)(uint8_t)) {

    string_from_const(out, "display:"                                  "\r\n");

    string_from_const_length(out, "  enabled" , 15);
                                                  string_from_const(out, ": ");
    string_from_bool(out, display_is_enabled());string_from_const(out, "\r\n");

    string_from_const(out,                                             "\r\n");
}

//**************************[robolib_display_print_compiled]******************** 27.09.2015
void robolib_display_print_compiled(void (*out)(uint8_t)) {

    string_from_const(out, ROBOLIB_DISPLAY_VERSION                      "\r\n");

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

//**************************[robolib_display_init]****************************** 27.09.2015
void robolib_display_init(void) {

    robolib_display_enabled = 0xFF;

    ROBOLIB_DISPLAY_PIN_SET_RW(0);
    ROBOLIB_DISPLAY_PIN_SET_RS(0);
    ROBOLIB_DISPLAY_PIN_SET_E(0);
    ROBOLIB_DISPLAY_DATA_SET_0();

    ROBOLIB_DISPLAY_DDR    |= _BV(DISPLAY_PIN_RW) |
                            _BV(DISPLAY_PIN_RS) |
                            _BV(DISPLAY_PIN_E );

    ROBOLIB_DISPLAY_DATA_DDR_W();
    delay_ms(15);

    ROBOLIB_DISPLAY_DATA_SET_L(0x03);
    robolib_display_pin_e_toggle();
    delay_ms(5);

    robolib_display_pin_e_toggle();
    delay_us(150);

    robolib_display_pin_e_toggle();

    ROBOLIB_DISPLAY_PIN_SET_RW(0);
    ROBOLIB_DISPLAY_DATA_DDR_W();
    ROBOLIB_DISPLAY_DATA_SET_L(0x02);
    robolib_display_pin_e_toggle();

    robolib_display_cmd(0x28); // System set     : 4-Bit mode; 2 rows; 5x7 dots
    robolib_display_cmd(0x06); // Entry Mode set : shift cursor to the right
    robolib_display_cmd(0x0C); // Display on/off : turn on; cursor off; no blinking
    robolib_display_cmd(0x01); // Display clear
}
