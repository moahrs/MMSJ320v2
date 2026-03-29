#include "float_alt_q16.h"

static q16_t clamp_i64_to_q16(int64_t v)
{
    if (v > 0x7FFFFFFFLL) {
        return (q16_t)0x7FFFFFFF;
    }
    if (v < -2147483648LL) {
        return (q16_t)0x80000000;
    }
    return (q16_t)v;
}

q16_t q16_from_int(int32_t v) { return (q16_t)(v << Q16_SHIFT); }
int32_t q16_to_int(q16_t v) { return (int32_t)(v >> Q16_SHIFT); }
q16_t q16_add(q16_t a, q16_t b) { return (q16_t)(a + b); }
q16_t q16_sub(q16_t a, q16_t b) { return (q16_t)(a - b); }
q16_t q16_abs(q16_t a) { return a < 0 ? (q16_t)(-a) : a; }
q16_t q16_neg(q16_t a) { return (q16_t)(-a); }

q16_t q16_mul(q16_t a, q16_t b)
{
    int64_t t = (int64_t)a * (int64_t)b;
    t >>= Q16_SHIFT;
    return clamp_i64_to_q16(t);
}

q16_t q16_div(q16_t a, q16_t b)
{
    if (b == 0) {
        return a >= 0 ? (q16_t)0x7FFFFFFF : (q16_t)0x80000000;
    }
    return (q16_t)(((int64_t)a << Q16_SHIFT) / b);
}

q16_t q16_sqrt(q16_t x)
{
    uint64_t n;
    uint64_t r;
    uint64_t prev;
    int i;

    if (x <= 0) {
        return 0;
    }

    n = ((uint64_t)(uint32_t)x) << Q16_SHIFT;
    r = n;
    prev = 0;

    for (i = 0; i < 20 && r != prev; i++) {
        prev = r;
        r = (r + n / r) >> 1;
    }

    return (q16_t)r;
}

static q16_t q16_wrap_pi(q16_t x)
{
    const q16_t q16_pi = 205887;
    const q16_t q16_two_pi = 411775;

    while (x > q16_pi) {
        x -= q16_two_pi;
    }
    while (x < -q16_pi) {
        x += q16_two_pi;
    }
    return x;
}

q16_t q16_sin(q16_t x)
{
    q16_t x2;
    q16_t x3;
    q16_t x5;
    q16_t x7;

    x = q16_wrap_pi(x);
    x2 = q16_mul(x, x);
    x3 = q16_mul(x2, x);
    x5 = q16_mul(x3, x2);
    x7 = q16_mul(x5, x2);

    return q16_sub(
        q16_add(x, q16_div(q16_neg(x3), q16_from_int(6))),
        q16_sub(q16_div(q16_neg(x7), q16_from_int(5040)), q16_div(x5, q16_from_int(120)))
    );
}

q16_t q16_cos(q16_t x)
{
    const q16_t q16_half_pi = 102944;
    return q16_sin((q16_t)(x + q16_half_pi));
}

q16_t q16_tan(q16_t x)
{
    q16_t c = q16_cos(x);
    if (c == 0) {
        return x >= 0 ? (q16_t)0x7FFFFFFF : (q16_t)0x80000000;
    }
    return q16_div(q16_sin(x), c);
}

q16_t q16_exp(q16_t x)
{
    q16_t x2 = q16_mul(x, x);
    q16_t x3 = q16_mul(x2, x);
    q16_t x4 = q16_mul(x3, x);
    q16_t res = Q16_ONE;

    res = q16_add(res, x);
    res = q16_add(res, q16_div(x2, q16_from_int(2)));
    res = q16_add(res, q16_div(x3, q16_from_int(6)));
    res = q16_add(res, q16_div(x4, q16_from_int(24)));
    return res;
}

q16_t q16_ln(q16_t x)
{
    q16_t y;
    q16_t y2;
    q16_t y3;
    q16_t y5;
    q16_t y7;
    q16_t sum;

    if (x <= 0) {
        return (q16_t)0x80000000;
    }

    y = q16_div(q16_sub(x, Q16_ONE), q16_add(x, Q16_ONE));
    y2 = q16_mul(y, y);
    y3 = q16_mul(y2, y);
    y5 = q16_mul(y3, y2);
    y7 = q16_mul(y5, y2);

    sum = y;
    sum = q16_add(sum, q16_div(y3, q16_from_int(3)));
    sum = q16_add(sum, q16_div(y5, q16_from_int(5)));
    sum = q16_add(sum, q16_div(y7, q16_from_int(7)));

    return q16_mul(q16_from_int(2), sum);
}

q16_t q16_from_str(const char *s)
{
    int sign = 1;
    int32_t ip = 0;
    int32_t frac = 0;
    int32_t scale = 1;
    int dec_count = 0;
    q16_t out;

    if (!s) {
        return 0;
    }

    while (*s == ' ' || *s == '\t') {
        s++;
    }

    if (*s == '-') {
        sign = -1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    while (*s >= '0' && *s <= '9') {
        ip = (ip * 10) + (int32_t)(*s - '0');
        s++;
    }

    if (*s == '.') {
        s++;
        while (*s >= '0' && *s <= '9') {
            if (dec_count >= 6) {
                break;
            }
            frac = (frac * 10) + (int32_t)(*s - '0');
            scale *= 10;
            dec_count++;
            s++;
        }
    }

    out = q16_from_int(ip);
    if (scale > 1) {
        out = q16_add(out, q16_div(q16_from_int(frac), q16_from_int(scale)));
    }

    return sign < 0 ? q16_neg(out) : out;
}

int q16_to_str(q16_t v, char *out, size_t out_len, uint8_t decimals)
{
    int neg = 0;
    uint32_t ip;
    uint32_t fp;
    uint32_t base = 1;
    uint32_t div;
    uint32_t frac_scaled;
    size_t i = 0;
    char tmp[16];
    size_t t = 0;
    uint8_t d;

    if (!out || out_len < 4) {
        return -1;
    }

    if (decimals > 6) {
        decimals = 6;
    }

    if (v < 0) {
        neg = 1;
        v = (q16_t)(-v);
    }

    ip = (uint32_t)(v >> Q16_SHIFT);
    fp = (uint32_t)(v & 0xFFFF);

    while (t == 0 || ip > 0) {
        tmp[t++] = (char)('0' + (ip % 10));
        ip /= 10;
    }

    if (neg) {
        if (i + 1 >= out_len) return -1;
        out[i++] = '-';
    }

    while (t > 0) {
        if (i + 1 >= out_len) return -1;
        out[i++] = tmp[--t];
    }

    if (decimals > 0) {
        if (i + 1 >= out_len) return -1;
        out[i++] = '.';

        for (d = 0; d < decimals; d++) {
            base *= 10;
        }
        frac_scaled = (uint32_t)(((uint64_t)fp * base) >> Q16_SHIFT);
        div = base / 10;

        for (d = 0; d < decimals; d++) {
            if (i + 1 >= out_len) return -1;
            out[i++] = (char)('0' + ((frac_scaled / div) % 10));
            if (div > 1) {
                div /= 10;
            }
        }
    }

    out[i] = '\0';
    return 0;
}
