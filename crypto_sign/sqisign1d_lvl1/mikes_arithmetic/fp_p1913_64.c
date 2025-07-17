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
    spint mask = ((spint)1 << 52u) - (spint)1;
    sspint carry = (sspint)n[0];
    carry >>= 52u;
    n[0] &= mask;
    for (i = 1; i < 4; i++) {
        carry += (sspint)n[i];
        n[i] = (spint)carry & mask;
        carry >>= 52u;
    }
    n[4] += (spint)carry;
    return -((n[4] >> 1) >> 62u);
}

// propagate carries and add p if negative, propagate carries again
static void inline flatten(spint *n) {
    spint carry = prop(n);
    n[0] -= (spint)1u & carry;
    n[1] += ((spint)0x4935514800000u) & carry;
    n[2] += ((spint)0x7407437252c9eu) & carry;
    n[3] += ((spint)0xd98c33a6a8658u) & carry;
    n[4] += ((spint)0x34e29e286b95u) & carry;
    (void)prop(n);
}

// Montgomery final subtract
static void inline modfsb(spint *n) {
    n[0] += (spint)1u;
    n[1] -= (spint)0x4935514800000u;
    n[2] -= (spint)0x7407437252c9eu;
    n[3] -= (spint)0xd98c33a6a8658u;
    n[4] -= (spint)0x34e29e286b95u;
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
    n[1] -= (spint)0x926aa29000000u;
    n[2] -= (spint)0xe80e86e4a593cu;
    n[3] -= (spint)0x1b318674d50cb0u;
    n[4] -= (spint)0x69c53c50d72au;
    carry = prop(n);
    n[0] -= (spint)2u & carry;
    n[1] += ((spint)0x926aa29000000u) & carry;
    n[2] += ((spint)0xe80e86e4a593cu) & carry;
    n[3] += ((spint)0x1b318674d50cb0u) & carry;
    n[4] += ((spint)0x69c53c50d72au) & carry;
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
    n[1] += ((spint)0x926aa29000000u) & carry;
    n[2] += ((spint)0xe80e86e4a593cu) & carry;
    n[3] += ((spint)0x1b318674d50cb0u) & carry;
    n[4] += ((spint)0x69c53c50d72au) & carry;
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
    n[1] += ((spint)0x926aa29000000u) & carry;
    n[2] += ((spint)0xe80e86e4a593cu) & carry;
    n[3] += ((spint)0x1b318674d50cb0u) & carry;
    n[4] += ((spint)0x69c53c50d72au) & carry;
    (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 133902696273723909579593872826746
// Modular multiplication, c=a*b mod 2p
static void inline modmul(const spint *a, const spint *b, spint *c) {
    dpint t = 0;
    spint p1 = 0x4935514800000u;
    spint p2 = 0x7407437252c9eu;
    spint p3 = 0xd98c33a6a8658u;
    spint p4 = 0x34e29e286b95u;
    spint q = ((spint)1 << 52u); // q is unsaturated radix
    spint mask = (spint)(q - (spint)1);
    t += (dpint)a[0] * b[0];
    spint v0 = ((spint)t & mask);
    t >>= 52;
    t += (dpint)a[0] * b[1];
    t += (dpint)a[1] * b[0];
    t += (dpint)v0 * (dpint)p1;
    spint v1 = ((spint)t & mask);
    t >>= 52;
    t += (dpint)a[0] * b[2];
    t += (dpint)a[1] * b[1];
    t += (dpint)a[2] * b[0];
    t += (dpint)v0 * (dpint)p2;
    t += (dpint)v1 * (dpint)p1;
    spint v2 = ((spint)t & mask);
    t >>= 52;
    t += (dpint)a[0] * b[3];
    t += (dpint)a[1] * b[2];
    t += (dpint)a[2] * b[1];
    t += (dpint)a[3] * b[0];
    t += (dpint)v0 * (dpint)p3;
    t += (dpint)v1 * (dpint)p2;
    t += (dpint)v2 * (dpint)p1;
    spint v3 = ((spint)t & mask);
    t >>= 52;
    t += (dpint)a[0] * b[4];
    t += (dpint)a[1] * b[3];
    t += (dpint)a[2] * b[2];
    t += (dpint)a[3] * b[1];
    t += (dpint)a[4] * b[0];
    t += (dpint)v0 * (dpint)p4;
    t += (dpint)v1 * (dpint)p3;
    t += (dpint)v2 * (dpint)p2;
    t += (dpint)v3 * (dpint)p1;
    spint v4 = ((spint)t & mask);
    t >>= 52;
    t += (dpint)a[1] * b[4];
    t += (dpint)a[2] * b[3];
    t += (dpint)a[3] * b[2];
    t += (dpint)a[4] * b[1];
    t += (dpint)v1 * (dpint)p4;
    t += (dpint)v2 * (dpint)p3;
    t += (dpint)v3 * (dpint)p2;
    t += (dpint)v4 * (dpint)p1;
    c[0] = ((spint)t & mask);
    t >>= 52;
    t += (dpint)a[2] * b[4];
    t += (dpint)a[3] * b[3];
    t += (dpint)a[4] * b[2];
    t += (dpint)v2 * (dpint)p4;
    t += (dpint)v3 * (dpint)p3;
    t += (dpint)v4 * (dpint)p2;
    c[1] = ((spint)t & mask);
    t >>= 52;
    t += (dpint)a[3] * b[4];
    t += (dpint)a[4] * b[3];
    t += (dpint)v3 * (dpint)p4;
    t += (dpint)v4 * (dpint)p3;
    c[2] = ((spint)t & mask);
    t >>= 52;
    t += (dpint)a[4] * b[4];
    t += (dpint)v4 * (dpint)p4;
    c[3] = ((spint)t & mask);
    t >>= 52;
    c[4] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void inline modsqr(const spint *a, spint *c) {
    udpint tot;
    udpint t = 0;
    spint p1 = 0x4935514800000u;
    spint p2 = 0x7407437252c9eu;
    spint p3 = 0xd98c33a6a8658u;
    spint p4 = 0x34e29e286b95u;
    spint q = ((spint)1 << 52u); // q is unsaturated radix
    spint mask = (spint)(q - (spint)1);
    tot = (udpint)a[0] * a[0];
    t = tot;
    spint v0 = ((spint)t & mask);
    t >>= 52;
    tot = (udpint)a[0] * a[1];
    tot *= 2;
    t += tot;
    t += (udpint)v0 * p1;
    spint v1 = ((spint)t & mask);
    t >>= 52;
    tot = (udpint)a[0] * a[2];
    tot *= 2;
    tot += (udpint)a[1] * a[1];
    t += tot;
    t += (udpint)v0 * p2;
    t += (udpint)v1 * p1;
    spint v2 = ((spint)t & mask);
    t >>= 52;
    tot = (udpint)a[0] * a[3];
    tot += (udpint)a[1] * a[2];
    tot *= 2;
    t += tot;
    t += (udpint)v0 * p3;
    t += (udpint)v1 * p2;
    t += (udpint)v2 * p1;
    spint v3 = ((spint)t & mask);
    t >>= 52;
    tot = (udpint)a[0] * a[4];
    tot += (udpint)a[1] * a[3];
    tot *= 2;
    tot += (udpint)a[2] * a[2];
    t += tot;
    t += (udpint)v0 * p4;
    t += (udpint)v1 * p3;
    t += (udpint)v2 * p2;
    t += (udpint)v3 * p1;
    spint v4 = ((spint)t & mask);
    t >>= 52;
    tot = (udpint)a[1] * a[4];
    tot += (udpint)a[2] * a[3];
    tot *= 2;
    t += tot;
    t += (udpint)v1 * p4;
    t += (udpint)v2 * p3;
    t += (udpint)v3 * p2;
    t += (udpint)v4 * p1;
    c[0] = ((spint)t & mask);
    t >>= 52;
    tot = (udpint)a[2] * a[4];
    tot *= 2;
    tot += (udpint)a[3] * a[3];
    t += tot;
    t += (udpint)v2 * p4;
    t += (udpint)v3 * p3;
    t += (udpint)v4 * p2;
    c[1] = ((spint)t & mask);
    t >>= 52;
    tot = (udpint)a[3] * a[4];
    tot *= 2;
    t += tot;
    t += (udpint)v3 * p4;
    t += (udpint)v4 * p3;
    c[2] = ((spint)t & mask);
    t >>= 52;
    tot = (udpint)a[4] * a[4];
    t += tot;
    t += (udpint)v4 * p4;
    c[3] = ((spint)t & mask);
    t >>= 52;
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
    spint x[5];
    spint t0[5];
    spint t1[5];
    spint t2[5];
    spint t3[5];
    spint t4[5];
    spint t5[5];
    spint t6[5];
    spint t7[5];
    spint t8[5];
    spint t9[5];
    spint t10[5];
    spint t11[5];
    spint t12[5];
    spint t13[5];
    spint t14[5];
    spint t15[5];
    spint t16[5];
    spint t17[5];
    spint t18[5];
    modcpy(w, x);
    modsqr(x, t13);
    modsqr(t13, t11);
    modmul(x, t11, t0);
    modsqr(t11, t1);
    modmul(t11, t1, t4);
    modmul(t13, t4, t9);
    modmul(t11, t9, t3);
    modsqr(t4, z);
    modmul(t13, z, t2);
    modmul(t0, z, t7);
    modmul(x, t7, t16);
    modmul(t1, t7, t5);
    modmul(t3, t5, t6);
    modmul(t4, t6, t15);
    modmul(t13, t15, t17);
    modmul(t1, t15, t8);
    modmul(t13, t8, t10);
    modmul(t1, t10, t1);
    modmul(t11, t1, t4);
    modmul(t9, t1, t12);
    modmul(t11, t12, t11);
    modmul(t3, t11, t3);
    modmul(t2, t3, t2);
    modmul(t9, t2, t9);
    modmul(z, t9, t14);
    modmul(t13, t14, t13);
    modmul(z, t13, z);
    modmul(t16, z, t16);
    modsqr(z, t18);
    modmul(t4, t18, z);
    modnsqr(t18, 6);
    modmul(t17, t18, t17);
    modnsqr(t17, 10);
    modmul(t16, t17, t16);
    modnsqr(t16, 8);
    modmul(t15, t16, t15);
    modnsqr(t15, 9);
    modmul(t14, t15, t14);
    modnsqr(t14, 9);
    modmul(t13, t14, t13);
    modnsqr(t13, 9);
    modmul(t12, t13, t12);
    modnsqr(t12, 11);
    modmul(t11, t12, t11);
    modnsqr(t11, 8);
    modmul(t10, t11, t10);
    modnsqr(t10, 9);
    modmul(t9, t10, t9);
    modnsqr(t9, 7);
    modmul(t8, t9, t8);
    modnsqr(t8, 9);
    modmul(t7, t8, t8);
    modnsqr(t8, 12);
    modmul(t7, t8, t7);
    modnsqr(t7, 10);
    modmul(t6, t7, t6);
    modnsqr(t6, 8);
    modmul(t5, t6, t5);
    modnsqr(t5, 9);
    modmul(t4, t5, t4);
    modnsqr(t4, 9);
    modmul(t3, t4, t3);
    modnsqr(t3, 10);
    modmul(t2, t3, t2);
    modnsqr(t2, 8);
    modmul(t1, t2, t1);
    modnsqr(t1, 6);
    modmul(t0, t1, t0);
    modnsqr(t0, 12);
    modmul(z, t0, t0);
    modnsqr(t0, 9);
    modmul(z, t0, t0);
    modnsqr(t0, 9);
    modmul(z, t0, t0);
    modnsqr(t0, 9);
    modmul(z, t0, t0);
    modnsqr(t0, 9);
    modmul(z, t0, t0);
    modnsqr(t0, 9);
    modmul(z, t0, t0);
    modnsqr(t0, 9);
    modmul(z, t0, t0);
    modsqr(t0, t0);
    modmul(x, t0, t0);
    modnsqr(t0, 9);
    modmul(z, t0, z);
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
    const spint c[5] = {0x5ae400674d441u, 0x6bd078e623362u, 0x907db203f8290u,
                        0x978af0e941cu, 0x55dae38dfc1u};
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
    return ((spint)1 & ((d - (spint)1) >> 52u) &
            (((c0 ^ (spint)1) - (spint)1) >> 52u));
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
    return ((spint)1 & ((d - (spint)1) >> 52u));
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
        modpro(x, y);
    } else {
        modcpy(h, y);
    }
    modmul(y, x, s);
    modcpy(s, r);
}

// shift left by less than a word
static void modshl(unsigned int n, spint *a) {
    int i;
    a[4] = ((a[4] << n)) | (a[3] >> (52u - n));
    for (i = 3; i > 0; i--) {
        a[i] = ((a[i] << n) & (spint)0xfffffffffffff) | (a[i - 1] >> (52u - n));
    }
    a[0] = (a[0] << n) & (spint)0xfffffffffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
    int i;
    spint r = a[0] & (((spint)1 << n) - (spint)1);
    for (i = 0; i < 4; i++) {
        a[i] = (a[i] >> n) | ((a[i + 1] << (52u - n)) & (spint)0xfffffffffffff);
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
      eq &= (((c[i] ^ d[i]) - 1) >> 52) & 1;
    }
    return eq;
  }

/* API functions calling generated code */

const digit_t p[NWORDS_ORDER] =  { 0xFFFFFFFFFFFFFFFF, 0x252C9E49355147FF, 0x33A6A86587407437, 0x34E29E286B95D98C };
const digit_t pre[NWORDS_ORDER] = { 0x691409bb8f09670f, 0xe28f48df47d79350, 0xa965ae39d498aa25, 0x1b2b0391188c4758 };

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

#define LIMB 52
#include "../../generic/bn.inc"
#include "../../generic/inversion.inc"
#include "../../generic/symbol.inc"

#endif
