#include "global.h"
#include "stringu32dictionary.h"
#include "libc/string.h"
#include "recompdata.h"
#include "recomputils.h"
#include "dynamicdataarray.h"
#include "iterableu32set.h"
#include "fnv.h"
#include "utils.h"
#include "logger.h"

static U32HashsetHandle sValidDicts;

#define PRINT_INVALID_PTR_ERR() Logger_printError("Received invalid IterableU32Set pointer.")

static bool isValidDict(const StringU32Dictionary *dict) {
    return sValidDicts && recomputil_u32_hashset_contains(sValidDicts, (uintptr_t)dict);
}

static u32 hasher(const void *data, size_t length) {
    return fnv_32a_buf(data, length, FNV1_32A_INIT);
}

static void destroySlotMembers(StringU32DictionaryEntrySlot *slot) {
    recomp_free(slot->key);
    slot->key = NULL;
}

bool StringU32Dictionary_init(StringU32Dictionary *dict) {
    if (!IterU32Set_init(&dict->keyHashes)) {
        Logger_printError("Failed to initialize keyHashes field in struct.");
        return false;
    }

    dict->hashmap = recomputil_create_u32_memory_hashmap(sizeof(DynamicDataArray));

    if (!sValidDicts) {
        sValidDicts = recomputil_create_u32_hashset();
    }

    recomputil_u32_hashset_insert(sValidDicts, (uintptr_t)dict);

    return true;
}

void StringU32Dictionary_destroyMembers(StringU32Dictionary *dict) {
    if (!isValidDict(dict)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    StringU32Dictionary_clear(dict);

    IterU32Set_destroyMembers(&dict->keyHashes);

    recomputil_destroy_u32_memory_hashmap(dict->hashmap);

    recomputil_u32_hashset_erase(sValidDicts, (uintptr_t)dict);
}

void StringU32Dictionary_clear(StringU32Dictionary *dict) {
    if (!isValidDict(dict)) {
        PRINT_INVALID_PTR_ERR();
        return;
    }

    const u32 *hashes = IterU32Set_data(&dict->keyHashes);
    size_t hashCount = IterU32Set_count(&dict->keyHashes);

    for (size_t i = 0; i < hashCount; ++i) {
        u32 hash = hashes[i];

        DynamicDataArray *slots = recomputil_u32_memory_hashmap_get(dict->hashmap, hash);

        if (slots) {
            StringU32DictionaryEntrySlot *slotData = DynDataArr_data(slots);
            size_t slotCount = DynDataArr_size(slots);
            for (size_t j = 0; j < slotCount; ++j) {
                destroySlotMembers(&slotData[j]);
            }

            DynDataArr_destroyMembers(slots);

            recomputil_u32_memory_hashmap_erase(dict->hashmap, hash);
        }
    }

    IterU32Set_clear(&dict->keyHashes);
}

bool StringU32Dictionary_set(StringU32Dictionary *dict, const char *key, u32 value) {
    if (!isValidDict(dict)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (!key) {
        return false;
    }

    size_t keyLen = strlen(key);

    u32 hash = hasher(key, keyLen);

    DynamicDataArray *dynMemArr = NULL;

    if (recomputil_u32_memory_hashmap_create(dict->hashmap, hash)) {
        dynMemArr = recomputil_u32_memory_hashmap_get(dict->hashmap, hash);

        // collisions should be rare, so start with 1 slot
        DynDataArr_init(dynMemArr, sizeof(StringU32DictionaryEntrySlot), 1);
    }

    if (!dynMemArr) {
        dynMemArr = recomputil_u32_memory_hashmap_get(dict->hashmap, hash);
    }

    StringU32DictionaryEntrySlot *slot = NULL;

    size_t slotCount = DynDataArr_size(dynMemArr);
    for (size_t i = 0; i < slotCount && !slot; ++i) {
        StringU32DictionaryEntrySlot *candidate = DynDataArr_get(dynMemArr, i);

        if (candidate && strcmp(key, candidate->key) == 0) {
            slot = candidate;
        }
    }

    bool shouldCreateElement = slot == NULL;

    if (shouldCreateElement) {
        slot = DynDataArr_createElement(dynMemArr);

        if (slot) {
            slot->key = Utils_StrDup(key);
        }
    }

    if (slot) {
        slot->value = value;

        IterU32Set_insert(&dict->keyHashes, hash);
    }

    return shouldCreateElement;
}

bool StringU32Dictionary_get(const StringU32Dictionary *dict, const char *key, u32 *out) {
    if (!isValidDict(dict)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (!key) {
        Logger_printError("key was NULL!");
        return false;
    }

    if (!out) {
        Logger_printError("out was NULL!");
        return false;
    }

    size_t keyLen = strlen(key);

    u32 hash = hasher(key, keyLen);

    DynamicDataArray *slots = recomputil_u32_memory_hashmap_get(dict->hashmap, hash);

    if (!slots) {
        return false;
    }

    size_t slotCount = DynDataArr_size(slots);
    for (size_t i = 0; i < slotCount; ++i) {
        StringU32DictionaryEntrySlot *candidate = DynDataArr_get(slots, i);

        if (candidate && strcmp(key, candidate->key) == 0) {
            *out = candidate->value;
            return true;
        }
    }

    return false;
}

bool StringU32Dictionary_contains(const StringU32Dictionary *dict, const char *key) {
    if (!isValidDict(dict)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (!key) {
        return false;
    }

    size_t keyLen = strlen(key);

    u32 hash = hasher(key, keyLen);

    DynamicDataArray *slots = recomputil_u32_memory_hashmap_get(dict->hashmap, hash);

    if (!slots) {
        return false;
    }

    size_t slotCount = DynDataArr_size(slots);
    for (size_t i = 0; i < slotCount; ++i) {
        StringU32DictionaryEntrySlot *candidate = DynDataArr_get(slots, i);

        if (candidate && strcmp(key, candidate->key) == 0) {
            return true;
        }
    }

    return false;
}

bool StringU32Dictionary_unset(StringU32Dictionary *dict, const char *key) {
    if (!isValidDict(dict)) {
        PRINT_INVALID_PTR_ERR();
        return false;
    }

    if (!key) {
        return false;
    }

    size_t keyLen = strlen(key);

    u32 hash = hasher(key, keyLen);

    DynamicDataArray *slots = recomputil_u32_memory_hashmap_get(dict->hashmap, hash);

    if (!slots) {
        return false;
    }

    size_t slotCount = DynDataArr_size(slots);
    for (size_t i = 0; i < slotCount; ++i) {
        StringU32DictionaryEntrySlot *candidate = DynDataArr_get(slots, i);

        if (candidate && strcmp(key, candidate->key) == 0) {
            destroySlotMembers(candidate);
            DynDataArr_removeByIndex(slots, i);
            return true;
        }
    }

    return false;
}
