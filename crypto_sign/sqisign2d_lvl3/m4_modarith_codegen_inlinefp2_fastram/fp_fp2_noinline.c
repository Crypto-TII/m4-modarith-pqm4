#include <fp.h>
#include <mp.h>
#include <inttypes.h>
#include <encoded_sizes.h>
#include <fp2.h>

void
fp2_inv(fp2_t *x)
{
    fp_t t0, t1;

    fp_sqr(&t0, &(x->re));
    fp_sqr(&t1, &(x->im));
    fp_add(&t0, &t0, &t1);
    fp_inv(&t0);
    fp_mul(&(x->re), &(x->re), &t0);
    fp_mul(&(x->im), &(x->im), &t0);
    fp_neg(&(x->im), &(x->im));
}

uint32_t
fp2_is_square(const fp2_t *x)
{
    fp_t t0, t1;

    fp_sqr(&t0, &(x->re));
    fp_sqr(&t1, &(x->im));
    fp_add(&t0, &t0, &t1);

    return fp_is_square(&t0);
}

void
fp2_sqrt(fp2_t *a)
{
    fp_t x0, x1, t0, t1;

    /* From "Optimized One-Dimensional SQIsign Verification on Intel and
     * Cortex-M4" by Aardal et al: https://eprint.iacr.org/2024/1563 */

    // x0 = \delta = sqrt(a0^2 + a1^2).
    fp_sqr(&x0, &(a->re));
    fp_sqr(&x1, &(a->im));
    fp_add(&x0, &x0, &x1);
    fp_sqrt(&x0);
    // If a1 = 0, there is a risk of \delta = -a0, which makes x0 = 0 below.
    // In that case, we restore the value \delta = a0.
    fp_select(&x0, &x0, &(a->re), fp_is_zero(&(a->im)));
    // x0 = \delta + a0, t0 = 2 * x0.
    fp_add(&x0, &x0, &(a->re));
    fp_add(&t0, &x0, &x0);

    // x1 = t0^(p-3)/4
    fp_exp3div4(&x1, &t0);

    // x0 = x0 * x1, x1 = x1 * a1, t1 = (2x0)^2.
    fp_mul(&x0, &x0, &x1);
    fp_mul(&x1, &x1, &(a->im));
    fp_add(&t1, &x0, &x0);
    fp_sqr(&t1, &t1);
    // If t1 = t0, return x0 + x1*i, otherwise x1 - x0*i.
    fp_sub(&t0, &t0, &t1);
    uint32_t f = fp_is_zero(&t0);
    fp_neg(&t1, &x0);
    fp_copy(&t0, &x1);
    fp_select(&t0, &t0, &x0, f);
    fp_select(&t1, &t1, &x1, f);

    // Check if t0 is zero
    uint32_t t0_is_zero = fp_is_zero(&t0);

    // Check whether t0, t1 are odd
    // Note: we encode to ensure canonical representation
    uint8_t tmp_bytes[FP_ENCODED_BYTES];
    fp_encode(tmp_bytes, &t0);
    uint32_t t0_is_odd = -((uint32_t)tmp_bytes[0] & 1);
    fp_encode(tmp_bytes, &t1);
    uint32_t t1_is_odd = -((uint32_t)tmp_bytes[0] & 1);

    // We negate the output if:
    // t0 is odd, or
    // t0 is zero and t1 is odd
    uint32_t negate_output = t0_is_odd | (t0_is_zero & t1_is_odd);
    fp_neg(&x0, &t0);
    fp_select(&(a->re), &t0, &x0, negate_output);
    fp_neg(&x0, &t1);
    fp_select(&(a->im), &t1, &x0, negate_output);
}

void
fp2_batched_inv(fp2_t *x, int len)
{
    fp2_t t1[len], t2[len];
    fp2_t inverse;

    // x = x0,...,xn
    // t1 = x0, x0*x1, ... ,x0 * x1 * ... * xn
    fp2_copy(&t1[0], &x[0]);
    for (int i = 1; i < len; i++) {
        fp2_mul(&t1[i], &t1[i - 1], &x[i]);
    }

    // inverse = 1/ (x0 * x1 * ... * xn)
    fp2_copy(&inverse, &t1[len - 1]);
    fp2_inv(&inverse);

    fp2_copy(&t2[0], &inverse);
    // t2 = 1/ (x0 * x1 * ... * xn), 1/ (x0 * x1 * ... * x(n-1)) , ... , 1/xO
    for (int i = 1; i < len; i++) {
        fp2_mul(&t2[i], &t2[i - 1], &x[len - i]);
    }

    fp2_copy(&x[0], &t2[len - 1]);

    for (int i = 1; i < len; i++) {
        fp2_mul(&x[i], &t1[i - 1], &t2[len - i - 1]);
    }
}