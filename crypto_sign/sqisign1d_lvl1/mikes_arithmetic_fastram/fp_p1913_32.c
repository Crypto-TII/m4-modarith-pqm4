#if defined(RADIX_32) || defined(PQM4)

#include <stdint.h>
#include <stdio.h>

#include <stdbool.h>
#include <fp.h>

#define sspint int32_t
#define spint uint32_t
#define udpint uint64_t
#define dpint uint64_t

// propagate carries
static spint inline prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 29u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 29u;
  n[0] &= mask;
  for (i = 1; i < 8; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 29u;
  }
  n[8] += (spint)carry;
  return -((n[8] >> 1) >> 30u);
}

// propagate carries and add p if negative, propagate carries again
static void inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] -= (spint)1u & carry;
  n[2] += ((spint)0x14520000u) & carry;
  n[3] += ((spint)0x193c926au) & carry;
  n[4] += ((spint)0x7437252u) & carry;
  n[5] += ((spint)0x1432c3a0u) & carry;
  n[6] += ((spint)0x630ce9au) & carry;
  n[7] += ((spint)0x50d72bbu) & carry;
  n[8] += ((spint)0x34e29eu) & carry;
  (void)prop(n);
}

// Montgomery final subtract
static void modfsb(spint *n) {
  n[0] += (spint)1u;
  n[2] -= (spint)0x14520000u;
  n[3] -= (spint)0x193c926au;
  n[4] -= (spint)0x7437252u;
  n[5] -= (spint)0x1432c3a0u;
  n[6] -= (spint)0x630ce9au;
  n[7] -= (spint)0x50d72bbu;
  n[8] -= (spint)0x34e29eu;
  flatten(n);
}

// Modular addition - reduce less than 2p
static void modadd(const spint *a, const spint *b, spint *n) {
  spint carry;
  n[0] = a[0] + b[0];
  n[1] = a[1] + b[1];
  n[2] = a[2] + b[2];
  n[3] = a[3] + b[3];
  n[4] = a[4] + b[4];
  n[5] = a[5] + b[5];
  n[6] = a[6] + b[6];
  n[7] = a[7] + b[7];
  n[8] = a[8] + b[8];
  n[0] += (spint)2u;
  n[2] -= (spint)0x28a40000u;
  n[3] -= (spint)0x327924d4u;
  n[4] -= (spint)0xe86e4a4u;
  n[5] -= (spint)0x28658740u;
  n[6] -= (spint)0xc619d34u;
  n[7] -= (spint)0xa1ae576u;
  n[8] -= (spint)0x69c53cu;
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[2] += ((spint)0x28a40000u) & carry;
  n[3] += ((spint)0x327924d4u) & carry;
  n[4] += ((spint)0xe86e4a4u) & carry;
  n[5] += ((spint)0x28658740u) & carry;
  n[6] += ((spint)0xc619d34u) & carry;
  n[7] += ((spint)0xa1ae576u) & carry;
  n[8] += ((spint)0x69c53cu) & carry;
  (void)prop(n);
}

// Modular subtraction - reduce less than 2p
static void modsub(const spint *a, const spint *b, spint *n) {
  spint carry;
  n[0] = a[0] - b[0];
  n[1] = a[1] - b[1];
  n[2] = a[2] - b[2];
  n[3] = a[3] - b[3];
  n[4] = a[4] - b[4];
  n[5] = a[5] - b[5];
  n[6] = a[6] - b[6];
  n[7] = a[7] - b[7];
  n[8] = a[8] - b[8];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[2] += ((spint)0x28a40000u) & carry;
  n[3] += ((spint)0x327924d4u) & carry;
  n[4] += ((spint)0xe86e4a4u) & carry;
  n[5] += ((spint)0x28658740u) & carry;
  n[6] += ((spint)0xc619d34u) & carry;
  n[7] += ((spint)0xa1ae576u) & carry;
  n[8] += ((spint)0x69c53cu) & carry;
  (void)prop(n);
}

// Modular negation
static void modneg(const spint *b, spint *n) {
  spint carry;
  n[0] = (spint)0 - b[0];
  n[1] = (spint)0 - b[1];
  n[2] = (spint)0 - b[2];
  n[3] = (spint)0 - b[3];
  n[4] = (spint)0 - b[4];
  n[5] = (spint)0 - b[5];
  n[6] = (spint)0 - b[6];
  n[7] = (spint)0 - b[7];
  n[8] = (spint)0 - b[8];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[2] += ((spint)0x28a40000u) & carry;
  n[3] += ((spint)0x327924d4u) & carry;
  n[4] += ((spint)0xe86e4a4u) & carry;
  n[5] += ((spint)0x28658740u) & carry;
  n[6] += ((spint)0xc619d34u) & carry;
  n[7] += ((spint)0xa1ae576u) & carry;
  n[8] += ((spint)0x69c53cu) & carry;
  (void)prop(n);
}

