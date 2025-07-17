#if defined(PQM4)

#include <stdint.h>
#include <stddef.h>

#include <stdbool.h>
#include <fp.h>

#define UMULL(rdlo, rdhi, rn, rm) asm volatile("umull %0, %1, %2, %3" : "=r"(rdlo), "=r"(rdhi) : "r"(rn), "r"(rm))
#define UMAAL(rdlo, rdhi, rn, rm) asm volatile("umaal %0, %1, %2, %3" : "+r"(rdlo), "+r"(rdhi) : "r"(rn), "r"(rm))
#define ADDS(rt, r1, r2) asm volatile("adds %0, %1, %2" : "+r"(rt) : "r"(r1), "r" (r2))
#define ADCS(rt, r1, r2) asm volatile("adcs %0, %1, %2" : "+r"(rt) : "r"(r1), "r" (r2))

#define spint uint32_t

static void modfsb_MFP318233_ct(spint *a) {
    const spint p[16] = {4294967295, 4294967295, 4294967295, 4294967295, 2799828991, 2571921581, 2014934222, 4210074266, 1242425957, 4265264528, 642809089, 45989475, 1765205906, 2350231555, 2827393990, 2447686};
    uint32_t t0, t1, t2, t3;
    uint32_t tmpvec[16];
    asm volatile(
        "ldrd %[t0], %[t1], [%[a]]\n"
        "ldrd %[t2], %[t3], [%[p]]\n"
        "subs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        "strd %[t0], %[t1], [%[tmpvec]]\n"
        "ldrd %[t0], %[t1], [%[a], #8]\n"
        "ldrd %[t2], %[t3], [%[p], #8]\n"
        "sbcs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        "strd %[t0], %[t1], [%[tmpvec], #8]\n"
        "ldrd %[t0], %[t1], [%[a], #16]\n"
        "ldrd %[t2], %[t3], [%[p], #16]\n"
        "sbcs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        "strd %[t0], %[t1], [%[tmpvec], #16]\n"
        "ldrd %[t0], %[t1], [%[a], #24]\n"
        "ldrd %[t2], %[t3], [%[p], #24]\n"
        "sbcs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        "strd %[t0], %[t1], [%[tmpvec], #24]\n"
        "ldrd %[t0], %[t1], [%[a], #32]\n"
        "ldrd %[t2], %[t3], [%[p], #32]\n"
        "sbcs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        "strd %[t0], %[t1], [%[tmpvec], #32]\n"
        "ldrd %[t0], %[t1], [%[a], #40]\n"
        "ldrd %[t2], %[t3], [%[p], #40]\n"
        "sbcs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        "strd %[t0], %[t1], [%[tmpvec], #40]\n"
        "ldrd %[t0], %[t1], [%[a], #48]\n"
        "ldrd %[t2], %[t3], [%[p], #48]\n"
        "sbcs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        "strd %[t0], %[t1], [%[tmpvec], #48]\n"
        "ldrd %[t0], %[t1], [%[a], #56]\n"
        "ldrd %[t2], %[t3], [%[p], #56]\n"
        "sbcs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        "strd %[t0], %[t1], [%[tmpvec], #56]\n"
        "itttt cs\n"
        "ldrdcs %[t0], %[t1], [%[tmpvec], #0]\n"
        "ldrdcs %[t2], %[t3], [%[tmpvec], #8]\n"
        "strdcs %[t0], %[t1], [%[a], #0]\n"
        "strdcs %[t2], %[t3], [%[a], #8]\n"
        "itttt cs\n"
        "ldrdcs %[t0], %[t1], [%[tmpvec], #16]\n"
        "ldrdcs %[t2], %[t3], [%[tmpvec], #24]\n"
        "strdcs %[t0], %[t1], [%[a], #16]\n"
        "strdcs %[t2], %[t3], [%[a], #24]\n"
        "itttt cs\n"
        "ldrdcs %[t0], %[t1], [%[tmpvec], #32]\n"
        "ldrdcs %[t2], %[t3], [%[tmpvec], #40]\n"
        "strdcs %[t0], %[t1], [%[a], #32]\n"
        "strdcs %[t2], %[t3], [%[a], #40]\n"
        "itttt cs\n"
        "ldrdcs %[t0], %[t1], [%[tmpvec], #48]\n"
        "ldrdcs %[t2], %[t3], [%[tmpvec], #56]\n"
        "strdcs %[t0], %[t1], [%[a], #48]\n"
        "strdcs %[t2], %[t3], [%[a], #56]\n"
        : [t0] "=&r" (t0), [t1] "=&r" (t1), [t2] "=&r" (t2), [t3] "=&r" (t3)
        : [a] "r" (a), [p] "r" (p), [tmpvec] "r" (tmpvec)
        : "cc", "memory");
}

static void modadd_MFP318233_ct(const spint *a, const spint *b, spint *res) {
    spint a0 = a[0], a1 = a[1], a2 = a[2], a3 = a[3], a4 = a[4], a5 = a[5], a6 = a[6], a7 = a[7], a8 = a[8], a9 = a[9], a10 = a[10], a11 = a[11], a12 = a[12], a13 = a[13], a14 = a[14], a15 = a[15];
    spint b0 = b[0], b1 = b[1], b2 = b[2], b3 = b[3], b4 = b[4], b5 = b[5], b6 = b[6], b7 = b[7], b8 = b[8], b9 = b[9], b10 = b[10], b11 = b[11], b12 = b[12], b13 = b[13], b14 = b[14], b15 = b[15];
    spint of = 0;
    spint nof[16];
    spint t = 0;
    const spint p0 = 4294967294;
    const spint p1 = 4294967295;
    const spint p2 = 4294967295;
    const spint p3 = 4294967295;
    const spint p4 = 1304690687;
    const spint p5 = 848875867;
    const spint p6 = 4029868445;
    const spint p7 = 4125181236;
    const spint p8 = 2484851915;
    const spint p9 = 4235561760;
    const spint p10 = 1285618179;
    const spint p11 = 91978950;
    const spint p12 = 3530411812;
    const spint p13 = 405495814;
    const spint p14 = 1359820685;
    const spint p15 = 4895373;
    UMAAL(a0, t, 1, b0);
    UMAAL(a1, t, 1, b1);
    UMAAL(a2, t, 1, b2);
    UMAAL(a3, t, 1, b3);
    UMAAL(a4, t, 1, b4);
    UMAAL(a5, t, 1, b5);
    UMAAL(a6, t, 1, b6);
    UMAAL(a7, t, 1, b7);
    UMAAL(a8, t, 1, b8);
    UMAAL(a9, t, 1, b9);
    UMAAL(a10, t, 1, b10);
    UMAAL(a11, t, 1, b11);
    UMAAL(a12, t, 1, b12);
    UMAAL(a13, t, 1, b13);
    UMAAL(a14, t, 1, b14);
    UMAAL(a15, t, 1, b15);
    of = t;
    res[0] = a0;
    res[1] = a1;
    res[2] = a2;
    res[3] = a3;
    res[4] = a4;
    res[5] = a5;
    res[6] = a6;
    res[7] = a7;
    res[8] = a8;
    res[9] = a9;
    res[10] = a10;
    res[11] = a11;
    res[12] = a12;
    res[13] = a13;
    res[14] = a14;
    res[15] = a15;
    spint sub = (of | (res[15] > p15));
    nof[0] = (res[0] - p0 > res[0]) & sub;
    nof[1] = (res[1] - p1 - nof[0] >= res[1]) & sub;
    nof[2] = (res[2] - p2 - nof[1] >= res[2]) & sub;
    nof[3] = (res[3] - p3 - nof[2] >= res[3]) & sub;
    nof[4] = (res[4] - p4 - nof[3] >= res[4]) & sub;
    nof[5] = (res[5] - p5 - nof[4] >= res[5]) & sub;
    nof[6] = (res[6] - p6 - nof[5] >= res[6]) & sub;
    nof[7] = (res[7] - p7 - nof[6] >= res[7]) & sub;
    nof[8] = (res[8] - p8 - nof[7] >= res[8]) & sub;
    nof[9] = (res[9] - p9 - nof[8] >= res[9]) & sub;
    nof[10] = (res[10] - p10 - nof[9] >= res[10]) & sub;
    nof[11] = (res[11] - p11 - nof[10] >= res[11]) & sub;
    nof[12] = (res[12] - p12 - nof[11] >= res[12]) & sub;
    nof[13] = (res[13] - p13 - nof[12] >= res[13]) & sub;
    nof[14] = (res[14] - p14 - nof[13] >= res[14]) & sub;
    nof[15] = (res[15] - p15 - nof[14] >= res[15]) & sub;
    res[0] = res[0] - p0*sub;
    res[1] = res[1] - p1*sub - nof[0];
    res[2] = res[2] - p2*sub - nof[1];
    res[3] = res[3] - p3*sub - nof[2];
    res[4] = res[4] - p4*sub - nof[3];
    res[5] = res[5] - p5*sub - nof[4];
    res[6] = res[6] - p6*sub - nof[5];
    res[7] = res[7] - p7*sub - nof[6];
    res[8] = res[8] - p8*sub - nof[7];
    res[9] = res[9] - p9*sub - nof[8];
    res[10] = res[10] - p10*sub - nof[9];
    res[11] = res[11] - p11*sub - nof[10];
    res[12] = res[12] - p12*sub - nof[11];
    res[13] = res[13] - p13*sub - nof[12];
    res[14] = res[14] - p14*sub - nof[13];
    res[15] = res[15] - p15*sub - nof[14];
}

static void modsub_MFP318233_ct(const spint *a, const spint *b, spint *res) {
    spint a0 = a[0], a1 = a[1], a2 = a[2], a3 = a[3], a4 = a[4], a5 = a[5], a6 = a[6], a7 = a[7], a8 = a[8], a9 = a[9], a10 = a[10], a11 = a[11], a12 = a[12], a13 = a[13], a14 = a[14], a15 = a[15];
    spint t, t1, t2;
    spint t3, t4;
    const spint p0 = 4294967294;
    const spint p1 = 4294967295;
    const spint p2 = 4294967295;
    const spint p3 = 4294967295;
    const spint p4 = 1304690687;
    const spint p5 = 848875867;
    const spint p6 = 4029868445;
    const spint p7 = 4125181236;
    const spint p8 = 2484851915;
    const spint p9 = 4235561760;
    const spint p10 = 1285618179;
    const spint p11 = 91978950;
    const spint p12 = 3530411812;
    const spint p13 = 405495814;
    const spint p14 = 1359820685;
    const spint p15 = 4895373;
    asm volatile(
        "mov %[t], #0\n"
        "ldrd %[t1], %[t2], [%[b]]\n"
        "ldrd %[t3], %[t4], [%[a]]\n"
        "subs %[t3], %[t3], %[t1]\n"
        "sbcs %[t4], %[t4], %[t2]\n"
        "strd %[t3], %[t4], [%[res]]\n"
        "ldrd %[t1], %[t2], [%[b], #8]\n"
        "ldrd %[t3], %[t4], [%[a], #8]\n"
        "sbcs %[t3], %[t3], %[t1]\n"
        "sbcs %[t4], %[t4], %[t2]\n"
        "strd %[t3], %[t4], [%[res], #8]\n"
        "ldrd %[t1], %[t2], [%[b], #16]\n"
        "ldrd %[t3], %[t4], [%[a], #16]\n"
        "sbcs %[t3], %[t3], %[t1]\n"
        "sbcs %[t4], %[t4], %[t2]\n"
        "strd %[t3], %[t4], [%[res], #16]\n"
        "ldrd %[t1], %[t2], [%[b], #24]\n"
        "ldrd %[t3], %[t4], [%[a], #24]\n"
        "sbcs %[t3], %[t3], %[t1]\n"
        "sbcs %[t4], %[t4], %[t2]\n"
        "strd %[t3], %[t4], [%[res], #24]\n"
        "ldrd %[t1], %[t2], [%[b], #32]\n"
        "ldrd %[t3], %[t4], [%[a], #32]\n"
        "sbcs %[t3], %[t3], %[t1]\n"
        "sbcs %[t4], %[t4], %[t2]\n"
        "strd %[t3], %[t4], [%[res], #32]\n"
        "ldrd %[t1], %[t2], [%[b], #40]\n"
        "ldrd %[t3], %[t4], [%[a], #40]\n"
        "sbcs %[t3], %[t3], %[t1]\n"
        "sbcs %[t4], %[t4], %[t2]\n"
        "strd %[t3], %[t4], [%[res], #40]\n"
        "ldrd %[t1], %[t2], [%[b], #48]\n"
        "ldrd %[t3], %[t4], [%[a], #48]\n"
        "sbcs %[t3], %[t3], %[t1]\n"
        "sbcs %[t4], %[t4], %[t2]\n"
        "strd %[t3], %[t4], [%[res], #48]\n"
        "ldrd %[t1], %[t2], [%[b], #56]\n"
        "ldrd %[t3], %[t4], [%[a], #56]\n"
        "sbcs %[t3], %[t3], %[t1]\n"
        "sbcs %[t4], %[t4], %[t2]\n"
        "strd %[t3], %[t4], [%[res], #56]\n"
        "sbcs %[t], %[t], %[t]\n"
        : [t] "=&r" (t), [t1] "=&r" (t1), [t2] "=&r" (t2), [t3] "=&r" (t3), [t4] "=&r" (t4)
        : [b] "r" (b), [a] "r" (a), [res] "r" (res)
        : "cc", "memory");
    t = -t | (res[15] > p15);
    t2 = 0;
    a0 = res[0];
    a1 = res[1];
    a2 = res[2];
    a3 = res[3];
    a4 = res[4];
    a5 = res[5];
    a6 = res[6];
    a7 = res[7];
    a8 = res[8];
    a9 = res[9];
    a10 = res[10];
    a11 = res[11];
    a12 = res[12];
    a13 = res[13];
    a14 = res[14];
    a15 = res[15];
    UMAAL(a0, t2, t, p0);
    UMAAL(a1, t2, t, p1);
    UMAAL(a2, t2, t, p2);
    UMAAL(a3, t2, t, p3);
    UMAAL(a4, t2, t, p4);
    UMAAL(a5, t2, t, p5);
    UMAAL(a6, t2, t, p6);
    UMAAL(a7, t2, t, p7);
    UMAAL(a8, t2, t, p8);
    UMAAL(a9, t2, t, p9);
    UMAAL(a10, t2, t, p10);
    UMAAL(a11, t2, t, p11);
    UMAAL(a12, t2, t, p12);
    UMAAL(a13, t2, t, p13);
    UMAAL(a14, t2, t, p14);
    UMAAL(a15, t2, t, p15);
    res[0] = a0;
    res[1] = a1;
    res[2] = a2;
    res[3] = a3;
    res[4] = a4;
    res[5] = a5;
    res[6] = a6;
    res[7] = a7;
    res[8] = a8;
    res[9] = a9;
    res[10] = a10;
    res[11] = a11;
    res[12] = a12;
    res[13] = a13;
    res[14] = a14;
    res[15] = a15;
}

static void modneg_MFP318233_ct(const spint *a, spint *r) {
    spint zero[16] = {0};
    modsub_MFP318233_ct(zero,a,r);
}
    
static void mul_MFP318233_ct(const spint *p, const spint *q, spint *r) {
    spint t0, t1, t2, t3, t4, a, b0, b1, b2, b3;
    b0 = q[0];
    b1 = q[1];
    b2 = q[2];
    b3 = q[3];
    asm volatile(
        ".align 4 \n"
        "mov.w  %[t1], #0 \n"
        "mov.w  %[t2], #0 \n"
        "mov.w  %[t3], #0 \n"
        "ldr.w   %[a], [%[p], #0] \n"
        "umull  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#4] \n"
        "str.w  %[t0], [%[r],#0] \n"
        "mov.w  %[t0], #0 \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#8] \n"
        "str.w  %[t1], [%[r],#4] \n"
        "mov.w  %[t1], #0 \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#12] \n"
        "str.w  %[t2], [%[r],#8] \n"
        "mov.w  %[t2], #0 \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#16] \n"
        "str.w  %[t3], [%[r],#12] \n"
        "mov.w  %[t3], #0 \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#20] \n"
        "str.w  %[t4], [%[r],#16] \n"
        "mov.w  %[t4], #0 \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#24] \n"
        "str.w  %[t0], [%[r],#20] \n"
        "mov.w  %[t0], #0 \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#28] \n"
        "str.w  %[t1], [%[r],#24] \n"
        "mov.w  %[t1], #0 \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#32] \n"
        "str.w  %[t2], [%[r],#28] \n"
        "mov.w  %[t2], #0 \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#36] \n"
        "str.w  %[t3], [%[r],#32] \n"
        "mov.w  %[t3], #0 \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#40] \n"
        "str.w  %[t4], [%[r],#36] \n"
        "mov.w  %[t4], #0 \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#44] \n"
        "str.w  %[t0], [%[r],#40] \n"
        "mov.w  %[t0], #0 \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#48] \n"
        "str.w  %[t1], [%[r],#44] \n"
        "mov.w  %[t1], #0 \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#52] \n"
        "str.w  %[t2], [%[r],#48] \n"
        "mov.w  %[t2], #0 \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#56] \n"
        "str.w  %[t3], [%[r],#52] \n"
        "mov.w  %[t3], #0 \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#60] \n"
        "str.w  %[t4], [%[r],#56] \n"
        "mov.w  %[t4], #0 \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "str.w  %[t0], [%[r],#60] \n"
        "str.w  %[t1], [%[r],#64] \n"
        "str.w  %[t2], [%[r],#68] \n"
        "str.w  %[t3], [%[r],#72] \n"
        "str.w  %[t4], [%[r],#76] \n"
        : [t0] "=&r"(t0), [t1] "=&r"(t1), [t2] "=&r"(t2), [t3] "=&r"(t3), [t4] "=&r"(t4), [a] "=&r"(a)
        : [b0] "r"(b0), [b1] "r"(b1), [b2] "r"(b2), [b3] "r"(b3), [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
    b0 = q[4]; b1 = q[5]; b2 = q[6]; b3 = q[7];
    t0 = r[4]; t1 = r[5]; t2 = r[6]; t3 = r[7]; t4 = 0;
    asm volatile(
        ".align 4 \n"
        "ldr.w   %[a], [%[p],#0] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#32] \n"
        "str.w  %[t0], [%[r],#16] \n"
        "mov.w  %[t0], #0 \n"
        "adds.w %[t4], %[t4], %[a] \n"
        "ldr.w   %[a], [%[p],#4] \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#36] \n"
        "str.w  %[t1], [%[r],#20] \n"
        "mov.w  %[t1], #0 \n"
        "adcs.w %[t0], %[t0], %[a] \n"
        "ldr.w   %[a], [%[p],#8] \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#40] \n"
        "str.w  %[t2], [%[r],#24] \n"
        "mov.w  %[t2], #0 \n"
        "adcs.w %[t1], %[t1], %[a] \n"
        "ldr.w   %[a], [%[p],#12] \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#44] \n"
        "str.w  %[t3], [%[r],#28] \n"
        "mov.w  %[t3], #0 \n"
        "adcs.w %[t2], %[t2], %[a] \n"
        "ldr.w   %[a], [%[p],#16] \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#48] \n"
        "str.w  %[t4], [%[r],#32] \n"
        "mov.w  %[t4], #0 \n"
        "adcs.w %[t3], %[t3], %[a] \n"
        "ldr.w   %[a], [%[p],#20] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#52] \n"
        "str.w  %[t0], [%[r],#36] \n"
        "mov.w  %[t0], #0 \n"
        "adcs.w %[t4], %[t4], %[a] \n"
        "ldr.w   %[a], [%[p],#24] \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#56] \n"
        "str.w  %[t1], [%[r],#40] \n"
        "mov.w  %[t1], #0 \n"
        "adcs.w %[t0], %[t0], %[a] \n"
        "ldr.w   %[a], [%[p],#28] \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#60] \n"
        "str.w  %[t2], [%[r],#44] \n"
        "mov.w  %[t2], #0 \n"
        "adcs.w %[t1], %[t1], %[a] \n"
        "ldr.w   %[a], [%[p],#32] \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#64] \n"
        "str.w  %[t3], [%[r],#48] \n"
        "mov.w  %[t3], #0 \n"
        "adcs.w %[t2], %[t2], %[a] \n"
        "ldr.w   %[a], [%[p],#36] \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#68] \n"
        "str.w  %[t4], [%[r],#52] \n"
        "mov.w  %[t4], #0 \n"
        "adcs.w %[t3], %[t3], %[a] \n"
        "ldr.w   %[a], [%[p],#40] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#72] \n"
        "str.w  %[t0], [%[r],#56] \n"
        "mov.w  %[t0], #0 \n"
        "adcs.w %[t4], %[t4], %[a] \n"
        "ldr.w   %[a], [%[p],#44] \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#76] \n"
        "str.w  %[t1], [%[r],#60] \n"
        "mov.w  %[t1], #0 \n"
        "adcs.w %[t0], %[t0], %[a] \n"
        "ldr.w   %[a], [%[p],#48] \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#52] \n"
        "str.w  %[t2], [%[r],#64] \n"
        "mov.w  %[t2], #0 \n"
#ifdef __clang__
        "adcs   %[t1], %[t1], #0\n"
#else
        "adcs.w %[t1], %[t1], #0\n"
#endif
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#56] \n"
        "str.w  %[t3], [%[r],#68] \n"
        "mov.w  %[t3], #0 \n"
#ifdef __clang__
        "adcs   %[t2], %[t2], #0\n"
#else
        "adcs.w %[t2], %[t2], #0\n"
#endif
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#60] \n"
        "str.w  %[t4], [%[r],#72] \n"
        "mov.w  %[t4], #0 \n"
#ifdef __clang__
        "adcs   %[t3], %[t3], #0\n"
#else
        "adcs.w %[t3], %[t3], #0\n"
#endif
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
#ifdef __clang__
        "adcs   %[t4], %[t4], #0 \n"
#else
        "adcs.w %[t4], %[t4], #0 \n"
#endif
        "str.w  %[t0], [%[r],#76] \n"
        "str.w  %[t1], [%[r],#80] \n"
        "str.w  %[t2], [%[r],#84] \n"
        "str.w  %[t3], [%[r],#88] \n"
        "str.w  %[t4], [%[r],#92] \n"
        "mov.w  %[t0], #0 \n"
#ifdef __clang__
        "adcs   %[t0], %[t0], #0 \n"
#else
        "adcs.w %[t0], %[t0], #0 \n"
#endif
        "str.w  %[t0], [%[r],#96] \n"
        : [t0] "+&r"(t0), [t1] "+&r"(t1), [t2] "+&r"(t2), [t3] "+&r"(t3), [t4] "+&r"(t4), [a] "=&r"(a)
        : [b0] "r"(b0), [b1] "r"(b1), [b2] "r"(b2), [b3] "r"(b3), [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
    b0 = q[8]; b1 = q[9]; b2 = q[10]; b3 = q[11];
    t0 = r[8]; t1 = r[9]; t2 = r[10]; t3 = r[11]; t4 = 0;
    asm volatile(
        ".align 4 \n"
        "ldr.w   %[a], [%[p],#0] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#48] \n"
        "str.w  %[t0], [%[r],#32] \n"
        "mov.w  %[t0], #0 \n"
        "adds.w %[t4], %[t4], %[a] \n"
        "ldr.w   %[a], [%[p],#4] \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#52] \n"
        "str.w  %[t1], [%[r],#36] \n"
        "mov.w  %[t1], #0 \n"
        "adcs.w %[t0], %[t0], %[a] \n"
        "ldr.w   %[a], [%[p],#8] \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#56] \n"
        "str.w  %[t2], [%[r],#40] \n"
        "mov.w  %[t2], #0 \n"
        "adcs.w %[t1], %[t1], %[a] \n"
        "ldr.w   %[a], [%[p],#12] \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#60] \n"
        "str.w  %[t3], [%[r],#44] \n"
        "mov.w  %[t3], #0 \n"
        "adcs.w %[t2], %[t2], %[a] \n"
        "ldr.w   %[a], [%[p],#16] \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#64] \n"
        "str.w  %[t4], [%[r],#48] \n"
        "mov.w  %[t4], #0 \n"
        "adcs.w %[t3], %[t3], %[a] \n"
        "ldr.w   %[a], [%[p],#20] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#68] \n"
        "str.w  %[t0], [%[r],#52] \n"
        "mov.w  %[t0], #0 \n"
        "adcs.w %[t4], %[t4], %[a] \n"
        "ldr.w   %[a], [%[p],#24] \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#72] \n"
        "str.w  %[t1], [%[r],#56] \n"
        "mov.w  %[t1], #0 \n"
        "adcs.w %[t0], %[t0], %[a] \n"
        "ldr.w   %[a], [%[p],#28] \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#76] \n"
        "str.w  %[t2], [%[r],#60] \n"
        "mov.w  %[t2], #0 \n"
        "adcs.w %[t1], %[t1], %[a] \n"
        "ldr.w   %[a], [%[p],#32] \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#80] \n"
        "str.w  %[t3], [%[r],#64] \n"
        "mov.w  %[t3], #0 \n"
        "adcs.w %[t2], %[t2], %[a] \n"
        "ldr.w   %[a], [%[p],#36] \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#84] \n"
        "str.w  %[t4], [%[r],#68] \n"
        "mov.w  %[t4], #0 \n"
        "adcs.w %[t3], %[t3], %[a] \n"
        "ldr.w   %[a], [%[p],#40] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#88] \n"
        "str.w  %[t0], [%[r],#72] \n"
        "mov.w  %[t0], #0 \n"
        "adcs.w %[t4], %[t4], %[a] \n"
        "ldr.w   %[a], [%[p],#44] \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#92] \n"
        "str.w  %[t1], [%[r],#76] \n"
        "mov.w  %[t1], #0 \n"
        "adcs.w %[t0], %[t0], %[a] \n"
        "ldr.w   %[a], [%[p],#48] \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#96] \n"
        "str.w  %[t2], [%[r],#80] \n"
        "mov.w  %[t2], #0 \n"
        "adcs.w %[t1], %[t1], %[a] \n"
        "ldr.w   %[a], [%[p],#52] \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#56] \n"
        "str.w  %[t3], [%[r],#84] \n"
        "mov.w  %[t3], #0 \n"
#ifdef __clang__
        "adcs   %[t2], %[t2], #0\n"
#else
        "adcs.w %[t2], %[t2], #0\n"
#endif
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#60] \n"
        "str.w  %[t4], [%[r],#88] \n"
        "mov.w  %[t4], #0 \n"
#ifdef __clang__
        "adcs   %[t3], %[t3], #0\n"
#else
        "adcs.w %[t3], %[t3], #0\n"
#endif
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
#ifdef __clang__
        "adcs   %[t4], %[t4], #0 \n"
#else
        "adcs.w %[t4], %[t4], #0 \n"
#endif
        "str.w  %[t0], [%[r],#92] \n"
        "str.w  %[t1], [%[r],#96] \n"
        "str.w  %[t2], [%[r],#100] \n"
        "str.w  %[t3], [%[r],#104] \n"
        "str.w  %[t4], [%[r],#108] \n"
        "mov.w  %[t0], #0 \n"
#ifdef __clang__
        "adcs   %[t0], %[t0], #0 \n"
#else
        "adcs.w %[t0], %[t0], #0 \n"
#endif
        "str.w  %[t0], [%[r],#112] \n"
        : [t0] "+&r"(t0), [t1] "+&r"(t1), [t2] "+&r"(t2), [t3] "+&r"(t3), [t4] "+&r"(t4), [a] "=&r"(a)
        : [b0] "r"(b0), [b1] "r"(b1), [b2] "r"(b2), [b3] "r"(b3), [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
    b0 = q[12]; b1 = q[13]; b2 = q[14]; b3 = q[15];
    t0 = r[12]; t1 = r[13]; t2 = r[14]; t3 = r[15]; t4 = 0;
    asm volatile(
        ".align 4 \n"
        "ldr.w   %[a], [%[p],#0] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#64] \n"
        "str.w  %[t0], [%[r],#48] \n"
        "mov.w  %[t0], #0 \n"
        "adds.w %[t4], %[t4], %[a] \n"
        "ldr.w   %[a], [%[p],#4] \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#68] \n"
        "str.w  %[t1], [%[r],#52] \n"
        "mov.w  %[t1], #0 \n"
        "adcs.w %[t0], %[t0], %[a] \n"
        "ldr.w   %[a], [%[p],#8] \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#72] \n"
        "str.w  %[t2], [%[r],#56] \n"
        "mov.w  %[t2], #0 \n"
        "adcs.w %[t1], %[t1], %[a] \n"
        "ldr.w   %[a], [%[p],#12] \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#76] \n"
        "str.w  %[t3], [%[r],#60] \n"
        "mov.w  %[t3], #0 \n"
        "adcs.w %[t2], %[t2], %[a] \n"
        "ldr.w   %[a], [%[p],#16] \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#80] \n"
        "str.w  %[t4], [%[r],#64] \n"
        "mov.w  %[t4], #0 \n"
        "adcs.w %[t3], %[t3], %[a] \n"
        "ldr.w   %[a], [%[p],#20] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#84] \n"
        "str.w  %[t0], [%[r],#68] \n"
        "mov.w  %[t0], #0 \n"
        "adcs.w %[t4], %[t4], %[a] \n"
        "ldr.w   %[a], [%[p],#24] \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#88] \n"
        "str.w  %[t1], [%[r],#72] \n"
        "mov.w  %[t1], #0 \n"
        "adcs.w %[t0], %[t0], %[a] \n"
        "ldr.w   %[a], [%[p],#28] \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#92] \n"
        "str.w  %[t2], [%[r],#76] \n"
        "mov.w  %[t2], #0 \n"
        "adcs.w %[t1], %[t1], %[a] \n"
        "ldr.w   %[a], [%[p],#32] \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#96] \n"
        "str.w  %[t3], [%[r],#80] \n"
        "mov.w  %[t3], #0 \n"
        "adcs.w %[t2], %[t2], %[a] \n"
        "ldr.w   %[a], [%[p],#36] \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#100] \n"
        "str.w  %[t4], [%[r],#84] \n"
        "mov.w  %[t4], #0 \n"
        "adcs.w %[t3], %[t3], %[a] \n"
        "ldr.w   %[a], [%[p],#40] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#104] \n"
        "str.w  %[t0], [%[r],#88] \n"
        "mov.w  %[t0], #0 \n"
        "adcs.w %[t4], %[t4], %[a] \n"
        "ldr.w   %[a], [%[p],#44] \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#108] \n"
        "str.w  %[t1], [%[r],#92] \n"
        "mov.w  %[t1], #0 \n"
        "adcs.w %[t0], %[t0], %[a] \n"
        "ldr.w   %[a], [%[p],#48] \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#112] \n"
        "str.w  %[t2], [%[r],#96] \n"
        "mov.w  %[t2], #0 \n"
        "adcs.w %[t1], %[t1], %[a] \n"
        "ldr.w   %[a], [%[p],#52] \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#56] \n"
        "str.w  %[t3], [%[r],#100] \n"
        "mov.w  %[t3], #0 \n"
#ifdef __clang__
        "adcs   %[t2], %[t2], #0\n"
#else
        "adcs.w %[t2], %[t2], #0\n"
#endif
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#60] \n"
        "str.w  %[t4], [%[r],#104] \n"
        "mov.w  %[t4], #0 \n"
#ifdef __clang__
        "adcs   %[t3], %[t3], #0\n"
#else
        "adcs.w %[t3], %[t3], #0\n"
#endif
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
#ifdef __clang__
        "adcs   %[t4], %[t4], #0 \n"
#else
        "adcs.w %[t4], %[t4], #0 \n"
#endif
        "str.w  %[t0], [%[r],#108] \n"
        "str.w  %[t1], [%[r],#112] \n"
        "str.w  %[t2], [%[r],#116] \n"
        "str.w  %[t3], [%[r],#120] \n"
        "str.w  %[t4], [%[r],#124] \n"
        : [t0] "+&r"(t0), [t1] "+&r"(t1), [t2] "+&r"(t2), [t3] "+&r"(t3), [t4] "+&r"(t4), [a] "=&r"(a)
        : [b0] "r"(b0), [b1] "r"(b1), [b2] "r"(b2), [b3] "r"(b3), [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
}

static void sqr_MFP318233_ct(const spint *p, spint *r) {
    spint t0, t1, t2, t3, t4, a, b0, b1, b2, b3;
    b0 = p[0];
    b1 = p[1];
    b2 = p[2];
    b3 = p[3];
    asm volatile(
        ".align 4 \n"
        "mov.w  %[t2], #0 \n"
        "mov.w  %[t3], #0 \n"
        "mov.w  %[t4], #0 \n"
        "umull  %[t0], %[t1], %[b0], %[b1]\n"
        "str.w  %[t0], [%[r],#4 ] \n"
        "mov.w  %[t0], #0 \n"
        "umaal  %[t1], %[t3], %[b0], %[b2]\n"
        "umaal  %[t2], %[t3], %[b1], %[b2]\n"
        "str.w  %[t1], [%[r],#8 ] \n"
        "mov.w  %[t1], #0 \n"
        "umaal  %[t2], %[t0], %[b0], %[b3]\n"
        "umaal  %[t3], %[t0], %[b1], %[b3]\n"
        "umaal  %[t4], %[t0], %[b2], %[b3]\n"
        "str.w  %[t2], [%[r],#12 ] \n"
        "mov.w  %[t2], #0 \n"
        "ldr.w   %[a], [%[p], #16] \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#20] \n"
        "str.w  %[t3], [%[r],#16] \n"
        "mov.w  %[t3], #0 \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#24] \n"
        "str.w  %[t4], [%[r],#20] \n"
        "mov.w  %[t4], #0 \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#28] \n"
        "str.w  %[t0], [%[r],#24] \n"
        "mov.w  %[t0], #0 \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#32] \n"
        "str.w  %[t1], [%[r],#28] \n"
        "mov.w  %[t1], #0 \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#36] \n"
        "str.w  %[t2], [%[r],#32] \n"
        "mov.w  %[t2], #0 \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#40] \n"
        "str.w  %[t3], [%[r],#36] \n"
        "mov.w  %[t3], #0 \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#44] \n"
        "str.w  %[t4], [%[r],#40] \n"
        "mov.w  %[t4], #0 \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#48] \n"
        "str.w  %[t0], [%[r],#44] \n"
        "mov.w  %[t0], #0 \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#52] \n"
        "str.w  %[t1], [%[r],#48] \n"
        "mov.w  %[t1], #0 \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#56] \n"
        "str.w  %[t2], [%[r],#52] \n"
        "mov.w  %[t2], #0 \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#60] \n"
        "str.w  %[t3], [%[r],#56] \n"
        "mov.w  %[t3], #0 \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "str.w  %[t4], [%[r],#60] \n"
        "str.w  %[t0], [%[r],#64] \n"
        "str.w  %[t1], [%[r],#68] \n"
        "str.w  %[t2], [%[r],#72] \n"
        "str.w  %[t3], [%[r],#76] \n"
        : [t4] "=&r"(t4), [t0] "=&r"(t0), [t1] "=&r"(t1), [t2] "=&r"(t2), [t3] "=&r"(t3), [a] "=&r"(a)
        : [b0] "r"(b0), [b1] "r"(b1), [b2] "r"(b2), [b3] "r"(b3), [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
    b0 = p[4]; b1 = p[5]; b2 = p[6]; b3 = p[7];
    t0 = r[9]; t1 = r[10]; t2 = r[11]; t3 = r[12]; t4 = r[13];
    asm volatile(
        ".align 4 \n"
        "mov.w  %[a], #0 \n"
        "umaal  %[t0], %[a], %[b0], %[b1]\n"
        "str.w  %[t0], [%[r],#36 ] \n"
        "mov.w  %[t0], #0 \n"
        "umaal  %[t1], %[a], %[b0], %[b2]\n"
        "umaal  %[t2], %[a], %[b1], %[b2]\n"
        "str.w  %[t1], [%[r],#40 ] \n"
        "mov.w  %[t1], #0 \n"
        "adds.w %[t3], %[t3], %[a] \n"
        "mov.w  %[a], #0 \n"
#ifdef __clang__
        "adcs   %[t4], %[t4], #0 \n"
#else
        "adcs.w %[t4], %[t4], #0 \n"
#endif
        "umaal  %[t2], %[a], %[b0], %[b3]\n"
        "umaal  %[t3], %[a], %[b1], %[b3]\n"
        "umaal  %[t4], %[a], %[b2], %[b3]\n"
        "ldr.w  %[t0], [%[r],#56] \n"
        "ldr.w  %[t1], [%[r],#60] \n"
        "str.w  %[t2], [%[r],#44 ] \n"
        "mov.w  %[t2], #0 \n"
#ifdef __clang__
        "adcs   %[t0], %[t0], %[a] \n"
        "adcs   %[t1], %[t1], #0 \n"
#else
        "adcs.w %[t0], %[t0], %[a] \n"
        "adcs.w %[t1], %[t1], #0 \n"
#endif
        "ldr.w   %[a], [%[p],#32] \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#64] \n"
        "str.w  %[t3], [%[r],#48] \n"
        "mov.w  %[t3], #0 \n"
#ifdef __clang__
        "adcs   %[t2], %[t2], %[a] \n"
#else
        "adcs.w %[t2], %[t2], %[a] \n"
#endif
        "ldr.w   %[a], [%[p],#36] \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#68] \n"
        "str.w  %[t4], [%[r],#52] \n"
        "mov.w  %[t4], #0 \n"
#ifdef __clang__
        "adcs   %[t3], %[t3], %[a] \n"
#else
        "adcs.w %[t3], %[t3], %[a] \n"
#endif
        "ldr.w   %[a], [%[p],#40] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#72] \n"
        "str.w  %[t0], [%[r],#56] \n"
        "mov.w  %[t0], #0 \n"
#ifdef __clang__
        "adcs   %[t4], %[t4], %[a] \n"
#else
        "adcs.w %[t4], %[t4], %[a] \n"
#endif
        "ldr.w   %[a], [%[p],#44] \n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#76] \n"
        "str.w  %[t1], [%[r],#60] \n"
        "mov.w  %[t1], #0 \n"
#ifdef __clang__
        "adcs   %[t0], %[t0], %[a] \n"
#else
        "adcs.w %[t0], %[t0], %[a] \n"
#endif
        "ldr.w   %[a], [%[p],#48] \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#52] \n"
        "str.w  %[t2], [%[r],#64] \n"
        "mov.w  %[t2], #0 \n"
#ifdef __clang__
        "adcs   %[t1], %[t1], #0\n"
#else
        "adcs.w %[t1], %[t1], #0\n"
#endif
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#56] \n"
        "str.w  %[t3], [%[r],#68] \n"
        "mov.w  %[t3], #0 \n"
#ifdef __clang__
        "adcs   %[t2], %[t2], #0\n"
#else
        "adcs.w %[t2], %[t2], #0\n"
#endif
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#60] \n"
        "str.w  %[t4], [%[r],#72] \n"
        "mov.w  %[t4], #0 \n"
#ifdef __clang__
        "adcs   %[t3], %[t3], #0\n"
#else
        "adcs.w %[t3], %[t3], #0\n"
#endif
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
#ifdef __clang__
        "adcs   %[t4], %[t4], #0 \n"
#else
        "adcs.w %[t4], %[t4], #0 \n"
#endif
        "str.w  %[t0], [%[r],#76] \n"
        "str.w  %[t1], [%[r],#80] \n"
        "str.w  %[t2], [%[r],#84] \n"
        "str.w  %[t3], [%[r],#88] \n"
        "str.w  %[t4], [%[r],#92] \n"
        "mov.w  %[t0], #0 \n"
#ifdef __clang__
        "adcs   %[t0], %[t0], #0 \n"
#else
        "adcs.w %[t0], %[t0], #0 \n"
#endif
        "str.w  %[t0], [%[r],#96] \n"
        : [t0] "+&r"(t0), [t1] "+&r"(t1), [t2] "+&r"(t2), [t3] "+&r"(t3), [t4] "+&r"(t4), [a] "=&r"(a)
        : [b0] "r"(b0), [b1] "r"(b1), [b2] "r"(b2), [b3] "r"(b3), [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
    b0 = p[8]; b1 = p[9]; b2 = p[10]; b3 = p[11];
    t0 = r[17]; t1 = r[18]; t2 = r[19]; t3 = r[20]; t4 = r[21];
    asm volatile(
        ".align 4 \n"
        "mov.w  %[a], #0 \n"
        "umaal  %[t0], %[a], %[b0], %[b1]\n"
        "str.w  %[t0], [%[r],#68 ] \n"
        "mov.w  %[t0], #0 \n"
        "umaal  %[t1], %[a], %[b0], %[b2]\n"
        "umaal  %[t2], %[a], %[b1], %[b2]\n"
        "str.w  %[t1], [%[r],#72 ] \n"
        "mov.w  %[t1], #0 \n"
        "adds.w %[t3], %[t3], %[a] \n"
        "mov.w  %[a], #0 \n"
#ifdef __clang__
        "adcs   %[t4], %[t4], #0 \n"
#else
        "adcs.w %[t4], %[t4], #0 \n"
#endif
        "umaal  %[t2], %[a], %[b0], %[b3]\n"
        "umaal  %[t3], %[a], %[b1], %[b3]\n"
        "umaal  %[t4], %[a], %[b2], %[b3]\n"
        "ldr.w  %[t0], [%[r],#88] \n"
        "ldr.w  %[t1], [%[r],#92] \n"
        "str.w  %[t2], [%[r],#76 ] \n"
        "mov.w  %[t2], #0 \n"
#ifdef __clang__
        "adcs   %[t0], %[t0], %[a] \n"
        "adcs   %[t1], %[t1], #0 \n"
#else
        "adcs.w %[t0], %[t0], %[a] \n"
        "adcs.w %[t1], %[t1], #0 \n"
#endif
        "ldr.w   %[a], [%[p],#48] \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#96] \n"
        "str.w  %[t3], [%[r],#80] \n"
        "mov.w  %[t3], #0 \n"
#ifdef __clang__
        "adcs   %[t2], %[t2], %[a] \n"
#else
        "adcs.w %[t2], %[t2], %[a] \n"
#endif
        "ldr.w   %[a], [%[p],#52] \n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#56] \n"
        "str.w  %[t4], [%[r],#84] \n"
        "mov.w  %[t4], #0 \n"
#ifdef __clang__
        "adcs   %[t3], %[t3], #0\n"
#else
        "adcs.w %[t3], %[t3], #0\n"
#endif
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p],#60] \n"
        "str.w  %[t0], [%[r],#88] \n"
        "mov.w  %[t0], #0 \n"
#ifdef __clang__
        "adcs   %[t4], %[t4], #0\n"
#else
        "adcs.w %[t4], %[t4], #0\n"
#endif
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
#ifdef __clang__
        "adcs   %[t0], %[t0], #0 \n"
#else
        "adcs.w %[t0], %[t0], #0 \n"
#endif
        "str.w  %[t1], [%[r],#92] \n"
        "str.w  %[t2], [%[r],#96] \n"
        "str.w  %[t3], [%[r],#100] \n"
        "str.w  %[t4], [%[r],#104] \n"
        "str.w  %[t0], [%[r],#108] \n"
        "mov.w  %[t1], #0 \n"
#ifdef __clang__
        "adcs   %[t1], %[t1], #0 \n"
#else
        "adcs.w %[t1], %[t1], #0 \n"
#endif
        "str.w  %[t1], [%[r],#112] \n"
        : [t0] "+&r"(t0), [t1] "+&r"(t1), [t2] "+&r"(t2), [t3] "+&r"(t3), [t4] "+&r"(t4), [a] "=&r"(a)
        : [b0] "r"(b0), [b1] "r"(b1), [b2] "r"(b2), [b3] "r"(b3), [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
    b0 = p[12]; b1 = p[13]; b2 = p[14]; b3 = p[15];
    t0 = r[25]; t1 = r[26]; t2 = r[27]; t3 = r[28]; t4 = 0;
    asm volatile(
        ".align 4 \n"
        "mov.w  %[a], #0 \n"
        "umaal  %[t0], %[t4], %[b0], %[b1]\n"
        "str.w  %[t0], [%[r],#100 ] \n"
        "mov.w  %[t0], #0 \n"
        "umaal  %[t1], %[t4], %[b0], %[b2]\n"
        "umaal  %[t2], %[t4], %[b1], %[b2]\n"
        "umaal  %[t2], %[t0], %[b0], %[b3]\n"
        "umaal  %[t3], %[t0], %[b1], %[b3]\n"
        "umaal  %[t3], %[t4], %[a],  %[a] \n"
        "umaal  %[t4], %[t0], %[b2], %[b3]\n"
        "str.w  %[t1], [%[r],#104 ] \n"
        "str.w  %[t2], [%[r],#108 ] \n"
        "str.w  %[t3], [%[r],#112 ] \n"
        "str.w  %[t4], [%[r],#116 ] \n"
        "str.w  %[t0], [%[r],#120 ] \n"
        : [t0] "+&r"(t0), [t1] "+&r"(t1), [t2] "+&r"(t2), [t3] "+&r"(t3), [t4] "+&r"(t4), [a] "=&r"(a)
        : [b0] "r"(b0), [b1] "r"(b1), [b2] "r"(b2), [b3] "r"(b3), [r] "r"(r)
        : "cc", "memory");
    r[0] = 0; r[31]=0;
    asm volatile(
        ".align 4 \n"
        "mov.w  %[a], #2 \n"
        "mov.w  %[t4], #0 \n"
        "ldr.w   %[b0], [%[p],#0] \n"
        "ldr.w   %[b1], [%[p],#4] \n"
        "umull  %[t0], %[t1], %[b0], %[b0]\n"
        "umull  %[t2], %[t3], %[b1], %[b1]\n"
        "ldr.w   %[b0], [%[r],#0] \n"
        "ldr.w   %[b1], [%[r],#4] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "ldr.w   %[b0], [%[r],#8] \n"
        "ldr.w   %[b1], [%[r],#12] \n"
        "str.w  %[t0], [%[r],#0 ] \n"
        "str.w  %[t1], [%[r],#4 ] \n"
        "umaal  %[t2], %[t4], %[a], %[b0]\n"
        "umaal  %[t3], %[t4], %[a], %[b1]\n"
        "str.w  %[t2], [%[r],#8 ] \n"
        "str.w  %[t3], [%[r],#12 ] \n"
        "ldr.w   %[b2], [%[p],#8] \n"
        "ldr.w   %[b3], [%[p],#12] \n"
        "umull  %[t0], %[t1], %[b2], %[b2]\n"
        "umull  %[t2], %[t3], %[b3], %[b3]\n"
        "ldr.w   %[b2], [%[r],#16] \n"
        "ldr.w   %[b3], [%[r],#20] \n"
        "umaal  %[t0], %[t4], %[a], %[b2]\n"
        "umaal  %[t1], %[t4], %[a], %[b3]\n"
        "ldr.w   %[b2], [%[r],#24] \n"
        "ldr.w   %[b3], [%[r],#28] \n"
        "str.w  %[t0], [%[r],#16 ] \n"
        "str.w  %[t1], [%[r],#20 ] \n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "str.w  %[t2], [%[r],#24 ] \n"
        "str.w  %[t3], [%[r],#28 ] \n"
        "ldr.w   %[b0], [%[p],#16] \n"
        "ldr.w   %[b1], [%[p],#20] \n"
        "umull  %[t0], %[t1], %[b0], %[b0]\n"
        "umull  %[t2], %[t3], %[b1], %[b1]\n"
        "ldr.w   %[b0], [%[r],#32] \n"
        "ldr.w   %[b1], [%[r],#36] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "ldr.w   %[b0], [%[r],#40] \n"
        "ldr.w   %[b1], [%[r],#44] \n"
        "str.w  %[t0], [%[r],#32 ] \n"
        "str.w  %[t1], [%[r],#36 ] \n"
        "umaal  %[t2], %[t4], %[a], %[b0]\n"
        "umaal  %[t3], %[t4], %[a], %[b1]\n"
        "str.w  %[t2], [%[r],#40 ] \n"
        "str.w  %[t3], [%[r],#44 ] \n"
        "ldr.w   %[b2], [%[p],#24] \n"
        "ldr.w   %[b3], [%[p],#28] \n"
        "umull  %[t0], %[t1], %[b2], %[b2]\n"
        "umull  %[t2], %[t3], %[b3], %[b3]\n"
        "ldr.w   %[b2], [%[r],#48] \n"
        "ldr.w   %[b3], [%[r],#52] \n"
        "umaal  %[t0], %[t4], %[a], %[b2]\n"
        "umaal  %[t1], %[t4], %[a], %[b3]\n"
        "ldr.w   %[b2], [%[r],#56] \n"
        "ldr.w   %[b3], [%[r],#60] \n"
        "str.w  %[t0], [%[r],#48 ] \n"
        "str.w  %[t1], [%[r],#52 ] \n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "str.w  %[t2], [%[r],#56 ] \n"
        "str.w  %[t3], [%[r],#60 ] \n"
        "ldr.w   %[b0], [%[p],#32] \n"
        "ldr.w   %[b1], [%[p],#36] \n"
        "umull  %[t0], %[t1], %[b0], %[b0]\n"
        "umull  %[t2], %[t3], %[b1], %[b1]\n"
        "ldr.w   %[b0], [%[r],#64] \n"
        "ldr.w   %[b1], [%[r],#68] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "ldr.w   %[b0], [%[r],#72] \n"
        "ldr.w   %[b1], [%[r],#76] \n"
        "str.w  %[t0], [%[r],#64 ] \n"
        "str.w  %[t1], [%[r],#68 ] \n"
        "umaal  %[t2], %[t4], %[a], %[b0]\n"
        "umaal  %[t3], %[t4], %[a], %[b1]\n"
        "str.w  %[t2], [%[r],#72 ] \n"
        "str.w  %[t3], [%[r],#76 ] \n"
        "ldr.w   %[b2], [%[p],#40] \n"
        "ldr.w   %[b3], [%[p],#44] \n"
        "umull  %[t0], %[t1], %[b2], %[b2]\n"
        "umull  %[t2], %[t3], %[b3], %[b3]\n"
        "ldr.w   %[b2], [%[r],#80] \n"
        "ldr.w   %[b3], [%[r],#84] \n"
        "umaal  %[t0], %[t4], %[a], %[b2]\n"
        "umaal  %[t1], %[t4], %[a], %[b3]\n"
        "ldr.w   %[b2], [%[r],#88] \n"
        "ldr.w   %[b3], [%[r],#92] \n"
        "str.w  %[t0], [%[r],#80 ] \n"
        "str.w  %[t1], [%[r],#84 ] \n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "str.w  %[t2], [%[r],#88 ] \n"
        "str.w  %[t3], [%[r],#92 ] \n"
        "ldr.w   %[b0], [%[p],#48] \n"
        "ldr.w   %[b1], [%[p],#52] \n"
        "umull  %[t0], %[t1], %[b0], %[b0]\n"
        "umull  %[t2], %[t3], %[b1], %[b1]\n"
        "ldr.w   %[b0], [%[r],#96] \n"
        "ldr.w   %[b1], [%[r],#100] \n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "ldr.w   %[b0], [%[r],#104] \n"
        "ldr.w   %[b1], [%[r],#108] \n"
        "str.w  %[t0], [%[r],#96 ] \n"
        "str.w  %[t1], [%[r],#100 ] \n"
        "umaal  %[t2], %[t4], %[a], %[b0]\n"
        "umaal  %[t3], %[t4], %[a], %[b1]\n"
        "str.w  %[t2], [%[r],#104 ] \n"
        "str.w  %[t3], [%[r],#108 ] \n"
        "ldr.w   %[b2], [%[p],#56] \n"
        "ldr.w   %[b3], [%[p],#60] \n"
        "umull  %[t0], %[t1], %[b2], %[b2]\n"
        "umull  %[t2], %[t3], %[b3], %[b3]\n"
        "ldr.w   %[b2], [%[r],#112] \n"
        "ldr.w   %[b3], [%[r],#116] \n"
        "umaal  %[t0], %[t4], %[a], %[b2]\n"
        "umaal  %[t1], %[t4], %[a], %[b3]\n"
        "ldr.w   %[b2], [%[r],#120] \n"
        "ldr.w   %[b3], [%[r],#124] \n"
        "str.w  %[t0], [%[r],#112 ] \n"
        "str.w  %[t1], [%[r],#116 ] \n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "str.w  %[t2], [%[r],#120 ] \n"
        "str.w  %[t3], [%[r],#124 ] \n"
        : [t0] "=&r"(t0), [t1] "=&r"(t1), [t2] "=&r"(t2), [t3] "=&r"(t3), [t4] "=&r"(t4), [a] "=&r"(a),
          [b0] "=&r"(b0), [b1] "=&r"(b1), [b2] "=&r"(b2), [b3] "=&r"(b3)
        : [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
}

static void mli_MFP318233_ct(const spint *p, const spint q, spint *r) {
    spint b, t0, t1, t2, t3, t4, t5, t6, t7, t8;
    b = q-1;
    t8 = 0;
    t0 = p[0];
    t1 = p[1];
    t2 = p[2];
    t3 = p[3];
    t4 = p[4];
    t5 = p[5];
    t6 = p[6];
    t7 = p[7];
    UMAAL(t0, t8, t0, b);
    UMAAL(t1, t8, t1, b);
    UMAAL(t2, t8, t2, b);
    UMAAL(t3, t8, t3, b);
    UMAAL(t4, t8, t4, b);
    UMAAL(t5, t8, t5, b);
    UMAAL(t6, t8, t6, b);
    UMAAL(t7, t8, t7, b);
    r[0] = t0;
    r[1] = t1;
    r[2] = t2;
    r[3] = t3;
    r[4] = t4;
    r[5] = t5;
    r[6] = t6;
    r[7] = t7;
    t0 = p[8];
    t1 = p[9];
    t2 = p[10];
    t3 = p[11];
    t4 = p[12];
    t5 = p[13];
    t6 = p[14];
    t7 = p[15];
    UMAAL(t0, t8, t0, b);
    UMAAL(t1, t8, t1, b);
    UMAAL(t2, t8, t2, b);
    UMAAL(t3, t8, t3, b);
    UMAAL(t4, t8, t4, b);
    UMAAL(t5, t8, t5, b);
    UMAAL(t6, t8, t6, b);
    UMAAL(t7, t8, t7, b);
    r[8] = t0;
    r[9] = t1;
    r[10] = t2;
    r[11] = t3;
    r[12] = t4;
    r[13] = t5;
    r[14] = t6;
    r[15] = t7;
    r[16] = t8;
}

static void modmul_MFP318233_ct(const spint *a, const spint *b, spint *res) {
    spint A[32];
    spint A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31;
    const spint p4 = 2799828991;
    const spint p5 = 2571921581;
    const spint p6 = 2014934222;
    const spint p7 = 4210074266;
    const spint p8 = 1242425957;
    const spint p9 = 4265264528;
    const spint p10 = 642809089;
    const spint p11 = 45989475;
    const spint p12 = 1765205906;
    const spint p13 = 2350231555;
    const spint p14 = 2827393990;
    const spint p15 = 2447686;
    spint q, t, t2 = 0;

    mul_MFP318233_ct(a, b, A);

    A0 = A[0];
    A1 = A[1];
    A2 = A[2];
    A3 = A[3];
    A4 = A[4];
    A5 = A[5];
    A6 = A[6];
    A7 = A[7];
    A8 = A[8];
    A9 = A[9];
    A10 = A[10];
    A11 = A[11];
    A12 = A[12];
    A13 = A[13];
    A14 = A[14];
    A15 = A[15];
    A16 = A[16];
    A17 = A[17];
    A18 = A[18];
    A19 = A[19];
    A20 = A[20];
    A21 = A[21];
    A22 = A[22];
    A23 = A[23];
    A24 = A[24];
    A25 = A[25];
    A26 = A[26];
    A27 = A[27];
    A28 = A[28];
    A29 = A[29];
    A30 = A[30];
    A31 = A[31];

    q = A0;
    t = 0;
    UMAAL(A4, t, q, p4 + 1);
    UMAAL(A5, t, q, p5);
    UMAAL(A6, t, q, p6);
    UMAAL(A7, t, q, p7);
    UMAAL(A8, t, q, p8);
    UMAAL(A9, t, q, p9);
    UMAAL(A10, t, q, p10);
    UMAAL(A11, t, q, p11);
    UMAAL(A12, t, q, p12);
    UMAAL(A13, t, q, p13);
    UMAAL(A14, t, q, p14);
    UMAAL(A15, t, q, p15);
    UMAAL(A16, t2, 1, t);

    q = A1;
    t = 0;
    UMAAL(A5, t, q, p4 + 1);
    UMAAL(A6, t, q, p5);
    UMAAL(A7, t, q, p6);
    UMAAL(A8, t, q, p7);
    UMAAL(A9, t, q, p8);
    UMAAL(A10, t, q, p9);
    UMAAL(A11, t, q, p10);
    UMAAL(A12, t, q, p11);
    UMAAL(A13, t, q, p12);
    UMAAL(A14, t, q, p13);
    UMAAL(A15, t, q, p14);
    UMAAL(A16, t, q, p15);
    UMAAL(A17, t2, 1, t);

    q = A2;
    t = 0;
    UMAAL(A6, t, q, p4 + 1);
    UMAAL(A7, t, q, p5);
    UMAAL(A8, t, q, p6);
    UMAAL(A9, t, q, p7);
    UMAAL(A10, t, q, p8);
    UMAAL(A11, t, q, p9);
    UMAAL(A12, t, q, p10);
    UMAAL(A13, t, q, p11);
    UMAAL(A14, t, q, p12);
    UMAAL(A15, t, q, p13);
    UMAAL(A16, t, q, p14);
    UMAAL(A17, t, q, p15);
    UMAAL(A18, t2, 1, t);

    q = A3;
    t = 0;
    UMAAL(A7, t, q, p4 + 1);
    UMAAL(A8, t, q, p5);
    UMAAL(A9, t, q, p6);
    UMAAL(A10, t, q, p7);
    UMAAL(A11, t, q, p8);
    UMAAL(A12, t, q, p9);
    UMAAL(A13, t, q, p10);
    UMAAL(A14, t, q, p11);
    UMAAL(A15, t, q, p12);
    UMAAL(A16, t, q, p13);
    UMAAL(A17, t, q, p14);
    UMAAL(A18, t, q, p15);
    UMAAL(A19, t2, 1, t);

    q = A4;
    t = 0;
    UMAAL(A8, t, q, p4 + 1);
    UMAAL(A9, t, q, p5);
    UMAAL(A10, t, q, p6);
    UMAAL(A11, t, q, p7);
    UMAAL(A12, t, q, p8);
    UMAAL(A13, t, q, p9);
    UMAAL(A14, t, q, p10);
    UMAAL(A15, t, q, p11);
    UMAAL(A16, t, q, p12);
    UMAAL(A17, t, q, p13);
    UMAAL(A18, t, q, p14);
    UMAAL(A19, t, q, p15);
    UMAAL(A20, t2, 1, t);

    q = A5;
    t = 0;
    UMAAL(A9, t, q, p4 + 1);
    UMAAL(A10, t, q, p5);
    UMAAL(A11, t, q, p6);
    UMAAL(A12, t, q, p7);
    UMAAL(A13, t, q, p8);
    UMAAL(A14, t, q, p9);
    UMAAL(A15, t, q, p10);
    UMAAL(A16, t, q, p11);
    UMAAL(A17, t, q, p12);
    UMAAL(A18, t, q, p13);
    UMAAL(A19, t, q, p14);
    UMAAL(A20, t, q, p15);
    UMAAL(A21, t2, 1, t);

    q = A6;
    t = 0;
    UMAAL(A10, t, q, p4 + 1);
    UMAAL(A11, t, q, p5);
    UMAAL(A12, t, q, p6);
    UMAAL(A13, t, q, p7);
    UMAAL(A14, t, q, p8);
    UMAAL(A15, t, q, p9);
    UMAAL(A16, t, q, p10);
    UMAAL(A17, t, q, p11);
    UMAAL(A18, t, q, p12);
    UMAAL(A19, t, q, p13);
    UMAAL(A20, t, q, p14);
    UMAAL(A21, t, q, p15);
    UMAAL(A22, t2, 1, t);

    q = A7;
    t = 0;
    UMAAL(A11, t, q, p4 + 1);
    UMAAL(A12, t, q, p5);
    UMAAL(A13, t, q, p6);
    UMAAL(A14, t, q, p7);
    UMAAL(A15, t, q, p8);
    UMAAL(A16, t, q, p9);
    UMAAL(A17, t, q, p10);
    UMAAL(A18, t, q, p11);
    UMAAL(A19, t, q, p12);
    UMAAL(A20, t, q, p13);
    UMAAL(A21, t, q, p14);
    UMAAL(A22, t, q, p15);
    UMAAL(A23, t2, 1, t);

    q = A8;
    t = 0;
    UMAAL(A12, t, q, p4 + 1);
    UMAAL(A13, t, q, p5);
    UMAAL(A14, t, q, p6);
    UMAAL(A15, t, q, p7);
    UMAAL(A16, t, q, p8);
    UMAAL(A17, t, q, p9);
    UMAAL(A18, t, q, p10);
    UMAAL(A19, t, q, p11);
    UMAAL(A20, t, q, p12);
    UMAAL(A21, t, q, p13);
    UMAAL(A22, t, q, p14);
    UMAAL(A23, t, q, p15);
    UMAAL(A24, t2, 1, t);

    q = A9;
    t = 0;
    UMAAL(A13, t, q, p4 + 1);
    UMAAL(A14, t, q, p5);
    UMAAL(A15, t, q, p6);
    UMAAL(A16, t, q, p7);
    UMAAL(A17, t, q, p8);
    UMAAL(A18, t, q, p9);
    UMAAL(A19, t, q, p10);
    UMAAL(A20, t, q, p11);
    UMAAL(A21, t, q, p12);
    UMAAL(A22, t, q, p13);
    UMAAL(A23, t, q, p14);
    UMAAL(A24, t, q, p15);
    UMAAL(A25, t2, 1, t);

    q = A10;
    t = 0;
    UMAAL(A14, t, q, p4 + 1);
    UMAAL(A15, t, q, p5);
    UMAAL(A16, t, q, p6);
    UMAAL(A17, t, q, p7);
    UMAAL(A18, t, q, p8);
    UMAAL(A19, t, q, p9);
    UMAAL(A20, t, q, p10);
    UMAAL(A21, t, q, p11);
    UMAAL(A22, t, q, p12);
    UMAAL(A23, t, q, p13);
    UMAAL(A24, t, q, p14);
    UMAAL(A25, t, q, p15);
    UMAAL(A26, t2, 1, t);

    q = A11;
    t = 0;
    UMAAL(A15, t, q, p4 + 1);
    UMAAL(A16, t, q, p5);
    UMAAL(A17, t, q, p6);
    UMAAL(A18, t, q, p7);
    UMAAL(A19, t, q, p8);
    UMAAL(A20, t, q, p9);
    UMAAL(A21, t, q, p10);
    UMAAL(A22, t, q, p11);
    UMAAL(A23, t, q, p12);
    UMAAL(A24, t, q, p13);
    UMAAL(A25, t, q, p14);
    UMAAL(A26, t, q, p15);
    UMAAL(A27, t2, 1, t);

    q = A12;
    t = 0;
    UMAAL(A16, t, q, p4 + 1);
    UMAAL(A17, t, q, p5);
    UMAAL(A18, t, q, p6);
    UMAAL(A19, t, q, p7);
    UMAAL(A20, t, q, p8);
    UMAAL(A21, t, q, p9);
    UMAAL(A22, t, q, p10);
    UMAAL(A23, t, q, p11);
    UMAAL(A24, t, q, p12);
    UMAAL(A25, t, q, p13);
    UMAAL(A26, t, q, p14);
    UMAAL(A27, t, q, p15);
    UMAAL(A28, t2, 1, t);

    q = A13;
    t = 0;
    UMAAL(A17, t, q, p4 + 1);
    UMAAL(A18, t, q, p5);
    UMAAL(A19, t, q, p6);
    UMAAL(A20, t, q, p7);
    UMAAL(A21, t, q, p8);
    UMAAL(A22, t, q, p9);
    UMAAL(A23, t, q, p10);
    UMAAL(A24, t, q, p11);
    UMAAL(A25, t, q, p12);
    UMAAL(A26, t, q, p13);
    UMAAL(A27, t, q, p14);
    UMAAL(A28, t, q, p15);
    UMAAL(A29, t2, 1, t);

    q = A14;
    t = 0;
    UMAAL(A18, t, q, p4 + 1);
    UMAAL(A19, t, q, p5);
    UMAAL(A20, t, q, p6);
    UMAAL(A21, t, q, p7);
    UMAAL(A22, t, q, p8);
    UMAAL(A23, t, q, p9);
    UMAAL(A24, t, q, p10);
    UMAAL(A25, t, q, p11);
    UMAAL(A26, t, q, p12);
    UMAAL(A27, t, q, p13);
    UMAAL(A28, t, q, p14);
    UMAAL(A29, t, q, p15);
    UMAAL(A30, t2, 1, t);

    q = A15;
    t = 0;
    UMAAL(A19, t, q, p4 + 1);
    UMAAL(A20, t, q, p5);
    UMAAL(A21, t, q, p6);
    UMAAL(A22, t, q, p7);
    UMAAL(A23, t, q, p8);
    UMAAL(A24, t, q, p9);
    UMAAL(A25, t, q, p10);
    UMAAL(A26, t, q, p11);
    UMAAL(A27, t, q, p12);
    UMAAL(A28, t, q, p13);
    UMAAL(A29, t, q, p14);
    UMAAL(A30, t, q, p15);
    UMAAL(A31, t2, 1, t);

    res[0] = A16;
    res[1] = A17;
    res[2] = A18;
    res[3] = A19;
    res[4] = A20;
    res[5] = A21;
    res[6] = A22;
    res[7] = A23;
    res[8] = A24;
    res[9] = A25;
    res[10] = A26;
    res[11] = A27;
    res[12] = A28;
    res[13] = A29;
    res[14] = A30;
    res[15] = A31;
}

static void modsqr_MFP318233_ct(const spint *a, spint *res) {
    spint A[32];
    spint A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31;
    const spint p4 = 2799828991;
    const spint p5 = 2571921581;
    const spint p6 = 2014934222;
    const spint p7 = 4210074266;
    const spint p8 = 1242425957;
    const spint p9 = 4265264528;
    const spint p10 = 642809089;
    const spint p11 = 45989475;
    const spint p12 = 1765205906;
    const spint p13 = 2350231555;
    const spint p14 = 2827393990;
    const spint p15 = 2447686;
    spint q, t, t2 = 0;

    sqr_MFP318233_ct(a, A);

    A0 = A[0];
    A1 = A[1];
    A2 = A[2];
    A3 = A[3];
    A4 = A[4];
    A5 = A[5];
    A6 = A[6];
    A7 = A[7];
    A8 = A[8];
    A9 = A[9];
    A10 = A[10];
    A11 = A[11];
    A12 = A[12];
    A13 = A[13];
    A14 = A[14];
    A15 = A[15];
    A16 = A[16];
    A17 = A[17];
    A18 = A[18];
    A19 = A[19];
    A20 = A[20];
    A21 = A[21];
    A22 = A[22];
    A23 = A[23];
    A24 = A[24];
    A25 = A[25];
    A26 = A[26];
    A27 = A[27];
    A28 = A[28];
    A29 = A[29];
    A30 = A[30];
    A31 = A[31];

    q = A0;
    t = 0;
    UMAAL(A4, t, q, p4 + 1);
    UMAAL(A5, t, q, p5);
    UMAAL(A6, t, q, p6);
    UMAAL(A7, t, q, p7);
    UMAAL(A8, t, q, p8);
    UMAAL(A9, t, q, p9);
    UMAAL(A10, t, q, p10);
    UMAAL(A11, t, q, p11);
    UMAAL(A12, t, q, p12);
    UMAAL(A13, t, q, p13);
    UMAAL(A14, t, q, p14);
    UMAAL(A15, t, q, p15);
    UMAAL(A16, t2, 1, t);

    q = A1;
    t = 0;
    UMAAL(A5, t, q, p4 + 1);
    UMAAL(A6, t, q, p5);
    UMAAL(A7, t, q, p6);
    UMAAL(A8, t, q, p7);
    UMAAL(A9, t, q, p8);
    UMAAL(A10, t, q, p9);
    UMAAL(A11, t, q, p10);
    UMAAL(A12, t, q, p11);
    UMAAL(A13, t, q, p12);
    UMAAL(A14, t, q, p13);
    UMAAL(A15, t, q, p14);
    UMAAL(A16, t, q, p15);
    UMAAL(A17, t2, 1, t);

    q = A2;
    t = 0;
    UMAAL(A6, t, q, p4 + 1);
    UMAAL(A7, t, q, p5);
    UMAAL(A8, t, q, p6);
    UMAAL(A9, t, q, p7);
    UMAAL(A10, t, q, p8);
    UMAAL(A11, t, q, p9);
    UMAAL(A12, t, q, p10);
    UMAAL(A13, t, q, p11);
    UMAAL(A14, t, q, p12);
    UMAAL(A15, t, q, p13);
    UMAAL(A16, t, q, p14);
    UMAAL(A17, t, q, p15);
    UMAAL(A18, t2, 1, t);

    q = A3;
    t = 0;
    UMAAL(A7, t, q, p4 + 1);
    UMAAL(A8, t, q, p5);
    UMAAL(A9, t, q, p6);
    UMAAL(A10, t, q, p7);
    UMAAL(A11, t, q, p8);
    UMAAL(A12, t, q, p9);
    UMAAL(A13, t, q, p10);
    UMAAL(A14, t, q, p11);
    UMAAL(A15, t, q, p12);
    UMAAL(A16, t, q, p13);
    UMAAL(A17, t, q, p14);
    UMAAL(A18, t, q, p15);
    UMAAL(A19, t2, 1, t);

    q = A4;
    t = 0;
    UMAAL(A8, t, q, p4 + 1);
    UMAAL(A9, t, q, p5);
    UMAAL(A10, t, q, p6);
    UMAAL(A11, t, q, p7);
    UMAAL(A12, t, q, p8);
    UMAAL(A13, t, q, p9);
    UMAAL(A14, t, q, p10);
    UMAAL(A15, t, q, p11);
    UMAAL(A16, t, q, p12);
    UMAAL(A17, t, q, p13);
    UMAAL(A18, t, q, p14);
    UMAAL(A19, t, q, p15);
    UMAAL(A20, t2, 1, t);

    q = A5;
    t = 0;
    UMAAL(A9, t, q, p4 + 1);
    UMAAL(A10, t, q, p5);
    UMAAL(A11, t, q, p6);
    UMAAL(A12, t, q, p7);
    UMAAL(A13, t, q, p8);
    UMAAL(A14, t, q, p9);
    UMAAL(A15, t, q, p10);
    UMAAL(A16, t, q, p11);
    UMAAL(A17, t, q, p12);
    UMAAL(A18, t, q, p13);
    UMAAL(A19, t, q, p14);
    UMAAL(A20, t, q, p15);
    UMAAL(A21, t2, 1, t);

    q = A6;
    t = 0;
    UMAAL(A10, t, q, p4 + 1);
    UMAAL(A11, t, q, p5);
    UMAAL(A12, t, q, p6);
    UMAAL(A13, t, q, p7);
    UMAAL(A14, t, q, p8);
    UMAAL(A15, t, q, p9);
    UMAAL(A16, t, q, p10);
    UMAAL(A17, t, q, p11);
    UMAAL(A18, t, q, p12);
    UMAAL(A19, t, q, p13);
    UMAAL(A20, t, q, p14);
    UMAAL(A21, t, q, p15);
    UMAAL(A22, t2, 1, t);

    q = A7;
    t = 0;
    UMAAL(A11, t, q, p4 + 1);
    UMAAL(A12, t, q, p5);
    UMAAL(A13, t, q, p6);
    UMAAL(A14, t, q, p7);
    UMAAL(A15, t, q, p8);
    UMAAL(A16, t, q, p9);
    UMAAL(A17, t, q, p10);
    UMAAL(A18, t, q, p11);
    UMAAL(A19, t, q, p12);
    UMAAL(A20, t, q, p13);
    UMAAL(A21, t, q, p14);
    UMAAL(A22, t, q, p15);
    UMAAL(A23, t2, 1, t);

    q = A8;
    t = 0;
    UMAAL(A12, t, q, p4 + 1);
    UMAAL(A13, t, q, p5);
    UMAAL(A14, t, q, p6);
    UMAAL(A15, t, q, p7);
    UMAAL(A16, t, q, p8);
    UMAAL(A17, t, q, p9);
    UMAAL(A18, t, q, p10);
    UMAAL(A19, t, q, p11);
    UMAAL(A20, t, q, p12);
    UMAAL(A21, t, q, p13);
    UMAAL(A22, t, q, p14);
    UMAAL(A23, t, q, p15);
    UMAAL(A24, t2, 1, t);

    q = A9;
    t = 0;
    UMAAL(A13, t, q, p4 + 1);
    UMAAL(A14, t, q, p5);
    UMAAL(A15, t, q, p6);
    UMAAL(A16, t, q, p7);
    UMAAL(A17, t, q, p8);
    UMAAL(A18, t, q, p9);
    UMAAL(A19, t, q, p10);
    UMAAL(A20, t, q, p11);
    UMAAL(A21, t, q, p12);
    UMAAL(A22, t, q, p13);
    UMAAL(A23, t, q, p14);
    UMAAL(A24, t, q, p15);
    UMAAL(A25, t2, 1, t);

    q = A10;
    t = 0;
    UMAAL(A14, t, q, p4 + 1);
    UMAAL(A15, t, q, p5);
    UMAAL(A16, t, q, p6);
    UMAAL(A17, t, q, p7);
    UMAAL(A18, t, q, p8);
    UMAAL(A19, t, q, p9);
    UMAAL(A20, t, q, p10);
    UMAAL(A21, t, q, p11);
    UMAAL(A22, t, q, p12);
    UMAAL(A23, t, q, p13);
    UMAAL(A24, t, q, p14);
    UMAAL(A25, t, q, p15);
    UMAAL(A26, t2, 1, t);

    q = A11;
    t = 0;
    UMAAL(A15, t, q, p4 + 1);
    UMAAL(A16, t, q, p5);
    UMAAL(A17, t, q, p6);
    UMAAL(A18, t, q, p7);
    UMAAL(A19, t, q, p8);
    UMAAL(A20, t, q, p9);
    UMAAL(A21, t, q, p10);
    UMAAL(A22, t, q, p11);
    UMAAL(A23, t, q, p12);
    UMAAL(A24, t, q, p13);
    UMAAL(A25, t, q, p14);
    UMAAL(A26, t, q, p15);
    UMAAL(A27, t2, 1, t);

    q = A12;
    t = 0;
    UMAAL(A16, t, q, p4 + 1);
    UMAAL(A17, t, q, p5);
    UMAAL(A18, t, q, p6);
    UMAAL(A19, t, q, p7);
    UMAAL(A20, t, q, p8);
    UMAAL(A21, t, q, p9);
    UMAAL(A22, t, q, p10);
    UMAAL(A23, t, q, p11);
    UMAAL(A24, t, q, p12);
    UMAAL(A25, t, q, p13);
    UMAAL(A26, t, q, p14);
    UMAAL(A27, t, q, p15);
    UMAAL(A28, t2, 1, t);

    q = A13;
    t = 0;
    UMAAL(A17, t, q, p4 + 1);
    UMAAL(A18, t, q, p5);
    UMAAL(A19, t, q, p6);
    UMAAL(A20, t, q, p7);
    UMAAL(A21, t, q, p8);
    UMAAL(A22, t, q, p9);
    UMAAL(A23, t, q, p10);
    UMAAL(A24, t, q, p11);
    UMAAL(A25, t, q, p12);
    UMAAL(A26, t, q, p13);
    UMAAL(A27, t, q, p14);
    UMAAL(A28, t, q, p15);
    UMAAL(A29, t2, 1, t);

    q = A14;
    t = 0;
    UMAAL(A18, t, q, p4 + 1);
    UMAAL(A19, t, q, p5);
    UMAAL(A20, t, q, p6);
    UMAAL(A21, t, q, p7);
    UMAAL(A22, t, q, p8);
    UMAAL(A23, t, q, p9);
    UMAAL(A24, t, q, p10);
    UMAAL(A25, t, q, p11);
    UMAAL(A26, t, q, p12);
    UMAAL(A27, t, q, p13);
    UMAAL(A28, t, q, p14);
    UMAAL(A29, t, q, p15);
    UMAAL(A30, t2, 1, t);

    q = A15;
    t = 0;
    UMAAL(A19, t, q, p4 + 1);
    UMAAL(A20, t, q, p5);
    UMAAL(A21, t, q, p6);
    UMAAL(A22, t, q, p7);
    UMAAL(A23, t, q, p8);
    UMAAL(A24, t, q, p9);
    UMAAL(A25, t, q, p10);
    UMAAL(A26, t, q, p11);
    UMAAL(A27, t, q, p12);
    UMAAL(A28, t, q, p13);
    UMAAL(A29, t, q, p14);
    UMAAL(A30, t, q, p15);
    UMAAL(A31, t2, 1, t);

    res[0] = A16;
    res[1] = A17;
    res[2] = A18;
    res[3] = A19;
    res[4] = A20;
    res[5] = A21;
    res[6] = A22;
    res[7] = A23;
    res[8] = A24;
    res[9] = A25;
    res[10] = A26;
    res[11] = A27;
    res[12] = A28;
    res[13] = A29;
    res[14] = A30;
    res[15] = A31;
}

static void modmli_MFP318233_ct(const spint *a, const spint b, spint *res) {
    spint A[32];
    spint A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31;
    const spint p4 = 2799828991;
    const spint p5 = 2571921581;
    const spint p6 = 2014934222;
    const spint p7 = 4210074266;
    const spint p8 = 1242425957;
    const spint p9 = 4265264528;
    const spint p10 = 642809089;
    const spint p11 = 45989475;
    const spint p12 = 1765205906;
    const spint p13 = 2350231555;
    const spint p14 = 2827393990;
    const spint p15 = 2447686;
    spint q, t, t2 = 0;

    mli_MFP318233_ct(a, b, A);

    A0 = A[0];
    A1 = A[1];
    A2 = A[2];
    A3 = A[3];
    A4 = A[4];
    A5 = A[5];
    A6 = A[6];
    A7 = A[7];
    A8 = A[8];
    A9 = A[9];
    A10 = A[10];
    A11 = A[11];
    A12 = A[12];
    A13 = A[13];
    A14 = A[14];
    A15 = A[15];
    A16 = A[16];
    A17 = 0;
    A18 = 0;
    A19 = 0;
    A20 = 0;
    A21 = 0;
    A22 = 0;
    A23 = 0;
    A24 = 0;
    A25 = 0;
    A26 = 0;
    A27 = 0;
    A28 = 0;
    A29 = 0;
    A30 = 0;
    A31 = 0;

    q = A0;
    t = 0;
    UMAAL(A4, t, q, p4 + 1);
    UMAAL(A5, t, q, p5);
    UMAAL(A6, t, q, p6);
    UMAAL(A7, t, q, p7);
    UMAAL(A8, t, q, p8);
    UMAAL(A9, t, q, p9);
    UMAAL(A10, t, q, p10);
    UMAAL(A11, t, q, p11);
    UMAAL(A12, t, q, p12);
    UMAAL(A13, t, q, p13);
    UMAAL(A14, t, q, p14);
    UMAAL(A15, t, q, p15);
    UMAAL(A16, t2, 1, t);

    q = A1;
    t = 0;
    UMAAL(A5, t, q, p4 + 1);
    UMAAL(A6, t, q, p5);
    UMAAL(A7, t, q, p6);
    UMAAL(A8, t, q, p7);
    UMAAL(A9, t, q, p8);
    UMAAL(A10, t, q, p9);
    UMAAL(A11, t, q, p10);
    UMAAL(A12, t, q, p11);
    UMAAL(A13, t, q, p12);
    UMAAL(A14, t, q, p13);
    UMAAL(A15, t, q, p14);
    UMAAL(A16, t, q, p15);
    UMAAL(A17, t2, 1, t);

    q = A2;
    t = 0;
    UMAAL(A6, t, q, p4 + 1);
    UMAAL(A7, t, q, p5);
    UMAAL(A8, t, q, p6);
    UMAAL(A9, t, q, p7);
    UMAAL(A10, t, q, p8);
    UMAAL(A11, t, q, p9);
    UMAAL(A12, t, q, p10);
    UMAAL(A13, t, q, p11);
    UMAAL(A14, t, q, p12);
    UMAAL(A15, t, q, p13);
    UMAAL(A16, t, q, p14);
    UMAAL(A17, t, q, p15);
    UMAAL(A18, t2, 1, t);

    q = A3;
    t = 0;
    UMAAL(A7, t, q, p4 + 1);
    UMAAL(A8, t, q, p5);
    UMAAL(A9, t, q, p6);
    UMAAL(A10, t, q, p7);
    UMAAL(A11, t, q, p8);
    UMAAL(A12, t, q, p9);
    UMAAL(A13, t, q, p10);
    UMAAL(A14, t, q, p11);
    UMAAL(A15, t, q, p12);
    UMAAL(A16, t, q, p13);
    UMAAL(A17, t, q, p14);
    UMAAL(A18, t, q, p15);
    UMAAL(A19, t2, 1, t);

    q = A4;
    t = 0;
    UMAAL(A8, t, q, p4 + 1);
    UMAAL(A9, t, q, p5);
    UMAAL(A10, t, q, p6);
    UMAAL(A11, t, q, p7);
    UMAAL(A12, t, q, p8);
    UMAAL(A13, t, q, p9);
    UMAAL(A14, t, q, p10);
    UMAAL(A15, t, q, p11);
    UMAAL(A16, t, q, p12);
    UMAAL(A17, t, q, p13);
    UMAAL(A18, t, q, p14);
    UMAAL(A19, t, q, p15);
    UMAAL(A20, t2, 1, t);

    q = A5;
    t = 0;
    UMAAL(A9, t, q, p4 + 1);
    UMAAL(A10, t, q, p5);
    UMAAL(A11, t, q, p6);
    UMAAL(A12, t, q, p7);
    UMAAL(A13, t, q, p8);
    UMAAL(A14, t, q, p9);
    UMAAL(A15, t, q, p10);
    UMAAL(A16, t, q, p11);
    UMAAL(A17, t, q, p12);
    UMAAL(A18, t, q, p13);
    UMAAL(A19, t, q, p14);
    UMAAL(A20, t, q, p15);
    UMAAL(A21, t2, 1, t);

    q = A6;
    t = 0;
    UMAAL(A10, t, q, p4 + 1);
    UMAAL(A11, t, q, p5);
    UMAAL(A12, t, q, p6);
    UMAAL(A13, t, q, p7);
    UMAAL(A14, t, q, p8);
    UMAAL(A15, t, q, p9);
    UMAAL(A16, t, q, p10);
    UMAAL(A17, t, q, p11);
    UMAAL(A18, t, q, p12);
    UMAAL(A19, t, q, p13);
    UMAAL(A20, t, q, p14);
    UMAAL(A21, t, q, p15);
    UMAAL(A22, t2, 1, t);

    q = A7;
    t = 0;
    UMAAL(A11, t, q, p4 + 1);
    UMAAL(A12, t, q, p5);
    UMAAL(A13, t, q, p6);
    UMAAL(A14, t, q, p7);
    UMAAL(A15, t, q, p8);
    UMAAL(A16, t, q, p9);
    UMAAL(A17, t, q, p10);
    UMAAL(A18, t, q, p11);
    UMAAL(A19, t, q, p12);
    UMAAL(A20, t, q, p13);
    UMAAL(A21, t, q, p14);
    UMAAL(A22, t, q, p15);
    UMAAL(A23, t2, 1, t);

    q = A8;
    t = 0;
    UMAAL(A12, t, q, p4 + 1);
    UMAAL(A13, t, q, p5);
    UMAAL(A14, t, q, p6);
    UMAAL(A15, t, q, p7);
    UMAAL(A16, t, q, p8);
    UMAAL(A17, t, q, p9);
    UMAAL(A18, t, q, p10);
    UMAAL(A19, t, q, p11);
    UMAAL(A20, t, q, p12);
    UMAAL(A21, t, q, p13);
    UMAAL(A22, t, q, p14);
    UMAAL(A23, t, q, p15);
    UMAAL(A24, t2, 1, t);

    q = A9;
    t = 0;
    UMAAL(A13, t, q, p4 + 1);
    UMAAL(A14, t, q, p5);
    UMAAL(A15, t, q, p6);
    UMAAL(A16, t, q, p7);
    UMAAL(A17, t, q, p8);
    UMAAL(A18, t, q, p9);
    UMAAL(A19, t, q, p10);
    UMAAL(A20, t, q, p11);
    UMAAL(A21, t, q, p12);
    UMAAL(A22, t, q, p13);
    UMAAL(A23, t, q, p14);
    UMAAL(A24, t, q, p15);
    UMAAL(A25, t2, 1, t);

    q = A10;
    t = 0;
    UMAAL(A14, t, q, p4 + 1);
    UMAAL(A15, t, q, p5);
    UMAAL(A16, t, q, p6);
    UMAAL(A17, t, q, p7);
    UMAAL(A18, t, q, p8);
    UMAAL(A19, t, q, p9);
    UMAAL(A20, t, q, p10);
    UMAAL(A21, t, q, p11);
    UMAAL(A22, t, q, p12);
    UMAAL(A23, t, q, p13);
    UMAAL(A24, t, q, p14);
    UMAAL(A25, t, q, p15);
    UMAAL(A26, t2, 1, t);

    q = A11;
    t = 0;
    UMAAL(A15, t, q, p4 + 1);
    UMAAL(A16, t, q, p5);
    UMAAL(A17, t, q, p6);
    UMAAL(A18, t, q, p7);
    UMAAL(A19, t, q, p8);
    UMAAL(A20, t, q, p9);
    UMAAL(A21, t, q, p10);
    UMAAL(A22, t, q, p11);
    UMAAL(A23, t, q, p12);
    UMAAL(A24, t, q, p13);
    UMAAL(A25, t, q, p14);
    UMAAL(A26, t, q, p15);
    UMAAL(A27, t2, 1, t);

    q = A12;
    t = 0;
    UMAAL(A16, t, q, p4 + 1);
    UMAAL(A17, t, q, p5);
    UMAAL(A18, t, q, p6);
    UMAAL(A19, t, q, p7);
    UMAAL(A20, t, q, p8);
    UMAAL(A21, t, q, p9);
    UMAAL(A22, t, q, p10);
    UMAAL(A23, t, q, p11);
    UMAAL(A24, t, q, p12);
    UMAAL(A25, t, q, p13);
    UMAAL(A26, t, q, p14);
    UMAAL(A27, t, q, p15);
    UMAAL(A28, t2, 1, t);

    q = A13;
    t = 0;
    UMAAL(A17, t, q, p4 + 1);
    UMAAL(A18, t, q, p5);
    UMAAL(A19, t, q, p6);
    UMAAL(A20, t, q, p7);
    UMAAL(A21, t, q, p8);
    UMAAL(A22, t, q, p9);
    UMAAL(A23, t, q, p10);
    UMAAL(A24, t, q, p11);
    UMAAL(A25, t, q, p12);
    UMAAL(A26, t, q, p13);
    UMAAL(A27, t, q, p14);
    UMAAL(A28, t, q, p15);
    UMAAL(A29, t2, 1, t);

    q = A14;
    t = 0;
    UMAAL(A18, t, q, p4 + 1);
    UMAAL(A19, t, q, p5);
    UMAAL(A20, t, q, p6);
    UMAAL(A21, t, q, p7);
    UMAAL(A22, t, q, p8);
    UMAAL(A23, t, q, p9);
    UMAAL(A24, t, q, p10);
    UMAAL(A25, t, q, p11);
    UMAAL(A26, t, q, p12);
    UMAAL(A27, t, q, p13);
    UMAAL(A28, t, q, p14);
    UMAAL(A29, t, q, p15);
    UMAAL(A30, t2, 1, t);

    q = A15;
    t = 0;
    UMAAL(A19, t, q, p4 + 1);
    UMAAL(A20, t, q, p5);
    UMAAL(A21, t, q, p6);
    UMAAL(A22, t, q, p7);
    UMAAL(A23, t, q, p8);
    UMAAL(A24, t, q, p9);
    UMAAL(A25, t, q, p10);
    UMAAL(A26, t, q, p11);
    UMAAL(A27, t, q, p12);
    UMAAL(A28, t, q, p13);
    UMAAL(A29, t, q, p14);
    UMAAL(A30, t, q, p15);
    UMAAL(A31, t2, 1, t);

    res[0] = A16;
    res[1] = A17;
    res[2] = A18;
    res[3] = A19;
    res[4] = A20;
    res[5] = A21;
    res[6] = A22;
    res[7] = A23;
    res[8] = A24;
    res[9] = A25;
    res[10] = A26;
    res[11] = A27;
    res[12] = A28;
    res[13] = A29;
    res[14] = A30;
    res[15] = A31;
}

static void modcpy_MFP318233_ct(const spint *a, spint *b) {
    b[0] = a[0];
    b[1] = a[1];
    b[2] = a[2];
    b[3] = a[3];
    b[4] = a[4];
    b[5] = a[5];
    b[6] = a[6];
    b[7] = a[7];
    b[8] = a[8];
    b[9] = a[9];
    b[10] = a[10];
    b[11] = a[11];
    b[12] = a[12];
    b[13] = a[13];
    b[14] = a[14];
    b[15] = a[15];
}

static void modnsqr_MFP318233_ct(spint *a, int n) {
    for (int i = 0; i < n; i++)
        modsqr_MFP318233_ct(a, a);
}

//Calculate progenitor
static void modpro_MFP318233_ct(const spint *w,spint *z) {
	spint x[16];
	spint t0[16];
	spint t1[16];
	spint t2[16];
	spint t3[16];
	spint t4[16];
	spint t5[16];
	spint t6[16];
	spint t7[16];
	spint t8[16];
	spint t9[16];
	spint t10[16];
	spint t11[16];
	spint t12[16];
	spint t13[16];
	spint t14[16];
	spint t15[16];
	spint t16[16];
	spint t17[16];
	spint t18[16];
	spint t19[16];
	spint t20[16];
	spint t21[16];
	spint t22[16];
	spint t23[16];
	spint t24[16];
	spint t25[16];
	spint t26[16];
	spint t27[16];
	spint t28[16];
	spint t29[16];
	spint t30[16];
	modcpy_MFP318233_ct(w,x);
	modsqr_MFP318233_ct(x,t16);
	modsqr_MFP318233_ct(t16,t19);
	modmul_MFP318233_ct(t16,t19,z);
	modmul_MFP318233_ct(t16,z,t6);
	modmul_MFP318233_ct(x,t6,t1);
	modmul_MFP318233_ct(x,t1,t20);
	modmul_MFP318233_ct(t16,t20,t8);
	modmul_MFP318233_ct(z,t1,t9);
	modmul_MFP318233_ct(z,t8,t13);
	modmul_MFP318233_ct(t16,t13,t0);
	modmul_MFP318233_ct(t1,t0,t17);
	modmul_MFP318233_ct(z,t17,t26);
	modmul_MFP318233_ct(t16,t26,t5);
	modmul_MFP318233_ct(t20,t5,t12);
	modmul_MFP318233_ct(t6,t12,t1);
	modmul_MFP318233_ct(t8,t1,t27);
	modmul_MFP318233_ct(t16,t27,t3);
	modmul_MFP318233_ct(t19,t3,t22);
	modmul_MFP318233_ct(t19,t22,t10);
	modmul_MFP318233_ct(t19,t10,t11);
	modmul_MFP318233_ct(t16,t11,t7);
	modmul_MFP318233_ct(z,t7,t15);
	modmul_MFP318233_ct(t20,t15,t4);
	modmul_MFP318233_ct(t16,t4,t29);
	modmul_MFP318233_ct(t6,t29,t2);
	modmul_MFP318233_ct(t16,t2,t14);
	modmul_MFP318233_ct(t13,t2,z);
	modmul_MFP318233_ct(t19,z,t18);
	modmul_MFP318233_ct(t13,t18,t21);
	modmul_MFP318233_ct(t19,t21,t19);
	modmul_MFP318233_ct(t16,t19,t24);
	modmul_MFP318233_ct(t13,t24,t25);
	modmul_MFP318233_ct(t16,t25,t21);
	modmul_MFP318233_ct(t13,t25,t13);
	modmul_MFP318233_ct(t8,t13,t8);
	modmul_MFP318233_ct(t20,t8,t28);
	modmul_MFP318233_ct(t20,t28,t20);
	modmul_MFP318233_ct(t6,t20,t23);
	modmul_MFP318233_ct(t16,t23,t6);
	modmul_MFP318233_ct(t16,t6,t16);
	modmul_MFP318233_ct(t0,t16,t0);
	modmul_MFP318233_ct(t27,t23,t30);
	modnsqr_MFP318233_ct(t30,7);
	modmul_MFP318233_ct(t29,t30,t29);
	modnsqr_MFP318233_ct(t29,11);
	modmul_MFP318233_ct(t28,t29,t28);
	modnsqr_MFP318233_ct(t28,10);
	modmul_MFP318233_ct(t27,t28,t27);
	modnsqr_MFP318233_ct(t27,9);
	modmul_MFP318233_ct(t24,t27,t27);
	modnsqr_MFP318233_ct(t27,7);
	modmul_MFP318233_ct(t4,t27,t27);
	modnsqr_MFP318233_ct(t27,7);
	modmul_MFP318233_ct(t26,t27,t26);
	modnsqr_MFP318233_ct(t26,13);
	modmul_MFP318233_ct(t25,t26,t25);
	modsqr_MFP318233_ct(t25,t25);
	modmul_MFP318233_ct(x,t25,t25);
	modnsqr_MFP318233_ct(t25,17);
	modmul_MFP318233_ct(t2,t25,t25);
	modnsqr_MFP318233_ct(t25,10);
	modmul_MFP318233_ct(t24,t25,t24);
	modnsqr_MFP318233_ct(t24,9);
	modmul_MFP318233_ct(t23,t24,t23);
	modnsqr_MFP318233_ct(t23,7);
	modmul_MFP318233_ct(t22,t23,t22);
	modnsqr_MFP318233_ct(t22,15);
	modmul_MFP318233_ct(t21,t22,t21);
	modnsqr_MFP318233_ct(t21,9);
	modmul_MFP318233_ct(t20,t21,t20);
	modnsqr_MFP318233_ct(t20,9);
	modmul_MFP318233_ct(t4,t20,t20);
	modnsqr_MFP318233_ct(t20,10);
	modmul_MFP318233_ct(t19,t20,t19);
	modnsqr_MFP318233_ct(t19,9);
	modmul_MFP318233_ct(t18,t19,t18);
	modnsqr_MFP318233_ct(t18,5);
	modmul_MFP318233_ct(t17,t18,t17);
	modnsqr_MFP318233_ct(t17,15);
	modmul_MFP318233_ct(t0,t17,t17);
	modnsqr_MFP318233_ct(t17,11);
	modmul_MFP318233_ct(t16,t17,t16);
	modnsqr_MFP318233_ct(t16,10);
	modmul_MFP318233_ct(t15,t16,t15);
	modnsqr_MFP318233_ct(t15,11);
	modmul_MFP318233_ct(t5,t15,t15);
	modnsqr_MFP318233_ct(t15,12);
	modmul_MFP318233_ct(t14,t15,t14);
	modnsqr_MFP318233_ct(t14,8);
	modmul_MFP318233_ct(t7,t14,t14);
	modnsqr_MFP318233_ct(t14,10);
	modmul_MFP318233_ct(t13,t14,t13);
	modnsqr_MFP318233_ct(t13,7);
	modmul_MFP318233_ct(t12,t13,t12);
	modnsqr_MFP318233_ct(t12,11);
	modmul_MFP318233_ct(t11,t12,t11);
	modnsqr_MFP318233_ct(t11,8);
	modmul_MFP318233_ct(t10,t11,t10);
	modnsqr_MFP318233_ct(t10,6);
	modmul_MFP318233_ct(t9,t10,t9);
	modnsqr_MFP318233_ct(t9,14);
	modmul_MFP318233_ct(t8,t9,t8);
	modnsqr_MFP318233_ct(t8,7);
	modmul_MFP318233_ct(t7,t8,t7);
	modnsqr_MFP318233_ct(t7,10);
	modmul_MFP318233_ct(t6,t7,t6);
	modnsqr_MFP318233_ct(t6,6);
	modmul_MFP318233_ct(t5,t6,t5);
	modnsqr_MFP318233_ct(t5,9);
	modmul_MFP318233_ct(t4,t5,t4);
	modnsqr_MFP318233_ct(t4,8);
	modmul_MFP318233_ct(t3,t4,t3);
	modnsqr_MFP318233_ct(t3,8);
	modmul_MFP318233_ct(t2,t3,t2);
	modnsqr_MFP318233_ct(t2,8);
	modmul_MFP318233_ct(t1,t2,t1);
	modnsqr_MFP318233_ct(t1,12);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t1);
	modnsqr_MFP318233_ct(t1,8);
	modmul_MFP318233_ct(t0,t1,t0);
	modnsqr_MFP318233_ct(t0,7);
	modmul_MFP318233_ct(z,t0,z);
}

//calculate inverse, provide progenitor h if available
static void modinv_MFP318233_ct(const spint *x,const spint *h,spint *z) {
	spint s[16];
	spint t[16];
	if (h==NULL) {
		modpro_MFP318233_ct(x,t);
	} else {
		modcpy_MFP318233_ct(h,t);
	}
	modcpy_MFP318233_ct(x,s);
	modnsqr_MFP318233_ct(t,2);
	modmul_MFP318233_ct(s,t,z);
}

static void nres_MFP318233_ct(const spint *a, spint *b) {
    uint32_t nresfactor[] = {3344211133, 1189406880, 1139318437, 3415817049, 790320584, 1415233695, 2896824044, 450599389, 1440266452, 2274970505, 798906312, 750971490, 3673227324, 662746610, 1803973816, 1479015};
    modmul_MFP318233_ct(a, nresfactor, b);
}

static void redc_MFP318233_ct(const spint *a, spint *b) {
    uint32_t rinv[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    modmul_MFP318233_ct(a, rinv, b);
    modfsb_MFP318233_ct(b);
}

static int modis1_MFP318233_ct(const spint *a) {
    spint c[16] = {0};
    spint d=0;
    redc_MFP318233_ct(a,c);
    c[0] = c[0] - 1;
    for(int i=0; i<16; i++){
        d|=c[i];
    }
    return 1 - ((d | -d) >> 31);
}
    
static int modis0_MFP318233_ct(const spint *a) {
    spint c[16] = {0};
    spint d=0;
    modcpy_MFP318233_ct(a, c);
    modfsb_MFP318233_ct(c);
    for(int i=0; i<16; i++){
        d|=c[i];
    }
    return 1 - ((d | -d) >> 31);
}
    
static void modzer_MFP318233_ct(spint *a) {
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    a[4] = 0;
    a[5] = 0;
    a[6] = 0;
    a[7] = 0;
    a[8] = 0;
    a[9] = 0;
    a[10] = 0;
    a[11] = 0;
    a[12] = 0;
    a[13] = 0;
    a[14] = 0;
    a[15] = 0;
}

static void modone_MFP318233_ct(spint *a) {
    a[0] = 1754;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    a[4] = 2542534656;
    a[5] = 2860173878;
    a[6] = 563458169;
    a[7] = 2873485733;
    a[8] = 2628256070;
    a[9] = 559047012;
    a[10] = 2089255000;
    a[11] = 938839211;
    a[12] = 500261273;
    a[13] = 862455969;
    a[14] = 1438167460;
    a[15] = 1724897;
}

static void modint_MFP318233_ct(int x, spint *a) {
    a[0] = (spint) x;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    a[4] = 0;
    a[5] = 0;
    a[6] = 0;
    a[7] = 0;
    a[8] = 0;
    a[9] = 0;
    a[10] = 0;
    a[11] = 0;
    a[12] = 0;
    a[13] = 0;
    a[14] = 0;
    a[15] = 0;
    nres_MFP318233_ct(a,a);
}

//Test for quadratic residue, provide progenitor h if available, NULL if not
static int modqr_MFP318233_ct(const spint *h,const spint *x) {
	spint r[16];
	if (h==NULL) {
		modpro_MFP318233_ct(x,r);
		modsqr_MFP318233_ct(r,r);
	} else {
		modsqr_MFP318233_ct(h,r);
	}
	modmul_MFP318233_ct(r,x,r);
	return modis1_MFP318233_ct(r);
}

static void modcmv_MFP318233_ct(int d, const spint *a, spint *b) {
    uint32_t t;
    d = -d;
    for (int i = 0; i < 16; i++) {
        t = d & (a[i] ^ b[i]);
        b[i] ^= t;
    }
}

static void modcsw_MFP318233_ct(int d, spint *a, spint *b) {
    uint32_t t;
    d = -d;
    for (int i = 0; i < 16; i++) {
        t = d & (a[i] ^ b[i]);
        a[i] ^= t;
        b[i] ^= t;
    }
}

//Modular square root, provide progenitor h if available, NULL if not
static void modsqrt_MFP318233_ct(const spint *x, const spint *h, spint *r) {
	spint s[16];
	spint y[16];
	if (h==NULL) {
		modpro_MFP318233_ct(x,y);
	} else {
		modcpy_MFP318233_ct(h,y);
	}
	modmul_MFP318233_ct(y,x,s);
	modcpy_MFP318233_ct(s,r);
}

static void modshl_MFP318233_ct(int n, spint *a) {
    a[15] = (a[15] << n) + (a[14] >> (32-n));
    a[14] = (a[14] << n) + (a[13] >> (32-n));
    a[13] = (a[13] << n) + (a[12] >> (32-n));
    a[12] = (a[12] << n) + (a[11] >> (32-n));
    a[11] = (a[11] << n) + (a[10] >> (32-n));
    a[10] = (a[10] << n) + (a[9] >> (32-n));
    a[9] = (a[9] << n) + (a[8] >> (32-n));
    a[8] = (a[8] << n) + (a[7] >> (32-n));
    a[7] = (a[7] << n) + (a[6] >> (32-n));
    a[6] = (a[6] << n) + (a[5] >> (32-n));
    a[5] = (a[5] << n) + (a[4] >> (32-n));
    a[4] = (a[4] << n) + (a[3] >> (32-n));
    a[3] = (a[3] << n) + (a[2] >> (32-n));
    a[2] = (a[2] << n) + (a[1] >> (32-n));
    a[1] = (a[1] << n) + (a[0] >> (32-n));
    a[0] = (a[0] << n);
}

// Only works for n <= 31
static uint32_t modshr_MFP318233_ct(int n, spint *a) {
    uint32_t r = a[0] & ((1 << n) - 1);
    a[0] = (a[0] >> n) | (a[0 + 1] << (32u - n));
    a[1] = (a[1] >> n) | (a[1 + 1] << (32u - n));
    a[2] = (a[2] >> n) | (a[2 + 1] << (32u - n));
    a[3] = (a[3] >> n) | (a[3 + 1] << (32u - n));
    a[4] = (a[4] >> n) | (a[4 + 1] << (32u - n));
    a[5] = (a[5] >> n) | (a[5 + 1] << (32u - n));
    a[6] = (a[6] >> n) | (a[6 + 1] << (32u - n));
    a[7] = (a[7] >> n) | (a[7 + 1] << (32u - n));
    a[8] = (a[8] >> n) | (a[8 + 1] << (32u - n));
    a[9] = (a[9] >> n) | (a[9 + 1] << (32u - n));
    a[10] = (a[10] >> n) | (a[10 + 1] << (32u - n));
    a[11] = (a[11] >> n) | (a[11 + 1] << (32u - n));
    a[12] = (a[12] >> n) | (a[12 + 1] << (32u - n));
    a[13] = (a[13] >> n) | (a[13 + 1] << (32u - n));
    a[14] = (a[14] >> n) | (a[14 + 1] << (32u - n));
    a[15] = a[15] >> n;
    return r;
}

// Code for export, not exponentiation
static void modexp_MFP318233_ct(const spint *a, char *b) {
    uint32_t c[16];
    redc_MFP318233_ct(a, c);
    for (int i = 63; i>= 0; i--) {
        b[i] = c[0] & (uint32_t)0xff;
        (void)modshr_MFP318233_ct(8, c);
    }
}

static void modimp_MFP318233_ct(const char *b, spint *a) {
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    a[4] = 0;
    a[5] = 0;
    a[6] = 0;
    a[7] = 0;
    a[8] = 0;
    a[9] = 0;
    a[10] = 0;
    a[11] = 0;
    a[12] = 0;
    a[13] = 0;
    a[14] = 0;
    a[15] = 0;
    for (int i=0; i<64; i++){
        modshl_MFP318233_ct(8,a);
        a[0] += (spint)(unsigned char)b[i];
    }
    nres_MFP318233_ct(a,a);
}

#define Nlimbs_MFP318233_ct 16
#define Nbits_MFP318233_ct 502
#define Nbytes_MFP318233_ct 64

/* API functions calling generated code */

const digit_t p[NWORDS_FIELD] =  { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xA6E1FFFF, 0x994C68AD, 0x781974CE, 0xFAF0A29A, 0x4A0DEA65, 0xFE3AC590, 0x26507D01, 0x02BDBE63, 0x6936E792, 0x8C15B003, 0xA8869BC6, 0x00255946 };
const digit_t pre[NWORDS_FIELD] = { 0x9EE85D1D, 0x640F3267, 0x77C81AF6, 0xFE228DB5, 0x1CF87CD4, 0x7CED1891, 0x5D69292E, 0xD6C85A4E, 0x14D3B0BF, 0xEE632C35, 0x719A2FAD, 0xA3B39266, 0xE267D0B9, 0x01E72F81, 0x65541D1A, 0x3EADA };

bool fp_is_equal(const digit_t* a, const digit_t* b) {
        spint c[16], d[16];
        int neq = 0;
        redc_MFP318233_ct(a, c);
        redc_MFP318233_ct(b, d);
        for (int i = 0; i < 16; i++) {
            neq |= c[i] ^ d[i];
        }
        neq = (neq | -neq) >> 31;
        return 1 - (neq & 1);
    }

bool fp_is_zero(const digit_t* a) {
    return (bool) modis0_MFP318233_ct(a);
}

void fp_copy(digit_t* out, const digit_t* a) {
    modcpy_MFP318233_ct(a, out);
}

void fp_add(digit_t* out, const digit_t* a, const digit_t* b) {
    modadd_MFP318233_ct(a, b, out);
}

void fp_sub(digit_t* out, const digit_t* a, const digit_t* b) {
    modsub_MFP318233_ct(a, b, out);
}

void fp_neg(digit_t* out, const digit_t* a) {
    spint zero[NWORDS_FIELD];
    modzer_MFP318233_ct(zero);
    modsub_MFP318233_ct(zero, a, out);
}

void fp_sqr(digit_t* out, const digit_t* a) {
    modsqr_MFP318233_ct(a, out);
}

void fp_mul(digit_t* out, const digit_t* a, const digit_t* b) {
    modmul_MFP318233_ct(a, b, out);
}

void fp_sqrt(digit_t* a) {
    modsqrt_MFP318233_ct(a, NULL, a);
}

void fp_exp3div4(digit_t* out, const digit_t* a) {
    modpro_MFP318233_ct(a, out);
}

void fp_tomont(digit_t* out, const digit_t* a) {
    nres_MFP318233_ct(a, out);
}

void fp_frommont(digit_t* out, const digit_t* a) {
    redc_MFP318233_ct(a, out);
}

void fp_mont_setone(digit_t* out) {
    modone_MFP318233_ct(out);
}

void fp_to_digit_array(digit_t* out, const digit_t* a) {
    digit_t x[NWORDS_FIELD];
    modcpy_MFP318233_ct(a, x);
    for (int i = 0; i < NWORDS_ORDER; i++) {
        out[i] = 0;
    }
    for (int i = 0; i < 63; i++) {
        ((char *) out)[i] = x[0] & 0xff;
        modshr_MFP318233_ct(8, x);
    }
}

void fp_from_digit_array(digit_t* out, const digit_t* a) {
    for (int i = 0; i < NWORDS_FIELD; i++) {
        out[i] = 0;
    }
    for (int i = 63 - 1; i >= 0; i--) {
        modshl_MFP318233_ct(8, out);
        out[0] += (digit_t)((unsigned char *) a)[i];
    }
}

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
