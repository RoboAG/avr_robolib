/******************************************************************************
* source/eeprom/eeprom.c                                                      *
* ======================                                                      *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

//**************************<File version>*************************************
#define SYSTEM_EEPROM_VERSION \
    "source/eeprom/eeprom.c 27.09.2015 V1.0.0"

//**************************<Included files>***********************************
#include "system/eeprom.h"
#include "system/string.h"

#include <avr/io.h>

//**************************<Variables>****************************************
volatile uint16_t system_eeprom_adress;

//**************************<Prototypes>***************************************

//**************************<Files>********************************************

//**************************[eeprom_write]************************************* 27.09.2015
void eeprom_write(uint8_t data) {

    eeprom_write_byte((uint8_t *)system_eeprom_adress, data);

    system_eeprom_adress++;
    if (system_eeprom_adress > E2END) {
        system_eeprom_adress = E2END;
    }
}

//**************************[eeprom_read]************************************** 27.09.2015
uint8_t eeprom_read() {

    uint8_t result;
    result = eeprom_read_byte((uint8_t *)system_eeprom_adress);

    system_eeprom_adress++;
    if (system_eeprom_adress > E2END) {
        system_eeprom_adress = E2END;
    }

    return result;
}

//**************************[eeprom_write_uint16]****************************** 27.09.2015
void eeprom_write_uint16(uint16_t data) {

    eeprom_write_word((uint16_t *)system_eeprom_adress, data);

    system_eeprom_adress+= 2;
    if (system_eeprom_adress > E2END) {
        system_eeprom_adress = E2END;
    }
}

//**************************[eeprom_read_uint16]******************************* 27.09.2015
uint16_t eeprom_read_uint16() {

    uint16_t result;
    result = eeprom_read_word((uint16_t *)system_eeprom_adress);

    system_eeprom_adress+= 2;
    if (system_eeprom_adress > E2END) {
        system_eeprom_adress = E2END;
    }

    return result;
}

//**************************[eeprom_adress_set]******************************** 27.09.2015
void eeprom_adress_set(uint16_t adr) {

    if (adr > E2END) {
        adr = E2END;
    }
    system_eeprom_adress = adr;
}

//**************************[eeprom_adress_get]******************************** 27.09.2015
uint16_t eeprom_adress_get() {

    return system_eeprom_adress;
}

//**************************[system_eeprom_print]****************************** 27.09.2015
void system_eeprom_print(void (*out)(uint8_t)) {

    string_from_const(out, "eeprom:"                                   "\r\n");

    string_from_const_length(out, "  adress", 15);
                                                  string_from_const(out, ": ");
    string_from_uint(out, system_eeprom_adress, 4);
                                                string_from_const(out, "\r\n");

    string_from_const(out,                                             "\r\n");
}

//**************************[system_eeprom_print_compiled]********************* 27.09.2015
void system_eeprom_print_compiled(void (*out)(uint8_t)) {

    string_from_const(out, SYSTEM_EEPROM_VERSION                       "\r\n");

    string_from_const(out,                                             "\r\n");
}

//**************************[system_eeprom_init]******************************* 27.09.2015
void system_eeprom_init() {

    system_eeprom_adress = 0x0000;
}
