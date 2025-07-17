#include <assert.h>
#include <fp.h>

extern const digit_t p[NWORDS_FIELD];
extern const digit_t pre[NWORDS_FIELD];

void fp_set(digit_t* x, const digit_t val)
{ // Set field element x = val, where val has wordsize

    x[0] = val;
    for (unsigned int i = 1; i < NWORDS_FIELD; i++) {
        x[i] = 0;
    }
}

bool fp_is_equal(const digit_t* a, const digit_t* b)
{ // Compare two field elements in constant time
  // Returns 1 (true) if a=b, 0 (false) otherwise
    digit_t r = 0;

    for (unsigned int i = 0; i < NWORDS_FIELD; i++)
        r |= a[i] ^ b[i];

    return (bool)is_digit_zero_ct(r);
}

bool fp_is_zero(const digit_t* a)
{ // Is a field element zero?
  // Returns 1 (true) if a=0, 0 (false) otherwise
    digit_t r = 0;

    for (unsigned int i = 0; i < NWORDS_FIELD; i++)
        r |= a[i] ^ 0;

    return (bool)is_digit_zero_ct(r);
}

void fp_copy(digit_t* out, const digit_t* a)
{
    memcpy(out, a, NWORDS_FIELD*RADIX/8);
}

void fp_neg(digit_t* out, const digit_t* a)
{ // Modular negation, out = -a mod p
  // Input: a in [0, p-1] 
  // Output: out in [0, p-1] 
    unsigned int i, borrow = 0;

    for (i = 0; i < NWORDS_FIELD; i++) {
        SUBC(out[i], borrow, ((digit_t*)p)[i], a[i], borrow);
    }
    fp_sub(out, out, (digit_t*)p);
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

void fp_exp3div4(digit_t* out, const digit_t* a)
{ 
#if defined(PRIME_P248)
    // Optimized implementation for p248

    // Addition chain for p248
    // (p248 - 3) / 4 = 2^248 + 2^246 - 1
    // Cost: 247 SQR + 12 MUL
    fp_t acc, acct, acc1, acc3, acc12;

    // acc = a^(2^1-1)
    memcpy((digit_t*)acc1, (digit_t*)a, NWORDS_FIELD*RADIX/8);

    // acc = a^(2^2-1)
    fp_sqr(acc, acc1);
    fp_mul(acc, acc, acc1);

    // acc = a^(2^3-1)
    fp_sqr(acc, acc);
    fp_mul(acc3, acc, acc1);

    // acc = a^(2^6-1)
    fp_sqr(acct, acc3);
    for (int i = 0; i < 2; i++) {
        fp_sqr(acct, acct);
    }
    fp_mul(acc, acct, acc3);

    // acc = a^(2^12-1)
    fp_sqr(acct, acc);
    for (int i = 0; i < 5; i++) {
        fp_sqr(acct, acct);
    }
    fp_mul(acc12, acct, acc);

    // acc = a^(2^24-1)
    fp_sqr(acct, acc12);
    for (int i = 0; i < 11; i++) {
        fp_sqr(acct, acct);
    }
    fp_mul(acc, acct, acc12);

    // acc = a^(2^48-1)
    fp_sqr(acct, acc);
    for (int i = 0; i < 23; i++) {
        fp_sqr(acct, acct);
    }
    fp_mul(acc, acct, acc);

    // acc = a^(2^60-1)
    fp_sqr(acct, acc);
    for (int i = 0; i < 11; i++) {
        fp_sqr(acct, acct);
    }
    fp_mul(acc, acct, acc12);

    // acc = a^(2^120-1)
    fp_sqr(acct, acc);
    for (int i = 0; i < 59; i++) {
        fp_sqr(acct, acct);
    }
    fp_mul(acc, acct, acc);

    // acc = a^(2^123-1)
    fp_sqr(acct, acc);
    for (int i = 0; i < 2; i++) {
        fp_sqr(acct, acct);
    }
    fp_mul(acc, acct, acc3);

    // acc = a^(2^246-1)
    fp_sqr(acct, acc);
    for (int i = 0; i < 122; i++) {
        fp_sqr(acct, acct);
    }
    fp_mul(acct, acct, acc);

    // acc = a^(2^246)
    fp_mul(acc, acct, a);

    // acc = a^(2^248)
    fp_sqr(acc, acc);
    fp_sqr(acc, acc);

    // acct = a^(2^248) * a^(2^246-1)
    fp_mul(out, acct, acc);

#else
    fp_t p_t, acc;
    digit_t bit;

    memcpy((digit_t*)p_t, (digit_t*)p, NWORDS_FIELD*RADIX/8);
    memcpy((digit_t*)acc, (digit_t*)a, NWORDS_FIELD*RADIX/8);
    mp_shiftr(p_t, 1, NWORDS_FIELD);
    mp_shiftr(p_t, 1, NWORDS_FIELD);
    fp_set(out, 1);
    fp_tomont(out, out);

    for (int i = 0; i < NWORDS_FIELD*RADIX-2; i++) {
        bit = p_t[0] & 1;
        mp_shiftr(p_t, 1, NWORDS_FIELD);
        if (bit == 1) {
            fp_mul(out, out, acc);
        }
        fp_sqr(acc, acc);
    }
#endif
}

void _fp_inv(digit_t* a)
{ // Modular inversion, out = x^-1*R mod p, where R = 2^(w*nwords), w is the computer wordsize and nwords is the number of words to represent p
  // Input: a=xR in [0, p-1] 
  // Output: out in [0, p-1]. It outputs 0 if the input does not have an inverse  
  // Requirement: Ceiling(Log(p)) < w*nwords
    fp_t t;

    fp_exp3div4(t, a);
    fp_sqr(t, t);
    fp_sqr(t, t);
    fp_mul(a, t, a);    // a^(p-2)
}

bool _fp_is_square(const digit_t* a)
{ // Is field element a square?
  // Output: out = 0 (false), 1 (true)
    fp_t t, one;

    fp_exp3div4(t, a);
    fp_sqr(t, t);
    fp_mul(t, t, a);    // a^((p-1)/2)
    fp_frommont(t, t);
    fp_set(one, 1);

    return fp_is_equal(t, one);
}

void fp_sqrt(digit_t* a)
{ // Square root computation, out = a^((p+1)/4) mod p
#if defined(PRIME_P248)
    // (p248 + 1) / 4 = 2^248 + 2^246
    fp_t out, acc;

    fp_sqr(out, a);
    for (int i = 0; i < 245; i++) {
        fp_sqr(out, out);
    }

    fp_sqr(acc, out);
    fp_sqr(acc, acc);
    fp_mul(a, out, acc);
#else
    fp_t t;

    fp_exp3div4(t, a);
    fp_mul(a, t, a);    // a^((p+1)/4)
#endif
}

void fp_to_digit_array(digit_t* out, const digit_t* a) {
    memcpy(out, a, NWORDS_FIELD*RADIX/8);
}

void fp_from_digit_array(digit_t* out, const digit_t* a) {
    memcpy(out, a, NWORDS_FIELD*RADIX/8);
}

#if defined(PQM4)
#include "bn.inc"
#include "inversion.inc"
#include "symbol.inc"
#else
#include "../../generic/bn.inc"
#include "../../generic/inversion.inc"
#include "../../generic/symbol.inc"
#endif
