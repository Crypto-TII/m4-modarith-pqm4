#include "tutil.h"
#include "isog.h"
#include <stdio.h>
#include <stdbool.h>

int ec_is_on_curve(const ec_curve_t* curve, const ec_point_t* P){

    fp2_t t0, t1, t2;

    // Check if xz*(C^2x^2+zACx+z^2C^2) is a square
    fp2_mul(&t0, &curve->C, &P->x); 
    fp2_mul(&t1, &t0, &P->z);       
    fp2_mul(&t1, &t1, &curve->A);   
    fp2_mul(&t2, &curve->C, &P->z); 
    fp2_sqr(&t0, &t0);              
    fp2_sqr(&t2, &t2);              
    fp2_add(&t0, &t0, &t1);
    fp2_add(&t0, &t0, &t2);
    fp2_mul(&t0, &t0, &P->x);
    fp2_mul(&t0, &t0, &P->z);
    return fp2_is_square(&t0) || fp2_is_zero(&t0);
}

void difference_point(ec_point_t* PQ, const ec_point_t* P, const ec_point_t* Q, const ec_curve_t* curve){
    // Given P,Q in projective x-only, computes a deterministic choice for (P-Q)
    // Based on Proposition 3 of https://eprint.iacr.org/2017/518.pdf

    fp2_t Bxx, Bxz, Bzz, t0, t1;

    fp2_mul(&t0, &P->x, &Q->x);
    fp2_mul(&t1, &P->z, &Q->z);
    fp2_sub(&Bxx, &t0, &t1);
    fp2_sqr(&Bxx, &Bxx);
    fp2_mul(&Bxx, &Bxx, &curve->C); // C*(P.x*Q.x-P.z*Q.z)^2
    fp2_add(&Bxz, &t0, &t1);
    fp2_mul(&t0, &P->x, &Q->z);
    fp2_mul(&t1, &P->z, &Q->x);
    fp2_add(&Bzz, &t0, &t1);
    fp2_mul(&Bxz, &Bxz, &Bzz); // (P.x*Q.x+P.z*Q.z)(P.x*Q.z+P.z*Q.x)
    fp2_sub(&Bzz, &t0, &t1);
    fp2_sqr(&Bzz, &Bzz);
    fp2_mul(&Bzz, &Bzz, &curve->C); // C*(P.x*Q.z-P.z*Q.x)^2
    fp2_mul(&Bxz, &Bxz, &curve->C); // C*(P.x*Q.x+P.z*Q.z)(P.x*Q.z+P.z*Q.x)
    fp2_mul(&t0, &t0, &t1);
    fp2_mul(&t0, &t0, &curve->A);
    fp2_add(&t0, &t0, &t0);
    fp2_add(&Bxz, &Bxz, &t0); // C*(P.x*Q.x+P.z*Q.z)(P.x*Q.z+P.z*Q.x) + 2*A*P.x*Q.z*P.z*Q.x

    // Normalization: our squareroot always has the same sign as long as P.z, Q.z, and C
    // are in Fp and C is a square, so the B's should be scaled by C*C_bar^2*P.z_bar^2*Q.Z_bar^2
    fp_copy(t0.re, curve->C.re);
    fp_neg(t0.im, curve->C.im);
    fp2_sqr(&t0, &t0);
    fp2_mul(&t0, &t0, &curve->C);
    fp_copy(t1.re, P->z.re);
    fp_neg(t1.im, P->z.im);
    fp2_sqr(&t1, &t1);
    fp2_mul(&t0, &t0, &t1);
    fp_copy(t1.re, Q->z.re);
    fp_neg(t1.im, Q->z.im);
    fp2_sqr(&t1, &t1);
    fp2_mul(&t0, &t0, &t1);
    fp2_mul(&Bxx, &Bxx, &t0);
    fp2_mul(&Bxz, &Bxz, &t0);
    fp2_mul(&Bzz, &Bzz, &t0);

    // Solving quadratic equation
    fp2_sqr(&t0, &Bxz); 
    fp2_mul(&t1, &Bxx, &Bzz);
    fp2_sub(&t0, &t0, &t1);
    fp2_sqrt(&t0);
    fp2_add(&PQ->x, &Bxz, &t0);
    fp2_copy(&PQ->z, &Bzz);
}

