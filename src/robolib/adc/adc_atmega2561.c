/******************************************************************************
* adc/adc_atmega2561.c                                                        *
* ====================                                                        *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/RoboAG/avr_robolib                                     *
******************************************************************************/

//**************************<File version>*************************************
#define ROBOLIB_ADC_SUB_VERSION \
  "robolib/adc/adc_atmega2561.c 27.09.2015 V1.0.0"

//**************************[robolib_adc_init]********************************** 27.09.2015
void robolib_adc_init() {

    // set hardware registers
    ADMUX  = _BV(REFS0);
      // Bit 6-7 (REFSx ) =   01 internal connected to AVCC
      // Bit 5   (ADLAR ) =    0 right justified result
      // Bit 0-4 (MUXx  ) = 0000 select adc-pin 0

    ADCSRA = _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
      // Bit 7   (ADEN  ) =    0 adc disabled
      // Bit 6   (ADSC  ) =    0 no conversion (no start)
      // Bit 5   (ADATE ) =    0 no auto trigger
      // Bit 4   (ADIF  ) =    0 interrupt flag
      // Bit 3   (ADIE  ) =    0 interrupt enable
      // Bit 0-2 (ADPSx ) =  111 prescaler (128)

    ADCSRB = 0x00;
      // Bit 6   (ACME  ) =    0 multiplexer for analog comparator disabled
      // Bit 0-2 (ADTSx ) =  000 auto trigger source (free running mode)


    // start adc
    adc_enable();
}

//**************************[robolib_adc_getchannel]**************************** 27.09.2015
uint8_t robolib_adc_getchannel(void) {

    uint8_t mSREG = SREG;
    uint8_t temp;

    cli();

    temp = ADMUX;
    temp&= _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);

    SREG = mSREG;

    return temp;
}

//**************************[robolib_adc_setchannel]**************************** 27.09.2015
void robolib_adc_setchannel(uint8_t nr) {

    uint8_t mSREG = SREG;

    cli();
    if (robolib_adc_adcok() == 0x00) {
        SREG = mSREG;
        return;
    }

    nr&= 0x1F;
    ADMUX = (ADMUX & 0xE0) | nr;

    // start conversion
    ADCSRA|= _BV(ADSC);
    SREG = mSREG;
}
