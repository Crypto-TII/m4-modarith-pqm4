#include <fp.h>
#include <mp.h>
#include <inttypes.h>
#include <encoded_sizes.h>
#include <fp2.h>
#include "code_MFP500_mont.c"

const digit_t p[NWORDS_FIELD] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0x1afffff };

const digit_t ZERO[NWORDS_FIELD] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const digit_t ONE[NWORDS_FIELD] = { 0x00000097, 0x00000000, 0x00000000, 0x00000000,
      0x00000000, 0x00000000, 0x00000000, 0x00000000,
      0x00000000, 0x00000000, 0x00000000, 0x00000000,
      0x00000000, 0x00000000, 0x00000000, 0x01300000 };

static const digit_t TWO_INV[NWORDS_FIELD] = { 0x0000004b, 0x00000000, 0x00000000, 0x00000000,
                 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                 0x00000000, 0x00000000, 0x00000000, 0x01700000 };

static const digit_t THREE_INV[NWORDS_FIELD] = { 0x55555587, 0x55555555, 0x55555555, 0x55555555,
                   0x55555555, 0x55555555, 0x55555555, 0x55555555,
                   0x55555555, 0x55555555, 0x55555555, 0x55555555,
                   0x55555555, 0x55555555, 0x55555555, 0x01855555 };

// Montgomery representation of 2^1024
static const digit_t R2[NWORDS_FIELD] = { 0x5ed0f19a, 0xed097b42, 0xd097b425, 0x097b425e,
            0x97b425ed, 0x7b425ed0, 0xb425ed09, 0x425ed097,
            0x25ed097b, 0x5ed097b4, 0xed097b42, 0xd097b425,
            0x097b425e, 0x97b425ed, 0x7b425ed0, 0x0045ed09 };

static const digit_t PM1O3[NWORDS_FIELD] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
               0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
               0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
               0xffffffff, 0xffffffff, 0xffffffff, 0x11fffff };
      
void
fp_set_small(fp_t *x, const digit_t val)
{
    modint_MFP500_ct((int)val, *x);
}

NOINLINE void
fp_mul_small(fp_t *x, const fp_t *a, const uint32_t val)
{
    modmli_MFP500_ct(*a, (int)val, *x);
}

void
fp_set_zero(fp_t *x)
{
    modzer_MFP500_ct(*x);
}

void
fp_set_one(fp_t *x)
{
    modone_MFP500_ct(*x);
}

uint32_t
fp_is_equal(const fp_t *a, const fp_t *b)
{
    return -(uint32_t)modcmp_MFP500_ct(*a, *b);
}

uint32_t
fp_is_zero(const fp_t *a)
{
    return -(uint32_t)modis0_MFP500_ct(*a);
}

void
fp_copy(fp_t *out, const fp_t *a)
{
    modcpy_MFP500_ct(*a, *out);
}

void
fp_cswap(fp_t *a, fp_t *b, uint32_t ctl)
{
    modcsw_MFP500_ct((int)(ctl & 0x1), *a, *b);
}

inline void
fp_add(fp_t *out, const fp_t *a, const fp_t *b)
{
    modadd_MFP500_ct(*a, *b, *out);
}

inline void
fp_sub(fp_t *out, const fp_t *a, const fp_t *b)
{
    modsub_MFP500_ct(*a, *b, *out);
}

void
fp_neg(fp_t *out, const fp_t *a)
{
    modneg_MFP500_ct(*a, *out);
}

void
fp_sqr(fp_t *out, const fp_t *a)
{
    modsqr_MFP500_ct(*a, *out);
}

void
fp_mul(fp_t *out, const fp_t *a, const fp_t *b)
{
    modmul_MFP500_ct(*a, *b, *out);
}

void
fp_inv(fp_t *x)
{
    modinv_MFP500_ct(*x, NULL, *x);
}

uint32_t
fp_is_square(const fp_t *a)
{
    return -(uint32_t)modqr_MFP500_ct(NULL, *a);
}

void
fp_sqrt(fp_t *a)
{
    modsqrt_MFP500_ct(*a, NULL, *a);
}

#if 1
void
fp_half(fp_t *out, const fp_t *a)
{
    uint32_t ctl = ((*a)[0] & 1) - 1; // 0 if odd, 0xffffffff if even
    modcpy_MFP500_ct(*a, *out);
    modshr_MFP500_ct(1, *out);
    (*out)[15] += 0x00d80000 & ~ctl;
}
#else
void
fp_half(fp_t *out, const fp_t *a)
{
    modmul_MFP500_ct(TWO_INV, *a, *out);
}
#endif

void
fp_exp3div4(fp_t *out, const fp_t *a)
{
    modpro_MFP500_ct(*a, *out);
}