void ec_curve_to_basis_3(ec_basis_t* PQ3, const ec_curve_t* curve){

    fp2_t x, t0, t1, t2;
    ec_point_t P, Q, Q3, P3, A24, A3;

    // Curve coefficient in the form A24 = (A+2C:4C)
    fp2_add(&A24.z, &curve->C, &curve->C);
    fp2_add(&A24.x, &curve->A, &A24.z);
    fp2_add(&A24.z, &A24.z, &A24.z);

    // Curve coefficient in the form A3 = (A+2C:A-2C)
    fp2_sub(&A3.z, &A24.x, &A24.z);
    fp2_copy(&A3.x, &A24.x);

    fp_mont_setone(x.re);
    fp_set(x.im, 0);

    // Find P
    while(1){
        fp_add(x.im, x.re, x.im);

        // Check if point is rational
        fp2_sqr(&t0, &curve->C);
        fp2_mul(&t1, &t0, &x);
        fp2_mul(&t2, &curve->A, &curve->C);
        fp2_add(&t1, &t1, &t2);
        fp2_mul(&t1, &t1, &x);
        fp2_add(&t1, &t1, &t0);
        fp2_mul(&t1, &t1, &x);
        if(fp2_is_square(&t1)){
            fp2_copy(&P.x, &x);
            fp_mont_setone(P.z.re);
            fp_set(P.z.im, 0);
        }
        else
            continue;

        // Clear non-3 factors from the order
        for(int i = 0; i < POWER_OF_2; i++){
            xDBLv2(&P, &P, &A24);
        }
        for(int i = 1; i < P_LEN; i++){
            for(int j = 0; j < TORSION_PLUS_ODD_POWERS[i]; j++){
                xMULdac(&P, &P, DACS[i], DAC_LEN[i], &A24);
            }
        }

        // Check if point has order 3^g
        copy_point(&P3, &P);
        for(int i = 0; i < POWER_OF_3 - 1; i++)
            xTPL(&P3, &P3, &A3);
        if(ec_is_zero(&P3))
            continue;
        else
            break;
    }
    
    // Find Q
    while(1){
        fp_add(x.im, x.re, x.im);

        // Check if point is rational
        fp2_sqr(&t0, &curve->C);
        fp2_mul(&t1, &t0, &x);
        fp2_mul(&t2, &curve->A, &curve->C);
        fp2_add(&t1, &t1, &t2);
        fp2_mul(&t1, &t1, &x);
        fp2_add(&t1, &t1, &t0);
        fp2_mul(&t1, &t1, &x);
        if(fp2_is_square(&t1)){
            fp2_copy(&Q.x, &x);
            fp_mont_setone(Q.z.re);
            fp_set(Q.z.im, 0);
        }
        else
            continue;

        // Clear non-3 factors from the order
        for(int i = 0; i < POWER_OF_2; i++){
            xDBLv2(&Q, &Q, &A24);
        }
        for(int i = 1; i < P_LEN; i++){
            for(int j = 0; j < TORSION_PLUS_ODD_POWERS[i]; j++){
                xMULdac(&Q, &Q, DACS[i], DAC_LEN[i], &A24);
            }
        }

        // Check if point has order 3^g
        copy_point(&Q3, &Q);
        for(int i = 0; i < POWER_OF_3 - 1; i++)
            xTPL(&Q3, &Q3, &A3);
        if(ec_is_zero(&Q3))
            continue;

        // Check if point is orthogonal to P
        if(is_point_equal(&P3, &Q3))
            continue;
        xDBLv2(&P3, &P3, &A24);
        if(is_point_equal(&P3, &Q3))
            continue;
        else
            break;
    }

    // Compute P-Q
    difference_point(&PQ3->PmQ, &P, &Q, curve);
    copy_point(&PQ3->P, &P);
    copy_point(&PQ3->Q, &Q);
}

