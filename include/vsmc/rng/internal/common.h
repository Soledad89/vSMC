//============================================================================
// include/vsmc/rng/internal/common.h
//----------------------------------------------------------------------------
//
//                         vSMC: Scalable Monte Carlo
//
// This file is distribured under the 2-clauses BSD License.
// See LICENSE for details.
//============================================================================

#ifndef VSMC_RNG_INTERNAL_COMMON_H
#define VSMC_RNG_INTERNAL_COMMON_H

#ifndef VSMC_HAS_OPENCL_DOUBLE
#define VSMC_HAS_OPENCL_DOUBLE 0
#endif

#ifdef __OPENCL_VERSION__
  #ifndef VSMC_HAS_OPENCL_DOUBLE
  #define VSMC_HAS_OPENCL_DOUBLE 0
  #endif
  #ifndef UINT64_C
  #define UINT64_C(x) ((ulong)(x##UL))
  #endif
  #ifndef VSMC_STATIC_INLINE
  #if defined(__OPENCL_C_VERSION__) && __OPENCL_C_VERSION__ >= 120
    #define VSMC_STATIC_INLINE static inline
  #else
    #define VSMC_STATIC_INLINE inline
  #endif
  #endif // VSMC_STATIC_INLINE
#else // __OPENCL_VERSION__
  #ifndef VSMC_HAS_OPENCL_DOUBLE
  #define VSMC_HAS_OPENCL_DOUBLE 1
  #endif
  #ifndef __STDC_CONSTANT_MACROS
  #define __STDC_CONSTANT_MACROS
  #endif
  #include <stdint.h>
  #ifndef UINT64_C
  #error __STDC_CONSTANT_MACROS not defined before #include<stdint.h>
  #endif
  #ifdef __cplusplus
    #ifndef VSMC_STATIC_INLINE
    #define VSMC_STATIC_INLINE inline
    #endif
    #include <cmath>
  #else // __cplusplus
    #ifndef VSMC_STATIC_INLINE
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
      #define VSMC_STATIC_INLINE static inline
    #else
      #define VSMC_STATIC_INLINE static
    #endif
    #endif // VSMC_STATIC_INLINE
    #include <math.h>
  #endif // __cplusplus
#endif // __OPENCL_VERSION__

#endif // VSMC_RNG_INTERNAL_COMMON_H