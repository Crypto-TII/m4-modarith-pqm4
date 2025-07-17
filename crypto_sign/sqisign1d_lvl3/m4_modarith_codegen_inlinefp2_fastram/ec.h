/** @file
 *
 * @authors Luca De Feo, Francisco RH
 *
 * @brief Elliptic curve stuff
*/

#ifndef EC_H
#define EC_H

#include <fp2.h>
#include <ec_params.h>


/** @defgroup ec Elliptic curves
 * @{
*/

/** @defgroup ec_t Data structures
 * @{
*/

/** @brief Projective point
 *
 * @typedef ec_point_t
 *
 * @struct ec_point_t
 *
 * A projective point in (X:Z) or (X:Y:Z) coordinates (tbd).
*/
typedef struct ec_point_t {
    fp2_t x;
    fp2_t z;
} ec_point_t;

/** @brief A basis of a torsion subgroup
 *
 * @typedef ec_basis_t
 *
 * @struct ec_basis_t
 *
 * A pair of points (or a triplet, tbd) forming a basis of a torsion subgroup.
*/
typedef struct ec_basis_t {
    ec_point_t P;
    ec_point_t Q;
    ec_point_t PmQ;
} ec_basis_t;

/** @brief An elliptic curve
 *
 * @typedef ec_curve_t
 *
 * @struct ec_curve_t
 *
 * An elliptic curve in projective Montgomery form
*/
typedef struct ec_curve_t {
    fp2_t A;
    fp2_t C; ///< cannot be 0
} ec_curve_t;

/** @brief An isogeny of degree a power of 2
 *
 * @typedef ec_isog_even_t
 *
 * @struct ec_isog_even_t
*/
typedef struct ec_isog_even_t {
    ec_curve_t curve;      ///< The domain curve
    ec_point_t kernel;     ///< A kernel generator
    unsigned short length; ///< The length as a 2-isogeny walk
} ec_isog_even_t;


/** @brief An odd divisor of p² - 1
 *
 * @typedef ec_isog_odd_t
 *
 * Given that the list of divisors of p² - 1 is known, this is
 * represented as a fixed-length vector of integer exponents.
*/

typedef uint8_t ec_degree_odd_t[P_LEN + M_LEN];

/** @brief An isogeny of odd degree dividing p² - 1
 *
 * @typedef ec_isog_odd_t
 *
 * @struct ec_isog_odd_t
*/
typedef struct ec_isog_odd_t {
    ec_curve_t curve;
    ec_point_t ker_plus;    ///< A generator of E[p+1] ∩ ker(φ)
    ec_point_t ker_minus;   ///< A generator of E[p-1] ∩ ker(φ)
    ec_degree_odd_t degree; ///< The degree of the isogeny
} ec_isog_odd_t;

/** @brief Isomorphism of Montgomery curves
 *
 * @typedef ec_isom_t
 *
 * @struct ec_isom_t
 *
 * The isomorphism is given by the map maps (X:Z) ↦ ( (Nx X - Nz Z) : (D Z) )
*/
typedef struct ec_isom_t {
    fp2_t Nx;
    fp2_t Nz;
    fp2_t D;
} ec_isom_t;

// end ec_t
/** @}
*/


/** @defgroup ec_curve_t Curves and isomorphisms
 * @{
*/

/**
 * @brief j-invariant in affine form.
 *
 * @param j_inv computed j_invariant
 * @param curve input curve
 */
void ec_j_inv(fp2_t* j_inv, const ec_curve_t* curve);

/**
 * @brief j-invariant in projective form.
 *
 * @param j_inv computed j_invariant
 * @param curve input curve
 */
void ec_j_inv_proj(ec_point_t* j_inv, const ec_curve_t* curve);

/**
 * @brief Isomorphism of elliptic curve
 *
 * @param isom computed isomorphism
 * @param from domain curve
 * @param to image curve
 */