void ec_curve_to_basis_6(ec_basis_t* PQ6, const ec_curve_t* curve){

    fp2_t x, t0, t1, t2;
    ec_point_t P, Q, Q6, P6, R, T, A24, A3;

    // Curve coefficient in the form A24 = (A+2C:4C)
    fp2_add(&A24.z, &curve->C, &curve->C);
    fp2_add(&A24.x, &curve->A, &A24.z);
    fp2_add(&A24.z, &A24.z, &A24.z);

    // Curve coefficient in the form A3 = (A+2C:A-2C)
    fp2_sub(&A3.z, &A24.x, &A24.z);
    fp2_copy(&A3.x, &A24.x);

    fp_mont_setone(x.re);
    fp_set(x.im, 0);

    // Find P
    while(1){
        fp_add(x.im, x.re, x.im);

        // Check if point is rational
        fp2_sqr(&t0, &curve->C);
        fp2_mul(&t1, &t0, &x);
        fp2_mul(&t2, &curve->A, &curve->C);
        fp2_add(&t1, &t1, &t2);
        fp2_mul(&t1, &t1, &x);
        fp2_add(&t1, &t1, &t0);
        fp2_mul(&t1, &t1, &x);
        if(fp2_is_square(&t1)){
            fp2_copy(&P.x, &x);
            fp_mont_setone(P.z.re);
            fp_set(P.z.im, 0);
        }
        else
            continue;

        // Clear non-2 factors and non-3 factors from the order
        for(int i = 1; i < P_LEN; i++){
            for(int j = 0; j < TORSION_PLUS_ODD_POWERS[i]; j++){
                xMULdac(&P, &P, DACS[i], DAC_LEN[i], &A24);
            }
        }

        // Check if point has order 2^f*3^g
        copy_point(&P6, &P);
        for(int i = 0; i < POWER_OF_2 - 1; i++)
            xDBLv2(&P6, &P6, &A24);
        for(int i = 0; i < POWER_OF_3 - 1; i++)
            xTPL(&P6, &P6, &A3);
        if(ec_is_zero(&P6))
            continue;
        xDBLv2(&T, &P6, &A24);
        if (ec_is_zero(&T))
            continue;
        xTPL(&T, &P6, &A3);
        if (ec_is_zero(&T))
            continue;
        break;
    }

    // Find Q
    while(1){
        fp_add(x.im, x.re, x.im);

        // Check if point is rational
        fp2_sqr(&t0, &curve->C);
        fp2_mul(&t1, &t0, &x);
        fp2_mul(&t2, &curve->A, &curve->C);
        fp2_add(&t1, &t1, &t2);
        fp2_mul(&t1, &t1, &x);
        fp2_add(&t1, &t1, &t0);
        fp2_mul(&t1, &t1, &x);
        if(fp2_is_square(&t1)){
            fp2_copy(&Q.x, &x);
            fp_mont_setone(Q.z.re);
            fp_set(Q.z.im, 0);
        }
        else
            continue;

        // Clear non-6 factors from the order
        for(int i = 1; i < P_LEN; i++){
            for(int j = 0; j < TORSION_PLUS_ODD_POWERS[i]; j++){
                xMULdac(&Q, &Q, DACS[i], DAC_LEN[i], &A24);
            }
        }

        // Check first if point has order 2^f*3^g
        copy_point(&Q6, &Q);
        for(int i = 0; i < POWER_OF_2 - 1; i++)
            xDBLv2(&Q6, &Q6, &A24);
        for(int i = 0; i < POWER_OF_3 - 1; i++)
            xTPL(&Q6, &Q6, &A3);
        if(ec_is_zero(&Q6))
            continue;
        xDBLv2(&T, &Q6, &A24);
        if (ec_is_zero(&T))
            continue;
        xTPL(&T, &Q6, &A3);
        if (ec_is_zero(&T))
            continue;

        // Check if point P is independent from point Q
        xTPL(&R, &P6, &A3);
        xTPL(&T, &Q6, &A3);
        if(is_point_equal(&R, &T))
            continue;
        xDBLv2(&R, &P6, &A24);
        xDBLv2(&T, &Q6, &A24);
        if(is_point_equal(&R, &T))
            continue;
        break;
    }

    // Compute P-Q
    difference_point(&PQ6->PmQ, &P, &Q, curve);
    copy_point(&PQ6->P, &P);
    copy_point(&PQ6->Q, &Q);
}

