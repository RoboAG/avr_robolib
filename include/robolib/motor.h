/******************************************************************************
* robolib/motor.h                                                             *
* ===============                                                             *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 09.04.16                                                           *
* Author : David Ehnert                                                       *
*                                                                             *
* Changelog:                                                                  *
*   09.04.16(V1.0.0)                                                          *
*     - first release                                                         *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

#ifndef _ROBOLIB_MOTOR_H_
#define _ROBOLIB_MOTOR_H_


//**************************<Macros>*******************************************
#ifndef __DOXYGEN__

    // for automated initializing see end of this file

#endif // #ifndef doxygen

//! The number of bytes the display modul uses in order to work
#define ROBOLIB_RAM_COUNT_DISPLAY 1

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/interrupt.h>

#include <robolib/delay.h>
#include <robolib/assembler.h>

//**************************<Types>********************************************

//**************************<Functions>****************************************
/**
 * @brief Sets the speed of both motors.
 *
 *
 *
 * @param l
 * 8-bit speed (maximum 100) of the left motor.
 *
 * @param l
 * 8-bit speed (maximum 100) of the right motor.
 */
void set_motors(int8_t l,int8_t r);


/**
 * @brief Internal function.
 *
 * Do not call this function at all.
 */
void robolib_motor_init(void);

// automated initializing
#ifndef __DOXYGEN__
    __inline__ void robolib_motor_init_inline(void) {
        #ifdef ROBOLIB_INIT
            ROBOLIB_INIT();
        #endif

        robolib_motor_init();
    }

    #undef ROBOLIB_INIT
    #define ROBOLIB_INIT()     robolib_motor_init_inline()
    #define robolib_init()     cli(); ROBOLIB_INIT(); sei()
    #define robolib_init_cli() cli(); ROBOLIB_INIT()
#endif

#endif // #ifndef _ROBOLIB_MOTOR_H_