#if 0
void
fp_div3(fp_t *d, const fp_t *a)
{
	digit_t c0, c1, f[2];
	fp_t t;

#if RADIX == 32
	const digit_t MAGIC = 0xAAAAAAAB; //3^-1 mod 2^32
#elif RADIX == 64
	const digit_t MAGIC = 0xAAAAAAAAAAAAAAAB; //3^-1 mod 2^64
#endif

	/* From "Efficient Multiplication in Finite Field Extensions of Degree 5"
	 * by El Mrabet, Guillevic and Ionica at ASIACRYPT 2011. */

	MUL(f, (*a)[NWORDS_FIELD - 1], MAGIC);
	t[NWORDS_FIELD - 1] = f[1] >> 1;
	c1 = (*a)[NWORDS_FIELD - 1] - 3 * t[NWORDS_FIELD - 1];

	for (size_t i = NWORDS_FIELD - 1; i > 0; i--) {
		c0 = c1;
		MUL(f, (*a)[i - 1], MAGIC);
		t[i - 1] = f[1] >> 1;
		c1 = c0 + (*a)[i - 1] - 3 * t[i - 1];
		t[i - 1] += c0 * ((MAGIC - 1) >> 1);
		f[0] = ((c1 >> 1) & c1); /* c1 == 3 */
		f[1] = ((c1 >> 2) & !(c1 & 0x11)); /* c1 == 4 */
		f[0] |= f[1];
		t[i - 1] += f[0];
		c1 = c1 - 3 * f[0];
	}
	fp_copy(d, &t);
	fp_sub(&t, d, &PM1O3);
	fp_select(d, d, &t, -((c1 & 1) | (c1 >> 1))); // c1 >= 1
	fp_sub(&t, d, &PM1O3);
	fp_select(d, d, &t, -(c1 == 2));
}
#else
void
fp_div3(fp_t *out, const fp_t *a)
{
    modmul_MFP500_ct(THREE_INV, *a, *out);
}
#endif

// Little-endian encoding of a 32-bit integer.
static inline void
enc32le(void *dst, uint32_t x)
{
    memcpy(dst, &x, sizeof(x));
}

// Little-endian decoding of a 32-bit integer.
static inline uint32_t
dec32le(const void *src)
{
    return *(uint32_t *)src;
}

void
fp_encode(void *dst, const fp_t *a)
{
    uint8_t *buf = dst;
    spint c[NWORDS_FIELD];
    redc_MFP500_ct(*a, c);

    for (int i = 0; i < NWORDS_FIELD; i++) {
        enc32le(buf + i * 4, c[i]);
    }
}

uint32_t
fp_decode(fp_t *d, const void *src)
{
    // Modified version of modimp()
    int i;
    spint res;
    const unsigned char *b = src;
    for (i = 0; i < NWORDS_FIELD; i++) {
        (*d)[i] = 0;
    }
    for (i = BITS / 8 - 1; i >= 0; i--) {
        modshl_MFP500_ct(8, *d);
        (*d)[0] += (spint)b[i];
    }
    res = (spint)-modfsb_MFP500_ct(*d);
    nres_MFP500_ct(*d, *d);
    // If the value was canonical then res = -1; otherwise, res = 0
    for (i = 0; i < NWORDS_FIELD; i++) {
        (*d)[i] &= res;
    }
    return (uint32_t)res;
}

void
fp_decode_reduce(fp_t *d, const void *src, size_t len)
{
    const uint8_t *buf = src;

    for (int i = 0; i < NWORDS_FIELD; i++) {
        (*d)[i] = dec32le(buf + i * 4);
    }

    nres_MFP500_ct(*d, *d);
}

/*
 * If ctl == 0x00000000, then *d is set to a0
 * If ctl == 0xFFFFFFFF, then *d is set to a1
 * ctl MUST be either 0x00000000 or 0xFFFFFFFF.
 */
void
fp_select(fp_t *d, const fp_t *a0, const fp_t *a1, uint32_t ctl)
{
    digit_t cw = (int32_t)ctl;
    for (unsigned int i = 0; i < NWORDS_FIELD; i++) {
        (*d)[i] = (*a0)[i] ^ (cw & ((*a0)[i] ^ (*a1)[i]));
    }
}

/* Arithmetic modulo X^2 + 1 */

void
fp2_set_small(fp2_t *x, const digit_t val)
{
    fp_set_small(&(x->re), val);
    fp_set_zero(&(x->im));
}

void
fp2_mul_small(fp2_t *x, const fp2_t *y, uint32_t n)
{
    fp_mul_small(&x->re, &y->re, n);
    fp_mul_small(&x->im, &y->im, n);
}

void
fp2_set_one(fp2_t *x)
{
    fp_set_one(&(x->re));
    fp_set_zero(&(x->im));
}

void
fp2_set_zero(fp2_t *x)
{
    fp_set_zero(&(x->re));
    fp_set_zero(&(x->im));
}

// Is a GF(p^2) element zero?
// Returns 0xFF...FF (true) if a=0, 0 (false) otherwise
uint32_t
fp2_is_zero(const fp2_t *a)
{
    return fp_is_zero(&(a->re)) & fp_is_zero(&(a->im));
}

// Compare two GF(p^2) elements in constant time
// Returns 0xFF...FF (true) if a=b, 0 (false) otherwise
uint32_t
fp2_is_equal(const fp2_t *a, const fp2_t *b)
{
    return fp_is_equal(&(a->re), &(b->re)) & fp_is_equal(&(a->im), &(b->im));
}

