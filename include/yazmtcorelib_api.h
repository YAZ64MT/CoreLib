#ifndef __YAZMT_CORELIB_API_H__
#define __YAZMT_CORELIB_API_H__

#include "libc/stdbool.h"
#include "PR/ultratypes.h"
#include "modding.h"

#define YAZMT_CORELIB_MOD_NAME "yazmt_mm_corelib"

typedef struct YAZMTCore_DynamicU32Array YAZMTCore_DynamicU32Array;
typedef struct YAZMTCore_DynamicDataArray YAZMTCore_DynamicDataArray;
typedef struct YAZMTCore_IterableU32Set YAZMTCore_IterableU32Set;
typedef struct YAZMTCore_StringU32Dictionary YAZMTCore_StringU32Dictionary;

// Compares two pointers byte for byte up to size.
// Returns 0 if there are no differences.
// Returns a positive number if a > b
// Returns a negative number if a < b
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, int YAZMTCore_Utils_MemCmp(void *a, const void *b, size_t size));

// Returns a copy of the passed in string. Caller is responsible for freeing it.
// Returns NULL if the memory allocation fails.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, char *YAZMTCore_Utils_StrDup(const char *s));

// Creates and returns a pointer to a DynamicU32Array.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, YAZMTCore_DynamicU32Array *YAZMTCore_DynamicU32Array_new());

// Destroys a DynamicU32Array.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_DynamicU32Array_destroy(YAZMTCore_DynamicU32Array *dynArr));

// Sets the size of a DynamicU32Array to 0.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_DynamicU32Array_clear(YAZMTCore_DynamicU32Array *dynArr));

// Writes the u32 at index in the array to the out pointer.
// Returns true if pointer was successfully written to, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_DynamicU32Array_get(const YAZMTCore_DynamicU32Array *dynArr, size_t index, u32 *out));

// Writes val at the given index into the array.
// Returns true if successfully written to, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_DynamicU32Array_set(YAZMTCore_DynamicU32Array *dynArr, size_t index, u32 val));

// Adds a new u32 to the end of the array. May invalidate previous calls to YAZMTCore_DynamicU32Array_data.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_DynamicU32Array_push(YAZMTCore_DynamicU32Array *dynArr, u32 val));

// Removes a u32 from the end of the array.
// Returns true if number successfully removed, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_DynamicU32Array_pop(YAZMTCore_DynamicU32Array *dynArr));

// Removes a u32 from the end of the array and writes it to out.
// Returns true if out was written to, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_DynamicU32Array_popAndGet(YAZMTCore_DynamicU32Array *dynArr, u32 *out));

// Returns true if val is in the array.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_DynamicU32Array_contains(YAZMTCore_DynamicU32Array *dynArr, u32 val));

// Returns true if val is in the array and its index is successfully written to out, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_DynamicU32Array_indexOf(YAZMTCore_DynamicU32Array *dynArr, u32 val, size_t *out));

// Removes the first value in array matching val.
// Returns true if a value is removed, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_DynamicU32Array_removeByValue(YAZMTCore_DynamicU32Array *dynArr, u32 val));

// Remove the value at the passed in index from the array.
// Returns true if the value is removed, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_DynamicU32Array_removeByIndex(YAZMTCore_DynamicU32Array *dynArr, size_t index));

// Reserves at least minimum amount of capacity in the array for resizing without reallocating.
// Does not change the size of the array.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_DynamicU32Array_reserve(YAZMTCore_DynamicU32Array *dynArr, size_t minimum));

// Shrinks the capacity before reallocation to the current size of the array.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_DynamicU32Array_shrinkToFit(YAZMTCore_DynamicU32Array *dynArr));

// Returns a pointer to the start of the array.
// Returns NULL if the array is empty.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, u32 *YAZMTCore_DynamicU32Array_data(const YAZMTCore_DynamicU32Array *dynArr));

// Returns the number of elements in the array.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, size_t YAZMTCore_DynamicU32Array_size(const YAZMTCore_DynamicU32Array *dynArr));

// Returns a pointer to the first element of the array.
// Returns NULL if the array is empty.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, u32 *YAZMTCore_DynamicU32Array_front(const YAZMTCore_DynamicU32Array *dynArr));

// Returns a pointer to the last element of the array.
// Returns NULL if the array is empty.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, u32 *YAZMTCore_DynamicU32Array_back(const YAZMTCore_DynamicU32Array *dynArr));

// Returns a pointer to one element past the last element of the array.
// Returns NULL if the array is empty.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, u32 *YAZMTCore_DynamicU32Array_end(const YAZMTCore_DynamicU32Array *dynArr));

// Creates and returns a pointer to a DynamicDataArray.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, YAZMTCore_DynamicDataArray *YAZMTCore_DynamicDataArray_new(size_t elementSize));

// Destroys a DynamicDataArray.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_DynamicDataArray_destroy(YAZMTCore_DynamicDataArray *dynArr));

// Sets the size of a DynamicDataArray to 0.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_DynamicDataArray_clear(YAZMTCore_DynamicDataArray *dynArr));

// Creates a new element at the end of the DynamicDataArray and returns a pointer to it.
// May cause a reallocation and invalidate previous pointers.
// Returns NULL if reallocation fails.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void *YAZMTCore_DynamicDataArray_createElement(YAZMTCore_DynamicDataArray *dynArr));

