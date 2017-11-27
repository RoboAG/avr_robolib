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
#include "tucbot.h"

#define UART_SELECT_RS232     0
#define UART_SELECT_INTERNAL  1
#define UART_SELECT_XBEE      2
#define UART_SELECT_USB       0xFF
uint8_t uart;

#define BUTTON1() ((PINA & 0x04) == 0x00)
#define BUTTON2() ((PINA & 0x08) == 0x00)
#define BUTTON3() ((PINA & 0x10) == 0x00)

#define LED1(x) (x ? (PORTB&= ~0x10) : (PORTB|=  0x10))
#define LED2(x) (x ? (PORTC&= ~0x08) : (PORTC|=  0x08))
#define LED3(x) (x ? (PORTC&= ~0x04) : (PORTC|=  0x04))

uint8_t count = 0;

//**************************[uart_select]**************************************
void uart_select(uint8_t uart) {

    if (uart < 3) {
        PORTA = (PORTA & 0xFC) | uart;
    }
}

//**************************[init_hardware]************************************
void init_hardware(void){

    // Ports
    DDRA = 0x03;
    PORTA = 0x00;
      // 0-1: UART_Select 1,2
      // 2-4: Button 1,2,3; 5-7: Sharp 1,2,3
    DDRB = 0x18;
    PORTB = 0x10;
      // 0-2: Display (RS, R/W, Enable)
      //   3: Beeper; 4: Led 1
      // 5-7: SPI (MOSI, MISO, SCK)
    DDRC = 0x0C;
    PORTC = 0x0C;
      // 0-1: TWI (SCL, SDA); 2-3: Led 3,2
      // 4-7: Displaypin 4,5,6,7
    DDRD = 0x00;
    PORTD = 0x00;
      // 0-1: USB (RX,TX); 2-3: UART (RX,TX)
      // 4-6: TSOP 1,2,3;  7: Led Status & Servo
}

//**************************[bootloader_start]*********************************
uint8_t bootloader_start(void) {

    init_hardware();

    delay_ms(100);
    if (BUTTON1()) {
        uart = UART_SELECT_RS232;
        uart0_disable();
    } else if (BUTTON2()) {
        uart = UART_SELECT_XBEE;
        uart0_disable();
    } else if (BUTTON3()) {
        uart = UART_SELECT_USB;
        uart1_disable();
    } else {
        return 0x00;
    }
    uart_select(uart);

    // print information
    bootloader_data_out('~');

    string_from_const(display_print, "Bootloader  V4.0");
    display_gotoxy(0,1);
    string_from_const(display_print, "[???  ] 27.11.17");

    display_gotoxy(1,1);
    switch (uart) {
        case UART_SELECT_RS232:
            string_from_const(display_print, "RS232");
            break;

        case UART_SELECT_XBEE:
            string_from_const(display_print, "XBEE");
            break;

        case UART_SELECT_USB:
            string_from_const(display_print, "USB");
            break;

        default:
            string_from_const(display_print, "error");
            break;
    }

    return 0xFF;
}

//**************************[bootloader_end]***********************************
void bootloader_end(void) {

    display_clear();

    uart0_flush();
    uart1_flush();
    delay_ms(10);
    uart0_disable();
    uart1_disable();
}

//**************************[bootloader_data_in]*******************************
uint8_t bootloader_data_in(void) {

    if (uart == UART_SELECT_USB) {
        return uart0_get();
    } else {
        return uart1_get();
    }
}

//**************************[bootloader_data_stat]*****************************
uint8_t bootloader_data_stat(void) {

    if (uart == UART_SELECT_USB) {
        return uart0_rxcount_get();
    } else {
        return uart1_rxcount_get();
    }
}

//**************************[bootloader_data_out]******************************
void bootloader_data_out(uint8_t data) {

    if (uart == UART_SELECT_USB) {
        uart0_send(data);
    } else {
        uart1_send(data);
    }
}

//**************************[bootloader_idle]**********************************
uint8_t bootloader_idle(void) {

    LED1(0); LED2(0); LED3(0);

    uint8_t count = 0;
    while (count < 50) {
        if (bootloader_data_stat()) { break; }

        delay_ms(1);
        count++;
    }

     switch (uart) {
         case UART_SELECT_RS232: LED1(1); break;
         case UART_SELECT_XBEE : LED2(1); break;
         case UART_SELECT_USB  : LED3(1); break;
     }

    return 0xFF;
}

//**************************[bootloader_help]**********************************
void bootloader_help(void) {
}

//**************************[bootloader_error]*********************************
uint8_t bootloader_error(void) {

    LED1(1); LED2(1); LED3(1);

    return 0xFF;
}
