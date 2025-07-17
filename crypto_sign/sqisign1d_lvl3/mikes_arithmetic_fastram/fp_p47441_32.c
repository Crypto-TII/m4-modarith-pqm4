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
  for (i = 1; i < 13; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 28u;
  }
  n[13] += (spint)carry;
  return -((n[13] >> 1) >> 30u);
}

// propagate carries and add p if negative, propagate carries again
static void inline flatten(spint *n) {
  spint carry = prop(n);
  n[0] -= (spint)1u & carry;
  n[3] += ((spint)0x74c2000u) & carry;
  n[4] += ((spint)0x4684c61u) & carry;
  n[5] += ((spint)0x69356eau) & carry;
  n[6] += ((spint)0x1c722f6u) & carry;
  n[7] += ((spint)0x90aeb75u) & carry;
  n[8] += ((spint)0x5bc2e0au) & carry;
  n[9] += ((spint)0xd10ad66u) & carry;
  n[10] += ((spint)0xe604a45u) & carry;
  n[11] += ((spint)0x71a2c6au) & carry;
  n[12] += ((spint)0xeeeab08u) & carry;
  n[13] += ((spint)0x3df6u) & carry;
  (void)prop(n);
}

// Montgomery final subtract
static void modfsb(spint *n) {
  n[0] += (spint)1u;
  n[3] -= (spint)0x74c2000u;
  n[4] -= (spint)0x4684c61u;
  n[5] -= (spint)0x69356eau;
  n[6] -= (spint)0x1c722f6u;
  n[7] -= (spint)0x90aeb75u;
  n[8] -= (spint)0x5bc2e0au;
  n[9] -= (spint)0xd10ad66u;
  n[10] -= (spint)0xe604a45u;
  n[11] -= (spint)0x71a2c6au;
  n[12] -= (spint)0xeeeab08u;
  n[13] -= (spint)0x3df6u;
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
  n[0] += (spint)2u;
  n[3] -= (spint)0xe984000u;
  n[4] -= (spint)0x8d098c2u;
  n[5] -= (spint)0xd26add4u;
  n[6] -= (spint)0x38e45ecu;
  n[7] -= (spint)0x1215d6eau;
  n[8] -= (spint)0xb785c14u;
  n[9] -= (spint)0x1a215accu;
  n[10] -= (spint)0x1cc0948au;
  n[11] -= (spint)0xe3458d4u;
  n[12] -= (spint)0x1ddd5610u;
  n[13] -= (spint)0x7becu;
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[3] += ((spint)0xe984000u) & carry;
  n[4] += ((spint)0x8d098c2u) & carry;
  n[5] += ((spint)0xd26add4u) & carry;
  n[6] += ((spint)0x38e45ecu) & carry;
  n[7] += ((spint)0x1215d6eau) & carry;
  n[8] += ((spint)0xb785c14u) & carry;
  n[9] += ((spint)0x1a215accu) & carry;
  n[10] += ((spint)0x1cc0948au) & carry;
  n[11] += ((spint)0xe3458d4u) & carry;
  n[12] += ((spint)0x1ddd5610u) & carry;
  n[13] += ((spint)0x7becu) & carry;
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
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[3] += ((spint)0xe984000u) & carry;
  n[4] += ((spint)0x8d098c2u) & carry;
  n[5] += ((spint)0xd26add4u) & carry;
  n[6] += ((spint)0x38e45ecu) & carry;
  n[7] += ((spint)0x1215d6eau) & carry;
  n[8] += ((spint)0xb785c14u) & carry;
  n[9] += ((spint)0x1a215accu) & carry;
  n[10] += ((spint)0x1cc0948au) & carry;
  n[11] += ((spint)0xe3458d4u) & carry;
  n[12] += ((spint)0x1ddd5610u) & carry;
  n[13] += ((spint)0x7becu) & carry;
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
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[3] += ((spint)0xe984000u) & carry;
  n[4] += ((spint)0x8d098c2u) & carry;
  n[5] += ((spint)0xd26add4u) & carry;
  n[6] += ((spint)0x38e45ecu) & carry;
  n[7] += ((spint)0x1215d6eau) & carry;
  n[8] += ((spint)0xb785c14u) & carry;
  n[9] += ((spint)0x1a215accu) & carry;
  n[10] += ((spint)0x1cc0948au) & carry;
  n[11] += ((spint)0xe3458d4u) & carry;
  n[12] += ((spint)0x1ddd5610u) & carry;
  n[13] += ((spint)0x7becu) & carry;
  (void)prop(n);
}

