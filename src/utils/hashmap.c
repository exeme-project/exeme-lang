/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "./array.c"
#include "./panic.c"

// Hashing function
typedef size_t (*hash_function_t)(const char *);

/**
 * Represents a value in a bucket of the hashmap (can also be the head of a bucket).
 */
struct HashmapValue {
    const char *KEY;           // The key associated with the value in the bucket.
    void *value;               // The value stored in the bucket.
    struct HashmapValue *next; // Pointer to the next value in the bucket chain.
};

/**
 * Represents a hashmap.
 */
struct Hashmap {
    hash_function_t hasher;        // Function to calculate the hash of a key.
    float load_factor;             // The load factor of the hashmap.
    size_t initialised_buckets;    // Number of initialised buckets in the hashmap.
    size_t table_length;           // The total number of buckets in the hashmap.
    struct HashmapValue **buckets; // Array of pointers to the buckets.
};

#define DEFAULT_INITIAL_TABLE_COUNT 16
#define DEFAULT_LOAD_FACTOR 0.7f

#define HASHMAP_VALUE_SIZE sizeof(struct HashmapValue)
#define HASHMAP_VALUE_PTR_SIZE sizeof(struct HashmapValue *)
#define HASHMAP_SIZE sizeof(struct Hashmap)

/**
 * Calculates the hash for a key using the DJB2 algorithm.
 *
 * @param KEY The key to calculate the hash for.
 *
 * @return The calculated hash.
 */
size_t hashmap___hashDJB2(const char *KEY) {
    size_t hash = 5381;
    int chr;

    while ((chr = *KEY++)) {
        hash = ((hash << 5) + hash) + chr; /* hash * 33 + chr */
    }

    return hash;
}

/**
 * Creates a new Hashmap struct.
 *
 * @return The created Hashmap struct.
 */
struct Hashmap *hashmap_new(hash_function_t hasher, size_t initial_table_length, float load_factor) {
    struct Hashmap *self = malloc(HASHMAP_SIZE);

    if (!self) {
        panic("failed to malloc Hashmap struct");
    }

    self->hasher = hasher;
    self->load_factor = load_factor;
    self->initialised_buckets = 0;
    self->table_length = initial_table_length;

    if (self->table_length == 0) {
        panic("table length cannot be 0");
    }

    self->buckets = calloc(initial_table_length, HASHMAP_VALUE_PTR_SIZE); // Initialise the buckets with NULL.

    if (!self->buckets) {
        panic("failed to allocate memory for hashmap buckets");
    }

    return self;
}

/**
 * Frees a Hashmap struct.
 *
 * @param self Pointer to the current Hashmap struct.
 * @param free_element The function to free the elements with.
 */
void hashmap_free(struct Hashmap **self, void (*free_element)(const void *)) {
    if (self && *self) {
        for (size_t index = 0; index < (*self)->table_length; index++) {
            struct HashmapValue *bucket = (*self)->buckets[index];

            while (bucket) {
                struct HashmapValue *next = bucket->next;

                if (free_element) {
                    free_element(bucket->value);
                }

                free(bucket);
                bucket = next;
            }
        }

        free(*self);
        *self = NULL;
    } else {
        panic("Hashmap struct has already been freed");
    }
}

/**
 * Retrieves a pointer to a bucket from the hashmap.
 *
 * @param self          The current Hashmap struct.
 * @param return_parent Whether to return the parent pointer of the bucket if it doesn't exist.
 * @param KEY           The key of the bucket.
 * @param hash_index    The hash index, calculated as hash mod table length.
 */
struct HashmapValue **hashmap___getBucket(struct Hashmap *self, bool return_parent, const char *KEY, size_t hash_index) {
    struct HashmapValue **bucket = &self->buckets[hash_index];

    if (!*bucket) {
        return return_parent ? bucket : NULL; // Return NULL if the bucket does not exist and not returning parent.
    }

    while (*bucket) {
        if (strcmp((*bucket)->KEY, KEY) == 0) {
            return bucket;
        }

        bucket = &(*bucket)->next;
    }

    return return_parent ? bucket : NULL; // Same as above.
}

void hashmap___resize(struct Hashmap *self) {
    size_t old_table_length = self->table_length;
    struct HashmapValue **old_buckets = self->buckets;

    self->table_length *= 2;
    self->buckets = calloc(self->table_length, HASHMAP_VALUE_PTR_SIZE);

    if (!self->buckets) {
        panic("failed to allocate memory for new hashmap buckets during resize");
    }

    for (size_t index = 0; index < old_table_length; index++) {
        struct HashmapValue *bucket = old_buckets[index];

        while (bucket) {
            struct HashmapValue *next = bucket->next; // Get the next value in the chain.
            bucket->next = NULL; // Disconnect from the old chain since it may not be at the same index in the new chain.

            size_t hash_raw = self->hasher(bucket->KEY);
            size_t hash_index = hash_raw % self->table_length;

            struct HashmapValue **new_bucket = hashmap___getBucket(self, true, bucket->KEY, hash_index);
            *new_bucket = bucket;

            bucket = next;
        }
    }

    free(old_buckets);
}

/**
 * Inserts a value into the hashmap.
 * IMPORTANT: Assumes that KEY is dynamically allocated and will exist for the lifetime of the hashmap.
 *
 * @param self  The current Hashmap struct.
 * @param KEY   The key to calculate the hash for.
 * @param value The value to insert.
 */
void hashmap_set(struct Hashmap *self, const char *KEY, void *value) {
    size_t hash_raw = self->hasher(KEY);
    size_t hash_index = hash_raw % self->table_length;

    struct HashmapValue **bucket = hashmap___getBucket(self, true, KEY, hash_index);

    if (*bucket) { // Key already exists, so update the value.
        (*bucket)->value = value;
    } else { // Create a new value in the bucket.
        if (!self->buckets[hash_index]) {
            self->initialised_buckets++;
        }

        *bucket = malloc(HASHMAP_VALUE_SIZE);

        if (!*bucket) {
            panic("failed to malloc HashmapValue struct");
        }

        (*bucket)->KEY = KEY;
        (*bucket)->value = value;
        (*bucket)->next = NULL; // Last element in the chain.
    }

    if ((float)self->initialised_buckets > (float)self->table_length * self->load_factor) { // Resize if needed.
        hashmap___resize(self);
    }
}

/**
 * Retrieves a value from the hashmap.
 *
 * @param self  The current Hashmap struct.
 * @param KEY   The key to calculate the hash for.
 *
 * @return The retrieved value or NULL if not found.
 */
void **hashmap_get(struct Hashmap *self, const char *KEY) {
    size_t hash_raw = self->hasher(KEY);
    size_t hash_index = hash_raw % self->table_length;

    struct HashmapValue **bucket = hashmap___getBucket(self, false, KEY, hash_index);

    if (!bucket) { // Check if the pointer itself is NULL.
        return NULL;
    }

    return &(*bucket)->value;
}

/**
 * Combines two hashmaps into one.
 *
 * @param self  The current Hashmap struct.
 * @param other The other Hashmap struct to combine with.
 */
void hashmap_combine(struct Hashmap *self, struct Hashmap *other) {
    for (size_t index = 0; index < other->table_length; index++) {
        struct HashmapValue *bucket = other->buckets[index];

        while (bucket) {
            hashmap_set(self, bucket->KEY, bucket->value);

            bucket = bucket->next;
        }
    }
}
