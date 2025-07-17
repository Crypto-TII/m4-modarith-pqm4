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
  spint mask = ((spint)1 << 28u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 28u;
  n[0] &= mask;
  for (i = 1; i < 17; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 28u;
  }
  n[17] += (spint)carry;
  return -((n[17] >> 1) >> 30u);
}

// propagate carries and add p if negative, propagate carries again
static void inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] -= (spint)1u & carry;
  n[5] += ((spint)0xada6e20u) & carry;
  n[6] += ((spint)0xe994c68u) & carry;
  n[7] += ((spint)0x781974cu) & carry;
  n[8] += ((spint)0xaf0a29au) & carry;
  n[9] += ((spint)0xdea65fu) & carry;
  n[10] += ((spint)0xac5904au) & carry;
  n[11] += ((spint)0x7d01fe3u) & carry;
  n[12] += ((spint)0xe632650u) & carry;
  n[13] += ((spint)0x9202bdbu) & carry;
  n[14] += ((spint)0x36936e7u) & carry;
  n[15] += ((spint)0x8c15b00u) & carry;
  n[16] += ((spint)0x8869bc6u) & carry;
  n[17] += ((spint)0x255946au) & carry;
  (void)prop(n);
}

// Montgomery final subtract
static void modfsb(spint *n) {
  n[0] += (spint)1u;
  n[5] -= (spint)0xada6e20u;
  n[6] -= (spint)0xe994c68u;
  n[7] -= (spint)0x781974cu;
  n[8] -= (spint)0xaf0a29au;
  n[9] -= (spint)0xdea65fu;
  n[10] -= (spint)0xac5904au;
  n[11] -= (spint)0x7d01fe3u;
  n[12] -= (spint)0xe632650u;
  n[13] -= (spint)0x9202bdbu;
  n[14] -= (spint)0x36936e7u;
  n[15] -= (spint)0x8c15b00u;
  n[16] -= (spint)0x8869bc6u;
  n[17] -= (spint)0x255946au;
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
  n[9] = a[9] + b[9];
  n[10] = a[10] + b[10];
  n[11] = a[11] + b[11];
  n[12] = a[12] + b[12];
  n[13] = a[13] + b[13];
  n[14] = a[14] + b[14];
  n[15] = a[15] + b[15];
  n[16] = a[16] + b[16];
  n[17] = a[17] + b[17];
  n[0] += (spint)2u;
  n[5] -= (spint)0x15b4dc40u;
  n[6] -= (spint)0x1d3298d0u;
  n[7] -= (spint)0xf032e98u;
  n[8] -= (spint)0x15e14534u;
  n[9] -= (spint)0x1bd4cbeu;
  n[10] -= (spint)0x158b2094u;
  n[11] -= (spint)0xfa03fc6u;
  n[12] -= (spint)0x1cc64ca0u;
  n[13] -= (spint)0x124057b6u;
  n[14] -= (spint)0x6d26dceu;
  n[15] -= (spint)0x1182b600u;
  n[16] -= (spint)0x110d378cu;
  n[17] -= (spint)0x4ab28d4u;
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[5] += ((spint)0x15b4dc40u) & carry;
  n[6] += ((spint)0x1d3298d0u) & carry;
  n[7] += ((spint)0xf032e98u) & carry;
  n[8] += ((spint)0x15e14534u) & carry;
  n[9] += ((spint)0x1bd4cbeu) & carry;
  n[10] += ((spint)0x158b2094u) & carry;
  n[11] += ((spint)0xfa03fc6u) & carry;
  n[12] += ((spint)0x1cc64ca0u) & carry;
  n[13] += ((spint)0x124057b6u) & carry;
  n[14] += ((spint)0x6d26dceu) & carry;
  n[15] += ((spint)0x1182b600u) & carry;
  n[16] += ((spint)0x110d378cu) & carry;
  n[17] += ((spint)0x4ab28d4u) & carry;
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
  n[9] = a[9] - b[9];
  n[10] = a[10] - b[10];
  n[11] = a[11] - b[11];
  n[12] = a[12] - b[12];
  n[13] = a[13] - b[13];
  n[14] = a[14] - b[14];
  n[15] = a[15] - b[15];
  n[16] = a[16] - b[16];
  n[17] = a[17] - b[17];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[5] += ((spint)0x15b4dc40u) & carry;
  n[6] += ((spint)0x1d3298d0u) & carry;
  n[7] += ((spint)0xf032e98u) & carry;
  n[8] += ((spint)0x15e14534u) & carry;
  n[9] += ((spint)0x1bd4cbeu) & carry;
  n[10] += ((spint)0x158b2094u) & carry;
  n[11] += ((spint)0xfa03fc6u) & carry;
  n[12] += ((spint)0x1cc64ca0u) & carry;
  n[13] += ((spint)0x124057b6u) & carry;
  n[14] += ((spint)0x6d26dceu) & carry;
  n[15] += ((spint)0x1182b600u) & carry;
  n[16] += ((spint)0x110d378cu) & carry;
  n[17] += ((spint)0x4ab28d4u) & carry;
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
  n[9] = (spint)0 - b[9];
  n[10] = (spint)0 - b[10];
  n[11] = (spint)0 - b[11];
  n[12] = (spint)0 - b[12];
  n[13] = (spint)0 - b[13];
  n[14] = (spint)0 - b[14];
  n[15] = (spint)0 - b[15];
  n[16] = (spint)0 - b[16];
  n[17] = (spint)0 - b[17];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[5] += ((spint)0x15b4dc40u) & carry;
  n[6] += ((spint)0x1d3298d0u) & carry;
  n[7] += ((spint)0xf032e98u) & carry;
  n[8] += ((spint)0x15e14534u) & carry;
  n[9] += ((spint)0x1bd4cbeu) & carry;
  n[10] += ((spint)0x158b2094u) & carry;
  n[11] += ((spint)0xfa03fc6u) & carry;
  n[12] += ((spint)0x1cc64ca0u) & carry;
  n[13] += ((spint)0x124057b6u) & carry;
  n[14] += ((spint)0x6d26dceu) & carry;
  n[15] += ((spint)0x1182b600u) & carry;
  n[16] += ((spint)0x110d378cu) & carry;
  n[17] += ((spint)0x4ab28d4u) & carry;
  (void)prop(n);
}

