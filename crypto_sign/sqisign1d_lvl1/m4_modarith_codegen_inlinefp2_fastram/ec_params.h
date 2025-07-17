#ifndef EC_PARAMS_H
#define EC_PARAMS_H

#include <tutil.h>
#include <fp_constants.h>

#define POWER_OF_2 75
#define POWER_OF_3 36
#define POWER_OF_2_SECPAR 128
#define DLOG_SCALAR_BITS 37

#define scaled 1
#define gap 83

#define P_LEN 9
#define M_LEN 19

extern const digit_t p_plus_minus_bitlength[P_LEN + M_LEN];

extern const digit_t STRATEGY4[];
extern const digit_t STRATEGY4_SMART[];
extern const digit_t STRATEGY4_CHAL[];

extern const digit_t sizeI[];
extern const digit_t sizeJ[];
extern const digit_t sizeK[];

#define sI_max 28
#define sJ_max 22
#define sK_max 41

#define ceil_log_sI_max 5
#define ceil_log_sJ_max 5

extern const digit_t TWOpFm1[NWORDS_ORDER];
extern const digit_t THREEpE[NWORDS_ORDER];
extern const digit_t THREEpFdiv2[NWORDS_ORDER];
extern const digit_t p_cofactor_for_2f[NWORDS_ORDER];
extern const digit_t p_cofactor_for_3g[NWORDS_ORDER];
extern const digit_t p_cofactor_for_6fg[NWORDS_ORDER];

#define P_COFACTOR_FOR_2F_BITLENGTH 179
#define P_COFACTOR_FOR_3G_BITLENGTH 197
#define P_COFACTOR_FOR_6FG_BITLENGTH 122

// differential addition chains
extern const digit_t DACS[28];
extern const int DAC_LEN[28];

#define NONRES_LEN 128
//Values of k for which 1+ik is a non-square
extern const digit_t NONRES[NONRES_LEN];
//Values of k for which 2+ik is a square but 1+ik is not
extern const digit_t SMARTZ[NONRES_LEN];

#endif
