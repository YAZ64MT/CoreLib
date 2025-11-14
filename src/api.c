#include "dynamicu32array.h"
#include "dynamicdataarray.h"
#include "stringu32dictionary.h"
#include "iterableu32set.h"
#include "utils.h"
#include "modding.h"
#include "recomputils.h"
#include "logger.h"

// ------------------------
// Utility Function Exports
// ------------------------

RECOMP_EXPORT int YAZMTCore_Utils_MemCmp(void *dest, const void *src, size_t size) {
    return Utils_MemCmp(dest, src, size);
}

RECOMP_EXPORT char *YAZMTCore_Utils_StrDup(const char* s) {
    return Utils_StrDup(s);
}

// -----------------------
// DynimicU32Array Exports
// -----------------------

RECOMP_EXPORT DynamicU32Array *YAZMTCore_DynamicU32Array_new() {
    DynamicU32Array *d = recomp_alloc(sizeof(DynamicU32Array));

    if (!d) {
        Logger_printError("Could not allocate space for DynamicU32Array struct!");
        return NULL;
    }

    if (!DynU32Arr_init(d)) {
        Logger_printError("Failed to init DynamicU32Array!");
        recomp_free(d);
        return NULL;
    }

    return d;
}

RECOMP_EXPORT void YAZMTCore_DynamicU32Array_destroy(DynamicU32Array *d) {
    if (d) {
        DynU32Arr_destroyMembers(d);
        recomp_free(d);
    }
}

RECOMP_EXPORT void YAZMTCore_DynamicU32Array_clear(DynamicU32Array *d) {
    DynU32Arr_clear(d);
}

RECOMP_EXPORT bool YAZMTCore_DynamicU32Array_get(const DynamicU32Array *d, size_t index, u32 *out) {
    return DynU32Arr_get(d, index, out);
}

RECOMP_EXPORT bool YAZMTCore_DynamicU32Array_set(DynamicU32Array *d, size_t index, u32 value) {
    return DynU32Arr_set(d, index, value);
}

RECOMP_EXPORT void YAZMTCore_DynamicU32Array_push(DynamicU32Array *d, u32 v) {
    DynU32Arr_push(d, v);
}

RECOMP_EXPORT bool YAZMTCore_DynamicU32Array_pop(DynamicU32Array *d) {
    return DynU32Arr_pop(d);
}

RECOMP_EXPORT bool YAZMTCore_DynamicU32Array_popAndGet(DynamicU32Array *d, u32 *out) {
    return DynU32Arr_popAndGet(d, out);
}

RECOMP_EXPORT bool YAZMTCore_DynamicU32Array_contains(const DynamicU32Array *d, u32 value) {
    return DynU32Arr_contains(d, value);
}

RECOMP_EXPORT bool YAZMTCore_DynamicU32Array_indexOf(const DynamicU32Array *d, u32 value, size_t *out) {
    return DynU32Arr_indexOf(d, value, out);
}

RECOMP_EXPORT bool YAZMTCore_DynamicU32Array_removeByValue(DynamicU32Array *d, u32 v) {
    return DynU32Arr_removeByValue(d, v);
}

RECOMP_EXPORT bool YAZMTCore_DynamicU32Array_removeByIndex(DynamicU32Array *d, size_t i) {
    return DynU32Arr_removeByIndex(d, i);
}

RECOMP_EXPORT void YAZMTCore_DynamicU32Array_reserve(DynamicU32Array *d, size_t minimum) {
    DynU32Arr_reserve(d, minimum);
}

RECOMP_EXPORT u32 *YAZMTCore_DynamicU32Array_data(const DynamicU32Array *d) {
    return DynU32Arr_data(d);
}

RECOMP_EXPORT size_t YAZMTCore_DynamicU32Array_size(const DynamicU32Array *d) {
    return DynU32Arr_size(d);
}

RECOMP_EXPORT u32 *YAZMTCore_DynamicU32Array_front(const DynamicU32Array *d) {
    return DynU32Arr_front(d);
}

RECOMP_EXPORT u32 *YAZMTCore_DynamicU32Array_back(const DynamicU32Array *d) {
    return DynU32Arr_back(d);
}

RECOMP_EXPORT u32 *YAZMTCore_DynamicU32Array_end(const DynamicU32Array *d) {
    return DynU32Arr_end(d);
}

RECOMP_EXPORT void YAZMTCore_DynamicU32Array_shrinkToFit(DynamicU32Array *d) {
    DynU32Arr_shrinkToFit(d);
}

// ------------------------
// DynamicDataArray exports
// ------------------------

RECOMP_EXPORT DynamicDataArray *YAZMTCore_DynamicDataArray_new(size_t elementSize) {
    if (elementSize < 1) {
        Logger_printError("Cannot allocate a DynamicDataArray with element size 0!");
        return NULL;
    }

    DynamicDataArray *d = recomp_alloc(sizeof(DynamicDataArray));

    if (!d) {
        Logger_printError("Could not allocate space for DynamicDataArray struct!");
        return NULL;
    }

    if (!DynDataArr_init(d, elementSize, 0)) {
        Logger_printError("Failed to init DynamicDataArray!");
        recomp_free(d);
        return NULL;
    }

    return d;
}

RECOMP_EXPORT void YAZMTCore_DynamicDataArray_destroy(DynamicDataArray *d) {
    if (d) {
        DynDataArr_destroyMembers(d);
        recomp_free(d);
    }
}

RECOMP_EXPORT void YAZMTCore_DynamicDataArray_clear(DynamicDataArray *d) {
    DynDataArr_clear(d);
}

RECOMP_EXPORT bool YAZMTCore_DynamicDataArray_contains(const DynamicDataArray *d, const void *value) {
    return DynDataArr_contains(d, value);
}

