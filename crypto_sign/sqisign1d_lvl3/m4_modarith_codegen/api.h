// SPDX-License-Identifier: Apache-2.0

#ifndef api_h
#define api_h

#include <stddef.h>

#define CRYPTO_SECRETKEYBYTES 1138
#define CRYPTO_PUBLICKEYBYTES   96
#define CRYPTO_BYTES           263

#define CRYPTO_ALGNAME "lvl3"

#if defined(ENABLE_SIGN) || defined(PQM4)
int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

int
crypto_sign(unsigned char *sm, size_t *smlen,
            const unsigned char *m, size_t mlen,
            const unsigned char *sk);
#endif

int
crypto_sign_open(unsigned char *m, size_t *mlen,
                 const unsigned char *sm, size_t smlen,
                 const unsigned char *pk);

#endif /* api_h */
