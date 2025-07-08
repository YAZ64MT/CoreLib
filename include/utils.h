#ifndef __UTILS_H__
#define __UTILS_H__

#include "global.h"

void Utils_MemCpy(void *dest, const void *src, size_t size);

int Utils_MemCmp(const void *a, const void *b, size_t size);

#endif