void ec_isomorphism(ec_isom_t* isom, const ec_curve_t* from, const ec_curve_t* to);

/**
 * @brief In-place inversion of an isomorphism
 *
 * @param isom an isomorphism
 */
void ec_iso_inv(ec_isom_t* isom);

/**
 * @brief In-place evaluation of an isomorphism
 *
 * @param P a point
 * @param isom an isomorphism
 */
void ec_iso_eval(ec_point_t* P, ec_isom_t* isom);

/**
 * @brief Given a Montgomery curve, computes a standard model for it and the isomorphism to it.
 *
 * @param new computed new curve
 * @param isom computed isomorphism from `old` to `new`
 * @param old A Montgomery curve
 */
void ec_curve_normalize(ec_curve_t *new, ec_isom_t *isom, const ec_curve_t *old);

/** @}
*/
/** @defgroup ec_point_t Point operations
 * @{
*/

/**
 * @brief Point equality
 *
 * @param P a point
 * @param Q a point
 * @return 1 if equal
 */
bool ec_is_equal(const ec_point_t* P, const ec_point_t* Q);

/**
 * @brief Reduce Z-coordinate of point in place
 *
 * @param P a point
 */
void ec_normalize(ec_point_t* P);

/**
 * @brief Test whether a point is on a curve
 *
 * @param curve a curve
 * @param P a point
 * @return 1 if P is on the curve
 */
int ec_is_on_curve(const ec_curve_t* curve, const ec_point_t* P);

/**
 * @brief Point negation
 *
 * @param res computed opposite of P
 * @param P a point
 */
void ec_neg(ec_point_t* res, const ec_point_t* P);

/**
 * @brief Point addition
 *
 * @param res computed sum of P and Q
 * @param P a point
 * @param Q a point
 * @param PQ the difference P-Q
 */
void ec_add(ec_point_t* res, const ec_point_t* P, const ec_point_t* Q, const ec_point_t* PQ);

/**
 * @brief Point doubling
 *
 * @param res computed double of P
 * @param P a point
 */
void ec_dbl(ec_point_t* res, const ec_curve_t* curve, const ec_point_t* P);

/**
 * @brief Same as ec_dbl but takes the curve coefficient A24=(A+2C:4C)
 *
 * @param res computed double of P
 * @param P a point
 */
void xDBLv2(ec_point_t* res, ec_point_t const* P, ec_point_t const* A24);

/**
 * @brief Point multiplication
 *
 * @param res computed scalar * P
 * @param curve the curve
 * @param scalar an unsigned multi-precision integer
 * @param P a point
 */
void ec_mul(ec_point_t* res, const ec_curve_t* curve, const digit_t* scalar, const ec_point_t* P);

/**
 * @brief Point multiplication by a scalar of limited length
 *
 * @param res computed scalar * P
 * @param curve the curve
 * @param scalar an unsigned multi-precision integer
 * @param kbits the bit size of scalar
 * @param A24 the Montgomery coefficient in projective form (A+2)/4
 * @param P a point
 */
void xMULv2(ec_point_t* Q, ec_point_t const* P, digit_t const* k, const int kbits, ec_point_t const* A24);

/**
 * @brief Point multiplication based on a precomputed differential addition chain
 *
 * @param res computed scalar * P
 * @param curve the curve
 * @param dac the binary differential addition chain presented as a digit
 * @param dac_len the bit length of the dac
 * @param A24 the Montgomery coefficient in projective form (A+2)/4
 * @param P a point
 */
void xMULdac(ec_point_t* Q, ec_point_t const* P, digit_t dac, int dac_len, ec_point_t const* A24);


/**
 * @brief Point tripling for x-only Montgomery coordinates
 *
 * @param res computed scalar * Q
 * @param A3 the Montgomery coefficient in projective form (A+2)/(A-2)
 * @param P a point
 */
void xTPL(ec_point_t* Q, const ec_point_t* P, const ec_point_t* A3);

