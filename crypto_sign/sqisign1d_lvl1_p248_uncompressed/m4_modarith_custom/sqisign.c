#include <protocols.h>
#include <string.h>

#if defined(ENABLE_SIGN)
int sqisign_keypair(unsigned char *pk, unsigned char *sk) { 
    int ret;
    secret_key_t skt;
    public_key_t pkt = { 0 };
    secret_key_init(&skt);

    ret = !protocols_keygen(&pkt, &skt);

    secret_key_encode(sk, &skt, &pkt);
    public_key_encode(pk, &pkt);
    secret_key_finalize(&skt);
    return ret;
}

int sqisign_sign(unsigned char *sm,
              size_t *smlen, const unsigned char *m,
              size_t mlen, const unsigned char *sk) {
    int ret = 0;
    secret_key_t skt;
    public_key_t pkt = { 0 };
    signature_t sigt;
    secret_key_init(&skt);
    signature_init(&sigt);
    secret_key_decode(&skt, &pkt, sk);

    ret = !protocols_sign(&sigt, &pkt, &skt, m, mlen);
    signature_encode(sm, &sigt);

    memcpy(sm + SIGNATURE_LEN, m, mlen);
    *smlen = SIGNATURE_LEN + mlen;

    secret_key_finalize(&skt);
    signature_finalize(&sigt);
    return ret;
}
#endif

int sqisign_open_smart(unsigned char *m,
              size_t *mlen, const unsigned char *sm,
              size_t smlen, const unsigned char *pk) { 
    int ret = 0;
    public_key_smart_t pkt = { 0 };
    signature_t sigt;
    signature_init(&sigt);

    public_key_decode_smart(&pkt, pk);
    signature_decode_smart(&sigt, sm);

    ret = !protocols_verif_smart(&sigt, &pkt, sm + SMART_SIGNATURE_LEN, smlen - SMART_SIGNATURE_LEN);

    if (!ret) {
        *mlen = smlen - SMART_SIGNATURE_LEN;
        memmove(m, sm + SMART_SIGNATURE_LEN, *mlen);
    }

    signature_finalize(&sigt);
    return ret;
}

int sqisign_open_uncompressed(unsigned char *m,
              size_t *mlen, const unsigned char *sm,
              size_t smlen, const unsigned char *pk) { 
    int ret = 0;
    public_key_t pkt = { 0 };
    signature_uncompressed_t sigt;

    public_key_decode(&pkt, pk);
    signature_decode_uncompressed(&sigt, sm);

    ret = !protocols_verif_uncompressed(&sigt, &pkt, sm + UNCOMPRESSED_SIGNATURE_LEN, smlen - UNCOMPRESSED_SIGNATURE_LEN);

    if (!ret) {
        *mlen = smlen - UNCOMPRESSED_SIGNATURE_LEN;
        memmove(m, sm + UNCOMPRESSED_SIGNATURE_LEN, *mlen);
    }

    return ret;
}

int sqisign_open_parallel(unsigned char *m,
              size_t *mlen, const unsigned char *sm,
              size_t smlen, const unsigned char *pk) { 
    int ret = 0;
    public_key_t pkt = { 0 };
    signature_parallel_t sigt;

    public_key_decode(&pkt, pk);
    signature_decode_parallel(&sigt, sm);

    ret = !protocols_verif_parallel(&sigt, &pkt, sm + PARALLEL_SIGNATURE_LEN, smlen - PARALLEL_SIGNATURE_LEN);

    if (!ret) {
        *mlen = smlen - PARALLEL_SIGNATURE_LEN;
        memmove(m, sm + PARALLEL_SIGNATURE_LEN, *mlen);
    }

    return ret;
}

