/******************************************************************************
* system/motor.h                                                              *
* ================                                                            *
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

#ifndef _SYSTEM_MOTOR_H_
#define _SYSTEM_MOTOR_H_


//**************************<Macros>*******************************************
#ifndef __DOXYGEN__

    // for automated initializing see end of this file

#endif // #ifndef doxygen

//! The number of bytes the display modul uses in order to work
#define SYSTEM_RAM_COUNT_DISPLAY 1

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/interrupt.h>

#include <system/delay.h>
#include <system/assembler.h>

//**************************<Types>********************************************

//**************************<Functions>****************************************
/**
 * @brief Sets the speed of both motors.
 *
 * This function sets the  one byte of data to the hardware buffer
 * of the connected display.
 *
 * If the display is not responding in a resonable amount of time
 * it will be disabled.
 *
 * If the display is disabled this and other display functions will return
 * without any effect.
 *
 * This function must be handled with care when called from within an interrupt.
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
void system_motor_init(void);

// automated initializing
#ifndef __DOXYGEN__
    __inline__ void system_motor_init_inline(void) {
        #ifdef SYSTEM_INIT
            SYSTEM_INIT();
        #endif;

        system_motor_init();
    }

    #define SYSTEM_INIT()     system_motor_init_inline()
    #define system_init()     cli(); SYSTEM_INIT(); sei()
    #define system_init_cli() cli(); SYSTEM_INIT()
#endif

#endif // #ifndef _SYSTEM_MOTOR_H_
