/******************************************************************************
* source/string/string.c                                                      *
* ======================                                                      *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

//**************************<File version>*************************************
#define SYSTEM_STRING_VERSION \
    "source/string/string.c 05.10.2015 V1.1.0"

//**************************<Included files>***********************************
#include "system/string.h"

#include "system/types.h"

//**************************<Variables>****************************************

//**************************<Prototypes>***************************************

//**************************<Renaming>*****************************************

//**************************<Files>********************************************

//**************************[string_from_uint]********************************* 27.09.2015
void string_from_uint(void (*out)(uint8_t), uint16_t number, uint8_t digits) {

    uint8_t pos;
    uint8_t temp_str[5];

    // init
    pos = 0;
    if (digits > 32) {digits = 32;}

    // calculate digits
    do {
        temp_str[pos++] = (number % 10) + 48;
        number/= 10;
    } while (number);

    // pad with spaces if necessary
    while (digits > pos) {out(32); digits--;}

    // write digits
    while (pos--) {
        out(temp_str[pos]);
    }
}

//**************************[string_from_int]********************************** 27.09.2015
void string_from_int(void (*out)(uint8_t), int16_t number, uint8_t digits) {

    // check if number is negativ
    if (number < 0) {
        number = -number;
        out('-');
        if (digits > 32) {digits = 32;}
        if (digits     ) {digits--;   }
    }
    string_from_uint(out, (uint16_t) number,digits);
}

//**************************[string_from_hex]********************************** 27.09.2015
void string_from_hex(void (*out)(uint8_t), uint16_t number, int8_t digits) {

    uint8_t temp_digit;
    // init
    if (digits < 0) {
        digits = -digits;
        out('0');
        out('x');
    }

    // calculate shift
    if (digits > 4) {
        digits = 0;
    } else {
        digits = (4 - digits) << 2;
    }

    // shift number
    number = number << digits;

    // write digits
    while (digits < 16) {
        temp_digit = (((union uint16) number).h >> 4);
        if (temp_digit < 10) {
            temp_digit+= 48; // '0'
        } else {
            temp_digit+= 55; // 'A' - 10
        }
        out(temp_digit);
        number = number << 4;
        digits+= 4;
    }
}

//**************************[string_from_bits]********************************* 27.09.2015
void string_from_bits(void (*out)(uint8_t), uint16_t number, int8_t digits) {

    // init
    if (digits < 0) {
        digits = -digits;
        out('0');
        out('b');
    }

    // calculate shift
    if (digits > 16) {
        digits = 0;
    } else {
        digits = 16 - digits;
    }

    // shift number
    number = number << digits;

    // write digits
    while (digits < 16) {
        if (((union uint16) number).h & 0x80) {
            out('1');
        } else {
            out('0');
        }
        number = number << 1;
        digits+= 1;
    }
}

//**************************[string_from_bool]********************************* 27.09.2015
void string_from_bool(void (*out)(uint8_t), uint8_t boolean) {

    if (boolean) {
        string_from_const(out,"true");
    } else {
        string_from_const(out,"false");
    }
}

//**************************[string_from_ram]********************************** 27.09.2015
void string_from_ram(void (*out)(uint8_t), const char *str) {

    uint8_t temp_char;

    while (1) {
        temp_char = (uint8_t) *(str++);

        if (temp_char == 0) {
            return;
        }
        out(temp_char);
    }
}

//**************************[string_from_flash]******************************** 05.10.2015
void string_from_flash(void (*out)(uint8_t), const char *str) {

    uint8_t temp_char;

    while (1) {
        #ifdef pgm_read_byte_far
            temp_char = pgm_read_byte_far (str++);
        #else
            temp_char = pgm_read_byte (str++);
        #endif

        if (temp_char == 0) {
            return;
        }
        out(temp_char);
    }
}

//**************************[string_from_flash_length]************************* 05.10.2015
void string_from_flash_length(void (*out)(uint8_t), const char *str,
  uint8_t length) {

    uint8_t temp_char;

    while (length--) {
        #ifdef pgm_read_byte_far
            temp_char = pgm_read_byte_far (str++);
        #else
            temp_char = pgm_read_byte (str++);
        #endif

        if (temp_char == 0) {
            while (length--) {
                out(' ');
            }
            return;
        }
        out(temp_char);
    }
}

//**************************[system_string_print]****************************** 27.09.2015
void system_string_print(void (*out)(uint8_t)) {

    string_from_const(out, "string:"                                   "\r\n");

    string_from_const(out, "  [empty]"                                 "\r\n");

    string_from_const(out,                                             "\r\n");
}

//**************************[system_string_print_compiled]********************* 27.09.2015
void system_string_print_compiled(void (*out)(uint8_t)) {

    string_from_const(out, SYSTEM_STRING_VERSION                       "\r\n");

    string_from_const(out, "  [empty]"                                 "\r\n");

    string_from_const(out,                                             "\r\n");
}
