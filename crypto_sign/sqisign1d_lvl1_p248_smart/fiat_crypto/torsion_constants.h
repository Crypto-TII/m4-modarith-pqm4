#include <tutil.h>
#include "fp_constants.h"
#define TORSION_2POWER_BYTES 32
#define TORSION_3POWER_BYTES 1
#define TORSION_23POWER_BYTES 32
extern const digit_t TORSION_PLUS_EVEN_POWER;
extern const digit_t TORSION_ODD_PRIMES[2];
extern const digit_t TORSION_ODD_POWERS[2];
extern const size_t TORSION_PLUS_ODD_POWERS[1];
extern const digit_t TORSION_PLUS_2POWER_DIGITS[NWORDS_ORDER];
extern const digit_t TORSION_PLUS_3POWER_DIGITS[NWORDS_ORDER];
#if defined(ENABLE_SIGN)
#include <intbig.h>
extern const digit_t TORSION_PLUS_ODD_PRIMES[1];
extern const digit_t TORSION_MINUS_ODD_PRIMES[1];
extern const size_t TORSION_MINUS_ODD_POWERS[1];
extern const size_t DEGREE_COMMITMENT_POWERS[2];
extern const ibz_t CHARACTERISTIC;
extern const ibz_t TORSION_ODD;
extern const ibz_t TORSION_ODD_PRIMEPOWERS[2];
extern const ibz_t TORSION_ODD_PLUS;
extern const ibz_t TORSION_ODD_MINUS;
extern const ibz_t TORSION_PLUS_2POWER;
extern const ibz_t TORSION_PLUS_3POWER;
extern const ibz_t TORSION_PLUS_23POWER;
extern const ibz_t DEGREE_COMMITMENT;
extern const ibz_t DEGREE_COMMITMENT_PLUS;
extern const ibz_t DEGREE_COMMITMENT_MINUS;
extern const ibz_t DEGREE_CHALLENGE;
#endif
