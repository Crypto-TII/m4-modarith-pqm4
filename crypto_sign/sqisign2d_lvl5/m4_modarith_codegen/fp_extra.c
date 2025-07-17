#include <fp.h>
#include <mp.h>
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

void
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

void
fp_add(fp_t *out, const fp_t *a, const fp_t *b)
{
    modadd_MFP500_ct(*a, *b, *out);
}

void
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
