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

static void modfsb_MFP248_ct(spint *a) {
    const spint p0 = 4294967295;
    const spint p1 = 4294967295;
    const spint p2 = 4294967295;
    const spint p3 = 4294967295;
    const spint p4 = 4294967295;
    const spint p5 = 4294967295;
    const spint p6 = 4294967295;
    const spint p7 = 83886079;
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

static void modadd_MFP248_ct(const spint *a, const spint *b, spint *res) {
    uint32_t a0 = a[0], a1 = a[1], a2 = a[2], a3 = a[3], a4 = a[4], a5 = a[5], a6 = a[6], a7 = a[7];
    uint32_t b0 = b[0], b1 = b[1], b2 = b[2], b3 = b[3], b4 = b[4], b5 = b[5], b6 = b[6], b7 = b[7];
    uint32_t res0, res1, res2, res3, res4, res5, res6, res7;
    uint32_t t = 0;
    const uint32_t p0 = 4294967294;
    const uint32_t p1 = 4294967295;
    const uint32_t p2 = 4294967295;
    const uint32_t p3 = 4294967295;
    const uint32_t p4 = 4294967295;
    const uint32_t p5 = 4294967295;
    const uint32_t p6 = 4294967295;
    const uint32_t p7 = 167772159;
    UMAAL(a0, t, 1, b0);
    UMAAL(a1, t, 1, b1);
    UMAAL(a2, t, 1, b2);
    UMAAL(a3, t, 1, b3);
    UMAAL(a4, t, 1, b4);
    UMAAL(a5, t, 1, b5);
    UMAAL(a6, t, 1, b6);
    UMAAL(a7, t, 1, b7);
    res[0] = a0;
    res0 = a0;
    res[1] = a1;
    res1 = a1;
    res[2] = a2;
    res2 = a2;
    res[3] = a3;
    res3 = a3;
    res[4] = a4;
    res4 = a4;
    res[5] = a5;
    res5 = a5;
    res[6] = a6;
    res6 = a6;
    res[7] = a7;
    res7 = a7;
    asm volatile(
        "subs %[res0], %[res0], %[p0]\n"
        "sbcs %[res1], %[res1], %[p1]\n"
        "sbcs %[res2], %[res2], %[p2]\n"
        "sbcs %[res3], %[res3], %[p3]\n"
        "sbcs %[res4], %[res4], %[p4]\n"
        "sbcs %[res5], %[res5], %[p5]\n"
        "sbcs %[res6], %[res6], %[p6]\n"
        "sbcs %[res7], %[res7], %[p7]\n"
        "itttt cs\n"
        "strdcs %[res0], %[res1], [%[res]]\n"
        "strdcs %[res2], %[res3], [%[res], #8]\n"
        "strdcs %[res4], %[res5], [%[res], #16]\n"
        "strdcs %[res6], %[res7], [%[res], #24]\n"
        : [res0] "+&r"(res0), [res1] "+&r"(res1), [res2] "+&r"(res2), [res3] "+&r"(res3), [res4] "+&r"(res4),
          [res5] "+&r"(res5), [res6] "+&r"(res6), [res7] "+&r"(res7)
        : [res] "r"(res), [p0] "r"(p0), [p1] "r"(p1), [p2] "r"(p2), [p3] "r"(p3), [p4] "r"(p4), [p5] "r"(p5),
          [p6] "r"(p6), [p7] "r"(p7)
        : "cc", "memory");
}

static void modsub_MFP248_ct(const spint *a, const spint *b, spint *res) {
    uint32_t a0 = a[0], a1 = a[1], a2 = a[2], a3 = a[3], a4 = a[4], a5 = a[5], a6 = a[6], a7 = a[7];
    uint32_t t = 0, t1, t2;
    const uint32_t p0 = 4294967294;
    const uint32_t p1 = 4294967295;
    const uint32_t p2 = 4294967295;
    const uint32_t p3 = 4294967295;
    const uint32_t p4 = 4294967295;
    const uint32_t p5 = 4294967295;
    const uint32_t p6 = 4294967295;
    const uint32_t p7 = 167772159;
    asm volatile(
        "ldrd %[t1], %[t2], [%[b]]\n"
        "subs %[a0], %[a0], %[t1]\n"
        "sbcs %[a1], %[a1], %[t2]\n"
        "ldrd %[t1], %[t2], [%[b], #8]\n"
        "sbcs %[a2], %[a2], %[t1]\n"
        "sbcs %[a3], %[a3], %[t2]\n"
        "ldrd %[t1], %[t2], [%[b], #16]\n"
        "sbcs %[a4], %[a4], %[t1]\n"
        "sbcs %[a5], %[a5], %[t2]\n"
        "ldrd %[t1], %[t2], [%[b], #24]\n"
        "sbcs %[a6], %[a6], %[t1]\n"
        "sbcs %[a7], %[a7], %[t2]\n"
        "sbcs %[t], %[t], %[t]\n"
        : [a0] "+&r"(a0), [a1] "+&r"(a1), [a2] "+&r"(a2), [a3] "+&r"(a3), [a4] "+&r"(a4), [a5] "+&r"(a5),
          [a6] "+&r"(a6), [a7] "+&r"(a7), [t] "+&r"(t), [t1] "=&r"(t1), [t2] "=&r"(t2)
        : [b] "r"(b)
        : "cc", "memory");
    t = -t;
    t2 = 0;
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

static void modneg_MFP248_ct(const spint *a, spint *r) {
    spint zero[8] = {0};
    modsub_MFP248_ct(zero,a,r);
}
    
static void mul_MFP248_ct(const spint *p, const spint *q, spint *r) {
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
    b0 = q[4];
    b1 = q[5];
    b2 = q[6];
    b3 = q[7];
    t0 = r[4];
    t1 = r[5];
    t2 = r[6];
    t3 = r[7];
    t4 = 0;
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
#ifdef __clang__
        "adcs   %[t0], %[t0], %[a] \n"
#else
        "adcs.w %[t0], %[t0], %[a] \n"
#endif
        "ldr.w   %[a], [%[p],#8] \n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#40] \n"
        "str.w  %[t2], [%[r],#24] \n"
        "mov.w  %[t2], #0 \n"
#ifdef __clang__
        "adcs   %[t1], %[t1], %[a] \n"
#else
        "adcs.w %[t1], %[t1], %[a] \n"
#endif
        "ldr.w   %[a], [%[p],#12] \n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r],#44] \n"
        "str.w  %[t3], [%[r],#28] \n"
        "mov.w  %[t3], #0 \n"
#ifdef __clang__
        "adcs   %[t2], %[t2], %[a] \n"
#else
        "adcs.w %[t2], %[t2], %[a] \n"
#endif
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
        "adcs   %[t1], #0 \n"
#else
        "adcs.w %[t1], #0 \n"
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

static void sqr_MFP248_ct(const spint *a, spint *res) {
    spint t5, t6, t7, t8, t9, t10, t11, t12, t13, t14;
    spint T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15;
    spint T;

    spint t, a0, a1, a2, a3, a4;

    T = 0;

    asm volatile(
        "ldr   %[a0], [%[a]]               \n"
        "ldr   %[a1], [%[a], #4]           \n"
        "ldr   %[a2], [%[a], #8]           \n"
        "ldr   %[a3], [%[a], #12]          \n"
        "ldr   %[a4], [%[a], #16]          \n"
        "umull %[t],  %[t8],  %[a0],  %[a1]\n"
        "umull %[T0], %[T1],  %[a0],  %[a0]\n"
        "umaal %[T1], %[T],   %[two], %[t] \n"
        "str   %[T0], [%[res]]             \n"
        "str   %[T1], [%[res], #4]         \n"
        "mov   %[t], #0                    \n"
        "umaal %[t],  %[t8],  %[a0],  %[a2]\n"
        "umull %[T0], %[T1],  %[a1],  %[a1]\n"
        "umaal %[T0], %[T],   %[two], %[t] \n"
        "umull %[t],  %[t10], %[a0],  %[a3]\n"
        "umaal %[t],  %[t8],  %[a1],  %[a2]\n"
        "umaal %[T1], %[T],   %[two], %[t] \n"
        "str   %[T0], [%[res], #8]         \n"
        "str   %[T1], [%[res], #12]        \n"
        "mov   %[t], #0                    \n"
        "umaal %[t],  %[t10], %[a0],  %[a4]\n"
        "umaal %[t],  %[t8],  %[a1],  %[a3]\n"
        "umull %[T0], %[T1],  %[a2],  %[a2]\n"
        "umaal %[T0], %[T],   %[two], %[t] \n"
        "str   %[T0], [%[res], #16]        \n"
        : [a0] "=&r"(a0), [a1] "=&r"(a1), [a2] "=&r"(a2), [a3] "=&r"(a3), [a4] "=&r"(a4), [t] "=&r"(t), [t8] "=&r"(t8),
          [t10] "=&r"(t10), [T0] "=&r"(T4), [T1] "=&r"(T5), [T] "+&r"(T)
        : [a] "r"(a), [res] "r"(res), [two] "r"(2)
        : "memory");

    uint32_t a5 = a[5];
    UMULL(t5, t12, a0, a5);
    UMAAL(t5, t10, a1, a4);
    UMAAL(t5, t8, a2, a3);
    UMAAL(T5, T, 2, t5);
    res[5] = T5;

    t6 = 0;
    uint32_t a6 = a[6];
    UMAAL(t6, t12, a0, a6);
    UMAAL(t6, t10, a1, a5);
    UMAAL(t6, t8, a2, a4);
    UMULL(T6, T7, a3, a3);
    UMAAL(T6, T, 2, t6);
    res[6] = T6;

    uint32_t a7 = a[7];
    UMULL(t7, t14, a0, a7);
    UMAAL(t7, t12, a1, a6);
    UMAAL(t7, t10, a2, a5);
    UMAAL(t7, t8, a3, a4);
    UMAAL(T7, T, 2, t7);
    res[7] = T7;

    UMAAL(t8, t14, a1, a7);
    UMAAL(t8, t12, a2, a6);
    UMAAL(t8, t10, a3, a5);
    UMULL(T8, T9, a4, a4);
    UMAAL(T8, T, 2, t8);
    res[8] = T8;

    t9 = 0;
    UMAAL(t9, t14, a2, a7);
    UMAAL(t9, t12, a3, a6);
    UMAAL(t9, t10, a4, a5);
    UMAAL(T9, T, 2, t9);
    res[9] = T9;

    UMAAL(t10, t14, a3, a7);
    UMAAL(t10, t12, a4, a6);
    UMULL(T10, T11, a5, a5);
    UMAAL(T10, T, 2, t10);
    res[10] = T10;

    t11 = 0;
    UMAAL(t11, t14, a4, a7);
    UMAAL(t11, t12, a5, a6);
    UMAAL(T11, T, 2, t11);
    res[11] = T11;

    UMAAL(t12, t14, a5, a7);
    UMULL(T12, T13, a6, a6);
    UMAAL(T12, T, 2, t12);
    res[12] = T12;

    t13 = 0;
    UMAAL(t13, t14, a6, a7);
    UMAAL(T13, T, 2, t13);
    res[13] = T13;

    UMULL(T14, T15, a7, a7);
    UMAAL(T14, T, 2, t14);
    res[14] = T14;

    T15 += T;
    res[15] = T15;
}

static void mli_MFP248_ct(const spint *p, const spint q, spint *r) {
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

static void modmul_MFP248_ct(const spint *a, const spint *b, spint *res) {
    spint A[16];
    spint A0, A1, A2, A3, A7, A8, A9, A10, A11;
    const uint32_t p7 = 83886079;
    spint t;

    mul_MFP248_ct(a, b, A);
    asm volatile(
        "ldr   %[A0_4],   [%[A], #0]                 \n"
        "ldr   %[A1_5],   [%[A], #4]                 \n"
        "ldr   %[A2_6],   [%[A], #8]                 \n"
        "ldr   %[A3_15],  [%[A], #12]                \n"
        "ldr   %[A7],     [%[A], #28]                \n"
        "ldr   %[A8_12],  [%[A], #32]                \n"
        "ldr   %[A9_13],  [%[A], #36]                \n"
        "ldr   %[A10_14], [%[A], #40]                \n"
        "ldr   %[A11],    [%[A], #44]                \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A7],     %[t],      %[A0_4],  %[p71]\n"
        "adds  %[A8_12],  %[A8_12],  %[t]            \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A8_12],  %[t],      %[A1_5],  %[p71]\n"
        "str   %[A8_12],  [%[res], #0]               \n"
        "adcs  %[A9_13],  %[A9_13],  %[t]            \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A9_13],  %[t],      %[A2_6],  %[p71]\n"
        "str   %[A9_13],  [%[res], #4]               \n"
        "adcs  %[A10_14], %[A10_14], %[t]            \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A10_14], %[t],      %[A3_15], %[p71]\n"
        "str   %[A10_14], [%[res], #8]               \n"
        "adcs  %[A11],    %[A11],    %[t]            \n"
        "ldr   %[A0_4],   [%[A], #16]                \n"
        "ldr   %[A1_5],   [%[A], #20]                \n"
        "ldr   %[A2_6],   [%[A], #24]                \n"
        "ldr   %[A8_12],  [%[A], #48]                \n"
        "ldr   %[A9_13],  [%[A], #52]                \n"
        "ldr   %[A10_14], [%[A], #56]                \n"
        "ldr   %[A3_15],  [%[A], #60]                \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A11],    %[t],      %[A0_4],  %[p71]\n"
        "str   %[A11],    [%[res], #12]              \n"
        "adcs  %[A8_12],  %[A8_12],  %[t]            \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A8_12],  %[t],      %[A1_5],  %[p71]\n"
        "str   %[A8_12],  [%[res], #16]              \n"
        "adcs  %[A9_13],  %[A9_13],  %[t]            \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A9_13],  %[t],      %[A2_6],  %[p71]\n"
        "str   %[A9_13],  [%[res], #20]              \n"
        "adcs  %[A10_14], %[A10_14], %[t]            \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A10_14], %[t],      %[A7],    %[p71]\n"
        "str   %[A10_14], [%[res], #24]              \n"
        "adcs  %[A3_15],  %[A3_15],  %[t]            \n"
        "str   %[A3_15],  [%[res], #28]              \n"
// Conditional subtraction (not needed for primes with 2bit slack from representation)
#if 0
        "sub   %[p71],    %[p71],     #1             \n"
        "ldr   %[A0_4],   [%[res], #0]               \n"
        "ldr   %[A1_5],   [%[res], #4]               \n"
        "ldr   %[A2_6],   [%[res], #8]               \n"
        "subs  %[A0_4],   %[A0_4],    #-1            \n"
        "sbcs  %[A1_5],   %[A1_5],    #-1            \n"
        "sbcs  %[A2_6],   %[A2_6],    #-1            \n"
        "sbcs  %[A11],    %[A11],     #-1            \n"
        "sbcs  %[A8_12],  %[A8_12],   #-1            \n"
        "sbcs  %[A9_13],  %[A9_13],   #-1            \n"
        "sbcs  %[A10_14], %[A10_14],  #-1            \n"
        "sbcs  %[A3_15],  %[A3_15],   %[p71]         \n"
        "itttt cs                                    \n"
        "strdcs %[A0_4],   %[A1_5],  [%[res]]        \n"
        "strdcs %[A2_6],   %[A11],   [%[res], #8]    \n"
        "strdcs %[A8_12],  %[A9_13], [%[res], #16]   \n"
        "strdcs %[A10_14], %[A3_15], [%[res], #24]   \n"
#endif
        : [A0_4] "=&r"(A0), [A1_5] "=&r"(A1), [A2_6] "=&r"(A2), [A3_15] "=&r"(A3), [A7] "=&r"(A7), [A8_12] "=&r"(A8),
          [A9_13] "=&r"(A9), [A10_14] "=&r"(A10), [A11] "=&r"(A11), [t] "=&r"(t)
        : [A] "r"(A), [res] "r"(res), [p71] "r"(p7 + 1)
        : "cc", "memory");
}

static void modsqr_MFP248_ct(const spint *a, spint *res) {
    spint A[16];
    spint A0, A1, A2, A3, A7, A8, A9, A10, A11;
    const uint32_t p7 = 83886079;
    spint t;

    sqr_MFP248_ct(a, A);

    asm volatile(
        "ldr   %[A0_4],   [%[A], #0]                 \n"
        "ldr   %[A1_5],   [%[A], #4]                 \n"
        "ldr   %[A2_6],   [%[A], #8]                 \n"
        "ldr   %[A3_15],  [%[A], #12]                \n"
        "ldr   %[A7],     [%[A], #28]                \n"
        "ldr   %[A8_12],  [%[A], #32]                \n"
        "ldr   %[A9_13],  [%[A], #36]                \n"
        "ldr   %[A10_14], [%[A], #40]                \n"
        "ldr   %[A11],    [%[A], #44]                \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A7],     %[t],      %[A0_4],  %[p71]\n"
        "adds  %[A8_12],  %[A8_12],  %[t]            \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A8_12],  %[t],      %[A1_5],  %[p71]\n"
        "str   %[A8_12],  [%[res], #0]               \n"
        "adcs  %[A9_13],  %[A9_13],  %[t]            \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A9_13],  %[t],      %[A2_6],  %[p71]\n"
        "str   %[A9_13],  [%[res], #4]               \n"
        "adcs  %[A10_14], %[A10_14], %[t]            \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A10_14], %[t],      %[A3_15], %[p71]\n"
        "str   %[A10_14], [%[res], #8]               \n"
        "adcs  %[A11],    %[A11],    %[t]            \n"
        "ldr   %[A0_4],   [%[A], #16]                \n"
        "ldr   %[A1_5],   [%[A], #20]                \n"
        "ldr   %[A2_6],   [%[A], #24]                \n"
        "ldr   %[A8_12],  [%[A], #48]                \n"
        "ldr   %[A9_13],  [%[A], #52]                \n"
        "ldr   %[A10_14], [%[A], #56]                \n"
        "ldr   %[A3_15],  [%[A], #60]                \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A11],    %[t],      %[A0_4],  %[p71]\n"
        "str   %[A11],    [%[res], #12]              \n"
        "adcs  %[A8_12],  %[A8_12],  %[t]            \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A8_12],  %[t],      %[A1_5],  %[p71]\n"
        "str   %[A8_12],  [%[res], #16]              \n"
        "adcs  %[A9_13],  %[A9_13],  %[t]            \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A9_13],  %[t],      %[A2_6],  %[p71]\n"
        "str   %[A9_13],  [%[res], #20]              \n"
        "adcs  %[A10_14], %[A10_14], %[t]            \n"
        "mov   %[t],      #0                         \n"
        "umaal %[A10_14], %[t],      %[A7],    %[p71]\n"
        "str   %[A10_14], [%[res], #24]              \n"
        "adcs  %[A3_15],  %[A3_15],  %[t]            \n"
        "str   %[A3_15],  [%[res], #28]              \n"
// Conditional subtraction (not needed for primes with 2bit slack from representation)
#if 0
        "sub   %[p71],    %[p71],     #1             \n"
        "ldr   %[A0_4],   [%[res], #0]               \n"
        "ldr   %[A1_5],   [%[res], #4]               \n"
        "ldr   %[A2_6],   [%[res], #8]               \n"
        "subs  %[A0_4],   %[A0_4],    #-1            \n"
        "sbcs  %[A1_5],   %[A1_5],    #-1            \n"
        "sbcs  %[A2_6],   %[A2_6],    #-1            \n"
        "sbcs  %[A11],    %[A11],     #-1            \n"
        "sbcs  %[A8_12],  %[A8_12],   #-1            \n"
        "sbcs  %[A9_13],  %[A9_13],   #-1            \n"
        "sbcs  %[A10_14], %[A10_14],  #-1            \n"
        "sbcs  %[A3_15],  %[A3_15],   %[p71]         \n"
        "itttt cs                                    \n"
        "strdcs %[A0_4],   %[A1_5],  [%[res]]        \n"
        "strdcs %[A2_6],   %[A11],   [%[res], #8]    \n"
        "strdcs %[A8_12],  %[A9_13], [%[res], #16]   \n"
        "strdcs %[A10_14], %[A3_15], [%[res], #24]   \n"
#endif
        : [A0_4] "=&r"(A0), [A1_5] "=&r"(A1), [A2_6] "=&r"(A2), [A3_15] "=&r"(A3), [A7] "=&r"(A7), [A8_12] "=&r"(A8),
          [A9_13] "=&r"(A9), [A10_14] "=&r"(A10), [A11] "=&r"(A11), [t] "=&r"(t)
        : [A] "r"(A), [res] "r"(res), [p71] "r"(p7 + 1)
        : "cc", "memory");
}

static void modmli_MFP248_ct(const spint *a, const spint b, spint *res) {
    spint A[16];
    spint A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15;
    const spint p7 = 83886079;
    spint q, t, t2 = 0;

    mli_MFP248_ct(a, b, A);

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
    UMAAL(A7, t, q, p7 + 1);
    UMAAL(A8, t2, 1, t);

    q = A1;
    t = 0;
    UMAAL(A8, t, q, p7 + 1);
    UMAAL(A9, t2, 1, t);

    q = A2;
    t = 0;
    UMAAL(A9, t, q, p7 + 1);
    UMAAL(A10, t2, 1, t);

    q = A3;
    t = 0;
    UMAAL(A10, t, q, p7 + 1);
    UMAAL(A11, t2, 1, t);

    q = A4;
    t = 0;
    UMAAL(A11, t, q, p7 + 1);
    UMAAL(A12, t2, 1, t);

    q = A5;
    t = 0;
    UMAAL(A12, t, q, p7 + 1);
    UMAAL(A13, t2, 1, t);

    q = A6;
    t = 0;
    UMAAL(A13, t, q, p7 + 1);
    UMAAL(A14, t2, 1, t);

    q = A7;
    t = 0;
    UMAAL(A14, t, q, p7 + 1);
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

static void modcpy_MFP248_ct(const spint *a, spint *b) {
    b[0] = a[0];
    b[1] = a[1];
    b[2] = a[2];
    b[3] = a[3];
    b[4] = a[4];
    b[5] = a[5];
    b[6] = a[6];
    b[7] = a[7];
}

static void modnsqr_MFP248_ct(spint *a, int n) {
    for (int i = 0; i < n; i++)
        modsqr_MFP248_ct(a, a);
}

//Calculate progenitor
static void modpro_MFP248_ct(const spint *w,spint *z) {
	spint x[8];
	spint t0[8];
	spint t1[8];
	spint t2[8];
	spint t3[8];
	spint t4[8];
	modcpy_MFP248_ct(w,x);
	modsqr_MFP248_ct(x,z);
	modmul_MFP248_ct(x,z,t0);
	modsqr_MFP248_ct(t0,z);
	modmul_MFP248_ct(x,z,z);
	modsqr_MFP248_ct(z,t1);
	modsqr_MFP248_ct(t1,t3);
	modsqr_MFP248_ct(t3,t2);
	modcpy_MFP248_ct(t2,t4);
	modnsqr_MFP248_ct(t4,3);
	modmul_MFP248_ct(t2,t4,t2);
	modcpy_MFP248_ct(t2,t4);
	modnsqr_MFP248_ct(t4,6);
	modmul_MFP248_ct(t2,t4,t2);
	modcpy_MFP248_ct(t2,t4);
	modnsqr_MFP248_ct(t4,2);
	modmul_MFP248_ct(t3,t4,t3);
	modnsqr_MFP248_ct(t3,13);
	modmul_MFP248_ct(t2,t3,t2);
	modcpy_MFP248_ct(t2,t3);
	modnsqr_MFP248_ct(t3,27);
	modmul_MFP248_ct(t2,t3,t2);
	modmul_MFP248_ct(z,t2,z);
	modcpy_MFP248_ct(z,t2);
	modnsqr_MFP248_ct(t2,4);
	modmul_MFP248_ct(t1,t2,t1);
	modmul_MFP248_ct(t0,t1,t0);
	modmul_MFP248_ct(t1,t0,t1);
	modmul_MFP248_ct(t0,t1,t0);
	modmul_MFP248_ct(t1,t0,t2);
	modmul_MFP248_ct(t0,t2,t0);
	modmul_MFP248_ct(t1,t0,t1);
	modnsqr_MFP248_ct(t1,63);
	modmul_MFP248_ct(t0,t1,t1);
	modnsqr_MFP248_ct(t1,64);
	modmul_MFP248_ct(t0,t1,t0);
	modnsqr_MFP248_ct(t0,57);
	modmul_MFP248_ct(z,t0,z);
}

//calculate inverse, provide progenitor h if available
static void modinv_MFP248_ct(const spint *x,const spint *h,spint *z) {
	spint s[8];
	spint t[8];
	if (h==NULL) {
		modpro_MFP248_ct(x,t);
	} else {
		modcpy_MFP248_ct(h,t);
	}
	modcpy_MFP248_ct(x,s);
	modnsqr_MFP248_ct(t,2);
	modmul_MFP248_ct(s,t,z);
}

static void nres_MFP248_ct(const spint *a, spint *b) {
    uint32_t nresfactor[] = {858996080, 858993459, 858993459, 858993459, 858993459, 858993459, 858993459, 53687091};
    modmul_MFP248_ct(a, nresfactor, b);
}

static void redc_MFP248_ct(const spint *a, spint *b) {
    uint32_t rinv[] = {1, 0, 0, 0, 0, 0, 0, 0};
    modmul_MFP248_ct(a, rinv, b);
    modfsb_MFP248_ct(b);
}

static int modis1_MFP248_ct(const spint *a) {
    spint c[8] = {0};
    spint d=0;
    redc_MFP248_ct(a,c);
    c[0] = c[0] - 1;
    for(int i=0; i<8; i++){
        d|=c[i];
    }
    return 1 - ((d | -d) >> 31);
}
    
static int modis0_MFP248_ct(const spint *a) {
    spint c[8] = {0};
    spint d=0;
    modcpy_MFP248_ct(a, c);
    modfsb_MFP248_ct(c);
    for(int i=0; i<8; i++){
        d|=c[i];
    }
    return 1 - ((d | -d) >> 31);
}
    
static void modzer_MFP248_ct(spint *a) {
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    a[4] = 0;
    a[5] = 0;
    a[6] = 0;
    a[7] = 0;
}

static void modone_MFP248_ct(spint *a) {
    a[0] = 51;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    a[4] = 0;
    a[5] = 0;
    a[6] = 0;
    a[7] = 16777216;
}

static void modint_MFP248_ct(int x, spint *a) {
    a[0] = (spint) x;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    a[4] = 0;
    a[5] = 0;
    a[6] = 0;
    a[7] = 0;
    nres_MFP248_ct(a,a);
}

//Test for quadratic residue, provide progenitor h if available, NULL if not
static int modqr_MFP248_ct(const spint *h,const spint *x) {
	spint r[8];
	if (h==NULL) {
		modpro_MFP248_ct(x,r);
		modsqr_MFP248_ct(r,r);
	} else {
		modsqr_MFP248_ct(h,r);
	}
	modmul_MFP248_ct(r,x,r);
	return modis1_MFP248_ct(r);
}

static void modcmv_MFP248_ct(int d, const spint *a, spint *b) {
    uint32_t t;
    d = -d;
    for (int i = 0; i < 8; i++) {
        t = d & (a[i] ^ b[i]);
        b[i] ^= t;
    }
}

static void modcsw_MFP248_ct(int d, spint *a, spint *b) {
    uint32_t t;
    d = -d;
    for (int i = 0; i < 8; i++) {
        t = d & (a[i] ^ b[i]);
        a[i] ^= t;
        b[i] ^= t;
    }
}

//Modular square root, provide progenitor h if available, NULL if not
static void modsqrt_MFP248_ct(const spint *x, const spint *h, spint *r) {
	spint s[8];
	spint y[8];
	if (h==NULL) {
		modpro_MFP248_ct(x,y);
	} else {
		modcpy_MFP248_ct(h,y);
	}
	modmul_MFP248_ct(y,x,s);
	modcpy_MFP248_ct(s,r);
}

static void modshl_MFP248_ct(int n, spint *a) {
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
static uint32_t modshr_MFP248_ct(int n, spint *a) {
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
static void modexp_MFP248_ct(const spint *a, char *b) {
    uint32_t c[8];
    redc_MFP248_ct(a, c);
    for (int i = 31; i>= 0; i--) {
        b[i] = c[0] & (uint32_t)0xff;
        (void)modshr_MFP248_ct(8, c);
    }
}

static void modimp_MFP248_ct(const char *b, spint *a) {
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    a[4] = 0;
    a[5] = 0;
    a[6] = 0;
    a[7] = 0;
    for (int i=0; i<32; i++){
        modshl_MFP248_ct(8,a);
        a[0] += (spint)(unsigned char)b[i];
    }
    nres_MFP248_ct(a,a);
}

#define Nlimbs_MFP248_ct 8
#define Nbits_MFP248_ct 251
#define Nbytes_MFP248_ct 32

/* API functions calling generated code */

const digit_t p[NWORDS_FIELD] =  { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,0xffffffff, 0x04ffffff};
const digit_t pre[NWORDS_FIELD] = { 0xd441b778, 0x86b4b5bd, 0x69c1878d, 0xbea87c37, 0x34ef11ef, 0xba9109a9, 0xdd00b817, 0x00efabb8 };

bool fp_is_equal(const digit_t* a, const digit_t* b) {
    spint c[8], d[8];
    int neq = 0;
    redc_MFP248_ct(a, c);
    redc_MFP248_ct(b, d);
    for (int i = 0; i < 8; i++) {
        neq |= c[i] ^ d[i];
    }
    neq = (neq | -neq) >> 31;
    return 1 - (neq & 1);
}

bool fp_is_zero(const digit_t* a) {
    return (bool) modis0_MFP248_ct(a);
}

void fp_copy(digit_t* out, const digit_t* a) {
    modcpy_MFP248_ct(a, out);
}

void fp_add(digit_t* out, const digit_t* a, const digit_t* b) {
    modadd_MFP248_ct(a, b, out);
}

void fp_sub(digit_t* out, const digit_t* a, const digit_t* b) {
    modsub_MFP248_ct(a, b, out);
}

void fp_neg(digit_t* out, const digit_t* a) {
    spint zero[NWORDS_FIELD];
    modzer_MFP248_ct(zero);
    modsub_MFP248_ct(zero, a, out);
}

void fp_sqr(digit_t* out, const digit_t* a) {
    modsqr_MFP248_ct(a, out);
}

void fp_mul(digit_t* out, const digit_t* a, const digit_t* b) {
    modmul_MFP248_ct(a, b, out);
}

void fp_sqrt(digit_t* a) {
    modsqrt_MFP248_ct(a, NULL, a);
}

void fp_exp3div4(digit_t* out, const digit_t* a) {
    modpro_MFP248_ct(a, out);
}

void fp_tomont(digit_t* out, const digit_t* a) {
    nres_MFP248_ct(a, out);
}

void fp_frommont(digit_t* out, const digit_t* a) {
    redc_MFP248_ct(a, out);
}

void fp_mont_setone(digit_t* out) {
    modone_MFP248_ct(out);
}

void fp_to_digit_array(digit_t* out, const digit_t* a) {
    digit_t x[NWORDS_FIELD];
    modcpy_MFP248_ct(a, x);
    for (int i = 0; i < NWORDS_ORDER; i++) {
        out[i] = 0;
    }
    for (int i = 0; i < 32; i++) {
        ((char *) out)[i] = x[0] & 0xff;
        modshr_MFP248_ct(8, x);
    }
}

void fp_from_digit_array(digit_t* out, const digit_t* a) {
    for (int i = 0; i < NWORDS_FIELD; i++) {
        out[i] = 0;
    }
    for (int i = 32 - 1; i >= 0; i--) {
        modshl_MFP248_ct(8, out);
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
