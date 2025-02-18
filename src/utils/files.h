/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include <stdio.h>

/**
 * Safely closes a file pointer.
 *
 * @param p_filePointer The file pointer to close.
 */
void fclose_safe(FILE* p_filePointer);

/**
 * Safely gets a character from a file pointer.
 *
 * @param p_filePointer The file pointer to get the character from.
 *
 * @return The character.
 */
int fgetc_safe(FILE* p_filePointer);
