#include <stdint.h>
#include <stddef.h>

#define UMULL(rdlo, rdhi, rn, rm) asm volatile("umull %0, %1, %2, %3" : "=r"(rdlo), "=r"(rdhi) : "r"(rn), "r"(rm))
#define UMAAL(rdlo, rdhi, rn, rm) asm volatile("umaal %0, %1, %2, %3" : "+r"(rdlo), "+r"(rdhi) : "r"(rn), "r"(rm))
#define ADDS(rt, r1, r2) asm volatile("adds %0, %1, %2" : "+r"(rt) : "r"(r1), "r" (r2))
#define ADCS(rt, r1, r2) asm volatile("adcs %0, %1, %2" : "+r"(rt) : "r"(r1), "r" (r2))

#define spint uint32_t

#ifndef NOINLINE
#define NOINLINE __attribute__((noinline))
#endif

#ifndef ALWAYS_INLINE
#define ALWAYS_INLINE __attribute__((always_inline))
#endif

static int modfsb_MFP248_ct(spint *a) {
#if 1
    const spint p0 = 4294967295;
    const spint p1 = 4294967295;
    const spint p2 = 4294967295;
    const spint p3 = 4294967295;
    const spint p4 = 4294967295;
    const spint p5 = 4294967295;
    const spint p6 = 4294967295;
    const spint p7 = 83886079;
    spint a0 = a[0], a1 = a[1], a2 = a[2], a3 = a[3], a4 = a[4], a5 = a[5], a6 = a[6], a7 = a[7];
    int r = 0;
    asm volatile(
        "subs   %[a0], %[a0], %[p0]\n"
        "sbcs   %[a1], %[a1], %[p1]\n"
        "sbcs   %[a2], %[a2], %[p2]\n"
        "sbcs   %[a3], %[a3], %[p3]\n"
        "sbcs   %[a4], %[a4], %[p4]\n"
        "sbcs   %[a5], %[a5], %[p5]\n"
        "sbcs   %[a6], %[a6], %[p6]\n"
        "sbcs   %[a7], %[a7], %[p7]\n"
        "itttt cs\n"
        "strcs %[a0], [%[a], #0]\n"
        "strcs %[a1], [%[a], #4]\n"
        "strcs %[a2], [%[a], #8]\n"
        "strcs %[a3], [%[a], #12]\n"
        "itttt cs\n"
        "strcs %[a4], [%[a], #16]\n"
        "strcs %[a5], [%[a], #20]\n"
        "strcs %[a6], [%[a], #24]\n"
        "strcs %[a7], [%[a], #28]\n"
        "adc    %[r], %[r], %[r]\n"
        : [a0] "+&r" (a0), [a1] "+&r" (a1), [a2] "+&r" (a2), [a3] "+&r" (a3), [a4] "+&r" (a4), [a5] "+&r" (a5), [a6] "+&r" (a6), [a7] "+&r" (a7), [r] "+&r" (r)
        : [a] "r" (a), [p0] "rI" (p0), [p1] "rI" (p1), [p2] "rI" (p2), [p3] "rI" (p3), [p4] "rI" (p4), [p5] "rI" (p5), [p6] "rI" (p6), [p7] "rI" (p7)
        : "cc", "memory");

    return 1 - r;
#else
    register spint a_ asm("r0") = (spint)a, p7 asm("r7");
    register spint a0, a1 asm("r1"), a2 asm("r2"), a3 asm("r3"), a4 asm("r4"), a5 asm("r5"), a6, a7 asm("r6");
    asm volatile(
        ".p2align 2\n"
        "ldr.w   %[a0], [%[a], #0]\n"
        "ldr.n   %[a1], [%[a], #4]\n"
        "ldr.n   %[a2], [%[a], #8]\n"
        "ldr.n   %[a3], [%[a], #12]\n"
        "ldr.n   %[a4], [%[a], #16]\n"
        "ldr.n   %[a5], [%[a], #20]\n"
        "ldr.n   %[a7], [%[a], #28]\n"
        "ldr.w   %[a6], [%[a], #24]\n"
        "mvn.w   %[p7], #4294967295-83886079\n"
        "subs.w  %[a0], %[a0], #-1\n"
        "sbcs.w  %[a1], %[a1], #-1\n"
        "sbcs.w  %[a2], %[a2], #-1\n"
        "sbcs.w  %[a3], %[a3], #-1\n"
        "sbcs.w  %[a4], %[a4], #-1\n"
        "sbcs.w  %[a5], %[a5], #-1\n"
        "sbcs.w  %[a6], %[a6], #-1\n"
        "sbcs.n  %[a7], %[a7], %[p7]\n"
        "itttt cs\n"
        "strcs.w %[a0], [%[a], #0]\n"
        "strcs.n %[a1], [%[a], #0]\n"
        "strcs.n %[a2], [%[a], #8]\n"
        "strcs.n %[a3], [%[a], #8]\n"
        "itttt cs\n"
        "strcs.w %[a4], [%[a], #16]\n"
        "strcs.w %[a5], [%[a], #16]\n"
        "strcs.w %[a6], [%[a], #24]\n"
        "strcs.w %[a7], [%[a], #24]\n"
        : [a0] "+&r"(a0), [a1] "+&r"(a1), [a2] "+&r"(a2), [a3] "+&r"(a3), [a4] "+&r"(a4), [a5] "+&r"(a5),
          [a6] "+&r"(a6), [a7] "+&r"(a7), [p7] "=&r"(p7)
        : [a] "r"(a_)
        : "cc", "memory");
#endif
}

