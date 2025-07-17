#include <string.h>
#include <tutil.h>
#include <protocols.h>
#include <fp_constants.h>
#include <assert.h>


// digits

static void encode_digits(unsigned char* enc, const digit_t* x, size_t nbytes)
{
#ifdef TARGET_BIG_ENDIAN
    const size_t ndigits = nbytes / sizeof(digit_t);
    const size_t rem = nbytes % sizeof(digit_t);

    for (size_t i = 0; i < ndigits; i++)
        ((digit_t*)enc)[i] = BSWAP_DIGIT(x[i]);
    if (rem) {
        digit_t ld = BSWAP_DIGIT(x[ndigits]);
        memcpy(enc + ndigits*sizeof(digit_t), (unsigned char*)&ld, rem);
    }
#else    
    memcpy(enc, (const unsigned char*)x, nbytes);
#endif
}

static void decode_digits(digit_t* dec, const unsigned char* x, size_t nbytes, size_t ndigits)
{
    assert(nbytes <= ndigits*sizeof(digit_t));
    memcpy((unsigned char*)dec, x, nbytes);
    memset((unsigned char*)dec + nbytes, 0, ndigits*sizeof(*dec)-nbytes);

#ifdef TARGET_BIG_ENDIAN
    for (size_t i = 0; i < ndigits; i++)
        dec[i] = BSWAP_DIGIT(dec[i]);
#endif
}


#if defined(ENABLE_SIGN)
// ibz_t

static void ibz_encode(unsigned char *enc, const ibz_t *x, size_t nbytes)
{
#ifndef NDEBUG
{
    // make sure there is enough space
    ibz_t abs, bnd;
    ibz_init(&bnd);
    ibz_init(&abs);
    ibz_pow(&bnd, &ibz_const_two, 8*nbytes-1);
    ibz_abs(&abs, x);
    assert(ibz_cmp(&abs, &bnd) < 0);
    ibz_finalize(&bnd);
    ibz_finalize(&abs);
}
#endif
    const size_t digits = (nbytes + sizeof(digit_t) - 1) / sizeof(digit_t);
    digit_t d[digits];
    memset(d, 0, sizeof(d));
    if (ibz_cmp(x, &ibz_const_zero) >= 0) {
        // non-negative, straightforward.
        ibz_to_digits(d, x);
    }
    else {
        // negative; use two's complement.
        ibz_t tmp;
        ibz_init(&tmp);
        ibz_neg(&tmp, x);
        ibz_sub(&tmp, &tmp, &ibz_const_one);
        ibz_to_digits(d, &tmp);
        for (size_t i = 0; i < digits; ++i)
            d[i] = ~d[i];
#ifndef NDEBUG
{
    // make sure the result is correct
    ibz_t chk;
    ibz_init(&chk);
    ibz_copy_digit_array(&tmp, d);
    ibz_sub(&tmp, &tmp, x);
    ibz_pow(&chk, &ibz_const_two, 8*sizeof(d));
    assert(!ibz_cmp(&tmp, &chk));
    ibz_finalize(&chk);
}
#endif
        ibz_finalize(&tmp);
    }
    encode_digits(enc, d, nbytes);
}

static void ibz_decode(ibz_t* dec, const unsigned char *x, size_t nbytes)
{
    assert(nbytes > 0);
    const size_t ndigits = (nbytes + sizeof(digit_t) - 1) / sizeof(digit_t);
    assert(ndigits > 0);
    digit_t d[ndigits];
    memset(d, 0, sizeof(d));
    decode_digits(d, x, nbytes, ndigits);
    if (x[nbytes-1] >> 7) {
        // negative, decode two's complement
        const size_t s = sizeof(digit_t)-1 - (sizeof(d) - nbytes);
        assert(s < sizeof(digit_t));
        d[ndigits-1] |= ((digit_t) -1) >> 8*s << 8*s;
        for (size_t i = 0; i < ndigits; ++i)
            d[i] = ~d[i];
        ibz_copy_digits(dec, d, ndigits);
        ibz_add(dec, dec, &ibz_const_one);
        ibz_neg(dec, dec);
    }
    else {
        // non-negative
        ibz_copy_digits(dec, d, ndigits);
    }
}
#endif


// fp2_t

