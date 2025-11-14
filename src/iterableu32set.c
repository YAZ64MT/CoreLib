#include "global.h"
#include "iterableu32set.h"
#include "logger.h"

static U32HashsetHandle sValidIterableSets;

#define PRINT_INVALID_PTR_ERR() Logger_printError("Received invalid IterableU32Set pointer.")

static bool isValidSet(const IterableU32Set *s) {
    return sValidIterableSets && recomputil_u32_hashset_contains(sValidIterableSets, (uintptr_t)s);
}

bool IterU32Set_init(IterableU32Set *s) {
    if (!DynU32Arr_init(&s->values)) {
        Logger_printError("Failed to initialize values field in struct.");
        return false;
    }
    
    s->hashset = recomputil_create_u32_hashset();

    if (!sValidIterableSets) {
        sValidIterableSets = recomputil_create_u32_hashset();
    }

    recomputil_u32_hashset_insert(sValidIterableSets, (uintptr_t)s);

    return true;
}

void IterU32Set_destroyMembers(IterableU32Set *s) {
    if (!isValidSet(s)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    DynU32Arr_destroyMembers(&s->values);
    recomputil_destroy_u32_hashset(s->hashset);

    recomputil_u32_hashset_erase(sValidIterableSets, (uintptr_t)s);
}

bool IterU32Set_insert(IterableU32Set *s, u32 v) {
    if (!isValidSet(s)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (recomputil_u32_hashset_insert(s->hashset, v)) {
        DynU32Arr_push(&s->values, v);
        return true;
    }

    return false;
}

bool IterU32Set_erase(IterableU32Set *s, u32 v) {
    if (!isValidSet(s)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (recomputil_u32_hashset_erase(s->hashset, v)) {
        DynU32Arr_removeByValue(&s->values, v);
        return true;
    }

    return false;
}

void IterU32Set_clear(IterableU32Set *s) {
    if (!isValidSet(s)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    size_t count = DynU32Arr_size(&s->values);
    u32 *data = DynU32Arr_data(&s->values);

    for (size_t i = 0; i < count; ++i) {
        recomputil_u32_hashset_erase(s->hashset, data[i]);
    }

    DynU32Arr_clear(&s->values);
}

bool IterU32Set_contains(const IterableU32Set *s, u32 v) {
    if (!isValidSet(s)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    return recomputil_u32_hashset_contains(s->hashset, v);
}

size_t IterU32Set_count(const IterableU32Set *s) {
    if (!isValidSet(s)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    return DynU32Arr_size(&s->values);
}

const u32 *IterU32Set_data(const IterableU32Set *s) {
    if (!isValidSet(s)) {
        PRINT_INVALID_PTR_ERR();
        return NULL;
    }

    return DynU32Arr_data(&s->values);
}