/**
 * @brief Combination P+m*Q
 *
 * @param R computed P + m * Q
 * @param curve the curve
 * @param m an unsigned multi-precision integer
 * @param P a point
 * @param Q a point
 * @param PQ the difference P-Q
 */
void ec_ladder3pt(ec_point_t *R, const digit_t* m, ec_point_t const *P, ec_point_t const *Q, ec_point_t const *PQ, ec_curve_t const *A);

/**
 * @brief Same as ec_ladder3pt nut takes the A24 = (A+2C:4C) coefficient and is optimize for a given scalar bitlength
 *
 * @param R computed P + m * Q
 * @param A24 the curve coeffcient in (A+2C:4C) form
 * @param m an unsigned multi-precision integer
 * @param mbits the bitlength of m
 * @param P a point
 * @param Q a point
 * @param PQ the difference P-Q
 */
void ec_ladder3ptv2(ec_point_t *R, const digit_t* m, const digit_t mbits, ec_point_t const *P, ec_point_t const *Q, ec_point_t const *PQ, ec_point_t const *A24);

/**
 * @brief Linear combination of points of a basis
 *
 * @param res computed scalarP * P + scalarQ * Q
 * @param curve the curve
 * @param scalarP an unsigned multi-precision integer
 * @param scalarQ an unsigned multi-precision integer
 * @param PQ a torsion basis consisting of points P and Q
 */
void ec_biscalar_mul(ec_point_t* res, const ec_curve_t* curve,
    const digit_t* scalarP, const digit_t* scalarQ,
    const ec_basis_t* PQ);
    
/**
 * @brief Finds a non-zero root of the montgomery curve equation. The curve 
 * is passed as A/C and the root returned is independent of the projective
 * representation as long as C is a square.
 *
 * @param curve the curve
 * @param P2 the projective non-zero root of the curve's equation (a.k.a. a point of order 2)
 */
void ec_mont_root(ec_point_t *P2, const ec_curve_t *curve);

/**
 * @brief Given a non-zero root of the montgomery curve equation, recovers
 * the curve coefficient in the form A24=(A+2C:4C)
 *
 * @param A24 computed curve
 * @param P2 the projective non-zero root of the curve's equation (a.k.a. a point of order 2)
 */
void ec_A24_from_mont_root(ec_point_t *A24, const ec_point_t *P2);

/** @}
*/

/** @defgroup ec_dlog_t Discrete logs and bases
 * @{
*/

/**
 * @brief Computes the difference of two x-only points
 *
 * The algorithm is deterministc
 *
 * @param PQ computed point P-Q
 * @param P a point
 * @param Q a point
 * @param curve the curve
 */
void difference_point(ec_point_t* PQ, const ec_point_t* P, const ec_point_t* Q, const ec_curve_t* curve);

/**
 * @brief Generate a Montgomery curve and a 2^f-torsion basis
 *
 * The algorithm is deterministc
 *
 * @param PQ2 computed basis of the 2^f-torsion
 * @param curve the computed curve
 */
void ec_curve_to_basis_2(ec_basis_t *PQ2, const ec_curve_t *curve);

/**
 * @brief Complete a basis of the 2^f-torsion
 *
 * The algorithm is deterministic
 *
 * @param PQ2 a basis of the 2^f-torsion containing P as first generator
 * @param curve the curve
 * @param P a point of order 2^f
 */
void ec_complete_basis_2(ec_basis_t* PQ2, const ec_curve_t* curve, const ec_point_t* P);

/**
 * @brief More efficient version of ec_complete_basis_2 when P is known to lie over (0,0)
 *
 * Provides the same answer as ec_complete_basis_2 as long as P satifies this condition.
 *
 * @param PQ2 a basis of the 2^f-torsion containing P as first generator
 * @param curve the curve
 * @param P a point of order 2^f over (0,0)
 */
void ec_complete_basis_2_singularP(ec_basis_t* PQ2, const ec_curve_t* curve, const ec_point_t* P);