static void fp2_encode(const fp2_t* x, unsigned char *enc)
{
    fp2_t y;
    fp2_frommont(&y, x);
    // TODO: Check whether NWORDS_ORDER is the best way to define the length of the array
    digit_t a[NWORDS_ORDER];
    fp_to_digit_array(a, y.re);
    encode_digits(enc, a, FP2_ENCODED_BYTES / 2);
    fp_to_digit_array(a, y.im);
    encode_digits(enc + FP2_ENCODED_BYTES / 2, a, FP2_ENCODED_BYTES / 2);
}

void fp2_decode(const unsigned char *x, fp2_t *dec)
{
    // TODO: Check whether NWORDS_ORDER is the best way to define the length of the array
    digit_t a[NWORDS_ORDER];
    decode_digits(a, x, FP2_ENCODED_BYTES / 2, NWORDS_ORDER);
    fp_from_digit_array(dec->re, a);
    decode_digits(a, x + FP2_ENCODED_BYTES / 2, FP2_ENCODED_BYTES / 2, NWORDS_ORDER);
    fp_from_digit_array(dec->im, a);
    fp2_tomont(dec, dec);
}


// curves and points

static void proj_encode(fp2_t const *x, fp2_t const *z, unsigned char *enc)
{
    assert(!fp2_is_zero(z));
    fp2_t tmp = *z;
    fp2_inv(&tmp);
#ifndef NDEBUG
{
fp2_t chk;
fp2_mul(&chk, z, &tmp);
fp2_t one = {0};
fp_mont_setone(one.re);
assert(fp2_is_equal(&chk, &one));
}
#endif
    fp2_mul(&tmp, x, &tmp);
    fp2_encode(&tmp, enc);
}

static void proj_decode(const unsigned char *enc, fp2_t *x, fp2_t *z)
{
    fp2_decode(enc, x);
    fp_mont_setone(z->re);
    memset(z->im, 0, sizeof(z->im));
}

static void ec_curve_encode(const ec_curve_t* curve, unsigned char *enc)
{
    proj_encode(&curve->A, &curve->C, enc);
}

static void ec_curve_decode(const unsigned char *enc, ec_curve_t* curve)
{
    proj_decode(enc, &curve->A, &curve->C);
}
static void ec_point_encode(unsigned char *enc, const ec_point_t* point)
{
    proj_encode(&point->x, &point->z, enc);
}

static void ec_point_decode(ec_point_t* point, const unsigned char *enc)
{
    proj_decode(enc, &point->x, &point->z);
}

#if defined(ENABLE_SIGN)

static void ec_basis_encode(unsigned char *enc, const ec_basis_t* basis)
{
    ec_point_encode(enc, &basis->P);
    enc += EC_POINT_ENCODED_BYTES;
    ec_point_encode(enc, &basis->Q);
    enc += EC_POINT_ENCODED_BYTES;
    ec_point_encode(enc, &basis->PmQ);
}

static void ec_basis_decode(ec_basis_t* basis, const unsigned char *enc)
{
    ec_point_decode(&basis->P, enc);
    enc += EC_POINT_ENCODED_BYTES;
    ec_point_decode(&basis->Q, enc);
    enc += EC_POINT_ENCODED_BYTES;
    ec_point_decode(&basis->PmQ, enc);
}


// quat_alg_elem_t

static void quat_alg_elem_encode(unsigned char *enc, const quat_alg_elem_t* elt)
{
    ibz_encode(enc, &elt->denom, QUAT_ALG_ELEM_ENCODED_BYTES);
    enc += QUAT_ALG_ELEM_ENCODED_BYTES;
    ibz_encode(enc, &elt->coord[0], QUAT_ALG_ELEM_ENCODED_BYTES);
    enc += QUAT_ALG_ELEM_ENCODED_BYTES;
    ibz_encode(enc, &elt->coord[1], QUAT_ALG_ELEM_ENCODED_BYTES);
    enc += QUAT_ALG_ELEM_ENCODED_BYTES;
    ibz_encode(enc, &elt->coord[2], QUAT_ALG_ELEM_ENCODED_BYTES);
    enc += QUAT_ALG_ELEM_ENCODED_BYTES;
    ibz_encode(enc, &elt->coord[3], QUAT_ALG_ELEM_ENCODED_BYTES);
}

