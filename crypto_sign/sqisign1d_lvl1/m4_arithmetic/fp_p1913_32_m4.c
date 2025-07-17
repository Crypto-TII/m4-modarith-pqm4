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

static void modfsb_MFP1973_ct(spint *a) {
    const spint p0 = 4294967295;
    const spint p1 = 4294967295;
    const spint p2 = 894519295;
    const spint p3 = 623681097;
    const spint p4 = 2269148215;
    const spint p5 = 866560101;
    const spint p6 = 1804982668;
    const spint p7 = 887266856;
    uint32_t t0, t1;
    uint32_t tmpvec[8];
    asm volatile(
        "ldrd %[t0], %[t1], [%[a]]\n"
        "subs %[t0], %[t0], %[p0]\n"
        "sbcs %[t1], %[t1], %[p1]\n"
        "strd %[t0], %[t1], [%[tmpvec]]\n"
        "ldrd %[t0], %[t1], [%[a], #8]\n"
        "sbcs %[t0], %[t0], %[p2]\n"
        "sbcs %[t1], %[t1], %[p3]\n"
        "strd %[t0], %[t1], [%[tmpvec], #8]\n"
        "ldrd %[t0], %[t1], [%[a], #16]\n"
        "sbcs %[t0], %[t0], %[p4]\n"
        "sbcs %[t1], %[t1], %[p5]\n"
        "strd %[t0], %[t1], [%[tmpvec], #16]\n"
        "ldrd %[t0], %[t1], [%[a], #24]\n"
        "sbcs %[t0], %[t0], %[p6]\n"
        "sbcs %[t1], %[t1], %[p7]\n"
        "strd %[t0], %[t1], [%[tmpvec], #24]\n"
        "itttt cs\n"
        "ldrdcs %[t0], %[t1], [%[tmpvec], #0]\n"
        "strdcs %[t0], %[t1], [%[a], #0]\n"
        "ldrdcs %[t0], %[t1], [%[tmpvec], #8]\n"
        "strdcs %[t0], %[t1], [%[a], #8]\n"
        "itttt cs\n"
        "ldrdcs %[t0], %[t1], [%[tmpvec], #16]\n"
        "strdcs %[t0], %[t1], [%[a], #16]\n"
        "ldrdcs %[t0], %[t1], [%[tmpvec], #24]\n"
        "strdcs %[t0], %[t1], [%[a], #24]\n"
        : [t0] "=&r" (t0), [t1] "=&r" (t1)
        : [a] "r" (a), [p0] "r" (p0), [p1] "r" (p1), [p2] "r" (p2), [p3] "r" (p3), [p4] "r" (p4), [p5] "r" (p5), [p6] "r" (p6), [p7] "r" (p7), [tmpvec] "r" (tmpvec)
        : "cc", "memory");
}

static void modadd_MFP1973_ct(const spint *a, const spint *b, spint *res) {
    spint a0 = a[0], a1 = a[1], a2 = a[2], a3 = a[3], a4 = a[4], a5 = a[5], a6 = a[6], a7 = a[7];
    spint b0 = b[0], b1 = b[1], b2 = b[2], b3 = b[3], b4 = b[4], b5 = b[5], b6 = b[6], b7 = b[7];
    spint of = 0;
    spint nof[8];
    spint t = 0;
    const spint p0 = 4294967294;
    const spint p1 = 4294967295;
    const spint p2 = 1789038591;
    const spint p3 = 1247362194;
    const spint p4 = 243329134;
    const spint p5 = 1733120203;
    const spint p6 = 3609965336;
    const spint p7 = 1774533712;
    UMAAL(a0, t, 1, b0);
    UMAAL(a1, t, 1, b1);
    UMAAL(a2, t, 1, b2);
    UMAAL(a3, t, 1, b3);
    UMAAL(a4, t, 1, b4);
    UMAAL(a5, t, 1, b5);
    UMAAL(a6, t, 1, b6);
    UMAAL(a7, t, 1, b7);
    of = t;
    res[0] = a0;
    res[1] = a1;
    res[2] = a2;
    res[3] = a3;
    res[4] = a4;
    res[5] = a5;
    res[6] = a6;
    res[7] = a7;
    spint sub = (of | (res[7] > p7));
    nof[0] = (res[0] - p0 > res[0]) & sub;
    nof[1] = (res[1] - p1 - nof[0] >= res[1]) & sub;
    nof[2] = (res[2] - p2 - nof[1] >= res[2]) & sub;
    nof[3] = (res[3] - p3 - nof[2] >= res[3]) & sub;
    nof[4] = (res[4] - p4 - nof[3] >= res[4]) & sub;
    nof[5] = (res[5] - p5 - nof[4] >= res[5]) & sub;
    nof[6] = (res[6] - p6 - nof[5] >= res[6]) & sub;
    nof[7] = (res[7] - p7 - nof[6] >= res[7]) & sub;
    res[0] = res[0] - p0*sub;
    res[1] = res[1] - p1*sub - nof[0];
    res[2] = res[2] - p2*sub - nof[1];
    res[3] = res[3] - p3*sub - nof[2];
    res[4] = res[4] - p4*sub - nof[3];
    res[5] = res[5] - p5*sub - nof[4];
    res[6] = res[6] - p6*sub - nof[5];
    res[7] = res[7] - p7*sub - nof[6];
}

