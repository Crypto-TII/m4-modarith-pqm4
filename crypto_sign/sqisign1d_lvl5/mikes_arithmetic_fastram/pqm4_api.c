// SPDX-License-Identifier: Apache-2.0

#include <api.h>
#include <string.h>
#include <sig.h>

const char kat_lvl5_pk[CRYPTO_PUBLICKEYBYTES] = {
    0x6A, 0x93, 0x02, 0xB5, 0x1F, 0x65, 0xAC, 0x67, 0xC5, 0x74, 0xA0, 0x5E,
    0x95, 0x98, 0x60, 0x4D, 0x95, 0xAA, 0x6E, 0x3C, 0x61, 0xB9, 0x3C, 0xBA,
    0x98, 0xEE, 0xFE, 0x80, 0x89, 0x05, 0x65, 0x2D, 0x43, 0x93, 0x3D, 0xC0,
    0xEC, 0xEF, 0x7B, 0xE2, 0x67, 0x37, 0x3E, 0xB1, 0xAC, 0x61, 0xD4, 0xA9,
    0x11, 0x38, 0xD2, 0x35, 0xF7, 0xBA, 0x19, 0xDD, 0x12, 0xE5, 0x5F, 0x48,
    0xD6, 0xFE, 0x1E, 0x00, 0x0A, 0xAC, 0xB9, 0x0A, 0xB4, 0xFF, 0xC3, 0xD9,
    0x0D, 0x79, 0x3F, 0x2A, 0x12, 0x91, 0xDC, 0x63, 0x8F, 0x80, 0x28, 0xE3,
    0x1F, 0x7D, 0xA9, 0xFE, 0x49, 0x76, 0x71, 0xDB, 0x2D, 0xBA, 0xC7, 0x89,
    0x48, 0x6A, 0x61, 0xC9, 0x4B, 0xCB, 0xE9, 0xA8, 0xDD, 0xD7, 0x86, 0x70,
    0x95, 0x5C, 0xFC, 0x3A, 0x10, 0x4D, 0x21, 0x41, 0x18, 0x89, 0x12, 0x5A,
    0x96, 0x68, 0x59, 0x1B, 0x16, 0xD4, 0x04, 0x00};

typedef struct {
    size_t mlen;
    char msg[59];
    size_t smlen;
    char sm[59 + CRYPTO_BYTES];
} SQISign_KAT_t;