static void quat_alg_elem_decode(quat_alg_elem_t* elt, const unsigned char *enc)
{
    ibz_decode(&elt->denom, enc, QUAT_ALG_ELEM_ENCODED_BYTES);
    enc += QUAT_ALG_ELEM_ENCODED_BYTES;
    ibz_decode(&elt->coord[0], enc, QUAT_ALG_ELEM_ENCODED_BYTES);
    enc += QUAT_ALG_ELEM_ENCODED_BYTES;
    ibz_decode(&elt->coord[1], enc, QUAT_ALG_ELEM_ENCODED_BYTES);
    enc += QUAT_ALG_ELEM_ENCODED_BYTES;
    ibz_decode(&elt->coord[2], enc, QUAT_ALG_ELEM_ENCODED_BYTES);
    enc += QUAT_ALG_ELEM_ENCODED_BYTES;
    ibz_decode(&elt->coord[3], enc, QUAT_ALG_ELEM_ENCODED_BYTES);
}
#endif


// compressed isogeny chains

static void id2iso_compressed_long_two_isog_encode(const id2iso_compressed_long_two_isog_t* isog, unsigned char *enc)
{
unsigned char *const start = enc;
    assert(isog->length == ZIP_CHAIN_LEN);
    for (int i = 0; i < ZIP_CHAIN_LEN; ++i) {
        encode_digits(enc, isog->zip_chain[i], ID2ISO_COMPRESSED_LONG_TWO_ISOG_ZIP_CHAIN_BYTES);
        enc += ID2ISO_COMPRESSED_LONG_TWO_ISOG_ZIP_CHAIN_BYTES;
    }
    *enc++ = isog->bit_first_step;
assert(enc - start == ID2ISO_COMPRESSED_LONG_TWO_ISOG_BYTES);
}

static void id2iso_compressed_long_two_isog_decode(const unsigned char *enc, id2iso_compressed_long_two_isog_t* isog)
{
const unsigned char *const start = enc;
    for (int i = 0; i < ZIP_CHAIN_LEN; ++i) {
        decode_digits(isog->zip_chain[i], enc, ID2ISO_COMPRESSED_LONG_TWO_ISOG_ZIP_CHAIN_BYTES, NWORDS_ORDER);
        enc += ID2ISO_COMPRESSED_LONG_TWO_ISOG_ZIP_CHAIN_BYTES;
    }
    isog->bit_first_step = *enc++;
assert(enc - start == ID2ISO_COMPRESSED_LONG_TWO_ISOG_BYTES);
}

// uncompressed isogeny chains

static void uncompressed_long_two_isog_encode(const ec_point_t* kernel_points, unsigned char *enc)
{
unsigned char *const start = enc;
    for (int i = 0; i < ZIP_CHAIN_LEN; ++i) {
        ec_point_encode(enc, &kernel_points[i]);
        enc += EC_POINT_ENCODED_BYTES;
    }
assert(enc - start == UNCOMPRESSED_LONG_TWO_ISOG_ENCODED_BYTES);
}

static void uncompressed_long_two_isog_decode(const unsigned char *enc, ec_point_t* kernel_points)
{
    const unsigned char *const start = enc;
    for (int i = 0; i < ZIP_CHAIN_LEN; ++i) {
        ec_point_decode(&kernel_points[i], enc);
        enc += EC_POINT_ENCODED_BYTES;
    }
assert(enc - start == UNCOMPRESSED_LONG_TWO_ISOG_ENCODED_BYTES);
}


// public API


/**
 * @brief Encodes a public key to a byte array
 *
 * @param enc Output: encoded public key
 * @param pk: the public key
 */
void public_key_encode(unsigned char *enc, const public_key_t* pk)
{
    ec_curve_encode(&pk->E, enc);
}

/**
 * @brief Decodes a public key from a byte array
 *
 * @param pk Output: the decoded public key
 * @param enc: encoded public key
 */
void public_key_decode(public_key_t* pk, const unsigned char *enc)
{
    ec_curve_decode(enc, &pk->E);
}

/**
 * @brief Encodes a smart-smaplic-based public key to a byte array
 *
 * @param enc Output: encoded public key
 * @param pk: the public key
 */
