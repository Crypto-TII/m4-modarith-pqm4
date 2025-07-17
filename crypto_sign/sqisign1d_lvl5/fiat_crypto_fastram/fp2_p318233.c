#include <fp2.h>

/* Arithmetic modulo X^2 + 1 */

fp2_t fp2_non_residue()
{ // 5 + 2i is a quadratic non-residue for p318233
    fp_t one = {0};
    fp2_t res;

    one[0] = 1;
    fp_tomont(one, one);
    fp_add(res.im, one, one);
    fp_add(res.re, res.im, res.im);
    fp_add(res.re, res.re, one);
    return res;
}