/**
 * @brief Generate a 3^e-torsion basis
 *
 * The algorithm is deterministic
 *
 * @param PQ3 the computed 3^e-torsion basis
 * @param curve a curve
 */
void ec_curve_to_basis_3(ec_basis_t* PQ3, const ec_curve_t* curve);

/**
 * @brief Generate a 6^e-torsion basis
 *
 * The algorithm is deterministic
 *
 * @param PQ6 the computed 2^f*3^g-torsion basis
 * @param curve a curve
 */
void ec_curve_to_basis_6(ec_basis_t* PQ6, const ec_curve_t* curve);


/**
 * @brief Compute the generalized dlog of R wrt the 2^f-basis PQ2
 *
 * Ensure that R = scalarP * P + scalarQ * Q
 *
 * @param scalarP the computed dlog
 * @param scalarQ the computed dlog
 * @param PQ2 a 2^f-torsion basis
 * @param R a point of order dividing 2^f
 */
void ec_dlog_2(digit_t* scalarP, digit_t* scalarQ,
    const ec_basis_t* PQ2, const ec_point_t* R, const ec_curve_t* curve);

/**
 * @brief Compute the generalized dlog of R wrt the 3^e-basis PQ3
 *
 * Ensure that R = scalarP * P + scalarQ * Q
 *
 * @param scalarP the computed dlog
 * @param scalarQ the computed dlog
 * @param PQ3 a 3^e-torsion basis
 * @param R a point of order dividing 3^e
 */
void ec_dlog_3(digit_t* scalarP, digit_t* scalarQ,
    const ec_basis_t* PQ3, const ec_point_t* R, const ec_curve_t* curve);
/** @}
*/

/** @defgroup ec_isog_t Isogenies
 * @{
*/

/**
 * @brief Evaluate isogeny of even degree on list of points
 *
 * @param image computed image curve
 * @param phi isogeny
 * @param points a list of points to evaluate the isogeny on, modified in place
 * @param length of the list points
 */
void ec_eval_even(ec_curve_t* image, const ec_isog_even_t* phi,
    ec_point_t* points, unsigned short length);
    
/**
 * @brief Evaluates an isogeny of degree 2^(f-1), where f is the power of 2 in the factorization of p+1,
 *          and outputs the codomain curve and a non-(0,0) point of order 2 in the image.
 *          WARNING: if f is odd, then this function only works if the kernel point is not over (0,0).
 *
 * @param Pa computed non-(0,0) point of order 2 in the image curve
 * @param A24out the computed image curve coefficient in the form (A+2C:4C)
 * @param A24in the domain curve coefficient in the form (A+2C:4C)
 * @param kernel a kernel generator of order 2^f (note the point must be of order 2^f even though the isogeny is only 2^(f-1)))
 */
    void ec_eval_even_strategy_smart(ec_point_t *Pa, ec_point_t* A24out,
    const ec_point_t* A24in, const ec_point_t *kernel);

/**
 * @brief Evaluates an isogeny of degree 2^(f-1), where f is the power of 2 in the factorization of p+1, evaluating the
 * A24 = (A+2C:4C) coefficient and also the j invariant of the second-to-last curve in the chain.
 * Warning: Only works for even f
 *
 * @param A24out computed codomain curve coefficient in the form (A+2C:4C)
 * @param A24in the domain curve coefficient in the form (A+2C:4C)
 * @param second_to_last_j_inv computed projective j-invariant of the second-to-last curve in the chain
 * @param kernel a kernel generator of order 2^(f-1)
 */
void ec_eval_even_strategy_uncompressed(ec_point_t* A24out, ec_point_t* second_to_last_j_inv, ec_point_t* A24in, const ec_point_t *kernel);