static bool profile(const ec_point_t *L, const fp2_t *x){
    // Computes the profile of an affine x with respect to a 2-torsion point different than (0,0)
    // e.g. the reduced tate pairing t_2(L, P) with P.x = x and P.z = 1.

    fp2_t t1;
    fp2_mul(&t1, &L->z, x);
    fp2_sub(&t1, &t1, &L->x);
    fp2_mul(&t1, &t1, &L->z);
    return fp2_is_square(&t1) || fp2_is_zero(&t1);
}


static bool profile2(const ec_point_t *L, const fp2_t *x){
    // Computes the product of the profiles of an affine x with the two
    // non-(0,0) points of order 2, Pa = (Lx:Lz) and Pb = (Lz:Lx).

    fp2_t t1, t2, t3;
    fp2_mul(&t1, &L->x, &L->z);
    fp2_sqr(&t2, x);
    fp2_mul(&t2, &t2, &t1); //x^2*Lx*Lz
    fp2_add(&t1, &t1, &t2);
    fp2_sqr(&t2, &L->x);
    fp2_sqr(&t3, &L->z);
    fp2_add(&t2, &t2, &t3); // Lx^2 + Lz^2
    fp2_mul(&t2, &t2, x);
    fp2_sub(&t1, &t1, &t2);
    return fp2_is_square(&t1) || fp2_is_zero(&t1);
}

static bool profile_proj(const ec_point_t *L, const ec_point_t *P){
    // Same as profile but for a projective x coordinate
    fp2_t t1, t2;
    fp2_mul(&t1, &L->z, &P->x);
    fp2_mul(&t2, &L->x, &P->z);
    fp2_sub(&t1, &t1, &t2);
    fp2_mul(&t1, &t1, &L->z);
    fp2_mul(&t1, &t1, &P->z);
    return fp2_is_square(&t1) || fp2_is_zero(&t1);
}

static void ec_curve_to_implicit_basis_seed(digit_t *kP, digit_t *kQ, const ec_point_t *Pa){
    // Profile-based method for computing an implicit basis for the 2^f-torsion given
    // a non-(0,0) point P2 of order 2. Returns only the constants kP, kQ so that
    // Px = 1 + kP*i and Qx = 1 + kQ*i
    // Compatible with Nist round 1 KATs

    fp2_t x;
    bool p0, pa, pb, pab;
    digit_t k = 0;
    int nonres_id = 0;
    bool found0 = false;
    bool founda = false;
    bool foundb = false;
    uint8_t found = 0;

    // The other point of order 2
    ec_point_t Pb;
    fp2_copy(&Pb.x, &Pa->z);
    fp2_copy(&Pb.z, &Pa->x);

    fp_mont_setone(x.re);
    fp_set(x.im, 0);
    while(found < 2){
        k += 1;
        fp_add(x.im, x.re, x.im);

        // (0,0) profile obtained form precomputed non square residues
        // Compare against the next element in NONRES
        if(nonres_id < NONRES_LEN && k != NONRES[nonres_id]){
            p0 = true;
        }
        else if(nonres_id < NONRES_LEN && k == NONRES[nonres_id]){
            p0 = false;
            nonres_id++;
        }
        else{ // Or compute the legendre if out of NONRES elements
            p0 = fp2_is_square(&x);
        }
        if(p0 && found0) continue; // point lies over (0,0) but we already have one

        // product of (alpha,0) and (1/alpha,0) profiles
        pab = profile2(Pa, &x);
        if(!(p0 ^ !pab)) continue; // point not on curve

        // (alpha,0) profile
        pa = profile(Pa, &x);
        if(pa && founda) continue; // point lies over (alpha,0) but we already have one
        if(pa && p0) continue; // point is non-full torsion

        // (1/alpha, 0) profile
        pb = pab ^ !pa;
        if(pb && foundb) continue; // point lies over (1/alpha,0) but we already have one

        // At this point we are sure to have a new basis point
        if(found == 0) *kP = k;
        else *kQ = k;
        found++;

        if(p0) found0 =  true;
        if(pa) founda =  true;
        if(pb) foundb =  true;
    }
}