const SQISign_KAT_t kat_lvl5[2] = {
    {
        .mlen = 32,
        .msg = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        .smlen = 32 + CRYPTO_BYTES,
        .sm = {0xDD, 0xBC, 0x1D, 0x77, 0x8D, 0x8A, 0xC7, 0x3D, 0x6A, 0xA6, 0x35,
               0x54, 0xD0, 0x6A, 0x27, 0x6C, 0xF6, 0xB5, 0x01, 0xFC, 0x1A, 0x86,
               0x9E, 0xB0, 0xDB, 0x97, 0xE9, 0xB5, 0xFA, 0xD6, 0xAC, 0xB0, 0x0F,
               0x90, 0x2E, 0x3D, 0x31, 0x01, 0xF6, 0xCC, 0xE1, 0x53, 0x60, 0xF3,
               0xFA, 0xD8, 0x5E, 0xC2, 0xAC, 0x25, 0x0E, 0xFC, 0x43, 0xE6, 0x10,
               0x2A, 0x01, 0x6F, 0x33, 0x1A, 0xA4, 0x98, 0xC0, 0x84, 0x5D, 0x1C,
               0x36, 0xA5, 0x62, 0xB2, 0xF5, 0xF5, 0xEF, 0xD3, 0x51, 0x00, 0xB8,
               0xC6, 0x59, 0x08, 0xC1, 0xC9, 0x36, 0xD6, 0x9C, 0x31, 0x80, 0x0A,
               0x06, 0xC3, 0xCA, 0xD9, 0x8A, 0x66, 0x00, 0x86, 0x73, 0x53, 0x39,
               0x64, 0xA5, 0x8C, 0x37, 0x77, 0x7D, 0x8F, 0xCB, 0xE9, 0xE0, 0x2B,
               0xEF, 0x15, 0x88, 0x00, 0x7E, 0xDE, 0x98, 0x57, 0x6D, 0x6D, 0x0B,
               0x1F, 0x0A, 0x2C, 0xFD, 0x98, 0xE8, 0x42, 0x4C, 0xC3, 0xCB, 0x58,
               0x00, 0x03, 0xCC, 0x97, 0xBC, 0x79, 0xF2, 0x90, 0x33, 0x6C, 0x9D,
               0xF3, 0xE9, 0xF9, 0xCE, 0x59, 0xA6, 0x15, 0x19, 0x01, 0x19, 0x26,
               0x9B, 0x91, 0x9F, 0xE0, 0xD8, 0x21, 0xCF, 0x0E, 0x70, 0x91, 0x3C,
               0x1D, 0x15, 0x39, 0x83, 0x4A, 0x01, 0x3A, 0x8C, 0xF1, 0xDE, 0x75,
               0x78, 0x95, 0xB6, 0x11, 0xEA, 0x1C, 0xBB, 0x08, 0xC1, 0x7E, 0x15,
               0x40, 0x79, 0x00, 0x35, 0xB2, 0xE6, 0xD9, 0x85, 0x05, 0x1D, 0x80,
               0xB8, 0xA1, 0x3E, 0xF0, 0x1D, 0xFB, 0x88, 0x06, 0xED, 0xD8, 0x01,
               0x76, 0xE7, 0xBE, 0xFD, 0x08, 0x81, 0x2D, 0xED, 0x7D, 0x2D, 0xCA,
               0x23, 0xB8, 0xF7, 0xBE, 0x70, 0x07, 0xBF, 0x01, 0xF5, 0xB3, 0x2C,
               0xA0, 0x8B, 0xF0, 0xF0, 0x03, 0xFA, 0x4A, 0x75, 0x8B, 0xD3, 0xD8,
               0xD3, 0x5F, 0x79, 0xA1, 0x01, 0xDC, 0xE0, 0x19, 0x90, 0xF5, 0x2E,
               0xD7, 0xA9, 0xD8, 0x0C, 0x88, 0x15, 0x88, 0x98, 0x9F, 0x9E, 0x49,
               0x5E, 0x00, 0x01, 0x7B, 0x77, 0x8D, 0x16, 0xA8, 0x3A, 0x76, 0xBF,
               0xCC, 0x5A, 0x52, 0xF8, 0x21, 0x9E, 0x34, 0xBA, 0x42, 0x01, 0x82,
               0xE9, 0xD4, 0x27, 0x4E, 0x75, 0xE8, 0x6D, 0x88, 0x0A, 0x7F, 0x8D,
               0x5B, 0x83, 0x04, 0x01, 0x74, 0x98, 0x83, 0x6C, 0xF6, 0x6D, 0x5D,
               0xE9, 0x33, 0xCF, 0x1C, 0x59, 0xC2, 0x9F, 0x5F, 0x2F, 0x7C, 0x4E,
               0x01, 0xEE, 0xC3, 0xA6, 0xEA, 0xF9, 0x68, 0x1D, 0x21, 0x5F, 0x5F,
               0x82, 0x38, 0xCF, 0xF6, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00},
    },
    {
        .mlen = 59,
        .msg = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        .smlen = 59 + CRYPTO_BYTES,
        .sm = {0x1B, 0x32, 0x7C, 0x45, 0xF5, 0x9F, 0xA4, 0xF6, 0x0C, 0xD8, 0xFC,
               0x1A, 0xBD, 0x22, 0x57, 0xF1, 0x23, 0x4F, 0x00, 0x10, 0x22, 0x16,
               0x56, 0x6C, 0x3E, 0xC0, 0x7D, 0x94, 0x39, 0x7C, 0x00, 0xC0, 0x2E,
               0x87, 0xAE, 0x42, 0x9A, 0x00, 0x63, 0xAA, 0x87, 0x75, 0xAF, 0xC7,
               0x17, 0x64, 0xE1, 0x43, 0x3F, 0x15, 0x94, 0x27, 0x88, 0x0F, 0x0B,
               0xAB, 0x00, 0xEE, 0x53, 0xC9, 0xF4, 0xAE, 0xB3, 0xD7, 0x30, 0x75,
               0xB3, 0x8A, 0x81, 0xD9, 0x83, 0x1F, 0x57, 0xF8, 0x7F, 0x01, 0x1C,
               0x7A, 0x8E, 0x66, 0x3E, 0x40, 0xB2, 0xD6, 0x48, 0x29, 0x4D, 0x97,
               0xA4, 0xBB, 0x8F, 0x0C, 0xA0, 0x1F, 0x01, 0xAD, 0x91, 0x65, 0x50,
               0x54, 0xBA, 0xB5, 0x77, 0xCB, 0xCA, 0x6C, 0x8A, 0x77, 0x09, 0x95,
               0x83, 0x58, 0x8B, 0x01, 0x03, 0x45, 0x49, 0x1C, 0xA9, 0xC0, 0x7D,
               0xD3, 0x28, 0x1C, 0x87, 0x19, 0xD2, 0xD0, 0x0B, 0xC1, 0x01, 0x42,
               0x01, 0xD8, 0x0F, 0xD9, 0x07, 0x22, 0xF9, 0x5A, 0xE4, 0x31, 0x12,
               0x01, 0x72, 0xDD, 0x7C, 0x1A, 0x1E, 0xDC, 0xC4, 0x00, 0x18, 0xD7,
               0xA0, 0x32, 0x99, 0xA0, 0xD5, 0x66, 0x42, 0xDB, 0x1D, 0x2B, 0x31,
               0x01, 0xC2, 0x26, 0x79, 0x29, 0x00, 0x45, 0xA2, 0x63, 0x21, 0xFB,
               0x8B, 0xBF, 0x0A, 0x77, 0x17, 0x4B, 0x80, 0x7D, 0xBD, 0x49, 0xE8,
               0x4A, 0x4C, 0x01, 0xB9, 0xD6, 0x17, 0xB8, 0xE2, 0xEE, 0x17, 0xC4,
               0x60, 0x7C, 0x17, 0x23, 0x85, 0xCE, 0xC5, 0x64, 0xB9, 0x4F, 0x00,
               0xB7, 0x4B, 0xC7, 0xC7, 0x85, 0xC5, 0x78, 0xA4, 0xC3, 0x60, 0x3A,
               0x3F, 0x52, 0x8C, 0x37, 0x65, 0x21, 0x1F, 0x01, 0x6F, 0x71, 0x4D,
               0x38, 0x37, 0xB3, 0x9B, 0x68, 0xEA, 0xEA, 0x9E, 0xFA, 0x73, 0xDF,
               0x0A, 0x7A, 0xFF, 0xB7, 0x01, 0x25, 0x24, 0xF2, 0xA9, 0x4D, 0x6E,
               0x93, 0x2F, 0xDB, 0xC9, 0x8A, 0xA7, 0x83, 0xA1, 0x72, 0x42, 0xF4,
               0x35, 0x01, 0x01, 0xE7, 0x51, 0xC9, 0x40, 0x0C, 0xA8, 0x35, 0x4D,
               0x3F, 0x3B, 0x84, 0x14, 0xAC, 0x8B, 0xEA, 0xE4, 0x35, 0xA3, 0x77,
               0x38, 0xDB, 0x54, 0x16, 0xF8, 0x3D, 0x01, 0x27, 0x21, 0x53, 0xF6,
               0x77, 0x84, 0x07, 0x03, 0xE8, 0xE1, 0x96, 0x36, 0x51, 0x32, 0x68,
               0x94, 0xD4, 0x48, 0xC6, 0x5C, 0x06, 0xE6, 0xF6, 0x23, 0xF0, 0x35,
               0x00, 0x75, 0x55, 0xDD, 0xFC, 0x5A, 0x42, 0x05, 0xEC, 0xB7, 0x92,
               0x96, 0xF0, 0x58, 0x99, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    },
};

int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk) {
    memcpy(pk, kat_lvl5_pk, CRYPTO_PUBLICKEYBYTES);
    // We don't need the secret key
    memcpy(sk, 0, CRYPTO_SECRETKEYBYTES);
}

int
crypto_sign(unsigned char *sm, size_t *smlen,
            const unsigned char *m, size_t mlen,
            const unsigned char *sk) {
    for (int i = 0; i < sizeof(kat_lvl5) / sizeof(kat_lvl5[0]); i++) {
        if (mlen == kat_lvl5[i].mlen) {
            memcpy(sm, kat_lvl5[i].sm, kat_lvl5[i].smlen);
            *smlen = kat_lvl5[i].smlen;
            return 0;
        }
    }

    return 1;
}

int
crypto_sign_open(unsigned char *m, size_t *mlen,
                 const unsigned char *sm, size_t smlen,
                 const unsigned char *pk) {
    return sqisign_open(m, mlen, sm, smlen, pk);
}
