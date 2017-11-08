/******************************************************************************
* tick/tick_atmega64.c                                                        *
* ====================                                                        *
*                                                                             *
* Author : Peter Weissig                                                      *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

//**************************<File version>*************************************
#define ROBOLIB_TICK_SUB_VERSION \
  "robolib/tick/tick_atmega64.c 27.09.2015 V1.0.0"

//**************************[tick_init]**************************************** 27.09.2015
void robolib_tick_init() {

    #if TICK_SYSTICK == TIMER0 // switch TIMER

        #if   F_CPU / 100 / 1024 > 256
            #error "can't setup systick with TIMER0 - prescaler maximum is 1024"

        #elif F_CPU / 100 /  256 > 256
            #define ROBOLIB_TICK_CS  0b111
            #define ROBOLIB_TICK_MAX ((uint8_t) (F_CPU / 100 / 1024))

        #elif F_CPU / 100 /  128 > 256
            #define ROBOLIB_TICK_CS  0b110
            #define ROBOLIB_TICK_MAX ((uint8_t) (F_CPU / 100 /  256))

        #elif F_CPU / 100 /   64 > 256
            #define ROBOLIB_TICK_CS  0b101
            #define ROBOLIB_TICK_MAX ((uint8_t) (F_CPU / 100 /  256))

        #elif F_CPU / 100 /   32 > 256
            #define ROBOLIB_TICK_CS  0b100
            #define ROBOLIB_TICK_MAX ((uint8_t) (F_CPU / 100 /  256))

        #elif F_CPU / 100 /    8 > 256
            #define ROBOLIB_TICK_CS  0b011
            #define ROBOLIB_TICK_MAX ((uint8_t) (F_CPU / 100 /   64))

        #elif F_CPU / 100 /    1 > 256
            #define ROBOLIB_TICK_CS  0b010
            #define ROBOLIB_TICK_MAX ((uint8_t) (F_CPU / 100 /    8))

        #else
            #define ROBOLIB_TICK_CS  0b001
            #define ROBOLIB_TICK_MAX ((uint8_t) (F_CPU / 100 /    1))
        #endif

        // 8-bit Timer
        // Modus 2 (CTC bis OCRA)
        TCCR0 =  _BV(WGM01) | (ROBOLIB_TICK_CS & 0x07);
            // Bit 6   (WGM00) = 0 select timer mode [WGM01 in TCCR0]
            // Bit 4-5 (COM0x) = 00 output mode (none)
            // Bit 3   (WGM01) = 1 select timer mode [WGM00 in TCCR0]
            // Bit 0-2 (CS02 ) = ??? [calculated]

        TCNT0 = 0;
            // Timer/Counter Register - current value of timer

        OCR0  = ROBOLIB_TICK_MAX;
            // Output Compare Register - top for timer

        ASSR = 0;
            // Asynchronous Status Register - Timer0 clocked from I/O clock

        TIMSK = _BV(OCIE0);
            // Bit 7   (OCIE2 ) =    0 [Timer2]
            // Bit 6   (TOIE2 ) =    0 [Timer2]
            // Bit 5   (TICIE1) =    0 [Timer1]
            // Bit 4   (OCIE1A) =    0 [Timer1]
            // Bit 3   (OCIE1B) =    0 [Timer1]
            // Bit 2   (TOIE1 ) =    0 [Timer1]
            // Bit 1   (OCIE0 ) =    1 interrupt for compare match (tick)
            // Bit 0   (TOIE0 ) =    0 interrupt for overflow

        TIFR = _BV(OCF0) | _BV(TOV0);
            // Bit 7   (OCF2  ) =    0 [Timer2]
            // Bit 6   (TOV2  ) =    0 [Timer2]
            // Bit 5   (ICF1  ) =    0 [Timer1]
            // Bit 4   (OCF1A ) =    0 [Timer1]
            // Bit 3   (OCF1B ) =    0 [Timer1]
            // Bit 2   (TOV1  ) =    0 [Timer1]
            // Bit 1   (OCF0  ) =    1 interrupt flag for compare match
            // Bit 0   (TOV0  ) =    1 interrupt flag for overflow

        #define ROBOLIB_TICK_ISR ISR(TIMER0_COMP_vect)
        #define ROBOLIB_TICK_ISR_SEI() ( \
           { TIMSK&= ~ _BV(OCIE0); sei();})
        #define ROBOLIB_TICK_ISR_CLI() ( \
           { cli(); TIMSK|= _BV(OCIE0);})

    #else                                 // switch TICK_SYSTICK
        #error "timer is not supported (yet) for this device"
    #endif                                // switch TICK_SYSTICK
}

//**************************[ISR(tick)]**************************************** 27.09.2015
ROBOLIB_TICK_ISR {

    robolib_tick_time++;

    ROBOLIB_TICK_ISR_SEI();

    #ifdef ROBOLIB_TICK_FUNCTION
        ROBOLIB_TICK_FUNCTION();
    #endif;

    ROBOLIB_TICK_ISR_CLI();
}
