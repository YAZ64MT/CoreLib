#ifndef __DYNAMICU32ARRAY_H__
#define __DYNAMICU32ARRAY_H__

// Array of 32-bit integers that automatically expands as it fills.

#include "libc/stdbool.h"
#include "PR/ultratypes.h"

typedef struct DynamicU32Array {
    u32 *data;
    size_t capacity;
    size_t count;
} DynamicU32Array;

bool DynU32Arr_init(DynamicU32Array *dArr);

bool DynU32Arr_get(const DynamicU32Array *dArr, size_t index, u32 *out);

bool DynU32Arr_set(DynamicU32Array *dArr, size_t index, u32 value);

void DynU32Arr_clear(DynamicU32Array *dArr);

void DynU32Arr_destroyMembers(DynamicU32Array *dArr);

void DynU32Arr_push(DynamicU32Array *dArr, u32 value);

bool DynU32Arr_pop(DynamicU32Array *dArr);

bool DynU32Arr_popAndGet(DynamicU32Array *dArr, u32 *out);

u32 *DynU32Arr_front(DynamicU32Array *dArr);

u32 *DynU32Arr_back(DynamicU32Array *dArr);

u32 *DynU32Arr_end(DynamicU32Array *dArr);

bool DynU32Arr_contains(const DynamicU32Array *dArr, u32 value);

bool DynU32Arr_indexOf(const DynamicU32Array *dArr, u32 value, size_t *out);

bool DynU32Arr_removeByValue(DynamicU32Array *dArr, u32 value);

bool DynU32Arr_removeByIndex(DynamicU32Array *dArr, size_t index);

bool DynU32Arr_resize(DynamicU32Array *dArr, size_t newCapacity);

void DynU32Arr_reserve(DynamicU32Array *dArr, size_t minimumCapacity);

void DynU32Arr_shrinkToFit(DynamicU32Array *dArr);

u32 *DynU32Arr_data(DynamicU32Array *dArr);

size_t DynU32Arr_size(const DynamicU32Array *dArr);

#endif