void public_key_encode_smart(unsigned char *enc, const public_key_smart_t* pk)
{
    ec_point_encode(enc, &pk->Pa);
}
/**
 * @brief Decodes a smart-sampling-based public key from a byte array
 *
 * @param pk Output: the decoded public key
 * @param enc: encoded public key
 */
void public_key_decode_smart(public_key_smart_t* pk, const unsigned char *enc)
{
    ec_point_decode(&pk->Pa, enc);
}


#if defined(ENABLE_SIGN)
/**
 * @brief Encodes a secret key to a byte array
 *
 * @param enc Output: encoded secret key
 * @param sk: the secret key
 * @param pk: the associated public key
 */
void secret_key_encode(unsigned char *enc, const secret_key_t* sk, const public_key_t* pk)
{
unsigned char *const start = enc;
#ifndef NDEBUG
{
fp2_t lhs, rhs;
fp2_mul(&lhs, &sk->curve.A, &pk->E.C);
fp2_mul(&rhs, &sk->curve.C, &pk->E.A);
assert(fp2_is_equal(&lhs, &rhs));
}
#endif
    ec_curve_encode(&sk->curve, enc);
    enc += EC_CURVE_ENCODED_BYTES;
    quat_alg_elem_encode(enc, &sk->gen_two);
    enc += 5*QUAT_ALG_ELEM_ENCODED_BYTES;
    ec_point_encode(enc, &sk->kernel_dual);
    enc += EC_POINT_ENCODED_BYTES;
    ec_basis_encode(enc, &sk->basis_plus);
    enc += EC_BASIS_ENCODED_BYTES;
    ec_basis_encode(enc, &sk->basis_minus);
    enc += EC_BASIS_ENCODED_BYTES;
assert(enc - start == SECRETKEY_BYTES);
}

static void secret_key_populate_full(secret_key_t* sk);

/**
 * @brief Decodes a secret key from a byte array
 *
 * @param sk Output: the decoded secret key
 * @param pk Output: the associated public key
 * @param enc: encoded secret key
 */
void secret_key_decode(secret_key_t* sk, public_key_t* pk, const unsigned char *enc)
{
const unsigned char *const start = enc;
    ec_curve_decode(enc, &sk->curve);
    enc += EC_CURVE_ENCODED_BYTES;
    quat_alg_elem_decode(&sk->gen_two, enc);
    enc += 5*QUAT_ALG_ELEM_ENCODED_BYTES;
    ec_point_decode(&sk->kernel_dual, enc);
    enc += EC_POINT_ENCODED_BYTES;
    ec_basis_decode(&sk->basis_plus, enc);
    enc += EC_BASIS_ENCODED_BYTES;
    ec_basis_decode(&sk->basis_minus, enc);
    enc += EC_BASIS_ENCODED_BYTES;
    pk->E = sk->curve;
assert(enc - start == SECRETKEY_BYTES);

    secret_key_populate_full(sk);
}

/**
 * Fully populates the decoded key
 */
static void secret_key_populate_full(secret_key_t* sk)
{
    // have: curve, gen_two, kernel_dual, basis_plus, basis_minus
    // want: order, lideal_small, lideal_two

    ibz_t norm_two, norm_small;
    ibz_init(&norm_two);
    ibz_init(&norm_small);

    ibz_pow(&norm_two, &ibz_const_two, KLPT_keygen_length);
    {
        ibq_t norm;
        ibq_init(&norm);
        ibz_t tmp;
        ibz_init(&tmp);
        quat_alg_norm(&norm, &sk->gen_two, &QUATALG_PINFTY);
        int r = ibq_to_ibz(&tmp, &norm);
        assert(r);
        ibz_div(&norm_small, &tmp, &tmp, &norm_two);
        assert(ibz_is_zero(&tmp));
        ibq_finalize(&norm);
        ibz_finalize(&tmp);
    }

    quat_lideal_make_primitive_then_create(&sk->lideal_two, &sk->gen_two, &norm_two, &STANDARD_EXTREMAL_ORDER.order, &QUATALG_PINFTY);

    quat_alg_elem_t conj;
    quat_alg_elem_init(&conj);
    quat_alg_conj(&conj, &sk->gen_two);
    quat_lideal_make_primitive_then_create(&sk->lideal_small, &conj, &norm_small, &STANDARD_EXTREMAL_ORDER.order, &QUATALG_PINFTY);
    quat_alg_elem_finalize(&conj);

    ibz_finalize(&norm_small);
    ibz_finalize(&norm_two);
}
#endif


