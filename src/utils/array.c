/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "./panic.c"
#include "./string.c"

/**
 * Represents an array.
 */
struct Array {
    size_t length;
    const void **_values;
};

#define ARRAY_STRUCT_SIZE sizeof(struct Array)
#define ARRAY_STRUCT_ELEMENT_SIZE sizeof(const void *)
#define array_new_stack(...)                                                                                                \
    ((struct Array){.length = sizeof((const void *[]){__VA_ARGS__}) / ARRAY_STRUCT_ELEMENT_SIZE,                            \
                    ._values = (const void *[]){__VA_ARGS__}}) // Variadic args to the rescue!
#define array_upgrade_stack(array, _length)                                                                                 \
    ((struct Array){.length = _length, ._values = array}) // Changes a const void *[] to a struct Array on the stack

/**
 * Creates a new Array struct.
 *
 * @return The created Array struct.
 */
struct Array *array_new(void) {
    struct Array *self = malloc(ARRAY_STRUCT_SIZE);

    if (!self) {
        panic("failed to malloc Array struct");
    }

    self->length = 0;
    self->_values = calloc(1, ARRAY_STRUCT_ELEMENT_SIZE);

    return self;
}

/**
 * Reallocates the struct's array.
 * IMPORTANT: Update the new length AFTER calling this function.
 *
 * @param self The current Array struct.
 * @param size The new size of the array.
 */
void array___realloc(struct Array *self, size_t size) {
    self->_values = realloc(self->_values, size);

    if (size > self->length * ARRAY_STRUCT_ELEMENT_SIZE) {
        memset(self->_values + self->length, 0,
               size - (self->length *
                       ARRAY_STRUCT_ELEMENT_SIZE)); // Zero out the new memory. First parameter is the pointer for the array,
                                                    // starting from where we reallocated from. The third parameter is the
                                                    // size of the newly allocated memory.
    }

    if (!self->_values) {
        panic("failed to realloc array");
    }
}

/**
 * Inserts a value at the specified index. Will expand the array's size if
 * needed.
 *
 * @param self  The current Array struct.
 * @param index The index at which to insert the value.
 * @param value The value to insert.
 */
void array_insert(struct Array *self, size_t index, const void *value) {
    if (index + 1 > self->length) {
        array___realloc(self, (index + 1) * ARRAY_STRUCT_ELEMENT_SIZE);
        self->length = index + 1;
    }

    self->_values[index] = value;
}

/**
 * Appends a value to the end of the array.
 *
 * @param self  The current Array struct.
 * @param value The value to append.
 */
void array_append(struct Array *self, const void *value) { array_insert(self, self->length, value); }

/**
 * Removes the last element from the array.
 *
 * @param self The current Array struct.
 */
void array_pop(struct Array *self) {
    if (self->length < 1) {
        panic("nothing to pop from array");
    } else if (self->length == 1) {
        array___realloc(self, 1);
        self->length = 0;
    } else {
        array___realloc(self, self->length * ARRAY_STRUCT_ELEMENT_SIZE);
        self->length--;
    }
}

/**
 * Removes all the elements from the array.
 *
 * @param self         The current Array struct.
 * @param free_element The function to free the elements with.
 */
void array_clear(struct Array *self, void (*free_element)(const void *)) {
    if (free_element) {
        for (size_t index = 0; index < self->length; index++) {
            free_element(self->_values[index]);
        }
    }

    array___realloc(self, 1);
    self->length = 0;
}

/**
 * Returns the element at the specified index in the array.
 *
 * @param self  The current Array struct.
 * @param index The index from which to get the element.
 *
 * @return The retrieved element.
 */
const void *array_get(struct Array *self, size_t index) {
    if (index + 1 > self->length) {
        panic("array get index out of bounds");
    }

    return self->_values[index];
}

/**
 * Iterates through the array, using the passed function to check for matches. If a match is found then the index is
 * returned.
 *
 * @param self    The current Array struct.
 * @param matcher The function to check for matches.
 * @param match   The value to match against.
 *
 * @return The index of the match.
 */
int array_find(struct Array *self, bool (*matcher)(const void *, const void *), void *match) {
    for (size_t index = 0; index < self->length; index++) {
        const void *element = self->_values[index];

        if (matcher(element, match)) {
            return (int)index;
        }
    }

    return -1;
}

/**
 * Checks if the array contains a value, internally using the array_find function.
 *
 * @param self    The current Array struct.
 * @param matcher The function to check for matches.
 * @param match   The value to match against.
 *
 * @return If a match was found.
 */
bool array_contains(struct Array *self, bool (*matcher)(const void *, const void *), void *match) {
    return array_find(self, matcher, match) != -1;
}

/**
 * Checks if the array contains a value at the specified index.
 *
 * @param self  The current Array struct.
 * @param index The index to check.
 *
 * @return If the index is occupied.
 */
bool array_index_occupied(struct Array *self, size_t index) {
    if (index + 1 > self->length) {
        return false;
    }

    return self->_values[index] != NULL;
}

char *array_join(struct Array *self, const char *separator, char *(*stringify)(const void *)) {
    char *joined = stringDuplicate(stringify(self->_values[0])); // Malloc a duplicate of the first element

    for (size_t index = 1; index < self->length; index++) {
        char *temp =
            stringConcatenate(joined, separator, stringify(self->_values[index])); // Concatenate the next element with the
                                                                                   // separator in-between the previous.
        free(joined); // Since stringConcatenate mallocs a new string, we need to free the old one

        joined = temp;
    }

    return joined;
}

/**
 * Frees an Array struct.
 *
 * @param self The current Array struct.
 */
void array_free(struct Array **self) {
    if (self && *self) {
        free((*self)->_values);

        free(*self);
        *self = NULL;
    } else {
        panic("Array struct has already been freed");
    }
}
