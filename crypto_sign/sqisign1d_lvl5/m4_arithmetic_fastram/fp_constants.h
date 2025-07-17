#ifndef FP_CONSTANTS_H
#define FP_CONSTANTS_H

#define PRIME_P318233

#if 0
#elif 8*DIGIT_LEN == 16

#define NWORDS_FIELD 32
#define NWORDS_ORDER 32

#elif 8*DIGIT_LEN == 32

#if defined(ARITH_REF) || defined(ARITH_M4) || defined(ARITH_BROADWELL)
#define NWORDS_FIELD 16
#elif defined(ARITH_MIKE)
#define NWORDS_FIELD 18
#endif
#define NWORDS_ORDER 16

#elif 8*DIGIT_LEN == 64

#if defined(ARITH_REF) || defined(ARITH_BROADWELL)
#define NWORDS_FIELD 8
#elif defined(ARITH_MIKE)
#define NWORDS_FIELD 9
#endif
#define NWORDS_ORDER 8

#endif

#define BITS 512
#define LOG2P 502

#endif
