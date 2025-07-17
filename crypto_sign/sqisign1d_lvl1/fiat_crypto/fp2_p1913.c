#include <fp2.h>

/* Arithmetic modulo X^2 + 1 */

fp2_t fp2_non_residue()
{ // 2 + i is a quadratic non-residue for p1913
    fp_t one = {0};
    fp2_t res;

    one[0] = 1;
    fp_tomont(one, one);
    fp_add(res.re, one, one);
    fp_copy(res.im, one);
    return res;
}
