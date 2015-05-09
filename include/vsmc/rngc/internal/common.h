//============================================================================
// vSMC/include/vsmc/rngc/internal/common.h
//----------------------------------------------------------------------------
//                         vSMC: Scalable Monte Carlo
//----------------------------------------------------------------------------
// Copyright (c) 2013-2015, Yan Zhou
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#ifndef VSMC_RNGC_INTERNAL_COMMON_H
#define VSMC_RNGC_INTERNAL_COMMON_H

#ifdef __OPENCL_VERSION__
#ifndef VSMC_HAS_RNGC_DOUBLE
#define VSMC_HAS_RNGC_DOUBLE 0
#endif
typedef uint uint32_t;
typedef ulong uint64_t;
#define UINT32_C(x) ((uint)(x##U))
#define UINT64_C(x) ((ulong)(x##UL))
#ifndef VSMC_STATIC_INLINE
#if defined(__OPENCL_C_VERSION__) && __OPENCL_C_VERSION__ >= 120
#define VSMC_STATIC_INLINE static inline
#else
#define VSMC_STATIC_INLINE inline
#endif
#endif // VSMC_STATIC_INLINE
#else  // __OPENCL_VERSION__
#ifndef VSMC_HAS_RNGC_DOUBLE
#define VSMC_HAS_RNGC_DOUBLE 1
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

#define VSMC_FP24_C(x) (x##f)
#define VSMC_FP53_C(x) (x)

#define VSMC_LOG53 log
#define VSMC_EXP53 exp
#define VSMC_EXPM153 expm1
#define VSMC_SQRT53 sqrt
#define VSMC_SIN53 sin
#define VSMC_COS53 cos

#ifdef __OPENCL_VERSION__
#define VSMC_LOG24 log
#define VSMC_EXP24 exp
#define VSMC_EXPM124 expm1
#define VSMC_SQRT24 sqrt
#define VSMC_SIN24 sin
#define VSMC_COS24 cos
#else // __OPENCL_VERSION__
#define VSMC_LOG24 logf
#define VSMC_EXP24 expf
#define VSMC_EXPM124 expm1f
#define VSMC_SQRT24 sqrtf
#define VSMC_SIN24 sinf
#define VSMC_COS24 cosf
#endif // __OPENCL_VERSION__

/// \brief Default RNG state
/// \ingroup RNGC
#ifndef vsmc_rng
#define vsmc_rng vsmc_philox4x32
#endif

/// \brief Initialize default RNG state
/// \ingroup RNGC
#ifndef vsmc_rng_init
#define vsmc_rng_init vsmc_philox4x32_init
#endif

/// \brief Generate random 32-bits integers from default RNG
/// \ingroup RNGC
#ifndef vsmc_rng_rand
#define vsmc_rng_rand vsmc_philox4x32_rand
#endif

#endif // VSMC_RNGC_INTERNAL_COMMON_H