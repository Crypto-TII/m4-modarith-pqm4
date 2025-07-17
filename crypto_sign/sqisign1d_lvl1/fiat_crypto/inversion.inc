#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#ifdef RADIX_32
typedef int32_t sidig_t;
#elif defined(RADIX_64)
typedef int64_t sidig_t;
#endif

#define SELECT(A, B, C)	((-(C) & ((A) ^ (B))) ^ (A))
#define SIGN(D)	(((digit_t)D) >> (RADIX - 1))
#define MASK(B)	((-(digit_t)((B) >= RADIX)) | (((digit_t)1 << ((B) % RADIX)) - 1))

digit_t bn_muls_low(digit_t *c, const digit_t *a, digit_t sa, sidig_t digit, size_t n);
digit_t bn_addn_low(digit_t *c, const digit_t *a, const digit_t *b, size_t n);

static void fp_copy_sec(digit_t *c, const digit_t *a, digit_t cond) {
	digit_t mask, t;

	mask = -cond;
	for (size_t i = 0; i < NWORDS_FIELD; i++) {
		t = (a[i] ^ c[i]) & mask;
		c[i] ^= t;
	}
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

static sidig_t jumpdivstep(sidig_t m[4], sidig_t delta, digit_t f, digit_t g, int s) {
	digit_t u = 1, v = 0, q = 0, r = 1, c0, c1;

	for (s--; s >= 0; s--) {
		/* First handle the else part: if delta < 0, compute -(f,u,v). */
		c0 = delta >> (RADIX - 1);
		c1 = -(g & 1);
		c0 &= c1;
		/* Conditionally add -(f,u,v) to (g,q,r) */
		g += ((f ^ c0) - c0) & c1;
		q += ((u ^ c0) - c0) & c1;
		r += ((v ^ c0) - c0) & c1;
		/* Now handle the 'if' part, so c0 will be (delta < 0) && (g & 1)) */
		/* delta = SELECT(delta, -delta, c0 & 1) - 2 (for half-divstep), thus
		 * delta = - delta - 2 or delta - 1 */
		delta = (delta ^ c0) - 1;
		f = f + (g & c0);
		u = u + (q & c0);
		v = v + (r & c0);
		g >>= 1;
		u += u;
		v += v;
	}
	m[0] = u;
	m[1] = v;
	m[2] = q;
	m[3] = r;
	return delta;
}

#ifdef ARITH_MIKE
#undef NWORDS_FIELD
#define NWORDS_FIELD NWORDS_ORDER
#define NAILS	(RADIX - LIMB)
#endif

void fp_inv(digit_t* a) {
	sidig_t m[4], d = -1;
	/* Compute number of iterations based on modulus size. */
	/* Iterations taken directly from https://github.com/sipa/safegcd-bounds */
	const int iterations = (45907 * LOG2P + 26313) / 19929;
	int loops, i, j = 0, s = RADIX - 2;
	fp_t t, u0, u1, v0, v1, p01, p11;
	digit_t t0[NWORDS_FIELD + 1], t1[NWORDS_FIELD + 1];
	digit_t f[NWORDS_FIELD + 5] = { 0 }, g[NWORDS_FIELD + 3] = { 0 }, sf, sg;

	if (fp_is_zero(a)) {
		return;
	}

	fp_set(u0, 0);
	fp_set(u1, 0);
	fp_set(v0, 0);
	fp_set(v1, 0);

#ifdef ARITH_MIKE
	fp_frommont(f, a);
	for (size_t i = 0; i < NWORDS_FIELD; i++) {
		j = (RADIX * i) / LIMB;
		g[i] = (f[j] >> ((NAILS*i) % LIMB)) | ((f[j+1] & MASK((NAILS*(i+1)))) << (LIMB - ((NAILS*i) % LIMB)));
	}
	memcpy(f, p, NWORDS_FIELD * sizeof(digit_t));
#else
	/* Convert a from Montgomery form. */
	fp_frommont(g, a);
	fp_copy(f, p);
#endif

	d = jumpdivstep(m, d, f[0], g[0], s);
	t0[NWORDS_FIELD] = bn_muls_low(t0, f, 0, m[0], NWORDS_FIELD);
	t1[NWORDS_FIELD] = bn_muls_low(t1, g, 0, m[1], NWORDS_FIELD);
	bn_addn_low(t0, t0, t1, NWORDS_FIELD + 1);

	f[NWORDS_FIELD] = bn_muls_low(f, f, 0, m[2], NWORDS_FIELD);
	t1[NWORDS_FIELD] = bn_muls_low(t1, g, 0, m[3], NWORDS_FIELD);
	bn_addn_low(t1, t1, f, NWORDS_FIELD + 1);

	/* Update f and g. */
	bn_rshs_low(f, t0, NWORDS_FIELD + 1, s);
	bn_rshs_low(g, t1, NWORDS_FIELD + 1, s);

	/* Update column vector below. */
	v1[0] = SELECT(m[1], -m[1], SIGN(m[1]));
	fp_neg(t, v1);
	fp_copy_sec(v1, t, SIGN(m[1]));
	u1[0] = SELECT(m[3], -m[3], SIGN(m[3]));
	fp_neg(t, u1);
	fp_copy_sec(u1, t, SIGN(m[3]));

	fp_copy(p01, v1);
	fp_copy(p11, u1);
#ifdef ARITH_MIKE
	for (size_t i = 0; i < NWORDS_FIELD; i++) {
		j = (RADIX * i) / LIMB;
		t[i] = (p01[j] >> ((NAILS*i) % LIMB));
		if ((j+1) * LIMB < LOG2P) t[i] |= ((p01[j+1] & MASK((NAILS*(i+1)))) << (LIMB - ((NAILS*i) % LIMB)));
	}
#endif

	loops = iterations / s;
	loops = (iterations % s == 0 ? loops - 1 : loops);

	for (i = 1; i < loops; i++) {
#ifdef ARITH_MIKE
	for (size_t i = 0; i < NWORDS_FIELD; i++) {
		j = (RADIX * i) / LIMB;
		t[i] = (p01[j] >> ((NAILS*i) % LIMB));
		if ((j+1) * LIMB < LOG2P) t[i] |= ((p01[j+1] & MASK((NAILS*(i+1)))) << (LIMB - ((NAILS*i) % LIMB)));
	}
#endif
		d = jumpdivstep(m, d, f[0] & MASK(s), g[0] & MASK(s), s);

		sf = SIGN(f[NWORDS_FIELD]);
		sg = SIGN(g[NWORDS_FIELD]);
		bn_negs_low(u0, f, sf, NWORDS_FIELD);
		bn_negs_low(u1, g, sg, NWORDS_FIELD);

		t0[NWORDS_FIELD] = bn_muls_low(t0, u0, sf, m[0], NWORDS_FIELD);
		t1[NWORDS_FIELD] = bn_muls_low(t1, u1, sg, m[1], NWORDS_FIELD);
		bn_addn_low(t0, t0, t1, NWORDS_FIELD + 1);
		bn_rshs_low(f, t0, NWORDS_FIELD + 1, s);

		t0[NWORDS_FIELD] = bn_muls_low(t0, u0, sf, m[2], NWORDS_FIELD);
		t1[NWORDS_FIELD] = bn_muls_low(t1, u1, sg, m[3], NWORDS_FIELD);
		bn_addn_low(t1, t1, t0, NWORDS_FIELD + 1);
		bn_rshs_low(g, t1, NWORDS_FIELD + 1, s);

		/* Update column vector below. */
		fp_set(t, SELECT(m[0], -m[0], SIGN(m[0])));
		fp_mul(v0, p01, t);
		fp_neg(t, v0);
		fp_copy_sec(v0, t, SIGN(m[0]));

		fp_set(t, SELECT(m[1], -m[1], SIGN(m[1])));
		fp_mul(v1, p11, t);
		fp_neg(t, v1);
		fp_copy_sec(v1, t, SIGN(m[1]));

		fp_set(t, SELECT(m[2], -m[2], SIGN(m[2])));
		fp_mul(u0, p01, t);
		fp_neg(t, u0);
		fp_copy_sec(u0, t, SIGN(m[2]));

		fp_set(t, SELECT(m[3], -m[3], SIGN(m[3])));
		fp_mul(u1, p11, t);
		fp_neg(t, u1);
		fp_copy_sec(u1, t, SIGN(m[3]));

		fp_add(p11, u0, u1);
		fp_add(p01, v0, v1);
	}

	s = iterations - loops * s;
	d = jumpdivstep(m, d, f[0], g[0], s);

	sf = SIGN(f[NWORDS_FIELD]);
	sg = SIGN(g[NWORDS_FIELD]);
	bn_negs_low(u0, f, sf, NWORDS_FIELD);
	bn_negs_low(u1, g, sg, NWORDS_FIELD);

	t0[NWORDS_FIELD] = bn_muls_low(t0, u0, sf, m[0], NWORDS_FIELD);
	t1[NWORDS_FIELD] = bn_muls_low(t1, u1, sg, m[1], NWORDS_FIELD);
	bn_addn_low(t0, t0, t1, NWORDS_FIELD + 1);
	bn_rshs_low(f, t0, NWORDS_FIELD + 1, s);

	t0[NWORDS_FIELD] = bn_muls_low(t0, u0, sf, m[2], NWORDS_FIELD);
	t1[NWORDS_FIELD] = bn_muls_low(t1, u1, sg, m[3], NWORDS_FIELD);
	bn_addn_low(t1, t1, t0, NWORDS_FIELD + 1);
	bn_rshs_low(g, t1, NWORDS_FIELD + 1, s);

	/* Update column vector below. */
	fp_set(t, SELECT(m[0], -m[0], SIGN(m[0])));
	fp_mul(v0, p01, t);
	fp_neg(t, v0);
	fp_copy_sec(v0, t, SIGN(m[0]));
	fp_set(t, SELECT(m[1], -m[1], SIGN(m[1])));
	fp_mul(v1, p11, t);
	fp_neg(t, v1);
	fp_copy_sec(v1, t, SIGN(m[1]));
	fp_add(p01, v0, v1);

	/* Negate based on sign of f at the end. */
	fp_neg(t, p01);
	fp_copy_sec(p01, t, f[NWORDS_FIELD] >> (RADIX - 1));

	/* Multiply by (precomp * R^j) % p, one for each iteration of the loop,
	 * one for the constant, one more to be removed by reduction. */
#ifdef ARITH_MIKE
	memcpy(f, pre, NWORDS_FIELD * sizeof(digit_t));
	i = 0;
	while (f[0] != 0) {
		g[i++] = f[0] & MASK(LIMB);
		bn_rshs_low(f, f, NWORDS_FIELD, LIMB);
	}
	fp_mul(a, p01, g);
	for (size_t i = 0; i < NWORDS_FIELD; i++) {
		j = (RADIX * i) / LIMB;
		g[i] = (a[j] >> ((NAILS*i) % LIMB)) | ((a[j+1] & MASK((NAILS*(i+1)))) << (LIMB - ((NAILS*i) % LIMB)));
	}
#else
	fp_mul(a, p01, pre);
#endif
}
