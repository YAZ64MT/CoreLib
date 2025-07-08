#include "utils.h"

void Utils_MemCpy(void *dest, const void *src, size_t size) {
    char *d = dest;
    const char *s = src;

    for (size_t i = 0; i < size; ++i) {
        *d = *s;
        d++;
        s++;
    }
}

int Utils_MemCmp(const void *a, const void *b, size_t size) {
    const char *c = a;
    const char *d = b;

    for (size_t i = 0; i < size; ++i) {
        if (*c != *d) {
            return *c - *d;
        }

        c++;
        d++;
    }

    return 0;
}