/**
 * @brief Encodes a signature to a byte array
 *
 * @param enc Output: encoded signature
 * @param sig: the signature
 */
void signature_encode(unsigned char* enc, const signature_t* sig)
{
unsigned char *const start = enc;
    id2iso_compressed_long_two_isog_encode(&sig->zip, enc);
    enc += ID2ISO_COMPRESSED_LONG_TWO_ISOG_BYTES;
    encode_digits(enc, sig->r, TORSION_23POWER_BYTES);
    enc += TORSION_23POWER_BYTES;
    *enc++ = sig->s.select23;
    encode_digits(enc, sig->s.scalar2, TORSION_2POWER_BYTES);
    enc += TORSION_2POWER_BYTES;
    encode_digits(enc, sig->s.scalar3, TORSION_3POWER_BYTES);
    enc += TORSION_3POWER_BYTES;
assert(enc - start == SIGNATURE_LEN);
}

/**
 * @brief Decodes a signature from a byte array
 *
 * @param sig Output: the decoded signature
 * @param enc: encoded signature
 */
void signature_decode(signature_t* sig, const unsigned char* enc)
{
const unsigned char *const start = enc;
    id2iso_compressed_long_two_isog_decode(enc, &sig->zip);
    enc += ID2ISO_COMPRESSED_LONG_TWO_ISOG_BYTES;
    memset(&sig->r, 0, sizeof(sig->r));
    decode_digits(sig->r, enc, TORSION_23POWER_BYTES, sizeof(sig->r)/sizeof(*sig->r));
    enc += TORSION_23POWER_BYTES;
    memset(&sig->s, 0, sizeof(sig->s));
    sig->s.select23 = *enc++;
    decode_digits(sig->s.scalar2, enc, TORSION_2POWER_BYTES, sizeof(sig->s.scalar2)/sizeof(*sig->s.scalar2));
    enc += TORSION_2POWER_BYTES;
    decode_digits(sig->s.scalar3, enc, TORSION_3POWER_BYTES, sizeof(sig->s.scalar3)/sizeof(*sig->s.scalar3));
    enc += TORSION_3POWER_BYTES;
assert(enc - start == SIGNATURE_LEN);
}

/**
 * @brief Encodes a smart-smapling-based signature to a byte array
 *
 * @param enc Output: encoded signature
 * @param sig: the signature
 */
void signature_encode_smart(unsigned char* enc, const signature_t* sig)
{
unsigned char *const start = enc;
    id2iso_compressed_long_two_isog_encode(&sig->zip, enc);
    enc += ID2ISO_COMPRESSED_LONG_TWO_ISOG_BYTES;
    encode_digits(enc, sig->r, POWER_OF_2_SECPAR/8);
    enc += POWER_OF_2_SECPAR/8;
    encode_digits(enc, sig->s.scalar2, POWER_OF_2_SECPAR/8);
    enc += POWER_OF_2_SECPAR/8;
assert(enc - start == SMART_SIGNATURE_LEN);
}

/**
 * @brief Encodes a signature to a byte array with uncompressed points
 *
 * @param enc Output: encoded signature
 * @param sig: the signature
 */
void signature_encode_uncompressed(unsigned char* enc, const signature_uncompressed_t* sig)
{
unsigned char *const start = enc;
    uncompressed_long_two_isog_encode(sig->kernel_points, enc);
    enc += UNCOMPRESSED_LONG_TWO_ISOG_ENCODED_BYTES;
    ec_curve_encode(&sig->E_COM, enc);
    enc += EC_CURVE_ENCODED_BYTES;
assert(enc - start == UNCOMPRESSED_SIGNATURE_LEN);
}

/**
 * @brief Encodes a parallel-friendly signature to a byte array
 *
 * @param enc Output: encoded signature
 * @param sig: the signature
 */