// Is a GF(p^2) element one?
// Returns 0xFF...FF (true) if a=1, 0 (false) otherwise
uint32_t
fp2_is_one(const fp2_t *a)
{
    return fp_is_equal(&(a->re), &ONE) & fp_is_zero(&(a->im));
}

void
fp2_copy(fp2_t *x, const fp2_t *y)
{
    fp_copy(&(x->re), &(y->re));
    fp_copy(&(x->im), &(y->im));
}

void
fp2_add(fp2_t *x, const fp2_t *y, const fp2_t *z)
{
    fp_add(&(x->re), &(y->re), &(z->re));
    fp_add(&(x->im), &(y->im), &(z->im));
}

void
fp2_add_one(fp2_t *x, const fp2_t *y)
{
    fp_add(&x->re, &y->re, &ONE);
    fp_copy(&x->im, &y->im);
}

void
fp2_sub(fp2_t *x, const fp2_t *y, const fp2_t *z)
{
    fp_sub(&(x->re), &(y->re), &(z->re));
    fp_sub(&(x->im), &(y->im), &(z->im));
}

void
fp2_neg(fp2_t *x, const fp2_t *y)
{
    fp_neg(&(x->re), &(y->re));
    fp_neg(&(x->im), &(y->im));
}

NOINLINE void
fp2_mul(fp2_t *x, const fp2_t *y, const fp2_t *z)
{
    fp_t t0, t1;

    fp_add(&t0, &(y->re), &(y->im));
    fp_add(&t1, &(z->re), &(z->im));
    fp_mul(&t0, &t0, &t1);
    fp_mul(&t1, &(y->im), &(z->im));
    fp_mul(&(x->re), &(y->re), &(z->re));
    fp_sub(&(x->im), &t0, &t1);
    fp_sub(&(x->im), &(x->im), &(x->re));
    fp_sub(&(x->re), &(x->re), &t1);
}

NOINLINE void
fp2_sqr(fp2_t *x, const fp2_t *y)
{
    fp_t sum, diff;

    fp_add(&sum, &(y->re), &(y->im));
    fp_sub(&diff, &(y->re), &(y->im));
    fp_mul(&(x->im), &(y->re), &(y->im));
    fp_add(&(x->im), &(x->im), &(x->im));
    fp_mul(&(x->re), &sum, &diff);
}

uint32_t
fp2_sqrt_verify(fp2_t *a)
{
    fp2_t t0, t1;

    fp2_copy(&t0, a);
    fp2_sqrt(a);
    fp2_sqr(&t1, a);

    return (fp2_is_equal(&t0, &t1));
}

void
fp2_half(fp2_t *x, const fp2_t *y)
{
    fp_half(&(x->re), &(y->re));
    fp_half(&(x->im), &(y->im));
}

// exponentiation using square and multiply
// Warning!! Not constant time!
void
fp2_pow_vartime(fp2_t *out, const fp2_t *x, const digit_t *exp, const int size)
{
    fp2_t acc;
    digit_t bit;

    fp2_copy(&acc, x);
    fp2_set_one(out);

    // Iterate over each word of exp
    for (int j = 0; j < size; j++) {
        // Iterate over each bit of the word
        for (int i = 0; i < RADIX; i++) {
            bit = (exp[j] >> i) & 1;
            if (bit == 1) {
                fp2_mul(out, out, &acc);
            }
            fp2_sqr(&acc, &acc);
        }
    }
}

void
fp2_print(const char *name, const fp2_t *a)
{
    printf("%s0x", name);

    uint8_t buf[FP_ENCODED_BYTES];
    fp_encode(&buf, &a->re); // Encoding ensures canonical rep
    for (int i = 0; i < FP_ENCODED_BYTES; i++) {
        printf("%02x", buf[FP_ENCODED_BYTES - i - 1]);
    }

    printf(" + i*0x");

    fp_encode(&buf, &a->im);
    for (int i = 0; i < FP_ENCODED_BYTES; i++) {
        printf("%02x", buf[FP_ENCODED_BYTES - i - 1]);
    }
    printf("\n");
}

void
fp2_encode(void *dst, const fp2_t *a)
{
    uint8_t *buf = dst;
    fp_encode(buf, &(a->re));
    fp_encode(buf + FP_ENCODED_BYTES, &(a->im));
}

uint32_t
fp2_decode(fp2_t *d, const void *src)
{
    const uint8_t *buf = src;
    uint32_t re, im;

    re = fp_decode(&(d->re), buf);
    im = fp_decode(&(d->im), buf + FP_ENCODED_BYTES);
    return re & im;
}

void
fp2_select(fp2_t *d, const fp2_t *a0, const fp2_t *a1, uint32_t ctl)
{
    fp_select(&(d->re), &(a0->re), &(a1->re), ctl);
    fp_select(&(d->im), &(a0->im), &(a1->im), ctl);
}

void
fp2_cswap(fp2_t *a, fp2_t *b, uint32_t ctl)
{
    fp_cswap(&(a->re), &(b->re), ctl);
    fp_cswap(&(a->im), &(b->im), ctl);
}