// Overflow limit   = 18446744073709551616
// maximum possible = 3354897564326958010
// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p2 = 0x14520000u;
  spint p3 = 0x193c926au;
  spint p4 = 0x7437252u;
  spint p5 = 0x1432c3a0u;
  spint p6 = 0x630ce9au;
  spint p7 = 0x50d72bbu;
  spint p8 = 0x34e29eu;
  spint q = ((spint)1 << 29u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  t += (dpint)a[0] * b[0];
  spint v0 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[0] * b[1];
  t += (dpint)a[1] * b[0];
  spint v1 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[0] * b[2];
  t += (dpint)a[1] * b[1];
  t += (dpint)a[2] * b[0];
  t += (dpint)v0 * (dpint)p2;
  spint v2 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[0] * b[3];
  t += (dpint)a[1] * b[2];
  t += (dpint)a[2] * b[1];
  t += (dpint)a[3] * b[0];
  t += (dpint)v0 * (dpint)p3;
  t += (dpint)v1 * (dpint)p2;
  spint v3 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[0] * b[4];
  t += (dpint)a[1] * b[3];
  t += (dpint)a[2] * b[2];
  t += (dpint)a[3] * b[1];
  t += (dpint)a[4] * b[0];
  t += (dpint)v0 * (dpint)p4;
  t += (dpint)v1 * (dpint)p3;
  t += (dpint)v2 * (dpint)p2;
  spint v4 = ((spint)t & mask);
  t >>= 29;
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
  spint v5 = ((spint)t & mask);
  t >>= 29;
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
  spint v6 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[0] * b[7];
  t += (dpint)a[1] * b[6];
  t += (dpint)a[2] * b[5];
  t += (dpint)a[3] * b[4];
  t += (dpint)a[4] * b[3];
  t += (dpint)a[5] * b[2];
  t += (dpint)a[6] * b[1];
  t += (dpint)a[7] * b[0];
  t += (dpint)v0 * (dpint)p7;
  t += (dpint)v1 * (dpint)p6;
  t += (dpint)v2 * (dpint)p5;
  t += (dpint)v3 * (dpint)p4;
  t += (dpint)v4 * (dpint)p3;
  t += (dpint)v5 * (dpint)p2;
  spint v7 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[0] * b[8];
  t += (dpint)a[1] * b[7];
  t += (dpint)a[2] * b[6];
  t += (dpint)a[3] * b[5];
  t += (dpint)a[4] * b[4];
  t += (dpint)a[5] * b[3];
  t += (dpint)a[6] * b[2];
  t += (dpint)a[7] * b[1];
  t += (dpint)a[8] * b[0];
  t += (dpint)v0 * (dpint)p8;
  t += (dpint)v1 * (dpint)p7;
  t += (dpint)v2 * (dpint)p6;
  t += (dpint)v3 * (dpint)p5;
  t += (dpint)v4 * (dpint)p4;
  t += (dpint)v5 * (dpint)p3;
  t += (dpint)v6 * (dpint)p2;
  spint v8 = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[1] * b[8];
  t += (dpint)a[2] * b[7];
  t += (dpint)a[3] * b[6];
  t += (dpint)a[4] * b[5];
  t += (dpint)a[5] * b[4];
  t += (dpint)a[6] * b[3];
  t += (dpint)a[7] * b[2];
  t += (dpint)a[8] * b[1];
  t += (dpint)v1 * (dpint)p8;
  t += (dpint)v2 * (dpint)p7;
  t += (dpint)v3 * (dpint)p6;
  t += (dpint)v4 * (dpint)p5;
  t += (dpint)v5 * (dpint)p4;
  t += (dpint)v6 * (dpint)p3;
  t += (dpint)v7 * (dpint)p2;
  c[0] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[2] * b[8];
  t += (dpint)a[3] * b[7];
  t += (dpint)a[4] * b[6];
  t += (dpint)a[5] * b[5];
  t += (dpint)a[6] * b[4];
  t += (dpint)a[7] * b[3];
  t += (dpint)a[8] * b[2];
  t += (dpint)v2 * (dpint)p8;
  t += (dpint)v3 * (dpint)p7;
  t += (dpint)v4 * (dpint)p6;
  t += (dpint)v5 * (dpint)p5;
  t += (dpint)v6 * (dpint)p4;
  t += (dpint)v7 * (dpint)p3;
  t += (dpint)v8 * (dpint)p2;
  c[1] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[3] * b[8];
  t += (dpint)a[4] * b[7];
  t += (dpint)a[5] * b[6];
  t += (dpint)a[6] * b[5];
  t += (dpint)a[7] * b[4];
  t += (dpint)a[8] * b[3];
  t += (dpint)v3 * (dpint)p8;
  t += (dpint)v4 * (dpint)p7;
  t += (dpint)v5 * (dpint)p6;
  t += (dpint)v6 * (dpint)p5;
  t += (dpint)v7 * (dpint)p4;
  t += (dpint)v8 * (dpint)p3;
  c[2] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[4] * b[8];
  t += (dpint)a[5] * b[7];
  t += (dpint)a[6] * b[6];
  t += (dpint)a[7] * b[5];
  t += (dpint)a[8] * b[4];
  t += (dpint)v4 * (dpint)p8;
  t += (dpint)v5 * (dpint)p7;
  t += (dpint)v6 * (dpint)p6;
  t += (dpint)v7 * (dpint)p5;
  t += (dpint)v8 * (dpint)p4;
  c[3] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[5] * b[8];
  t += (dpint)a[6] * b[7];
  t += (dpint)a[7] * b[6];
  t += (dpint)a[8] * b[5];
  t += (dpint)v5 * (dpint)p8;
  t += (dpint)v6 * (dpint)p7;
  t += (dpint)v7 * (dpint)p6;
  t += (dpint)v8 * (dpint)p5;
  c[4] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[6] * b[8];
  t += (dpint)a[7] * b[7];
  t += (dpint)a[8] * b[6];
  t += (dpint)v6 * (dpint)p8;
  t += (dpint)v7 * (dpint)p7;
  t += (dpint)v8 * (dpint)p6;
  c[5] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[7] * b[8];
  t += (dpint)a[8] * b[7];
  t += (dpint)v7 * (dpint)p8;
  t += (dpint)v8 * (dpint)p7;
  c[6] = ((spint)t & mask);
  t >>= 29;
  t += (dpint)a[8] * b[8];
  t += (dpint)v8 * (dpint)p8;
  c[7] = ((spint)t & mask);
  t >>= 29;
  c[8] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p2 = 0x14520000u;
  spint p3 = 0x193c926au;
  spint p4 = 0x7437252u;
  spint p5 = 0x1432c3a0u;
  spint p6 = 0x630ce9au;
  spint p7 = 0x50d72bbu;
  spint p8 = 0x34e29eu;
  spint q = ((spint)1 << 29u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  tot = (udpint)a[0] * a[0];
  t = tot;
  spint v0 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  spint v1 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[2];
  tot *= 2;
  tot += (udpint)a[1] * a[1];
  t += tot;
  t += (udpint)v0 * p2;
  spint v2 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[3];
  tot += (udpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p3;
  t += (udpint)v1 * p2;
  spint v3 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[4];
  tot += (udpint)a[1] * a[3];
  tot *= 2;
  tot += (udpint)a[2] * a[2];
  t += tot;
  t += (udpint)v0 * p4;
  t += (udpint)v1 * p3;
  t += (udpint)v2 * p2;
  spint v4 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[5];
  tot += (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p5;
  t += (udpint)v1 * p4;
  t += (udpint)v2 * p3;
  t += (udpint)v3 * p2;
  spint v5 = ((spint)t & mask);
  t >>= 29;
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
  spint v6 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[7];
  tot += (udpint)a[1] * a[6];
  tot += (udpint)a[2] * a[5];
  tot += (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p7;
  t += (udpint)v1 * p6;
  t += (udpint)v2 * p5;
  t += (udpint)v3 * p4;
  t += (udpint)v4 * p3;
  t += (udpint)v5 * p2;
  spint v7 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[0] * a[8];
  tot += (udpint)a[1] * a[7];
  tot += (udpint)a[2] * a[6];
  tot += (udpint)a[3] * a[5];
  tot *= 2;
  tot += (udpint)a[4] * a[4];
  t += tot;
  t += (udpint)v0 * p8;
  t += (udpint)v1 * p7;
  t += (udpint)v2 * p6;
  t += (udpint)v3 * p5;
  t += (udpint)v4 * p4;
  t += (udpint)v5 * p3;
  t += (udpint)v6 * p2;
  spint v8 = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[1] * a[8];
  tot += (udpint)a[2] * a[7];
  tot += (udpint)a[3] * a[6];
  tot += (udpint)a[4] * a[5];
  tot *= 2;
  t += tot;
  t += (udpint)v1 * p8;
  t += (udpint)v2 * p7;
  t += (udpint)v3 * p6;
  t += (udpint)v4 * p5;
  t += (udpint)v5 * p4;
  t += (udpint)v6 * p3;
  t += (udpint)v7 * p2;
  c[0] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[2] * a[8];
  tot += (udpint)a[3] * a[7];
  tot += (udpint)a[4] * a[6];
  tot *= 2;
  tot += (udpint)a[5] * a[5];
  t += tot;
  t += (udpint)v2 * p8;
  t += (udpint)v3 * p7;
  t += (udpint)v4 * p6;
  t += (udpint)v5 * p5;
  t += (udpint)v6 * p4;
  t += (udpint)v7 * p3;
  t += (udpint)v8 * p2;
  c[1] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[3] * a[8];
  tot += (udpint)a[4] * a[7];
  tot += (udpint)a[5] * a[6];
  tot *= 2;
  t += tot;
  t += (udpint)v3 * p8;
  t += (udpint)v4 * p7;
  t += (udpint)v5 * p6;
  t += (udpint)v6 * p5;
  t += (udpint)v7 * p4;
  t += (udpint)v8 * p3;
  c[2] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[4] * a[8];
  tot += (udpint)a[5] * a[7];
  tot *= 2;
  tot += (udpint)a[6] * a[6];
  t += tot;
  t += (udpint)v4 * p8;
  t += (udpint)v5 * p7;
  t += (udpint)v6 * p6;
  t += (udpint)v7 * p5;
  t += (udpint)v8 * p4;
  c[3] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[5] * a[8];
  tot += (udpint)a[6] * a[7];
  tot *= 2;
  t += tot;
  t += (udpint)v5 * p8;
  t += (udpint)v6 * p7;
  t += (udpint)v7 * p6;
  t += (udpint)v8 * p5;
  c[4] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[6] * a[8];
  tot *= 2;
  tot += (udpint)a[7] * a[7];
  t += tot;
  t += (udpint)v6 * p8;
  t += (udpint)v7 * p7;
  t += (udpint)v8 * p6;
  c[5] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[7] * a[8];
  tot *= 2;
  t += tot;
  t += (udpint)v7 * p8;
  t += (udpint)v8 * p7;
  c[6] = ((spint)t & mask);
  t >>= 29;
  tot = (udpint)a[8] * a[8];
  t += tot;
  t += (udpint)v8 * p8;
  c[7] = ((spint)t & mask);
  t >>= 29;
  c[8] = (spint)t;
}

// copy
static void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 9; i++) {
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
  spint x[9];
  spint t0[9];
  spint t1[9];
  spint t2[9];
  spint t3[9];
  spint t4[9];
  spint t5[9];
  spint t6[9];
  spint t7[9];
  spint t8[9];
  spint t9[9];
  spint t10[9];
  spint t11[9];
  spint t12[9];
  spint t13[9];
  spint t14[9];
  spint t15[9];
  spint t16[9];
  spint t17[9];
  spint t18[9];
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
  spint s[9];
  spint t[9];
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
  const spint c[9] = {0x19d35104u, 0x4b5c800u,  0x18e62336u,
                      0x14835e83u, 0x16c80fe0u, 0x1283920fu,
                      0x978af0eu,  0x1c6fe080u, 0x1576b8u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[9];
  c[0] = 1;
  for (i = 1; i < 9; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  modfsb(m);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[9];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 9; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 29u) &
          (((c0 ^ (spint)1) - (spint)1) >> 29u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[9];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 9; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 29u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 9; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 9; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[9];
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
  spint s[9];
  spint y[9];
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
  a[8] = ((a[8] << n)) | (a[7] >> (29u - n));
  for (i = 7; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0x1fffffff) | (a[i - 1] >> (29u - n));
  }
  a[0] = (a[0] << n) & (spint)0x1fffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 8; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (29u - n)) & (spint)0x1fffffff);
  }
  a[8] = a[8] >> n;
  return r;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[9], d[9];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 9; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 29) & 1;
  }
  return eq;
}

/* API functions calling generated code */

const digit_t p[NWORDS_ORDER] =  { 0xFFFFFFFF, 0xFFFFFFFF, 0x355147FF, 0x252C9E49, 0x87407437, 0x33A6A865, 0x6B95D98C, 0x34E29E28 };
const digit_t pre[NWORDS_ORDER] = { 0x38ddd801, 0x506e1baf, 0xd03784cb, 0x79b2fe70, 0x026e31a1, 0xa232625e, 0xc194ab7f, 0x2b590630 };

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

#define LIMB 29
#ifdef PQM4
#include "bn.inc"
#include "inversion.inc"
#include "symbol.inc"
#else
#include "../../generic/bn.inc"
#include "../../generic/inversion.inc"
#include "../../generic/symbol.inc"
#endif

#endif