void signature_encode_parallel(unsigned char* enc, const signature_parallel_t* sig)
{
unsigned char *const start = enc;
    ec_point_encode(enc, &sig->kernel_points[0]);
    enc += EC_POINT_ENCODED_BYTES;
    ec_point_encode(enc, &sig->kernel_points[1]);
    enc += EC_POINT_ENCODED_BYTES;
    ec_point_encode(enc, &sig->kernel_points[2]);
    enc += EC_POINT_ENCODED_BYTES;
    ec_point_encode(enc, &sig->kernel_points[3]);
    enc += EC_POINT_ENCODED_BYTES;
    ec_curve_encode(&sig->E_COM, enc);
    enc += EC_CURVE_ENCODED_BYTES;
    ec_curve_encode(&sig->E_1, enc);
    enc += EC_CURVE_ENCODED_BYTES;
    ec_curve_encode(&sig->E_3, enc);
    enc += EC_CURVE_ENCODED_BYTES;
assert(enc - start == PARALLEL_SIGNATURE_LEN);
}

/**
 * @brief Encodes a parallel-friendly smart-smapling-based signature to a byte array
 *
 * @param enc Output: encoded signature
 * @param sig: the signature
 */
void signature_encode_cparallel(unsigned char* enc, const signature_cparallel_t* sig)
{
unsigned char *const start = enc;
    id2iso_compressed_long_two_isog_encode(&sig->zip, enc);
    enc += ID2ISO_COMPRESSED_LONG_TWO_ISOG_BYTES;
    encode_digits(enc, sig->r, POWER_OF_2_SECPAR/8);
    enc += POWER_OF_2_SECPAR/8;
    encode_digits(enc, sig->s, POWER_OF_2_SECPAR/8);
    enc += POWER_OF_2_SECPAR/8;
    ec_point_encode(enc, &sig->alphas[0]);
    enc += EC_POINT_ENCODED_BYTES;
    ec_point_encode(enc, &sig->alphas[1]);
    enc += EC_POINT_ENCODED_BYTES;
assert(enc - start == COMPRESSED_PARALLEL_SIGNATURE_LEN);
}

/**
 * @brief Decodes a smart-sampling-based signature from a byte array
 *
 * @param sig Output: the decoded signature
 * @param enc: encoded signature
 */
void signature_decode_smart(signature_t* sig, const unsigned char* enc)
{
const unsigned char *const start = enc;
    id2iso_compressed_long_two_isog_decode(enc, &sig->zip);
    enc += ID2ISO_COMPRESSED_LONG_TWO_ISOG_BYTES;
    memset(&sig->r, 0, sizeof(sig->r));
    decode_digits(sig->r, enc, POWER_OF_2_SECPAR/8, sizeof(sig->r)/sizeof(*sig->r));
    enc += POWER_OF_2_SECPAR/8;
    memset(&sig->s, 0, sizeof(sig->s));
    decode_digits(sig->s.scalar2, enc, POWER_OF_2_SECPAR/8, sizeof(sig->s.scalar2)/sizeof(*sig->s.scalar2));
    enc += POWER_OF_2_SECPAR/8;
assert(enc - start == SMART_SIGNATURE_LEN);
}

/**
 * @brief Decodes a signature with uncompressed points from a byte array
 *
 * @param sig Output: the decoded signature
 * @param enc: encoded signature
 */
void signature_decode_uncompressed(signature_uncompressed_t* sig, const unsigned char* enc)
{
const unsigned char *const start = enc;
    uncompressed_long_two_isog_decode(enc, sig->kernel_points);
    enc += UNCOMPRESSED_LONG_TWO_ISOG_ENCODED_BYTES;
    memset(&sig->E_COM, 0, sizeof(ec_point_t));
    ec_curve_decode(enc, &sig->E_COM);
    enc += EC_CURVE_ENCODED_BYTES;
assert(enc - start == UNCOMPRESSED_SIGNATURE_LEN);
}

/**
 * @brief Decodes a parallel-friendly signature from a byte array
 *
 * @param sig Output: the decoded signature
 * @param enc: encoded signature
 */
