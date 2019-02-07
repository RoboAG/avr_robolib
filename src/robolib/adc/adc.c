/******************************************************************************
* adc/adc.c                                                                   *
* =========                                                                   *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/RoboAG/avr_robolib                                     *
******************************************************************************/

//**************************<File version>*************************************
#define ROBOLIB_ADC_VERSION \
  "robolib/adc/adc.c 23.09.2014 V1.0.0"

//**************************<Included files>***********************************
#include <robolib/adc.h>

#include <robolib/string.h>

#include <avr/io.h>

//**************************<Variables>****************************************
volatile uint16_t robolib_adc_values[8];

//**************************<Prototypes>***************************************

//**************************<Renaming>*****************************************

//**************************<Files>********************************************
#if    defined (__AVR_ATmega2561__) // switch micro controller
#  include <adc/adc_atmega2561.c>
#  define MCU atmega2561
#elif  defined (__AVR_ATmega64__)   // switch micro controller
#  include <adc/adc_atmega64.c>
#  define MCU atmega64
#else                               // switch micro controller
#  error "device is not supported (yet)"
#endif                              // switch micro controller

//**************************[adc_get]****************************************** 17.09.2014
uint16_t adc_get(uint8_t nr) {
  uint16_t result;

  if (robolib_adc_wait() == 0x00) {
    // robolib_adc_wait aktiviert Interrupts
    return 0xFFFF;
  }
  // robolib_adc_wait deaktiviert Interrupts

  robolib_adc_savevalue();
  robolib_adc_setchannel(nr);

  if (robolib_adc_wait() == 0x00) {
    // robolib_adc_wait aktiviert Interrupts
    return 0xFFFF;
  }
  // robolib_adc_wait deaktiviert Interrupts
  if (robolib_adc_savevalue() == nr) {
    result = ADC;
  } else if (nr < 8) {
    result = robolib_adc_values[nr];
  } else {
    result = 0xFFFF;
  }

  sei();
  return result;
}

//**************************[adc_buffer_get]*********************************** 17.09.2014
uint16_t adc_buffer_get(uint8_t nr) {

  if (nr > 7) {
    return 0xFFFF;
  }

  uint16_t result;
  uint8_t mSREG = SREG;

  cli();
  result = robolib_adc_values[nr];
  SREG = mSREG;

  return result;
}

//**************************[adc_enable]*************************************** 06.09.2014
void adc_enable(void) {
  uint8_t mSREG = SREG;
  cli();

  if ((ADCSRA & _BV(ADEN)) == 0x00) {
    ADCSRA|= _BV(ADEN);
  }

  SREG = mSREG;
}

//**************************[adc_disable]************************************** 06.09.2014
void adc_disable(void) {
  uint8_t mSREG = SREG;
  cli();

  ADCSRA&= ~_BV(ADEN);

  SREG = mSREG;
}

//**************************[adc_is_enabled]*********************************** 11.09.2014
uint8_t adc_is_enabled(void) {
  if (ADCSRA & _BV(ADEN)) {
    return 0xFF;
  } else {
    return 0x00;
  }
}

//**************************[robolib_adc_print]********************************* 16.09.2014
void robolib_adc_print(void (*out)(uint8_t)) {
  string_from_const(out, "adc:"                                        "\r\n");

  string_from_const_length(out, "  enabled", 15); string_from_const(out, ": ");
  string_from_bool(out, adc_is_enabled());      string_from_const(out, "\r\n");

  uint8_t nr;
  for (nr = 0; nr < 8; nr++) {
    string_from_const(out, "  channel["); out(nr + 48);
    string_from_const_length(out, "]", 15 - 11);
    string_from_const(out, ": ");
    string_from_uint(out, robolib_adc_values[nr], 4);
    string_from_const(out, "\r\n");
  }

  string_from_const(out,                                               "\r\n");
}

//**************************[robolib_adc_print_compiled]************************ 16.09.2014
void robolib_adc_print_compiled(void (*out)(uint8_t)) {
  string_from_const(out, ROBOLIB_ADC_VERSION                            "\r\n");
  string_from_const(out, ROBOLIB_ADC_SUB_VERSION                        "\r\n");

  string_from_const_length(out, "  MCU"    , 15); string_from_const(out, ": ");
  string_from_macro(out, MCU);                  string_from_const(out, "\r\n");

  string_from_const(out,                                               "\r\n");
}

//**************************[robolib_adc_init]**********************************
// siehe controllerspezifische Datei

//**************************[robolib_adc_adcok]********************************* 16.09.2014
uint8_t robolib_adc_adcok(void) {
  // adc disabled ?
  if (adc_is_enabled() == 0x00) { return 0x00;}
  // conversation in progress ?
  if (ADCSRA & _BV(ADSC)) { return 0x00;}

  return 0xFF;
}

//**************************[robolib_adc_getchannel]****************************
// siehe controllerspezifische Datei

//**************************[robolib_adc_setchannel]****************************
// siehe controllerspezifische Datei

//**************************[robolib_adc_savevalue]***************************** 16.09.2014
uint8_t robolib_adc_savevalue(void) {
  uint8_t mSREG = SREG;
  uint8_t temp;

  cli();
  if (robolib_adc_adcok() == 0x00) {
    SREG = mSREG;
    return 0xFF;
  }

  temp = robolib_adc_getchannel();

  if (temp <= 7) {
    robolib_adc_values[temp] = ADC;
  }
  SREG = mSREG;

  return temp;
}

//**************************[robolib_adc_wait]********************************** 16.09.2014
// Achtung: bei positiven Ausgang bleiben die Interrupts deaktiviert!
uint8_t robolib_adc_wait(void) {
  uint8_t mSREG = SREG;

  while (1) {
    cli();
    if (robolib_adc_adcok()) { return 0xFF;}
    SREG = mSREG;

    if (adc_is_enabled() == 0x00) {
      return 0x00;
    }
  }
}

