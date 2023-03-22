// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include "../includes.c"
#include "./array.c"

struct map {
	struct array *_values;
};

#define MAP_STRUCT_SIZE sizeof(struct map)

struct map *array_map(void) {
	struct map *self = malloc(MAP_STRUCT_SIZE);

	self->_values = array_new();

	return self;
}

size_t map___hash(const char *key) {
	size_t hash = 5381;
	int chr;

	while ((chr = *key++)) {
		hash = ((hash << 5) + hash) + (size_t)chr; /* hash * 33 + c */
	}

	return hash;
}

void map_set(struct map *self, const char *KEY, void *value) {
	array_insert(self->_values, map___hash(KEY), value);
}

void *map_get(struct map *self, const char *KEY) {
	return array_get(self->_values, map___hash(KEY));
}