/**
 * @brief Evaluates an isogeny of degre 2^(f-1) while recording the point of order 2 in the kernel and the the curve found midway after lambda steps.
 * Warning: Assumes that (0,0) is not in the kernel, and only works for even f.
 *
 * @param A24out computed codomain curve coefficient in the form (A+2C:4C)
 * @param A24mid computed midway curve after degree 2^lambda, in the form (A+2C:4C)
 * @param K2 computed point of order 2 over which the kernel point lays
 * @param A24in the domain curve coefficient in the form (A+2C:4C)
 * @param kernel a kernel generator of order 2^(f-1)
 */
void ec_eval_even_strategy_parallel(ec_point_t* A24out, ec_point_t *A24mid, ec_point_t *K2, ec_point_t* A24in, const ec_point_t *kernel);

/**
 * @brief Evaluates an isogeny of degree 2^lambda while pushing a single point. Assumes that the kernel doesn't contain (0,0)
 *
 * @param image the codomain curve in the ( A : C) form
 * @param push_point a point to be pusehd in-place
 * @param A24 the domain curve coefficient in the form (A+2C:4C)
 * @param kernel a kernel generator of order 2^lambda not over (0,0)
 */
void ec_eval_even_strategy_chal(ec_curve_t* image, ec_point_t* push_point,
    ec_point_t* A24, const ec_point_t *kernel);

/**
 * @brief Same as ec_eval_even_strategy_chal but doesn't push a point and instead stores the j invariant
 * of the second-to-last curve in the chain.
 *
 * @param image computed codomain curve in the ( A : C) form
 * @param second_to_last_j_inv computed projective j-invariant of the second-to-last curve in the chain
 * @param A24 the domain curve coefficient in the form (A+2C:4C)
 * @param kernel a kernel generator of order 2^lambda not over (0,0)
 */
void ec_eval_even_strategy_chal_uncompressed(ec_curve_t* image, ec_point_t* second_to_last_j_inv, const ec_point_t* A24, const ec_point_t *kernel);

/**
 * @brief Evaluate isogeny of even degree on list of points, assuming the point (0,0) is not in the kernel
 *
 * @param image computed image curve
 * @param phi isogeny
 * @param points a list of points to evaluate the isogeny on, modified in place
 * @param length of the list points
 */
void ec_eval_even_nonzero(ec_curve_t* image, const ec_isog_even_t* phi,
    ec_point_t* points, unsigned short length);

/**
 * @brief Evaluate isogeny of even degree on list of torsion bases
 *
 * @param image computed image curve
 * @param phi isogeny
 * @param points a list of bases to evaluate the isogeny on, modified in place
 * @param length of the list bases
 */
static inline void ec_eval_even_basis(ec_curve_t* image, const ec_isog_even_t* phi,
    ec_basis_t* points, unsigned short length) {
    ec_eval_even(image, phi, (ec_point_t*)points, sizeof(ec_basis_t) / sizeof(ec_point_t) * length);
}

/**
 * @brief Evaluate isogeny of odd degree on list of points
 *
 * @param image computed image curve
 * @param phi isogeny
 * @param points a list of points to evaluate the isogeny on, modified in place
 * @param length of the list points
 */
void ec_eval_odd(ec_curve_t* image, const ec_isog_odd_t* phi,
    ec_point_t* points, unsigned short length);

/**
 * @brief Evaluate isogeny of odd degree on list of torsion bases
 *
 * @param image computed image curve
 * @param phi isogeny
 * @param points a list of bases to evaluate the isogeny on, modified in place
 * @param length of the list bases
 */
static inline void ec_eval_odd_basis(ec_curve_t* image, const ec_isog_odd_t* phi,
    ec_basis_t* points, unsigned short length) {
    ec_eval_odd(image, phi, (ec_point_t*)points, sizeof(ec_basis_t) / sizeof(ec_point_t) * length);
}

