
#include <stddef.h>

static unsigned char heap_buffer[64 * 1024];
static size_t heap_used;

void *memcpy(void *dest, const void *src, size_t n) {
    char *d = dest;
    const char *s = src;
    while (n--) *d++ = *s++;
    return dest;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) *p++ = (unsigned char)c;
    return s;
}

size_t strlen(const char *s) {
    size_t n = 0;
    while (s[n] != '\0') n++;
    return n;
}

char *strcpy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++) != '\0') {
    }
    return dest;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (int)((unsigned char)*s1 - (unsigned char)*s2);
}

static long parse_int(const char *s) {
    long sign = 1;
    long v = 0;
    if (*s == '-') {
        sign = -1;
        s++;
    }
    while (*s >= '0' && *s <= '9') {
        v = (v * 10) + (*s - '0');
        s++;
    }
    return sign * v;
}

long atol(const char *nptr) {
    return parse_int(nptr);
}

static char *to_base(unsigned long value, char *str, int base) {
    static const unsigned long dec_divs[] = {
        1000000000UL, 100000000UL, 10000000UL, 1000000UL, 100000UL,
        10000UL, 1000UL, 100UL, 10UL, 1UL
    };
    int i = 0;

    if (base != 10 && base != 16) {
        str[0] = '\0';
        return str;
    }

    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    if (base == 16) {
        int shift;
        int started = 0;
        for (shift = 28; shift >= 0; shift -= 4) {
            unsigned long d = (value >> (unsigned long)shift) & 0xFUL;
            if (!started && d == 0) {
                continue;
            }
            started = 1;
            str[i++] = (char)(d < 10 ? ('0' + d) : ('a' + (d - 10)));
        }
        str[i] = '\0';
        return str;
    }

    {
        int idx;
        int started = 0;
        for (idx = 0; idx < (int)(sizeof(dec_divs) / sizeof(dec_divs[0])); idx++) {
            unsigned long div = dec_divs[idx];
            unsigned char digit = 0;
            while (value >= div) {
                value -= div;
                digit++;
            }
            if (digit || started) {
                str[i++] = (char)('0' + digit);
                started = 1;
            }
        }
    }

    str[i] = '\0';
    return str;
}

char *ltoa(long value, char *str, int base) {
    if (base == 10 && value < 0) {
        str[0] = '-';
        to_base((unsigned long)(-value), str + 1, base);
        return str;
    }
    return to_base((unsigned long)value, str, base);
}

char *itoa(int value, char *str, int base) {
    return ltoa((long)value, str, base);
}

void *malloc(size_t size) {
    size_t align = 4;
    size_t aligned_size = (size + (align - 1)) & ~(align - 1);
    if (aligned_size == 0 || heap_used + aligned_size > sizeof(heap_buffer)) {
        return NULL;
    }
    void *ptr = &heap_buffer[heap_used];
    heap_used += aligned_size;
    return ptr;
}

void free(void *ptr) {
    (void)ptr;
}

void *realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return malloc(size);
    }
    void *new_ptr = malloc(size);
    if (new_ptr == NULL) {
        return NULL;
    }
    return new_ptr;
}

    int atoi(const char *nptr) {
        return (int)atol(nptr);
    }

    char *strcat(char *dest, const char *src) {
        char *d = dest;
        while (*d) d++;
        while ((*d++ = *src++) != '\0') {}
        return dest;
    }

    char *strncat(char *dest, const char *src, size_t n) {
        char *d = dest;
        while (*d) d++;
        while (n-- && (*d = *src++) != '\0') d++;
        *d = '\0';
        return dest;
    }

    char *strchr(const char *s, int c) {
        for (; *s != '\0'; s++)
            if ((unsigned char)*s == (unsigned char)c) return (char *)s;
        if (c == '\0') return (char *)s;
        return NULL;
    }

    char *strrchr(const char *s, int c) {
        const char *last = NULL;
        for (; *s != '\0'; s++)
            if ((unsigned char)*s == (unsigned char)c) last = s;
        if (c == '\0') return (char *)s;
        return (char *)last;
    }

    int strncmp(const char *s1, const char *s2, size_t n) {
        while (n-- && *s1 != '\0' && *s1 == *s2) { s1++; s2++; }
        if (!n || (*s1 == '\0' && *s2 == '\0')) return 0;
        return (int)((unsigned char)*s1 - (unsigned char)*s2);
    }

    char *strncpy(char *dest, const char *src, size_t n) {
        char *d = dest;
        while (n && (*d++ = *src++) != '\0') n--;
        while (n-- > 1) *d++ = '\0';
        return dest;
    }

    /* _ctype_ table: index 0 = EOF, index 1..256 = chars 0..255
     * Bits: _U=0x01 upper, _L=0x02 lower, _N=0x04 digit,
     *       _S=0x08 space, _P=0x10 punct, _C=0x20 ctrl,
     *       _X=0x40 hex digit, _B=0x80 blank */
    const char _ctype_[257] = {
        0,                  /* EOF */
        0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,  /* 0x00-0x07 ctrl */
        0xA8,               /* 0x09 \t: ctrl+space+blank */
        0x28,0x28,0x28,0x28,/* 0x0A-0x0D \n\v\f\r: ctrl+space */
        0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,  /* 0x0E-0x15 ctrl */
        0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,  /* 0x16-0x1D ctrl */
        0x20,0x20,          /* 0x1E-0x1F ctrl */
        0x88,               /* 0x20 ' ': space+blank */
        0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,  /* 0x21-0x28 punct */
        0x10,0x10,0x10,0x10,0x10,0x10,0x10,        /* 0x29-0x2F punct */
        0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,  /* 0x30-0x37 digits + hex */
        0x44,0x44,          /* 0x38-0x39 digits + hex */
        0x10,0x10,0x10,0x10,0x10,0x10,0x10,        /* 0x3A-0x40 punct */
        0x41,0x41,0x41,0x41,0x41,0x41,             /* 0x41-0x46 A-F: upper+hex */
        0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,  /* 0x47-0x4E G-N upper */
        0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,  /* 0x4F-0x56 O-V upper */
        0x01,0x01,0x01,0x01,/* 0x57-0x5A W-Z upper */
        0x10,0x10,0x10,0x10,0x10,0x10,             /* 0x5B-0x60 punct */
        0x42,0x42,0x42,0x42,0x42,0x42,             /* 0x61-0x66 a-f: lower+hex */
        0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,  /* 0x67-0x6E g-n lower */
        0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,  /* 0x6F-0x76 o-v lower */
        0x02,0x02,0x02,0x02,/* 0x77-0x7A w-z lower */
        0x10,0x10,0x10,0x10,/* 0x7B-0x7E punct */
        0x20,               /* 0x7F DEL ctrl */
        /* 0x80-0xFF: 128 zeroes */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
