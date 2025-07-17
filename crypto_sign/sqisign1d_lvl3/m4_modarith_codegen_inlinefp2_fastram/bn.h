#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <fp.h>

extern const digit_t p[NWORDS_FIELD];

#ifdef RADIX_32
typedef int32_t sidig_t;
#elif defined(RADIX_64)
typedef int64_t sidig_t;
#endif

#define SELECT(A, B, C)	((-(C) & ((A) ^ (B))) ^ (A))
#define SIGN(D)	(((digit_t)D) >> (RADIX - 1))
#define MASK(B)	((-(digit_t)((B) >= RADIX)) | (((digit_t)1 << ((B) % RADIX)) - 1))

static digit_t bn_addn_low(digit_t *c, const digit_t *a, const digit_t *b,
		size_t n) {
	register digit_t carry, c0, c1, r0, r1;

	carry = 0;
	for (size_t i = 0; i < n; i++, a++, b++, c++) {
		r0 = (*a) + (*b);
		c0 = (r0 < (*a));
		r1 = r0 + carry;
		c1 = (r1 < r0);
		carry = c0 | c1;
		(*c) = r1;
	}
	return carry;
}

static digit_t bn_muls_low(digit_t *c, const digit_t *a, digit_t sa, sidig_t digit,
		size_t n) {
	digit_t r[2] = { 0, 0 }, c0, c1, sign, sd = digit >> (RADIX - 1);

	sa = -sa;
	sign = sa ^ sd;
	digit = (digit ^ sd) - sd;

	MUL(r, a[0], (digit_t)digit);
	r[0] ^= sign;
	c[0] = r[0] - sign;
	c1 = (c[0] < r[0]);
	c0 = r[1];
	for (size_t i = 1; i < n; i++) {
		MUL(r, a[i], (digit_t)digit);
		r[0] += c0;
		c0 = r[1] + (r[0] < c0);
		r[0] ^= sign;
		c[i] = r[0] + c1;
		c1 = (c[i] < r[0]);
	}

	return (c0 ^ sign) + c1;
}

static void bn_negs_low(digit_t c[], const digit_t a[], digit_t sa, size_t n) {
    digit_t carry = sa & 1;

    sa = -sa;
    for (size_t i = 0; i < n; i++) {
        c[i] = (a[i] ^ sa) + carry;
		carry = (c[i] < carry);
    }
}

static digit_t bn_rshs_low(digit_t *c, const digit_t *a, size_t n, int bits) {
	digit_t r, carry, shift, mask;

	/* Prepare the bit mask. */
	shift = (RADIX - bits) % RADIX;
	mask = MASK(bits);
	carry = a[n - 1] & mask;
	c[n - 1] = (sidig_t)a[n - 1] >> bits;
	for (int i = n - 2; i >= 0; i--) {
		r = a[i] & mask;
		c[i] = (a[i] >> bits) | (carry << shift);
		carry = r;
	}
	return carry;
}
