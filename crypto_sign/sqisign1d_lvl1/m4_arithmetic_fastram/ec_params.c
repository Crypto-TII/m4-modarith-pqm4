#include <ec_params.h>

const digit_t p_plus_minus_bitlength[P_LEN + M_LEN] =
	{2, 5, 6, 7, 7, 8, 9, 10, 11, 3, 4, 4, 6, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 10, 11, 11, 11, 11};

const digit_t STRATEGY4[] =
	{15, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1};
const digit_t STRATEGY4_SMART[] =
	{16, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 7, 4, 2, 1, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1};
const digit_t STRATEGY4_CHAL[] =
	{27, 16, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 7, 4, 2, 1, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 12, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1};

const digit_t sizeI[] =
	{0, 2, 4, 6, 6, 7, 12, 14, 28, 1, 2, 3, 3, 5, 6, 6, 6, 6, 9, 8, 10, 12, 12, 12, 16, 16, 18, 22};
const digit_t sizeJ[] =
	{0, 2, 3, 4, 4, 7, 10, 13, 17, 1, 1, 1, 3, 4, 4, 4, 5, 5, 6, 7, 9, 8, 10, 12, 16, 16, 16, 22};
const digit_t sizeK[] =
	{1, 3, 5, 2, 6, 0, 5, 7, 4, 1, 1, 0, 0, 4, 0, 5, 5, 8, 3, 7, 11, 2, 9, 15, 4, 12, 20, 18};

#if 0
#elif 8*DIGIT_LEN == 16
const digit_t TWOpFm1[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x0, 0x400, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t THREEpE[NWORDS_ORDER] = {0x9149, 0x1717, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t THREEpFdiv2[NWORDS_ORDER] = {0xe368, 0x45c8, 0x9f23, 0x10a, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_2f[NWORDS_ORDER] = {0xaa29, 0xc926, 0xa593, 0x86e4, 0xe80e, 0xcb0, 0x74d5, 0x3186, 0x72bb, 0xc50d, 0x9c53, 0x6, 0x0, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_3g[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x0, 0xc800, 0xd9e, 0xdace, 0x74f9, 0x5001, 0x7f65, 0x5b43, 0x63a2, 0x19, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_6fg[NWORDS_ORDER] = {0xb3d9, 0x59c1, 0x9f3b, 0x2e, 0xecaa, 0x686f, 0x744b, 0x32c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
#elif 8*DIGIT_LEN == 32
const digit_t TWOpFm1[NWORDS_ORDER] = {0x0, 0x0, 0x400, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t THREEpE[NWORDS_ORDER] = {0x17179149, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t THREEpFdiv2[NWORDS_ORDER] = {0x45c8e368, 0x10a9f23, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_2f[NWORDS_ORDER] = {0xc926aa29, 0x86e4a593, 0xcb0e80e, 0x318674d5, 0xc50d72bb, 0x69c53, 0x0, 0x0};
const digit_t p_cofactor_for_3g[NWORDS_ORDER] = {0x0, 0x0, 0xd9ec800, 0x74f9dace, 0x7f655001, 0x63a25b43, 0x19, 0x0};
const digit_t p_cofactor_for_6fg[NWORDS_ORDER] = {0x59c1b3d9, 0x2e9f3b, 0x686fecaa, 0x32c744b, 0x0, 0x0, 0x0, 0x0};
#elif 8*DIGIT_LEN == 64
const digit_t TWOpFm1[NWORDS_ORDER] = {0x0, 0x400, 0x0, 0x0};
const digit_t THREEpE[NWORDS_ORDER] = {0x17179149, 0x0, 0x0, 0x0};
const digit_t THREEpFdiv2[NWORDS_ORDER] = {0x10a9f2345c8e368, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_2f[NWORDS_ORDER] = {0x86e4a593c926aa29, 0x318674d50cb0e80e, 0x69c53c50d72bb, 0x0};
const digit_t p_cofactor_for_3g[NWORDS_ORDER] = {0x0, 0x74f9dace0d9ec800, 0x63a25b437f655001, 0x19};
const digit_t p_cofactor_for_6fg[NWORDS_ORDER] = {0x2e9f3b59c1b3d9, 0x32c744b686fecaa, 0x0, 0x0};
#endif

// differential addition chains
const digit_t DACS[28] = {0, 24, 88, 164, 132, 785, 3210, 1288, 2208, 2, 4, 0, 48, 0, 192, 96, 64, 424, 708, 682, 1345, 1285, 544, 1304, 4418, 4353, 2564, 8256, };
const int DAC_LEN[28] = {0, 5, 7, 8, 8, 10, 12, 12, 14, 2, 3, 3, 6, 7, 8, 8, 8, 9, 10, 10, 11, 11, 11, 12, 13, 13, 13, 14, };

//Values of k for which 1+ik is a non-square
const digit_t NONRES[NONRES_LEN] = { 2, 3, 4, 8, 12, 16, 17, 20, 21, 22, 24, 25, 26, 28, 29, 33, 35, 36, 37, 38, 39, 40, 44, 46, 54, 55, 56, 57, 59, 64, 65, 66, 68, 73, 74, 76, 77, 79, 80, 82, 84, 86, 87, 90, 92, 93, 94, 96, 97, 100, 101, 102, 103, 105, 108, 110, 114, 115, 116, 117, 118, 119, 120, 121, 122, 124, 126, 128, 129, 130, 132, 133, 135, 136, 137, 142, 143, 144, 145, 146, 148, 149, 152, 153, 154, 155, 156, 157, 160, 161, 162, 168, 169, 170, 172, 174, 177, 178, 179, 180, 181, 182, 184, 189, 190, 191, 195, 199, 200, 201, 202, 204, 208, 210, 212, 213, 219, 220, 222, 225, 230, 231, 232, 233, 235, 236, 237, 241 };
//Values of k for which 2+ik is a square but 1+ik is not
const digit_t SMARTZ[NONRES_LEN] = { 2, 3, 12, 20, 22, 26, 28, 33, 35, 36, 38, 39, 46, 54, 59, 64, 68, 82, 84, 86, 87, 90, 93, 94, 96, 100, 102, 116, 117, 119, 120, 122, 124, 126, 135, 137, 142, 143, 144, 153, 155, 156, 157, 162, 170, 178, 179, 181, 182, 190, 201, 208, 212, 213, 219, 222, 235, 241, 246, 250, 259, 261, 262, 265, 268, 276, 277, 278, 280, 287, 293, 301, 305, 311, 313, 315, 318, 321, 323, 326, 327, 330, 333, 337, 346, 366, 367, 370, 383, 388, 392, 393, 394, 396, 401, 412, 414, 428, 429, 432, 433, 435, 436, 437, 442, 449, 451, 452, 454, 458, 459, 461, 477, 478, 479, 491, 496, 502, 503, 508, 510, 512, 520, 525, 527, 528, 547, 550 };