ALWAYS_INLINE static void mul_MFP248_ct(const spint *p, const spint *q, spint *r) {
    spint t0, t1, t2, t3, t4, a, b0, b1, b2, b3;
    b0 = q[0];
    b1 = q[1];
    b2 = q[2];
    b3 = q[3];
    asm volatile(
        ".p2align 2\n"
        "mov.w  %[t1], #0\n"
        "mov.w  %[t2], #0\n"
        "mov.w  %[t3], #0\n"
        "ldr.w   %[a], [%[p], #0]\n"
        "umull  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #4]\n"
        "str.w  %[t0], [%[r], #0]\n"
        "mov.w  %[t0], #0\n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #8]\n"
        "str.w  %[t1], [%[r], #4]\n"
        "mov.w  %[t1], #0\n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #12]\n"
        "str.w  %[t2], [%[r], #8]\n"
        "mov.w  %[t2], #0\n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #16]\n"
        "str.w  %[t3], [%[r], #12]\n"
        "mov.w  %[t3], #0\n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #20]\n"
        "str.w  %[t4], [%[r], #16]\n"
        "mov.w  %[t4], #0\n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #24]\n"
        "str.w  %[t0], [%[r], #20]\n"
        "mov.w  %[t0], #0\n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #28]\n"
        "str.w  %[t1], [%[r], #24]\n"
        "mov.w  %[t1], #0\n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "str.w  %[t2], [%[r], #28]\n"
        "str.w  %[t3], [%[r], #32]\n"
        "str.w  %[t4], [%[r], #36]\n"
        "str.w  %[t0], [%[r], #40]\n"
        "str.w  %[t1], [%[r], #44]\n"
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
        ".p2align 2\n"
        "ldr.w   %[a], [%[p], #0]\n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #32]\n"
        "str.w  %[t0], [%[r], #16]\n"
        "mov.w  %[t0], #0\n"
        "adds.w %[t4], %[t4], %[a]\n"
        "ldr.w   %[a], [%[p], #4]\n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #36]\n"
        "str.w  %[t1], [%[r], #20]\n"
        "mov.w  %[t1], #0\n"
        "adcs.w %[t0], %[t0], %[a]\n"
        "ldr.w   %[a], [%[p], #8]\n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #40]\n"
        "str.w  %[t2], [%[r], #24]\n"
        "mov.w  %[t2], #0\n"
        "adcs.w %[t1], %[t1], %[a]\n"
        "ldr.w   %[a], [%[p], #12]\n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #44]\n"
        "str.w  %[t3], [%[r], #28]\n"
        "mov.w  %[t3], #0\n"
        "adcs.w %[t2], %[t2], %[a]\n"
        "ldr.w   %[a], [%[p], #16]\n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #20]\n"
        "str.w  %[t4], [%[r], #32]\n"
        "mov.w  %[t4], #0\n"
#ifdef __clang__
        "adcs   %[t3], %[t3], #0\n"
#else
        "adcs.w %[t3], %[t3], #0\n"
#endif
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #24]\n"
        "str.w  %[t0], [%[r], #36]\n"
        "mov.w  %[t0], #0\n"
#ifdef __clang__
        "adcs   %[t4], %[t4], #0\n"
#else
        "adcs.w %[t4], %[t4], #0\n"
#endif
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #28]\n"
        "str.w  %[t1], [%[r], #40]\n"
        "mov.w  %[t1], #0\n"
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
        "adcs   %[t1], %[t1], #0\n"
#else
        "adcs.w %[t1], %[t1], #0\n"
#endif
        "str.w  %[t2], [%[r], #44]\n"
        "str.w  %[t3], [%[r], #48]\n"
        "str.w  %[t4], [%[r], #52]\n"
        "str.w  %[t0], [%[r], #56]\n"
        "str.w  %[t1], [%[r], #60]\n"
        : [t0] "+&r"(t0), [t1] "+&r"(t1), [t2] "+&r"(t2), [t3] "+&r"(t3), [t4] "+&r"(t4), [a] "=&r"(a)
        : [b0] "r"(b0), [b1] "r"(b1), [b2] "r"(b2), [b3] "r"(b3), [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
}

ALWAYS_INLINE static void sqr_MFP248_ct(const spint *p, spint *r) {
    spint t5, t6, t7, t8, t9, t10, t11, t12, t13, t14;
    spint T, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15;

    spint t, a0, a1, a2, a3, a4;

    T = 0;

    asm volatile(
        ".p2align 2\n"
        "ldr.w %[a0], [%[a]]\n"
        "ldr.w %[a1], [%[a], #4]\n"
        "umull  %[t], %[t8],  %[a0],  %[a1]\n"
        "umull %[T0], %[T1],  %[a0],  %[a0]\n"
        "umaal %[T1], %[T],   %[two], %[t]\n"
        "ldr.w %[a2], [%[a], #8]\n"
        "str.w %[T0], [%[res]]\n"
        "ldr.w %[a3], [%[a], #12]\n"
        "str.w %[T1], [%[res], #4]\n"
        "mov.w  %[t], #0\n"
        "umaal  %[t], %[t8],  %[a0],  %[a2]\n"
        "umull %[T0], %[T1],  %[a1],  %[a1]\n"
        "umaal %[T0], %[T],   %[two], %[t]\n"
        "umull  %[t], %[t10], %[a0],  %[a3]\n"
        "umaal  %[t], %[t8],  %[a1],  %[a2]\n"
        "umaal %[T1], %[T],   %[two], %[t]\n"
        "ldr.w %[a4], [%[a], #16]\n"
        "str.w %[T0], [%[res], #8]\n"
        "str.w %[T1], [%[res], #12]\n"
        "mov.w  %[t], #0\n"
        "umaal  %[t], %[t10], %[a0],  %[a4]\n"
        "umaal  %[t], %[t8],  %[a1],  %[a3]\n"
        "umull %[T0], %[T1],  %[a2],  %[a2]\n"
        "umaal %[T0], %[T],   %[two], %[t]\n"
        "str.w %[T0], [%[res], #16]\n"
        : [a0] "=&r"(a0), [a1] "=&r"(a1), [a2] "=&r"(a2), [a3] "=&r"(a3), [a4] "=&r"(a4), [t] "=&r"(t), [t8] "=&r"(t8),
          [t10] "=&r"(t10), [T0] "=&r"(T4), [T1] "=&r"(T5), [T] "+&r"(T)
        : [a] "r"(p), [res] "r"(r), [two] "r"(2)
        : "memory");


    spint a5 = p[5];
    UMULL(t5, t12, a0, a5);
    UMAAL(t5, t10, a1, a4);
    UMAAL(t5, t8,  a2, a3);
    UMAAL(T5, T,   2,  t5);
    r[5] = T5;

    t6 = 0;
    spint a6 = p[6];
    UMAAL(t6, t12, a0, a6);
    UMAAL(t6, t10, a1, a5);
    UMAAL(t6, t8,  a2, a4);
    UMULL(T6, T7,  a3, a3);
    UMAAL(T6, T,   2,  t6);
    r[6] = T6;

    spint a7 = p[7];
    UMULL(t7, t14, a0, a7);
    UMAAL(t7, t12, a1, a6);
    UMAAL(t7, t10, a2, a5);
    UMAAL(t7, t8,  a3, a4);
    UMAAL(T7, T,   2,  t7);
    r[7] = T7;

    UMAAL(t8, t14, a1, a7);
    UMAAL(t8, t12, a2, a6);
    UMAAL(t8, t10, a3, a5);
    UMULL(T8, T9,  a4, a4);
    UMAAL(T8, T,   2,  t8);
    r[8] = T8;

    t9 = 0;
    UMAAL(t9, t14, a2, a7);
    UMAAL(t9, t12, a3, a6);
    UMAAL(t9, t10, a4, a5);
    UMAAL(T9, T,   2,  t9);
    r[9] = T9;


    UMAAL(t10, t14, a3, a7);
    UMAAL(t10, t12, a4, a6);
    UMULL(T10, T11, a5, a5);
    UMAAL(T10, T,   2,  t10);
    r[10] = T10;

    t11 = 0;
    UMAAL(t11, t14, a4, a7);
    UMAAL(t11, t12, a5, a6);
    UMAAL(T11, T,   2,  t11);
    r[11] = T11;

    UMAAL(t12, t14, a5, a7);
    UMULL(T12, T13, a6, a6);
    UMAAL(T12, T,   2,  t12);
    r[12] = T12;

    t13 = 0;
    UMAAL(t13, t14, a6, a7);
    UMAAL(T13, T,   2,  t13);
    r[13] = T13;

    UMULL(T14, T15, a7, a7);
    UMAAL(T14, T,   2,  t14);
    r[14] = T14;

    T15 += T;
    r[15] = T15;
}

static void modmul_MFP248_ct(const spint *a, const spint *b, spint *res) {
    spint A[16];
    spint A0, A1, A2, A3, A7, A8, A9, A10, A11;
    const spint p7 = 83886079;
    spint t;

    mul_MFP248_ct(a, b, A);
    asm volatile(
        ".p2align 2\n"
        "ldr.w  %[A0_4],   [%[A], #0]                 \n"
        "ldr.w  %[A1_5],   [%[A], #4]                 \n"
        "ldr.w  %[A2_6],   [%[A], #8]                 \n"
        "ldr.w  %[A3_15],  [%[A], #12]                \n"
        "ldr.w  %[A7],     [%[A], #28]                \n"
        "ldr.w  %[A8_12],  [%[A], #32]                \n"
        "ldr.w  %[A9_13],  [%[A], #36]                \n"
        "ldr.w  %[A10_14], [%[A], #40]                \n"
        "ldr.w  %[A11],    [%[A], #44]                \n"
        "mov.w  %[t],      #0                         \n"
        "umaal  %[A7],     %[t],      %[A0_4],  %[p71]\n"
        "umaal  %[A8_12],  %[t],      %[A1_5],  %[p71]\n"
        "str.w  %[A8_12],  [%[res], #0]               \n"
        "umaal  %[A9_13],  %[t],      %[A2_6],  %[p71]\n"
        "str.w  %[A9_13],  [%[res], #4]               \n"
        "umaal  %[A10_14], %[t],      %[A3_15], %[p71]\n"
        "ldr.w  %[A0_4],   [%[A], #16]                \n"
        "ldr.w  %[A1_5],   [%[A], #20]                \n"
        "ldr.w  %[A2_6],   [%[A], #24]                \n"
        "ldr.w  %[A8_12],  [%[A], #48]                \n"
        "ldr.w  %[A9_13],  [%[A], #52]                \n"
        "ldr.w  %[A3_15],  [%[A], #60]                \n"
        "str.w  %[A10_14], [%[res], #8]               \n"
        "umaal  %[A11],    %[t],      %[A0_4],  %[p71]\n"
        "ldr.w  %[A10_14], [%[A], #56]                \n"
        "str.w  %[A11],    [%[res], #12]              \n"
        "umaal  %[A8_12],  %[t],      %[A1_5],  %[p71]\n"
        "str.w  %[A8_12],  [%[res], #16]              \n"
        "umaal  %[A9_13],  %[t],      %[A2_6],  %[p71]\n"
        "str.w  %[A9_13],  [%[res], #20]              \n"
        "umaal  %[A10_14], %[t],      %[A7],    %[p71]\n"
        "str.w  %[A10_14], [%[res], #24]              \n"
        "adds.w %[A3_15],  %[A3_15],  %[t]            \n"
        "str.w  %[A3_15],  [%[res], #28]              \n"
        : [A0_4] "=&r"(A0), [A1_5] "=&r"(A1), [A2_6] "=&r"(A2), [A3_15] "=&r"(A3), [A7] "=&r"(A7), [A8_12] "=&r"(A8),
          [A9_13] "=&r"(A9), [A10_14] "=&r"(A10), [A11] "=&r"(A11), [t] "=&r"(t)
        : [A] "r"(A), [res] "r"(res), [p71] "r"(p7 + 1)
        : "cc", "memory");
}

ALWAYS_INLINE static void inner_modsqr_MFP248_ct(const spint *a, spint *res) {
    spint A[16];
    spint A0, A1, A2, A3, A7, A8, A9, A10, A11;
    const spint p7 = 83886079;
    spint t;

    sqr_MFP248_ct(a, A);

    asm volatile(
        ".p2align 2\n"
        "ldr.w  %[A0_4],   [%[A], #0]                 \n"
        "ldr.w  %[A1_5],   [%[A], #4]                 \n"
        "ldr.w  %[A2_6],   [%[A], #8]                 \n"
        "ldr.w  %[A3_15],  [%[A], #12]                \n"
        "ldr.w  %[A7],     [%[A], #28]                \n"
        "ldr.w  %[A8_12],  [%[A], #32]                \n"
        "ldr.w  %[A9_13],  [%[A], #36]                \n"
        "ldr.w  %[A10_14], [%[A], #40]                \n"
        "ldr.w  %[A11],    [%[A], #44]                \n"
        "mov.w  %[t],      #0                         \n"
        "umaal  %[A7],     %[t],      %[A0_4],  %[p71]\n"
        "umaal  %[A8_12],  %[t],      %[A1_5],  %[p71]\n"
        "str.w  %[A8_12],  [%[res], #0]               \n"
        "umaal  %[A9_13],  %[t],      %[A2_6],  %[p71]\n"
        "str.w  %[A9_13],  [%[res], #4]               \n"
        "umaal  %[A10_14], %[t],      %[A3_15], %[p71]\n"
        "ldr.w  %[A0_4],   [%[A], #16]                \n"
        "ldr.w  %[A1_5],   [%[A], #20]                \n"
        "ldr.w  %[A2_6],   [%[A], #24]                \n"
        "ldr.w  %[A8_12],  [%[A], #48]                \n"
        "ldr.w  %[A9_13],  [%[A], #52]                \n"
        "ldr.w  %[A3_15],  [%[A], #60]                \n"
        "str.w  %[A10_14], [%[res], #8]               \n"
        "umaal  %[A11],    %[t],      %[A0_4],  %[p71]\n"
        "ldr.w  %[A10_14], [%[A], #56]                \n"
        "str.w  %[A11],    [%[res], #12]              \n"
        "umaal  %[A8_12],  %[t],      %[A1_5],  %[p71]\n"
        "str.w  %[A8_12],  [%[res], #16]              \n"
        "umaal  %[A9_13],  %[t],      %[A2_6],  %[p71]\n"
        "str.w  %[A9_13],  [%[res], #20]              \n"
        "umaal  %[A10_14], %[t],      %[A7],    %[p71]\n"
        "str.w  %[A10_14], [%[res], #24]              \n"
        "adds.w %[A3_15],  %[A3_15],  %[t]            \n"
        "str.w  %[A3_15],  [%[res], #28]              \n"
        : [A0_4] "=&r"(A0), [A1_5] "=&r"(A1), [A2_6] "=&r"(A2), [A3_15] "=&r"(A3), [A7] "=&r"(A7), [A8_12] "=&r"(A8),
          [A9_13] "=&r"(A9), [A10_14] "=&r"(A10), [A11] "=&r"(A11), [t] "=&r"(t)
        : [A] "r"(A), [res] "r"(res), [p71] "r"(p7 + 1)
        : "cc", "memory");
}

static void modsqr_MFP248_ct(const spint *a, spint *res) {
    inner_modsqr_MFP248_ct(a, res);
}

static void mli_MFP248_ct(const spint *p, const spint q, spint *r) {
    spint b, t0, t1, t2, t3, t4, t5, t6, t7, t8;
    b = q - 1;
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

static void nres_MFP248_ct(const spint *a, spint *b) {
    spint nresfactor[] = {858996080, 858993459, 858993459, 858993459, 858993459, 858993459, 858993459, 53687091};
    modmul_MFP248_ct(a, nresfactor, b);
}

static void redc_MFP248_ct(const spint *a, spint *b) {
    spint rinv[] = {1, 0, 0, 0, 0, 0, 0, 0};
    modmul_MFP248_ct(a, rinv, b);
    modfsb_MFP248_ct(b);
}

static void modcmv_MFP248_ct(int d, const spint *a, volatile spint *b) {
    spint c0, c1, s, t, aux;
    spint r = 0x5aa5a55a;
    c0 = (~d) & (r + 1);
    c1 = d + r;
    for (int i = 0; i < 8; i++) {
        s = a[i];
        t = b[i];
        b[i] = aux = c0 * t + c1 * s;
        b[i] = aux - r * (t + s);
    }
}

static void modcsw_MFP248_ct(int d, volatile spint *a, volatile spint *b) {
    spint c0, c1, s, t, w, aux;
    spint r = 0x5aa5a55a;
    c0 = (~d) & (r + 1);
    c1 = d + r;
    for (int i = 0; i < 8; i++) {
        s = a[i];
        t = b[i];
        w = r * (t + s);
        b[i] = aux = c0 * t + c1 * s;
        b[i] = aux - w;
        a[i] = aux = c0 * s + c1 * t;
        a[i] = aux - w;
    }
}

inline static void modadd_MFP248_ct(const spint *a, const spint *b, spint *res) {
#if 1
    register spint a0 asm("r4"), a1 asm("r5"), a2 asm("r6"), a3 asm("r7"), a4, a5, a6, a_a7 asm("r1") = (spint)a;
    register spint b_i asm("r3"), b_p7 asm("r2") = (spint)b;
    register spint *res_ asm("r0") = res;
    asm volatile(
        ".p2align 2\n"
        "ldr.n  %[a0], [%[a_a7], #0]\n"
        "ldr.n  %[b_i], [%[b_p7], #0]\n"
        "adds.n %[a0], %[a0], %[b_i]\n"

        "ldr.n  %[a1], [%[a_a7], #4]\n"
        "ldr.n  %[b_i], [%[b_p7], #4]\n"
        "str.n  %[a0], [%[res], #0]\n"
        "adcs.n %[a1], %[a1], %[b_i]\n"

        "ldr.n  %[a2], [%[a_a7], #8]\n"
        "ldr.n  %[b_i], [%[b_p7], #8]\n"
        "str.n  %[a1], [%[res], #4]\n"
        "adcs.n %[a2], %[a2], %[b_i]\n"

        "ldr.n  %[a3], [%[a_a7], #12]\n"
        "ldr.n  %[b_i], [%[b_p7], #12]\n"
        "str.n  %[a2], [%[res], #8]\n"
        "adcs.w %[a3], %[a3], %[b_i]\n"

        ".p2align 2\n"
        "ldr.w  %[a4], [%[a_a7], #16]\n"
        "ldr.n  %[b_i], [%[b_p7], #16]\n"
        "str.n  %[a3], [%[res], #12]\n"
        "adcs.w %[a4], %[a4], %[b_i]\n"

        "ldr.w  %[a5], [%[a_a7], #20]\n"
        "ldr.w  %[b_i], [%[b_p7], #20]\n"
        "str.w  %[a4], [%[res], #16]\n"
        "adcs.w %[a5], %[a5], %[b_i]\n"

        "ldr.w  %[a6], [%[a_a7], #24]\n"
        "ldr.w  %[b_i], [%[b_p7], #24]\n"
        "str.w  %[a5], [%[res], #20]\n"
        "adcs.w %[a6], %[a6], %[b_i]\n"

        "ldr.w  %[a_a7], [%[a_a7], #28]\n"
        "ldr.w  %[b_i], [%[b_p7], #28]\n"
        "str.w  %[a6], [%[res], #24]\n"
        "adcs.w %[a_a7], %[a_a7], %[b_i]\n"

        "str.w  %[a_a7], [%[res], #28]\n"
        "mvn.w  %[b_p7], #4294967295-167772159\n"

        "subs.w %[a0], %[a0], #-2\n"
        "sbcs.w %[a1], %[a1], #-1\n"
        "sbcs.w %[a2], %[a2], #-1\n"
        "sbcs.w %[a3], %[a3], #-1\n"
        "sbcs.w %[a4], %[a4], #-1\n"
        "sbcs.w %[a5], %[a5], #-1\n"
        "sbcs.w %[a6], %[a6], #-1\n"
        "sbcs.n %[a_a7], %[a_a7], %[b_p7]\n"

        "itttt cs\n"
        "strcs.w %[a0], [%[res], #0]\n"
        "strcs.n %[a1], [%[res], #4]\n"
        "strcs.n %[a2], [%[res], #8]\n"
        "strcs.n %[a3], [%[res], #12]\n"
        "itttt cs\n"
        "strcs.w %[a4], [%[res], #16]\n"
        "strcs.w %[a5], [%[res], #20]\n"
        "strcs.w %[a6], [%[res], #24]\n"
        "strcs.n %[a_a7], [%[res], #28]\n"
        : [a0] "=&r"(a0), [a1] "=&r"(a1), [a2] "=&r"(a2), [a3] "=&r"(a3), [a4] "=&r"(a4), [a5] "=&r"(a5),
          [a6] "=&r"(a6), [a_a7] "+&r"(a_a7), [b_i] "=&r"(b_i), [b_p7] "+&r"(b_p7)
        : [res] "r"(res_)
        : "cc", "memory");
#else
    spint a0, a1, a2, a3, a4, a5, a6, a7;
    spint b0, b1, b2, b3, b4, b5, b6, b7;
    spint t = 0;
    const spint p0 = 4294967294;
    const spint p1 = 4294967295;
    const spint p2 = 4294967295;
    const spint p3 = 4294967295;
    const spint p4 = 4294967295;
    const spint p5 = 4294967295;
    const spint p6 = 4294967295;
    const spint p7 = 167772159;
    a0 = a[0];
    b0 = b[0];
    UMAAL(a0, t, 1, b0);
    res[0] = a0;
    a1 = a[1];
    b1 = b[1];
    UMAAL(a1, t, 1, b1);
    res[1] = a1;
    a2 = a[2];
    b2 = b[2];
    UMAAL(a2, t, 1, b2);
    res[2] = a2;
    a3 = a[3];
    b3 = b[3];
    UMAAL(a3, t, 1, b3);
    res[3] = a3;
    a4 = a[4];
    b4 = b[4];
    UMAAL(a4, t, 1, b4);
    res[4] = a4;
    a5 = a[5];
    b5 = b[5];
    UMAAL(a5, t, 1, b5);
    res[5] = a5;
    a6 = a[6];
    b6 = b[6];
    UMAAL(a6, t, 1, b6);
    res[6] = a6;
    a7 = a[7];
    b7 = b[7];
    UMAAL(a7, t, 1, b7);
    res[7] = a7;
    asm volatile(
        "subs %[a0], %[a0], %[p0]\n"
        "sbcs %[a1], %[a1], %[p1]\n"
        "sbcs %[a2], %[a2], %[p2]\n"
        "sbcs %[a3], %[a3], %[p3]\n"
        "sbcs %[a4], %[a4], %[p4]\n"
        "sbcs %[a5], %[a5], %[p5]\n"
        "sbcs %[a6], %[a6], %[p6]\n"
        "sbcs %[a7], %[a7], %[p7]\n"
        "itttt cs\n"
        "strcs %[a0], [%[res], #0]\n"
        "strcs %[a1], [%[res], #4]\n"
        "strcs %[a2], [%[res], #8]\n"
        "strcs %[a3], [%[res], #12]\n"
        "itttt cs\n"
        "strcs %[a4], [%[res], #16]\n"
        "strcs %[a5], [%[res], #20]\n"
        "strcs %[a6], [%[res], #24]\n"
        "strcs %[a7], [%[res], #28]\n"
        : [a0] "+&r" (a0), [a1] "+&r" (a1), [a2] "+&r" (a2), [a3] "+&r" (a3), [a4] "+&r" (a4), [a5] "+&r" (a5), [a6] "+&r" (a6), [a7] "+&r" (a7)
        : [res] "r" (res), [p0] "rI" (p0), [p1] "rI" (p1), [p2] "rI" (p2), [p3] "rI" (p3), [p4] "rI" (p4), [p5] "rI" (p5), [p6] "rI" (p6), [p7] "rI" (p7)
        : "cc", "memory");
#endif
}

ALWAYS_INLINE static void modsub_MFP248_ct(const spint *a, const spint *b, spint *res) {
    spint a0, a1, a2, a3, a4, a5, a6, a7;
    spint t0 = (spint)a, t1, t2, t3;
    const spint p0 = 4294967294;
    const spint p1 = 4294967295;
    const spint p2 = 4294967295;
    const spint p3 = 4294967295;
    const spint p4 = 4294967295;
    const spint p5 = 4294967295;
    const spint p6 = 4294967295;
    const spint p7 = 167772159;
    asm volatile(
        ".p2align 2\n"
        "ldr.w  %[a0], [%[t0]]\n"
        "ldr.w  %[a1], [%[t0], #4]\n"
        "ldr.w  %[a2], [%[t0], #8]\n"
        "ldr.w  %[a3], [%[t0], #12]\n"
        "ldr.w  %[a4], [%[t0], #16]\n"
        "ldr.w  %[a5], [%[t0], #20]\n"
        "ldr.w  %[a6], [%[t0], #24]\n"
        "ldr.w  %[a7], [%[t0], #28]\n"
        "ldr.w  %[t0], [%[b]]\n"
        "ldr.w  %[t1], [%[b], #4]\n"
        "ldr.w  %[t2], [%[b], #8]\n"
        "ldr.w  %[t3], [%[b], #12]\n"
        "subs.w %[a0], %[a0], %[t0]\n"
        "sbcs.w %[a1], %[a1], %[t1]\n"
        "sbcs.w %[a2], %[a2], %[t2]\n"
        "sbcs.w %[a3], %[a3], %[t3]\n"
        "ldr.w  %[t0], [%[b], #16]\n"
        "ldr.w  %[t1], [%[b], #20]\n"
        "ldr.w  %[t2], [%[b], #24]\n"
        "ldr.w  %[t3], [%[b], #28]\n"
        "sbcs.w %[a4], %[a4], %[t0]\n"
        "sbcs.w %[a5], %[a5], %[t1]\n"
        "sbcs.w %[a6], %[a6], %[t2]\n"
        "sbcs.w %[a7], %[a7], %[t3]\n"
        "sbcs.w %[t0], %[t0], %[t0]\n"
        : [a0] "=&r" (a0), [a1] "=&r" (a1), [a2] "=&r" (a2), [a3] "=&r" (a3), [a4] "=&r" (a4), [a5] "=&r" (a5), [a6] "=&r" (a6), [a7] "=&r" (a7), [t0] "+&r" (t0), [t1] "=&r" (t1), [t2] "=&r" (t2), [t3] "=&r" (t3)
        : [b] "r" (b)
        : "cc", "memory");
    t0 = -t0;
    t1 = 0;
    UMAAL(a0, t1, t0, p0);
    res[0] = a0;
    UMAAL(a1, t1, t0, p1);
    res[1] = a1;
    UMAAL(a2, t1, t0, p2);
    res[2] = a2;
    UMAAL(a3, t1, t0, p3);
    res[3] = a3;
    UMAAL(a4, t1, t0, p4);
    res[4] = a4;
    UMAAL(a5, t1, t0, p5);
    res[5] = a5;
    UMAAL(a6, t1, t0, p6);
    res[6] = a6;
    UMAAL(a7, t1, t0, p7);
    res[7] = a7;
}

static void modneg_MFP248_ct(const spint *a, spint *r) {
    spint zero[8] = {0};
    modsub_MFP248_ct(zero, a, r);
}

static void modshl_MFP248_ct(int n, spint *a) {
#pragma GCC unroll 7
    for (int i = 7; i > 0; i--) {
        a[i] = (a[i] << n) + (a[i - 1] >> (32 - n));
    }
    a[0] = (a[0] << n);
}

// Only works for n <= 31
static spint modshr_MFP248_ct(int n, spint *a) {
    spint r = a[0] & ((1 << n) - 1);
#pragma GCC unroll 7
    for (int i = 0; i < 7; i++) {
        a[i] = (a[i] >> n) | (a[i + 1] << (32 - n));
    }
    a[7] = a[7] >> n;
    return r;
}

static void modcpy_MFP248_ct(const spint *a, spint *b) {
#pragma GCC unroll 8
    for (int i = 0; i < 8; i++) {
        b[i] = a[i];
    }
}

static int modis0_MFP248_ct(const spint *a) {
    spint c[8] = {0};
    spint d = 0;
    modcpy_MFP248_ct(a, c);
    modfsb_MFP248_ct(c);
    for (int i = 0; i < 8; i++) {
        d |= c[i];
    }
    return 1 - ((d | -d) >> 31);
}

static int modis1_MFP248_ct(const spint *a) {
    spint c[8] = {0};
    spint d = 0;
    redc_MFP248_ct(a, c);
    c[0] = c[0] - 1;
    for (int i = 0; i < 8; i++) {
        d |= c[i];
    }
    return 1 - ((d | -d) >> 31);
}

static int modcmp_MFP248_ct(const spint *a, const spint *b) {
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

static int modsign_MFP248_ct(const spint *a) {
    spint c[8];
    redc_MFP248_ct(a, c);
    return c[0] % 2;
}

NOINLINE static void modnsqr_MFP248_ct(spint *a, int n) {
    for (int i = 0; i < n; i++) {
        inner_modsqr_MFP248_ct(a, a);
    }
}

// Calculate progenitor
NOINLINE static void modpro_MFP248_ct(const spint *w, spint *z) {
    spint x[8];
    spint t0[8];
    spint t1[8];
    spint t2[8];
    spint t3[8];
    spint t4[8];
    modcpy_MFP248_ct(w, x);
    modsqr_MFP248_ct(x, z);
    modmul_MFP248_ct(x, z, t0);
    modsqr_MFP248_ct(t0, z);
    modmul_MFP248_ct(x, z, z);
    modsqr_MFP248_ct(z, t1);
    modsqr_MFP248_ct(t1, t3);
    modsqr_MFP248_ct(t3, t2);
    modcpy_MFP248_ct(t2, t4);
    modnsqr_MFP248_ct(t4, 3);
    modmul_MFP248_ct(t2, t4, t2);
    modcpy_MFP248_ct(t2, t4);
    modnsqr_MFP248_ct(t4, 6);
    modmul_MFP248_ct(t2, t4, t2);
    modcpy_MFP248_ct(t2, t4);
    modnsqr_MFP248_ct(t4, 2);
    modmul_MFP248_ct(t3, t4, t3);
    modnsqr_MFP248_ct(t3, 13);
    modmul_MFP248_ct(t2, t3, t2);
    modcpy_MFP248_ct(t2, t3);
    modnsqr_MFP248_ct(t3, 27);
    modmul_MFP248_ct(t2, t3, t2);
    modmul_MFP248_ct(z, t2, z);
    modcpy_MFP248_ct(z, t2);
    modnsqr_MFP248_ct(t2, 4);
    modmul_MFP248_ct(t1, t2, t1);
    modmul_MFP248_ct(t0, t1, t0);
    modmul_MFP248_ct(t1, t0, t1);
    modmul_MFP248_ct(t0, t1, t0);
    modmul_MFP248_ct(t1, t0, t2);
    modmul_MFP248_ct(t0, t2, t0);
    modmul_MFP248_ct(t1, t0, t1);
    modnsqr_MFP248_ct(t1, 63);
    modmul_MFP248_ct(t0, t1, t1);
    modnsqr_MFP248_ct(t1, 64);
    modmul_MFP248_ct(t0, t1, t0);
    modnsqr_MFP248_ct(t0, 57);
    modmul_MFP248_ct(z, t0, z);
}

// Test for quadratic residue, provide progenitor h if available, NULL if not
static int modqr_MFP248_ct(const spint *h, const spint *x) {
    spint r[8];
    if (h == NULL) {
        modpro_MFP248_ct(x, r);
        modsqr_MFP248_ct(r, r);
    }
    else {
        modsqr_MFP248_ct(h, r);
    }
    modmul_MFP248_ct(r, x, r);
    return modis1_MFP248_ct(r);
}

// Modular square root, provide progenitor h if available, NULL if not
static void modsqrt_MFP248_ct(const spint *x, const spint *h, spint *r) {
    spint s[8];
    spint y[8];
    if (h == NULL) {
        modpro_MFP248_ct(x, y);
    }
    else {
        modcpy_MFP248_ct(h, y);
    }
    modmul_MFP248_ct(y, x, s);
    modcpy_MFP248_ct(s, r);
}

// Calculate inverse, provide progenitor h if available
static void modinv_MFP248_ct(const spint *x, const spint *h, spint *z) {
    spint s[8];
    spint t[8];
    if (h == NULL) {
        modpro_MFP248_ct(x, t);
    }
    else {
        modcpy_MFP248_ct(h, t);
    }
    modcpy_MFP248_ct(x, s);
    modnsqr_MFP248_ct(t, 2);
    modmul_MFP248_ct(s, t, z);
}

static void modzer_MFP248_ct(spint *a) {
#pragma GCC unroll 8
    for (int i = 0; i < 8; i++) {
        a[i] = 0;
    }
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
    a[0] = (spint)x;
#pragma GCC unroll 7
    for (int i = 1; i < 8; i++) {
        a[i] = 0;
    }
    nres_MFP248_ct(a, a);
}

static void mod2r_MFP248_ct(unsigned int r, spint *a) {
    unsigned int n = r >> 5;
    unsigned int m = r & 31;
    modzer_MFP248_ct(a);
    if (r >= 256) {
        return;
    }
    a[n] = 1 << m;
    nres_MFP248_ct(a, a);
}

static void modmli_MFP248_ct(const spint *p, const spint q, spint *r) {
    // TODO: replace this by an optimized version
    spint t[8];
    modint_MFP248_ct(q, t);
    modmul_MFP248_ct(p, t, r);
}

static void modimp_MFP248_ct(const char *b, spint *a) {
    const spint *b32 = (const spint *)b;
#pragma GCC unroll 8
    for (int i = 0; i < 8; i++) {
        a[i] = __builtin_bswap32(b32[7 - i]);
    }
    nres_MFP248_ct(a, a);
}

// Code for export, not exponentiation
static void modexp_MFP248_ct(const spint *a, char *b) {
    spint c[8], *b32 = (spint *)b;
    redc_MFP248_ct(a, c);
#pragma GCC unroll 8
    for (int i = 0; i < 8; i++) {
        b32[7 - i] = __builtin_bswap32(c[i]);
    }
}

#define Nlimbs_MFP248_ct 8
#define Nbits_MFP248_ct  251
#define Nbytes_MFP248_ct 32
