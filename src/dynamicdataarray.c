#include "global.h"
#include "recomputils.h"
#include "dynamicdataarray.h"
#include "utils.h"
#include "libc/string.h"
#include "recompdata.h"
#include "logger.h"

static U32HashsetHandle sValidDataArrays;

#define PRINT_INVALID_PTR_ERR() Logger_printError("Received invalid DynamicDataArray pointer.")

static bool isValidDataArray(const DynamicDataArray *dArr) {
    return sValidDataArrays && recomputil_u32_hashset_contains(sValidDataArrays, (uintptr_t)dArr);
}

#define DEFAULT_CAPACITY 1
#define NEXT_CAPACITY(current) ((current == 0) ? DEFAULT_CAPACITY : ((current + 1) * 3 / 2))

bool DynDataArr_resize(DynamicDataArray *dArr, size_t newCapacity) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (newCapacity > 0) {
        size_t newByteSize = newCapacity * dArr->elementSize;
        u32 *newData = recomp_alloc(newByteSize);

        if (!newData) {
            Logger_printError("Could not allocate space for new DynamicDataArray.");
            return false;
        }

        Lib_MemSet(newData, 0, newByteSize);

        size_t min = dArr->count;
        if (min > newCapacity) {
            min = newCapacity;
        }

        size_t newCount = dArr->count;
        if (newCapacity < dArr->count) {
            newCount = newCapacity;
        }

        if (dArr->data) {
            memcpy(newData, dArr->data, min * dArr->elementSize);
            recomp_free(dArr->data);
        }

        dArr->count = newCount;
        dArr->capacity = newCapacity;
        dArr->data = newData;
    } else {
        dArr->capacity = 0;
        dArr->count = 0;
        recomp_free(dArr->data);
        dArr->data = NULL;
    }

    return true;
}

void *DynDataArr_data(const DynamicDataArray *dArr) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return dArr->data;
}

size_t DynDataArr_size(const DynamicDataArray *dArr) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return 0;
    }

    return dArr->count;
}

static void resetStruct(DynamicDataArray *dArr) {
    dArr->capacity = 0;
    dArr->count = 0;
    dArr->elementSize = 0;
    dArr->data = NULL;
}

bool DynDataArr_init(DynamicDataArray *dArr, size_t elementSize, size_t initialCapacity) {
    if (elementSize < 1) {
        return false;
    }

    resetStruct(dArr);

    dArr->elementSize = elementSize;

    if (!sValidDataArrays) {
        sValidDataArrays = recomputil_create_u32_hashset();
    }

    recomputil_u32_hashset_insert(sValidDataArrays, (uintptr_t)dArr);

    if (initialCapacity) {
        if (!DynDataArr_resize(dArr, initialCapacity)) {
            DynDataArr_destroyMembers(dArr);
            return false;
        }
    }

    return true;
}

void DynDataArr_clear(DynamicDataArray *dArr) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    dArr->count = 0;
}

void DynDataArr_destroyMembers(DynamicDataArray *dArr) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    recomp_free(dArr->data);
    resetStruct(dArr);

    recomputil_u32_hashset_erase(sValidDataArrays, (uintptr_t)dArr);
}

void *DynDataArr_createElement(DynamicDataArray *dArr) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    size_t newCount = dArr->count + 1;

    if (newCount > dArr->capacity) {
        if (!DynDataArr_resize(dArr, NEXT_CAPACITY(dArr->capacity))) {
            return NULL;
        }
    }

    u8 *data = dArr->data;

    dArr->count = newCount;

    void *element = DynDataArr_get(dArr, dArr->count - 1);

    return element;
}

static void *rawGet(const DynamicDataArray *dArr, size_t index) {
    u8 *data = dArr->data;

    return &data[dArr->elementSize * index];
}

void *DynDataArr_get(const DynamicDataArray *dArr, size_t index) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    if (index >= dArr->count) {
        Logger_printError("Received out of range index! (Array size is %d and index is %d)", dArr->count, index);
        return NULL;
    }

    return rawGet(dArr, index);
}

bool DynDataArr_set(DynamicDataArray *dArr, size_t index, const void *value) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (index >= dArr->count) {
        Logger_printError("Received out of range index! (Array size is %d and index is %d)", dArr->count, index);
        return false;
    }

    void *dest = DynDataArr_get(dArr, index);
    if (!dest) {
        Logger_printError("Failed to get pointer to data at target index!");
        return false;
    }

    memcpy(dest, value, dArr->elementSize);
    return true;
}

void DynDataArr_push(DynamicDataArray *dArr, void *value) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    if (!value) {
        Logger_printError("Passed in value was NULL!");
        return;
    }

    void *newElement = DynDataArr_createElement(dArr);

    if (newElement) {
        memcpy(newElement, value, dArr->elementSize);
    } else {
        Logger_printError("Failed to create a new element at the end of the array!");
    }
}

bool DynDataArr_pop(DynamicDataArray *dArr) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    if (dArr->count < 1) {
        return false;
    }

    dArr->count--;

    return true;
}

void *DynDataArr_first(const DynamicDataArray *dArr) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    if (dArr->count > 0) {
        return DynDataArr_get(dArr, 0);
    }

    return NULL;
}

void *DynDataArr_last(const DynamicDataArray *dArr) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    if (dArr->count > 0) {
        return DynDataArr_get(dArr, dArr->count - 1);
    }

    return NULL;
}

void *DynDataArr_end(const DynamicDataArray *dArr) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    if (dArr->count > 0) {
        return rawGet(dArr, dArr->count);
    }

    return NULL;
}

bool DynDataArr_contains(const DynamicDataArray *dArr, const void *value) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    size_t dummy;
    return DynDataArr_indexOf(dArr, value, &dummy);
}

bool DynDataArr_indexOf(const DynamicDataArray *dArr, const void *value, size_t *out) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (!out) {
        Logger_printError("out is NULL!");
        return false;
    }

    for (size_t i = 0; i < dArr->count; ++i) {
        if (Utils_MemCmp(DynDataArr_get(dArr, i), value, dArr->elementSize) == 0) {
            *out = i;
            return true;
        }
    }

    return false;
}

bool DynDataArr_removeByIndex(DynamicDataArray *dArr, size_t index) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (index >= dArr->count) {
        Logger_printError("Received out of range index! (Array size is %d and index is %d)", dArr->count, index);
        return false;
    }

    u8 *data = dArr->data;

    for (size_t i = index + 1; i < dArr->count - 1; ++i) {
        void *src = DynDataArr_get(dArr, i + 1);
        void *dest = DynDataArr_get(dArr, i);

        if (src && dest) {
            memcpy(dest, src, dArr->elementSize);
        }
    }

    dArr->count--;

    return true;
}

bool DynDataArr_removeByValue(DynamicDataArray *dArr, const void *value) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    size_t index;
    if (DynDataArr_indexOf(dArr, value, &index)) {
        DynDataArr_removeByIndex(dArr, index);
    }

    return false;
}

void DynDataArr_reserve(DynamicDataArray *dArr, size_t minimumCapacity) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    if (minimumCapacity > dArr->count) {
        DynDataArr_resize(dArr, minimumCapacity);
    }
}

void DynDataArr_shrinkToFit(DynamicDataArray *dArr) {
    if (!isValidDataArray(dArr)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    if (dArr->count < dArr->capacity) {
        DynDataArr_resize(dArr, dArr->count);
    }
}
