#if defined(PQM4)

#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#include <stdbool.h>
#include <fp.h>
#include <fp2.h>

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

static int modfsb_MFP1973_ct(spint *a) {
    const spint p0 = 4294967295;
    const spint p1 = 4294967295;
    const spint p2 = 894519295;
    const spint p3 = 623681097;
    const spint p4 = 2269148215;
    const spint p5 = 866560101;
    const spint p6 = 1804982668;
    const spint p7 = 887266856;
    spint t0, t1;
    spint tmpvec[8];
    int r = 0;
    asm volatile(
        "ldr    %[t0], [%[a], #0]\n"
        "subs   %[t0], %[t0], %[p0]\n"
        "ldr    %[t1], [%[a], #4]\n"
        "str    %[t0], [%[tmpvec], #0]\n"
        "sbcs   %[t1], %[t1], %[p1]\n"
        "ldr    %[t0], [%[a], #8]\n"
        "str    %[t1], [%[tmpvec], #4]\n"
        "sbcs   %[t0], %[t0], %[p2]\n"
        "ldr    %[t1], [%[a], #12]\n"
        "str    %[t0], [%[tmpvec], #8]\n"
        "sbcs   %[t1], %[t1], %[p3]\n"
        "ldr    %[t0], [%[a], #16]\n"
        "str    %[t1], [%[tmpvec], #12]\n"
        "sbcs   %[t0], %[t0], %[p4]\n"
        "ldr    %[t1], [%[a], #20]\n"
        "str    %[t0], [%[tmpvec], #16]\n"
        "sbcs   %[t1], %[t1], %[p5]\n"
        "ldr    %[t0], [%[a], #24]\n"
        "str    %[t1], [%[tmpvec], #20]\n"
        "sbcs   %[t0], %[t0], %[p6]\n"
        "ldr    %[t1], [%[a], #28]\n"
        "str    %[t0], [%[tmpvec], #24]\n"
        "sbcs   %[t1], %[t1], %[p7]\n"
        "str    %[t1], [%[tmpvec], #28]\n"
        "itttt cs\n"
        "ldrcs %[t0], [%[tmpvec], #0]\n"
        "ldrcs %[t1], [%[tmpvec], #4]\n"
        "strcs %[t0], [%[a], #0]\n"
        "strcs %[t1], [%[a], #4]\n"
        "itttt cs\n"
        "ldrcs %[t0], [%[tmpvec], #8]\n"
        "ldrcs %[t1], [%[tmpvec], #12]\n"
        "strcs %[t0], [%[a], #8]\n"
        "strcs %[t1], [%[a], #12]\n"
        "itttt cs\n"
        "ldrcs %[t0], [%[tmpvec], #16]\n"
        "ldrcs %[t1], [%[tmpvec], #20]\n"
        "strcs %[t0], [%[a], #16]\n"
        "strcs %[t1], [%[a], #20]\n"
        "itttt cs\n"
        "ldrcs %[t0], [%[tmpvec], #24]\n"
        "ldrcs %[t1], [%[tmpvec], #28]\n"
        "strcs %[t0], [%[a], #24]\n"
        "strcs %[t1], [%[a], #28]\n"
        "adc    %[r], %[r], %[r]\n"
        : [t0] "=&r" (t0), [t1] "=&r" (t1), [r] "+&r" (r)
        : [a] "r" (a), [p0] "rI" (p0), [p1] "rI" (p1), [p2] "rI" (p2), [p3] "rI" (p3), [p4] "rI" (p4), [p5] "rI" (p5), [p6] "rI" (p6), [p7] "rI" (p7), [tmpvec] "r" (tmpvec)
        : "cc", "memory");

    return 1 - r;
}

