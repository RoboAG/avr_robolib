/******************************************************************************
* bootloader.c                                                                *
* ============                                                                *
*                                                                             *
* Version: 1.1.1                                                              *
* Date   : 27.11.17                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

// include from gcc
#include <inttypes.h>
#include <avr/io.h>

// include all necessary headers from system
#include "patchbox.h"

#define GET_BOOT_PIN()     (PING & _BV(2))

//**************************[init_hardware]************************************
void init_hardware(void){
    DDRA  = 0x00; // Display
    PORTA = 0x00; // Display
    DDRB  = 0x2F; // Leds (heartbeat, error, switch, ...)
    PORTB = 0x0F; // Leds (heartbeat, error, switch, ...)
    DDRC  = 0x00; // Switches
    PORTC = 0x00; // Switches
    DDRD  = 0x10; // EEPROM + DMX
    PORTD = 0x03; // EEPROM + DMX
    DDRE  = 0x00; // UART0
    PORTE = 0x00; // UART0
    DDRF  = 0x00; // Fader
    PORTF = 0x00; // Fader

    DDRG  = 0x00; // Boot pin
    PORTG = 0x04;
}

//**************************[clear_hardware]***********************************
void clear_hardware(void){
    DDRA  = 0x00; // Display
    PORTA = 0x00; // Display
    DDRB  = 0x00; // Leds (heartbeat, error, switch, ...)
    PORTB = 0x00; // Leds (heartbeat, error, switch, ...)
    DDRC  = 0x00; // Switches
    PORTC = 0x00; // Switches
    DDRD  = 0x00; // EEPROM + DMX
    PORTD = 0x00; // EEPROM + DMX
    DDRE  = 0x00; // UART0
    PORTE = 0x00; // UART0
    DDRF  = 0x00; // Fader
    PORTF = 0x00; // Fader

    DDRG  = 0x00; // Boot pin
    PORTG = 0x00;
}

//**************************[bootloader_start]*********************************
uint8_t bootloader_start(void) {

    init_hardware();

    delay_ms(50);
    if (GET_BOOT_PIN()) {
        return 0x00;
    }

    // print information
    bootloader_data_out('~');

    delay_ms(200);
    display_clear();
    string_from_const(display_print, "Bootloader  V4.0");
    display_gotoxy(0,1);
    string_from_const(display_print, "Patchbox 11.2017");

    return 0xFF;
}

//**************************[bootloader_end]***********************************
void bootloader_end(void) {

    clear_hardware();

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