/**
 * @brief Generate an implicit 2^f-torsion basis using smart sampling.
 *
 * Not compatible with NIST round 1 KATs.
 *
 * @param P computed point of order a multiple of 2^f, not over (0,0)
 * @param Q computed point of order a multiple of 2^f, over (0,0)
 * @param Pa a non-(0,0) point of order 2 in the curve
 */
void ec_curve_to_implicit_basis_2_smart(ec_point_t *P, ec_point_t *Q, const ec_point_t *Pa);

/**
 * @brief All-in-one procedure to find an implicit basis with smart sampling and compute K = P + [scalar]*Q.
 *
 * @param K computed kernel point of order 2^f
 * @param Pa a non-(0,0) point of order 2 in the curve
 * @param scalar an unsigned multi-digit scalar
 * @param swapPQ if true, computes Q + [scalar]*P instead of P + [scalar]*Q
 */
void ec_scalar_to_kernel_smart(ec_point_t *K, const ec_point_t *Pa, const digit_t *scalar, const bool swapPQ);

/**
 * @brief Same as ec_scalar_to_kernel_smart, but produces a point of order 2^lambda that is not over (0,0). Also returns
 *      an auxiliary point of the same order that lays over (0,0).
 *
 * @param K computed kernel point of order 2^lambda
 * @param R computer point of order 2^lambda not in the kernel
 * @param Pa a non-(0,0) point of order 2 in the curve
 * @param A24 the curve coefficient in (A+2C:4C) form
 * @param scalar an unsigned multi-digit scalar
 * @param swapPQ if true, computes Q + [scalar]*P instead of P + [scalar]*Q
 */
void ec_scalar_to_kernel_secpar_smart(ec_point_t *K, ec_point_t *R, const ec_point_t *Pa, const ec_point_t *A24, const digit_t *scalar);

/** @}
*/

// end ec
/** @}
*/



#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// ORIGINAL VERSION

#if 0

/** @file
 *
 * @authors Luca De Feo, Francisco RH
 *
 * @brief Elliptic curve stuff
*/

#ifndef EC_H
#define EC_H

#include <gf.h>

/** @defgroup ec Elliptic curves
 * @{
*/

/** @defgroup ec_t Data structures
 * @{
*/

/** @brief Projective point
 *
 * @typedef ec_point_t
 *
 * @struct ec_point_t
 *
 * A projective point in (X:Z) or (X:Y:Z) coordinates (tbd).
*/
typedef struct ec_point_t {
    fp2_t X;
    //fp2_t Y;
    fp2_t Z;
} ec_point_t;

/** @brief A basis of a torsion subgroup
 *
 * @typedef ec_basis_t
 *
 * @struct ec_basis_t
 *
 * A pair of points (or a triplet, tbd) forming a basis of a torsion subgroup.
*/
typedef struct ec_basis_t {
    ec_point_t P;
    ec_point_t Q;
    ec_point_t PmQ;  // or maybe not
} ec_basis_t;

/** @brief An elliptic curve
 *
 * @typedef ec_curve_t
 *
 * @struct ec_curve_t
 *
 * An elliptic curve in projective Montgomery form
*/
typedef struct ec_curve_t {
    fp2_t A;
    fp2_t C; ///< cannot be 0
} ec_curve_t;

/** @brief An isogeny of degree a power of 2
 *
 * @typedef ec_isog_even_t
 *
 * @struct ec_isog_even_t
*/
typedef struct ec_isog_even_t {
    ec_curve_t curve;      ///< The domain curve
    ec_point_t kernel;     ///< A kernel generator
    unsigned short length; ///< The length as a 2-isogeny walk
} ec_isog_even_t;


/** @brief An odd divisor of p² - 1
 *
 * @typedef ec_isog_odd_t
 *
 * @struct ec_isog_odd_t
 *
 * Given that the list of divisors of p² - 1 is known, this could be
 * represented as a fixed-length vector of integer exponents, possibly
 * distinguishing the divisors of p + 1 from those of p - 1.
*/
typedef struct ec_degree_odd_t {
    // todo (basically a ushort vector)
} ec_degree_odd_t;