static void ec_curve_to_implicit_basis_seed_smart(digit_t *kP, digit_t *kQ, const ec_point_t *Pa){
    // Optimized version of ec_curve_to_implicit_basis_seed based on the smart sampling
    // technique from https://eprint.iacr.org/2023/1559.pdf
    // NOT compatible with Nist round 1 KATs

    fp2_t x;
    digit_t k = 0;

    // The other point of order 2
    ec_point_t Pb, Q;
    fp2_copy(&Pb.x, &Pa->z);
    fp2_copy(&Pb.z, &Pa->x);

    *kP = 0;
    *kQ = 0;

    fp_mont_setone(x.re);

    // Find point P not over (0,0)
    for(int i = 0; i < NONRES_LEN; i++){
        k = NONRES[i];
        fp_set(x.im, k);
        fp_tomont(x.im, x.im);
        if(!profile2(Pa, &x)){
            *kP = k;
            break;
        }
    }
    // On the unlikely event that we run out of NONRES elements without finding the point
    while(*kP == 0){
        k += 1;
        fp_set(x.im, k);
        fp_tomont(x.im, x.im);
        if(fp2_is_square(&x)) continue;
        if(!profile2(Pa, &x)) *kP = k;
    }

    // Find point Q over (0,0)
    fp_add(x.re, x.re, x.re);
    for(int i = 0; i < NONRES_LEN; i++){
        k = SMARTZ[i];
        fp_set(x.im, k);
        fp_tomont(x.im, x.im);
        fp2_mul(&Q.x, &x, &Pa->x);
        fp2_copy(&Q.z, &Pa->z);
        if(!profile_proj(&Pb, &Q)){
            *kQ = k;
            break;
        }
    }
    // On the unlikely event that we run out of SMARTZ elements without finding the point
    while(*kQ == 0){
        k += 1;
        fp_set(x.im, k);
        fp_tomont(x.im, x.im);
        if(!fp2_is_square(&x)) continue;
        if(profile(&Pb, &x)) continue;
        if(!profile2(Pa, &x)) *kQ = k;
    }
}

void ec_curve_to_basis_2(ec_basis_t *PQ2, const ec_curve_t *curve){
    // Profile-based method for computing a basis for the 2^f-torsion
    // KAT-compatible with original NIST submission

    ec_point_t A24, A3, Pa;
    digit_t kP, kQ;

    // Curve coefficient in the form A24 = (A+2C:4C)
    fp2_add(&A24.z, &curve->C, &curve->C);
    fp2_add(&A24.x, &curve->A, &A24.z);
    fp2_add(&A24.z, &A24.z, &A24.z);

    // Curve coefficient in the form A3 = (A+2C:A-2C)
    fp2_copy(&A3.x, &A24.x);
    fp2_sub(&A3.z, &A24.x, &A24.z);

    // Root of the curve poly
    ec_mont_root(&Pa, curve);

    ec_curve_to_implicit_basis_seed(&kP, &kQ, &Pa);

    fp_set(PQ2->P.x.re, 1);
    fp_set(PQ2->Q.x.re, 1);
    fp_set(PQ2->P.x.im, kP);
    fp_set(PQ2->Q.x.im, kQ);
    fp_set(PQ2->P.z.re, 1);
    fp_set(PQ2->Q.z.re, 1);
    fp_set(PQ2->P.z.im, 0);
    fp_set(PQ2->Q.z.im, 0);

    // Clear cofactors
    for(int i = 0; i < POWER_OF_3; i++){
        xTPL(&PQ2->P, &PQ2->P, &A3);
        xTPL(&PQ2->Q, &PQ2->Q, &A3);
    }
    for(int i = (POWER_OF_3 > 0) & 1; i < P_LEN; i++){
        for(int j = 0; j < TORSION_PLUS_ODD_POWERS[i]; j++){
            xMULdac(&PQ2->P, &PQ2->P, DACS[i], DAC_LEN[i], &A24);
            xMULdac(&PQ2->Q, &PQ2->Q, DACS[i], DAC_LEN[i], &A24);
        }
    }
    
    // Compute P-Q
    difference_point(&PQ2->PmQ, &PQ2->P, &PQ2->Q, curve);
}

