#ifndef VSMC_UTILITY_RANDOM_NORMAL01_H
#define VSMC_UTILITY_RANDOM_NORMAL01_H

#include <vsmc/utility/random/common.h>

#define VSMC_DEFINE_NORMAL01(W, F, FT) \
    typedef struct {                                                         \
        CBRNG4x##W##_KEY_T *k;                                               \
        CBRNG4x##W##_CTR_T *c;                                               \
        CBRNG4x##W##_CTR_T r;                                                \
        FT u[4];                                                             \
        unsigned char remain;                                                \
    } normal01_##W##_##F;                                                    \
                                                                             \
    VSMC_STATIC_INLINE void normal01_##W##_##F##_step (                      \
            normal01_##W##_##F *rnorm)                                       \
    {                                                                        \
        rnorm->c->v[0]++;                                                    \
        rnorm->r = CBRNG4x##W(*(rnorm->c), *(rnorm->k));                     \
        rnorm->u[0] = u01_open_closed_##W##_##F(rnorm->r.v[0]);              \
        rnorm->u[1] = u01_open_closed_##W##_##F(rnorm->r.v[1]);              \
        rnorm->u[2] = u01_open_closed_##W##_##F(rnorm->r.v[2]);              \
        rnorm->u[3] = u01_open_closed_##W##_##F(rnorm->r.v[3]);              \
        rnorm->remain = 4;                                                   \
    }                                                                        \
                                                                             \
    VSMC_STATIC_INLINE void normal01_##W##_##F##_init (                      \
            normal01_##W##_##F *rnorm,                                       \
            CBRNG4x##W##_KEY_T *k, CBRNG4x##W##_CTR_T *c)                    \
    {                                                                        \
        rnorm->k = k;                                                        \
        rnorm->c = c;                                                        \
        normal01_##W##_##F##_step(rnorm);                                    \
    }                                                                        \
                                                                             \
    VSMC_STATIC_INLINE FT normal01_##W##_##F##_rand (                        \
            normal01_##W##_##F *rnorm)                                       \
    {                                                                        \
        if (!rnorm->remain)                                                  \
            normal01_##W##_##F##_step(rnorm);                                \
                                                                             \
        rnorm->remain--;                                                     \
        unsigned char k = rnorm->remain;                                     \
        unsigned char i = (k>>1)<<1;                                         \
        return k == i ?                                                      \
            sqrt(-2 * log(rnorm->u[i])) * cos(2 * M_PI_##F * rnorm->u[i+1]): \
            sqrt(-2 * log(rnorm->u[i])) * sin(2 * M_PI_##F * rnorm->u[i+1]); \
    }

VSMC_DEFINE_NORMAL01(32, 24, float);

#if R123_USE_U01_DOUBLE
VSMC_DEFINE_NORMAL01(32, 53, float);
VSMC_DEFINE_NORMAL01(64, 53, double);
#endif

#endif // VSMC_UTILITY_RANDOM_NORMAL01_H
