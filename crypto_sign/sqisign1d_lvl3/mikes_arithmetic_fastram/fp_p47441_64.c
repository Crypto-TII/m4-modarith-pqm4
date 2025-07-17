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
    spint mask = ((spint)1 << 55u) - (spint)1;
    sspint carry = (sspint)n[0];
    carry >>= 55u;
    n[0] &= mask;
    for (i = 1; i < 6; i++) {
        carry += (sspint)n[i];
        n[i] = (spint)carry & mask;
        carry >>= 55u;
    }
    n[6] += (spint)carry;
    return -((n[6] >> 1) >> 62u);
}

// propagate carries and add p if negative, propagate carries again
static void inline flatten(spint *n) {
    spint carry = prop(n);
    n[0] -= (spint)1u & carry;
    n[1] += ((spint)0x69840000000000u) & carry;
    n[2] += ((spint)0x24d5ba91a13185u) & carry;
    n[3] += ((spint)0x575ba8e3917b3u) & carry;
    n[4] += ((spint)0x10ad665bc2e0a9u) & carry;
    n[5] += ((spint)0x3458d5cc0948bau) & carry;
    n[6] += ((spint)0xf7dbbbaac21cu) & carry;
    (void)prop(n);
}

// Montgomery final subtract
static void inline modfsb(spint *n) {
    n[0] += (spint)1u;
    n[1] -= (spint)0x69840000000000u;
    n[2] -= (spint)0x24d5ba91a13185u;
    n[3] -= (spint)0x575ba8e3917b3u;
    n[4] -= (spint)0x10ad665bc2e0a9u;
    n[5] -= (spint)0x3458d5cc0948bau;
    n[6] -= (spint)0xf7dbbbaac21cu;
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
    n[5] = a[5] + b[5];
    n[6] = a[6] + b[6];
    n[0] += (spint)2u;
    n[1] -= (spint)0xd3080000000000u;
    n[2] -= (spint)0x49ab752342630au;
    n[3] -= (spint)0xaeb751c722f66u;
    n[4] -= (spint)0x215accb785c152u;
    n[5] -= (spint)0x68b1ab98129174u;
    n[6] -= (spint)0x1efb777558438u;
    carry = prop(n);
    n[0] -= (spint)2u & carry;
    n[1] += ((spint)0xd3080000000000u) & carry;
    n[2] += ((spint)0x49ab752342630au) & carry;
    n[3] += ((spint)0xaeb751c722f66u) & carry;
    n[4] += ((spint)0x215accb785c152u) & carry;
    n[5] += ((spint)0x68b1ab98129174u) & carry;
    n[6] += ((spint)0x1efb777558438u) & carry;
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
    n[5] = a[5] - b[5];
    n[6] = a[6] - b[6];
    carry = prop(n);
    n[0] -= (spint)2u & carry;
    n[1] += ((spint)0xd3080000000000u) & carry;
    n[2] += ((spint)0x49ab752342630au) & carry;
    n[3] += ((spint)0xaeb751c722f66u) & carry;
    n[4] += ((spint)0x215accb785c152u) & carry;
    n[5] += ((spint)0x68b1ab98129174u) & carry;
    n[6] += ((spint)0x1efb777558438u) & carry;
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
    n[5] = (spint)0 - b[5];
    n[6] = (spint)0 - b[6];
    carry = prop(n);
    n[0] -= (spint)2u & carry;
    n[1] += ((spint)0xd3080000000000u) & carry;
    n[2] += ((spint)0x49ab752342630au) & carry;
    n[3] += ((spint)0xaeb751c722f66u) & carry;
    n[4] += ((spint)0x215accb785c152u) & carry;
    n[5] += ((spint)0x68b1ab98129174u) & carry;
    n[6] += ((spint)0x1efb777558438u) & carry;
    (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 11295303651797634038207769553587024
// Modular multiplication, c=a*b mod 2p
static void inline modmul(const spint *a, const spint *b, spint *c) {
    dpint t = 0;
    spint p1 = 0x69840000000000u;
    spint p2 = 0x24d5ba91a13185u;
    spint p3 = 0x575ba8e3917b3u;
    spint p4 = 0x10ad665bc2e0a9u;
    spint p5 = 0x3458d5cc0948bau;
    spint p6 = 0xf7dbbbaac21cu;
    spint q = ((spint)1 << 55u); // q is unsaturated radix
    spint mask = (spint)(q - (spint)1);
    t += (dpint)a[0] * b[0];
    spint v0 = ((spint)t & mask);
    t >>= 55;
    t += (dpint)a[0] * b[1];
    t += (dpint)a[1] * b[0];
    t += (dpint)v0 * (dpint)p1;
    spint v1 = ((spint)t & mask);
    t >>= 55;
    t += (dpint)a[0] * b[2];
    t += (dpint)a[1] * b[1];
    t += (dpint)a[2] * b[0];
    t += (dpint)v0 * (dpint)p2;
    t += (dpint)v1 * (dpint)p1;
    spint v2 = ((spint)t & mask);
    t >>= 55;
    t += (dpint)a[0] * b[3];
    t += (dpint)a[1] * b[2];
    t += (dpint)a[2] * b[1];
    t += (dpint)a[3] * b[0];
    t += (dpint)v0 * (dpint)p3;
    t += (dpint)v1 * (dpint)p2;
    t += (dpint)v2 * (dpint)p1;
    spint v3 = ((spint)t & mask);
    t >>= 55;
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
    t >>= 55;
    t += (dpint)a[0] * b[5];
    t += (dpint)a[1] * b[4];
    t += (dpint)a[2] * b[3];
    t += (dpint)a[3] * b[2];
    t += (dpint)a[4] * b[1];
    t += (dpint)a[5] * b[0];
    t += (dpint)v0 * (dpint)p5;
    t += (dpint)v1 * (dpint)p4;
    t += (dpint)v2 * (dpint)p3;
    t += (dpint)v3 * (dpint)p2;
    t += (dpint)v4 * (dpint)p1;
    spint v5 = ((spint)t & mask);
    t >>= 55;
    t += (dpint)a[0] * b[6];
    t += (dpint)a[1] * b[5];
    t += (dpint)a[2] * b[4];
    t += (dpint)a[3] * b[3];
    t += (dpint)a[4] * b[2];
    t += (dpint)a[5] * b[1];
    t += (dpint)a[6] * b[0];
    t += (dpint)v0 * (dpint)p6;
    t += (dpint)v1 * (dpint)p5;
    t += (dpint)v2 * (dpint)p4;
    t += (dpint)v3 * (dpint)p3;
    t += (dpint)v4 * (dpint)p2;
    t += (dpint)v5 * (dpint)p1;
    spint v6 = ((spint)t & mask);
    t >>= 55;
    t += (dpint)a[1] * b[6];
    t += (dpint)a[2] * b[5];
    t += (dpint)a[3] * b[4];
    t += (dpint)a[4] * b[3];
    t += (dpint)a[5] * b[2];
    t += (dpint)a[6] * b[1];
    t += (dpint)v1 * (dpint)p6;
    t += (dpint)v2 * (dpint)p5;
    t += (dpint)v3 * (dpint)p4;
    t += (dpint)v4 * (dpint)p3;
    t += (dpint)v5 * (dpint)p2;
    t += (dpint)v6 * (dpint)p1;
    c[0] = ((spint)t & mask);
    t >>= 55;
    t += (dpint)a[2] * b[6];
    t += (dpint)a[3] * b[5];
    t += (dpint)a[4] * b[4];
    t += (dpint)a[5] * b[3];
    t += (dpint)a[6] * b[2];
    t += (dpint)v2 * (dpint)p6;
    t += (dpint)v3 * (dpint)p5;
    t += (dpint)v4 * (dpint)p4;
    t += (dpint)v5 * (dpint)p3;
    t += (dpint)v6 * (dpint)p2;
    c[1] = ((spint)t & mask);
    t >>= 55;
    t += (dpint)a[3] * b[6];
    t += (dpint)a[4] * b[5];
    t += (dpint)a[5] * b[4];
    t += (dpint)a[6] * b[3];
    t += (dpint)v3 * (dpint)p6;
    t += (dpint)v4 * (dpint)p5;
    t += (dpint)v5 * (dpint)p4;
    t += (dpint)v6 * (dpint)p3;
    c[2] = ((spint)t & mask);
    t >>= 55;
    t += (dpint)a[4] * b[6];
    t += (dpint)a[5] * b[5];
    t += (dpint)a[6] * b[4];
    t += (dpint)v4 * (dpint)p6;
    t += (dpint)v5 * (dpint)p5;
    t += (dpint)v6 * (dpint)p4;
    c[3] = ((spint)t & mask);
    t >>= 55;
    t += (dpint)a[5] * b[6];
    t += (dpint)a[6] * b[5];
    t += (dpint)v5 * (dpint)p6;
    t += (dpint)v6 * (dpint)p5;
    c[4] = ((spint)t & mask);
    t >>= 55;
    t += (dpint)a[6] * b[6];
    t += (dpint)v6 * (dpint)p6;
    c[5] = ((spint)t & mask);
    t >>= 55;
    c[6] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void inline modsqr(const spint *a, spint *c) {
    udpint tot;
    udpint t = 0;
    spint p1 = 0x69840000000000u;
    spint p2 = 0x24d5ba91a13185u;
    spint p3 = 0x575ba8e3917b3u;
    spint p4 = 0x10ad665bc2e0a9u;
    spint p5 = 0x3458d5cc0948bau;
    spint p6 = 0xf7dbbbaac21cu;
    spint q = ((spint)1 << 55u); // q is unsaturated radix
    spint mask = (spint)(q - (spint)1);
    tot = (udpint)a[0] * a[0];
    t = tot;
    spint v0 = ((spint)t & mask);
    t >>= 55;
    tot = (udpint)a[0] * a[1];
    tot *= 2;
    t += tot;
    t += (udpint)v0 * p1;
    spint v1 = ((spint)t & mask);
    t >>= 55;
    tot = (udpint)a[0] * a[2];
    tot *= 2;
    tot += (udpint)a[1] * a[1];
    t += tot;
    t += (udpint)v0 * p2;
    t += (udpint)v1 * p1;
    spint v2 = ((spint)t & mask);
    t >>= 55;
    tot = (udpint)a[0] * a[3];
    tot += (udpint)a[1] * a[2];
    tot *= 2;
    t += tot;
    t += (udpint)v0 * p3;
    t += (udpint)v1 * p2;
    t += (udpint)v2 * p1;
    spint v3 = ((spint)t & mask);
    t >>= 55;
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
    t >>= 55;
    tot = (udpint)a[0] * a[5];
    tot += (udpint)a[1] * a[4];
    tot += (udpint)a[2] * a[3];
    tot *= 2;
    t += tot;
    t += (udpint)v0 * p5;
    t += (udpint)v1 * p4;
    t += (udpint)v2 * p3;
    t += (udpint)v3 * p2;
    t += (udpint)v4 * p1;
    spint v5 = ((spint)t & mask);
    t >>= 55;
    tot = (udpint)a[0] * a[6];
    tot += (udpint)a[1] * a[5];
    tot += (udpint)a[2] * a[4];
    tot *= 2;
    tot += (udpint)a[3] * a[3];
    t += tot;
    t += (udpint)v0 * p6;
    t += (udpint)v1 * p5;
    t += (udpint)v2 * p4;
    t += (udpint)v3 * p3;
    t += (udpint)v4 * p2;
    t += (udpint)v5 * p1;
    spint v6 = ((spint)t & mask);
    t >>= 55;
    tot = (udpint)a[1] * a[6];
    tot += (udpint)a[2] * a[5];
    tot += (udpint)a[3] * a[4];
    tot *= 2;
    t += tot;
    t += (udpint)v1 * p6;
    t += (udpint)v2 * p5;
    t += (udpint)v3 * p4;
    t += (udpint)v4 * p3;
    t += (udpint)v5 * p2;
    t += (udpint)v6 * p1;
    c[0] = ((spint)t & mask);
    t >>= 55;
    tot = (udpint)a[2] * a[6];
    tot += (udpint)a[3] * a[5];
    tot *= 2;
    tot += (udpint)a[4] * a[4];
    t += tot;
    t += (udpint)v2 * p6;
    t += (udpint)v3 * p5;
    t += (udpint)v4 * p4;
    t += (udpint)v5 * p3;
    t += (udpint)v6 * p2;
    c[1] = ((spint)t & mask);
    t >>= 55;
    tot = (udpint)a[3] * a[6];
    tot += (udpint)a[4] * a[5];
    tot *= 2;
    t += tot;
    t += (udpint)v3 * p6;
    t += (udpint)v4 * p5;
    t += (udpint)v5 * p4;
    t += (udpint)v6 * p3;
    c[2] = ((spint)t & mask);
    t >>= 55;
    tot = (udpint)a[4] * a[6];
    tot *= 2;
    tot += (udpint)a[5] * a[5];
    t += tot;
    t += (udpint)v4 * p6;
    t += (udpint)v5 * p5;
    t += (udpint)v6 * p4;
    c[3] = ((spint)t & mask);
    t >>= 55;
    tot = (udpint)a[5] * a[6];
    tot *= 2;
    t += tot;
    t += (udpint)v5 * p6;
    t += (udpint)v6 * p5;
    c[4] = ((spint)t & mask);
    t >>= 55;
    tot = (udpint)a[6] * a[6];
    t += tot;
    t += (udpint)v6 * p6;
    c[5] = ((spint)t & mask);
    t >>= 55;
    c[6] = (spint)t;
}

// copy
static void inline modcpy(const spint *a, spint *c) {
    int i;
    for (i = 0; i < 7; i++) {
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
    spint x[7];
    spint t0[7];
    spint t1[7];
    spint t2[7];
    spint t3[7];
    spint t4[7];
    spint t5[7];
    spint t6[7];
    spint t7[7];
    spint t8[7];
    spint t9[7];
    spint t10[7];
    spint t11[7];
    spint t12[7];
    spint t13[7];
    spint t14[7];
    spint t15[7];
    spint t16[7];
    spint t17[7];
    spint t18[7];
    spint t19[7];
    spint t20[7];
    spint t21[7];
    spint t22[7];
    spint t23[7];
    spint t24[7];
    modcpy(w, x);
    modsqr(x, t0);
    modmul(x, t0, t1);
    modmul(x, t1, t8);
    modmul(t0, t8, t6);
    modmul(t8, t6, t20);
    modmul(t8, t20, t2);
    modmul(t0, t2, t4);
    modmul(t8, t4, z);
    modmul(t2, z, t5);
    modmul(t1, t5, t19);
    modmul(t8, t19, t7);
    modmul(t20, t7, t16);
    modmul(z, t16, t10);
    modmul(t20, t10, t18);
    modmul(t8, t18, t22);
    modmul(t0, t22, t13);
    modmul(t6, t13, t2);
    modmul(t8, t2, t21);
    modmul(t0, t21, t3);
    modmul(t5, t3, t15);
    modmul(t6, t15, t9);
    modmul(t6, t9, t5);
    modmul(t4, t5, t4);
    modmul(t0, t4, t11);
    modmul(t20, t11, t17);
    modmul(t20, t17, t12);
    modmul(t8, t12, t23);
    modmul(t6, t23, t14);
    modmul(z, t14, t6);
    modmul(t8, t6, t8);
    modmul(t0, t8, t0);
    modmul(t20, t8, t20);
    modmul(z, t20, z);
    modnsqr(z, 2);
    modcpy(z, t24);
    modnsqr(t24, 5);
    modmul(z, t24, z);
    modmul(t11, z, z);
    modsqr(t24, t24);
    modmul(t0, t24, t24);
    modsqr(z, t0);
    modmul(x, t0, t0);
    modnsqr(t24, 8);
    modmul(t23, t24, t23);
    modnsqr(t23, 7);
    modmul(t22, t23, t22);
    modnsqr(t22, 8);
    modmul(t21, t22, t21);
    modnsqr(t21, 12);
    modmul(t20, t21, t20);
    modnsqr(t20, 9);
    modmul(t9, t20, t20);
    modnsqr(t20, 11);
    modmul(t6, t20, t20);
    modnsqr(t20, 6);
    modmul(t16, t20, t20);
    modnsqr(t20, 12);
    modmul(t19, t20, t19);
    modnsqr(t19, 10);
    modmul(t9, t19, t19);
    modnsqr(t19, 7);
    modmul(t18, t19, t18);
    modnsqr(t18, 12);
    modmul(t17, t18, t17);
    modnsqr(t17, 7);
    modmul(t16, t17, t16);
    modnsqr(t16, 10);
    modmul(t12, t16, t16);
    modnsqr(t16, 8);
    modmul(t15, t16, t15);
    modnsqr(t15, 8);
    modmul(t14, t15, t14);
    modnsqr(t14, 7);
    modmul(t7, t14, t14);
    modnsqr(t14, 11);
    modmul(t13, t14, t13);
    modnsqr(t13, 9);
    modmul(t12, t13, t12);
    modnsqr(t12, 9);
    modmul(t11, t12, t11);
    modnsqr(t11, 7);
    modmul(t10, t11, t10);
    modnsqr(t10, 10);
    modmul(t9, t10, t9);
    modnsqr(t9, 8);
    modmul(t8, t9, t8);
    modnsqr(t8, 6);
    modmul(t7, t8, t7);
    modnsqr(t7, 10);
    modmul(t6, t7, t6);
    modnsqr(t6, 7);
    modmul(t2, t6, t6);
    modnsqr(t6, 9);
    modmul(t5, t6, t5);
    modnsqr(t5, 8);
    modmul(t4, t5, t4);
    modnsqr(t4, 9);
    modmul(t3, t4, t3);
    modnsqr(t3, 11);
    modmul(t2, t3, t2);
    modnsqr(t2, 4);
    modmul(t1, t2, t1);
    modnsqr(t1, 21);
    modmul(t0, t1, t1);
    modnsqr(t1, 16);
    modmul(t0, t1, t1);
    modnsqr(t1, 16);
    modmul(t0, t1, t1);
    modnsqr(t1, 16);
    modmul(t0, t1, t1);
    modnsqr(t1, 16);
    modmul(t0, t1, t0);
    modnsqr(t0, 15);
    modmul(z, t0, z);
}

// calculate inverse, provide progenitor h if available
static void modinv(const spint *x, const spint *h, spint *z) {
    spint s[7];
    spint t[7];
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
    const spint c[7] = {0x4fa09a199185fbu, 0x75a1ff250d623du, 0x20a5c269fa2c7au,
                        0x294e86bbda4a59u, 0x47bd98ef8d71eu,  0x1a7afdeed9dd4au,
                        0xa1841b2d0fccu};
    modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
    int i;
    spint c[7];
    c[0] = 1;
    for (i = 1; i < 7; i++) {
        c[i] = 0;
    }
    modmul(n, c, m);
    modfsb(m);
}

// is unity?
static int modis1(const spint *a) {
    int i;
    spint c[7];
    spint c0;
    spint d = 0;
    redc(a, c);
    for (i = 1; i < 7; i++) {
        d |= c[i];
    }
    c0 = (spint)c[0];
    return ((spint)1 & ((d - (spint)1) >> 55u) &
            (((c0 ^ (spint)1) - (spint)1) >> 55u));
}

// is zero?
static int modis0(const spint *a) {
    int i;
    spint c[7];
    spint d = 0;
    redc(a, c);
    for (i = 0; i < 7; i++) {
        d |= c[i];
    }
    return ((spint)1 & ((d - (spint)1) >> 55u));
}

// set to zero
static void modzer(spint *a) {
    int i;
    for (i = 0; i < 7; i++) {
        a[i] = 0;
    }
}

// set to one
static void modone(spint *a) {
    int i;
    a[0] = 1;
    for (i = 1; i < 7; i++) {
        a[i] = 0;
    }
    nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
    spint r[7];
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
    spint s[7];
    spint y[7];
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
    a[6] = ((a[6] << n)) | (a[5] >> (55u - n));
    for (i = 5; i > 0; i--) {
        a[i] = ((a[i] << n) & (spint)0x7fffffffffffff) | (a[i - 1] >> (55u - n));
    }
    a[0] = (a[0] << n) & (spint)0x7fffffffffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
    int i;
    spint r = a[0] & (((spint)1 << n) - (spint)1);
    for (i = 0; i < 6; i++) {
        a[i] = (a[i] >> n) | ((a[i + 1] << (55u - n)) & (spint)0x7fffffffffffff);
    }
    a[6] = a[6] >> n;
    return r;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
    spint c[7], d[7];
    int i, eq = 1;
    redc(a, c);
    redc(b, d);
    for (i = 0; i < 7; i++) {
      eq &= (((c[i] ^ d[i]) - 1) >> 55) & 1;
    }
    return eq;
  }

/* API functions calling generated code */

const digit_t p[NWORDS_ORDER] =  { 0xFFFFFFFFFFFFFFFF, 0x4C6174C1FFFFFFFF, 0xC722F669356EA468, 0x65BC2E0A90AEB751, 0xC6AE604A45D10AD6, 0x03DF6EEEAB0871A2 };
const digit_t pre[NWORDS_FIELD] = { 0x935c6f50ea0c1583, 0xdaf35a5981be46be, 0xa30e7b140bc15fa6, 0x2c9aa357cf997d1c, 0xdb94cec8bfab71c4, 0x1a422caee8c5c6f };

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
    for (int i = 0; i < 48; i++) {
        ((char *) out)[i] = x[0] & 0xff;
        modshr(8, x);
    }
}

void fp_from_digit_array(digit_t* out, const digit_t* a) {
    for (int i = 0; i < NWORDS_FIELD; i++) {
        out[i] = 0;
    }
    for (int i = 47; i >= 0; i--) {
        modshl(8, out);
        out[0] += (digit_t)((unsigned char *) a)[i];
    }
}

#define LIMB 55
#include "../../generic/bn.inc"
#include "../../generic/inversion.inc"
#include "../../generic/symbol.inc"

#endif