RECOMP_EXPORT bool YAZMTCore_DynamicData_indexOf(const DynamicDataArray *d, const void *value, size_t *out) {
    return DynDataArr_indexOf(d, value, out);
}

RECOMP_EXPORT void *YAZMTCore_DynamicDataArray_createElement(DynamicDataArray *d) {
    return DynDataArr_createElement(d);
}

RECOMP_EXPORT void *YAZMTCore_DynamicDataArray_getElement(const DynamicDataArray *d, size_t i) {
    return DynDataArr_get(d, i);
}

RECOMP_EXPORT bool YAZMTCore_DynamicDataArray_setElement(DynamicDataArray *d, size_t i, const void *value) {
    return DynDataArr_set(d, i, value);
}

RECOMP_EXPORT void YAZMTCore_DynamicDataArray_push(DynamicDataArray *d, void *v) {
    DynDataArr_push(d, v);
}

RECOMP_EXPORT bool YAZMTCore_DynamicDataArray_pop(DynamicDataArray *d) {
    return DynDataArr_pop(d);
}

RECOMP_EXPORT void *YAZMTCore_DynamicDataArray_data(DynamicDataArray *d) {
    return DynDataArr_data(d);
}

RECOMP_EXPORT size_t YAZMTCore_DynamicDataArray_size(const DynamicDataArray *d) {
    return DynDataArr_size(d);
}

RECOMP_EXPORT void *YAZMTCore_DynamicDataArray_first(const DynamicDataArray *d) {
    return DynDataArr_first(d);
}

RECOMP_EXPORT void *YAZMTCore_DynamicDataArray_last(const DynamicDataArray *d) {
    return DynDataArr_last(d);
}

RECOMP_EXPORT void *YAZMTCore_DynamicDataArray_end(const DynamicDataArray *d) {
    return DynDataArr_end(d);
}

RECOMP_EXPORT void YAZMTCore_DynamicDataArray_reserve(DynamicDataArray *d, size_t minimumCapacity) {
    DynDataArr_reserve(d, minimumCapacity);
}

RECOMP_EXPORT void YAZMTCore_DynamicDataArray_shrinkToFit(DynamicDataArray *d) {
    DynDataArr_shrinkToFit(d);
}

// ----------------------
// IterableU32Set exports
// ----------------------

RECOMP_EXPORT IterableU32Set *YAZMTCore_IterableU32Set_new() {
    IterableU32Set *set = recomp_alloc(sizeof(IterableU32Set));

    if (!set) {
        Logger_printError("Could not allocate space for IterableU32Set struct!");
        return NULL;
    }

    if (!IterU32Set_init(set)) {
        Logger_printError("Failed to init IterableU32Set!");
        recomp_free(set);
        return NULL;
    }

    return set;
}

RECOMP_EXPORT void YAZMTCore_IterableU32Set_destroy(IterableU32Set *set) {
    if (set) {
        IterU32Set_destroyMembers(set);
        recomp_free(set);
    }
}

RECOMP_EXPORT bool YAZMTCore_IterableU32Set_insert(IterableU32Set *set, u32 value) {
    return IterU32Set_insert(set, value);
}

RECOMP_EXPORT bool YAZMTCore_IterableU32Set_erase(IterableU32Set *set, u32 value) {
    return IterU32Set_erase(set, value);
}

RECOMP_EXPORT bool YAZMTCore_IterableU32Set_contains(const IterableU32Set *set, u32 value) {
    return IterU32Set_contains(set, value);
}

RECOMP_EXPORT size_t YAZMTCore_IterableU32Set_size(const IterableU32Set *set) {
    return IterU32Set_count(set);
}

RECOMP_EXPORT void YAZMTCore_IterableU32Set_clear(IterableU32Set *set) {
    IterU32Set_clear(set);
}

RECOMP_EXPORT const u32 *YAZMTCore_IterableU32Set_values(const IterableU32Set *set) {
    return IterU32Set_data(set);
}

// ---------------------------
// StringU32Dictionary exports
// ---------------------------

RECOMP_EXPORT StringU32Dictionary *YAZMTCore_StringU32Dictionary_new() {
    StringU32Dictionary *dict = recomp_alloc(sizeof(StringU32Dictionary));

    if (!dict) {
        Logger_printError("Could not allocate space for StringU32Dictionary struct!");
        return NULL;
    }

    if (!StringU32Dictionary_init(dict)) {
        Logger_printError("Failed to init StringU32Dictionary");
        recomp_free(dict);
        return NULL;
    }

    return dict;
}

RECOMP_EXPORT void YAZMTCore_StringU32Dictionary_destroy(StringU32Dictionary *dict) {
    if (!dict) {
        return;
    }

    StringU32Dictionary_destroyMembers(dict);

    recomp_free(dict);
}

RECOMP_EXPORT void YAZMTCore_StringU32Dictionary_clear(StringU32Dictionary *dict) {
    StringU32Dictionary_clear(dict);
}

RECOMP_EXPORT bool YAZMTCore_StringU32Dictionary_set(StringU32Dictionary *dict, const char *key, u32 value) {
    return StringU32Dictionary_set(dict, key, value);
}

RECOMP_EXPORT bool YAZMTCore_StringU32Dictionary_get(const StringU32Dictionary *dict, const char *key, u32 *out) {
    return StringU32Dictionary_get(dict, key, out);
}

RECOMP_EXPORT bool YAZMTCore_StringU32Dictionary_unset(StringU32Dictionary *dict, const char *key) {
    return StringU32Dictionary_unset(dict, key);
}

RECOMP_EXPORT bool YAZMTCore_StringU32Dictionary_contains(const StringU32Dictionary *dict, const char *key) {
    return StringU32Dictionary_contains(dict, key);
}