ALWAYS_INLINE static void mul_MFP1973_ct(const spint *p, const spint *q, spint *r) {
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

ALWAYS_INLINE static void sqr_MFP1973_ct(const spint *p, spint *r) {
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

ALWAYS_INLINE static void inner_modsqr_MFP1973_ct(const spint *a, spint *res) {
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

static void modsqr_MFP1973_ct(const spint *a, spint *res) {
    inner_modsqr_MFP1973_ct(a, res);
}

static void nres_MFP1973_ct(const spint *a, spint *b) {
    spint nresfactor[] = {1074164948, 590751150, 39459886, 548394689, 153118045, 816333227, 2083573053, 225634262};
    modmul_MFP1973_ct(a, nresfactor, b);
}

static void redc_MFP1973_ct(const spint *a, spint *b) {
    spint rinv[] = {1, 0, 0, 0, 0, 0, 0, 0};
    modmul_MFP1973_ct(a, rinv, b);
    modfsb_MFP1973_ct(b);
}

static void modcmv_MFP1973_ct(int d, const spint *a, volatile spint *b) {
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

static void modcsw_MFP1973_ct(int d, volatile spint *a, volatile spint *b) {
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

inline static void modadd_MFP1973_ct(const spint *a, const spint *b, spint *res) {
    const spint p[8] = {4294967294, 4294967295, 1789038591, 1247362194, 243329134, 1733120203, 3609965336, 1774533712};
    spint tmpres[8];
    spint t = 0, pp, tt, aa, bb;

    asm volatile(
        "ldr    %[aa], [%[a]]\n"
        "ldr    %[bb], [%[b]]\n"
        "ldr    %[pp], [%[p]]\n"
        "umaal  %[aa], %[t], %[one], %[bb]\n"
        "str    %[aa], [%[res]]\n"
        "subs   %[tt], %[aa], %[pp]\n"
        "ldr    %[aa], [%[a], #4]\n"
        "ldr    %[bb], [%[b], #4]\n"
        "ldr    %[pp], [%[p], #4]\n"
        "str    %[tt], [%[tmpres], #0]\n"
        "umaal  %[aa], %[t], %[one], %[bb]\n"
        "str    %[aa], [%[res], #4]\n"
        "sbcs   %[tt], %[aa], %[pp]\n"
        "ldr    %[aa], [%[a], #8]\n"
        "ldr    %[bb], [%[b], #8]\n"
        "ldr    %[pp], [%[p], #8]\n"
        "str    %[tt], [%[tmpres], #4]\n"
        "umaal  %[aa], %[t], %[one], %[bb]\n"
        "str    %[aa], [%[res], #8]\n"
        "sbcs   %[tt], %[aa], %[pp]\n"
        "ldr    %[aa], [%[a], #12]\n"
        "ldr    %[bb], [%[b], #12]\n"
        "ldr    %[pp], [%[p], #12]\n"
        "str    %[tt], [%[tmpres], #8]\n"
        "umaal  %[aa], %[t], %[one], %[bb]\n"
        "str    %[aa], [%[res], #12]\n"
        "sbcs   %[tt], %[aa], %[pp]\n"
        "ldr    %[aa], [%[a], #16]\n"
        "ldr    %[bb], [%[b], #16]\n"
        "ldr    %[pp], [%[p], #16]\n"
        "str    %[tt], [%[tmpres], #12]\n"
        "umaal  %[aa], %[t], %[one], %[bb]\n"
        "str    %[aa], [%[res], #16]\n"
        "sbcs   %[tt], %[aa], %[pp]\n"
        "ldr    %[aa], [%[a], #20]\n"
        "ldr    %[bb], [%[b], #20]\n"
        "ldr    %[pp], [%[p], #20]\n"
        "str    %[tt], [%[tmpres], #16]\n"
        "umaal  %[aa], %[t], %[one], %[bb]\n"
        "str    %[aa], [%[res], #20]\n"
        "sbcs   %[tt], %[aa], %[pp]\n"
        "ldr    %[aa], [%[a], #24]\n"
        "ldr    %[bb], [%[b], #24]\n"
        "ldr    %[pp], [%[p], #24]\n"
        "str    %[tt], [%[tmpres], #20]\n"
        "umaal  %[aa], %[t], %[one], %[bb]\n"
        "str    %[aa], [%[res], #24]\n"
        "sbcs   %[tt], %[aa], %[pp]\n"
        "ldr    %[aa], [%[a], #28]\n"
        "ldr    %[bb], [%[b], #28]\n"
        "ldr    %[pp], [%[p], #28]\n"
        "str    %[tt], [%[tmpres], #24]\n"
        "umaal  %[aa], %[t], %[one], %[bb]\n"
        "str    %[aa], [%[res], #28]\n"
        "sbcs   %[tt], %[aa], %[pp]\n"
        "str    %[tt], [%[tmpres], #28]\n"
        "itttt cs\n"
        "ldrcs %[t], [%[tmpres], #0]\n"
        "ldrcs %[tt], [%[tmpres], #4]\n"
        "strcs %[t], [%[res], #0]\n"
        "strcs %[tt], [%[res], #4]\n"
        "itttt cs\n"
        "ldrcs %[t], [%[tmpres], #8]\n"
        "ldrcs %[tt], [%[tmpres], #12]\n"
        "strcs %[t], [%[res], #8]\n"
        "strcs %[tt], [%[res], #12]\n"
        "itttt cs\n"
        "ldrcs %[t], [%[tmpres], #16]\n"
        "ldrcs %[tt], [%[tmpres], #20]\n"
        "strcs %[t], [%[res], #16]\n"
        "strcs %[tt], [%[res], #20]\n"
        "itttt cs\n"
        "ldrcs %[t], [%[tmpres], #24]\n"
        "ldrcs %[tt], [%[tmpres], #28]\n"
        "strcs %[t], [%[res], #24]\n"
        "strcs %[tt], [%[res], #28]\n"
        : [t] "+&r" (t), [tt] "=&r" (tt), [aa] "=&r" (aa), [bb] "=&r" (bb), [pp] "=&r" (pp)
        : [res] "r" (res), [tmpres] "r" (tmpres), [a] "r" (a), [b] "r" (b), [p] "r" (p), [one] "r" (1)
        : "cc", "memory");
}

inline static void modsub_MFP1973_ct(const spint *a, const spint *b, spint *res) {
    spint ae, ao;
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
        "ldr    %[t3], [%[a], #0]\n"
        "ldr    %[t4], [%[b], #0]\n"
        "subs   %[t3], %[t3], %[t4]\n"
        "ldr    %[t1], [%[a], #4]\n"
        "ldr    %[t4], [%[b], #4]\n"
        "str    %[t3], [%[res], #0]\n"
        "sbcs   %[t1], %[t1], %[t4]\n"
        "ldr    %[t3], [%[a], #8]\n"
        "ldr    %[t4], [%[b], #8]\n"
        "str    %[t1], [%[res], #4]\n"
        "sbcs   %[t3], %[t3], %[t4]\n"
        "ldr    %[t1], [%[a], #12]\n"
        "ldr    %[t4], [%[b], #12]\n"
        "str    %[t3], [%[res], #8]\n"
        "sbcs   %[t1], %[t1], %[t4]\n"
        "ldr    %[t3], [%[a], #16]\n"
        "ldr    %[t4], [%[b], #16]\n"
        "str    %[t1], [%[res], #12]\n"
        "sbcs   %[t3], %[t3], %[t4]\n"
        "ldr    %[t1], [%[a], #20]\n"
        "ldr    %[t4], [%[b], #20]\n"
        "str    %[t3], [%[res], #16]\n"
        "sbcs   %[t1], %[t1], %[t4]\n"
        "ldr    %[t3], [%[a], #24]\n"
        "ldr    %[t4], [%[b], #24]\n"
        "str    %[t1], [%[res], #20]\n"
        "sbcs   %[t3], %[t3], %[t4]\n"
        "ldr    %[t1], [%[a], #28]\n"
        "ldr    %[t4], [%[b], #28]\n"
        "str    %[t3], [%[res], #24]\n"
        "sbcs   %[t1], %[t1], %[t4]\n"
        "str    %[t1], [%[res], #28]\n"
        "sbcs    %[t], %[t3], %[t3]\n"
        : [t] "=&r" (t), [t1] "=&r" (t1), [t2] "=&r" (t2), [t3] "=&r" (t3), [t4] "=&r" (t4)
        : [b] "r" (b), [a] "r" (a), [res] "r" (res)
        : "cc", "memory");
    t = -t | (t1 > p7);
    t2 = 0;
    ae = res[0];
    UMAAL(ae, t2, t, p0);
    ao = res[1];
    res[0] = ae;
    UMAAL(ao, t2, t, p1);
    ae = res[2];
    res[1] = ao;
    UMAAL(ae, t2, t, p2);
    ao = res[3];
    res[2] = ae;
    UMAAL(ao, t2, t, p3);
    ae = res[4];
    res[3] = ao;
    UMAAL(ae, t2, t, p4);
    ao = res[5];
    res[4] = ae;
    UMAAL(ao, t2, t, p5);
    ae = res[6];
    res[5] = ao;
    UMAAL(ae, t2, t, p6);
    ao = res[7];
    res[6] = ae;
    UMAAL(ao, t2, t, p7);
    res[7] = ao;
}

static void modneg_MFP1973_ct(const spint *a, spint *r) {
    spint zero[8] = {0};
    modsub_MFP1973_ct(zero, a, r);
}

static void modshl_MFP1973_ct(int n, spint *a) {
#pragma GCC unroll 7
    for (int i = 7; i > 0; i--) {
        a[i] = (a[i] << n) + (a[i - 1] >> (32 - n));
    }
    a[0] = (a[0] << n);
}

// Only works for n <= 31
static spint modshr_MFP1973_ct(int n, spint *a) {
    spint r = a[0] & ((1 << n) - 1);
#pragma GCC unroll 7
    for (int i = 0; i < 7; i++) {
        a[i] = (a[i] >> n) | (a[i + 1] << (32 - n));
    }
    a[7] = a[7] >> n;
    return r;
}

static void modcpy_MFP1973_ct(const spint *a, spint *b) {
#pragma GCC unroll 8
    for (int i = 0; i < 8; i++) {
        b[i] = a[i];
    }
}

static int modis0_MFP1973_ct(const spint *a) {
    spint c[8] = {0};
    spint d = 0;
    modcpy_MFP1973_ct(a, c);
    modfsb_MFP1973_ct(c);
    for (int i = 0; i < 8; i++) {
        d |= c[i];
    }
    return 1 - ((d | -d) >> 31);
}

static int modis1_MFP1973_ct(const spint *a) {
    spint c[8] = {0};
    spint d = 0;
    redc_MFP1973_ct(a, c);
    c[0] = c[0] - 1;
    for (int i = 0; i < 8; i++) {
        d |= c[i];
    }
    return 1 - ((d | -d) >> 31);
}

static int modcmp_MFP1973_ct(const spint *a, const spint *b) {
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

static int modsign_MFP1973_ct(const spint *a) {
    spint c[8];
    redc_MFP1973_ct(a, c);
    return c[0] % 2;
}

NOINLINE static void modnsqr_MFP1973_ct(spint *a, int n) {
    for (int i = 0; i < n; i++) {
        inner_modsqr_MFP1973_ct(a, a);
    }
}

// Calculate progenitor
NOINLINE static void modpro_MFP1973_ct(const spint *w, spint *z) {
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
    modcpy_MFP1973_ct(w, x);
    modsqr_MFP1973_ct(x, t13);
    modsqr_MFP1973_ct(t13, t11);
    modmul_MFP1973_ct(x, t11, t0);
    modsqr_MFP1973_ct(t11, t1);
    modmul_MFP1973_ct(t11, t1, t4);
    modmul_MFP1973_ct(t13, t4, t9);
    modmul_MFP1973_ct(t11, t9, t3);
    modsqr_MFP1973_ct(t4, z);
    modmul_MFP1973_ct(t13, z, t2);
    modmul_MFP1973_ct(t0, z, t7);
    modmul_MFP1973_ct(x, t7, t16);
    modmul_MFP1973_ct(t1, t7, t5);
    modmul_MFP1973_ct(t3, t5, t6);
    modmul_MFP1973_ct(t4, t6, t15);
    modmul_MFP1973_ct(t13, t15, t17);
    modmul_MFP1973_ct(t1, t15, t8);
    modmul_MFP1973_ct(t13, t8, t10);
    modmul_MFP1973_ct(t1, t10, t1);
    modmul_MFP1973_ct(t11, t1, t4);
    modmul_MFP1973_ct(t9, t1, t12);
    modmul_MFP1973_ct(t11, t12, t11);
    modmul_MFP1973_ct(t3, t11, t3);
    modmul_MFP1973_ct(t2, t3, t2);
    modmul_MFP1973_ct(t9, t2, t9);
    modmul_MFP1973_ct(z, t9, t14);
    modmul_MFP1973_ct(t13, t14, t13);
    modmul_MFP1973_ct(z, t13, z);
    modmul_MFP1973_ct(t16, z, t16);
    modsqr_MFP1973_ct(z, t18);
    modmul_MFP1973_ct(t4, t18, z);
    modnsqr_MFP1973_ct(t18, 6);
    modmul_MFP1973_ct(t17, t18, t17);
    modnsqr_MFP1973_ct(t17, 10);
    modmul_MFP1973_ct(t16, t17, t16);
    modnsqr_MFP1973_ct(t16, 8);
    modmul_MFP1973_ct(t15, t16, t15);
    modnsqr_MFP1973_ct(t15, 9);
    modmul_MFP1973_ct(t14, t15, t14);
    modnsqr_MFP1973_ct(t14, 9);
    modmul_MFP1973_ct(t13, t14, t13);
    modnsqr_MFP1973_ct(t13, 9);
    modmul_MFP1973_ct(t12, t13, t12);
    modnsqr_MFP1973_ct(t12, 11);
    modmul_MFP1973_ct(t11, t12, t11);
    modnsqr_MFP1973_ct(t11, 8);
    modmul_MFP1973_ct(t10, t11, t10);
    modnsqr_MFP1973_ct(t10, 9);
    modmul_MFP1973_ct(t9, t10, t9);
    modnsqr_MFP1973_ct(t9, 7);
    modmul_MFP1973_ct(t8, t9, t8);
    modnsqr_MFP1973_ct(t8, 9);
    modmul_MFP1973_ct(t7, t8, t8);
    modnsqr_MFP1973_ct(t8, 12);
    modmul_MFP1973_ct(t7, t8, t7);
    modnsqr_MFP1973_ct(t7, 10);
    modmul_MFP1973_ct(t6, t7, t6);
    modnsqr_MFP1973_ct(t6, 8);
    modmul_MFP1973_ct(t5, t6, t5);
    modnsqr_MFP1973_ct(t5, 9);
    modmul_MFP1973_ct(t4, t5, t4);
    modnsqr_MFP1973_ct(t4, 9);
    modmul_MFP1973_ct(t3, t4, t3);
    modnsqr_MFP1973_ct(t3, 10);
    modmul_MFP1973_ct(t2, t3, t2);
    modnsqr_MFP1973_ct(t2, 8);
    modmul_MFP1973_ct(t1, t2, t1);
    modnsqr_MFP1973_ct(t1, 6);
    modmul_MFP1973_ct(t0, t1, t0);
    modnsqr_MFP1973_ct(t0, 12);
    modmul_MFP1973_ct(z, t0, t0);
    modnsqr_MFP1973_ct(t0, 9);
    modmul_MFP1973_ct(z, t0, t0);
    modnsqr_MFP1973_ct(t0, 9);
    modmul_MFP1973_ct(z, t0, t0);
    modnsqr_MFP1973_ct(t0, 9);
    modmul_MFP1973_ct(z, t0, t0);
    modnsqr_MFP1973_ct(t0, 9);
    modmul_MFP1973_ct(z, t0, t0);
    modnsqr_MFP1973_ct(t0, 9);
    modmul_MFP1973_ct(z, t0, t0);
    modnsqr_MFP1973_ct(t0, 9);
    modmul_MFP1973_ct(z, t0, t0);
    modsqr_MFP1973_ct(t0, t0);
    modmul_MFP1973_ct(x, t0, t0);
    modnsqr_MFP1973_ct(t0, 9);
    modmul_MFP1973_ct(z, t0, z);
}

// Test for quadratic residue, provide progenitor h if available, NULL if not
static int modqr_MFP1973_ct(const spint *h, const spint *x) {
    spint r[8];
    if (h == NULL) {
        modpro_MFP1973_ct(x, r);
        modsqr_MFP1973_ct(r, r);
    }
    else {
        modsqr_MFP1973_ct(h, r);
    }
    modmul_MFP1973_ct(r, x, r);
    return modis1_MFP1973_ct(r);
}

// Modular square root, provide progenitor h if available, NULL if not
static void modsqrt_MFP1973_ct(const spint *x, const spint *h, spint *r) {
    spint s[8];
    spint y[8];
    if (h == NULL) {
        modpro_MFP1973_ct(x, y);
    }
    else {
        modcpy_MFP1973_ct(h, y);
    }
    modmul_MFP1973_ct(y, x, s);
    modcpy_MFP1973_ct(s, r);
}

// Calculate inverse, provide progenitor h if available
static void modinv_MFP1973_ct(const spint *x, const spint *h, spint *z) {
    spint s[8];
    spint t[8];
    if (h == NULL) {
        modpro_MFP1973_ct(x, t);
    }
    else {
        modcpy_MFP1973_ct(h, t);
    }
    modcpy_MFP1973_ct(x, s);
    modnsqr_MFP1973_ct(t, 2);
    modmul_MFP1973_ct(s, t, z);
}

static void modzer_MFP1973_ct(spint *a) {
#pragma GCC unroll 8
    for (int i = 0; i < 8; i++) {
        a[i] = 0;
    }
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
    a[0] = (spint)x;
#pragma GCC unroll 7
    for (int i = 1; i < 8; i++) {
        a[i] = 0;
    }
    nres_MFP1973_ct(a, a);
}

static void mod2r_MFP1973_ct(unsigned int r, spint *a) {
    unsigned int n = r >> 5;
    unsigned int m = r & 31;
    modzer_MFP1973_ct(a);
    if (r >= 256) {
        return;
    }
    a[n] = 1 << m;
    nres_MFP1973_ct(a, a);
}

static void modmli_MFP1973_ct(const spint *p, const spint q, spint *r) {
    spint t[8];
    modint_MFP1973_ct(q, t);
    modmul_MFP1973_ct(p, t, r);
}

static void modimp_MFP1973_ct(const char *b, spint *a) {
    const spint *b32 = (const spint *)b;
#pragma GCC unroll 8
    for (int i = 0; i < 8; i++) {
        a[i] = __builtin_bswap32(b32[7 - i]);
    }
    nres_MFP1973_ct(a, a);
}

// Code for export, not exponentiation
static void modexp_MFP1973_ct(const spint *a, char *b) {
    spint c[8], *b32 = (spint *)b;
    redc_MFP1973_ct(a, c);
#pragma GCC unroll 8
    for (int i = 0; i < 8; i++) {
        b32[7 - i] = __builtin_bswap32(c[i]);
    }
}

#define Nlimbs_MFP1973_ct 8
#define Nbits_MFP1973_ct  254
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

void fp_set(digit_t* x, const digit_t val)
{ // Set field element x = val, where val has wordsize

    x[0] = val;
    for (unsigned int i = 1; i < NWORDS_FIELD; i++) {
        x[i] = 0;
    }
}

void MUL(digit_t* out, const digit_t a, const digit_t b)
{ // Digit multiplication, digit*digit -> 2-digit result
  // Inputs: a, b in [0, 2^w-1], where w is the computer wordsize
  // Output: 0 < out < 2^(2w)-1
    register digit_t al, ah, bl, bh, temp;
    digit_t albl, albh, ahbl, ahbh, res1, res2, res3, carry;
    digit_t mask_low = (digit_t)(-1) >> (sizeof(digit_t)*4), mask_high = (digit_t)(-1) << (sizeof(digit_t)*4);

    al = a & mask_low;                        // Low part
    ah = a >> (sizeof(digit_t)*4);            // High part
    bl = b & mask_low;
    bh = b >> (sizeof(digit_t)*4);

    albl = al * bl;
    albh = al * bh;
    ahbl = ah * bl;
    ahbh = ah * bh;
    out[0] = albl & mask_low;                 // out00

    res1 = albl >> (sizeof(digit_t)*4);
    res2 = ahbl & mask_low;
    res3 = albh & mask_low;
    temp = res1 + res2 + res3;
    carry = temp >> (sizeof(digit_t)*4);
    out[0] ^= temp << (sizeof(digit_t)*4);    // out01

    res1 = ahbl >> (sizeof(digit_t)*4);
    res2 = albh >> (sizeof(digit_t)*4);
    res3 = ahbh & mask_low;
    temp = res1 + res2 + res3 + carry;
    out[1] = temp & mask_low;                 // out10
    carry = temp & mask_high;
    out[1] ^= (ahbh & mask_high) + carry;     // out11
}

digit_t mp_shiftr(digit_t* x, const unsigned int shift, const unsigned int nwords)
{ // Multiprecision right shift
    digit_t bit_out = x[0] & 1;

    for (unsigned int i = 0; i < nwords-1; i++) {
        SHIFTR(x[i+1], x[i], shift, x[i], RADIX);
    }
    x[nwords-1] >>= shift;
    return bit_out;
}

void mp_shiftl(digit_t* x, const unsigned int shift, const unsigned int nwords)
{ // Multiprecision left shift

    assert(shift < RADIX*nwords);

    int shift_words = shift / RADIX;

    for (int i = nwords-1; i > shift_words; i--) {
        SHIFTL(x[i-shift_words], x[i-shift_words-1], shift % RADIX, x[i], RADIX);
    }

    x[shift_words] = x[0] << (shift % RADIX);

    for (int i = shift_words - 1; i >= 0; i--) {
        x[i] = 0;
    }
}

/* Arithmetic modulo X^2 + 1 */

void fp2_set(fp2_t* x, const digit_t val)
{
    fp_set(x->re, val);
    fp_set(x->im, 0);
}

bool fp2_is_zero(const fp2_t* a)
{ // Is a GF(p^2) element zero?
  // Returns 1 (true) if a=0, 0 (false) otherwise

    return fp_is_zero(a->re) & fp_is_zero(a->im);
}

bool fp2_is_equal(const fp2_t* a, const fp2_t* b)
{ // Compare two GF(p^2) elements in constant time
  // Returns 1 (true) if a=b, 0 (false) otherwise

    return fp_is_equal(a->re, b->re) & fp_is_equal(a->im, b->im);
}

void fp2_copy(fp2_t* x, const fp2_t* y)
{
    fp_copy(x->re, y->re);
    fp_copy(x->im, y->im);
}

fp2_t fp2_non_residue()
{ // 2 + i is a quadratic non-residue for p1913
    fp_t one = {0};
    fp2_t res;

    one[0] = 1;
    fp_tomont(one, one);
    fp_add(res.re, one, one);
    fp_copy(res.im, one);
    return res;
}

void fp2_add(fp2_t* x, const fp2_t* y, const fp2_t* z)
{
    fp_add(x->re, y->re, z->re);
    fp_add(x->im, y->im, z->im);
}

void fp2_sub(fp2_t* x, const fp2_t* y, const fp2_t* z)
{
    fp_sub(x->re, y->re, z->re);
    fp_sub(x->im, y->im, z->im);
}

void fp2_neg(fp2_t* x, const fp2_t* y)
{
    fp_neg(x->re, y->re);
    fp_neg(x->im, y->im);
}

void fp2_mul(fp2_t* x, const fp2_t* y, const fp2_t* z)
{
    fp_t t0, t1;

    fp_add(t0, y->re, y->im);
    fp_add(t1, z->re, z->im);
    fp_mul(t0, t0, t1);
    fp_mul(t1, y->im, z->im);
    fp_mul(x->re, y->re, z->re);
    fp_sub(x->im, t0, t1);
    fp_sub(x->im, x->im, x->re);
    fp_sub(x->re, x->re, t1);
}

void fp2_sqr(fp2_t* x, const fp2_t* y)
{
    fp_t sum, diff;

    fp_add(sum, y->re, y->im);
    fp_sub(diff, y->re, y->im);
    fp_mul(x->im, y->re, y->im);
    fp_add(x->im, x->im, x->im);
    fp_mul(x->re, sum, diff);
}

void fp2_inv(fp2_t* x)
{
    fp_t t0, t1;

    fp_sqr(t0, x->re);
    fp_sqr(t1, x->im);
    fp_add(t0, t0, t1);
    fp_inv(t0);
    fp_mul(x->re, x->re, t0);
    fp_mul(x->im, x->im, t0);
    fp_neg(x->im, x->im);
}

bool fp2_is_square(const fp2_t* x)
{
    fp_t t0, t1;

    fp_sqr(t0, x->re);
    fp_sqr(t1, x->im);
    fp_add(t0, t0, t1);

    return fp_is_square(t0);
}

void fp2_frob(fp2_t* x, const fp2_t* y)
{
    memcpy((digit_t*)x->re, (digit_t*)y->re, NWORDS_FIELD*RADIX/8);
    fp_neg(x->im, y->im);
}

void fp2_tomont(fp2_t* x, const fp2_t* y)
{
    fp_tomont(x->re, y->re);
    fp_tomont(x->im, y->im);
}

void fp2_frommont(fp2_t* x, const fp2_t* y)
{
    fp_frommont(x->re, y->re);
    fp_frommont(x->im, y->im);
}

// Lexicographic comparison of two field elements. Returns +1 if x > y, -1 if x < y, 0 if x = y
int fp2_cmp(fp2_t* x, fp2_t* y){
    fp2_t a, b;
    fp2_frommont(&a, x);
    fp2_frommont(&b, y);
    for(int i = NWORDS_FIELD-1; i >= 0; i--){
        if(a.re[i] > b.re[i])
            return 1;
        if(a.re[i] < b.re[i])
            return -1;
    }
    for(int i = NWORDS_FIELD-1; i >= 0; i--){
        if(a.im[i] > b.im[i])
            return 1;
        if(a.im[i] < b.im[i])
            return -1;
    }
    return 0;
}

#endif
