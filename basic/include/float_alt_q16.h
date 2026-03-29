#ifndef FLOAT_ALT_Q16_H
#define FLOAT_ALT_Q16_H

#include <stddef.h>
#include <stdint.h>

/*
 * Q16.16 fixed-point alternative for Motorola FFP.
 *
 * Important:
 * - This is NOT binary-compatible with Motorola FFP format.
 * - It is an optional software math backend for evaluation.
 */

typedef int32_t q16_t;

#define Q16_SHIFT 16
#define Q16_ONE   ((q16_t)0x00010000)

q16_t q16_from_int(int32_t v);
int32_t q16_to_int(q16_t v);
q16_t q16_add(q16_t a, q16_t b);
q16_t q16_sub(q16_t a, q16_t b);
q16_t q16_mul(q16_t a, q16_t b);
q16_t q16_div(q16_t a, q16_t b);
q16_t q16_abs(q16_t a);
q16_t q16_neg(q16_t a);
q16_t q16_sqrt(q16_t x);
q16_t q16_sin(q16_t x);
q16_t q16_cos(q16_t x);
q16_t q16_tan(q16_t x);
q16_t q16_exp(q16_t x);
q16_t q16_ln(q16_t x);

/* Decimal helpers (no exponent support in parser). */
q16_t q16_from_str(const char *s);
int q16_to_str(q16_t v, char *out, size_t out_len, uint8_t decimals);

#endif