// Overflow limit   = 18446744073709551616
// maximum possible = 1388517001176019771
// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p3 = 0x74c2000u;
  spint p4 = 0x4684c61u;
  spint p5 = 0x69356eau;
  spint p6 = 0x1c722f6u;
  spint p7 = 0x90aeb75u;
  spint p8 = 0x5bc2e0au;
  spint p9 = 0xd10ad66u;
  spint p10 = 0xe604a45u;
  spint p11 = 0x71a2c6au;
  spint p12 = 0xeeeab08u;
  spint p13 = 0x3df6u;
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
  t += (dpint)v0 * (dpint)p3;
  spint v3 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[4];
  t += (dpint)a[1] * b[3];
  t += (dpint)a[2] * b[2];
  t += (dpint)a[3] * b[1];
  t += (dpint)a[4] * b[0];
  t += (dpint)v0 * (dpint)p4;
  t += (dpint)v1 * (dpint)p3;
  spint v4 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[5];
  t += (dpint)a[1] * b[4];
  t += (dpint)a[2] * b[3];
  t += (dpint)a[3] * b[2];
  t += (dpint)a[4] * b[1];
  t += (dpint)a[5] * b[0];
  t += (dpint)v0 * (dpint)p5;
  t += (dpint)v1 * (dpint)p4;
  t += (dpint)v2 * (dpint)p3;
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
  t += (dpint)v2 * (dpint)p4;
  t += (dpint)v3 * (dpint)p3;
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
  t += (dpint)v3 * (dpint)p4;
  t += (dpint)v4 * (dpint)p3;
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
  t += (dpint)v4 * (dpint)p4;
  t += (dpint)v5 * (dpint)p3;
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
  t += (dpint)v5 * (dpint)p4;
  t += (dpint)v6 * (dpint)p3;
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
  t += (dpint)v6 * (dpint)p4;
  t += (dpint)v7 * (dpint)p3;
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
  t += (dpint)v7 * (dpint)p4;
  t += (dpint)v8 * (dpint)p3;
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
  t += (dpint)v8 * (dpint)p4;
  t += (dpint)v9 * (dpint)p3;
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
  t += (dpint)v9 * (dpint)p4;
  t += (dpint)v10 * (dpint)p3;
  spint v13 = ((spint)t & mask);
  t >>= 28;
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
  t += (dpint)v1 * (dpint)p13;
  t += (dpint)v2 * (dpint)p12;
  t += (dpint)v3 * (dpint)p11;
  t += (dpint)v4 * (dpint)p10;
  t += (dpint)v5 * (dpint)p9;
  t += (dpint)v6 * (dpint)p8;
  t += (dpint)v7 * (dpint)p7;
  t += (dpint)v8 * (dpint)p6;
  t += (dpint)v9 * (dpint)p5;
  t += (dpint)v10 * (dpint)p4;
  t += (dpint)v11 * (dpint)p3;
  c[0] = ((spint)t & mask);
  t >>= 28;
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
  t += (dpint)v2 * (dpint)p13;
  t += (dpint)v3 * (dpint)p12;
  t += (dpint)v4 * (dpint)p11;
  t += (dpint)v5 * (dpint)p10;
  t += (dpint)v6 * (dpint)p9;
  t += (dpint)v7 * (dpint)p8;
  t += (dpint)v8 * (dpint)p7;
  t += (dpint)v9 * (dpint)p6;
  t += (dpint)v10 * (dpint)p5;
  t += (dpint)v11 * (dpint)p4;
  t += (dpint)v12 * (dpint)p3;
  c[1] = ((spint)t & mask);
  t >>= 28;
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
  t += (dpint)v3 * (dpint)p13;
  t += (dpint)v4 * (dpint)p12;
  t += (dpint)v5 * (dpint)p11;
  t += (dpint)v6 * (dpint)p10;
  t += (dpint)v7 * (dpint)p9;
  t += (dpint)v8 * (dpint)p8;
  t += (dpint)v9 * (dpint)p7;
  t += (dpint)v10 * (dpint)p6;
  t += (dpint)v11 * (dpint)p5;
  t += (dpint)v12 * (dpint)p4;
  t += (dpint)v13 * (dpint)p3;
  c[2] = ((spint)t & mask);
  t >>= 28;
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
  t += (dpint)v4 * (dpint)p13;
  t += (dpint)v5 * (dpint)p12;
  t += (dpint)v6 * (dpint)p11;
  t += (dpint)v7 * (dpint)p10;
  t += (dpint)v8 * (dpint)p9;
  t += (dpint)v9 * (dpint)p8;
  t += (dpint)v10 * (dpint)p7;
  t += (dpint)v11 * (dpint)p6;
  t += (dpint)v12 * (dpint)p5;
  t += (dpint)v13 * (dpint)p4;
  c[3] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[5] * b[13];
  t += (dpint)a[6] * b[12];
  t += (dpint)a[7] * b[11];
  t += (dpint)a[8] * b[10];
  t += (dpint)a[9] * b[9];
  t += (dpint)a[10] * b[8];
  t += (dpint)a[11] * b[7];
  t += (dpint)a[12] * b[6];
  t += (dpint)a[13] * b[5];
  t += (dpint)v5 * (dpint)p13;
  t += (dpint)v6 * (dpint)p12;
  t += (dpint)v7 * (dpint)p11;
  t += (dpint)v8 * (dpint)p10;
  t += (dpint)v9 * (dpint)p9;
  t += (dpint)v10 * (dpint)p8;
  t += (dpint)v11 * (dpint)p7;
  t += (dpint)v12 * (dpint)p6;
  t += (dpint)v13 * (dpint)p5;
  c[4] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[6] * b[13];
  t += (dpint)a[7] * b[12];
  t += (dpint)a[8] * b[11];
  t += (dpint)a[9] * b[10];
  t += (dpint)a[10] * b[9];
  t += (dpint)a[11] * b[8];
  t += (dpint)a[12] * b[7];
  t += (dpint)a[13] * b[6];
  t += (dpint)v6 * (dpint)p13;
  t += (dpint)v7 * (dpint)p12;
  t += (dpint)v8 * (dpint)p11;
  t += (dpint)v9 * (dpint)p10;
  t += (dpint)v10 * (dpint)p9;
  t += (dpint)v11 * (dpint)p8;
  t += (dpint)v12 * (dpint)p7;
  t += (dpint)v13 * (dpint)p6;
  c[5] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[7] * b[13];
  t += (dpint)a[8] * b[12];
  t += (dpint)a[9] * b[11];
  t += (dpint)a[10] * b[10];
  t += (dpint)a[11] * b[9];
  t += (dpint)a[12] * b[8];
  t += (dpint)a[13] * b[7];
  t += (dpint)v7 * (dpint)p13;
  t += (dpint)v8 * (dpint)p12;
  t += (dpint)v9 * (dpint)p11;
  t += (dpint)v10 * (dpint)p10;
  t += (dpint)v11 * (dpint)p9;
  t += (dpint)v12 * (dpint)p8;
  t += (dpint)v13 * (dpint)p7;
  c[6] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[8] * b[13];
  t += (dpint)a[9] * b[12];
  t += (dpint)a[10] * b[11];
  t += (dpint)a[11] * b[10];
  t += (dpint)a[12] * b[9];
  t += (dpint)a[13] * b[8];
  t += (dpint)v8 * (dpint)p13;
  t += (dpint)v9 * (dpint)p12;
  t += (dpint)v10 * (dpint)p11;
  t += (dpint)v11 * (dpint)p10;
  t += (dpint)v12 * (dpint)p9;
  t += (dpint)v13 * (dpint)p8;
  c[7] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[9] * b[13];
  t += (dpint)a[10] * b[12];
  t += (dpint)a[11] * b[11];
  t += (dpint)a[12] * b[10];
  t += (dpint)a[13] * b[9];
  t += (dpint)v9 * (dpint)p13;
  t += (dpint)v10 * (dpint)p12;
  t += (dpint)v11 * (dpint)p11;
  t += (dpint)v12 * (dpint)p10;
  t += (dpint)v13 * (dpint)p9;
  c[8] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[10] * b[13];
  t += (dpint)a[11] * b[12];
  t += (dpint)a[12] * b[11];
  t += (dpint)a[13] * b[10];
  t += (dpint)v10 * (dpint)p13;
  t += (dpint)v11 * (dpint)p12;
  t += (dpint)v12 * (dpint)p11;
  t += (dpint)v13 * (dpint)p10;
  c[9] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[11] * b[13];
  t += (dpint)a[12] * b[12];
  t += (dpint)a[13] * b[11];
  t += (dpint)v11 * (dpint)p13;
  t += (dpint)v12 * (dpint)p12;
  t += (dpint)v13 * (dpint)p11;
  c[10] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[12] * b[13];
  t += (dpint)a[13] * b[12];
  t += (dpint)v12 * (dpint)p13;
  t += (dpint)v13 * (dpint)p12;
  c[11] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[13] * b[13];
  t += (dpint)v13 * (dpint)p13;
  c[12] = ((spint)t & mask);
  t >>= 28;
  c[13] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p3 = 0x74c2000u;
  spint p4 = 0x4684c61u;
  spint p5 = 0x69356eau;
  spint p6 = 0x1c722f6u;
  spint p7 = 0x90aeb75u;
  spint p8 = 0x5bc2e0au;
  spint p9 = 0xd10ad66u;
  spint p10 = 0xe604a45u;
  spint p11 = 0x71a2c6au;
  spint p12 = 0xeeeab08u;
  spint p13 = 0x3df6u;
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
  t += (udpint)v0 * p3;
  spint v3 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[4];
  tot += (udpint)a[1] * a[3];
  tot *= 2;
  tot += (udpint)a[2] * a[2];
  t += tot;
  t += (udpint)v0 * p4;
  t += (udpint)v1 * p3;
  spint v4 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[5];
  tot += (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p5;
  t += (udpint)v1 * p4;
  t += (udpint)v2 * p3;
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
  t += (udpint)v2 * p4;
  t += (udpint)v3 * p3;
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
  t += (udpint)v3 * p4;
  t += (udpint)v4 * p3;
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
  t += (udpint)v4 * p4;
  t += (udpint)v5 * p3;
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
  t += (udpint)v5 * p4;
  t += (udpint)v6 * p3;
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
  t += (udpint)v6 * p4;
  t += (udpint)v7 * p3;
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
  t += (udpint)v7 * p4;
  t += (udpint)v8 * p3;
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
  t += (udpint)v8 * p4;
  t += (udpint)v9 * p3;
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
  t += (udpint)v9 * p4;
  t += (udpint)v10 * p3;
  spint v13 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[1] * a[13];
  tot += (udpint)a[2] * a[12];
  tot += (udpint)a[3] * a[11];
  tot += (udpint)a[4] * a[10];
  tot += (udpint)a[5] * a[9];
  tot += (udpint)a[6] * a[8];
  tot *= 2;
  tot += (udpint)a[7] * a[7];
  t += tot;
  t += (udpint)v1 * p13;
  t += (udpint)v2 * p12;
  t += (udpint)v3 * p11;
  t += (udpint)v4 * p10;
  t += (udpint)v5 * p9;
  t += (udpint)v6 * p8;
  t += (udpint)v7 * p7;
  t += (udpint)v8 * p6;
  t += (udpint)v9 * p5;
  t += (udpint)v10 * p4;
  t += (udpint)v11 * p3;
  c[0] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[2] * a[13];
  tot += (udpint)a[3] * a[12];
  tot += (udpint)a[4] * a[11];
  tot += (udpint)a[5] * a[10];
  tot += (udpint)a[6] * a[9];
  tot += (udpint)a[7] * a[8];
  tot *= 2;
  t += tot;
  t += (udpint)v2 * p13;
  t += (udpint)v3 * p12;
  t += (udpint)v4 * p11;
  t += (udpint)v5 * p10;
  t += (udpint)v6 * p9;
  t += (udpint)v7 * p8;
  t += (udpint)v8 * p7;
  t += (udpint)v9 * p6;
  t += (udpint)v10 * p5;
  t += (udpint)v11 * p4;
  t += (udpint)v12 * p3;
  c[1] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[3] * a[13];
  tot += (udpint)a[4] * a[12];
  tot += (udpint)a[5] * a[11];
  tot += (udpint)a[6] * a[10];
  tot += (udpint)a[7] * a[9];
  tot *= 2;
  tot += (udpint)a[8] * a[8];
  t += tot;
  t += (udpint)v3 * p13;
  t += (udpint)v4 * p12;
  t += (udpint)v5 * p11;
  t += (udpint)v6 * p10;
  t += (udpint)v7 * p9;
  t += (udpint)v8 * p8;
  t += (udpint)v9 * p7;
  t += (udpint)v10 * p6;
  t += (udpint)v11 * p5;
  t += (udpint)v12 * p4;
  t += (udpint)v13 * p3;
  c[2] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[4] * a[13];
  tot += (udpint)a[5] * a[12];
  tot += (udpint)a[6] * a[11];
  tot += (udpint)a[7] * a[10];
  tot += (udpint)a[8] * a[9];
  tot *= 2;
  t += tot;
  t += (udpint)v4 * p13;
  t += (udpint)v5 * p12;
  t += (udpint)v6 * p11;
  t += (udpint)v7 * p10;
  t += (udpint)v8 * p9;
  t += (udpint)v9 * p8;
  t += (udpint)v10 * p7;
  t += (udpint)v11 * p6;
  t += (udpint)v12 * p5;
  t += (udpint)v13 * p4;
  c[3] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[5] * a[13];
  tot += (udpint)a[6] * a[12];
  tot += (udpint)a[7] * a[11];
  tot += (udpint)a[8] * a[10];
  tot *= 2;
  tot += (udpint)a[9] * a[9];
  t += tot;
  t += (udpint)v5 * p13;
  t += (udpint)v6 * p12;
  t += (udpint)v7 * p11;
  t += (udpint)v8 * p10;
  t += (udpint)v9 * p9;
  t += (udpint)v10 * p8;
  t += (udpint)v11 * p7;
  t += (udpint)v12 * p6;
  t += (udpint)v13 * p5;
  c[4] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[6] * a[13];
  tot += (udpint)a[7] * a[12];
  tot += (udpint)a[8] * a[11];
  tot += (udpint)a[9] * a[10];
  tot *= 2;
  t += tot;
  t += (udpint)v6 * p13;
  t += (udpint)v7 * p12;
  t += (udpint)v8 * p11;
  t += (udpint)v9 * p10;
  t += (udpint)v10 * p9;
  t += (udpint)v11 * p8;
  t += (udpint)v12 * p7;
  t += (udpint)v13 * p6;
  c[5] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[7] * a[13];
  tot += (udpint)a[8] * a[12];
  tot += (udpint)a[9] * a[11];
  tot *= 2;
  tot += (udpint)a[10] * a[10];
  t += tot;
  t += (udpint)v7 * p13;
  t += (udpint)v8 * p12;
  t += (udpint)v9 * p11;
  t += (udpint)v10 * p10;
  t += (udpint)v11 * p9;
  t += (udpint)v12 * p8;
  t += (udpint)v13 * p7;
  c[6] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[8] * a[13];
  tot += (udpint)a[9] * a[12];
  tot += (udpint)a[10] * a[11];
  tot *= 2;
  t += tot;
  t += (udpint)v8 * p13;
  t += (udpint)v9 * p12;
  t += (udpint)v10 * p11;
  t += (udpint)v11 * p10;
  t += (udpint)v12 * p9;
  t += (udpint)v13 * p8;
  c[7] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[9] * a[13];
  tot += (udpint)a[10] * a[12];
  tot *= 2;
  tot += (udpint)a[11] * a[11];
  t += tot;
  t += (udpint)v9 * p13;
  t += (udpint)v10 * p12;
  t += (udpint)v11 * p11;
  t += (udpint)v12 * p10;
  t += (udpint)v13 * p9;
  c[8] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[10] * a[13];
  tot += (udpint)a[11] * a[12];
  tot *= 2;
  t += tot;
  t += (udpint)v10 * p13;
  t += (udpint)v11 * p12;
  t += (udpint)v12 * p11;
  t += (udpint)v13 * p10;
  c[9] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[11] * a[13];
  tot *= 2;
  tot += (udpint)a[12] * a[12];
  t += tot;
  t += (udpint)v11 * p13;
  t += (udpint)v12 * p12;
  t += (udpint)v13 * p11;
  c[10] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[12] * a[13];
  tot *= 2;
  t += tot;
  t += (udpint)v12 * p13;
  t += (udpint)v13 * p12;
  c[11] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[13] * a[13];
  t += tot;
  t += (udpint)v13 * p13;
  c[12] = ((spint)t & mask);
  t >>= 28;
  c[13] = (spint)t;
}