void ec_curve_to_implicit_basis_2_smart(ec_point_t *P, ec_point_t *Q, const ec_point_t *Pa){
    // Computes basis point P and Q (without difference point) for an implicit basis using
    // a non-(0,0) point Pa of order 2 on the curve.

    digit_t kP, kQ;
    ec_curve_to_implicit_basis_seed_smart(&kP, &kQ, Pa);

    fp_set(P->x.re, 1);
    fp_set(Q->x.re, 2);
    fp_set(P->x.im, kP);
    fp_set(Q->x.im, kQ);
    fp_set(P->z.re, 1);
    fp_set(Q->z.re, 1);
    fp_set(P->z.im, 0);
    fp_set(Q->z.im, 0);

    fp2_mul(&Q->x, &Q->x, &Pa->x);
    fp2_mul(&Q->z, &Q->z, &Pa->z);
}

void ec_complete_basis_2(ec_basis_t* PQ2, const ec_curve_t* curve, const ec_point_t* P){

    fp2_t x, t0, t1, t2;
    ec_point_t Q, Q2, P2, A24, A3;

    // Curve coefficient in the form A24 = (A+2C:4C)
    fp2_add(&A24.z, &curve->C, &curve->C);
    fp2_add(&A24.x, &curve->A, &A24.z);
    fp2_add(&A24.z, &A24.z, &A24.z);

    // Curve coefficient in the form A3 = (A+2C:A-2C)
    fp2_copy(&A3.x, &A24.x);
    fp2_sub(&A3.z, &A24.x, &A24.z);

    // Point of order 2 generated by P
    copy_point(&P2, P);
    for(int i = 0; i < POWER_OF_2 - 1; i++)
        xDBLv2(&P2, &P2, &A24);

    // Find Q
    fp_mont_setone(x.re);
    fp_set(x.im, 0);
    while(1){
        fp_add(x.im, x.re, x.im);

        // Check if point is rational
        fp2_sqr(&t0, &curve->C);
        fp2_mul(&t1, &t0, &x);
        fp2_mul(&t2, &curve->A, &curve->C);
        fp2_add(&t1, &t1, &t2);
        fp2_mul(&t1, &t1, &x);
        fp2_add(&t1, &t1, &t0);
        fp2_mul(&t1, &t1, &x);
        if(fp2_is_square(&t1)){
            fp2_copy(&Q.x, &x);
            fp_mont_setone(Q.z.re);
            fp_set(Q.z.im, 0);
        }
        else
            continue;

        // Clear odd factors from the order
        for(int i = 0; i < POWER_OF_3; i++){
            xTPL(&Q, &Q, &A3);
        }
        for(int i = (POWER_OF_3 > 0) & 1; i < P_LEN; i++){
            for(int j = 0; j < TORSION_PLUS_ODD_POWERS[i]; j++){
                xMULdac(&Q, &Q, DACS[i], DAC_LEN[i], &A24);
            }
        }

        // Check if point has order 2^f
        copy_point(&Q2, &Q);
        for(int i = 0; i < POWER_OF_2 - 1; i++)
            xDBLv2(&Q2, &Q2, &A24);
        if(ec_is_zero(&Q2))
            continue;

        // Check if point is orthogonal to P
        if(is_point_equal(&P2, &Q2))
            continue;
        else
            break;
    }

    // Compute P-Q
    difference_point(&PQ2->PmQ, P, &Q, curve);
    copy_point(&PQ2->Q, &Q);
    copy_point(&PQ2->P, P);
}

