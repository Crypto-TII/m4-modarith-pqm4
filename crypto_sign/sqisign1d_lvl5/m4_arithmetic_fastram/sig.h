// SPDX-License-Identifier: Apache-2.0

#ifndef SQISIGN_H
#define SQISIGN_H

#include <stddef.h>
#include <stdint.h>

#if defined(ENABLE_SIGN)
/**
 * SQIsign keypair generation.
 *
 * The implementation corresponds to SQIsign.CompactKeyGen() in the SQIsign spec.
 * The caller is responsible to allocate sufficient memory to hold pk and sk.
 *
 * @param[out] pk SQIsign public key
 * @param[out] sk SQIsign secret key
 * @return int status code
 */
int sqisign_keypair(unsigned char *pk, unsigned char *sk);

/**
 * SQIsign signature generation.
 *
 * The implementation performs SQIsign.expandSK() + SQIsign.sign() in the SQIsign spec.
 * Keys provided is a compacted secret keys.
 * The caller is responsible to allocate sufficient memory to hold sm.
 *
 * @param[out] sm Signature concatenated with message
 * @param[out] smlen Pointer to the length of sm
 * @param[in] m Message to be signed
 * @param[in] mlen Message length
 * @param[in] sk Compacted secret key
 * @return int status code
 */
int sqisign_sign(unsigned char *sm,
              size_t *smlen, const unsigned char *m,
              size_t mlen, const unsigned char *sk);
#endif

/**
 * SQIsign open signature.
 *
 * The implementation performs SQIsign.verify(). If the signature verification succeeded, the original message is stored in m.
 * Keys provided is a compact public key.
 * The caller is responsible to allocate sufficient memory to hold m.
 *
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sm Signature concatenated with message
 * @param[in] smlen Length of sm
 * @param[in] pk Compacted public key
 * @return int status code
 */
int sqisign_open(unsigned char *m,
              size_t *mlen, const unsigned char *sm,
              size_t smlen, const unsigned char *pk);
              
/**
 * SQIsign open smart-sampling-based signature.
 *
 * The implementation performs SQIsign.verify() using various smart sampling optimizations,
 * which are not compatible with the NIST round 1 KATs.
 * If the signature verification succeeded, the original message is stored in m.
 * Keys provided is a compact public key.
 * The caller is responsible to allocate sufficient memory to hold m.
 *
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sm Signature concatenated with message
 * @param[in] smlen Length of sm
 * @param[in] pk Compacted public key
 * @return int status code
 */
int sqisign_open_smart(unsigned char *m,
              size_t *mlen, const unsigned char *sm,
              size_t smlen, const unsigned char *pk);
              
/**
 * SQIsign open signature with uncompressed points.
 *
 * The implementation performs SQIsign.verify() with uncompressed points,
 * which are not compatible with the NIST round 1 KATs.
 * If the signature verification succeeded, the original message is stored in m.
 * Keys provided is a compact public key.
 * The caller is responsible to allocate sufficient memory to hold m.
 *
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sm Signature concatenated with message
 * @param[in] smlen Length of sm
 * @param[in] pk Compacted public key
 * @return int status code
 */
int sqisign_open_uncompressed(unsigned char *m,
              size_t *mlen, const unsigned char *sm,
              size_t smlen, const unsigned char *pk);
              
/**
 * SQIsign open parallel-friendly signature.
 *
 * The implementation performs SQIsign.verify() assuming the signature contains
 * additional data that allows for parallel verification. Not compatible with the NIST round 1 KATs.
 * If the signature verification succeeded, the original message is stored in m.
 * Keys provided is a compact public key.
 * The caller is responsible to allocate sufficient memory to hold m.
 *
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sm Signature concatenated with message
 * @param[in] smlen Length of sm
 * @param[in] pk Compacted public key
 * @return int status code
 */
int sqisign_open_parallel(unsigned char *m,
              size_t *mlen, const unsigned char *sm,
              size_t smlen, const unsigned char *pk);
              
/**
 * SQIsign open parallel-friendly smart-sampling-based signature.
 *
 * The implementation performs SQIsign.verify() using various smart sampling optimizations,
 * which are not compatible with the NIST round 1 KATs.
 * If the signature verification succeeded, the original message is stored in m.
 * Keys provided is a compact public key.
 * The caller is responsible to allocate sufficient memory to hold m.
 *
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sm Signature concatenated with message
 * @param[in] smlen Length of sm
 * @param[in] pk Compacted public key
 * @return int status code
 */
int sqisign_open_cparallel(unsigned char *m,
              size_t *mlen, const unsigned char *sm,
              size_t smlen, const unsigned char *pk);


/**
 * SQIsign verify signature.
 *
 * If the signature verification succeeded, returns 0, otherwise 1.
 *
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sig Signature
 * @param[in] siglen Length of sig
 * @param[in] pk Compacted public key
 * @return int 0 if verification succeeded, 1 otherwise.
 */
int sqisign_verify(const unsigned char *m,
                size_t mlen, const unsigned char *sig,
                size_t siglen, const unsigned char *pk);

/**
 * SQIsign verify signature, using smart sampling optimizations which are
 * not compatible with the NIST round 1 KATs.
 *
 * If the signature verification succeeded, returns 0, otherwise 1.
 *
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sig Signature
 * @param[in] siglen Length of sig
 * @param[in] pk Compacted public key
 * @return int 0 if verification succeeded, 1 otherwise.
 */
int sqisign_verify_smart(const unsigned char *m,
                size_t mlen, const unsigned char *sig,
                size_t siglen, const unsigned char *pk);

/**
 * SQIsign verify signature, using signatures with uncompressed points which are
 * note compatible with NIST round 1 KATs.
 *
 * If the signature verification succeeded, returns 0, otherwise 1.
 *
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sig Signature
 * @param[in] siglen Length of sig
 * @param[in] pk Compacted public key
 * @return int 0 if verification succeeded, 1 otherwise.
 */
int sqisign_verify_uncompressed(const unsigned char *m,
                size_t mlen, const unsigned char *sig,
                size_t siglen, const unsigned char *pk);

/**
 * SQIsign verify signature, assuming the signature contains
 * additional data that allows for parallel verification. Not compatible with the NIST round 1 KATs.
 *
 * If the signature verification succeeded, returns 0, otherwise 1.
 *
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sig Signature
 * @param[in] siglen Length of sig
 * @param[in] pk Compacted public key
 * @return int 0 if verification succeeded, 1 otherwise.
 */
int sqisign_verify_parallel(const unsigned char *m,
                size_t mlen, const unsigned char *sig,
                size_t siglen, const unsigned char *pk);

/**
 * SQIsign verify parallel-friendly signature, using smart sampling optimizations which are
 * not compatible with the NIST round 1 KATs.
 *
 * If the signature verification succeeded, returns 0, otherwise 1.
 *
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sig Signature
 * @param[in] siglen Length of sig
 * @param[in] pk Compacted public key
 * @return int 0 if verification succeeded, 1 otherwise.
 */
int sqisign_verify_cparallel(const unsigned char *m,
                size_t mlen, const unsigned char *sig,
                size_t siglen, const unsigned char *pk);

#endif