/** @brief An isogeny of odd degree dividing p² - 1
 *
 * @typedef ec_isog_odd_t
 *
 * @struct ec_isog_odd_t
*/
typedef struct ec_isog_odd_t {
    ec_point_t ker_plus;    ///< A generator of E[p+1] ∩ ker(φ)
    ec_point_t ker_minus;   ///< A generator of E[p-1] ∩ ker(φ)
    ec_degree_odd_t degree; ///< The degree of the isogeny
} ec_isog_odd_t;

/** @brief Isomorphism of Montgomery curves
 *
 * @typedef ec_isom_t
 *
 * @struct ec_isom_t
 *
 * The isomorphism is given by the map maps (X:Z) ↦ ( (Nx X - Nz Z) : (D Z) )
 * TODO: fix if (X:Y:Z) coordinates.
*/
typedef struct ec_isom_t {
    fp2_t Nx;
    fp2_t Nz;
    fp2_t D;
} ec_isom_t;

// end ec_t
/** @}
*/


/** @defgroup ec_curve_t Curves and isomorphisms
 * @{
*/

/**
 * @brief j-invariant.
 *
 * @param j_inv computed j_invariant
 * @param curve input curve
 */
void ec_j_inv(fp2_t* j_inv, const ec_curve_t* curve);

/**
 * @brief Isomorphism of elliptic curve
 *
 * @param isom computed isomorphism
 * @param from domain curve
 * @param to image curve
 */
void ec_isomorphism(ec_isom_t* isom, const ec_curve_t* from, const ec_curve_t* to);

/** @}
*/
/** @defgroup ec_point_t Point operations
 * @{
*/

/**
 * @brief Point equality
 *
 * @param P a point
 * @param Q a point
 * @return 1 if equal
 */
int ec_is_equal(const ec_point_t* P, const ec_point_t* Q);

/**
 * @brief Reduce Z-coordinate of point in place
 *
 * @param P a point
 */
void ec_normalize(ec_point_t* P);

/**
 * @brief Test whether a point is on a curve
 *
 * @param curve a curve
 * @param P a point
 * @return 1 if P is on the curve
 */
int ec_is_on_curve(const ec_curve_t* curve, const ec_point_t* P);

/**
 * @brief Point negation
 *
 * @param res computed opposite of P
 * @param P a point
 */
void ec_neg(ec_point_t* res, const ec_point_t* P);

/**
 * @brief Point addition
 *
 * Needs to be adjusted if (X:Z) arithmetic.
 *
 * @param res computed sum of P and Q
 * @param P a point
 * @param Q a point
 */
void ec_add(ec_point_t* res, const ec_point_t* P, const ec_point_t* Q);

/**
 * @brief Point doubling
 *
 * @param res computed double of P
 * @param P a point
 */
void ec_dbl(ec_point_t* res, const ec_curve_t* curve, const ec_point_t* P);

/**
 * @brief Point multiplication
 *
 * @param res computed scalar * P
 * @param curve the curve
 * @param scalar an unsigned multi-precision integer
 * @param P a point
 */
void ec_mul(ec_point_t* res, const ec_curve_t* curve, const digit_t* scalar, const ec_point_t* P);

/**
 * @brief Linear combination of points of a basis
 *
 * @param res computed scalarP * P + scalarQ * Q
 * @param curve the curve
 * @param scalarP an unsigned multi-precision integer
 * @param scalarQ an unsigned multi-precision integer
 * @param PQ a torsion basis consisting of points P and Q
 */
void ec_biscalar_mul(ec_point_t* res, const ec_curve_t* curve,
    const digit_t* scalarP, const digit_t* scalarQ,
    const ec_basis_t* PQ);

/** @}
*/

/** @defgroup ec_dlog_t Discrete logs and bases
 * @{
*/

