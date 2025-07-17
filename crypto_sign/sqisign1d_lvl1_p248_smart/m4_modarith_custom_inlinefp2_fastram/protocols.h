/** @file
 * 
 * @authors Antonin Leroux
 * 
 * @brief The protocols
 */

#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <klpt_constants.h>
#include <torsion_constants.h>
#include <encoded_sizes.h>
#include <fp_constants.h>

#include <stdio.h>
#include <id2iso.h>

#if defined(ENABLE_SIGN)
#include <intbig.h>
#include <quaternion.h>
#include <quaternion_data.h>
#include <rng.h>
#include <endomorphism_action.h>

#include <klpt.h>
#endif

/** @defgroup protocols_protocols SQIsign protocols
 * @{
*/
/** @defgroup protocols_t Types for sqisign protocols
 * @{
*/

/** @brief Type for the signature
 * 
 * @typedef signature_t
 * 
 * @struct signature
 * 
*/


typedef struct signature {
    id2iso_compressed_long_two_isog_t zip;  /// the compressed isogeny 
    digit_t r[NWORDS_ORDER];          /// first scalar encoding the challenge
    struct {
        unsigned char select23; // this&1 is bit2, this&2 is bit3
        digit_t scalar2[NWORDS_ORDER];
        digit_t scalar3[NWORDS_ORDER];
    } s;                                    /// second scalar encoding the challenge
} signature_t;

typedef struct signature_uncompressed {
    ec_point_t kernel_points[ZIP_CHAIN_LEN]; // x coordinate for the generator of each block of the response isogeny
    ec_curve_t E_COM; // Montgomery coefficient of the commitment curve
} signature_uncompressed_t;

typedef struct signature_parallel {
    ec_point_t kernel_points[ZIP_CHAIN_LEN]; // x coordinate for the generator of each block of the response isogeny
    ec_curve_t E_COM; // Montgomery coefficient of the commitment curve
    ec_curve_t E_1; // Montgomery coefficient of the first intermediate curve in the response isogeny chain
    ec_curve_t E_3; // Montgomery coefficient of the third intermediate curve in the response isogeny chain
} signature_parallel_t;

typedef struct signature_cparallel {
    id2iso_compressed_long_two_isog_t zip;  /// the compressed isogeny 
    digit_t r[NWORDS_ORDER];  
    digit_t s[NWORDS_ORDER];  
    ec_point_t alphas[2];
} signature_cparallel_t;

/** @brief Type for the regular public keys
 * 
 * @typedef public_key_t
 * 
 * @struct public_key
 * 
*/
typedef struct public_key {
	ec_curve_t E; /// the normalized A coefficient of the Montgomery curve
} public_key_t;

/** @brief Type for the smart-sampling-based public keys
 * 
 * @typedef public_key_t
 * 
 * @struct public_key
 * 
*/
typedef struct public_key_smart {
	ec_point_t Pa; /// A non-(0,0) point of order 2 in the public curve
} public_key_smart_t;

#if defined(ENABLE_SIGN)
/** @brief Type for the secret keys
 * 
 * @typedef secret_key_t
 * 
 * @struct secret_key
 * 
*/
typedef struct secret_key {
	ec_curve_t curve; /// the j-invariant
    quat_left_ideal_t lideal_small; /// ideal of prime norm
    quat_left_ideal_t lideal_two; /// ideal of norm a power of 2
    quat_alg_elem_t gen_two; /// generator of lideal_two
    ec_point_t kernel_dual; /// kernel of the dual of the last step of the phi_two  
    ec_basis_t basis_plus; /// basis pushed through phi_two
    ec_basis_t basis_minus; /// basis pushed through phi_two
} secret_key_t;
#endif

/** @brief Type for vector of 2 scalars
 * 
 * @typedef digit_vec_2_t
*/
typedef digit_t digit_vec_2_t[2][NWORDS_ORDER];

/** @}
*/


/*************************** Functions *****************************/


#if defined(ENABLE_SIGN)
void secret_key_init(secret_key_t *sk);
void secret_key_finalize(secret_key_t *sk);
#endif

static inline void signature_init(signature_t *sig)
{
    id2iso_compressed_long_two_isog_init(&sig->zip, SQISIGN_signing_length);
}

static inline void signature_finalize(signature_t *sig)
{
    id2iso_compressed_long_two_isog_finalize(&sig->zip);
}

/** @defgroup signature The signature protocol
 * @{
*/

#if defined(ENABLE_SIGN)
/**
 * @brief Computing a key pair
 *
 * @param pk : Output the public key 
 * @param sk : Output the secret key
 * @returns a bit indicating if the computation succeeded  
    */
int protocols_keygen(public_key_t *pk, secret_key_t *sk);