// copy
static void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 14; i++) {
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
  spint x[14];
  spint t0[14];
  spint t1[14];
  spint t2[14];
  spint t3[14];
  spint t4[14];
  spint t5[14];
  spint t6[14];
  spint t7[14];
  spint t8[14];
  spint t9[14];
  spint t10[14];
  spint t11[14];
  spint t12[14];
  spint t13[14];
  spint t14[14];
  spint t15[14];
  spint t16[14];
  spint t17[14];
  spint t18[14];
  spint t19[14];
  spint t20[14];
  spint t21[14];
  spint t22[14];
  spint t23[14];
  spint t24[14];
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
  spint s[14];
  spint t[14];
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
  const spint c[14] = {0x17ee9b4u, 0x2686646u, 0xc47b3e8u, 0x157ca1au,
                       0x5365e7bu, 0xd3dc7f5u, 0xdb4bdeeu, 0xbe00e06u,
                       0x7ac68b0u, 0x3771313u, 0xc212c80u, 0xfdb63dau,
                       0x3db3011u, 0x23cfu};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[14];
  c[0] = 1;
  for (i = 1; i < 14; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  modfsb(m);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[14];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 14; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 28u) &
          (((c0 ^ (spint)1) - (spint)1) >> 28u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[14];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 14; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 28u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 14; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 14; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[14];
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
  spint s[14];
  spint y[14];
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
  a[13] = ((a[13] << n)) | (a[12] >> (28u - n));
  for (i = 12; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0xfffffff) | (a[i - 1] >> (28u - n));
  }
  a[0] = (a[0] << n) & (spint)0xfffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 13; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (28u - n)) & (spint)0xfffffff);
  }
  a[13] = a[13] >> n;
  return r;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[14], d[14];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 14; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 28) & 1;
  }
  return eq;
}

/* API functions calling generated code */

const digit_t p[NWORDS_ORDER] =  { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x4C6174C1, 0x356EA468, 0xC722F669, 0x90AEB751, 0x65BC2E0A, 0x45D10AD6, 0xC6AE604A, 0xAB0871A2, 0x03DF6EEE };
const digit_t pre[NWORDS_ORDER] = { 0x011d13c6, 0x209507bc, 0x18ede469, 0xd64fb447, 0x59dd54d3, 0xa39a06af, 0x2acfba5a, 0xa4ee313a, 0x2ea814d7, 0x4767febd, 0x196e62ce, 0x222acbe };

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
