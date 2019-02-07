/******************************************************************************
* examples/bootloader.c                                                       *
* =====================                                                       *
*                                                                             *
* Version: 1.0.2                                                              *
* Date   : 23.10.15                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/RoboAG/avr_robolib                                     *
******************************************************************************/

// include from gcc
#include <inttypes.h>
#include <avr/io.h>

// include all necessary headers from robolib
#include "bootloader.h"

//**************************[bootloader_start]*********************************
uint8_t bootloader_start(void) {

    // check boot pin
    #if ( defined (__AVR_ATmega2561__) || \
      defined (__AVR_ATmega64__))
        DDRG  = 0x00;
        PORTG = _BV(2);
        delay_ms(50);
        if (PING & _BV(2)) {
            PORTG = 0x00;
            return 0x00;
        }
        PORTG = 0x00;
    #else
        DDRB  = 0x00;
        PORTB = _BV(2);
        delay_ms(50);
        if (PINB & _BV(2)) {
            PORTB = 0x00;
            return 0x00;
        }
        PORTB = 0x00;
    #endif

    uart0_send('~');

    string_from_const(display_print, "Bootloader  V4.0");
    display_gotoxy(0,1);
    string_from_const(display_print, "P.W.  23.10.2015");

    return 0xFF;
}

//**************************[bootloader_end]***********************************
void bootloader_end(void) {

    display_clear();

    uart0_flush();
    delay_ms(10);
    uart0_disable();
}

//**************************[bootloader_data_in]*******************************
uint8_t bootloader_data_in(void) {

    return uart0_get();
}

//**************************[bootloader_data_stat]*****************************
uint8_t bootloader_data_stat(void) {

    return uart0_rxcount_get();
}

//**************************[bootloader_data_out]******************************
void bootloader_data_out(uint8_t data) {
    uart0_send(data);
}

//**************************[bootloader_idle]**********************************
uint8_t bootloader_idle(void) {

    return 0xFF;
}

//**************************[bootloader_command]*******************************
void bootloader_command(void) {
}

//**************************[bootloader_help]**********************************
void bootloader_help(void) {
}

//**************************[bootloader_error]*********************************
uint8_t bootloader_error(void) {

    return 0xFF;
}


/******************************************************************************
*                                                                             *
* How to compile this example                                                 *
* ===========================                                                 *
*                                                                             *
* required files                                                              *
*   + bootloader.c          (source code, this file)                          *
*   + bootloader_header.ini (headerfile as '.ini')                            *
*   + Makefile              (needed for compiling, linking and downloading)   *
*                                                                             *
* 1. adjust Makefile                              (only needed once)          *
*   + set "MCU" to the controller type              (e.g. atmega64)           *
*   + set "PATH_LIB" to the library                 (e.g. ~/avr/robolib)      *
*   + set "BOOTSTART" to boot adress                (e.g. 0xE000)             *
*                                                                             *
* 2. compile library and create headerfile        (only needed once)          *
*   + open a console                                (e.g. strg+alt+t)         *
*   + change to the directory of the files          (e.g. cd ~/avr/test/)     *
*   + run make with "ini" as parameter              (e.g. make ini)           *
*     ==> check output for errors                                             *
*                                                                             *
* 3. compile main file and link everything        (needed for all changes)    *
*   + open a console                                (e.g. strg+alt+t)         *
*   + change to the directory of the files          (e.g. cd ~/avr/test/)     *
*   + run make with "all" as parameter              (e.g. make all)           *
*     ==> check output for errors                                             *
*                                                                             *
* How to download this example                                                *
* ============================                                                *
*                                                                             *
* required files                                                              *
*   + out.hex          (machine code, created in the steps above)             *
*   + Makefile         (needed for compiling, linking and downloading)        *
*                                                                             *
* 1. adjust Makefile                              (only needed once)          *
*   + set "COMPORT_AVRDUDE" to the serial device    (e.g. /dev/ttyACM0)       *
*   + set "BAUDRATE_AVRDUDE" to the speed           (e.g. 4µs)                *
*                                                                             *
* 2. download result                              (needed for all changes)    *
*   + open a console                                (e.g. strg+alt+t)         *
*   + change to the directory of the files          (e.g. cd ~/avr/test/)     *
*   + run make with "avrdude" as parameter          (e.g. make avrdude)       *
*     ==> check output for errors                                             *
*                                                                             *
******************************************************************************/
