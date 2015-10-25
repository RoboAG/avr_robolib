/******************************************************************************
* source/bootloader/bootloader.c                                              *
* ==============================                                              *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

//**************************<File version>*************************************
#define SYSTEM_BOOTLOADER_VERSION \
    "source/bootloader/bootloader.c 23.10.2015 V1.0.1"

//**************************<Included files>***********************************
#include "system/bootloader.h"

#include <avr/boot.h>

#include "system/assembler.h"
#include "system/types.h"
#include "system/delay.h"

//**************************<Variables>****************************************
#if (FLASHEND > 0xFFFF)
    union uint32 system_bootloader_flashadr;
#else //#if (FLASHEND > 0xFFFF)
    union uint16 system_bootloader_flashadr;
#endif //#if (FLASHEND > 0xFFFF)

//**************************<Prototypes>***************************************
uint8_t system_bootloader_idle(void);
uint8_t system_bootloader_wait(void);
uint8_t system_bootloader_wait_char(uint8_t character);
void system_bootloader_error(void);
void system_bootloader_whoami(void);
void system_bootloader_info(void);
void system_bootloader_help(void);
void system_bootloader_exit(void);
void system_bootloader_setadr(void);
void system_bootloader_adrget(void);
void system_bootloader_adrget_out(void);
void system_bootloader_read(void);
void system_bootloader_write(void);
void system_bootloader_reset(void);

//**************************<Files>********************************************

//**************************[system_bootloader_idle]*************************** 27.09.2015
uint8_t system_bootloader_idle() {

    // wait for new data (maximum 50 ms)
    uint8_t t = 50;
    while (t--) {
        if (bootloader_data_stat()) {
            return 0x00; // return value is reversed: not idle any more
        }
        delay_ms(1);
    }

    // notify user
    if (! bootloader_idle()) {
        system_bootloader_reset();
    }
    return 0xFF; // return value is reversed: still idle
}

//**************************[system_bootloader_wait]*************************** 27.09.2015
uint8_t system_bootloader_wait() {

    // wait for next data (maximum 100 ms)
    uint8_t t = 100;
    while (t--) {
        if (bootloader_data_stat()) {
            return 0xFF;
        }
        delay_ms(1);
    }

    // no data leads to an error
    system_bootloader_error();
    return 0x00;
}

//**************************[system_bootloader_wait_char]********************** 27.09.2015
uint8_t system_bootloader_wait_char(uint8_t character) {

    // wait for next data (maximum 100 ms)
    if (! system_bootloader_wait()) {
        return 0x00;
    }

    // check if data was expected data
    uint8_t data = bootloader_data_in();
    if (data == character) {
        return 0xFF;
    }

    system_bootloader_error();
    return 0x00;
}

//**************************[system_bootloader_error]************************** 27.09.2015
void system_bootloader_error(){

    bootloader_data_out('e');
    bootloader_data_out('r');
    bootloader_data_out('r');
    bootloader_data_out('o');
    bootloader_data_out('r');
    bootloader_data_out(13 );

    // notify user
    if (! bootloader_error()) {
        system_bootloader_reset();
    }

    uint8_t t = 100;

    // dump every data until there is no more data for 0.1 second
    while (t--) {
        if (bootloader_data_stat()) {
            bootloader_data_in();
            t = 100;
        }
        delay_ms(1);
    }
}

//**************************[system_bootloader_whoami]************************* 27.09.2015
void system_bootloader_whoami(){

    bootloader_data_out('F');
    bootloader_data_out('4');
    bootloader_data_out( 13);
}

//**************************[system_bootloader_info]*************************** 27.09.2015
void system_bootloader_info() {

    //  system_bootloader_wait_char('i') &&
    if (system_bootloader_wait_char('n') &&
      system_bootloader_wait_char(  'f') &&
      system_bootloader_wait_char(  'o')) {

        bootloader_data_out('I');

        #if (defined(boot_signature_byte_get) && defined(SIGRD))
            bootloader_data_out(boot_signature_byte_get(0));
            bootloader_data_out(boot_signature_byte_get(2));
            bootloader_data_out(boot_signature_byte_get(4));
        #else // #if (defined(boot_signature_byte_get) && defined(SIGRD))
            bootloader_data_out(SIGNATURE_0);
            bootloader_data_out(SIGNATURE_1);
            bootloader_data_out(SIGNATURE_2);
        #endif // #if (defined(boot_signature_byte_get) && defined(SIGRD))

        bootloader_data_out( SPM_PAGESIZE       & 0xFF);
        bootloader_data_out((SPM_PAGESIZE >> 8) & 0xFF);

        bootloader_data_out( BOOTSTART        & 0xFF);
        bootloader_data_out((BOOTSTART >>  8) & 0xFF);
        #if (BOOTSTART > 0xFFFF)
        bootloader_data_out(((uint32_t) BOOTSTART >> 16) & 0xFF);
        #else
            bootloader_data_out(0x00);
        #endif

        bootloader_data_out(13);
    }
}

//**************************[system_bootloader_help]*************************** 27.09.2015
void system_bootloader_help() {

    //  system_bootloader_wait_char('h') &&
    if (system_bootloader_wait_char('e') &&
      system_bootloader_wait_char(  'l') &&
      system_bootloader_wait_char(  'p')) {

        bootloader_help();
    }
}

//**************************[system_bootloader_exit]*************************** 27.09.2015
void system_bootloader_exit() {

    //  system_bootloader_wait_char('e') &&
    if (system_bootloader_wait_char('x') &&
      system_bootloader_wait_char(  'i') &&
      system_bootloader_wait_char(  't')) {

        bootloader_data_out('@');
        system_bootloader_reset();
    }
}

//**************************[system_bootloader_setadr]************************* 27.09.2015
void system_bootloader_setadr() {

    //  system_bootloader_wait_char('s') &&
    if (system_bootloader_wait_char('e') &&
      system_bootloader_wait_char(  't') &&
      system_bootloader_wait_char(  'a') &&
      system_bootloader_wait_char(  'd') &&
      system_bootloader_wait_char(  'r')) {

        union uint32 flashadr;
        if (! system_bootloader_wait()) { return;}
        flashadr.l  = bootloader_data_in();

        if (! system_bootloader_wait()) { return;}
        flashadr.h  = bootloader_data_in();

        if (! system_bootloader_wait()) { return;}
        #if (FLASHEND > 0xFFFF)
            flashadr.hl = bootloader_data_in();
        #else
            bootloader_data_in();
        #endif // #if (FLASHEND > 0xFFFF)

        #if (FLASHEND > 0xFFFF)
            flashadr.u &= ~((uint32_t) SPM_PAGESIZE - 1);
        #else //#if (FLASHEND > 0xFFFF)
            flashadr.u &= ~((uint16_t) SPM_PAGESIZE - 1);
        #endif //#if (FLASHEND > 0xFFFF)

        if (flashadr.u >= BOOTSTART) {
            system_bootloader_error();
            return;
        }

        system_bootloader_flashadr.u = flashadr.u;
        system_bootloader_adrget_out();
    }
}

//**************************[system_bootloader_adrget]************************* 27.09.2015
void system_bootloader_adrget() {

    //  system_bootloader_wait_char('a') &&
    if (system_bootloader_wait_char('d') &&
      system_bootloader_wait_char(  'r') &&
      system_bootloader_wait_char(  'g') &&
      system_bootloader_wait_char(  'e') &&
      system_bootloader_wait_char(  't')) {

        system_bootloader_adrget_out();
    }
}

//**************************[system_bootloader_adrget_out]********************* 27.09.2015
void system_bootloader_adrget_out() {

    bootloader_data_out('A');

    bootloader_data_out(system_bootloader_flashadr.l);
    bootloader_data_out(system_bootloader_flashadr.h);

    #if (FLASHEND > 0xFFFF)
        bootloader_data_out(system_bootloader_flashadr.hl);
    #else //#if (FLASHEND > 0xFFFF)
        bootloader_data_out(0);
    #endif //#if (FLASHEND > 0xFFFF)

    bootloader_data_out(13);
}

//**************************[system_bootloader_write]************************** 27.09.2015
void system_bootloader_write() {

    //  system_bootloader_wait_char('w') &&
    if (system_bootloader_wait_char('r') &&
      system_bootloader_wait_char(  'i') &&
      system_bootloader_wait_char(  't') &&
      system_bootloader_wait_char(  'e')) {

        // erase page buffer
        boot_page_erase_safe(system_bootloader_flashadr.u);
        boot_spm_busy_wait();

        // fill page buffer
        uint8_t count,data,crc;
        union uint16 w;

        #if (FLASHEND > 0xFFFF)
        union uint32 flashadr;
        #else //#if (FLASHEND > 0xFFFF)
        union uint16 flashadr;
        #endif //#if (FLASHEND > 0xFFFF)

        flashadr.u = system_bootloader_flashadr.u;
        crc = 0;
        for (count = (SPM_PAGESIZE / 2); count; count--) {
            if (! system_bootloader_wait()) { return;}
            data = bootloader_data_in(); crc+= data; w.l = data;
            if (! system_bootloader_wait()) { return;}
            data = bootloader_data_in(); crc+= data; w.h = data;
            cli();
            boot_page_fill_safe (flashadr.u, w.u);
            sei();
            flashadr.u+= 2;
        }

        // write page buffer
        cli();
        boot_page_write_safe(system_bootloader_flashadr.u);
        boot_spm_busy_wait();
        sei();

        boot_rww_enable_safe();

        // send checksum
        bootloader_data_out('W');
        bootloader_data_out(crc);
        bootloader_data_out(13);
    }
}

//**************************[system_bootloader_read]*************************** 23.10.2015
void system_bootloader_read() {

    //  system_bootloader_wait_char('r') &&
    if (system_bootloader_wait_char('e') &&
      system_bootloader_wait_char(  'a') &&
      system_bootloader_wait_char(  'd')) {

        bootloader_data_out('R');

        boot_rww_enable_safe();

        // read page
        uint16_t count;
        uint8_t  data,crc;

        #if (FLASHEND > 0xFFFF)
            union uint32 flashadr;
        #else //#if (FLASHEND > 0xFFFF)
            union uint16 flashadr;
        #endif //#if (FLASHEND > 0xFFFF)

        flashadr.u = system_bootloader_flashadr.u;
        crc = 0;
        for (count = SPM_PAGESIZE; count; count--) {
            #if (FLASHEND > 0xFFFF)
                data = pgm_read_byte_far (flashadr.u);
            #else // #if (FLASHEND > 0xFFFF)
                data = pgm_read_byte (flashadr.u);
            #endif // #if (FLASHEND > 0xFFFF)

            crc+= data;
            bootloader_data_out(data);
            flashadr.u++;
        }

        // send checksum
        bootloader_data_out(crc);
        bootloader_data_out(13);
    }
}

//**************************[main]********************************************* 27.09.2015
int main (void) {

    system_init_cli();

    if (! bootloader_start()) {
        system_bootloader_reset();
    }
    sei();

    while (1) {
        while (system_bootloader_idle()) {}

        switch (bootloader_data_in()) {
            case '?' : system_bootloader_whoami(); break;
            case 'i' : system_bootloader_info();   break;
            case 'h' : system_bootloader_help();   break;

            case 'e' : system_bootloader_exit();   break;

            case 's' : system_bootloader_setadr(); break;
            case 'a' : system_bootloader_adrget(); break;
            case 'r' : system_bootloader_read();   break;
            case 'w' : system_bootloader_write();  break;
            default  : system_bootloader_error();   break;
        }
    }
    return (0);
}

//**************************[system_bootloader_print]************************** 27.09.2015
void system_bootloader_print(void (*out)(uint8_t)) {
    string_from_const(out, "bootloader:"                               "\r\n");

    string_from_const(out,"  <empty>"                                  "\r\n");
}

//**************************[system_bootloader_print_compiled]***************** 27.09.2015
void system_bootloader_print_compiled(void (*out)(uint8_t)) {
    string_from_const(out, SYSTEM_BOOTLOADER_VERSION                   "\r\n");

    string_from_const(out,                                             "\r\n");
}

//**************************[system_bootloader_reset]************************** 27.09.2015
void system_bootloader_reset(){

    // notify user
    sei();
    bootloader_end();
    cli();

    #if (defined(GICR))
        GICR = _BV(IVCE); // set   Interrupt Vector Change Enable Bit (IVCE)
        GICR =      0x00; // clear Interrupt Vector Select Bit (IVSEL)
                          // ==> Vectortable within normal code area
    #else //#if (defined(GICR))
        MCUCR = _BV(IVCE); // set   Interrupt Vector Change Enable Bit (IVCE)
        MCUCR =      0x00; // clear Interrupt Vector Select Bit (IVSEL)
                           // ==> Vectortable within normal code area
    #endif //#if (defined(GICR))

    boot_rww_enable_safe();
    delay_ms(20);
    jmp(0x000000);
}

//**************************[system_bootloader_init]*************************** 27.09.2015
void system_bootloader_init(void) {

    #if (defined(GICR))
        GICR = _BV(IVCE);  // set Interrupt Vector Change Enable Bit (IVCE)
        GICR = _BV(IVSEL); // set Interrupt Vector Select Bit (IVSEL)
                           // ==> Vectortable within bootloader area
    #else //#if (defined(GICR))
        MCUCR = _BV(IVCE);  // set Interrupt Vector Change Enable Bit (IVCE)
        MCUCR = _BV(IVSEL); // set Interrupt Vector Select Bit (IVSEL)
                           // ==> Vectortable within bootloader area
    #endif //#if (defined(GICR))
}