static void modsub_MFP1973_ct(const spint *a, const spint *b, spint *res) {
    spint a0 = a[0], a1 = a[1], a2 = a[2], a3 = a[3], a4 = a[4], a5 = a[5], a6 = a[6], a7 = a[7];
    spint t, t1, t2;
    spint t3, t4;
    const spint p0 = 4294967294;
    const spint p1 = 4294967295;
    const spint p2 = 1789038591;
    const spint p3 = 1247362194;
    const spint p4 = 243329134;
    const spint p5 = 1733120203;
    const spint p6 = 3609965336;
    const spint p7 = 1774533712;
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
        "sbcs %[t], %[t], %[t]\n"
        : [t] "=&r" (t), [t1] "=&r" (t1), [t2] "=&r" (t2), [t3] "=&r" (t3), [t4] "=&r" (t4)
        : [b] "r" (b), [a] "r" (a), [res] "r" (res)
        : "cc", "memory");
    t = -t | (res[7] > p7);
    t2 = 0;
    a0 = res[0];
    a1 = res[1];
    a2 = res[2];
    a3 = res[3];
    a4 = res[4];
    a5 = res[5];
    a6 = res[6];
    a7 = res[7];
    UMAAL(a0, t2, t, p0);
    UMAAL(a1, t2, t, p1);
    UMAAL(a2, t2, t, p2);
    UMAAL(a3, t2, t, p3);
    UMAAL(a4, t2, t, p4);
    UMAAL(a5, t2, t, p5);
    UMAAL(a6, t2, t, p6);
    UMAAL(a7, t2, t, p7);
    res[0] = a0;
    res[1] = a1;
    res[2] = a2;
    res[3] = a3;
    res[4] = a4;
    res[5] = a5;
    res[6] = a6;
    res[7] = a7;
}

static void modneg_MFP1973_ct(const spint *a, spint *r) {
    spint zero[8] = {0};
    modsub_MFP1973_ct(zero,a,r);
}
    
