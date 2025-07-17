#include <ec_params.h>

const digit_t p_plus_minus_bitlength[P_LEN + M_LEN] =
	{2, 3, 4, 6, 8, 9, 10, 3, 4, 7, 7, 8, 8, 8, 8, 9, 10, 11, 12, 12, 13, 13, 13, 13, 14, 15, 16, 16};

const digit_t STRATEGY4[] =
	{20, 12, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 8, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 2, 1, 1};
const digit_t STRATEGY4_SMART[] =
	{21, 12, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1};
const digit_t STRATEGY4_CHAL[] =
	{38, 26, 15, 8, 4, 2, 1, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 11, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 4, 3, 2, 1, 1, 1, 1, 2, 1, 1, 17, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 8, 4, 2, 1, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1};

const digit_t sizeI[] =
	{0, 1, 2, 3, 6, 14, 14, 1, 3, 5, 5, 7, 8, 8, 10, 11, 14, 19, 26, 30, 35, 40, 44, 44, 52, 88, 114, 114};
const digit_t sizeJ[] =
	{0, 1, 1, 3, 6, 9, 13, 1, 1, 4, 5, 6, 7, 7, 6, 10, 10, 16, 23, 28, 32, 32, 32, 33, 45, 76, 87, 104};
const digit_t sizeK[] =
	{1, 1, 1, 5, 6, 2, 16, 0, 0, 4, 6, 2, 4, 7, 0, 1, 4, 6, 0, 5, 18, 13, 30, 2, 18, 12, 3, 8};

#if 0
#elif 8*DIGIT_LEN == 16
const digit_t TWOpFm1[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t THREEpE[NWORDS_ORDER] = {0x3289, 0xf310, 0x3fce, 0x3b, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t THREEpFdiv2[NWORDS_ORDER] = {0xe6a8, 0x388c, 0x41b9, 0xac6f, 0xc632, 0x3ca5, 0x6db, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_2f[NWORDS_ORDER] = {0xba61, 0x2630, 0x5234, 0x9ab7, 0x7b34, 0xe391, 0x5ba8, 0x4857, 0x1705, 0x32de, 0x856b, 0x22e8, 0x3025, 0x6357, 0x38d1, 0x5584, 0xb777, 0x1ef, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_3g[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3022, 0x97e5, 0xe081, 0x13cf, 0xedda, 0xee54, 0x9f3f, 0xd920, 0xe309, 0xc6bf, 0x484c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_6fg[NWORDS_ORDER] = {0x9811, 0xcbf2, 0xf040, 0x9e7, 0x76ed, 0xf72a, 0x4f9f, 0xec90, 0xf184, 0x635f, 0x2426, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
#elif 8*DIGIT_LEN == 32
const digit_t TWOpFm1[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t THREEpE[NWORDS_ORDER] = {0xf3103289, 0x3b3fce, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t THREEpFdiv2[NWORDS_ORDER] = {0x388ce6a8, 0xac6f41b9, 0x3ca5c632, 0x6db, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_2f[NWORDS_ORDER] = {0x2630ba61, 0x9ab75234, 0xe3917b34, 0x48575ba8, 0x32de1705, 0x22e8856b, 0x63573025, 0x558438d1, 0x1efb777, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_3g[NWORDS_ORDER] = {0x0, 0x0, 0x0, 0x97e53022, 0x13cfe081, 0xee54edda, 0xd9209f3f, 0xc6bfe309, 0x484c, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_6fg[NWORDS_ORDER] = {0xcbf29811, 0x9e7f040, 0xf72a76ed, 0xec904f9f, 0x635ff184, 0x2426, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
#elif 8*DIGIT_LEN == 64
const digit_t TWOpFm1[NWORDS_ORDER] = {0x0, 0x100000000, 0x0, 0x0, 0x0, 0x0};
const digit_t THREEpE[NWORDS_ORDER] = {0x3b3fcef3103289, 0x0, 0x0, 0x0, 0x0, 0x0};
const digit_t THREEpFdiv2[NWORDS_ORDER] = {0xac6f41b9388ce6a8, 0x6db3ca5c632, 0x0, 0x0, 0x0, 0x0};
const digit_t p_cofactor_for_2f[NWORDS_ORDER] = {0x9ab752342630ba61, 0x48575ba8e3917b34, 0x22e8856b32de1705, 0x558438d163573025, 0x1efb777, 0x0};
const digit_t p_cofactor_for_3g[NWORDS_ORDER] = {0x0, 0x97e5302200000000, 0xee54edda13cfe081, 0xc6bfe309d9209f3f, 0x484c, 0x0};
const digit_t p_cofactor_for_6fg[NWORDS_ORDER] = {0x9e7f040cbf29811, 0xec904f9ff72a76ed, 0x2426635ff184, 0x0, 0x0, 0x0};
#endif

// differential addition chains
const digit_t DACS[28] = {0, 2, 4, 32, 384, 3346, 6920, 0, 0, 0, 464, 192, 0, 682, 832, 1282, 2400, 12628, 26656, 2176, 17474, 107088, 70336, 98448, 148097, 545026, 1050964, 333056, };
const int DAC_LEN[28] = {0, 2, 3, 6, 9, 12, 13, 1, 3, 7, 9, 9, 9, 10, 10, 11, 12, 14, 15, 15, 16, 17, 17, 17, 18, 20, 21, 21, };

//Values of k for which 1+ik is a non-square
const digit_t NONRES[NONRES_LEN] = { 6, 10, 11, 12, 14, 16, 17, 20, 22, 25, 26, 29, 31, 40, 43, 44, 45, 46, 50, 51, 58, 61, 62, 67, 68, 70, 72, 78, 80, 84, 85, 87, 88, 90, 91, 96, 97, 99, 102, 104, 108, 115, 119, 124, 125, 126, 127, 128, 130, 135, 137, 138, 139, 140, 141, 142, 143, 150, 151, 153, 154, 155, 157, 160, 161, 162, 163, 166, 167, 170, 176, 178, 179, 180, 183, 190, 192, 193, 195, 196, 198, 202, 204, 211, 212, 215, 219, 222, 223, 228, 232, 233, 234, 235, 240, 241, 244, 246, 248, 253, 254, 255, 257, 258, 259, 260, 261, 264, 265, 266, 267, 269, 271, 272, 274, 277, 278, 279, 281, 284, 286, 287, 288, 289, 291, 292, 293, 294 };
//Values of k for which 2+ik is a square but 1+ik is not
const digit_t SMARTZ[NONRES_LEN] = { 6, 10, 11, 14, 16, 26, 29, 31, 43, 45, 46, 68, 70, 72, 78, 84, 96, 97, 104, 108, 115, 119, 125, 126, 127, 128, 130, 135, 137, 138, 139, 142, 143, 150, 154, 157, 162, 166, 167, 178, 183, 190, 193, 195, 196, 202, 211, 212, 215, 219, 222, 228, 232, 233, 234, 235, 240, 241, 244, 246, 257, 258, 259, 261, 264, 265, 266, 267, 269, 272, 277, 287, 288, 292, 294, 295, 296, 304, 307, 312, 316, 328, 329, 330, 331, 336, 338, 342, 350, 354, 355, 357, 362, 370, 376, 382, 387, 388, 397, 401, 402, 407, 410, 412, 413, 417, 418, 421, 426, 427, 434, 435, 440, 441, 442, 445, 447, 448, 451, 452, 453, 462, 463, 472, 473, 474, 477, 478 };