void signature_decode_parallel(signature_parallel_t* sig, const unsigned char* enc)
{
const unsigned char *const start = enc;
    ec_point_decode(&sig->kernel_points[0], enc);
    enc += EC_POINT_ENCODED_BYTES;
    ec_point_decode(&sig->kernel_points[1], enc);
    enc += EC_POINT_ENCODED_BYTES;
    ec_point_decode(&sig->kernel_points[2], enc);
    enc += EC_POINT_ENCODED_BYTES;
    ec_point_decode(&sig->kernel_points[3], enc);
    enc += EC_POINT_ENCODED_BYTES;
    ec_curve_decode(enc, &sig->E_COM);
    enc += EC_CURVE_ENCODED_BYTES;
    ec_curve_decode(enc, &sig->E_1);
    enc += EC_CURVE_ENCODED_BYTES;
    ec_curve_decode(enc, &sig->E_3);
    enc += EC_CURVE_ENCODED_BYTES;
assert(enc - start == PARALLEL_SIGNATURE_LEN);
}

/**
 * @brief Decodes a parallel-friendly smart-sampling-based signature with from a byte array
 *
 * @param sig Output: the decoded signature
 * @param enc: encoded signature
 */
void signature_decode_cparallel(signature_cparallel_t* sig, const unsigned char* enc)
{
const unsigned char *const start = enc;
    id2iso_compressed_long_two_isog_decode(enc, &sig->zip);
    enc += ID2ISO_COMPRESSED_LONG_TWO_ISOG_BYTES;
    memset(&sig->r, 0, sizeof(sig->r));
    decode_digits(sig->r, enc, POWER_OF_2_SECPAR/8, sizeof(sig->r)/sizeof(*sig->r));
    enc += POWER_OF_2_SECPAR/8;
    memset(&sig->s, 0, sizeof(sig->s));
    decode_digits(sig->s, enc, POWER_OF_2_SECPAR/8, sizeof(sig->s)/sizeof(*sig->s));
    enc += POWER_OF_2_SECPAR/8;
    ec_point_decode(&sig->alphas[0], enc);
    enc += EC_POINT_ENCODED_BYTES;
    ec_point_decode(&sig->alphas[1], enc);
    enc += EC_POINT_ENCODED_BYTES;
assert(enc - start == COMPRESSED_PARALLEL_SIGNATURE_LEN);
}



#include <fips202.h>

#if defined(PQM4)
#define SHAKE256 shake256
#endif

void hash_to_challenge(digit_vec_2_t *scalars, const ec_curve_t *curve, const unsigned char *message, size_t length)
{
    unsigned char buf[FP2_ENCODED_BYTES];
    {
        fp2_t j;
        ec_j_inv(&j, curve);
        fp2_encode(&j, buf);
    }

    //FIXME omits some vectors, notably (a,1) with gcd(a,6)!=1 but also things like (2,3).
    {
        //The code below implicitly assumes a little-endian ordering; check when working on big-endian version

        //FIXME should use SHAKE128 for smaller parameter sets?
        shake256incctx ctx;

        shake256_inc_init(&ctx);
        shake256_inc_absorb(&ctx, buf, FP2_ENCODED_BYTES);
        shake256_inc_absorb(&ctx, message, length);
        shake256_inc_finalize(&ctx);
        shake256_inc_squeeze((void *) (*scalars)[1], NWORDS_ORDER * sizeof(digit_t), &ctx);

        //FIXME
        memset((*scalars)[0], 0, NWORDS_ORDER * sizeof(digit_t));
        (*scalars)[0][0] = 1;
    }

#ifndef NDEBUG
#if defined(ENABLE_SIGN)
{
ibz_t gcd;
ibz_init(&gcd);
ibz_vec_2_t scalars_ibz;
ibz_vec_2_init(&scalars_ibz);
ibz_copy_digit_array(&scalars_ibz[0], (*scalars)[0]);
ibz_copy_digit_array(&scalars_ibz[1], (*scalars)[1]);
ibz_set(&gcd, 6);
ibz_gcd(&gcd, &gcd, &scalars_ibz[0]);
ibz_gcd(&gcd, &gcd, &scalars_ibz[1]);
assert(ibz_is_one(&gcd));
ibz_finalize(&gcd);
ibz_vec_2_finalize(&scalars_ibz);
}
#endif
#endif
}

