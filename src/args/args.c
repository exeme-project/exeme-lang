/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "../includes.c"

#include "../utils/panic.c"

/**
 * Represents arguments.
 */
struct Args {
	char **argv;
	int argc;
};

#define ARGS_STRUCT_SIZE sizeof(struct Args)

/* Forward declarations to silence warnings */
void args_parse(struct Args *self);

/**
 * Creates a new Args struct.
 *
 * @param argc The number of arguments.
 * @param argv The arguments.
 *
 * @return The created Args struct.
 */
struct Args *args_new(int argc, char **argv) {
	struct Args *self = malloc(ARGS_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc Args struct");
	}

	self->argv = argv;
	self->argc = argc;

	args_parse(self);

	return self;
}

void args_parse(struct Args *self) {}
