#include <stdint.h>
#include <stddef.h>

#include <stdbool.h>
#include <fp.h>
#include <fp2.h>

void fp2_sqrt(fp2_t* x)
{
    fp_t t, two, sdelta, re, im, tmp;

    if (fp_is_zero(x->im)) {
        fp_copy(re, x->re);
        fp_sqrt(re);
        fp_sqr(t, re);
        fp_frommont(t, t);
        fp_frommont(tmp, x->re);
        if (fp_is_equal(tmp, t)) {
            memcpy((digit_t*)x->re, (digit_t*)re, NWORDS_FIELD*RADIX/8);
            return;
        } else {
            memcpy((digit_t*)x->im, (digit_t*)re, NWORDS_FIELD*RADIX/8);
            fp_set(x->re, 0);
            return;
        }
    }

    // sdelta = sqrt(re^2 + im^2)
    fp_sqr(tmp, x->re);
    fp_sqr(t, x->im);
    fp_add(tmp, tmp, t);
    fp_copy(sdelta, tmp);
    fp_sqrt(sdelta);   // (re^2 + im^2)^((p+1)/4)

    fp_set(two, 2);
    fp_tomont(two, two);

    fp_add(re, x->re, sdelta);
    fp_mul(tmp, re, two);    // 2(x->re+sdelta)
    fp_exp3div4(t, tmp);

    fp_mul(re, t, re);       // (x->re+sdelta) * (2(x->re+sdelta))^((p-3)/4)
    fp_mul(im, t, x->im);    //          x->im * (2(x->re+sdelta))^((p-3)/4)

    fp_mul(t, re, two);      // (2(x->re+sdelta))^((p+1)/4)
    fp_sqr(t, t);
    fp_frommont(t, t);
    fp_frommont(tmp, tmp);
    if (fp_is_equal(tmp, t)) {
        memcpy((digit_t*)x->re, (digit_t*)re, NWORDS_FIELD*RADIX/8);
        memcpy((digit_t*)x->im, (digit_t*)im, NWORDS_FIELD*RADIX/8);
    } else {
#if defined(PRIME_P248)
        memcpy((digit_t*)x->re, (digit_t*)im, NWORDS_FIELD*RADIX/8);
        fp_neg(re, re);
        memcpy((digit_t*)x->im, (digit_t*)re, NWORDS_FIELD*RADIX/8);
#else
        if (fp_is_square(x->im)) {
            memcpy((digit_t*)x->im, (digit_t*)re, NWORDS_FIELD*RADIX/8);
            fp_neg(im, im);
            memcpy((digit_t*)x->re, (digit_t*)im, NWORDS_FIELD*RADIX/8);
        } else {
            memcpy((digit_t*)x->re, (digit_t*)im, NWORDS_FIELD*RADIX/8);
            fp_neg(re, re);
            memcpy((digit_t*)x->im, (digit_t*)re, NWORDS_FIELD*RADIX/8);
        }
#endif
    }
}