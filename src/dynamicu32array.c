#include "global.h"
#include "recomputils.h"
#include "dynamicu32array.h"
#include "recompdata.h"
#include "logger.h"

static U32HashsetHandle sValidU32Arrays;

#define PRINT_INVALID_PTR_ERR() Logger_printError("Received invalid DynamicU32Array pointer.")

static bool isValidU32Array(const DynamicU32Array *dArr) {
    return sValidU32Arrays && recomputil_u32_hashset_contains(sValidU32Arrays, (uintptr_t)dArr);
}

#define DEFAULT_CAPACITY 1
#define NEXT_CAPACITY(current) ((current == 0) ? DEFAULT_CAPACITY : (current + (current + 1) / 2))

bool DynU32Arr_resize(DynamicU32Array *dArr, size_t newCapacity) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (newCapacity > 0) {
        u32 *newData = recomp_alloc(sizeof(*newData) * newCapacity);

        if (!newData) {
            return false;
        }

        size_t min = dArr->count;
        if (min > newCapacity) {
            min = newCapacity;
        }

        for (size_t i = 0; i < min; ++i) {
            newData[i] = dArr->data[i];
        }

        recomp_free(dArr->data);

        dArr->count = min;
        dArr->capacity = newCapacity;
        dArr->data = newData;
    }

    return true;
}

void DynU32Arr_reserve(DynamicU32Array *dArr, size_t minimumCapacity) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    if (minimumCapacity > dArr->capacity) {
        if (!DynU32Arr_resize(dArr, dArr->capacity)) {
            Logger_printError("Failed to reserve new capacity!");
        }
    }
}

void DynU32Arr_shrinkToFit(DynamicU32Array *dArr) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    if (dArr->count < dArr->capacity) {
        if (!DynU32Arr_resize(dArr, dArr->count)) {
            Logger_printError("Failed to reserve new capacity!");
        }
    }
}

u32 *DynU32Arr_data(const DynamicU32Array *dArr) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return dArr->data;
}

size_t DynU32Arr_size(const DynamicU32Array *dArr) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return 0;
    }

    return dArr->count;
}

bool DynU32Arr_init(DynamicU32Array *dArr) {
    dArr->capacity = 0;
    dArr->count = 0;
    dArr->data = NULL;

    if (!sValidU32Arrays) {
        sValidU32Arrays = recomputil_create_u32_hashset();
    }

    recomputil_u32_hashset_insert(sValidU32Arrays, (uintptr_t)dArr);

    return true;
}

bool DynU32Arr_get(const DynamicU32Array *dArr, size_t index, u32 *out) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (dArr->count >= index) {
        Logger_printError("Received out of range index! (Array size is %d and index is %d)", dArr->count, index);
        return false;
    }

    if (!out) {
        Logger_printError("out is NULL!");
        return false;
    }

    *out = dArr->data[index];
    return true;
}

bool DynU32Arr_set(DynamicU32Array *dArr, size_t index, u32 value) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (dArr->count >= index) {
        Logger_printError("Received out of range index! (Array size is %d and index is %d)", dArr->count, index);
    }

    dArr->data[index] = value;
    return true;
}

void DynU32Arr_clear(DynamicU32Array *dArr) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    dArr->count = 0;
}

void DynU32Arr_destroyMembers(DynamicU32Array *dArr) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    recomp_free(dArr->data);
    dArr->capacity = 0;
    dArr->count = 0;
    dArr->data = NULL;
    recomputil_u32_hashset_erase(sValidU32Arrays, (uintptr_t)dArr);
}

void DynU32Arr_push(DynamicU32Array *dArr, u32 value) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    size_t newCount = dArr->count + 1;

    if (newCount > dArr->capacity) {
        if (!DynU32Arr_resize(dArr, NEXT_CAPACITY(dArr->capacity))) {
            Logger_printError("Failed to allocate larger array!");
            return;
        }
    }

    dArr->data[dArr->count] = value;

    dArr->count = newCount;
}

bool DynU32Arr_pop(DynamicU32Array *dArr) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (dArr->count > 0) {
        dArr->count--;
        return true;
    }

    return false;
}

bool DynU32Arr_popAndGet(DynamicU32Array *dArr, u32 *out) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (!out) {
        Logger_printError("out is NULL!");
        return false;
    }

    u32 *last = DynU32Arr_back(dArr);

    if (last) {
        *out = *last;
        return DynU32Arr_pop(dArr);
    }

    return false;
}

u32 *DynU32Arr_front(const DynamicU32Array *dArr) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    if (dArr->count > 0) {
        return &dArr->data[0];
    }

    return NULL;
}

u32 *DynU32Arr_back(const DynamicU32Array *dArr) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    if (dArr->count < 1) {
        return NULL;
    }

    return &dArr->data[dArr->count - 1];
}

u32 *DynU32Arr_end(const DynamicU32Array *dArr) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    if (dArr->count < 1) {
        return NULL;
    }

    return dArr->data + dArr->count;
}

bool DynU32Arr_contains(const DynamicU32Array *dArr, u32 value) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    size_t dummy;
    return DynU32Arr_indexOf(dArr, value, &dummy);
}

bool DynU32Arr_indexOf(const DynamicU32Array *dArr, u32 value, size_t *out) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    for (size_t i = 0; i < dArr->count; ++i) {
        if (dArr->data[i] == value) {
            if (out) {
                *out = i;
            }
            return true;
        }
    }

    return false;
}

bool DynU32Arr_removeByValue(DynamicU32Array *dArr, u32 value) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (dArr->count > 0) {
        u32 *arr = dArr->data;
        for (size_t i = 0; i < dArr->count; ++i) {
            if (arr[i] == value) {
                DynU32Arr_removeByIndex(dArr, i);
                return true;
            }
        }
    }

    return false;
}

bool DynU32Arr_removeByIndex(DynamicU32Array *dArr, size_t index) {
    if (!isValidU32Array(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (index >= dArr->count) {
        Logger_printError("Received out of range index! (Array size is %d and index is %d)", dArr->count, index);
        return false;
    }

    for (size_t i = index + 1; i < dArr->count; ++i) {
        dArr->data[i - 1] = dArr->data[i];
    }

    dArr->count--;

    return true;
}
