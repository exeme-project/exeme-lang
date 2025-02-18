/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./files.h"
#include "./panic.h"
#include "./str.h"
#include <errno.h>
#include <limits.h>
#include <string.h>

void fclose_safe(FILE* p_filePointer) {
	if (fclose(p_filePointer) != 0) {
		PANIC(CONCATENATE_STRING("failed to fclose file pointer: ",
								 strerror(errno))); // NOLINT(concurrency-mt-unsafe)
	}
}

int fgetc_safe(FILE* p_filePointer) {
	int chr = fgetc(p_filePointer);

	if (chr == EOF || chr < 0 || chr > INT_MAX) {
		if (ferror(p_filePointer)) {
			PANIC(CONCATENATE_STRING("failed to fgetc file pointer: ",
									 strerror(errno))); // NOLINT(concurrency-mt-unsafe)
		}
	}

	return chr;
}