/**
 * @brief Computing a commitment isogeny from the starting curve
 *
 * @param ideal Output: the left O0-ideal defining the commitment isogeny
 * @param E1 Output: the codomain curve, normalized à la ec_curve_normalize()
 * @param basis Input and output: a basis on E0 which will be pushed through the commitment i
sogeny
 */
void protocols_commit(quat_left_ideal_t *ideal, ec_curve_t *E1, ec_basis_t *basis);
#endif

/**
 * @brief Hashing a commitment curve and a message to a challenge
 *
 * @param scalars Output: scalars defining the challenge kernel in terms of a deterministic t
orsion basis of the commitment curve
 * @param curve: the commitment curve
 * @param message: byte string to be signed
 * @param length: length of the message
 */
void hash_to_challenge(digit_vec_2_t *scalars, const ec_curve_t *curve, const unsigned char *message, size_t length);

/**
 * @brief Hashing a commitment curve and a message directly to a kernel point.
 *          Not compatible with NIST round 1 KATs
 *
 * @param output: the computed kernel point
 * @param curve: the commitment curve
 * @param message: byte string to be signed
 * @param length: length of the message
 */
void hash_to_challenge_smart(ec_point_t *output, const ec_curve_t *curve, const unsigned char *message, size_t length);

/**
 * @brief Same as hash_to_challenge_smart but returns a point of order 2^(f-1) laying over the intended kernel point of order 2^lambda
 *
 * @param output: the computed kernel point
 * @param curve: the commitment curve
 * @param message: byte string to be signed
 * @param length: length of the message
 */
void hash_to_challenge_parallel(ec_point_t *output, const ec_curve_t *curve, const unsigned char *message, size_t length);


#if defined(ENABLE_SIGN)
/**
 * @brief Computing a challenge isogeny from a commitment curve
 *
 * @param ideal Output: the left O0-ideal defining the challenge isogeny (pulled back through
 the commitment)
 * @param sig Output: signature structure in which the members r and s will be filled by this
 function
 * @param E1: commitment curve
 * @param pushedbasis6: image of the precomputed fixed torsion basis #BASIS_CHALLENGE under t
he commitment isogeny
 * @param hash: message hash according to hash_to_challenge()
 * @param out_E2: optional output (can be NULL) to store the value of the challenge curve for
 testing and debugging
 */
void protocols_challenge(quat_left_ideal_t *ideal, signature_t *sig, const ec_curve_t *E1, const ec_basis_t *pushedbasis6, const digit_vec_2_t *hash, ec_curve_t *out_E2);


/**
 * @brief Computing a signature
 *
 * @param sig Output: the signature
 * @param pk the public key 
 * @param sk the secret key
 * @param m the message
 * @param l the message length
 * @returns a bit indicating if the computation succeeded  
    */
int protocols_sign(signature_t *sig,const public_key_t *pk, const secret_key_t *sk,const unsigned char* m, size_t l);
#endif


/** @}
*/


/** @defgroup verification The verification protocol
 * @{
*/


/**
 * @brief Recovering the challenge curve from a signature
 *
 * @param E2 Output: the challenge curve, will be normalized à la ec_curve_normalize()
 * @param dual Output: point defining the first step of the dual of the response isogeny
 * @param sig the signature
 * @param pk the public key
 */
void protocols_verif_unpack_chall(ec_curve_t *E2, ec_point_t *dual, const signature_t *sig, const public_key_t *pk);

/**
 * @brief Verifying the challenge from a signature
 *
 * @param sig the signature
 * @param E2 the challenge curve
 * @param dual point defining the first step of the dual of the response isogeny
 * @param m the message
 * @param l the message length
 * @returns a single bit indicating whether the given data forms correct signature data
 */
int protocols_verif_from_chall(const signature_t *sig, ec_curve_t const *E2, const ec_point_t *dual, const unsigned char* m, size_t l);


/**
 * @brief Verifying a signature
 *
 * @param sig: the signature
 * @param pk the public key 
 * @param m the message
 * @param l length of the message
 * @returns a bit indicating if the verification succeeded  
    */
int protocols_verif(const signature_t *sig,const public_key_t *pk,const unsigned char* m, size_t l);

/**
 * @brief Verifying a smart-sampling-based signature
 *
 * @param sig: the signature
 * @param pk the public key 
 * @param m the message
 * @param l length of the message
 * @returns a bit indicating if the verification succeeded  
    */
int protocols_verif_smart(const signature_t *sig, const public_key_smart_t *pk, const unsigned char* m, size_t l);

/**
 * @brief Verifying a signature with uncompressed points
 *
 * @param sig: the signature
 * @param pk the public key 
 * @param m the message
 * @param l length of the message
 * @returns a bit indicating if the verification succeeded  
    */
int protocols_verif_uncompressed(const signature_uncompressed_t *sig, const public_key_t *pk, const unsigned char* m, size_t l);

