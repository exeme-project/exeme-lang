/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "./panic.c"

/**
 * Converts the char into a string.
 *
 * @param chr The char to convert into a string.
 *
 * @return The repeated string.
 */
char *chrToString(char chr) {
    char *string = malloc(2);

    if (!string) {
        panic("failed to malloc string");
    }

    string[0] = chr;
    string[1] = '\0';

    return string;
}

/**
 * Converts the unsigned long into a string.
 *
 * @param num The unsigned long to convert into a string.
 *
 * @return The converted string.
 */
char *ulToString(size_t num) {
    size_t length = (size_t)snprintf(NULL, 0, "%zu", num);

    char *str = malloc(length + 1);

    if (!str) {
        panic("failed to malloc string");
    }

    snprintf(str, length + 1, "%zu", num);

    return str;
}

/**
 * Converts the data contained in a string into its corresponding type.
 *
 * @param data The data to convert.
 * @param type The type to convert to.
 *
 * @return The converted data.
 */
void *convertToType(char *data, enum VariableType type) {
    switch (type) {
    case VARIABLE_TYPE_STRING:
        return data;
    case VARIABLE_TYPE_INT: {
        long *result = malloc(LONG_SIZE);

        if (!result) {
            panic("failed to malloc long while converting to int");
        }

        char *endptr;
        *result = strtol(data, &endptr, 10);

        if (endptr == data || *endptr != '\0') {
            free(result);

            return NULL;
        }

        return result;
    }
    case VARIABLE_TYPE_FLOAT: {
        float *result = malloc(FLOAT_SIZE);

        if (!result) {
            panic("failed to malloc float while converting to float");
        }

        char *endptr;
        *result = strtof(data, &endptr);

        if (endptr == data || *endptr != '\0') {
            free(result);

            return NULL;
        }

        return result;
    }
    case VARIABLE_TYPE_BOOL:
        return strcmp(data, "true") == 0 ? (void *)1 : (void *)0;
    default:
        panic("invalid variable type for conversion");
    }
}
