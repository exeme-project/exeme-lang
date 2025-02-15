/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "./hashmap.h"
#include "./panic.h"
#include <stdlib.h>
#include <string.h>

size_t hashmap_hash_djb2(const char* p_key) {
	size_t hash = DJB2_HASH;
	int	   chr	= 0;

	while ((chr = (unsigned char)*p_key++)) {
		hash = ((hash << DJB2_HASH_SHIFT) + hash) + chr; /* hash * 33 + chr */
	}

	return hash;
}

struct Hashmap* hashmap_new(hash_function_t p_hasher, size_t initialTableLength,
							const float LOAD_FACTOR) {
	struct Hashmap* lp_self = malloc(HASHMAP_SIZE);

	if (!lp_self) {
		PANIC("failed to malloc Hashmap struct");
	}

	lp_self->hasher				 = p_hasher;
	lp_self->load_factor		 = LOAD_FACTOR;
	lp_self->initialised_buckets = 0;
	lp_self->table_length		 = initialTableLength;

	if (lp_self->table_length == 0) {
		PANIC("table length cannot be 0");
	}

	lp_self->buckets =
		calloc(initialTableLength, HASHMAP_VALUE_PTR_SIZE); // Initialise the buckets with NULL.

	if (!lp_self->buckets) {
		PANIC("failed to allocate memory for hashmap buckets");
	}

	return lp_self;
}

void hashmap_free(struct Hashmap** p_self, void (*p_freeElement)(const void*)) {
	if (p_self && *p_self) {
		for (size_t index = 0; index < (*p_self)->table_length; index++) {
			struct HashmapValue* lp_bucket = (*p_self)->buckets[index];

			while (lp_bucket) {
				struct HashmapValue* lp_next = lp_bucket->next;

				if (p_freeElement) {
					p_freeElement(lp_bucket->value);
				}

				free(lp_bucket);
				lp_bucket = lp_next;
			}
		}

		free(*p_self);
		*p_self = NULL;
	} else {
		PANIC("Hashmap struct has already been freed");
	}
}

struct HashmapValue** hashmap_get_bucket(struct Hashmap* p_self, bool returnParent,
										 const char* p_key, size_t hashIndex) {
	struct HashmapValue** lp_bucket = &p_self->buckets[hashIndex];

	if (!*lp_bucket) {
		return returnParent
				   ? lp_bucket
				   : NULL; // Return NULL if the bucket does not exist and not returning parent.
	}

	while (*lp_bucket) {
		if (strcmp((*lp_bucket)->KEY, p_key) == 0) {
			return lp_bucket;
		}

		lp_bucket = &(*lp_bucket)->next;
	}

	return returnParent ? lp_bucket : NULL; // Same as above.
}

void hashmap___resize(struct Hashmap* p_self) {
	size_t				  oldTableLength = p_self->table_length;
	struct HashmapValue** lp_oldBuckets	 = p_self->buckets;

	p_self->table_length *= 2;
	p_self->buckets = calloc(p_self->table_length, HASHMAP_VALUE_PTR_SIZE);

	if (!p_self->buckets) {
		PANIC("failed to allocate memory for new hashmap buckets during resize");
	}

	for (size_t index = 0; index < oldTableLength; index++) {
		struct HashmapValue* lp_bucket = lp_oldBuckets[index];

		while (lp_bucket) {
			struct HashmapValue* lp_next = lp_bucket->next; // Get the next value in the chain.
			lp_bucket->next = NULL; // Disconnect from the old chain since it may not be at the same
									// index in the new chain.

			size_t hashRaw	 = p_self->hasher(lp_bucket->KEY);
			size_t hashIndex = hashRaw % p_self->table_length;

			struct HashmapValue** lp_newBucket =
				hashmap_get_bucket(p_self, true, lp_bucket->KEY, hashIndex);
			*lp_newBucket = lp_bucket;

			lp_bucket = lp_next;
		}
	}

	free(lp_oldBuckets);
}

void hashmap_set(struct Hashmap* p_self, const char* p_key, void* p_value) {
	size_t hashRaw	 = p_self->hasher(p_key);
	size_t hashIndex = hashRaw % p_self->table_length;

	struct HashmapValue** lp_bucket = hashmap_get_bucket(p_self, true, p_key, hashIndex);

	if (*lp_bucket) { // Key already exists, so update the value.
		(*lp_bucket)->value = p_value;
	} else { // Create a new value in the bucket.
		if (!p_self->buckets[hashIndex]) {
			p_self->initialised_buckets++;
		}

		*lp_bucket = malloc(HASHMAP_VALUE_SIZE);

		if (!*lp_bucket) {
			PANIC("failed to malloc HashmapValue struct");
		}

		(*lp_bucket)->KEY	= p_key;
		(*lp_bucket)->value = p_value;
		(*lp_bucket)->next	= NULL; // Last element in the chain.
	}

	if ((float)p_self->initialised_buckets
		> (float)p_self->table_length * p_self->load_factor) { // Resize if needed.
		hashmap___resize(p_self);
	}
}

void** hashmap_get(struct Hashmap* p_self, const char* p_key) {
	size_t hashRaw	 = p_self->hasher(p_key);
	size_t hashIndex = hashRaw % p_self->table_length;

	struct HashmapValue** lp_bucket = hashmap_get_bucket(p_self, false, p_key, hashIndex);

	if (!lp_bucket) { // Check if the pointer itself is NULL.
		return NULL;
	}

	return &(*lp_bucket)->value;
}

void hashmap_combine(struct Hashmap* p_self, const struct Hashmap* p_other) {
	for (size_t index = 0; index < p_other->table_length; index++) {
		struct HashmapValue* lp_bucket = p_other->buckets[index];

		while (lp_bucket) {
			hashmap_set(p_self, lp_bucket->KEY, lp_bucket->value);

			lp_bucket = lp_bucket->next;
		}
	}
}
