/******************************************************************************
* source/motor/motor.c                                                    *
* ========================                                                    *
*                                                                             *
* Author : David Ehnert                                                       *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

//**************************<File version>*************************************
#define SYSTEM_DISPLAY_VERSION \
    "source/motor/motor.c 09.04.2016 V1.0.0"

//**************************<Included files>***********************************
#include "system/motor.h"

#include "system/assembler.h"
#include "system/types.h"

#include <avr/io.h>

//**************************<Variables>****************************************

//**************************<Prototypes>***************************************

//**************************<Files>********************************************

//**************************[set_motors]*************************************** 09.04.2016
void set_motors(int8_t l,int8_t r) {
  if (l <= 100) {
    if (l < 0) {
      l = -l;
      PORTB&= ~_BV(4);
    } else {
      PORTB|=  _BV(4);
    }
    uint8_t MSREG = SREG;
    cli();
    OCR1AH = 0;
    OCR1AL = l;
    sei();
    SREG = MSREG;
  }
  if (r <= 100) {
    if (r < 0) {
      r = -r;
      PORTB&= ~_BV(3);
    } else {
      PORTB|=  _BV(3);
    }
    uint8_t MSREG = SREG;
    cli();
    OCR1BH = 0;
    OCR1BL = r;
    SREG = MSREG;
  }
}

//**************************[system_motor_init]****************************** 09.04.2016
void system_motor_init(void) {
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(COM1C1);
  // Ausgaenge A-C an; PWM-Modus 8 (Phasen und Frequenzrichtig)
  TCCR1B = _BV(CS11) | _BV(WGM13);
  // PWM-Modus 8 (Phasen und Frequenzrichtig); Prescaler 8; No Capture
  //TIMSK = 0;
  ICR1H = 0;
  ICR1L = 100;
  //Set Maximum Value to 100
}