int sqisign_open_cparallel(unsigned char *m,
              size_t *mlen, const unsigned char *sm,
              size_t smlen, const unsigned char *pk) { 
    int ret = 0;
    public_key_smart_t pkt = { 0 };
    signature_cparallel_t sigt;

    public_key_decode_smart(&pkt, pk);
    signature_decode_cparallel(&sigt, sm);

    ret = !protocols_verif_cparallel(&sigt, &pkt, sm + COMPRESSED_PARALLEL_SIGNATURE_LEN, smlen - COMPRESSED_PARALLEL_SIGNATURE_LEN);

    if (!ret) {
        *mlen = smlen - COMPRESSED_PARALLEL_SIGNATURE_LEN;
        memmove(m, sm + COMPRESSED_PARALLEL_SIGNATURE_LEN, *mlen);
    }

    return ret;
}

int sqisign_open(unsigned char *m,
              size_t *mlen, const unsigned char *sm,
              size_t smlen, const unsigned char *pk) { 
    #if POWER_OF_3 > 0
    int ret = 0;
    public_key_t pkt = { 0 };
    signature_t sigt;
    signature_init(&sigt);

    public_key_decode(&pkt, pk);
    signature_decode(&sigt, sm);

    ret = !protocols_verif(&sigt, &pkt, sm + SIGNATURE_LEN, smlen - SIGNATURE_LEN);

    if (!ret) {
        *mlen = smlen - SIGNATURE_LEN;
        memmove(m, sm + SIGNATURE_LEN, *mlen);
    }

    signature_finalize(&sigt);
    return ret;
    #else
    return sqisign_open_smart(m, mlen, sm, smlen, pk);
    #endif
}

int sqisign_verify_smart(const unsigned char *m,
                size_t mlen, const unsigned char *sig,
                size_t siglen, const unsigned char *pk) {

    int ret = 0;
    public_key_smart_t pkt = { 0 };
    signature_t sigt;
    signature_init(&sigt);

    public_key_decode_smart(&pkt, pk);
    signature_decode_smart(&sigt, sig);

    ret = !protocols_verif_smart(&sigt, &pkt, m, mlen);

    signature_finalize(&sigt);
    return ret;
}

int sqisign_verify_uncompressed(const unsigned char *m,
                size_t mlen, const unsigned char *sig,
                size_t siglen, const unsigned char *pk) {

    int ret = 0;
    public_key_t pkt = { 0 };
    signature_uncompressed_t sigt;

    public_key_decode(&pkt, pk);
    signature_decode_uncompressed(&sigt, sig);

    ret = !protocols_verif_uncompressed(&sigt, &pkt, m, mlen);
    return ret;
}

int sqisign_verify_parallel(const unsigned char *m,
                size_t mlen, const unsigned char *sig,
                size_t siglen, const unsigned char *pk) {

    int ret = 0;
    public_key_t pkt = { 0 };
    signature_parallel_t sigt;

    public_key_decode(&pkt, pk);
    signature_decode_parallel(&sigt, sig);

    ret = !protocols_verif_parallel(&sigt, &pkt, m, mlen);
    return ret;
}

int sqisign_verify_cparallel(const unsigned char *m,
                size_t mlen, const unsigned char *sig,
                size_t siglen, const unsigned char *pk) {

    int ret = 0;
    public_key_smart_t pkt = { 0 };
    signature_cparallel_t sigt;

    public_key_decode_smart(&pkt, pk);
    signature_decode_cparallel(&sigt, sig);

    ret = !protocols_verif_cparallel(&sigt, &pkt, m, mlen);
    return ret;
}

int sqisign_verify(const unsigned char *m,
                size_t mlen, const unsigned char *sig,
                size_t siglen, const unsigned char *pk) {

    #if POWER_OF_3 > 0
    int ret = 0;
    public_key_t pkt = { 0 };
    signature_t sigt;
    signature_init(&sigt);

    public_key_decode(&pkt, pk);
    signature_decode(&sigt, sig);

    ret = !protocols_verif(&sigt, &pkt, m, mlen);

    signature_finalize(&sigt);
    return ret;
    #else
    return sqisign_verify_smart(m, mlen, sig, siglen, pk);
    #endif
}