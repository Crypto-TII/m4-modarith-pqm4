#ifdef RADIX_64

#include <stdint.h>
#include <stdio.h>

#include <stdbool.h>
#include <fp.h>

#define sspint int64_t
#define spint uint64_t
#define udpint __uint128_t
#define dpint __uint128_t

// propagate carries
static spint inline prop(spint *n) {
    int i;
    spint mask = ((spint)1 << 51u) - (spint)1;
    sspint carry = (sspint)n[0];
    carry >>= 51u;
    n[0] &= mask;
    for (i = 1; i < 4; i++) {
        carry += (sspint)n[i];
        n[i] = (spint)carry & mask;
        carry >>= 51u;
    }
    n[4] += (spint)carry;
    return -((n[4] >> 1) >> 62u);
}

// propagate carries and add p if negative, propagate carries again
static void inline flatten(spint *n) {
    spint carry = prop(n);
    n[0] -= (spint)1u & carry;
    n[4] += ((spint)0x500000000000u) & carry;
    (void)prop(n);
}

// Montgomery final subtract
static void inline modfsb(spint *n) {
    n[0] += (spint)1u;
    n[4] -= (spint)0x500000000000u;
    flatten(n);
}

// Modular addition - reduce less than 2p
static void inline modadd(const spint *a, const spint *b, spint *n) {
    spint carry;
    n[0] = a[0] + b[0];
    n[1] = a[1] + b[1];
    n[2] = a[2] + b[2];
    n[3] = a[3] + b[3];
    n[4] = a[4] + b[4];
    n[0] += (spint)2u;
    n[4] -= (spint)0xa00000000000u;
    carry = prop(n);
    n[0] -= (spint)2u & carry;
    n[4] += ((spint)0xa00000000000u) & carry;
    (void)prop(n);
}

// Modular subtraction - reduce less than 2p
static void inline modsub(const spint *a, const spint *b, spint *n) {
    spint carry;
    n[0] = a[0] - b[0];
    n[1] = a[1] - b[1];
    n[2] = a[2] - b[2];
    n[3] = a[3] - b[3];
    n[4] = a[4] - b[4];
    carry = prop(n);
    n[0] -= (spint)2u & carry;
    n[4] += ((spint)0xa00000000000u) & carry;
    (void)prop(n);
}

