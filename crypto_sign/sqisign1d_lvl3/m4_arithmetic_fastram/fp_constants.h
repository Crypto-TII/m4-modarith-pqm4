#ifndef FP_CONSTANTS_H
#define FP_CONSTANTS_H

#define PRIME_P47441

#if 0
#elif 8*DIGIT_LEN == 16

#define NWORDS_FIELD 24
#define NWORDS_ORDER 24

#elif 8*DIGIT_LEN == 32

#if defined(ARITH_REF) || defined(ARITH_M4) || defined(ARITH_BROADWELL)
#define NWORDS_FIELD 12
#elif defined(ARITH_MIKE)
#define NWORDS_FIELD 14
#endif
#define NWORDS_ORDER 12

#elif 8*DIGIT_LEN == 64

#if defined(ARITH_REF) || defined(ARITH_BROADWELL)
#define NWORDS_FIELD 6
#elif defined(ARITH_MIKE)
#define NWORDS_FIELD 7
#endif
#define NWORDS_ORDER 6

#endif

#define BITS 384
#define LOG2P 378

#endif
