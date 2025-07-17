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

// #define USE_MODFSB
// #define DEBUG_PRINT

static int modfsb_MFP376_ct(spint *a) {
    const spint p0 = 4294967295;
    const spint p1 = 4294967295;
    const spint p2 = 4294967295;
    const spint p3 = 4294967295;
    const spint p4 = 4294967295;
    const spint p5 = 4294967295;
    const spint p6 = 4294967295;
    const spint p7 = 4294967295;
    const spint p8 = 4294967295;
    const spint p9 = 4294967295;
    const spint p10 = 4294967295;
    const spint p11 = 1090519039;
    spint t0, t1;
    spint tmpvec[12];
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
        "ldr    %[t0], [%[a], #32]\n"
        "str    %[t1], [%[tmpvec], #28]\n"
        "sbcs   %[t0], %[t0], %[p8]\n"
        "ldr    %[t1], [%[a], #36]\n"
        "str    %[t0], [%[tmpvec], #32]\n"
        "sbcs   %[t1], %[t1], %[p9]\n"
        "ldr    %[t0], [%[a], #40]\n"
        "str    %[t1], [%[tmpvec], #36]\n"
        "sbcs   %[t0], %[t0], %[p10]\n"
        "ldr    %[t1], [%[a], #44]\n"
        "str    %[t0], [%[tmpvec], #40]\n"
        "sbcs   %[t1], %[t1], %[p11]\n"
        "str    %[t1], [%[tmpvec], #44]\n"
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
        "itttt cs\n"
        "ldrcs %[t0], [%[tmpvec], #32]\n"
        "ldrcs %[t1], [%[tmpvec], #36]\n"
        "strcs %[t0], [%[a], #32]\n"
        "strcs %[t1], [%[a], #36]\n"
        "itttt cs\n"
        "ldrcs %[t0], [%[tmpvec], #40]\n"
        "ldrcs %[t1], [%[tmpvec], #44]\n"
        "strcs %[t0], [%[a], #40]\n"
        "strcs %[t1], [%[a], #44]\n"
        "adc    %[r], %[r], %[r]\n"
        : [t0] "=&r" (t0), [t1] "=&r" (t1), [r] "+&r" (r)
        : [a] "r" (a), [p0] "rI" (p0), [p1] "rI" (p1), [p2] "rI" (p2), [p3] "rI" (p3), [p4] "rI" (p4), [p5] "rI" (p5), [p6] "rI" (p6), [p7] "rI" (p7), [p8] "rI" (p8), [p9] "rI" (p9), [p10] "rI" (p10), [p11] "rI" (p11), [tmpvec] "r" (tmpvec)
        : "cc", "memory");

    return 1 - r;
}