void hash_to_challenge_smart(ec_point_t *output, const ec_curve_t *curve, const unsigned char *message, size_t length)
{
    unsigned char buf[FP2_ENCODED_BYTES];
    digit_t scalar[NWORDS_ORDER];
    fp2_t j;
    ec_point_t A24;
    shake256incctx ctx;

    // Curve coefficient in the form A24 = (A+2C:4C)
    fp2_add(&A24.z, &curve->C, &curve->C);
    fp2_add(&A24.x, &curve->A, &A24.z);
    fp2_add(&A24.z, &A24.z, &A24.z);

    ec_j_inv(&j, curve);
    fp2_encode(&j, buf);

    fp_mont_setone(output->z.re);
    fp_set(output->z.im, 0);
    
    uint8_t ctr = 0;
    while(1)
    {
        shake256_inc_init(&ctx);
        shake256_inc_absorb(&ctx, buf, FP2_ENCODED_BYTES);
        shake256_inc_absorb(&ctx, message, length);
        shake256_inc_absorb(&ctx, &ctr, 1);
        shake256_inc_finalize(&ctx);
        shake256_inc_squeeze((void *) scalar, NWORDS_ORDER * sizeof(digit_t), &ctx);

        fp_from_digit_array(output->x.re, scalar);
        fp2_tomont(&output->x, &output->x);
        fp_set(output->x.im, 0);
        for(int i = 0; i < NONRES[0]; i++){
            fp_add(output->x.im, output->x.im, output->x.re);
        }
        if(ec_is_on_curve(curve, output))
        {
            break;
        }
        ctr++;
    }

    // Clear cofactors
#if POWER_OF_3 > 0
    ec_point_t A3;
    fp2_copy(&A3.x, &A24.x);
    fp2_sub(&A3.z, &A24.x, &A24.z);
    for(int i = 0; i < POWER_OF_3; i++){
        xTPL(output, output, &A3);
    }
#endif

    for(int i = (POWER_OF_3 > 0) & 1; i < P_LEN; i++){
        for(int j = 0; j < TORSION_PLUS_ODD_POWERS[i]; j++){
            xMULdac(output, output, DACS[i], DAC_LEN[i], &A24);
        }
    }

    for(int i = 0; i < POWER_OF_2-POWER_OF_2_SECPAR; i++)
    {
        xDBLv2(output, output, &A24);
    }
}

void hash_to_challenge_parallel(ec_point_t *output, const ec_curve_t *curve, const unsigned char *message, size_t length)
{
    unsigned char buf[FP2_ENCODED_BYTES];
    digit_t scalar[NWORDS_ORDER];
    fp2_t j;
    ec_point_t A24;
    shake256incctx ctx;

    // Curve coefficient in the form A24 = (A+2C:4C)
    fp2_add(&A24.z, &curve->C, &curve->C);
    fp2_add(&A24.x, &curve->A, &A24.z);
    fp2_add(&A24.z, &A24.z, &A24.z);

    ec_j_inv(&j, curve);
    fp2_encode(&j, buf);

    fp_mont_setone(output->z.re);
    fp_set(output->z.im, 0);
    
    uint8_t ctr = 0;
    while(1)
    {
        shake256_inc_init(&ctx);
        shake256_inc_absorb(&ctx, buf, FP2_ENCODED_BYTES);
        shake256_inc_absorb(&ctx, message, length);
        shake256_inc_absorb(&ctx, &ctr, 1);
        shake256_inc_finalize(&ctx);
        shake256_inc_squeeze((void *) scalar, NWORDS_ORDER * sizeof(digit_t), &ctx);

        fp_from_digit_array(output->x.re, scalar);
        fp2_tomont(&output->x, &output->x);
        fp_set(output->x.im, 0);
        for(int i = 0; i < NONRES[0]; i++){
            fp_add(output->x.im, output->x.im, output->x.re);
        }
        if(ec_is_on_curve(curve, output))
        {
            break;
        }
        ctr++;
    }

    // Clear cofactors
#if POWER_OF_3 > 0
    ec_point_t A3;
    fp2_copy(&A3.x, &A24.x);
    fp2_sub(&A3.z, &A24.x, &A24.z);
    for(int i = 0; i < POWER_OF_3; i++){
        xTPL(output, output, &A3);
    }
#endif

    for(int i = (POWER_OF_3 > 0) & 1; i < P_LEN; i++){
        for(int j = 0; j < TORSION_PLUS_ODD_POWERS[i]; j++){
            xMULdac(output, output, DACS[i], DAC_LEN[i], &A24);
        }
    }
    xDBLv2(output, output, &A24);
}