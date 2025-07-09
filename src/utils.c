#include "utils.h"

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
