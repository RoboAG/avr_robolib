/******************************************************************************
* bootloader.c                                                                *
* ============                                                                *
*                                                                             *
* Version: 1.1.2                                                              *
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
#include "keplerboard.h"

#define UART_SELECT_RS232 0x01
#define UART_SELECT_XBEE  0x02
uint8_t uart_select;

#define GET_BOOT_PIN()     (PING & _BV(2))
#define ENABLE_BOOT_PIN()  do {DDRG  = 0x00; PORTG = _BV(2); } while (0)
#define DISABLE_BOOT_PIN() do {DDRG  = 0x00; PORTG =   0x00; } while (0)
uint8_t boot_pin;

//**************************[bootloader_start]*********************************
uint8_t bootloader_start(void) {

    // check boot pin
    ENABLE_BOOT_PIN();

    delay_ms(50);
    if (GET_BOOT_PIN()) {
        return 0x00;
    }

    // set variables
    boot_pin = 0x00;
    uart_select = UART_SELECT_XBEE;

    // print information
    bootloader_data_out('~');

    string_from_const(display_print, "Bootloader  V4.0");
    display_gotoxy(0,1);
    string_from_const(display_print, "[XBEE ] 27.11.17");

    return 0xFF;
}

//**************************[bootloader_end]***********************************
void bootloader_end(void) {

    DISABLE_BOOT_PIN();

    display_clear();

    uart0_flush();
    uart1_flush();
    delay_ms(10);
    uart0_disable();
    uart1_disable();
}

//**************************[bootloader_data_in]*******************************
uint8_t bootloader_data_in(void) {

    while (1) {

        if (uart_select & UART_SELECT_RS232) {
            if (uart0_rxcount_get()) {
                return uart0_get();
            }
        }

        if (uart_select & UART_SELECT_XBEE) {
            if (uart1_rxcount_get()) {
                return uart1_get();
            }
        }
    }
}

//**************************[bootloader_data_stat]*****************************
uint8_t bootloader_data_stat(void) {

    uint16_t count;
    count = 0;
    if (uart_select & UART_SELECT_RS232) {
        count+= uart0_rxcount_get();
    }

    if (uart_select & UART_SELECT_XBEE) {
        count+= uart1_rxcount_get();
    }

    if (count > 255) {
        return 255;
    } else {
        return count;
    }
}

//**************************[bootloader_data_out]******************************
void bootloader_data_out(uint8_t data) {

    if (uart_select & UART_SELECT_RS232) {
        uart0_send(data);
    }
    if (uart_select & UART_SELECT_XBEE) {
        uart1_send(data);
    }
}

//**************************[bootloader_idle]**********************************
uint8_t bootloader_idle(void) {

    uint8_t pin = GET_BOOT_PIN();

    if (pin != boot_pin) {
        boot_pin = pin;

        if (pin == 0x00) {
            bootloader_data_out('@');

            display_gotoxy(1,1);
            if (uart_select != UART_SELECT_XBEE) {
                uart_select = UART_SELECT_XBEE;
                string_from_const(display_print, "XBEE ");
            } else {
                uart_select = UART_SELECT_RS232;
                string_from_const(display_print, "RS232");
            }

            uart0_flush();
            uart1_flush();
            bootloader_data_out('~');
        }
    }

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