// Modular negation
static void inline modneg(const spint *b, spint *n) {
    spint carry;
    n[0] = (spint)0 - b[0];
    n[1] = (spint)0 - b[1];
    n[2] = (spint)0 - b[2];
    n[3] = (spint)0 - b[3];
    n[4] = (spint)0 - b[4];
    carry = prop(n);
    n[0] -= (spint)2u & carry;
    n[4] += ((spint)0xa00000000000u) & carry;
    (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 25551082561965953719787503747077
// Modular multiplication, c=a*b mod 2p
static void inline modmul(const spint *a, const spint *b, spint *c) {
    dpint t = 0;
    spint p4 = 0x500000000000u;
    spint q = ((spint)1 << 51u); // q is unsaturated radix
    spint mask = (spint)(q - (spint)1);
    t += (dpint)a[0] * b[0];
    spint v0 = ((spint)t & mask);
    t >>= 51;
    t += (dpint)a[0] * b[1];
    t += (dpint)a[1] * b[0];
    spint v1 = ((spint)t & mask);
    t >>= 51;
    t += (dpint)a[0] * b[2];
    t += (dpint)a[1] * b[1];
    t += (dpint)a[2] * b[0];
    spint v2 = ((spint)t & mask);
    t >>= 51;
    t += (dpint)a[0] * b[3];
    t += (dpint)a[1] * b[2];
    t += (dpint)a[2] * b[1];
    t += (dpint)a[3] * b[0];
    spint v3 = ((spint)t & mask);
    t >>= 51;
    t += (dpint)a[0] * b[4];
    t += (dpint)a[1] * b[3];
    t += (dpint)a[2] * b[2];
    t += (dpint)a[3] * b[1];
    t += (dpint)a[4] * b[0];
    t += (dpint)v0 * (dpint)p4;
    spint v4 = ((spint)t & mask);
    t >>= 51;
    t += (dpint)a[1] * b[4];
    t += (dpint)a[2] * b[3];
    t += (dpint)a[3] * b[2];
    t += (dpint)a[4] * b[1];
    t += (dpint)v1 * (dpint)p4;
    c[0] = ((spint)t & mask);
    t >>= 51;
    t += (dpint)a[2] * b[4];
    t += (dpint)a[3] * b[3];
    t += (dpint)a[4] * b[2];
    t += (dpint)v2 * (dpint)p4;
    c[1] = ((spint)t & mask);
    t >>= 51;
    t += (dpint)a[3] * b[4];
    t += (dpint)a[4] * b[3];
    t += (dpint)v3 * (dpint)p4;
    c[2] = ((spint)t & mask);
    t >>= 51;
    t += (dpint)a[4] * b[4];
    t += (dpint)v4 * (dpint)p4;
    c[3] = ((spint)t & mask);
    t >>= 51;
    c[4] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void inline modsqr(const spint *a, spint *c) {
    udpint tot;
    udpint t = 0;
    spint p4 = 0x500000000000u;
    spint q = ((spint)1 << 51u); // q is unsaturated radix
    spint mask = (spint)(q - (spint)1);
    tot = (udpint)a[0] * a[0];
    t = tot;
    spint v0 = ((spint)t & mask);
    t >>= 51;
    tot = (udpint)a[0] * a[1];
    tot *= 2;
    t += tot;
    spint v1 = ((spint)t & mask);
    t >>= 51;
    tot = (udpint)a[0] * a[2];
    tot *= 2;
    tot += (udpint)a[1] * a[1];
    t += tot;
    spint v2 = ((spint)t & mask);
    t >>= 51;
    tot = (udpint)a[0] * a[3];
    tot += (udpint)a[1] * a[2];
    tot *= 2;
    t += tot;
    spint v3 = ((spint)t & mask);
    t >>= 51;
    tot = (udpint)a[0] * a[4];
    tot += (udpint)a[1] * a[3];
    tot *= 2;
    tot += (udpint)a[2] * a[2];
    t += tot;
    t += (udpint)v0 * p4;
    spint v4 = ((spint)t & mask);
    t >>= 51;
    tot = (udpint)a[1] * a[4];
    tot += (udpint)a[2] * a[3];
    tot *= 2;
    t += tot;
    t += (udpint)v1 * p4;
    c[0] = ((spint)t & mask);
    t >>= 51;
    tot = (udpint)a[2] * a[4];
    tot *= 2;
    tot += (udpint)a[3] * a[3];
    t += tot;
    t += (udpint)v2 * p4;
    c[1] = ((spint)t & mask);
    t >>= 51;
    tot = (udpint)a[3] * a[4];
    tot *= 2;
    t += tot;
    t += (udpint)v3 * p4;
    c[2] = ((spint)t & mask);
    t >>= 51;
    tot = (udpint)a[4] * a[4];
    t += tot;
    t += (udpint)v4 * p4;
    c[3] = ((spint)t & mask);
    t >>= 51;
    c[4] = (spint)t;
}

// copy
static void inline modcpy(const spint *a, spint *c) {
    int i;
    for (i = 0; i < 5; i++) {
        c[i] = a[i];
    }
}

// square n times
static void modnsqr(spint *a, int n) {
    int i;
    for (i = 0; i < n; i++) {
        modsqr(a, a);
    }
}

// Calculate progenitor
static void modpro(const spint *w, spint *z) {
    spint t0[5];
    spint t1[5];
    spint t2[5];
    spint t3[5];
    spint t4[5];
    modcpy(w,t1);
    modsqr(t1,t0);
    modmul(t0, t1, t0);
    modsqr(t0, t0);
    modmul(t0, t1, t3);
    modcpy(t3, t2);
    modnsqr(t2, 3);
    modmul(t2, t3, t0);
    modcpy(t0, t2);
    modnsqr(t2, 6);
    modmul(t2, t0, t4);
    modcpy(t4, t2);
    modnsqr(t2, 12);
    modmul(t2, t4, t0);
    modcpy(t0, t2);
    modnsqr(t2, 24);
    modmul(t2, t0, t0);
    modcpy(t0, t2);
    modnsqr(t2, 12);
    modmul(t2, t4, t0);
    modcpy(t0, t2);
    modnsqr(t2, 60);
    modmul(t2, t0, t0);
    modcpy(t0, t2);
    modnsqr(t2, 3);
    modmul(t2, t3, t0);
    modcpy(t0, t2);
    modnsqr(t2, 123);
    modmul(t2, t0, t2);
    modmul(t2, w, t0);
    modsqr(t0, t0);
    modsqr(t0, t0);
    modmul(t2, t0, z);
}

// calculate inverse, provide progenitor h if available
static void modinv(const spint *x, const spint *h, spint *z) {
    spint s[5];
    spint t[5];
    if (h == NULL) {
        modpro(x, t);
    } else {
        modcpy(h, t);
    }
    modcpy(x, s);
    modnsqr(t, 2);
    modmul(s, t, z);
}

// Convert m to n-residue form, n=nres(m)
static void nres(const spint *m, spint *n) {
    const spint c[5] = {0x4cccccccccf5cu, 0x1999999999999u, 0x3333333333333u,
                        0x6666666666666u, 0xcccccccccccu};
    modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
    int i;
    spint c[5];
    c[0] = 1;
    for (i = 1; i < 5; i++) {
        c[i] = 0;
    }
    modmul(n, c, m);
    modfsb(m);
}

// is unity?
static int modis1(const spint *a) {
    int i;
    spint c[5];
    spint c0;
    spint d = 0;
    redc(a, c);
    for (i = 1; i < 5; i++) {
        d |= c[i];
    }
    c0 = (spint)c[0];
    return ((spint)1 & ((d - (spint)1) >> 51u) &
            (((c0 ^ (spint)1) - (spint)1) >> 51u));
}

// is zero?
static int modis0(const spint *a) {
    int i;
    spint c[5];
    spint d = 0;
    redc(a, c);
    for (i = 0; i < 5; i++) {
        d |= c[i];
    }
    return ((spint)1 & ((d - (spint)1) >> 51u));
}

// set to zero
static void modzer(spint *a) {
    int i;
    for (i = 0; i < 5; i++) {
        a[i] = 0;
    }
}

// set to one
static void modone(spint *a) {
    int i;
    a[0] = 1;
    for (i = 1; i < 5; i++) {
        a[i] = 0;
    }
    nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
    spint r[5];
    if (h == NULL) {
        modpro(x, r);
        modsqr(r, r);
    } else {
        modsqr(h, r);
    }
    modmul(r, x, r);
    return modis1(r);
}

// Modular square root, provide progenitor h if available, NULL if not
static void modsqrt(const spint *x, const spint *h, spint *r) {
    spint s[5];
    spint y[5];
    if (h == NULL) {
        // (p248 + 1) / 4 = 2^248 + 2^246
        modcpy(x, y);
        modnsqr(y, 246);
        modcpy(y, s);
        modnsqr(s, 2);
        modmul(y, s, r);
    } else {
        modcpy(h, y);
        modmul(y, x, s);
        modcpy(s, r);
    }
}

// shift left by less than a word
static void modshl(unsigned int n, spint *a) {
    int i;
    a[4] = ((a[4] << n)) | (a[3] >> (51u - n));
    for (i = 3; i > 0; i--) {
        a[i] = ((a[i] << n) & (spint)0x7ffffffffffff) | (a[i - 1] >> (51u - n));
    }
    a[0] = (a[0] << n) & (spint)0x7ffffffffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
    int i;
    spint r = a[0] & (((spint)1 << n) - (spint)1);
    for (i = 0; i < 4; i++) {
        a[i] = (a[i] >> n) | ((a[i + 1] << (51u - n)) & (spint)0x7ffffffffffff);
    }
    a[4] = a[4] >> n;
    return r;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
    spint c[5], d[5];
    int i, eq = 1;
    redc(a, c);
    redc(b, d);
    for (i = 0; i < 5; i++) {
      eq &= (((c[i] ^ d[i]) - 1) >> 51) & 1;
    }
    return eq;
  }

/* API functions calling generated code */
const digit_t p[NWORDS_ORDER] =  { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,0x04ffffffffffffff };
const digit_t pre[NWORDS_ORDER] = { 0x66c937c1e339cba8, 0x9e5a9e871430e479, 0x5830ee8a61c33e83, 0x38b18ce022bff37 };

bool fp_is_equal(const digit_t* a, const digit_t* b) {
    return (bool) modcmp(a, b);
}

bool fp_is_zero(const digit_t* a) {
    return (bool) modis0(a);
}

void fp_copy(digit_t* out, const digit_t* a) {
    modcpy(a, out);
}

void fp_add(digit_t* out, const digit_t* a, const digit_t* b) {
    modadd(a, b, out);
}

void fp_sub(digit_t* out, const digit_t* a, const digit_t* b) {
    modsub(a, b, out);
}

void fp_neg(digit_t* out, const digit_t* a) {
    modneg(a, out);
}

void fp_sqr(digit_t* out, const digit_t* a) {
    modsqr(a, out);
}

void fp_mul(digit_t* out, const digit_t* a, const digit_t* b) {
    modmul(a, b, out);
}

void _fp_inv(digit_t* a) {
    modinv(a, NULL, a);
}

bool _fp_is_square(const digit_t* a) {
    return (bool) modqr(NULL, a);
}

void fp_sqrt(digit_t* a) {
    modsqrt(a, NULL, a);
}

void fp_exp3div4(digit_t* out, const digit_t* a) {
    modpro(a, out);
}

void fp_tomont(digit_t* out, const digit_t* a) {
    nres(a, out);
}

void fp_frommont(digit_t* out, const digit_t* a) {
    redc(a, out);
}

void fp_mont_setone(digit_t* out) {
    modone(out);
}

void fp_to_digit_array(digit_t* out, const digit_t* a) {
    digit_t x[NWORDS_FIELD];
    modcpy(a, x);
    for (int i = 0; i < NWORDS_ORDER; i++) {
        out[i] = 0;
    }
    for (int i = 0; i < 32; i++) {
        ((char *) out)[i] = x[0] & 0xff;
        modshr(8, x);
    }
}

void fp_from_digit_array(digit_t* out, const digit_t* a) {
    for (int i = 0; i < NWORDS_FIELD; i++) {
        out[i] = 0;
    }
    for (int i = 31; i >= 0; i--) {
        modshl(8, out);
        out[0] += (digit_t)((unsigned char *) a)[i];
    }
}

#define LIMB 51
#include "../../generic/bn.inc"
#include "../../generic/inversion.inc"
#include "../../generic/symbol.inc"

#endif