// Returns true if there exists an element with the same byte representation of val in the array, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_DynamicDataArray_contains(const YAZMTCore_DynamicDataArray *dynArr, const void *val));

// Finds the first element matching an exact byte representation of val in the array.
// If an element is found, its index is written to out, and if that is successful, it returns true.
// Otherwise, false is returned.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_DynamicDataArray_indexOf(const YAZMTCore_DynamicDataArray *dynArr, const void *val, size_t *out));

// Returns a pointer to the element at index in the array.
// Returns NULL if the element cannot be retrieved.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void *YAZMTCore_DynamicDataArray_getElement(const YAZMTCore_DynamicDataArray *dynArr, size_t index));

// Copies the element in val to the element in the given index of the array.
// Returns true if element was successfully copied in, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_DynamicDataArray_setElement(YAZMTCore_DynamicDataArray *dynArr, size_t index, const void *val));

// Creates a new element at the end of the array and copies the contents of val into it.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_DynamicDataArray_push(YAZMTCore_DynamicDataArray *dynArr, void *val));

// Reduces the size of the array by 1.
// Returns true if successful, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_DynamicDataArray_pop(YAZMTCore_DynamicDataArray *dynArr));

// Returns a pointer to the array's data.
// Returns NULL if the array is empty.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void *YAZMTCore_DynamicDataArray_data(const YAZMTCore_DynamicDataArray *dynArr));

// Returns the number of elements in the array.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, size_t YAZMTCore_DynamicDataArray_size(const YAZMTCore_DynamicDataArray *dynArr));

// Returns a pointer to the first element of the array.
// Returns NULL if the array is empty.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void *YAZMTCore_DynamicDataArray_first(const YAZMTCore_DynamicDataArray *dynArr));

// Returns a pointer to the last element of the array.
// Returns NULL if the array is empty.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void *YAZMTCore_DynamicDataArray_last(const YAZMTCore_DynamicDataArray *dynArr));

// Returns a pointer to one element past the last element of the array.
// Returns NULL if the array is empty.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void *YAZMTCore_DynamicDataArray_end(const YAZMTCore_DynamicDataArray *dynArr));

// Resizes DynamicDataArray's internal capacity to at least the minimum capacity passed in.
// May cause a reallocation.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_DynamicDataArray_reserve(YAZMTCore_DynamicDataArray *dynArr, size_t minimumCapacity));

// Resizes DynamicDataArray's internal capacity to current number of elements in the array.
// May cause a reallocation.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_DynamicDataArray_shrinkToFit(YAZMTCore_DynamicDataArray *dynArr));

// Creates and returns a pointer to a new IterableU32Set.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, YAZMTCore_IterableU32Set *YAZMTCore_IterableU32Set_new());

// Destroys an IterableU32Set.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_IterableU32Set_destroy(YAZMTCore_IterableU32Set *set));

// Inserts a new element into the IterableU32Set.
// Returns true if the item was successfully inserted (was not already in set), false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_IterableU32Set_insert(YAZMTCore_IterableU32Set *set, u32 val));

// Removes an element from the IterableU32Set.
// Returns true if the item was successfully removed, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_IterableU32Set_erase(YAZMTCore_IterableU32Set *set, u32 val));

// Returns true if val exists in the set, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_IterableU32Set_contains(const YAZMTCore_IterableU32Set *set, u32 val));

// Returns the number of elements in the set.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, size_t YAZMTCore_IterableU32Set_size(const YAZMTCore_IterableU32Set *set));

// Removes all elements from the set.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_IterableU32Set_clear(YAZMTCore_IterableU32Set *set));

// Returns a pointer to an array containing all numbers in the set or NULL if there are no values.
// The values in this array should NOT be modified.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, const u32 *YAZMTCore_IterableU32Set_values(const YAZMTCore_IterableU32Set *set));

// Creates a new StringU32Dictionary.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, YAZMTCore_StringU32Dictionary *YAZMTCore_StringU32Dictionary_new());

// Destroys a StringU32Dictionary.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_StringU32Dictionary_destroy(YAZMTCore_StringU32Dictionary *dict));

// Removes all keys from a StringU32Dictionary.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, void YAZMTCore_StringU32Dictionary_clear(YAZMTCore_StringU32Dictionary *dict));

// Associates a u32 value with a key in a StringU32Dictionary. Returns true if a new key value pair is created, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_StringU32Dictionary_set(YAZMTCore_StringU32Dictionary *dict, const char *key, u32 val));

// Checks if a key exists in a StringU32Dictionary and writes the value associated with it to out.
// Returns true if a value is written to out.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_StringU32Dictionary_get(YAZMTCore_StringU32Dictionary *dict, const char *key, u32 *out));

// Removes a key from a StringU32Dictionary. Returns true if the key exists and was removed, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_StringU32Dictionary_unset(YAZMTCore_StringU32Dictionary *dict, const char *key));

// Returns true if the key exists in the StringU32Dictionary, false otherwise.
RECOMP_IMPORT(YAZMT_CORELIB_MOD_NAME, bool YAZMTCore_StringU32Dictionary_contains(YAZMTCore_StringU32Dictionary *dict, const char *key));

#endif
