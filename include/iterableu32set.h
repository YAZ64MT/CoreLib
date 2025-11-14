#ifndef __ITERABLEU32SET_H__
#define __ITERABLEU32SET_H__

#include "PR/ultratypes.h"
#include "recompdata.h"
#include "dynamicu32array.h"

typedef struct {
    DynamicU32Array values;
    U32HashsetHandle hashset;
} IterableU32Set;

bool IterU32Set_init(IterableU32Set *s);

void IterU32Set_destroyMembers(IterableU32Set *s);

bool IterU32Set_insert(IterableU32Set *s, u32 v);

bool IterU32Set_erase(IterableU32Set *s, u32 v);

void IterU32Set_clear(IterableU32Set *s);

bool IterU32Set_contains(const IterableU32Set *s, u32 v);

size_t IterU32Set_count(const IterableU32Set *s);

const u32 *IterU32Set_data(const IterableU32Set *s);

#endif