static void mul_MFP1973_ct(const spint *p, const spint *q, spint *r) {
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
        "str.w  %[t2], [%[r],#28] \n"
        "str.w  %[t3], [%[r],#32] \n"
        "str.w  %[t4], [%[r],#36] \n"
        "str.w  %[t0], [%[r],#40] \n"
        "str.w  %[t1], [%[r],#44] \n"
        : [t2] "=&r"(t2), [t3] "=&r"(t3), [t4] "=&r"(t4), [t0] "=&r"(t0), [t1] "=&r"(t1), [a] "=&r"(a)
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
        "ldr.w   %[a], [%[p],#20] \n"
        "str.w  %[t4], [%[r],#32] \n"
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
        "ldr.w   %[a], [%[p],#24] \n"
        "str.w  %[t0], [%[r],#36] \n"
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
        "ldr.w   %[a], [%[p],#28] \n"
        "str.w  %[t1], [%[r],#40] \n"
        "mov.w  %[t1], #0 \n"
#ifdef __clang__
        "adcs   %[t0], %[t0], #0\n"
#else
        "adcs.w %[t0], %[t0], #0\n"
#endif
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
#ifdef __clang__
        "adcs   %[t1], %[t1], #0 \n"
#else
        "adcs.w %[t1], %[t1], #0 \n"
#endif
        "str.w  %[t2], [%[r],#44] \n"
        "str.w  %[t3], [%[r],#48] \n"
        "str.w  %[t4], [%[r],#52] \n"
        "str.w  %[t0], [%[r],#56] \n"
        "str.w  %[t1], [%[r],#60] \n"
        : [t0] "+&r"(t0), [t1] "+&r"(t1), [t2] "+&r"(t2), [t3] "+&r"(t3), [t4] "+&r"(t4), [a] "=&r"(a)
        : [b0] "r"(b0), [b1] "r"(b1), [b2] "r"(b2), [b3] "r"(b3), [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
}

static void sqr_MFP1973_ct(const spint *p, spint *r) {
    register spint a0, a1, a2, a3, a4, a5, a6, a7;
    register spint t0, t1, t2, t3;

    asm volatile(
        "ldr   %[a0], [%[p]]     \n"
        "ldr   %[a1], [%[p],#4]  \n"
        "ldr   %[a2], [%[p],#8]  \n"
        "ldr   %[a3], [%[p],#12] \n"
        "ldr   %[a4], [%[p],#16] \n"
        "ldr   %[a5], [%[p],#20] \n"
        "ldr   %[a6], [%[p],#24] \n"
        "ldr   %[a7], [%[p],#28] \n"

        "umull %[t0], %[t1], %[a1], %[a0]\n"
        "str   %[t0], [%[r],#4]\n"
        "mov   %[t0], #0\n"

        "umaal %[t0], %[t1], %[a2], %[a0]\n"
        "str   %[t0], [%[r],#8]\n"

        "umull %[t0], %[t2], %[a2], %[a1]\n"
        "umaal %[t0], %[t1], %[a3], %[a0]\n"
        "str   %[t0], [%[r],#12]\n"
        "mov   %[t0], #0\n"

        "umaal %[t0], %[t2], %[a3], %[a1]\n"
        "umaal %[t0], %[t1], %[a4], %[a0]\n"
        "str   %[t0], [%[r],#16]\n"

        "umull %[t0], %[t3], %[a5], %[a0]\n"
        "umaal %[t0], %[t2], %[a4], %[a1]\n"
        "umaal %[t0], %[t1], %[a3], %[a2]\n"
        "str   %[t0], [%[r],#20]\n"
        "mov   %[t0], #0\n"

        "umaal %[t0], %[t3], %[a6], %[a0]\n"
        "umaal %[t0], %[t2], %[a5], %[a1]\n"
        "umaal %[t0], %[t1], %[a4], %[a2]\n"
        "str   %[t0], [%[r],#24]\n"

        "umull %[t0], %[a0], %[a7], %[a0]\n"
        "umaal %[t0], %[t3], %[a4], %[a3]\n"
        "umaal %[t0], %[t2], %[a6], %[a1]\n"
        "umaal %[t0], %[t1], %[a5], %[a2]\n"
        "str   %[t0], [%[r],#28]\n"
        "mov   %[t0], #0\n"

        "umaal %[a0], %[t3], %[a7], %[a1]\n"
        "umaal %[a0], %[t2], %[a6], %[a2]\n"
        "umaal %[a0], %[t1], %[a5], %[a3]\n"
        "str   %[a0], [%[r],#32]\n"

        "umaal %[t0], %[t3], %[a7], %[a2]\n"
        "umaal %[t0], %[t2], %[a6], %[a3]\n"
        "umaal %[t0], %[t1], %[a5], %[a4]\n"
        "str   %[t0], [%[r],#36]\n"
        "mov   %[t0], #0\n"

        "umaal %[t1], %[t3], %[a7], %[a3]\n"
        "umaal %[t1], %[t2], %[a6], %[a4]\n"
        "str   %[t1], [%[r],#40]\n"

        "umaal %[t0], %[t3], %[a7], %[a4]\n"
        "umaal %[t0], %[t2], %[a6], %[a5]\n"
        "str   %[t0], [%[r],#44]\n"
        "mov   %[t0], #0\n"

        "umaal %[t2], %[t3], %[a7], %[a5]\n"
        "str   %[t2], [%[r],#48]\n"

        "umaal %[t0], %[t3], %[a7], %[a6]\n"
        "str   %[t0], [%[r],#52]\n"
        "str   %[t3], [%[r],#56]\n"
        : [t0] "=&r"(t0), [t1] "=&r"(t1), [t2] "=&r"(t2), [t3] "=&r"(t3), [a0] "=&r"(a0), [a1] "=&r"(a1),
          [a2] "=&r"(a2), [a3] "=&r"(a3), [a4] "=&r"(a4), [a5] "=&r"(a5), [a6] "=&r"(a6), [a7] "=&r"(a7)
        : [p] "r"(p), [r] "r"(r)
        : "cc", "memory");

    // lower part ai x ai + doubling

    asm volatile(
        "ldr    %[a0], [%[p]] \n"
        "ldr    %[a1], [%[p],#4] \n"
        "ldr    %[a2], [%[p],#8] \n"
        "ldr    %[a3], [%[p],#12] \n"

        "ldr    %[t0], [%[r],#4] \n"
        "ldr    %[t1], [%[r],#8] \n"
        "ldr    %[t2], [%[r],#12] \n"
        "ldr    %[t3], [%[r],#16] \n"

        "umull %[a0], %[a4], %[a0], %[a0]\n"
        "str   %[a0], [%[r]] \n"
        "mov   %[a0], #2 \n"
        "mov   %[a5], #0 \n"

        "umaal %[a5], %[a4], %[a0], %[t0]\n"
        "str   %[a5], [%[r],#4] \n"

        "umull %[a1], %[t0], %[a1], %[a1]\n"
        "umaal %[a1], %[a4], %[a0], %[t1]\n"
        "str   %[a1], [%[r],#8] \n"

        "umaal %[t0], %[a4], %[a0], %[t2]\n"
        "str   %[t0], [%[r],#12] \n"
        "mov   %[t0], #0 \n"

        "umull %[a1], %[a2], %[a2], %[a2]\n"
        "umaal %[a1], %[a4], %[a0], %[t3]\n"
        "str   %[a1], [%[r],#16] \n"

        "ldr    %[t0], [%[r],#20] \n"
        "ldr    %[t1], [%[r],#24] \n"
        "ldr    %[t2], [%[r],#28] \n"
        "ldr    %[t3], [%[r],#32] \n"

        "umaal %[a2], %[a4], %[a0], %[t0]\n"
        "str   %[a2], [%[r],#20] \n"

        "umull %[t0], %[a3], %[a3], %[a3]\n"
        "umaal %[a4], %[t0], %[a0], %[t1]\n"
        "str   %[a4], [%[r],#24] \n"

        "umaal %[t0], %[a3], %[a0], %[t2]\n"
        "str   %[t0], [%[r],#28] \n"

        "ldr    %[a4], [%[p],#16] \n"
        "ldr    %[a5], [%[p],#20] \n"
        "ldr    %[a6], [%[p],#24] \n"
        "ldr    %[a7], [%[p],#28] \n"

        "umull %[a1], %[a2], %[a4], %[a4]\n"
        "umaal %[a1], %[a3], %[a0], %[t3]\n"
        "str   %[a1], [%[r],#32] \n"

        "ldr    %[t0], [%[r],#36] \n"
        "ldr    %[t1], [%[r],#40] \n"
        "ldr    %[t2], [%[r],#44] \n"
        "ldr    %[t3], [%[r],#48] \n"

        "umaal %[a2], %[a3], %[a0], %[t0]\n"
        "str   %[a2], [%[r],#36] \n"

        "umull %[t0], %[a2], %[a5], %[a5]\n"
        "umaal %[t0], %[a3], %[a0], %[t1]\n"
        "str   %[t0], [%[r],#40] \n"

        "ldr   %[t0], [%[r],#52] \n"
        "ldr   %[t1], [%[r],#56] \n"

        "umaal %[a2], %[a3], %[a0], %[t2]\n"
        "str   %[a2], [%[r],#44] \n"

        "umull %[a2], %[a4], %[a6], %[a6]\n"
        "umaal %[a2], %[a3], %[a0], %[t3]\n"
        "str   %[a2], [%[r],#48] \n"

        "umaal %[a3], %[a4], %[a0], %[t0]\n"
        "str   %[a3], [%[r],#52] \n"

        "umull %[a2], %[a5], %[a7], %[a7]\n"
        "umaal %[a2], %[a4], %[a0], %[t1]\n"
        "str   %[a2], [%[r],#56] \n"
        "add   %[a4], %[a5]\n"
        "str   %[a4], [%[r],#60]\n"

        : [t0] "=&r"(t0), [t1] "=&r"(t1), [t2] "=&r"(t2), [t3] "=&r"(t3), [a0] "=&r"(a0), [a1] "=&r"(a1),
          [a2] "=&r"(a2), [a3] "=&r"(a3), [a4] "=&r"(a4), [a5] "=&r"(a5), [a6] "=&r"(a6), [a7] "=&r"(a7)
        : [p] "r"(p), [r] "r"(r)
        : "cc", "memory");

}

static void mli_MFP1973_ct(const spint *p, const spint q, spint *r) {
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
    r[8] = t8;
}

static void modmul_MFP1973_ct(const spint *a, const spint *b, spint *res) {
    spint A[16];
    spint A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15;
    const spint p2 = 894519295;
    const spint p3 = 623681097;
    const spint p4 = 2269148215;
    const spint p5 = 866560101;
    const spint p6 = 1804982668;
    const spint p7 = 887266856;
    spint q, t, t2 = 0;

    mul_MFP1973_ct(a, b, A);

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

    q = A0;
    t = 0;
    UMAAL(A2, t, q, p2 + 1);
    UMAAL(A3, t, q, p3);
    UMAAL(A4, t, q, p4);
    UMAAL(A5, t, q, p5);
    UMAAL(A6, t, q, p6);
    UMAAL(A7, t, q, p7);
    UMAAL(A8, t2, 1, t);

    q = A1;
    t = 0;
    UMAAL(A3, t, q, p2 + 1);
    UMAAL(A4, t, q, p3);
    UMAAL(A5, t, q, p4);
    UMAAL(A6, t, q, p5);
    UMAAL(A7, t, q, p6);
    UMAAL(A8, t, q, p7);
    UMAAL(A9, t2, 1, t);

    q = A2;
    t = 0;
    UMAAL(A4, t, q, p2 + 1);
    UMAAL(A5, t, q, p3);
    UMAAL(A6, t, q, p4);
    UMAAL(A7, t, q, p5);
    UMAAL(A8, t, q, p6);
    UMAAL(A9, t, q, p7);
    UMAAL(A10, t2, 1, t);

    q = A3;
    t = 0;
    UMAAL(A5, t, q, p2 + 1);
    UMAAL(A6, t, q, p3);
    UMAAL(A7, t, q, p4);
    UMAAL(A8, t, q, p5);
    UMAAL(A9, t, q, p6);
    UMAAL(A10, t, q, p7);
    UMAAL(A11, t2, 1, t);

    q = A4;
    t = 0;
    UMAAL(A6, t, q, p2 + 1);
    UMAAL(A7, t, q, p3);
    UMAAL(A8, t, q, p4);
    UMAAL(A9, t, q, p5);
    UMAAL(A10, t, q, p6);
    UMAAL(A11, t, q, p7);
    UMAAL(A12, t2, 1, t);

    q = A5;
    t = 0;
    UMAAL(A7, t, q, p2 + 1);
    UMAAL(A8, t, q, p3);
    UMAAL(A9, t, q, p4);
    UMAAL(A10, t, q, p5);
    UMAAL(A11, t, q, p6);
    UMAAL(A12, t, q, p7);
    UMAAL(A13, t2, 1, t);

    q = A6;
    t = 0;
    UMAAL(A8, t, q, p2 + 1);
    UMAAL(A9, t, q, p3);
    UMAAL(A10, t, q, p4);
    UMAAL(A11, t, q, p5);
    UMAAL(A12, t, q, p6);
    UMAAL(A13, t, q, p7);
    UMAAL(A14, t2, 1, t);

    q = A7;
    t = 0;
    UMAAL(A9, t, q, p2 + 1);
    UMAAL(A10, t, q, p3);
    UMAAL(A11, t, q, p4);
    UMAAL(A12, t, q, p5);
    UMAAL(A13, t, q, p6);
    UMAAL(A14, t, q, p7);
    UMAAL(A15, t2, 1, t);

    res[0] = A8;
    res[1] = A9;
    res[2] = A10;
    res[3] = A11;
    res[4] = A12;
    res[5] = A13;
    res[6] = A14;
    res[7] = A15;
}

static void modsqr_MFP1973_ct(const spint *a, spint *res) {
    spint A[16];
    spint A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15;
    const spint p2 = 894519295;
    const spint p3 = 623681097;
    const spint p4 = 2269148215;
    const spint p5 = 866560101;
    const spint p6 = 1804982668;
    const spint p7 = 887266856;
    spint q, t, t2 = 0;

    sqr_MFP1973_ct(a, A);

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

    q = A0;
    t = 0;
    UMAAL(A2, t, q, p2 + 1);
    UMAAL(A3, t, q, p3);
    UMAAL(A4, t, q, p4);
    UMAAL(A5, t, q, p5);
    UMAAL(A6, t, q, p6);
    UMAAL(A7, t, q, p7);
    UMAAL(A8, t2, 1, t);

    q = A1;
    t = 0;
    UMAAL(A3, t, q, p2 + 1);
    UMAAL(A4, t, q, p3);
    UMAAL(A5, t, q, p4);
    UMAAL(A6, t, q, p5);
    UMAAL(A7, t, q, p6);
    UMAAL(A8, t, q, p7);
    UMAAL(A9, t2, 1, t);

    q = A2;
    t = 0;
    UMAAL(A4, t, q, p2 + 1);
    UMAAL(A5, t, q, p3);
    UMAAL(A6, t, q, p4);
    UMAAL(A7, t, q, p5);
    UMAAL(A8, t, q, p6);
    UMAAL(A9, t, q, p7);
    UMAAL(A10, t2, 1, t);

    q = A3;
    t = 0;
    UMAAL(A5, t, q, p2 + 1);
    UMAAL(A6, t, q, p3);
    UMAAL(A7, t, q, p4);
    UMAAL(A8, t, q, p5);
    UMAAL(A9, t, q, p6);
    UMAAL(A10, t, q, p7);
    UMAAL(A11, t2, 1, t);

    q = A4;
    t = 0;
    UMAAL(A6, t, q, p2 + 1);
    UMAAL(A7, t, q, p3);
    UMAAL(A8, t, q, p4);
    UMAAL(A9, t, q, p5);
    UMAAL(A10, t, q, p6);
    UMAAL(A11, t, q, p7);
    UMAAL(A12, t2, 1, t);

    q = A5;
    t = 0;
    UMAAL(A7, t, q, p2 + 1);
    UMAAL(A8, t, q, p3);
    UMAAL(A9, t, q, p4);
    UMAAL(A10, t, q, p5);
    UMAAL(A11, t, q, p6);
    UMAAL(A12, t, q, p7);
    UMAAL(A13, t2, 1, t);

    q = A6;
    t = 0;
    UMAAL(A8, t, q, p2 + 1);
    UMAAL(A9, t, q, p3);
    UMAAL(A10, t, q, p4);
    UMAAL(A11, t, q, p5);
    UMAAL(A12, t, q, p6);
    UMAAL(A13, t, q, p7);
    UMAAL(A14, t2, 1, t);

    q = A7;
    t = 0;
    UMAAL(A9, t, q, p2 + 1);
    UMAAL(A10, t, q, p3);
    UMAAL(A11, t, q, p4);
    UMAAL(A12, t, q, p5);
    UMAAL(A13, t, q, p6);
    UMAAL(A14, t, q, p7);
    UMAAL(A15, t2, 1, t);

    res[0] = A8;
    res[1] = A9;
    res[2] = A10;
    res[3] = A11;
    res[4] = A12;
    res[5] = A13;
    res[6] = A14;
    res[7] = A15;
}

static void modmli_MFP1973_ct(const spint *a, const spint b, spint *res) {
    spint A[16];
    spint A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15;
    const spint p2 = 894519295;
    const spint p3 = 623681097;
    const spint p4 = 2269148215;
    const spint p5 = 866560101;
    const spint p6 = 1804982668;
    const spint p7 = 887266856;
    spint q, t, t2 = 0;

    mli_MFP1973_ct(a, b, A);

    A0 = A[0];
    A1 = A[1];
    A2 = A[2];
    A3 = A[3];
    A4 = A[4];
    A5 = A[5];
    A6 = A[6];
    A7 = A[7];
    A8 = A[8];
    A9 = 0;
    A10 = 0;
    A11 = 0;
    A12 = 0;
    A13 = 0;
    A14 = 0;
    A15 = 0;

    q = A0;
    t = 0;
    UMAAL(A2, t, q, p2 + 1);
    UMAAL(A3, t, q, p3);
    UMAAL(A4, t, q, p4);
    UMAAL(A5, t, q, p5);
    UMAAL(A6, t, q, p6);
    UMAAL(A7, t, q, p7);
    UMAAL(A8, t2, 1, t);

    q = A1;
    t = 0;
    UMAAL(A3, t, q, p2 + 1);
    UMAAL(A4, t, q, p3);
    UMAAL(A5, t, q, p4);
    UMAAL(A6, t, q, p5);
    UMAAL(A7, t, q, p6);
    UMAAL(A8, t, q, p7);
    UMAAL(A9, t2, 1, t);

    q = A2;
    t = 0;
    UMAAL(A4, t, q, p2 + 1);
    UMAAL(A5, t, q, p3);
    UMAAL(A6, t, q, p4);
    UMAAL(A7, t, q, p5);
    UMAAL(A8, t, q, p6);
    UMAAL(A9, t, q, p7);
    UMAAL(A10, t2, 1, t);

    q = A3;
    t = 0;
    UMAAL(A5, t, q, p2 + 1);
    UMAAL(A6, t, q, p3);
    UMAAL(A7, t, q, p4);
    UMAAL(A8, t, q, p5);
    UMAAL(A9, t, q, p6);
    UMAAL(A10, t, q, p7);
    UMAAL(A11, t2, 1, t);

    q = A4;
    t = 0;
    UMAAL(A6, t, q, p2 + 1);
    UMAAL(A7, t, q, p3);
    UMAAL(A8, t, q, p4);
    UMAAL(A9, t, q, p5);
    UMAAL(A10, t, q, p6);
    UMAAL(A11, t, q, p7);
    UMAAL(A12, t2, 1, t);

    q = A5;
    t = 0;
    UMAAL(A7, t, q, p2 + 1);
    UMAAL(A8, t, q, p3);
    UMAAL(A9, t, q, p4);
    UMAAL(A10, t, q, p5);
    UMAAL(A11, t, q, p6);
    UMAAL(A12, t, q, p7);
    UMAAL(A13, t2, 1, t);

    q = A6;
    t = 0;
    UMAAL(A8, t, q, p2 + 1);
    UMAAL(A9, t, q, p3);
    UMAAL(A10, t, q, p4);
    UMAAL(A11, t, q, p5);
    UMAAL(A12, t, q, p6);
    UMAAL(A13, t, q, p7);
    UMAAL(A14, t2, 1, t);

    q = A7;
    t = 0;
    UMAAL(A9, t, q, p2 + 1);
    UMAAL(A10, t, q, p3);
    UMAAL(A11, t, q, p4);
    UMAAL(A12, t, q, p5);
    UMAAL(A13, t, q, p6);
    UMAAL(A14, t, q, p7);
    UMAAL(A15, t2, 1, t);

    res[0] = A8;
    res[1] = A9;
    res[2] = A10;
    res[3] = A11;
    res[4] = A12;
    res[5] = A13;
    res[6] = A14;
    res[7] = A15;
}

static void modcpy_MFP1973_ct(const spint *a, spint *b) {
    b[0] = a[0];
    b[1] = a[1];
    b[2] = a[2];
    b[3] = a[3];
    b[4] = a[4];
    b[5] = a[5];
    b[6] = a[6];
    b[7] = a[7];
}

static void modnsqr_MFP1973_ct(spint *a, int n) {
    for (int i = 0; i < n; i++)
        modsqr_MFP1973_ct(a, a);
}

//Calculate progenitor
static void modpro_MFP1973_ct(const spint *w,spint *z) {
	spint x[8];
	spint t0[8];
	spint t1[8];
	spint t2[8];
	spint t3[8];
	spint t4[8];
	spint t5[8];
	spint t6[8];
	spint t7[8];
	spint t8[8];
	spint t9[8];
	spint t10[8];
	spint t11[8];
	spint t12[8];
	spint t13[8];
	spint t14[8];
	spint t15[8];
	spint t16[8];
	spint t17[8];
	spint t18[8];
	modcpy_MFP1973_ct(w,x);
	modsqr_MFP1973_ct(x,t13);
	modsqr_MFP1973_ct(t13,t11);
	modmul_MFP1973_ct(x,t11,t0);
	modsqr_MFP1973_ct(t11,t1);
	modmul_MFP1973_ct(t11,t1,t4);
	modmul_MFP1973_ct(t13,t4,t9);
	modmul_MFP1973_ct(t11,t9,t3);
	modsqr_MFP1973_ct(t4,z);
	modmul_MFP1973_ct(t13,z,t2);
	modmul_MFP1973_ct(t0,z,t7);
	modmul_MFP1973_ct(x,t7,t16);
	modmul_MFP1973_ct(t1,t7,t5);
	modmul_MFP1973_ct(t3,t5,t6);
	modmul_MFP1973_ct(t4,t6,t15);
	modmul_MFP1973_ct(t13,t15,t17);
	modmul_MFP1973_ct(t1,t15,t8);
	modmul_MFP1973_ct(t13,t8,t10);
	modmul_MFP1973_ct(t1,t10,t1);
	modmul_MFP1973_ct(t11,t1,t4);
	modmul_MFP1973_ct(t9,t1,t12);
	modmul_MFP1973_ct(t11,t12,t11);
	modmul_MFP1973_ct(t3,t11,t3);
	modmul_MFP1973_ct(t2,t3,t2);
	modmul_MFP1973_ct(t9,t2,t9);
	modmul_MFP1973_ct(z,t9,t14);
	modmul_MFP1973_ct(t13,t14,t13);
	modmul_MFP1973_ct(z,t13,z);
	modmul_MFP1973_ct(t16,z,t16);
	modsqr_MFP1973_ct(z,t18);
	modmul_MFP1973_ct(t4,t18,z);
	modnsqr_MFP1973_ct(t18,6);
	modmul_MFP1973_ct(t17,t18,t17);
	modnsqr_MFP1973_ct(t17,10);
	modmul_MFP1973_ct(t16,t17,t16);
	modnsqr_MFP1973_ct(t16,8);
	modmul_MFP1973_ct(t15,t16,t15);
	modnsqr_MFP1973_ct(t15,9);
	modmul_MFP1973_ct(t14,t15,t14);
	modnsqr_MFP1973_ct(t14,9);
	modmul_MFP1973_ct(t13,t14,t13);
	modnsqr_MFP1973_ct(t13,9);
	modmul_MFP1973_ct(t12,t13,t12);
	modnsqr_MFP1973_ct(t12,11);
	modmul_MFP1973_ct(t11,t12,t11);
	modnsqr_MFP1973_ct(t11,8);
	modmul_MFP1973_ct(t10,t11,t10);
	modnsqr_MFP1973_ct(t10,9);
	modmul_MFP1973_ct(t9,t10,t9);
	modnsqr_MFP1973_ct(t9,7);
	modmul_MFP1973_ct(t8,t9,t8);
	modnsqr_MFP1973_ct(t8,9);
	modmul_MFP1973_ct(t7,t8,t8);
	modnsqr_MFP1973_ct(t8,12);
	modmul_MFP1973_ct(t7,t8,t7);
	modnsqr_MFP1973_ct(t7,10);
	modmul_MFP1973_ct(t6,t7,t6);
	modnsqr_MFP1973_ct(t6,8);
	modmul_MFP1973_ct(t5,t6,t5);
	modnsqr_MFP1973_ct(t5,9);
	modmul_MFP1973_ct(t4,t5,t4);
	modnsqr_MFP1973_ct(t4,9);
	modmul_MFP1973_ct(t3,t4,t3);
	modnsqr_MFP1973_ct(t3,10);
	modmul_MFP1973_ct(t2,t3,t2);
	modnsqr_MFP1973_ct(t2,8);
	modmul_MFP1973_ct(t1,t2,t1);
	modnsqr_MFP1973_ct(t1,6);
	modmul_MFP1973_ct(t0,t1,t0);
	modnsqr_MFP1973_ct(t0,12);
	modmul_MFP1973_ct(z,t0,t0);
	modnsqr_MFP1973_ct(t0,9);
	modmul_MFP1973_ct(z,t0,t0);
	modnsqr_MFP1973_ct(t0,9);
	modmul_MFP1973_ct(z,t0,t0);
	modnsqr_MFP1973_ct(t0,9);
	modmul_MFP1973_ct(z,t0,t0);
	modnsqr_MFP1973_ct(t0,9);
	modmul_MFP1973_ct(z,t0,t0);
	modnsqr_MFP1973_ct(t0,9);
	modmul_MFP1973_ct(z,t0,t0);
	modnsqr_MFP1973_ct(t0,9);
	modmul_MFP1973_ct(z,t0,t0);
	modsqr_MFP1973_ct(t0,t0);
	modmul_MFP1973_ct(x,t0,t0);
	modnsqr_MFP1973_ct(t0,9);
	modmul_MFP1973_ct(z,t0,z);
}

//calculate inverse, provide progenitor h if available
static void modinv_MFP1973_ct(const spint *x,const spint *h,spint *z) {
	spint s[8];
	spint t[8];
	if (h==NULL) {
		modpro_MFP1973_ct(x,t);
	} else {
		modcpy_MFP1973_ct(h,t);
	}
	modcpy_MFP1973_ct(x,s);
	modnsqr_MFP1973_ct(t,2);
	modmul_MFP1973_ct(s,t,z);
}

static void nres_MFP1973_ct(const spint *a, spint *b) {
    uint32_t nresfactor[] = {1074164948, 590751150, 39459886, 548394689, 153118045, 816333227, 2083573053, 225634262};
    modmul_MFP1973_ct(a, nresfactor, b);
}

static void redc_MFP1973_ct(const spint *a, spint *b) {
    uint32_t rinv[] = {1, 0, 0, 0, 0, 0, 0, 0};
    modmul_MFP1973_ct(a, rinv, b);
    modfsb_MFP1973_ct(b);
}

static int modis1_MFP1973_ct(const spint *a) {
    spint c[8] = {0};
    spint d=0;
    redc_MFP1973_ct(a,c);
    c[0] = c[0] - 1;
    for(int i=0; i<8; i++){
        d|=c[i];
    }
    return 1 - ((d | -d) >> 31);
}
    
static int modis0_MFP1973_ct(const spint *a) {
    spint c[8] = {0};
    spint d=0;
    modcpy_MFP1973_ct(a, c);
    modfsb_MFP1973_ct(c);
    for(int i=0; i<8; i++){
        d|=c[i];
    }
    return 1 - ((d | -d) >> 31);
}
    
static void modzer_MFP1973_ct(spint *a) {
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    a[4] = 0;
    a[5] = 0;
    a[6] = 0;
    a[7] = 0;
}

static void modone_MFP1973_ct(spint *a) {
    a[0] = 4;
    a[1] = 0;
    a[2] = 716890112;
    a[3] = 1800242907;
    a[4] = 3808309027;
    a[5] = 828726889;
    a[6] = 1370003919;
    a[7] = 745899870;
}

static void modint_MFP1973_ct(int x, spint *a) {
    a[0] = (spint) x;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    a[4] = 0;
    a[5] = 0;
    a[6] = 0;
    a[7] = 0;
    nres_MFP1973_ct(a,a);
}

//Test for quadratic residue, provide progenitor h if available, NULL if not
static int modqr_MFP1973_ct(const spint *h,const spint *x) {
	spint r[8];
	if (h==NULL) {
		modpro_MFP1973_ct(x,r);
		modsqr_MFP1973_ct(r,r);
	} else {
		modsqr_MFP1973_ct(h,r);
	}
	modmul_MFP1973_ct(r,x,r);
	return modis1_MFP1973_ct(r);
}

static void modcmv_MFP1973_ct(int d, const spint *a, spint *b) {
    uint32_t t;
    d = -d;
    for (int i = 0; i < 8; i++) {
        t = d & (a[i] ^ b[i]);
        b[i] ^= t;
    }
}

static void modcsw_MFP1973_ct(int d, spint *a, spint *b) {
    uint32_t t;
    d = -d;
    for (int i = 0; i < 8; i++) {
        t = d & (a[i] ^ b[i]);
        a[i] ^= t;
        b[i] ^= t;
    }
}

//Modular square root, provide progenitor h if available, NULL if not
static void modsqrt_MFP1973_ct(const spint *x, const spint *h, spint *r) {
	spint s[8];
	spint y[8];
	if (h==NULL) {
		modpro_MFP1973_ct(x,y);
	} else {
		modcpy_MFP1973_ct(h,y);
	}
	modmul_MFP1973_ct(y,x,s);
	modcpy_MFP1973_ct(s,r);
}

static void modshl_MFP1973_ct(int n, spint *a) {
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
static uint32_t modshr_MFP1973_ct(int n, spint *a) {
    uint32_t r = a[0] & ((1 << n) - 1);
    a[0] = (a[0] >> n) | (a[0 + 1] << (32u - n));
    a[1] = (a[1] >> n) | (a[1 + 1] << (32u - n));
    a[2] = (a[2] >> n) | (a[2 + 1] << (32u - n));
    a[3] = (a[3] >> n) | (a[3 + 1] << (32u - n));
    a[4] = (a[4] >> n) | (a[4 + 1] << (32u - n));
    a[5] = (a[5] >> n) | (a[5 + 1] << (32u - n));
    a[6] = (a[6] >> n) | (a[6 + 1] << (32u - n));
    a[7] = a[7] >> n;
    return r;
}

// Code for export, not exponentiation
static void modexp_MFP1973_ct(const spint *a, char *b) {
    uint32_t c[8];
    redc_MFP1973_ct(a, c);
    for (int i = 31; i>= 0; i--) {
        b[i] = c[0] & (uint32_t)0xff;
        (void)modshr_MFP1973_ct(8, c);
    }
}

static void modimp_MFP1973_ct(const char *b, spint *a) {
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    a[4] = 0;
    a[5] = 0;
    a[6] = 0;
    a[7] = 0;
    for (int i=0; i<32; i++){
        modshl_MFP1973_ct(8,a);
        a[0] += (spint)(unsigned char)b[i];
    }
    nres_MFP1973_ct(a,a);
}

#define Nlimbs_MFP1973_ct 8
#define Nbits_MFP1973_ct 254
#define Nbytes_MFP1973_ct 32

/* API functions calling generated code */

const digit_t p[NWORDS_FIELD] =  { 0xFFFFFFFF, 0xFFFFFFFF, 0x355147FF, 0x252C9E49, 0x87407437, 0x33A6A865, 0x6B95D98C, 0x34E29E28 };
const digit_t pre[NWORDS_FIELD] = { 0xB9713934, 0x14CF4F2D, 0x7482B63C, 0x61C30BED, 0xF05CE6CB, 0x3DA9F365, 0xA8AE3E69, 0x10B5541E };

bool fp_is_equal(const digit_t* a, const digit_t* b) {
    spint c[8], d[8];
    int neq = 0;
    redc_MFP1973_ct(a, c);
    redc_MFP1973_ct(b, d);
    for (int i = 0; i < 8; i++) {
        neq |= c[i] ^ d[i];
    }
    neq = (neq | -neq) >> 31;
    return 1 - (neq & 1);
}

bool fp_is_zero(const digit_t* a) {
    return (bool) modis0_MFP1973_ct(a);
}

void fp_copy(digit_t* out, const digit_t* a) {
    modcpy_MFP1973_ct(a, out);
}

void fp_add(digit_t* out, const digit_t* a, const digit_t* b) {
    modadd_MFP1973_ct(a, b, out);
}

void fp_sub(digit_t* out, const digit_t* a, const digit_t* b) {
    modsub_MFP1973_ct(a, b, out);
}

void fp_neg(digit_t* out, const digit_t* a) {
    spint zero[NWORDS_FIELD];
    modzer_MFP1973_ct(zero);
    modsub_MFP1973_ct(zero, a, out);
}

void fp_sqr(digit_t* out, const digit_t* a) {
    modsqr_MFP1973_ct(a, out);
}

void fp_mul(digit_t* out, const digit_t* a, const digit_t* b) {
    modmul_MFP1973_ct(a, b, out);
}

void fp_sqrt(digit_t* a) {
    modsqrt_MFP1973_ct(a, NULL, a);
}

void fp_exp3div4(digit_t* out, const digit_t* a) {
    modpro_MFP1973_ct(a, out);
}

void fp_tomont(digit_t* out, const digit_t* a) {
    nres_MFP1973_ct(a, out);
}

void fp_frommont(digit_t* out, const digit_t* a) {
    redc_MFP1973_ct(a, out);
}

void fp_mont_setone(digit_t* out) {
    modone_MFP1973_ct(out);
}

void fp_to_digit_array(digit_t* out, const digit_t* a) {
    digit_t x[NWORDS_FIELD];
    modcpy_MFP1973_ct(a, x);
    for (int i = 0; i < NWORDS_ORDER; i++) {
        out[i] = 0;
    }
    for (int i = 0; i < 32; i++) {
        ((char *) out)[i] = x[0] & 0xff;
        modshr_MFP1973_ct(8, x);
    }
}

void fp_from_digit_array(digit_t* out, const digit_t* a) {
    for (int i = 0; i < NWORDS_FIELD; i++) {
        out[i] = 0;
    }
    for (int i = 32 - 1; i >= 0; i--) {
        modshl_MFP1973_ct(8, out);
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
