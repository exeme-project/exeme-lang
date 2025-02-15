/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * Hashing function type.
 */
typedef size_t (*hash_function_t)(const char*);

/**
 * Represents a value in a bucket of the hashmap (can also be the head of a bucket).
 */
struct HashmapValue {
	const char*			 KEY;	// The key associated with the value in the bucket.
	void*				 value; // The value stored in the bucket.
	struct HashmapValue* next;	// Pointer to the next value in the bucket chain.
};

/**
 * Represents a hashmap.
 */
struct Hashmap {
	hash_function_t		  hasher;			   // Function to calculate the hash of a key.
	float				  load_factor;		   // The load factor of the hashmap.
	size_t				  initialised_buckets; // Number of initialised buckets in the hashmap.
	size_t				  table_length;		   // The total number of buckets in the hashmap.
	struct HashmapValue** buckets;			   // Array of pointers to the buckets.
};

enum { DEFAULT_INITIAL_TABLE_COUNT = 16U };
#define DEFAULT_LOAD_FACTOR 0.7f

#define HASHMAP_VALUE_SIZE	   sizeof(struct HashmapValue)
#define HASHMAP_VALUE_PTR_SIZE sizeof(struct HashmapValue*)
#define HASHMAP_SIZE		   sizeof(struct Hashmap)

#define DJB2_HASH		5381U
#define DJB2_HASH_SHIFT 5U

/**
 * Calculates the hash for a key using the DJB2 algorithm.
 *
 * @param p_key The key to calculate the hash for.
 *
 * @return The calculated hash.
 */
size_t hashmap_hash_djb2(const char* p_key);

/**
 * Creates a new Hashmap struct.
 *
 * @param p_hasher The hashing function to use.
 * @param initialTableLength The initial length of the table.
 * @param LOAD_FACTOR The load factor of the hashmap.
 *
 * @return The created Hashmap struct.
 */
struct Hashmap*
hashmap_new(hash_function_t p_hasher, size_t initialTableLength,
			const float LOAD_FACTOR); // NOLINT(readability-avoid-const-params-in-decls)

/**
 * Frees a Hashmap struct.
 *
 * @param p_self Pointer to the current Hashmap struct.
 * @param p_freeElement The function to free the elements with.
 */
void hashmap_free(struct Hashmap** p_self, void (*p_freeElement)(const void*));

/**
 * Retrieves a pointer to a bucket from the hashmap.
 *
 * @param p_self The current Hashmap struct.
 * @param returnParent Whether to return the parent pointer of the bucket if it doesn't exist.
 * @param p_key The key of the bucket.
 * @param hashIndex The hash index, calculated as hash mod table length.
 *
 * @return The pointer to the bucket.
 */
struct HashmapValue** hashmap_get_bucket(struct Hashmap* p_self, bool returnParent,
										 const char* p_key, size_t hashIndex);

/**
 * Inserts a value into the hashmap.
 * IMPORTANT: Assumes that key is dynamically allocated and will exist for the lifetime of the
 * hashmap.
 *
 * @param p_self The current Hashmap struct.
 * @param p_key The key to calculate the hash for.
 * @param p_value The value to insert.
 */
void hashmap_set(struct Hashmap* p_self, const char* p_key, void* p_value);

/**
 * Retrieves a value from the hashmap.
 *
 * @param p_self The current Hashmap struct.
 * @param p_key The key to calculate the hash for.
 *
 * @return The retrieved value or NULL if not found.
 */
void** hashmap_get(struct Hashmap* p_self, const char* p_key);

/**
 * Combines two hashmaps into one.
 *
 * @param p_self The current Hashmap struct.
 * @param p_other The other Hashmap struct to combine with.
 */
void hashmap_combine(struct Hashmap* p_self, const struct Hashmap* p_other);
