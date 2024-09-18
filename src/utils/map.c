/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "./array.c"
#include "./panic.c"

/**
 * Represents a map.
 */
struct Map {
    struct Array *_values;
};

#define MAP_STRUCT_SIZE sizeof(struct Map)

/**
 * Creates a new Map struct.
 *
 * @return The created Map struct.
 */
struct Map *map_new(void) {
    struct Map *self = malloc(MAP_STRUCT_SIZE);

    if (!self) {
        panic("failed to malloc Map struct");
    }

    self->_values = array_new();

    return self;
}

/**
 * Calculates the hash for a key.
 *
 * @param KEY The key to calculate the hash for.
 *
 * @return The calculated hash.
 */
size_t map___hash(const char *KEY) {
    size_t hash = 5381;
    int chr;

    while ((chr = *KEY++)) {
        hash = ((hash << 5) + hash) + (size_t)chr; /* hash * 33 + c */
    }

    return hash;
}

/**
 * Calculates the hash of the key and inserts the value at that index.
 *
 * @param self  The current Map struct.
 * @param KEY   The key to calculate the hash for.
 * @param VALUE The value to insert.
 */
void map_set(struct Map *self, const char *KEY, void *VALUE) { array_insert(self->_values, map___hash(KEY), VALUE); }

/**
 * Calculates the hash of the key and and returns the element at that index.
 *
 * @param self  The current Map struct.
 * @param KEY   The key to calculate the hash for.
 *
 * @return The retrieved element.
 */
const void *map_get(struct Map *self, const char *KEY) { return array_get(self->_values, map___hash(KEY)); }

/**
 * Frees an Map struct.
 *
 * @param self The current Map struct.
 */
void map_free(struct Map **self) {
    if (self && *self) {
        array_free(&(*self)->_values);

        free(*self);
        *self = NULL;
    } else {
        panic("Map struct has already been freed");
    }
}