// Overflow limit   = 18446744073709551616
// maximum possible = 1791946672152748246
// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p5 = 0xada6e20u;
  spint p6 = 0xe994c68u;
  spint p7 = 0x781974cu;
  spint p8 = 0xaf0a29au;
  spint p9 = 0xdea65fu;
  spint p10 = 0xac5904au;
  spint p11 = 0x7d01fe3u;
  spint p12 = 0xe632650u;
  spint p13 = 0x9202bdbu;
  spint p14 = 0x36936e7u;
  spint p15 = 0x8c15b00u;
  spint p16 = 0x8869bc6u;
  spint p17 = 0x255946au;
  spint q = ((spint)1 << 28u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  t += (dpint)a[0] * b[0];
  spint v0 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[1];
  t += (dpint)a[1] * b[0];
  spint v1 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[2];
  t += (dpint)a[1] * b[1];
  t += (dpint)a[2] * b[0];
  spint v2 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[3];
  t += (dpint)a[1] * b[2];
  t += (dpint)a[2] * b[1];
  t += (dpint)a[3] * b[0];
  spint v3 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[4];
  t += (dpint)a[1] * b[3];
  t += (dpint)a[2] * b[2];
  t += (dpint)a[3] * b[1];
  t += (dpint)a[4] * b[0];
  spint v4 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[5];
  t += (dpint)a[1] * b[4];
  t += (dpint)a[2] * b[3];
  t += (dpint)a[3] * b[2];
  t += (dpint)a[4] * b[1];
  t += (dpint)a[5] * b[0];
  t += (dpint)v0 * (dpint)p5;
  spint v5 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[6];
  t += (dpint)a[1] * b[5];
  t += (dpint)a[2] * b[4];
  t += (dpint)a[3] * b[3];
  t += (dpint)a[4] * b[2];
  t += (dpint)a[5] * b[1];
  t += (dpint)a[6] * b[0];
  t += (dpint)v0 * (dpint)p6;
  t += (dpint)v1 * (dpint)p5;
  spint v6 = ((spint)t & mask);
  t >>= 28;
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
  spint v7 = ((spint)t & mask);
  t >>= 28;
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
  spint v8 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[9];
  t += (dpint)a[1] * b[8];
  t += (dpint)a[2] * b[7];
  t += (dpint)a[3] * b[6];
  t += (dpint)a[4] * b[5];
  t += (dpint)a[5] * b[4];
  t += (dpint)a[6] * b[3];
  t += (dpint)a[7] * b[2];
  t += (dpint)a[8] * b[1];
  t += (dpint)a[9] * b[0];
  t += (dpint)v0 * (dpint)p9;
  t += (dpint)v1 * (dpint)p8;
  t += (dpint)v2 * (dpint)p7;
  t += (dpint)v3 * (dpint)p6;
  t += (dpint)v4 * (dpint)p5;
  spint v9 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[10];
  t += (dpint)a[1] * b[9];
  t += (dpint)a[2] * b[8];
  t += (dpint)a[3] * b[7];
  t += (dpint)a[4] * b[6];
  t += (dpint)a[5] * b[5];
  t += (dpint)a[6] * b[4];
  t += (dpint)a[7] * b[3];
  t += (dpint)a[8] * b[2];
  t += (dpint)a[9] * b[1];
  t += (dpint)a[10] * b[0];
  t += (dpint)v0 * (dpint)p10;
  t += (dpint)v1 * (dpint)p9;
  t += (dpint)v2 * (dpint)p8;
  t += (dpint)v3 * (dpint)p7;
  t += (dpint)v4 * (dpint)p6;
  t += (dpint)v5 * (dpint)p5;
  spint v10 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[11];
  t += (dpint)a[1] * b[10];
  t += (dpint)a[2] * b[9];
  t += (dpint)a[3] * b[8];
  t += (dpint)a[4] * b[7];
  t += (dpint)a[5] * b[6];
  t += (dpint)a[6] * b[5];
  t += (dpint)a[7] * b[4];
  t += (dpint)a[8] * b[3];
  t += (dpint)a[9] * b[2];
  t += (dpint)a[10] * b[1];
  t += (dpint)a[11] * b[0];
  t += (dpint)v0 * (dpint)p11;
  t += (dpint)v1 * (dpint)p10;
  t += (dpint)v2 * (dpint)p9;
  t += (dpint)v3 * (dpint)p8;
  t += (dpint)v4 * (dpint)p7;
  t += (dpint)v5 * (dpint)p6;
  t += (dpint)v6 * (dpint)p5;
  spint v11 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[12];
  t += (dpint)a[1] * b[11];
  t += (dpint)a[2] * b[10];
  t += (dpint)a[3] * b[9];
  t += (dpint)a[4] * b[8];
  t += (dpint)a[5] * b[7];
  t += (dpint)a[6] * b[6];
  t += (dpint)a[7] * b[5];
  t += (dpint)a[8] * b[4];
  t += (dpint)a[9] * b[3];
  t += (dpint)a[10] * b[2];
  t += (dpint)a[11] * b[1];
  t += (dpint)a[12] * b[0];
  t += (dpint)v0 * (dpint)p12;
  t += (dpint)v1 * (dpint)p11;
  t += (dpint)v2 * (dpint)p10;
  t += (dpint)v3 * (dpint)p9;
  t += (dpint)v4 * (dpint)p8;
  t += (dpint)v5 * (dpint)p7;
  t += (dpint)v6 * (dpint)p6;
  t += (dpint)v7 * (dpint)p5;
  spint v12 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[13];
  t += (dpint)a[1] * b[12];
  t += (dpint)a[2] * b[11];
  t += (dpint)a[3] * b[10];
  t += (dpint)a[4] * b[9];
  t += (dpint)a[5] * b[8];
  t += (dpint)a[6] * b[7];
  t += (dpint)a[7] * b[6];
  t += (dpint)a[8] * b[5];
  t += (dpint)a[9] * b[4];
  t += (dpint)a[10] * b[3];
  t += (dpint)a[11] * b[2];
  t += (dpint)a[12] * b[1];
  t += (dpint)a[13] * b[0];
  t += (dpint)v0 * (dpint)p13;
  t += (dpint)v1 * (dpint)p12;
  t += (dpint)v2 * (dpint)p11;
  t += (dpint)v3 * (dpint)p10;
  t += (dpint)v4 * (dpint)p9;
  t += (dpint)v5 * (dpint)p8;
  t += (dpint)v6 * (dpint)p7;
  t += (dpint)v7 * (dpint)p6;
  t += (dpint)v8 * (dpint)p5;
  spint v13 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[14];
  t += (dpint)a[1] * b[13];
  t += (dpint)a[2] * b[12];
  t += (dpint)a[3] * b[11];
  t += (dpint)a[4] * b[10];
  t += (dpint)a[5] * b[9];
  t += (dpint)a[6] * b[8];
  t += (dpint)a[7] * b[7];
  t += (dpint)a[8] * b[6];
  t += (dpint)a[9] * b[5];
  t += (dpint)a[10] * b[4];
  t += (dpint)a[11] * b[3];
  t += (dpint)a[12] * b[2];
  t += (dpint)a[13] * b[1];
  t += (dpint)a[14] * b[0];
  t += (dpint)v0 * (dpint)p14;
  t += (dpint)v1 * (dpint)p13;
  t += (dpint)v2 * (dpint)p12;
  t += (dpint)v3 * (dpint)p11;
  t += (dpint)v4 * (dpint)p10;
  t += (dpint)v5 * (dpint)p9;
  t += (dpint)v6 * (dpint)p8;
  t += (dpint)v7 * (dpint)p7;
  t += (dpint)v8 * (dpint)p6;
  t += (dpint)v9 * (dpint)p5;
  spint v14 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[15];
  t += (dpint)a[1] * b[14];
  t += (dpint)a[2] * b[13];
  t += (dpint)a[3] * b[12];
  t += (dpint)a[4] * b[11];
  t += (dpint)a[5] * b[10];
  t += (dpint)a[6] * b[9];
  t += (dpint)a[7] * b[8];
  t += (dpint)a[8] * b[7];
  t += (dpint)a[9] * b[6];
  t += (dpint)a[10] * b[5];
  t += (dpint)a[11] * b[4];
  t += (dpint)a[12] * b[3];
  t += (dpint)a[13] * b[2];
  t += (dpint)a[14] * b[1];
  t += (dpint)a[15] * b[0];
  t += (dpint)v0 * (dpint)p15;
  t += (dpint)v1 * (dpint)p14;
  t += (dpint)v2 * (dpint)p13;
  t += (dpint)v3 * (dpint)p12;
  t += (dpint)v4 * (dpint)p11;
  t += (dpint)v5 * (dpint)p10;
  t += (dpint)v6 * (dpint)p9;
  t += (dpint)v7 * (dpint)p8;
  t += (dpint)v8 * (dpint)p7;
  t += (dpint)v9 * (dpint)p6;
  t += (dpint)v10 * (dpint)p5;
  spint v15 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[16];
  t += (dpint)a[1] * b[15];
  t += (dpint)a[2] * b[14];
  t += (dpint)a[3] * b[13];
  t += (dpint)a[4] * b[12];
  t += (dpint)a[5] * b[11];
  t += (dpint)a[6] * b[10];
  t += (dpint)a[7] * b[9];
  t += (dpint)a[8] * b[8];
  t += (dpint)a[9] * b[7];
  t += (dpint)a[10] * b[6];
  t += (dpint)a[11] * b[5];
  t += (dpint)a[12] * b[4];
  t += (dpint)a[13] * b[3];
  t += (dpint)a[14] * b[2];
  t += (dpint)a[15] * b[1];
  t += (dpint)a[16] * b[0];
  t += (dpint)v0 * (dpint)p16;
  t += (dpint)v1 * (dpint)p15;
  t += (dpint)v2 * (dpint)p14;
  t += (dpint)v3 * (dpint)p13;
  t += (dpint)v4 * (dpint)p12;
  t += (dpint)v5 * (dpint)p11;
  t += (dpint)v6 * (dpint)p10;
  t += (dpint)v7 * (dpint)p9;
  t += (dpint)v8 * (dpint)p8;
  t += (dpint)v9 * (dpint)p7;
  t += (dpint)v10 * (dpint)p6;
  t += (dpint)v11 * (dpint)p5;
  spint v16 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[17];
  t += (dpint)a[1] * b[16];
  t += (dpint)a[2] * b[15];
  t += (dpint)a[3] * b[14];
  t += (dpint)a[4] * b[13];
  t += (dpint)a[5] * b[12];
  t += (dpint)a[6] * b[11];
  t += (dpint)a[7] * b[10];
  t += (dpint)a[8] * b[9];
  t += (dpint)a[9] * b[8];
  t += (dpint)a[10] * b[7];
  t += (dpint)a[11] * b[6];
  t += (dpint)a[12] * b[5];
  t += (dpint)a[13] * b[4];
  t += (dpint)a[14] * b[3];
  t += (dpint)a[15] * b[2];
  t += (dpint)a[16] * b[1];
  t += (dpint)a[17] * b[0];
  t += (dpint)v0 * (dpint)p17;
  t += (dpint)v1 * (dpint)p16;
  t += (dpint)v2 * (dpint)p15;
  t += (dpint)v3 * (dpint)p14;
  t += (dpint)v4 * (dpint)p13;
  t += (dpint)v5 * (dpint)p12;
  t += (dpint)v6 * (dpint)p11;
  t += (dpint)v7 * (dpint)p10;
  t += (dpint)v8 * (dpint)p9;
  t += (dpint)v9 * (dpint)p8;
  t += (dpint)v10 * (dpint)p7;
  t += (dpint)v11 * (dpint)p6;
  t += (dpint)v12 * (dpint)p5;
  spint v17 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[1] * b[17];
  t += (dpint)a[2] * b[16];
  t += (dpint)a[3] * b[15];
  t += (dpint)a[4] * b[14];
  t += (dpint)a[5] * b[13];
  t += (dpint)a[6] * b[12];
  t += (dpint)a[7] * b[11];
  t += (dpint)a[8] * b[10];
  t += (dpint)a[9] * b[9];
  t += (dpint)a[10] * b[8];
  t += (dpint)a[11] * b[7];
  t += (dpint)a[12] * b[6];
  t += (dpint)a[13] * b[5];
  t += (dpint)a[14] * b[4];
  t += (dpint)a[15] * b[3];
  t += (dpint)a[16] * b[2];
  t += (dpint)a[17] * b[1];
  t += (dpint)v1 * (dpint)p17;
  t += (dpint)v2 * (dpint)p16;
  t += (dpint)v3 * (dpint)p15;
  t += (dpint)v4 * (dpint)p14;
  t += (dpint)v5 * (dpint)p13;
  t += (dpint)v6 * (dpint)p12;
  t += (dpint)v7 * (dpint)p11;
  t += (dpint)v8 * (dpint)p10;
  t += (dpint)v9 * (dpint)p9;
  t += (dpint)v10 * (dpint)p8;
  t += (dpint)v11 * (dpint)p7;
  t += (dpint)v12 * (dpint)p6;
  t += (dpint)v13 * (dpint)p5;
  c[0] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[2] * b[17];
  t += (dpint)a[3] * b[16];
  t += (dpint)a[4] * b[15];
  t += (dpint)a[5] * b[14];
  t += (dpint)a[6] * b[13];
  t += (dpint)a[7] * b[12];
  t += (dpint)a[8] * b[11];
  t += (dpint)a[9] * b[10];
  t += (dpint)a[10] * b[9];
  t += (dpint)a[11] * b[8];
  t += (dpint)a[12] * b[7];
  t += (dpint)a[13] * b[6];
  t += (dpint)a[14] * b[5];
  t += (dpint)a[15] * b[4];
  t += (dpint)a[16] * b[3];
  t += (dpint)a[17] * b[2];
  t += (dpint)v2 * (dpint)p17;
  t += (dpint)v3 * (dpint)p16;
  t += (dpint)v4 * (dpint)p15;
  t += (dpint)v5 * (dpint)p14;
  t += (dpint)v6 * (dpint)p13;
  t += (dpint)v7 * (dpint)p12;
  t += (dpint)v8 * (dpint)p11;
  t += (dpint)v9 * (dpint)p10;
  t += (dpint)v10 * (dpint)p9;
  t += (dpint)v11 * (dpint)p8;
  t += (dpint)v12 * (dpint)p7;
  t += (dpint)v13 * (dpint)p6;
  t += (dpint)v14 * (dpint)p5;
  c[1] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[3] * b[17];
  t += (dpint)a[4] * b[16];
  t += (dpint)a[5] * b[15];
  t += (dpint)a[6] * b[14];
  t += (dpint)a[7] * b[13];
  t += (dpint)a[8] * b[12];
  t += (dpint)a[9] * b[11];
  t += (dpint)a[10] * b[10];
  t += (dpint)a[11] * b[9];
  t += (dpint)a[12] * b[8];
  t += (dpint)a[13] * b[7];
  t += (dpint)a[14] * b[6];
  t += (dpint)a[15] * b[5];
  t += (dpint)a[16] * b[4];
  t += (dpint)a[17] * b[3];
  t += (dpint)v3 * (dpint)p17;
  t += (dpint)v4 * (dpint)p16;
  t += (dpint)v5 * (dpint)p15;
  t += (dpint)v6 * (dpint)p14;
  t += (dpint)v7 * (dpint)p13;
  t += (dpint)v8 * (dpint)p12;
  t += (dpint)v9 * (dpint)p11;
  t += (dpint)v10 * (dpint)p10;
  t += (dpint)v11 * (dpint)p9;
  t += (dpint)v12 * (dpint)p8;
  t += (dpint)v13 * (dpint)p7;
  t += (dpint)v14 * (dpint)p6;
  t += (dpint)v15 * (dpint)p5;
  c[2] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[4] * b[17];
  t += (dpint)a[5] * b[16];
  t += (dpint)a[6] * b[15];
  t += (dpint)a[7] * b[14];
  t += (dpint)a[8] * b[13];
  t += (dpint)a[9] * b[12];
  t += (dpint)a[10] * b[11];
  t += (dpint)a[11] * b[10];
  t += (dpint)a[12] * b[9];
  t += (dpint)a[13] * b[8];
  t += (dpint)a[14] * b[7];
  t += (dpint)a[15] * b[6];
  t += (dpint)a[16] * b[5];
  t += (dpint)a[17] * b[4];
  t += (dpint)v4 * (dpint)p17;
  t += (dpint)v5 * (dpint)p16;
  t += (dpint)v6 * (dpint)p15;
  t += (dpint)v7 * (dpint)p14;
  t += (dpint)v8 * (dpint)p13;
  t += (dpint)v9 * (dpint)p12;
  t += (dpint)v10 * (dpint)p11;
  t += (dpint)v11 * (dpint)p10;
  t += (dpint)v12 * (dpint)p9;
  t += (dpint)v13 * (dpint)p8;
  t += (dpint)v14 * (dpint)p7;
  t += (dpint)v15 * (dpint)p6;
  t += (dpint)v16 * (dpint)p5;
  c[3] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[5] * b[17];
  t += (dpint)a[6] * b[16];
  t += (dpint)a[7] * b[15];
  t += (dpint)a[8] * b[14];
  t += (dpint)a[9] * b[13];
  t += (dpint)a[10] * b[12];
  t += (dpint)a[11] * b[11];
  t += (dpint)a[12] * b[10];
  t += (dpint)a[13] * b[9];
  t += (dpint)a[14] * b[8];
  t += (dpint)a[15] * b[7];
  t += (dpint)a[16] * b[6];
  t += (dpint)a[17] * b[5];
  t += (dpint)v5 * (dpint)p17;
  t += (dpint)v6 * (dpint)p16;
  t += (dpint)v7 * (dpint)p15;
  t += (dpint)v8 * (dpint)p14;
  t += (dpint)v9 * (dpint)p13;
  t += (dpint)v10 * (dpint)p12;
  t += (dpint)v11 * (dpint)p11;
  t += (dpint)v12 * (dpint)p10;
  t += (dpint)v13 * (dpint)p9;
  t += (dpint)v14 * (dpint)p8;
  t += (dpint)v15 * (dpint)p7;
  t += (dpint)v16 * (dpint)p6;
  t += (dpint)v17 * (dpint)p5;
  c[4] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[6] * b[17];
  t += (dpint)a[7] * b[16];
  t += (dpint)a[8] * b[15];
  t += (dpint)a[9] * b[14];
  t += (dpint)a[10] * b[13];
  t += (dpint)a[11] * b[12];
  t += (dpint)a[12] * b[11];
  t += (dpint)a[13] * b[10];
  t += (dpint)a[14] * b[9];
  t += (dpint)a[15] * b[8];
  t += (dpint)a[16] * b[7];
  t += (dpint)a[17] * b[6];
  t += (dpint)v6 * (dpint)p17;
  t += (dpint)v7 * (dpint)p16;
  t += (dpint)v8 * (dpint)p15;
  t += (dpint)v9 * (dpint)p14;
  t += (dpint)v10 * (dpint)p13;
  t += (dpint)v11 * (dpint)p12;
  t += (dpint)v12 * (dpint)p11;
  t += (dpint)v13 * (dpint)p10;
  t += (dpint)v14 * (dpint)p9;
  t += (dpint)v15 * (dpint)p8;
  t += (dpint)v16 * (dpint)p7;
  t += (dpint)v17 * (dpint)p6;
  c[5] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[7] * b[17];
  t += (dpint)a[8] * b[16];
  t += (dpint)a[9] * b[15];
  t += (dpint)a[10] * b[14];
  t += (dpint)a[11] * b[13];
  t += (dpint)a[12] * b[12];
  t += (dpint)a[13] * b[11];
  t += (dpint)a[14] * b[10];
  t += (dpint)a[15] * b[9];
  t += (dpint)a[16] * b[8];
  t += (dpint)a[17] * b[7];
  t += (dpint)v7 * (dpint)p17;
  t += (dpint)v8 * (dpint)p16;
  t += (dpint)v9 * (dpint)p15;
  t += (dpint)v10 * (dpint)p14;
  t += (dpint)v11 * (dpint)p13;
  t += (dpint)v12 * (dpint)p12;
  t += (dpint)v13 * (dpint)p11;
  t += (dpint)v14 * (dpint)p10;
  t += (dpint)v15 * (dpint)p9;
  t += (dpint)v16 * (dpint)p8;
  t += (dpint)v17 * (dpint)p7;
  c[6] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[8] * b[17];
  t += (dpint)a[9] * b[16];
  t += (dpint)a[10] * b[15];
  t += (dpint)a[11] * b[14];
  t += (dpint)a[12] * b[13];
  t += (dpint)a[13] * b[12];
  t += (dpint)a[14] * b[11];
  t += (dpint)a[15] * b[10];
  t += (dpint)a[16] * b[9];
  t += (dpint)a[17] * b[8];
  t += (dpint)v8 * (dpint)p17;
  t += (dpint)v9 * (dpint)p16;
  t += (dpint)v10 * (dpint)p15;
  t += (dpint)v11 * (dpint)p14;
  t += (dpint)v12 * (dpint)p13;
  t += (dpint)v13 * (dpint)p12;
  t += (dpint)v14 * (dpint)p11;
  t += (dpint)v15 * (dpint)p10;
  t += (dpint)v16 * (dpint)p9;
  t += (dpint)v17 * (dpint)p8;
  c[7] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[9] * b[17];
  t += (dpint)a[10] * b[16];
  t += (dpint)a[11] * b[15];
  t += (dpint)a[12] * b[14];
  t += (dpint)a[13] * b[13];
  t += (dpint)a[14] * b[12];
  t += (dpint)a[15] * b[11];
  t += (dpint)a[16] * b[10];
  t += (dpint)a[17] * b[9];
  t += (dpint)v9 * (dpint)p17;
  t += (dpint)v10 * (dpint)p16;
  t += (dpint)v11 * (dpint)p15;
  t += (dpint)v12 * (dpint)p14;
  t += (dpint)v13 * (dpint)p13;
  t += (dpint)v14 * (dpint)p12;
  t += (dpint)v15 * (dpint)p11;
  t += (dpint)v16 * (dpint)p10;
  t += (dpint)v17 * (dpint)p9;
  c[8] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[10] * b[17];
  t += (dpint)a[11] * b[16];
  t += (dpint)a[12] * b[15];
  t += (dpint)a[13] * b[14];
  t += (dpint)a[14] * b[13];
  t += (dpint)a[15] * b[12];
  t += (dpint)a[16] * b[11];
  t += (dpint)a[17] * b[10];
  t += (dpint)v10 * (dpint)p17;
  t += (dpint)v11 * (dpint)p16;
  t += (dpint)v12 * (dpint)p15;
  t += (dpint)v13 * (dpint)p14;
  t += (dpint)v14 * (dpint)p13;
  t += (dpint)v15 * (dpint)p12;
  t += (dpint)v16 * (dpint)p11;
  t += (dpint)v17 * (dpint)p10;
  c[9] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[11] * b[17];
  t += (dpint)a[12] * b[16];
  t += (dpint)a[13] * b[15];
  t += (dpint)a[14] * b[14];
  t += (dpint)a[15] * b[13];
  t += (dpint)a[16] * b[12];
  t += (dpint)a[17] * b[11];
  t += (dpint)v11 * (dpint)p17;
  t += (dpint)v12 * (dpint)p16;
  t += (dpint)v13 * (dpint)p15;
  t += (dpint)v14 * (dpint)p14;
  t += (dpint)v15 * (dpint)p13;
  t += (dpint)v16 * (dpint)p12;
  t += (dpint)v17 * (dpint)p11;
  c[10] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[12] * b[17];
  t += (dpint)a[13] * b[16];
  t += (dpint)a[14] * b[15];
  t += (dpint)a[15] * b[14];
  t += (dpint)a[16] * b[13];
  t += (dpint)a[17] * b[12];
  t += (dpint)v12 * (dpint)p17;
  t += (dpint)v13 * (dpint)p16;
  t += (dpint)v14 * (dpint)p15;
  t += (dpint)v15 * (dpint)p14;
  t += (dpint)v16 * (dpint)p13;
  t += (dpint)v17 * (dpint)p12;
  c[11] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[13] * b[17];
  t += (dpint)a[14] * b[16];
  t += (dpint)a[15] * b[15];
  t += (dpint)a[16] * b[14];
  t += (dpint)a[17] * b[13];
  t += (dpint)v13 * (dpint)p17;
  t += (dpint)v14 * (dpint)p16;
  t += (dpint)v15 * (dpint)p15;
  t += (dpint)v16 * (dpint)p14;
  t += (dpint)v17 * (dpint)p13;
  c[12] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[14] * b[17];
  t += (dpint)a[15] * b[16];
  t += (dpint)a[16] * b[15];
  t += (dpint)a[17] * b[14];
  t += (dpint)v14 * (dpint)p17;
  t += (dpint)v15 * (dpint)p16;
  t += (dpint)v16 * (dpint)p15;
  t += (dpint)v17 * (dpint)p14;
  c[13] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[15] * b[17];
  t += (dpint)a[16] * b[16];
  t += (dpint)a[17] * b[15];
  t += (dpint)v15 * (dpint)p17;
  t += (dpint)v16 * (dpint)p16;
  t += (dpint)v17 * (dpint)p15;
  c[14] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[16] * b[17];
  t += (dpint)a[17] * b[16];
  t += (dpint)v16 * (dpint)p17;
  t += (dpint)v17 * (dpint)p16;
  c[15] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[17] * b[17];
  t += (dpint)v17 * (dpint)p17;
  c[16] = ((spint)t & mask);
  t >>= 28;
  c[17] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p5 = 0xada6e20u;
  spint p6 = 0xe994c68u;
  spint p7 = 0x781974cu;
  spint p8 = 0xaf0a29au;
  spint p9 = 0xdea65fu;
  spint p10 = 0xac5904au;
  spint p11 = 0x7d01fe3u;
  spint p12 = 0xe632650u;
  spint p13 = 0x9202bdbu;
  spint p14 = 0x36936e7u;
  spint p15 = 0x8c15b00u;
  spint p16 = 0x8869bc6u;
  spint p17 = 0x255946au;
  spint q = ((spint)1 << 28u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  tot = (udpint)a[0] * a[0];
  t = tot;
  spint v0 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  spint v1 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[2];
  tot *= 2;
  tot += (udpint)a[1] * a[1];
  t += tot;
  spint v2 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[3];
  tot += (udpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  spint v3 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[4];
  tot += (udpint)a[1] * a[3];
  tot *= 2;
  tot += (udpint)a[2] * a[2];
  t += tot;
  spint v4 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[5];
  tot += (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p5;
  spint v5 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[6];
  tot += (udpint)a[1] * a[5];
  tot += (udpint)a[2] * a[4];
  tot *= 2;
  tot += (udpint)a[3] * a[3];
  t += tot;
  t += (udpint)v0 * p6;
  t += (udpint)v1 * p5;
  spint v6 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[7];
  tot += (udpint)a[1] * a[6];
  tot += (udpint)a[2] * a[5];
  tot += (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p7;
  t += (udpint)v1 * p6;
  t += (udpint)v2 * p5;
  spint v7 = ((spint)t & mask);
  t >>= 28;
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
  spint v8 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[9];
  tot += (udpint)a[1] * a[8];
  tot += (udpint)a[2] * a[7];
  tot += (udpint)a[3] * a[6];
  tot += (udpint)a[4] * a[5];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p9;
  t += (udpint)v1 * p8;
  t += (udpint)v2 * p7;
  t += (udpint)v3 * p6;
  t += (udpint)v4 * p5;
  spint v9 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[10];
  tot += (udpint)a[1] * a[9];
  tot += (udpint)a[2] * a[8];
  tot += (udpint)a[3] * a[7];
  tot += (udpint)a[4] * a[6];
  tot *= 2;
  tot += (udpint)a[5] * a[5];
  t += tot;
  t += (udpint)v0 * p10;
  t += (udpint)v1 * p9;
  t += (udpint)v2 * p8;
  t += (udpint)v3 * p7;
  t += (udpint)v4 * p6;
  t += (udpint)v5 * p5;
  spint v10 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[11];
  tot += (udpint)a[1] * a[10];
  tot += (udpint)a[2] * a[9];
  tot += (udpint)a[3] * a[8];
  tot += (udpint)a[4] * a[7];
  tot += (udpint)a[5] * a[6];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p11;
  t += (udpint)v1 * p10;
  t += (udpint)v2 * p9;
  t += (udpint)v3 * p8;
  t += (udpint)v4 * p7;
  t += (udpint)v5 * p6;
  t += (udpint)v6 * p5;
  spint v11 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[12];
  tot += (udpint)a[1] * a[11];
  tot += (udpint)a[2] * a[10];
  tot += (udpint)a[3] * a[9];
  tot += (udpint)a[4] * a[8];
  tot += (udpint)a[5] * a[7];
  tot *= 2;
  tot += (udpint)a[6] * a[6];
  t += tot;
  t += (udpint)v0 * p12;
  t += (udpint)v1 * p11;
  t += (udpint)v2 * p10;
  t += (udpint)v3 * p9;
  t += (udpint)v4 * p8;
  t += (udpint)v5 * p7;
  t += (udpint)v6 * p6;
  t += (udpint)v7 * p5;
  spint v12 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[13];
  tot += (udpint)a[1] * a[12];
  tot += (udpint)a[2] * a[11];
  tot += (udpint)a[3] * a[10];
  tot += (udpint)a[4] * a[9];
  tot += (udpint)a[5] * a[8];
  tot += (udpint)a[6] * a[7];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p13;
  t += (udpint)v1 * p12;
  t += (udpint)v2 * p11;
  t += (udpint)v3 * p10;
  t += (udpint)v4 * p9;
  t += (udpint)v5 * p8;
  t += (udpint)v6 * p7;
  t += (udpint)v7 * p6;
  t += (udpint)v8 * p5;
  spint v13 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[14];
  tot += (udpint)a[1] * a[13];
  tot += (udpint)a[2] * a[12];
  tot += (udpint)a[3] * a[11];
  tot += (udpint)a[4] * a[10];
  tot += (udpint)a[5] * a[9];
  tot += (udpint)a[6] * a[8];
  tot *= 2;
  tot += (udpint)a[7] * a[7];
  t += tot;
  t += (udpint)v0 * p14;
  t += (udpint)v1 * p13;
  t += (udpint)v2 * p12;
  t += (udpint)v3 * p11;
  t += (udpint)v4 * p10;
  t += (udpint)v5 * p9;
  t += (udpint)v6 * p8;
  t += (udpint)v7 * p7;
  t += (udpint)v8 * p6;
  t += (udpint)v9 * p5;
  spint v14 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[15];
  tot += (udpint)a[1] * a[14];
  tot += (udpint)a[2] * a[13];
  tot += (udpint)a[3] * a[12];
  tot += (udpint)a[4] * a[11];
  tot += (udpint)a[5] * a[10];
  tot += (udpint)a[6] * a[9];
  tot += (udpint)a[7] * a[8];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p15;
  t += (udpint)v1 * p14;
  t += (udpint)v2 * p13;
  t += (udpint)v3 * p12;
  t += (udpint)v4 * p11;
  t += (udpint)v5 * p10;
  t += (udpint)v6 * p9;
  t += (udpint)v7 * p8;
  t += (udpint)v8 * p7;
  t += (udpint)v9 * p6;
  t += (udpint)v10 * p5;
  spint v15 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[16];
  tot += (udpint)a[1] * a[15];
  tot += (udpint)a[2] * a[14];
  tot += (udpint)a[3] * a[13];
  tot += (udpint)a[4] * a[12];
  tot += (udpint)a[5] * a[11];
  tot += (udpint)a[6] * a[10];
  tot += (udpint)a[7] * a[9];
  tot *= 2;
  tot += (udpint)a[8] * a[8];
  t += tot;
  t += (udpint)v0 * p16;
  t += (udpint)v1 * p15;
  t += (udpint)v2 * p14;
  t += (udpint)v3 * p13;
  t += (udpint)v4 * p12;
  t += (udpint)v5 * p11;
  t += (udpint)v6 * p10;
  t += (udpint)v7 * p9;
  t += (udpint)v8 * p8;
  t += (udpint)v9 * p7;
  t += (udpint)v10 * p6;
  t += (udpint)v11 * p5;
  spint v16 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[17];
  tot += (udpint)a[1] * a[16];
  tot += (udpint)a[2] * a[15];
  tot += (udpint)a[3] * a[14];
  tot += (udpint)a[4] * a[13];
  tot += (udpint)a[5] * a[12];
  tot += (udpint)a[6] * a[11];
  tot += (udpint)a[7] * a[10];
  tot += (udpint)a[8] * a[9];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p17;
  t += (udpint)v1 * p16;
  t += (udpint)v2 * p15;
  t += (udpint)v3 * p14;
  t += (udpint)v4 * p13;
  t += (udpint)v5 * p12;
  t += (udpint)v6 * p11;
  t += (udpint)v7 * p10;
  t += (udpint)v8 * p9;
  t += (udpint)v9 * p8;
  t += (udpint)v10 * p7;
  t += (udpint)v11 * p6;
  t += (udpint)v12 * p5;
  spint v17 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[1] * a[17];
  tot += (udpint)a[2] * a[16];
  tot += (udpint)a[3] * a[15];
  tot += (udpint)a[4] * a[14];
  tot += (udpint)a[5] * a[13];
  tot += (udpint)a[6] * a[12];
  tot += (udpint)a[7] * a[11];
  tot += (udpint)a[8] * a[10];
  tot *= 2;
  tot += (udpint)a[9] * a[9];
  t += tot;
  t += (udpint)v1 * p17;
  t += (udpint)v2 * p16;
  t += (udpint)v3 * p15;
  t += (udpint)v4 * p14;
  t += (udpint)v5 * p13;
  t += (udpint)v6 * p12;
  t += (udpint)v7 * p11;
  t += (udpint)v8 * p10;
  t += (udpint)v9 * p9;
  t += (udpint)v10 * p8;
  t += (udpint)v11 * p7;
  t += (udpint)v12 * p6;
  t += (udpint)v13 * p5;
  c[0] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[2] * a[17];
  tot += (udpint)a[3] * a[16];
  tot += (udpint)a[4] * a[15];
  tot += (udpint)a[5] * a[14];
  tot += (udpint)a[6] * a[13];
  tot += (udpint)a[7] * a[12];
  tot += (udpint)a[8] * a[11];
  tot += (udpint)a[9] * a[10];
  tot *= 2;
  t += tot;
  t += (udpint)v2 * p17;
  t += (udpint)v3 * p16;
  t += (udpint)v4 * p15;
  t += (udpint)v5 * p14;
  t += (udpint)v6 * p13;
  t += (udpint)v7 * p12;
  t += (udpint)v8 * p11;
  t += (udpint)v9 * p10;
  t += (udpint)v10 * p9;
  t += (udpint)v11 * p8;
  t += (udpint)v12 * p7;
  t += (udpint)v13 * p6;
  t += (udpint)v14 * p5;
  c[1] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[3] * a[17];
  tot += (udpint)a[4] * a[16];
  tot += (udpint)a[5] * a[15];
  tot += (udpint)a[6] * a[14];
  tot += (udpint)a[7] * a[13];
  tot += (udpint)a[8] * a[12];
  tot += (udpint)a[9] * a[11];
  tot *= 2;
  tot += (udpint)a[10] * a[10];
  t += tot;
  t += (udpint)v3 * p17;
  t += (udpint)v4 * p16;
  t += (udpint)v5 * p15;
  t += (udpint)v6 * p14;
  t += (udpint)v7 * p13;
  t += (udpint)v8 * p12;
  t += (udpint)v9 * p11;
  t += (udpint)v10 * p10;
  t += (udpint)v11 * p9;
  t += (udpint)v12 * p8;
  t += (udpint)v13 * p7;
  t += (udpint)v14 * p6;
  t += (udpint)v15 * p5;
  c[2] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[4] * a[17];
  tot += (udpint)a[5] * a[16];
  tot += (udpint)a[6] * a[15];
  tot += (udpint)a[7] * a[14];
  tot += (udpint)a[8] * a[13];
  tot += (udpint)a[9] * a[12];
  tot += (udpint)a[10] * a[11];
  tot *= 2;
  t += tot;
  t += (udpint)v4 * p17;
  t += (udpint)v5 * p16;
  t += (udpint)v6 * p15;
  t += (udpint)v7 * p14;
  t += (udpint)v8 * p13;
  t += (udpint)v9 * p12;
  t += (udpint)v10 * p11;
  t += (udpint)v11 * p10;
  t += (udpint)v12 * p9;
  t += (udpint)v13 * p8;
  t += (udpint)v14 * p7;
  t += (udpint)v15 * p6;
  t += (udpint)v16 * p5;
  c[3] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[5] * a[17];
  tot += (udpint)a[6] * a[16];
  tot += (udpint)a[7] * a[15];
  tot += (udpint)a[8] * a[14];
  tot += (udpint)a[9] * a[13];
  tot += (udpint)a[10] * a[12];
  tot *= 2;
  tot += (udpint)a[11] * a[11];
  t += tot;
  t += (udpint)v5 * p17;
  t += (udpint)v6 * p16;
  t += (udpint)v7 * p15;
  t += (udpint)v8 * p14;
  t += (udpint)v9 * p13;
  t += (udpint)v10 * p12;
  t += (udpint)v11 * p11;
  t += (udpint)v12 * p10;
  t += (udpint)v13 * p9;
  t += (udpint)v14 * p8;
  t += (udpint)v15 * p7;
  t += (udpint)v16 * p6;
  t += (udpint)v17 * p5;
  c[4] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[6] * a[17];
  tot += (udpint)a[7] * a[16];
  tot += (udpint)a[8] * a[15];
  tot += (udpint)a[9] * a[14];
  tot += (udpint)a[10] * a[13];
  tot += (udpint)a[11] * a[12];
  tot *= 2;
  t += tot;
  t += (udpint)v6 * p17;
  t += (udpint)v7 * p16;
  t += (udpint)v8 * p15;
  t += (udpint)v9 * p14;
  t += (udpint)v10 * p13;
  t += (udpint)v11 * p12;
  t += (udpint)v12 * p11;
  t += (udpint)v13 * p10;
  t += (udpint)v14 * p9;
  t += (udpint)v15 * p8;
  t += (udpint)v16 * p7;
  t += (udpint)v17 * p6;
  c[5] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[7] * a[17];
  tot += (udpint)a[8] * a[16];
  tot += (udpint)a[9] * a[15];
  tot += (udpint)a[10] * a[14];
  tot += (udpint)a[11] * a[13];
  tot *= 2;
  tot += (udpint)a[12] * a[12];
  t += tot;
  t += (udpint)v7 * p17;
  t += (udpint)v8 * p16;
  t += (udpint)v9 * p15;
  t += (udpint)v10 * p14;
  t += (udpint)v11 * p13;
  t += (udpint)v12 * p12;
  t += (udpint)v13 * p11;
  t += (udpint)v14 * p10;
  t += (udpint)v15 * p9;
  t += (udpint)v16 * p8;
  t += (udpint)v17 * p7;
  c[6] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[8] * a[17];
  tot += (udpint)a[9] * a[16];
  tot += (udpint)a[10] * a[15];
  tot += (udpint)a[11] * a[14];
  tot += (udpint)a[12] * a[13];
  tot *= 2;
  t += tot;
  t += (udpint)v8 * p17;
  t += (udpint)v9 * p16;
  t += (udpint)v10 * p15;
  t += (udpint)v11 * p14;
  t += (udpint)v12 * p13;
  t += (udpint)v13 * p12;
  t += (udpint)v14 * p11;
  t += (udpint)v15 * p10;
  t += (udpint)v16 * p9;
  t += (udpint)v17 * p8;
  c[7] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[9] * a[17];
  tot += (udpint)a[10] * a[16];
  tot += (udpint)a[11] * a[15];
  tot += (udpint)a[12] * a[14];
  tot *= 2;
  tot += (udpint)a[13] * a[13];
  t += tot;
  t += (udpint)v9 * p17;
  t += (udpint)v10 * p16;
  t += (udpint)v11 * p15;
  t += (udpint)v12 * p14;
  t += (udpint)v13 * p13;
  t += (udpint)v14 * p12;
  t += (udpint)v15 * p11;
  t += (udpint)v16 * p10;
  t += (udpint)v17 * p9;
  c[8] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[10] * a[17];
  tot += (udpint)a[11] * a[16];
  tot += (udpint)a[12] * a[15];
  tot += (udpint)a[13] * a[14];
  tot *= 2;
  t += tot;
  t += (udpint)v10 * p17;
  t += (udpint)v11 * p16;
  t += (udpint)v12 * p15;
  t += (udpint)v13 * p14;
  t += (udpint)v14 * p13;
  t += (udpint)v15 * p12;
  t += (udpint)v16 * p11;
  t += (udpint)v17 * p10;
  c[9] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[11] * a[17];
  tot += (udpint)a[12] * a[16];
  tot += (udpint)a[13] * a[15];
  tot *= 2;
  tot += (udpint)a[14] * a[14];
  t += tot;
  t += (udpint)v11 * p17;
  t += (udpint)v12 * p16;
  t += (udpint)v13 * p15;
  t += (udpint)v14 * p14;
  t += (udpint)v15 * p13;
  t += (udpint)v16 * p12;
  t += (udpint)v17 * p11;
  c[10] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[12] * a[17];
  tot += (udpint)a[13] * a[16];
  tot += (udpint)a[14] * a[15];
  tot *= 2;
  t += tot;
  t += (udpint)v12 * p17;
  t += (udpint)v13 * p16;
  t += (udpint)v14 * p15;
  t += (udpint)v15 * p14;
  t += (udpint)v16 * p13;
  t += (udpint)v17 * p12;
  c[11] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[13] * a[17];
  tot += (udpint)a[14] * a[16];
  tot *= 2;
  tot += (udpint)a[15] * a[15];
  t += tot;
  t += (udpint)v13 * p17;
  t += (udpint)v14 * p16;
  t += (udpint)v15 * p15;
  t += (udpint)v16 * p14;
  t += (udpint)v17 * p13;
  c[12] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[14] * a[17];
  tot += (udpint)a[15] * a[16];
  tot *= 2;
  t += tot;
  t += (udpint)v14 * p17;
  t += (udpint)v15 * p16;
  t += (udpint)v16 * p15;
  t += (udpint)v17 * p14;
  c[13] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[15] * a[17];
  tot *= 2;
  tot += (udpint)a[16] * a[16];
  t += tot;
  t += (udpint)v15 * p17;
  t += (udpint)v16 * p16;
  t += (udpint)v17 * p15;
  c[14] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[16] * a[17];
  tot *= 2;
  t += tot;
  t += (udpint)v16 * p17;
  t += (udpint)v17 * p16;
  c[15] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[17] * a[17];
  t += tot;
  t += (udpint)v17 * p17;
  c[16] = ((spint)t & mask);
  t >>= 28;
  c[17] = (spint)t;
}

// copy
static void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 18; i++) {
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
  spint x[18];
  spint t0[18];
  spint t1[18];
  spint t2[18];
  spint t3[18];
  spint t4[18];
  spint t5[18];
  spint t6[18];
  spint t7[18];
  spint t8[18];
  spint t9[18];
  spint t10[18];
  spint t11[18];
  spint t12[18];
  spint t13[18];
  spint t14[18];
  spint t15[18];
  spint t16[18];
  spint t17[18];
  spint t18[18];
  spint t19[18];
  spint t20[18];
  spint t21[18];
  spint t22[18];
  spint t23[18];
  spint t24[18];
  spint t25[18];
  spint t26[18];
  spint t27[18];
  spint t28[18];
  spint t29[18];
  spint t30[18];
  modcpy(w, x);
  modsqr(x, t16);
  modsqr(t16, t19);
  modmul(t16, t19, z);
  modmul(t16, z, t6);
  modmul(x, t6, t1);
  modmul(x, t1, t20);
  modmul(t16, t20, t8);
  modmul(z, t1, t9);
  modmul(z, t8, t13);
  modmul(t16, t13, t0);
  modmul(t1, t0, t17);
  modmul(z, t17, t26);
  modmul(t16, t26, t5);
  modmul(t20, t5, t12);
  modmul(t6, t12, t1);
  modmul(t8, t1, t27);
  modmul(t16, t27, t3);
  modmul(t19, t3, t22);
  modmul(t19, t22, t10);
  modmul(t19, t10, t11);
  modmul(t16, t11, t7);
  modmul(z, t7, t15);
  modmul(t20, t15, t4);
  modmul(t16, t4, t29);
  modmul(t6, t29, t2);
  modmul(t16, t2, t14);
  modmul(t13, t2, z);
  modmul(t19, z, t18);
  modmul(t13, t18, t21);
  modmul(t19, t21, t19);
  modmul(t16, t19, t24);
  modmul(t13, t24, t25);
  modmul(t16, t25, t21);
  modmul(t13, t25, t13);
  modmul(t8, t13, t8);
  modmul(t20, t8, t28);
  modmul(t20, t28, t20);
  modmul(t6, t20, t23);
  modmul(t16, t23, t6);
  modmul(t16, t6, t16);
  modmul(t0, t16, t0);
  modmul(t27, t23, t30);
  modnsqr(t30, 7);
  modmul(t29, t30, t29);
  modnsqr(t29, 11);
  modmul(t28, t29, t28);
  modnsqr(t28, 10);
  modmul(t27, t28, t27);
  modnsqr(t27, 9);
  modmul(t24, t27, t27);
  modnsqr(t27, 7);
  modmul(t4, t27, t27);
  modnsqr(t27, 7);
  modmul(t26, t27, t26);
  modnsqr(t26, 13);
  modmul(t25, t26, t25);
  modsqr(t25, t25);
  modmul(x, t25, t25);
  modnsqr(t25, 17);
  modmul(t2, t25, t25);
  modnsqr(t25, 10);
  modmul(t24, t25, t24);
  modnsqr(t24, 9);
  modmul(t23, t24, t23);
  modnsqr(t23, 7);
  modmul(t22, t23, t22);
  modnsqr(t22, 15);
  modmul(t21, t22, t21);
  modnsqr(t21, 9);
  modmul(t20, t21, t20);
  modnsqr(t20, 9);
  modmul(t4, t20, t20);
  modnsqr(t20, 10);
  modmul(t19, t20, t19);
  modnsqr(t19, 9);
  modmul(t18, t19, t18);
  modnsqr(t18, 5);
  modmul(t17, t18, t17);
  modnsqr(t17, 15);
  modmul(t0, t17, t17);
  modnsqr(t17, 11);
  modmul(t16, t17, t16);
  modnsqr(t16, 10);
  modmul(t15, t16, t15);
  modnsqr(t15, 11);
  modmul(t5, t15, t15);
  modnsqr(t15, 12);
  modmul(t14, t15, t14);
  modnsqr(t14, 8);
  modmul(t7, t14, t14);
  modnsqr(t14, 10);
  modmul(t13, t14, t13);
  modnsqr(t13, 7);
  modmul(t12, t13, t12);
  modnsqr(t12, 11);
  modmul(t11, t12, t11);
  modnsqr(t11, 8);
  modmul(t10, t11, t10);
  modnsqr(t10, 6);
  modmul(t9, t10, t9);
  modnsqr(t9, 14);
  modmul(t8, t9, t8);
  modnsqr(t8, 7);
  modmul(t7, t8, t7);
  modnsqr(t7, 10);
  modmul(t6, t7, t6);
  modnsqr(t6, 6);
  modmul(t5, t6, t5);
  modnsqr(t5, 9);
  modmul(t4, t5, t4);
  modnsqr(t4, 8);
  modmul(t3, t4, t3);
  modnsqr(t3, 8);
  modmul(t2, t3, t2);
  modnsqr(t2, 8);
  modmul(t1, t2, t1);
  modnsqr(t1, 12);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t1);
  modnsqr(t1, 8);
  modmul(t0, t1, t0);
  modnsqr(t0, 7);
  modmul(z, t0, z);
}

// calculate inverse, provide progenitor h if available
static void modinv(const spint *x, const spint *h, spint *z) {
  spint s[18];
  spint t[18];
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
  const spint c[18] = {
      0x8a0c754u, 0xa546e4eu, 0x943e89eu, 0xcb993b5u, 0xbf555c8u, 0x8dda851u,
      0x328de11u, 0xc717daau, 0xb090199u, 0x307cf4eu, 0xd096114u, 0x8537d92u,
      0x2dce4eeu, 0x96f0b3du, 0x5bc94fdu, 0x73aacacu, 0x943ddd4u, 0x16de108u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[18];
  c[0] = 1;
  for (i = 1; i < 18; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  modfsb(m);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[18];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 18; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 28u) &
          (((c0 ^ (spint)1) - (spint)1) >> 28u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[18];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 18; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 28u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 18; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 18; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[18];
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
  spint s[18];
  spint y[18];
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
  a[17] = ((a[17] << n)) | (a[16] >> (28u - n));
  for (i = 16; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0xfffffff) | (a[i - 1] >> (28u - n));
  }
  a[0] = (a[0] << n) & (spint)0xfffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 17; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (28u - n)) & (spint)0xfffffff);
  }
  a[17] = a[17] >> n;
  return r;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[18], d[18];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 18; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 28) & 1;
  }
  return eq;
}

