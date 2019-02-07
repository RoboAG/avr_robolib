/******************************************************************************
* delay/delay.c                                                               *
* =============                                                               *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/RoboAG/avr_robolib                                     *
******************************************************************************/

//**************************<File version>*************************************
#define ROBOLIB_DELAY_VERSION \
  "robolib/delay/delay.c 27.09.2015 V1.0.0"

//**************************<Included files>***********************************
#include <robolib/delay.h>

#include <robolib/assembler.h>
#include <robolib/string.h>

#include <avr/io.h>

//**************************<Variables>****************************************

//**************************<Prototypes>***************************************

//**************************<Renaming>*****************************************

//**************************<Files>********************************************

//**************************[delay_ms]***************************************** 27.09.2015
void delay_ms(uint16_t mseconds){

    #if (((F_CPU + 4000) / 8000) > 255)
        uint16_t a;
    #else // if (((F_CPU + 4000) / 8000) > 255)
        uint8_t a;
    #endif // if (((F_CPU + 4000) / 8000) > 255)

    while (mseconds) {
        #if (((F_CPU + 4000) / 8000) > 255)
            for (a = (uint16_t) ((F_CPU + 4000) / 8000); a; a--) {
                nop();
                nop();
                nop();
                nop();
            }
        #else // if (((F_CPU + 4000) / 8000) > 255)
            for (a = (uint8_t) ((F_CPU + 4000) / 8000); a; a--) {
                nop();
                nop();
                nop();
                nop();
                nop();
            }
        #endif // if (((F_CPU + 4000) / 8000) > 255)
        mseconds--;
    }
}

//**************************[loopdelay]**************************************** xx.xx.2007
/* Frome file: /util/delay_basic.h
   Copyright (c) 2002, Marek Michalkiewicz
   Copyright (c) 2007 Joerg Wunsch
*/
__inline__ void loopdelay(uint8_t __count) {
    // waits for (__count * 3 - 1) cylces
    __asm__ volatile (
        "1: dec %0" "\n\t"
        "brne 1b"
        : "=r" (__count)
        : "0" (__count)
    );
}

//**************************[delay_us]***************************************** 27.09.2015
void delay_us(uint16_t useconds){

    // + 7/8 cylces for call and return
    // +   3 cycles for if and correction
    // =  10 cycles to many

    if (useconds >= ((2000000 / (F_CPU + 50000) - 1) / 2)) {
        useconds-= ((2000000 / (F_CPU + 50000) - 1) / 2);
    }

    #if (((F_CPU - 350000) / 300000) > 5)
        while (useconds) {
            // while-schleife dauert 4 Takte
            loopdelay((F_CPU - 350000) / 300000);
            useconds--;
        }
    #else // if (((F_CPU - 350000) / 300000) > 5)
        while (useconds) {
            nop();
            #if ((((F_CPU - 350000) / 100000) & 0x01) == 0x01)
                nop();
            #endif
            #if ((((F_CPU - 350000) / 100000) & 0x02) == 0x02)
                nop(); nop();
            #endif
            #if ((((F_CPU - 350000) / 100000) & 0x04) == 0x04)
                nop(); nop(); nop(); nop();
            #endif
            #if ((((F_CPU - 350000) / 100000) & 0x08) == 0x08)
                nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop();
            #endif
            useconds--;
        }
    #endif
}
//**************************[robolib_delay_print]******************************* 27.09.2015
void robolib_delay_print(void (*out)(uint8_t)) {

    string_from_const(out, "delay:"                                    "\r\n");

    string_from_const(out, "  [empty]"                                 "\r\n");

    string_from_const(out,                                             "\r\n");
}

//**************************[robolib_delay_print_compiled]********************** 27.09.2015
void robolib_delay_print_compiled(void (*out)(uint8_t)) {

    string_from_const(out, ROBOLIB_DELAY_VERSION                        "\r\n");

    string_from_const_length(out, "  F_CPU", 15); string_from_const(out, ": ");
    string_from_macro(out, F_CPU);              string_from_const(out, "\r\n");

    string_from_const(out,                                             "\r\n");
}
