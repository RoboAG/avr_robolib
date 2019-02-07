/******************************************************************************
* robolib/types.h                                                             *
* ===============                                                             *
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
*   https://github.com/RoboAG/avr_robolib                                     *
******************************************************************************/

#ifndef _ROBOLIB_TYPES_H_
#define _ROBOLIB_TYPES_H_

//**************************<Macros>*******************************************
//! The number of bytes the types modul uses in order to work
#define ROBOLIB_RAM_COUNT_TYPES 0

#ifndef __DOXYGEN__

    // for automated initializing see end of this file

#endif // #ifndef doxygen

//**************************<Included files>***********************************
#include <inttypes.h>
#include <avr/interrupt.h>

//**************************<Types>********************************************
/// 8-bit Integer
union uint8 {
   int8_t i;  ///< signed   8-bit integer
  uint8_t u;  ///< unsigned 8-bit integer
};

/// 16-bit Integer
union uint16 {
   int16_t i; ///< signed   16-bit integer
  uint16_t u; ///< unsigned 16-bit integer

  struct {
    uint8_t l; ///< unsigned 8-bit integer, refers to the low  byte
    uint8_t h; ///< unsigned 8-bit integer, refers to the high byte
  };
};

/// 32-bit Integer
union uint32 {
   int32_t i; ///< signed   32-bit integer
  uint32_t u; ///< unsigned 32-bit integer

  struct {
    uint16_t lu; ///< unsigned 16-bit integer, refers to the low  word
    uint16_t hu; ///< unsigned 16-bit integer, refers to the high word
  };

  struct {
    int16_t li; ///< signed 16-bit integer, refers to the low  word
    int16_t hi; ///< signed 16-bit integer, refers to the high word
  };

  struct {
    uint8_t ll; ///< unsigned 8-bit integer, refers to the low  byte of the low  word
    uint8_t lh; ///< unsigned 8-bit integer, refers to the high byte of the low  word
    uint8_t hl; ///< unsigned 8-bit integer, refers to the low  byte of the high word
    uint8_t hh; ///< unsigned 8-bit integer, refers to the high byte of the high word
  };

  struct {
    uint8_t l;  ///< unsigned 8-bit integer, refers to the low  byte of the low  word
    uint8_t h;  ///< unsigned 8-bit integer, refers to the high byte of the low  word
  };
};

//**************************<Functions>****************************************
// automated initializing
#ifndef __DOXYGEN__
    __inline__ void robolib_types_init_inline(void) {
        #ifdef ROBOLIB_INIT
            ROBOLIB_INIT();
        #endif
    }

    #undef ROBOLIB_INIT
    #define ROBOLIB_INIT()     robolib_types_init_inline()
    #define robolib_init()     cli(); ROBOLIB_INIT(); sei()
    #define robolib_init_cli() cli(); ROBOLIB_INIT()
#endif

#endif // #ifndef _ROBOLIB_TYPES_H_