void ec_complete_basis_2_singularP(ec_basis_t* PQ2, const ec_curve_t* curve, const ec_point_t* P){
    // profile-based optimzed version of ec_complete_basis_2 when P lies over (0,0)

    fp2_t x, x_mont, t0, t1, t2;
    ec_point_t Q, Q2, P2, A24, A3;
    int n = -1;

    // Curve coefficient in the form A24 = (A+2C:4C)
    fp2_add(&A24.z, &curve->C, &curve->C);
    fp2_add(&A24.x, &curve->A, &A24.z);
    fp2_add(&A24.z, &A24.z, &A24.z);

    // Curve coefficient in the form A3 = (A+2C:A-2C)
    fp2_copy(&A3.x, &A24.x);
    fp2_sub(&A3.z, &A24.x, &A24.z);

    // Find Q
    fp_set(x.re, 1);
    while(1){
        n++;
        if(n < NONRES_LEN){
            // draw x.im from a precomputed list of non-squares
            fp_set(x.im, NONRES[n]);
        }
        else{
            // continue searching one by one if we run out of precomputed non-squares
            fp_set(x.im, NONRES[NONRES_LEN-1] + n - NONRES_LEN + 1);

            //only accept non-square values here for x, these are the ones with 2^f torsion
            if(fp2_is_square(&x)){
                continue;
            }
        }
        fp2_tomont(&x_mont, &x);

        // Check if point is rational
        fp2_sqr(&t0, &curve->C);
        fp2_mul(&t1, &t0, &x_mont);
        fp2_mul(&t2, &curve->A, &curve->C);
        fp2_add(&t1, &t1, &t2);
        fp2_mul(&t1, &t1, &x_mont);
        fp2_add(&t1, &t1, &t0);
        fp2_mul(&t1, &t1, &x_mont);
        if(fp2_is_square(&t1)){
            fp2_copy(&Q.x, &x);
            fp_set(Q.z.re, 1);
            fp_set(Q.z.im, 0);
        }
        else
            continue;

        // Clear odd factors from the order
        for(int i = 0; i < POWER_OF_3; i++){
            xTPL(&Q, &Q, &A3);
        }
        for(int i = (POWER_OF_3 > 0) & 1; i < P_LEN; i++){
            for(int j = 0; j < TORSION_PLUS_ODD_POWERS[i]; j++){
                xMULdac(&Q, &Q, DACS[i], DAC_LEN[i], &A24);
            }
        }

        break;

    }

    // Compute P-Q
    difference_point(&PQ2->PmQ, P, &Q, curve);
    copy_point(&PQ2->Q, &Q);
    copy_point(&PQ2->P, P);
}

void ec_scalar_to_kernel_smart(ec_point_t *K, const ec_point_t *Pa, const digit_t *scalar, const bool swapPQ){

    ec_point_t P,Q,PQ,A24;
    ec_curve_t curve;

    // A24 curve coefficient
    ec_A24_from_mont_root(&A24, Pa);

    // Regular curve coefficient
    fp2_add(&curve.A, &A24.x, &A24.x);
    fp2_sub(&curve.A, &curve.A, &A24.z);
    fp2_add(&curve.A, &curve.A, &curve.A);
    fp2_copy(&curve.C, &A24.z);

    ec_curve_to_implicit_basis_2_smart(&P, &Q, Pa);
    swap_points(&P, &Q, -(digit_t)(swapPQ));

    // Note: we could do a single xMUL after the ladder
    for(int i = 0; i < P_LEN; i++){
        for(int j = 0; j < TORSION_PLUS_ODD_POWERS[i]; j++){
            xMULdac(&P, &P, DACS[i], DAC_LEN[i], &A24);
            xMULdac(&Q, &Q, DACS[i], DAC_LEN[i], &A24);
        }
    }

    difference_point(&PQ, &P, &Q, &curve);
    ec_ladder3ptv2(K, scalar, POWER_OF_2, &P, &Q, &PQ, &A24);
}

void ec_scalar_to_kernel_secpar_smart(ec_point_t *K, ec_point_t *R, const ec_point_t *Pa, const ec_point_t *A24, const digit_t *scalar){

    ec_point_t P,Q,PQ;
    ec_curve_t curve;

    // Regular curve coefficient
    fp2_add(&curve.A, &A24->x, &A24->x);
    fp2_sub(&curve.A, &curve.A, &A24->z);
    fp2_add(&curve.A, &curve.A, &curve.A);
    fp2_copy(&curve.C, &A24->z);

    ec_curve_to_implicit_basis_2_smart(&P, &Q, Pa);

    for(int i = 0; i < P_LEN; i++){
        for(int j = 0; j < TORSION_PLUS_ODD_POWERS[i]; j++){
            xMULdac(&P, &P, DACS[i], DAC_LEN[i], A24);
            xMULdac(&Q, &Q, DACS[i], DAC_LEN[i], A24);
        }
    }

    copy_point(R, &Q);
    difference_point(&PQ, &P, &Q, &curve);
    ec_ladder3ptv2(K, scalar, POWER_OF_2_SECPAR, &P, &Q, &PQ, A24);

    for(int i = 0; i < POWER_OF_2 - POWER_OF_2_SECPAR; i++){
        xDBLv2(K, K, A24);
        xDBLv2(R, R, A24);
    }
}