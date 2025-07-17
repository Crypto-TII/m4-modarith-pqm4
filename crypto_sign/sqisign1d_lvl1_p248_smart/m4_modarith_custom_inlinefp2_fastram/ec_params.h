#ifndef EC_PARAMS_H
#define EC_PARAMS_H

#include <tutil.h>
#include <fp_constants.h>

#define POWER_OF_2 248
#define POWER_OF_3 0
#define POWER_OF_2_SECPAR 128
#define DLOG_SCALAR_BITS 124

#define scaled 1
#define gap 83

#define P_LEN 1
#define M_LEN 1

extern const digit_t p_plus_minus_bitlength[P_LEN + M_LEN];

extern const digit_t STRATEGY4[];
extern const digit_t STRATEGY4_SMART[];
extern const digit_t STRATEGY4_CHAL[];

extern const digit_t sizeI[];
extern const digit_t sizeJ[];
extern const digit_t sizeK[];

#define sI_max 1
#define sJ_max 1
#define sK_max 41

#define ceil_log_sI_max 0
#define ceil_log_sJ_max 0

extern const digit_t TWOpFm1[NWORDS_ORDER];
extern const digit_t THREEpE[NWORDS_ORDER];
extern const digit_t THREEpFdiv2[NWORDS_ORDER];
extern const digit_t p_cofactor_for_2f[NWORDS_ORDER];
extern const digit_t p_cofactor_for_3g[NWORDS_ORDER];
extern const digit_t p_cofactor_for_6fg[NWORDS_ORDER];

#define P_COFACTOR_FOR_2F_BITLENGTH 3
#define P_COFACTOR_FOR_3G_BITLENGTH 251
#define P_COFACTOR_FOR_6FG_BITLENGTH 3

// differential addition chains
extern const digit_t DACS[2];
extern const int DAC_LEN[2];

#define NONRES_LEN 128
//Values of k for which 1+ik is a non-square
extern const digit_t NONRES[NONRES_LEN];
//Values of k for which 2+ik is a square but 1+ik is not
extern const digit_t SMARTZ[NONRES_LEN];

#endif
