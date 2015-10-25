/******************************************************************************
* system/assembler.h                                                          *
* ==================                                                          *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 27.09.15                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
* Changelog:                                                                  *
*   27.09.15(V1.0.0)                                                          *
*     - first release                                                         *
*                                                                             *
* For help or bug report please visit:                                        *
*   https://github.com/peterweissig/robolib                                   *
******************************************************************************/

#ifndef _SYSTEM_ASSEMBLER_H_
#define _SYSTEM_ASSEMBLER_H_

//**************************<Macros>*******************************************
//! The number of bytes the types modul uses in order to work
#define SYSTEM_RAM_COUNT_ASSEMBLER 0

#ifndef __DOXYGEN__
    // used to create portnames like "PORTA" for further processing
    #define SYSTEM_PORT_CREATE2(port_letter) PORT ## port_letter
    #define SYSTEM_PIN_CREATE2( port_letter) PIN  ## port_letter
    #define SYSTEM_DDR_CREATE2( port_letter) DDR  ## port_letter

    // needed for full macro expansion
    #define SYSTEM_PORT_CREATE(port_letter) SYSTEM_PORT_CREATE2(port_letter)
    #define SYSTEM_PIN_CREATE( port_letter) SYSTEM_PIN_CREATE2( port_letter)
    #define SYSTEM_DDR_CREATE( port_letter) SYSTEM_DDR_CREATE2( port_letter)

    
    // for automated initializing see end of this file

#endif // #ifndef doxygen

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/interrupt.h>

//**************************<Types>********************************************

//**************************<Functions>****************************************
/**
 * @brief No operation.
 *
 * This comand has no effect, but it takes exactly one clock cycle to execute.
 */
#ifdef __DOXYGEN__
    void nop(void);
#else // #ifdef __DOXYGEN__
    #define nop()  __asm__ __volatile__ ("nop" ::)
#endif // #ifdef __DOXYGEN__

/**
 * @brief Jump to adress.
 *
 * This comand directly jumps to the given 16-bit adress.
 */
#ifdef __DOXYGEN__
    void jmp(uint16_t adr);
#else // #ifdef __DOXYGEN__
    #define jmp(adr) __asm__ __volatile__ ("JMP " #adr ::)
#endif // #ifdef __DOXYGEN__


// automated initializing
#ifndef __DOXYGEN__
    __inline__ void system_assembler_init_inline(void) {
        #ifdef SYSTEM_INIT
            SYSTEM_INIT();
        #endif;
    }

    #define SYSTEM_INIT()     system_assembler_init_inline()
    #define system_init()     cli(); SYSTEM_INIT(); sei()
    #define system_init_cli() cli(); SYSTEM_INIT()
#endif

#endif // #ifndef _SYSTEM_ASSEMBLER_H_
