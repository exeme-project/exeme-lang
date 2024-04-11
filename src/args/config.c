/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "../includes.c"

#include "../utils/array.c"

extern const struct Array CONFIG;

/**
 * Represents an argument.
 */
struct Arg {
    bool required;
    char *description, *name, *flagShort, *flagLong;
};

/*
 * Represents the config for parsing arguments.
 */
const struct Array CONFIG = {
    1,
    (const void *[]){&(struct Arg){
                         true,
                         "The path of the file to compile",
                         "file",
                         NULL,
                         NULL,
                     },
                     &(struct Arg){
                         false,
                         "The path to the folder containing the standard library",
                         "stdlib",
                         "-s",
                         "--stdlib",
                     }},
};

/**
 * Parses the arguments.
 *
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @return The parsed arguments.
 */
