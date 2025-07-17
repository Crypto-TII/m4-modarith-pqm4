#include <ec_params.h>

const digit_t p_plus_minus_bitlength[P_LEN + M_LEN] =
	{3, 2};

const digit_t STRATEGY4[] =
	{53, 32, 17, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 8, 4, 2, 1, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 15, 8, 4, 2, 1, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 21, 15, 8, 4, 2, 1, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1};
const digit_t STRATEGY4_SMART[] =
	{54, 32, 17, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 8, 4, 2, 1, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 15, 8, 4, 2, 1, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 22, 15, 8, 4, 2, 1, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 9, 5, 4, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1};
const digit_t STRATEGY4_CHAL[] =
	{27, 16, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 7, 4, 2, 1, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 12, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1};

const digit_t sizeI[] =
	{1, 1};
const digit_t sizeJ[] =
	{1, 0};
const digit_t sizeK[] =
	{0, 1};

#if 0
#elif 8*DIGIT_LEN == 16
const digit_t TWOpFm1[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80};
const digit_t THREEpE[NWORDS_ORDER] = {0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t THREEpFdiv2[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_2f[NWORDS_ORDER] = {0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_3g[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x500};
const digit_t p_cofactor_for_6fg[NWORDS_ORDER] = {0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
#elif 8*DIGIT_LEN == 32
const digit_t TWOpFm1[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x800000};
const digit_t THREEpE[NWORDS_ORDER] = {0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t THREEpFdiv2[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_2f[NWORDS_ORDER] = {0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_3g[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5000000};
const digit_t p_cofactor_for_6fg[NWORDS_ORDER] = {0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
#elif 8*DIGIT_LEN == 64
const digit_t TWOpFm1[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x80000000000000};
const digit_t THREEpE[NWORDS_ORDER] = {0x1, 0x0, 0x0, 0x0};
const digit_t THREEpFdiv2[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_2f[NWORDS_ORDER] = {0x5, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_3g[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x500000000000000};
const digit_t p_cofactor_for_6fg[NWORDS_ORDER] = {0x5, 0x0, 0x0, 0x0};
#endif

// differential addition chains
const digit_t DACS[2] = {0, 0, };
const int DAC_LEN[2] = {1, 0, };

//Values of k for which 1+ik is a non-square
const digit_t NONRES[NONRES_LEN] = { 5, 8, 11, 12, 14, 16, 17, 18, 20, 21, 24, 26, 27, 29, 31, 33, 36, 37, 39, 40, 44, 45, 47, 48, 49, 50, 53, 54, 55, 56, 57, 58, 59, 61, 64, 65, 67, 69, 70, 72, 73, 74, 75, 76, 78, 79, 81, 83, 85, 88, 95, 96, 97, 99, 103, 104, 108, 109, 110, 111, 116, 118, 119, 120, 121, 122, 123, 125, 126, 127, 128, 130, 135, 137, 139, 141, 142, 144, 151, 154, 155, 157, 158, 159, 161, 162, 164, 172, 173, 175, 176, 178, 179, 183, 184, 185, 187, 189, 190, 191, 192, 195, 197, 198, 200, 201, 203, 204, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 219, 222, 226, 228, 230, 231, 232, 235, 237, 241 };
//Values of k for which 2+ik is a square but 1+ik is not
const digit_t SMARTZ[NONRES_LEN] = { 8, 11, 12, 14, 18, 20, 26, 27, 29, 37, 44, 47, 50, 56, 57, 59, 61, 64, 70, 73, 75, 76, 99, 103, 104, 119, 120, 123, 125, 126, 127, 135, 137, 139, 141, 142, 151, 154, 155, 157, 164, 172, 173, 175, 178, 179, 183, 184, 185, 187, 195, 197, 200, 201, 204, 210, 211, 212, 213, 214, 219, 226, 228, 230, 231, 237, 243, 245, 248, 251, 257, 258, 262, 271, 272, 277, 280, 283, 286, 289, 290, 291, 292, 294, 295, 304, 309, 312, 315, 319, 332, 334, 335, 336, 337, 338, 340, 343, 349, 354, 357, 361, 364, 371, 373, 381, 386, 388, 395, 398, 403, 415, 431, 433, 436, 442, 446, 453, 454, 458, 459, 463, 466, 468, 472, 477, 480, 483 };