/**
 * @brief Verifying a parallel-friendly signature
 *
 * @param sig: the signature
 * @param pk the public key 
 * @param m the message
 * @param l length of the message
 * @returns a bit indicating if the verification succeeded  
    */
int protocols_verif_parallel(const signature_parallel_t *sig, const public_key_t *pk, const unsigned char* m, size_t l);

/**
 * @brief Verifying a parallel-friendly smart-sampling-based signature
 *
 * @param sig: the signature
 * @param pk the public key 
 * @param m the message
 * @param l length of the message
 * @returns a bit indicating if the verification succeeded  
    */
int protocols_verif_cparallel(const signature_cparallel_t *sig, const public_key_smart_t *pk, const unsigned char* m, size_t l);


/** @}
*/


/*************************** Encoding *****************************/


/** @defgroup encoding Encoding and decoding functions
 * @{
*/

/**
 * @brief Encodes a public key as a byte array
 *
 * @param enc : Output the encoded public key
 * @param pk : public key
    */
void public_key_encode(unsigned char *enc, const public_key_t* pk);

/**
 * @brief Encodes a smart-smapling-based public key as a byte array
 *
 * @param enc : Output the encoded public key
 * @param pk : public key
    */
void public_key_encode_smart(unsigned char *enc, const public_key_smart_t* pk);

#if defined(ENABLE_SIGN)
/**
 * @brief Encodes a secret key as a byte array
 *
 * @param enc : Output the encoded secret key (including public key)
 * @param sk : secret key
 * @param pk : public key
  */
void secret_key_encode(unsigned char *enc, const secret_key_t* sk, const public_key_t* pk);
#endif

/**
 * @brief Encodes a signature as a byte array
 *
 * @param enc : Output the encoded signature
 * @param sig : signature
    */
void signature_encode(unsigned char* enc, const signature_t* sig);

/**
 * @brief Encodes a smart-sampling-based signature as a byte array
 *
 * @param enc : Output the encoded signature
 * @param sig : signature
    */
void signature_encode_smart(unsigned char* enc, const signature_t* sig);

/**
 * @brief Encodes a signature as a byte array with uncompressed points
 *
 * @param enc : Output the encoded signature
 * @param sig : signature
    */
void signature_encode_uncompressed(unsigned char* enc, const signature_uncompressed_t* sig);

/**
 * @brief Encodes a parallel-friendly signature as a byte array
 *
 * @param enc : Output the encoded signature
 * @param sig : signature
    */
void signature_encode_parallel(unsigned char* enc, const signature_parallel_t* sig);

/**
 * @brief Encodes a parallel-friendly smart-sampling-based signature as a byte array
 *
 * @param enc : Output the encoded signature
 * @param sig : signature
    */
void signature_encode_cparallel(unsigned char* enc, const signature_cparallel_t* sig);

/**
 * @brief Decodes a public key from a byte array
 *
 * @param pk : Output the decoded public key
 * @param enc : encoded public key
    */
void public_key_decode(public_key_t* pk, const unsigned char *enc);

/**
 * @brief Decodes a smart-sampling-based public key from a byte array
 *
 * @param pk : Output the decoded public key
 * @param enc : encoded public key
    */
void public_key_decode_smart(public_key_smart_t* pk, const unsigned char *enc);

#if defined(ENABLE_SIGN)
/**
 * @brief Encodes a secret key (and public key) as a byte array
 *
 * @param sk : Output the decoded secret key
 * @param pk : Output the decoded public key contained in enc
 * @param enc : encoded secret key
    */
void secret_key_decode(secret_key_t* sk, public_key_t* pk, const unsigned char *enc);
#endif

/**
 * @brief Decodes a signature from a byte array
 *
 * @param sig : Output the decoded signature
 * @param enc : encoded signature
    */
void signature_decode(signature_t* sig, const unsigned char* enc);

/**
 * @brief Decodes a smart-sampling-based signature from a byte array
 *
 * @param sig : Output the decoded signature
 * @param enc : encoded signature
    */
void signature_decode_smart(signature_t* sig, const unsigned char* enc);

/**
 * @brief Decodes a signature with uncompressed points from a byte array
 *
 * @param sig : Output the decoded signature
 * @param enc : encoded signature
    */
void signature_decode_uncompressed(signature_uncompressed_t* sig, const unsigned char* enc);

/**
 * @brief Decodes a parallel-friendly signature from a byte array
 *
 * @param sig : Output the decoded signature
 * @param enc : encoded signature
    */
void signature_decode_parallel(signature_parallel_t* sig, const unsigned char* enc);

/**
 * @brief Decodes a parallel-friendly smart-sampling-based signature from a byte array
 *
 * @param sig : Output the decoded signature
 * @param enc : encoded signature
    */
void signature_decode_cparallel(signature_cparallel_t* sig, const unsigned char* enc);

/** @}
*/
/** @}
*/

#endif
