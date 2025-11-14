#ifndef __DYNAMICDATAARRAY_H__
#define __DYNAMICDATAARRAY_H__

// Array of arbitrarily sized values that automatically expands as it fills.

#include "libc/stdbool.h"
#include "PR/ultratypes.h"

typedef struct DynamicDataArray {
    void *data;
    size_t capacity;
    size_t count;
    size_t elementSize;
} DynamicDataArray;

bool DynDataArr_init(DynamicDataArray *dArr, size_t elementSize, size_t initialCapacity);

void DynDataArr_clear(DynamicDataArray *dArr);

void DynDataArr_destroyMembers(DynamicDataArray *dArr);

void *DynDataArr_createElement(DynamicDataArray *dArr);

void *DynDataArr_get(const DynamicDataArray *dArr, size_t index);

bool DynDataArr_set(DynamicDataArray *dArr, size_t index, const void *value);

void DynDataArr_push(DynamicDataArray *dArr, void *value);

bool DynDataArr_pop(DynamicDataArray *dArr);

void *DynDataArr_first(const DynamicDataArray *dArr);

void *DynDataArr_last(const DynamicDataArray *dArr);

void *DynDataArr_end(const DynamicDataArray *dArr);

bool DynDataArr_contains(const DynamicDataArray *dArr, const void *value);

bool DynDataArr_indexOf(const DynamicDataArray *dArr, const void *value, size_t *out);

bool DynDataArr_removeByIndex(DynamicDataArray *dArr, size_t index);

bool DynDataArr_removeByValue(DynamicDataArray *dArr, const void *value);

void DynDataArr_reserve(DynamicDataArray *dArr, size_t minimumCapacity);

void DynDataArr_shrinkToFit(DynamicDataArray *dArr);

bool DynDataArr_resize(DynamicDataArray *dArr, size_t newCapacity);

void *DynDataArr_data(const DynamicDataArray *dArr);

size_t DynDataArr_size(const DynamicDataArray *dArr);

#endif