ALWAYS_INLINE static void mul_MFP376_ct(const spint *p, const spint *q, spint *r) {
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
        "ldr.w   %[a], [%[p], #32]\n"
        "str.w  %[t2], [%[r], #28]\n"
        "mov.w  %[t2], #0\n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #36]\n"
        "str.w  %[t3], [%[r], #32]\n"
        "mov.w  %[t3], #0\n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #40]\n"
        "str.w  %[t4], [%[r], #36]\n"
        "mov.w  %[t4], #0\n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #44]\n"
        "str.w  %[t0], [%[r], #40]\n"
        "mov.w  %[t0], #0\n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "str.w  %[t1], [%[r], #44]\n"
        "str.w  %[t2], [%[r], #48]\n"
        "str.w  %[t3], [%[r], #52]\n"
        "str.w  %[t4], [%[r], #56]\n"
        "str.w  %[t0], [%[r], #60]\n"
        : [t1] "=&r"(t1), [t2] "=&r"(t2), [t3] "=&r"(t3), [t4] "=&r"(t4), [t0] "=&r"(t0), [a] "=&r"(a)
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
        "ldr.w   %[a], [%[r], #48]\n"
        "str.w  %[t4], [%[r], #32]\n"
        "mov.w  %[t4], #0\n"
        "adcs.w %[t3], %[t3], %[a]\n"
        "ldr.w   %[a], [%[p], #20]\n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #52]\n"
        "str.w  %[t0], [%[r], #36]\n"
        "mov.w  %[t0], #0\n"
        "adcs.w %[t4], %[t4], %[a]\n"
        "ldr.w   %[a], [%[p], #24]\n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #56]\n"
        "str.w  %[t1], [%[r], #40]\n"
        "mov.w  %[t1], #0\n"
        "adcs.w %[t0], %[t0], %[a]\n"
        "ldr.w   %[a], [%[p], #28]\n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #60]\n"
        "str.w  %[t2], [%[r], #44]\n"
        "mov.w  %[t2], #0\n"
        "adcs.w %[t1], %[t1], %[a]\n"
        "ldr.w   %[a], [%[p], #32]\n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #36]\n"
        "str.w  %[t3], [%[r], #48]\n"
        "mov.w  %[t3], #0\n"
#ifdef __clang__
        "adcs   %[t2], %[t2], #0\n"
#else
        "adcs.w %[t2], %[t2], #0\n"
#endif
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #40]\n"
        "str.w  %[t4], [%[r], #52]\n"
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
        "ldr.w   %[a], [%[p], #44]\n"
        "str.w  %[t0], [%[r], #56]\n"
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
#ifdef __clang__
        "adcs   %[t0], %[t0], #0\n"
#else
        "adcs.w %[t0], %[t0], #0\n"
#endif
        "str.w  %[t1], [%[r], #60]\n"
        "str.w  %[t2], [%[r], #64]\n"
        "str.w  %[t3], [%[r], #68]\n"
        "str.w  %[t4], [%[r], #72]\n"
        "str.w  %[t0], [%[r], #76]\n"
        "mov.w  %[t1], #0\n"
#ifdef __clang__
        "adcs   %[t1], %[t1], #0\n"
#else
        "adcs.w %[t1], %[t1], #0\n"
#endif
        "str.w  %[t1], [%[r], #80]\n"
        : [t0] "+&r"(t0), [t1] "+&r"(t1), [t2] "+&r"(t2), [t3] "+&r"(t3), [t4] "+&r"(t4), [a] "=&r"(a)
        : [b0] "r"(b0), [b1] "r"(b1), [b2] "r"(b2), [b3] "r"(b3), [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
    b0 = q[8];
    b1 = q[9];
    b2 = q[10];
    b3 = q[11];
    t0 = r[8];
    t1 = r[9];
    t2 = r[10];
    t3 = r[11];
    t4 = 0;
    asm volatile(
        ".p2align 2\n"
        "ldr.w   %[a], [%[p], #0]\n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #48]\n"
        "str.w  %[t0], [%[r], #32]\n"
        "mov.w  %[t0], #0\n"
        "adds.w %[t4], %[t4], %[a]\n"
        "ldr.w   %[a], [%[p], #4]\n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #52]\n"
        "str.w  %[t1], [%[r], #36]\n"
        "mov.w  %[t1], #0\n"
        "adcs.w %[t0], %[t0], %[a]\n"
        "ldr.w   %[a], [%[p], #8]\n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #56]\n"
        "str.w  %[t2], [%[r], #40]\n"
        "mov.w  %[t2], #0\n"
        "adcs.w %[t1], %[t1], %[a]\n"
        "ldr.w   %[a], [%[p], #12]\n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #60]\n"
        "str.w  %[t3], [%[r], #44]\n"
        "mov.w  %[t3], #0\n"
        "adcs.w %[t2], %[t2], %[a]\n"
        "ldr.w   %[a], [%[p], #16]\n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #64]\n"
        "str.w  %[t4], [%[r], #48]\n"
        "mov.w  %[t4], #0\n"
        "adcs.w %[t3], %[t3], %[a]\n"
        "ldr.w   %[a], [%[p], #20]\n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #68]\n"
        "str.w  %[t0], [%[r], #52]\n"
        "mov.w  %[t0], #0\n"
        "adcs.w %[t4], %[t4], %[a]\n"
        "ldr.w   %[a], [%[p], #24]\n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #72]\n"
        "str.w  %[t1], [%[r], #56]\n"
        "mov.w  %[t1], #0\n"
        "adcs.w %[t0], %[t0], %[a]\n"
        "ldr.w   %[a], [%[p], #28]\n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #76]\n"
        "str.w  %[t2], [%[r], #60]\n"
        "mov.w  %[t2], #0\n"
        "adcs.w %[t1], %[t1], %[a]\n"
        "ldr.w   %[a], [%[p], #32]\n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[r], #80]\n"
        "str.w  %[t3], [%[r], #64]\n"
        "mov.w  %[t3], #0\n"
        "adcs.w %[t2], %[t2], %[a]\n"
        "ldr.w   %[a], [%[p], #36]\n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #40]\n"
        "str.w  %[t4], [%[r], #68]\n"
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
        "ldr.w   %[a], [%[p], #44]\n"
        "str.w  %[t0], [%[r], #72]\n"
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
#ifdef __clang__
        "adcs   %[t0], %[t0], #0\n"
#else
        "adcs.w %[t0], %[t0], #0\n"
#endif
        "str.w  %[t1], [%[r], #76]\n"
        "str.w  %[t2], [%[r], #80]\n"
        "str.w  %[t3], [%[r], #84]\n"
        "str.w  %[t4], [%[r], #88]\n"
        "str.w  %[t0], [%[r], #92]\n"
        : [t0] "+&r"(t0), [t1] "+&r"(t1), [t2] "+&r"(t2), [t3] "+&r"(t3), [t4] "+&r"(t4), [a] "=&r"(a)
        : [b0] "r"(b0), [b1] "r"(b1), [b2] "r"(b2), [b3] "r"(b3), [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
}

ALWAYS_INLINE static void sqr_MFP376_ct(const spint *p, spint *r) {
    spint t0, t1, t2, t3, t4, a, b0, b1, b2, b3;
    b0 = p[0];
    b1 = p[1];
    b2 = p[2];
    b3 = p[3];
    asm volatile(
        ".p2align 2\n"
        "mov.w  %[t2], #0\n"
        "mov.w  %[t3], #0\n"
        "mov.w  %[t4], #0\n"
        "umull  %[t0], %[t1], %[b0], %[b1]\n"
        "str.w  %[t0], [%[r], #4]\n"
        "mov.w  %[t0], #0\n"
        "umaal  %[t1], %[t3], %[b0], %[b2]\n"
        "umaal  %[t2], %[t3], %[b1], %[b2]\n"
        "str.w  %[t1], [%[r], #8]\n"
        "mov.w  %[t1], #0\n"
        "umaal  %[t2], %[t0], %[b0], %[b3]\n"
        "umaal  %[t3], %[t0], %[b1], %[b3]\n"
        "umaal  %[t4], %[t0], %[b2], %[b3]\n"
        "str.w  %[t2], [%[r], #12]\n"
        "mov.w  %[t2], #0\n"
        "ldr.w   %[a], [%[p], #16]\n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #20]\n"
        "str.w  %[t3], [%[r], #16]\n"
        "mov.w  %[t3], #0\n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #24]\n"
        "str.w  %[t4], [%[r], #20]\n"
        "mov.w  %[t4], #0\n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #28]\n"
        "str.w  %[t0], [%[r], #24]\n"
        "mov.w  %[t0], #0\n"
        "umaal  %[t1], %[t0], %[a], %[b0]\n"
        "umaal  %[t2], %[t0], %[a], %[b1]\n"
        "umaal  %[t3], %[t0], %[a], %[b2]\n"
        "umaal  %[t4], %[t0], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #32]\n"
        "str.w  %[t1], [%[r], #28]\n"
        "mov.w  %[t1], #0\n"
        "umaal  %[t2], %[t1], %[a], %[b0]\n"
        "umaal  %[t3], %[t1], %[a], %[b1]\n"
        "umaal  %[t4], %[t1], %[a], %[b2]\n"
        "umaal  %[t0], %[t1], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #36]\n"
        "str.w  %[t2], [%[r], #32]\n"
        "mov.w  %[t2], #0\n"
        "umaal  %[t3], %[t2], %[a], %[b0]\n"
        "umaal  %[t4], %[t2], %[a], %[b1]\n"
        "umaal  %[t0], %[t2], %[a], %[b2]\n"
        "umaal  %[t1], %[t2], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #40]\n"
        "str.w  %[t3], [%[r], #36]\n"
        "mov.w  %[t3], #0\n"
        "umaal  %[t4], %[t3], %[a], %[b0]\n"
        "umaal  %[t0], %[t3], %[a], %[b1]\n"
        "umaal  %[t1], %[t3], %[a], %[b2]\n"
        "umaal  %[t2], %[t3], %[a], %[b3]\n"
        "ldr.w   %[a], [%[p], #44]\n"
        "str.w  %[t4], [%[r], #40]\n"
        "mov.w  %[t4], #0\n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "str.w  %[t0], [%[r], #44]\n"
        "str.w  %[t1], [%[r], #48]\n"
        "str.w  %[t2], [%[r], #52]\n"
        "str.w  %[t3], [%[r], #56]\n"
        "str.w  %[t4], [%[r], #60]\n"
        : [t0] "=&r"(t0), [t1] "=&r"(t1), [t2] "=&r"(t2), [t3] "=&r"(t3), [t4] "=&r"(t4), [a] "=&r"(a)
        : [b0] "r"(b0), [b1] "r"(b1), [b2] "r"(b2), [b3] "r"(b3), [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
    spint a0=p[4], a1=p[5], a2=p[6], a3=p[7];
    spint a4=p[8], a5=p[9], a6=p[10], a7=p[11], c0;
    asm volatile(
        ".p2align 2\n"
        "ldr.w   %[c0], [%[r], #36]\n"
        "mov.w   %[t0], #0\n"
        "mov.w   %[t1], #0\n"
        "mov.w   %[t2], #0\n"
        "umaal   %[c0], %[t0], %[a1], %[a0]\n"
        "ldr.w   %[t3], [%[r], #40]\n"
        "str.w   %[c0], [%[r], #36]\n"
        "umaal   %[t3], %[t0], %[a2], %[a0]\n"
        "ldr.w   %[c0], [%[r], #44]\n"
        "str.w   %[t3], [%[r], #40]\n"
        "umaal   %[c0], %[t0], %[a3], %[a0]\n"
        "umaal   %[c0], %[t1], %[a2], %[a1]\n"
        "ldr.w   %[t3], [%[r], #48]\n"
        "str.w   %[c0], [%[r], #44]\n"
        "umaal   %[t3], %[t0], %[a4], %[a0]\n"
        "umaal   %[t3], %[t1], %[a3], %[a1]\n"
        "ldr.w   %[c0], [%[r], #52]\n"
        "str.w   %[t3], [%[r], #48]\n"
        "umaal   %[c0], %[t0], %[a5], %[a0]\n"
        "umaal   %[c0], %[t1], %[a4], %[a1]\n"
        "umaal   %[c0], %[t2], %[a3], %[a2]\n"
        "ldr.w   %[t3], [%[r], #56]\n"
        "str.w   %[c0], [%[r], #52]\n"
        "umaal   %[t3], %[t0], %[a6], %[a0]\n"
        "umaal   %[t3], %[t1], %[a5], %[a1]\n"
        "umaal   %[t3], %[t2], %[a4], %[a2]\n"
        "ldr.w   %[c0], [%[r], #60]\n"
        "str.w   %[t3], [%[r], #56]\n"
        "mov.w   %[t3], #0\n"
        "umaal   %[c0], %[t0], %[a7], %[a0]\n"
        "umaal   %[c0], %[t1], %[a6], %[a1]\n"
        "umaal   %[c0], %[t2], %[a5], %[a2]\n"
        "umaal   %[c0], %[t3], %[a4], %[a3]\n"
        "str.w   %[c0], [%[r], #60]\n"
        "umaal   %[t0], %[t1], %[a7], %[a1]\n"
        "umaal   %[t0], %[t2], %[a6], %[a2]\n"
        "umaal   %[t0], %[t3], %[a5], %[a3]\n"
        "str.w   %[t0], [%[r], #64]\n"
        "mov.w   %[t0], #0\n"
        "umaal   %[t0], %[t1], %[a7], %[a2]\n"
        "umaal   %[t0], %[t2], %[a6], %[a3]\n"
        "umaal   %[t0], %[t3], %[a5], %[a4]\n"
        "str.w   %[t0], [%[r], #68]\n"
        "umaal   %[t1], %[t2], %[a7], %[a3]\n"
        "umaal   %[t1], %[t3], %[a6], %[a4]\n"
        "str.w   %[t1], [%[r], #72]\n"
        "mov.w   %[t0], #0\n"
        "umaal   %[t2], %[t3], %[a7], %[a4]\n"
        "umaal   %[t2], %[t0], %[a6], %[a5]\n"
        "str.w   %[t2], [%[r], #76]\n"
        "umaal   %[t3], %[t0], %[a7], %[a5]\n"
        "str.w   %[t3], [%[r], #80]\n"
        "mov.w   %[t1], #0\n"
        "umaal   %[t0], %[t1], %[a7], %[a6]\n"
        "str.w   %[t0], [%[r], #84]\n"
        "str.w   %[t1], [%[r], #88]\n"
        : [a0] "+&r"(a0), [c0] "=&r"(c0), [t0] "=&r"(t0), [t1] "=&r"(t1), [t2] "=&r"(t2), [t3] "=&r"(t3)
        : [a1] "r"(a1), [a2] "r"(a2), [a3] "r"(a3), [a4] "r"(a4), [a5] "r"(a5), [a6] "r"(a6), [a7] "r"(a7), [r] "r"(r)
        : "memory");
    r[0] = 0;
    r[23] = 0;
    asm volatile(
        ".p2align 2\n"
        "mov.w  %[a], #2\n"
        "mov.w  %[t4], #0\n"
        "ldr.w   %[b0], [%[p], #0]\n"
        "ldr.w   %[b1], [%[p], #4]\n"
        "umull  %[t0], %[t1], %[b0], %[b0]\n"
        "umull  %[t2], %[t3], %[b1], %[b1]\n"
        "ldr.w   %[b0], [%[r], #0]\n"
        "ldr.w   %[b1], [%[r], #4]\n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "ldr.w   %[b0], [%[r], #8]\n"
        "ldr.w   %[b1], [%[r], #12]\n"
        "str.w  %[t0], [%[r], #0]\n"
        "str.w  %[t1], [%[r], #4]\n"
        "umaal  %[t2], %[t4], %[a], %[b0]\n"
        "umaal  %[t3], %[t4], %[a], %[b1]\n"
        "str.w  %[t2], [%[r], #8]\n"
        "str.w  %[t3], [%[r], #12]\n"
        "ldr.w   %[b2], [%[p], #8]\n"
        "ldr.w   %[b3], [%[p], #12]\n"
        "umull  %[t0], %[t1], %[b2], %[b2]\n"
        "umull  %[t2], %[t3], %[b3], %[b3]\n"
        "ldr.w   %[b2], [%[r], #16]\n"
        "ldr.w   %[b3], [%[r], #20]\n"
        "umaal  %[t0], %[t4], %[a], %[b2]\n"
        "umaal  %[t1], %[t4], %[a], %[b3]\n"
        "ldr.w   %[b2], [%[r], #24]\n"
        "ldr.w   %[b3], [%[r], #28]\n"
        "str.w  %[t0], [%[r], #16]\n"
        "str.w  %[t1], [%[r], #20]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "str.w  %[t2], [%[r], #24]\n"
        "str.w  %[t3], [%[r], #28]\n"
        "ldr.w   %[b0], [%[p], #16]\n"
        "ldr.w   %[b1], [%[p], #20]\n"
        "umull  %[t0], %[t1], %[b0], %[b0]\n"
        "umull  %[t2], %[t3], %[b1], %[b1]\n"
        "ldr.w   %[b0], [%[r], #32]\n"
        "ldr.w   %[b1], [%[r], #36]\n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "ldr.w   %[b0], [%[r], #40]\n"
        "ldr.w   %[b1], [%[r], #44]\n"
        "str.w  %[t0], [%[r], #32]\n"
        "str.w  %[t1], [%[r], #36]\n"
        "umaal  %[t2], %[t4], %[a], %[b0]\n"
        "umaal  %[t3], %[t4], %[a], %[b1]\n"
        "str.w  %[t2], [%[r], #40]\n"
        "str.w  %[t3], [%[r], #44]\n"
        "ldr.w   %[b2], [%[p], #24]\n"
        "ldr.w   %[b3], [%[p], #28]\n"
        "umull  %[t0], %[t1], %[b2], %[b2]\n"
        "umull  %[t2], %[t3], %[b3], %[b3]\n"
        "ldr.w   %[b2], [%[r], #48]\n"
        "ldr.w   %[b3], [%[r], #52]\n"
        "umaal  %[t0], %[t4], %[a], %[b2]\n"
        "umaal  %[t1], %[t4], %[a], %[b3]\n"
        "ldr.w   %[b2], [%[r], #56]\n"
        "ldr.w   %[b3], [%[r], #60]\n"
        "str.w  %[t0], [%[r], #48]\n"
        "str.w  %[t1], [%[r], #52]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "str.w  %[t2], [%[r], #56]\n"
        "str.w  %[t3], [%[r], #60]\n"
        "ldr.w   %[b0], [%[p], #32]\n"
        "ldr.w   %[b1], [%[p], #36]\n"
        "umull  %[t0], %[t1], %[b0], %[b0]\n"
        "umull  %[t2], %[t3], %[b1], %[b1]\n"
        "ldr.w   %[b0], [%[r], #64]\n"
        "ldr.w   %[b1], [%[r], #68]\n"
        "umaal  %[t0], %[t4], %[a], %[b0]\n"
        "umaal  %[t1], %[t4], %[a], %[b1]\n"
        "ldr.w   %[b0], [%[r], #72]\n"
        "ldr.w   %[b1], [%[r], #76]\n"
        "str.w  %[t0], [%[r], #64]\n"
        "str.w  %[t1], [%[r], #68]\n"
        "umaal  %[t2], %[t4], %[a], %[b0]\n"
        "umaal  %[t3], %[t4], %[a], %[b1]\n"
        "str.w  %[t2], [%[r], #72]\n"
        "str.w  %[t3], [%[r], #76]\n"
        "ldr.w   %[b2], [%[p], #40]\n"
        "ldr.w   %[b3], [%[p], #44]\n"
        "umull  %[t0], %[t1], %[b2], %[b2]\n"
        "umull  %[t2], %[t3], %[b3], %[b3]\n"
        "ldr.w   %[b2], [%[r], #80]\n"
        "ldr.w   %[b3], [%[r], #84]\n"
        "umaal  %[t0], %[t4], %[a], %[b2]\n"
        "umaal  %[t1], %[t4], %[a], %[b3]\n"
        "ldr.w   %[b2], [%[r], #88]\n"
        "ldr.w   %[b3], [%[r], #92]\n"
        "str.w  %[t0], [%[r], #80]\n"
        "str.w  %[t1], [%[r], #84]\n"
        "umaal  %[t2], %[t4], %[a], %[b2]\n"
        "umaal  %[t3], %[t4], %[a], %[b3]\n"
        "str.w  %[t2], [%[r], #88]\n"
        "str.w  %[t3], [%[r], #92]\n"
        : [t0] "=&r"(t0), [t1] "=&r"(t1), [t2] "=&r"(t2), [t3] "=&r"(t3), [t4] "=&r"(t4), [a] "=&r"(a),
          [b0] "=&r"(b0), [b1] "=&r"(b1), [b2] "=&r"(b2), [b3] "=&r"(b3)
        : [p] "r"(p), [r] "r"(r)
        : "cc", "memory");
}

static void modmul_MFP376_ct(const spint *a, const spint *b, spint *res) {
    spint A[24];
    spint A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23;
    const spint p11 = 1090519039;
    spint q, t, t2 = 0;

    mul_MFP376_ct(a, b, A);

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

    q = A0;
    t = 0;
    UMAAL(A11, t, q, p11 + 1);

    q = A1;
    UMAAL(A12, t, q, p11 + 1);

    q = A2;
    UMAAL(A13, t, q, p11 + 1);

    q = A3;
    UMAAL(A14, t, q, p11 + 1);

    q = A4;
    UMAAL(A15, t, q, p11 + 1);

    q = A5;
    UMAAL(A16, t, q, p11 + 1);

    q = A6;
    UMAAL(A17, t, q, p11 + 1);

    q = A7;
    UMAAL(A18, t, q, p11 + 1);

    q = A8;
    UMAAL(A19, t, q, p11 + 1);

    q = A9;
    UMAAL(A20, t, q, p11 + 1);

    q = A10;
    UMAAL(A21, t, q, p11 + 1);

    q = A11;
    UMAAL(A22, t, q, p11 + 1);
    A23 += t;

    res[0] = A12;
    res[1] = A13;
    res[2] = A14;
    res[3] = A15;
    res[4] = A16;
    res[5] = A17;
    res[6] = A18;
    res[7] = A19;
    res[8] = A20;
    res[9] = A21;
    res[10] = A22;
    res[11] = A23;

#ifdef USE_MODFSB
    modfsb_MFP376_ct(res);
#endif
}

ALWAYS_INLINE static void inner_modsqr_MFP376_ct(const spint *a, spint *res) {
    spint A[24];
    spint A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23;
    const spint p11 = 1090519039;
    spint q, t, t2 = 0;

    sqr_MFP376_ct(a, A);

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

    q = A0;
    t = 0;
    UMAAL(A11, t, q, p11 + 1);

    q = A1;
    UMAAL(A12, t, q, p11 + 1);

    q = A2;
    UMAAL(A13, t, q, p11 + 1);

    q = A3;
    UMAAL(A14, t, q, p11 + 1);

    q = A4;
    UMAAL(A15, t, q, p11 + 1);

    q = A5;
    UMAAL(A16, t, q, p11 + 1);

    q = A6;
    UMAAL(A17, t, q, p11 + 1);

    q = A7;
    UMAAL(A18, t, q, p11 + 1);

    q = A8;
    UMAAL(A19, t, q, p11 + 1);

    q = A9;
    UMAAL(A20, t, q, p11 + 1);

    q = A10;
    UMAAL(A21, t, q, p11 + 1);

    q = A11;
    UMAAL(A22, t, q, p11 + 1);
    A23 += t;

    res[0] = A12;
    res[1] = A13;
    res[2] = A14;
    res[3] = A15;
    res[4] = A16;
    res[5] = A17;
    res[6] = A18;
    res[7] = A19;
    res[8] = A20;
    res[9] = A21;
    res[10] = A22;
    res[11] = A23;

#ifdef USE_MODFSB
    modfsb_MFP376_ct(res);
#endif
}

static void modsqr_MFP376_ct(const spint *a, spint *res) {
    inner_modsqr_MFP376_ct(a, res);
}

static void nres_MFP376_ct(const spint *a, spint *b) {
    spint nresfactor[] = {66076435, 1057222719, 4030661616, 66076419, 1057222719, 4030661616, 66076419, 1057222719, 4030661616, 66076419, 1057222719, 490669040};
    modmul_MFP376_ct(a, nresfactor, b);
}

static void redc_MFP376_ct(const spint *a, spint *b) {
    spint rinv[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    modmul_MFP376_ct(a, rinv, b);
    modfsb_MFP376_ct(b);
}

static void modcmv_MFP376_ct(int d, const spint *a, volatile spint *b) {
    spint c0, c1, s, t, aux;
    spint r = 0x5aa5a55a;
    c0 = (~d) & (r + 1);
    c1 = d + r;
    for (int i = 0; i < 12; i++) {
        s = a[i];
        t = b[i];
        b[i] = aux = c0 * t + c1 * s;
        b[i] = aux - r * (t + s);
    }
}

static void modcsw_MFP376_ct(int d, volatile spint *a, volatile spint *b) {
    spint c0, c1, s, t, w, aux;
    spint r = 0x5aa5a55a;
    c0 = (~d) & (r + 1);
    c1 = d + r;
    for (int i = 0; i < 12; i++) {
        s = a[i];
        t = b[i];
        w = r * (t + s);
        b[i] = aux = c0 * t + c1 * s;
        b[i] = aux - w;
        a[i] = aux = c0 * s + c1 * t;
        a[i] = aux - w;
    }
}

inline static void modadd_MFP376_ct(const spint *a, const spint *b, spint *res) {
    spint a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11;
    spint b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11;
    spint t0, t1, t2, t3;
    spint tmpres[12];
    spint t = 0;
    volatile int x;
#ifdef USE_MODFSB
    const spint p[12] = {4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 1090519039};
#else
    const spint p[12] = {4294967295 - 1, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 4294967295, 2 * 1090519039 + 1};
#endif

#ifdef DEBUG_PRINT
    int case_ = which_case(a[11], b[11]);
    int wanted_case = 2;

    if (case_ == wanted_case) {
        char buf[32];
        hal_send_str("*********************************************************************************************************");
        sprintf(buf, "&a   = %p", a);
        hal_send_str(buf);
        sprintf(buf, "&b   = %p", b);
        hal_send_str(buf);
        sprintf(buf, "&res = %p", b);
        hal_send_str(buf);
        print_hex("a   ", a);
        print_hex("b   ", b);
    }
#endif

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
    a8 = a[8];
    b8 = b[8];
    UMAAL(a8, t, 1, b8);
    res[8] = a8;
    a9 = a[9];
    b9 = b[9];
    UMAAL(a9, t, 1, b9);
    res[9] = a9;
    a10 = a[10];
    b10 = b[10];
    UMAAL(a10, t, 1, b10);
    res[10] = a10;
    a11 = a[11];
    b11 = b[11];
    UMAAL(a11, t, 1, b11);
    res[11] = a11;

#ifdef DEBUG_PRINT
    if (case_ == wanted_case) {
        print_hex("res ", res);
        print_case(case_);
    }
#endif

    asm volatile(
        "ldr %[t0], [%[res]]\n"
        "ldr %[t1], [%[res], #4]\n"
        "ldr %[t2], [%[p]]\n"
        "ldr %[t3], [%[p], #4]\n"
        "subs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        "str %[t0], [%[tmpres]]\n"
        "str %[t1], [%[tmpres], #4]\n"
        "ldr %[t0], [%[res], #8]\n"
        "ldr %[t1], [%[res], #12]\n"
        "ldr %[t2], [%[p], #8]\n"
        "ldr %[t3], [%[p], #12]\n"
        "sbcs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        "str %[t0], [%[tmpres], #8]\n"
        "str %[t1], [%[tmpres], #12]\n"
        "ldr %[t0], [%[res], #16]\n"
        "ldr %[t1], [%[res], #20]\n"
        "ldr %[t2], [%[p], #16]\n"
        "ldr %[t3], [%[p], #20]\n"
        "sbcs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        "str %[t0], [%[tmpres], #16]\n"
        "str %[t1], [%[tmpres], #20]\n"
        "ldr %[t0], [%[res], #24]\n"
        "ldr %[t1], [%[res], #28]\n"
        "ldr %[t2], [%[p], #24]\n"
        "ldr %[t3], [%[p], #28]\n"
        "sbcs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        "str %[t0], [%[tmpres], #24]\n"
        "str %[t1], [%[tmpres], #28]\n"
        "ldr %[t0], [%[res], #32]\n"
        "ldr %[t1], [%[res], #36]\n"
        "ldr %[t2], [%[p], #32]\n"
        "ldr %[t3], [%[p], #36]\n"
        "sbcs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        "str %[t0], [%[tmpres], #32]\n"
        "str %[t1], [%[tmpres], #36]\n"
        "ldr %[t0], [%[res], #40]\n"
        "ldr %[t1], [%[res], #44]\n"
        "ldr %[t2], [%[p], #40]\n"
        "ldr %[t3], [%[p], #44]\n"
        "sbcs %[t0], %[t0], %[t2]\n"
        "sbcs %[t1], %[t1], %[t3]\n"
        // "str %[t0], [%[tmpres], #40]\n"
        // "str %[t1], [%[tmpres], #44]\n"
#ifdef USE_MODFSB
        "itttt cs\n"
        "ldrdcs %[t0], %[t1], [%[tmpres], #0]\n"
        "ldrdcs %[t2], %[t3], [%[tmpres], #8]\n"
        "strdcs %[t0], %[t1], [%[res], #0]\n"
        "strdcs %[t2], %[t3], [%[res], #8]\n"
        "itttt cs\n"
        "ldrdcs %[t0], %[t1], [%[tmpres], #16]\n"
        "ldrdcs %[t2], %[t3], [%[tmpres], #24]\n"
        "strdcs %[t0], %[t1], [%[res], #16]\n"
        "strdcs %[t2], %[t3], [%[res], #24]\n"
        "itttt cs\n"
        "ldrdcs %[t0], %[t1], [%[tmpres], #32]\n"
        "ldrdcs %[t2], %[t3], [%[tmpres], #40]\n"
        "strdcs %[t0], %[t1], [%[res], #32]\n"
        "strdcs %[t2], %[t3], [%[res], #40]\n"
#else
        "adcs %[t], %[t], %[t]\n"
        "ittt  ne\n"
        "ldrdne %[t2], %[t3], [%[tmpres], #32]\n"
        // "ldrdne %[t2], %[t3], [%[tmpres], #40]\n"
        "strdne %[t2], %[t3], [%[res], #32]\n"
        "strdne %[t0], %[t1], [%[res], #40]\n"
        "itttt ne\n"
        "ldrdne %[t0], %[t1], [%[tmpres], #0]\n"
        "ldrdne %[t2], %[t3], [%[tmpres], #8]\n"
        "strdne %[t0], %[t1], [%[res], #0]\n"
        "strdne %[t2], %[t3], [%[res], #8]\n"
        "itttt ne\n"
        "ldrdne %[t0], %[t1], [%[tmpres], #16]\n"
        "ldrdne %[t2], %[t3], [%[tmpres], #24]\n"
        "strdne %[t0], %[t1], [%[res], #16]\n"
        "strdne %[t2], %[t3], [%[res], #24]\n"
#endif
        : [t0] "=&r" (t0), [t1] "=&r" (t1), [t2] "=&r" (t2), [t3] "=&r" (t3)
#ifndef USE_MODFSB
        , [t] "+&r" (t)
#endif
        : [res] "r" (res), [tmpres] "r" (tmpres), [p] "r" (p)
        : "cc", "memory");
#ifdef DEBUG_PRINT
    if (case_ == wanted_case) {
        print_hex("res2", res);
    }
#endif
    x = 31337;
}

inline static void modsub_MFP376_ct(const spint *a, const spint *b, spint *res) {
    spint a0 = a[0], a1 = a[1], a2 = a[2], a3 = a[3], a4 = a[4], a5 = a[5], a6 = a[6], a7 = a[7], a8 = a[8], a9 = a[9], a10 = a[10], a11 = a[11];
    spint t, t1, t2;
    spint t3, t4;
#ifdef USE_MODFSB
    const spint p0 = 4294967295;
#else
    const spint p0 = 4294967295 - 1;
#endif
    const spint p1 = 4294967295;
    const spint p2 = 4294967295;
    const spint p3 = 4294967295;
    const spint p4 = 4294967295;
    const spint p5 = 4294967295;
    const spint p6 = 4294967295;
    const spint p7 = 4294967295;
    const spint p8 = 4294967295;
    const spint p9 = 4294967295;
    const spint p10 = 4294967295;
#ifdef USE_MODFSB
    const spint p11 = 1090519039;
#else
    const spint p11 = 2 * 1090519039 + 1;
#endif

    asm volatile(
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
        "sbcs %[t], %[t3], %[t3]\n"
        : [t] "=&r" (t), [t1] "=&r" (t1), [t2] "=&r" (t2), [t3] "=&r" (t3), [t4] "=&r" (t4)
        : [b] "r" (b), [a] "r" (a), [res] "r" (res)
        : "cc", "memory");
    t = -t | (res[11] > p11);
    t2 = 0;
    a0 = res[0];
    UMAAL(a0, t2, t, p0);
    res[0] = a0;
    a1 = res[1];
    UMAAL(a1, t2, t, p1);
    res[1] = a1;
    a2 = res[2];
    UMAAL(a2, t2, t, p2);
    res[2] = a2;
    a3 = res[3];
    UMAAL(a3, t2, t, p3);
    res[3] = a3;
    a4 = res[4];
    UMAAL(a4, t2, t, p4);
    res[4] = a4;
    a5 = res[5];
    UMAAL(a5, t2, t, p5);
    res[5] = a5;
    a6 = res[6];
    UMAAL(a6, t2, t, p6);
    res[6] = a6;
    a7 = res[7];
    UMAAL(a7, t2, t, p7);
    res[7] = a7;
    a8 = res[8];
    UMAAL(a8, t2, t, p8);
    res[8] = a8;
    a9 = res[9];
    UMAAL(a9, t2, t, p9);
    res[9] = a9;
    a10 = res[10];
    UMAAL(a10, t2, t, p10);
    res[10] = a10;
    a11 = res[11];
    UMAAL(a11, t2, t, p11);
    res[11] = a11;
}

static void modneg_MFP376_ct(const spint *a, spint *r) {
    spint zero[12] = {0};
    modsub_MFP376_ct(zero, a, r);
}

static void modshl_MFP376_ct(int n, spint *a) {
#pragma GCC unroll 11
    for (int i = 11; i > 0; i--) {
        a[i] = (a[i] << n) + (a[i - 1] >> (32 - n));
    }
    a[0] = (a[0] << n);
}

// Only works for n <= 31
static spint modshr_MFP376_ct(int n, spint *a) {
    spint r = a[0] & ((1 << n) - 1);
#pragma GCC unroll 11
    for (int i = 0; i < 11; i++) {
        a[i] = (a[i] >> n) | (a[i + 1] << (32 - n));
    }
    a[11] = a[11] >> n;
    return r;
}

static void modcpy_MFP376_ct(const spint *a, spint *b) {
#pragma GCC unroll 12
    for (int i = 0; i < 12; i++) {
        b[i] = a[i];
    }
}

static int modis0_MFP376_ct(const spint *a) {
    spint c[12] = {0};
    spint d = 0;
    modcpy_MFP376_ct(a, c);
    modfsb_MFP376_ct(c);
    for (int i = 0; i < 12; i++) {
        d |= c[i];
    }
    return 1 - ((d | -d) >> 31);
}

static int modis1_MFP376_ct(const spint *a) {
    spint c[12] = {0};
    spint d = 0;
    redc_MFP376_ct(a, c);
    c[0] = c[0] - 1;
    for (int i = 0; i < 12; i++) {
        d |= c[i];
    }
    return 1 - ((d | -d) >> 31);
}

static int modcmp_MFP376_ct(const spint *a, const spint *b) {
    spint c[12], d[12];
    int neq = 0;
    redc_MFP376_ct(a, c);
    redc_MFP376_ct(b, d);
    for (int i = 0; i < 12; i++) {
        neq |= c[i] ^ d[i];
    }
    neq = (neq | -neq) >> 31;
    return 1 - (neq & 1);
}

static int modsign_MFP376_ct(const spint *a) {
    spint c[12];
    redc_MFP376_ct(a, c);
    return c[0] % 2;
}

NOINLINE static void modnsqr_MFP376_ct(spint *a, int n) {
    for (int i = 0; i < n; i++) {
        inner_modsqr_MFP376_ct(a, a);
    }
}

// Calculate progenitor
NOINLINE static void modpro_MFP376_ct(const spint *w, spint *z) {
    spint x[12];
    spint t0[12];
    spint t1[12];
    spint t2[12];
    spint t3[12];
    spint t4[12];
    spint t5[12];
    modcpy_MFP376_ct(w, x);
    modsqr_MFP376_ct(x, z);
    modsqr_MFP376_ct(z, t0);
    modmul_MFP376_ct(x, t0, t1);
    modmul_MFP376_ct(z, t1, z);
    modsqr_MFP376_ct(z, t0);
    modsqr_MFP376_ct(t0, t3);
    modsqr_MFP376_ct(t3, t4);
    modsqr_MFP376_ct(t4, t2);
    modcpy_MFP376_ct(t2, t5);
    modnsqr_MFP376_ct(t5, 3);
    modmul_MFP376_ct(t2, t5, t2);
    modcpy_MFP376_ct(t2, t5);
    modnsqr_MFP376_ct(t5, 6);
    modmul_MFP376_ct(t2, t5, t2);
    modcpy_MFP376_ct(t2, t5);
    modnsqr_MFP376_ct(t5, 2);
    modmul_MFP376_ct(t4, t5, t5);
    modnsqr_MFP376_ct(t5, 13);
    modmul_MFP376_ct(t2, t5, t2);
    modcpy_MFP376_ct(t2, t5);
    modnsqr_MFP376_ct(t5, 2);
    modmul_MFP376_ct(t4, t5, t4);
    modnsqr_MFP376_ct(t4, 28);
    modmul_MFP376_ct(t2, t4, t2);
    modsqr_MFP376_ct(t2, t4);
    modmul_MFP376_ct(t3, t4, t3);
    modnsqr_MFP376_ct(t3, 59);
    modmul_MFP376_ct(t2, t3, t2);
    modmul_MFP376_ct(t1, t2, t1);
    modmul_MFP376_ct(z, t1, z);
    modmul_MFP376_ct(t0, z, t0);
    modmul_MFP376_ct(t1, t0, t1);
    modsqr_MFP376_ct(t1, t2);
    modmul_MFP376_ct(t1, t2, t2);
    modsqr_MFP376_ct(t2, t2);
    modmul_MFP376_ct(t1, t2, t2);
    modmul_MFP376_ct(t0, t2, t0);
    modmul_MFP376_ct(z, t0, z);
    modsqr_MFP376_ct(z, t2);
    modmul_MFP376_ct(z, t2, t2);
    modmul_MFP376_ct(t0, t2, t0);
    modmul_MFP376_ct(t1, t0, t1);
    modcpy_MFP376_ct(t1, t2);
    modnsqr_MFP376_ct(t2, 128);
    modmul_MFP376_ct(t1, t2, t1);
    modmul_MFP376_ct(t0, t1, t0);
    modnsqr_MFP376_ct(t0, 125);
    modmul_MFP376_ct(z, t0, z);
}

// Test for quadratic residue, provide progenitor h if available, NULL if not
static int modqr_MFP376_ct(const spint *h, const spint *x) {
    spint r[12];
    if (h == NULL) {
        modpro_MFP376_ct(x, r);
        modsqr_MFP376_ct(r, r);
    }
    else {
        modsqr_MFP376_ct(h, r);
    }
    modmul_MFP376_ct(r, x, r);
    return modis1_MFP376_ct(r);
}

// Modular square root, provide progenitor h if available, NULL if not
static void modsqrt_MFP376_ct(const spint *x, const spint *h, spint *r) {
    spint s[12];
    spint y[12];
    if (h == NULL) {
        modpro_MFP376_ct(x, y);
    }
    else {
        modcpy_MFP376_ct(h, y);
    }
    modmul_MFP376_ct(y, x, s);
    modcpy_MFP376_ct(s, r);
}

// Calculate inverse, provide progenitor h if available
static void modinv_MFP376_ct(const spint *x, const spint *h, spint *z) {
    spint s[12];
    spint t[12];
    if (h == NULL) {
        modpro_MFP376_ct(x, t);
    }
    else {
        modcpy_MFP376_ct(h, t);
    }
    modcpy_MFP376_ct(x, s);
    modnsqr_MFP376_ct(t, 2);
    modmul_MFP376_ct(s, t, z);
}

static void modzer_MFP376_ct(spint *a) {
#pragma GCC unroll 12
    for (int i = 0; i < 12; i++) {
        a[i] = 0;
    }
}

static void modone_MFP376_ct(spint *a) {
    a[0] = 3;
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
    a[11] = 1023410176;
}

static void modint_MFP376_ct(int x, spint *a) {
    a[0] = (spint)x;
#pragma GCC unroll 11
    for (int i = 1; i < 12; i++) {
        a[i] = 0;
    }
    nres_MFP376_ct(a, a);
}

static void mod2r_MFP376_ct(unsigned int r, spint *a) {
    unsigned int n = r >> 5;
    unsigned int m = r & 31;
    modzer_MFP376_ct(a);
    if (r >= 384) {
        return;
    }
    a[n] = 1 << m;
    nres_MFP376_ct(a, a);
}

static void modmli_MFP376_ct(const spint *p, const spint q, spint *r) {
    spint t[12];
    modint_MFP376_ct(q, t);
    modmul_MFP376_ct(p, t, r);
}

static void modimp_MFP376_ct(const char *b, spint *a) {
    const spint *b32 = (const spint *)b;
#pragma GCC unroll 12
    for (int i = 0; i < 12; i++) {
        a[i] = __builtin_bswap32(b32[11 - i]);
    }
    nres_MFP376_ct(a, a);
}

// Code for export, not exponentiation
static void modexp_MFP376_ct(const spint *a, char *b) {
    spint c[12], *b32 = (spint *)b;
    redc_MFP376_ct(a, c);
#pragma GCC unroll 12
    for (int i = 0; i < 12; i++) {
        b32[11 - i] = __builtin_bswap32(c[i]);
    }
}

#define Nlimbs_MFP376_ct 12
#define Nbits_MFP376_ct  383
#define Nbytes_MFP376_ct 48
