#ifndef FP_CONSTANTS_H
#define FP_CONSTANTS_H

#define PRIME_P248

#if 0
#elif 8*DIGIT_LEN == 16

#define NWORDS_FIELD 16
#define NWORDS_ORDER 16

#elif 8*DIGIT_LEN == 32

#if defined(ARITH_REF) || defined(ARITH_M4) || defined(ARITH_BROADWELL)
#define NWORDS_FIELD 8
#elif defined(ARITH_MIKE)
#define NWORDS_FIELD 9
#endif
#define NWORDS_ORDER 8

#elif 8*DIGIT_LEN == 64

#if defined(ARITH_REF) || defined(ARITH_BROADWELL)
#define NWORDS_FIELD 4
#elif defined(ARITH_MIKE)
#define NWORDS_FIELD 5
#endif
#define NWORDS_ORDER 4

#endif

#define BITS 256
#define LOG2P 251

#endif