/* API functions calling generated code */

const digit_t p[NWORDS_ORDER] =  { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xA6E1FFFF, 0x994C68AD, 0x781974CE, 0xFAF0A29A, 0x4A0DEA65, 0xFE3AC590, 0x26507D01, 0x02BDBE63, 0x6936E792, 0x8C15B003, 0xA8869BC6, 0x00255946 };
const digit_t pre[NWORDS_FIELD] = { 0xd612b0df, 0x55b8fc0e, 0xddbe7c8d, 0x06089756, 0xdace79eb, 0xc3dde686, 0x4129884e, 0x7e14cf99, 0x8a7cd13b, 0x21b60b8e, 0x0cdbb254, 0xa90c2a0a, 0x25d0c42f, 0x93a5d577, 0x1b5ad05c, 0x5feaf }; 

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
    for (int i = 0; i < 63; i++) {
        ((char *) out)[i] = x[0] & 0xff;
        modshr(8, x);
    }
}

void fp_from_digit_array(digit_t* out, const digit_t* a) {
    for (int i = 0; i < NWORDS_FIELD; i++) {
        out[i] = 0;
    }
    for (int i = 62; i >= 0; i--) {
        modshl(8, out);
        out[0] += (digit_t)((unsigned char *) a)[i];
    }
}

#define LIMB 28
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