/**
 * @brief Generate a Montgomery curve and a 2^f-torsion basis
 *
 * The algorithm is deterministc
 *
 * @param curve the computed curve
 * @param PQ2 a basis of the 2^f-torsion
 * @param j_inv a j-invariant
 */
void ec_j_to_basis_2(ec_curve_t* curve, ec_basis_t* PQ2, const fp2_t* j_inv);

/**
 * @brief Complete a basis of the 2^f-torsion
 *
 * The algorithm is deterministc
 *
 * @param PQ2 a basis of the 2^f-torsion containing P as first generator
 * @param curve the curve
 * @param P a point of order 2^f
 */
void ec_complete_basis_2(ec_basis_t* PQ2, const ec_curve_t* curve, const ec_point_t* P);

/**
 * @brief Generate a 3^e-torsion basis
 *
 * The algorithm is deterministc
 *
 * @param PQ3 the computed 3^e-torsion basis
 * @param curve a curve
 */
void ec_curve_to_basis_3(ec_basis_t* PQ3, const ec_curve_t* curve);

/**
 * @brief Compute the generalized dlog of R wrt the 2^f-basis PQ2
 *
 * Ensure that R = scalarP * P + scalarQ * Q
 *
 * @param scalarP the computed dlog
 * @param scalarQ the computed dlog
 * @param PQ2 a 2^f-torsion basis
 * @param R a point of order dividing 2^f
 */
void ec_dlog_2(digit_t* scalarP, digit_t* scalarQ,
    const ec_basis_t* PQ2, const ec_point_t* R);

/**
 * @brief Compute the generalized dlog of R wrt the 3^e-basis PQ3
 *
 * Ensure that R = scalarP * P + scalarQ * Q
 *
 * @param scalarP the computed dlog
 * @param scalarQ the computed dlog
 * @param PQ3 a 3^e-torsion basis
 * @param R a point of order dividing 3^e
 */
void ec_dlog_3(digit_t* scalarP, digit_t* scalarQ,
    const ec_basis_t* PQ3, const ec_point_t* R);
/** @}
*/

/** @defgroup ec_isog_t Isogenies
 * @{
*/

/**
 * @brief Evaluate isogeny of even degree on list of points
 *
 * @param image computed image curve
 * @param phi isogeny
 * @param points a list of points to evaluate the isogeny on, modified in place
 * @param length of the list points
 */
void ec_eval_even(ec_curve_t* image, const ec_isog_even_t* phi,
    ec_point_t* points, unsigned short length);

/**
 * @brief Evaluate isogeny of even degree on list of torsion bases
 *
 * @param image computed image curve
 * @param phi isogeny
 * @param points a list of bases to evaluate the isogeny on, modified in place
 * @param length of the list bases
 */
static inline void ec_eval_even_basis(ec_curve_t* image, const ec_isog_even_t* phi,
    ec_basis_t* points, unsigned short length) {
    ec_eval_even(image, phi, points, sizeof(ec_basis_t) / sizeof(ec_point_t) * length);
}

/**
 * @brief Evaluate isogeny of odd degree on list of points
 *
 * @param image computed image curve
 * @param phi isogeny
 * @param points a list of points to evaluate the isogeny on, modified in place
 * @param length of the list points
 */
void ec_eval_odd(ec_curve_t* image, const ec_isog_odd_t* phi,
    ec_point_t* points, unsigned short length);

/**
 * @brief Evaluate isogeny of odd degree on list of torsion bases
 *
 * @param image computed image curve
 * @param phi isogeny
 * @param points a list of bases to evaluate the isogeny on, modified in place
 * @param length of the list bases
 */
static inline void ec_eval_odd_basis(ec_curve_t* image, const ec_isog_odd_t* phi,
    ec_basis_t* points, unsigned short length) {
    ec_eval_odd(image, phi, points, sizeof(ec_basis_t) / sizeof(ec_point_t) * length);
}

/** @}
*/

// end ec
/** @}
*/

#endif


#endif
