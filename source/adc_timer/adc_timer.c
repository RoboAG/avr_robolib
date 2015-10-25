/******************************************************************************
* source/adc_timer/adc_timer.c                                                *
* ============================                                                *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

//**************************<File version>*************************************
#define SYSTEM_ADC_TIMER_VERSION \
    "source/adc_timer/adc_timer.c 27.09.2015 V1.0.0"

//**************************<Included files>***********************************
#include "system/adc_timer.h"

#include "system/adc.h"
#include "system/string.h"

#include <avr/io.h>

//**************************<Variables>****************************************
volatile uint8_t  system_adc_timer_nr;
volatile uint8_t  system_adc_timer_mask;

//**************************<Prototypes>***************************************

//**************************<Renaming>*****************************************

//**************************<Files>********************************************

//**************************[adc_timer_mask_set]******************************* 27.09.2015
void adc_timer_mask_set(uint8_t mask) {

    system_adc_timer_mask = mask;
}

//**************************[adc_timer_mask_get]******************************* 27.09.2015
uint8_t adc_timer_mask_get(void) {

    return system_adc_timer_mask;
}

//**************************[adc_timer_pin_add]******************************** 27.09.2015
void adc_timer_pin_add(uint8_t nr) {

    uint8_t mSREG = SREG;
    if (nr > 7) { return;}

    cli();
    system_adc_timer_mask|= 1 << nr;
    SREG = mSREG;
}

//**************************[adc_timer_pin_remove]***************************** 27.09.2015
void adc_timer_mask_pin_remove(uint8_t nr) {

    uint8_t mSREG = SREG;
    if (nr > 7) { return;}

    cli();
    system_adc_timer_mask&= ~(1 << nr);
    SREG = mSREG;
}

//**************************[system_adc_timer_print]*************************** 27.09.2015
void system_adc_timer_print(void (*out)(uint8_t)) {

    string_from_const(out, "adc_timer:"                                "\r\n");

    string_from_const_length(out, "  mask" , 15); string_from_const(out, ": ");
    string_from_bits(out, system_adc_timer_mask,-8);
                                                string_from_const(out, "\r\n");

    string_from_const_length(out, "  next" , 15); string_from_const(out, ": ");
    string_from_uint(out, system_adc_timer_nr,1);
                                                string_from_const(out, "\r\n");

    string_from_const(out,                                             "\r\n");
}

//**************************[system_adc_timer_print_compiled]****************** 27.09.2015
void system_adc_timer_print_compiled(void (*out)(uint8_t)) {

    string_from_const(out, SYSTEM_ADC_TIMER_VERSION                    "\r\n");

    string_from_const(out, "  [empty]"                                 "\r\n");

    string_from_const(out,                                             "\r\n");
}

//**************************[system_adc_timer_tick]**************************** 27.09.2015
void system_adc_timer_tick() {

    uint8_t mSREG = SREG;

    cli();
    uint8_t old_nr = system_adc_savevalue();
    if (old_nr == 0xFF) {
        SREG = mSREG;
        return;
    }

    uint8_t temp_nr = system_adc_timer_nr;
    temp_nr&= 0x07;

    if (old_nr == temp_nr) {
        temp_nr++;
    }

    uint8_t mask = system_adc_timer_mask;

    uint8_t temp_mask = 1 << temp_nr;
    while ((temp_mask & mask) == 0) {
        temp_nr++;
        temp_mask = temp_mask << 1;
        if (temp_mask == 0x00) {
            temp_mask = 0x01;
            temp_nr = 0;
            while ((temp_mask & mask) == 0) {
                temp_nr++;
                temp_mask = temp_mask << 1;
                if (temp_mask == 0x00) {
                temp_nr = 0;
                SREG = mSREG;
                return;
                }
            }
            break;
        }
    }
    system_adc_timer_nr = temp_nr;
    system_adc_setchannel(temp_nr);
    SREG = mSREG;
}

//**************************[system_adc_timer_init]**************************** 27.09.2015
void system_adc_timer_init() {

    system_adc_timer_nr   = 0;
    system_adc_timer_mask = 0b11111111;
}
