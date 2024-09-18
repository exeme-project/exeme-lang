/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "./includes.c"

#define LONG_SIZE sizeof(long)
#define FLOAT_SIZE sizeof(float)

const size_t negativeULL = (size_t)-1;

enum VariableType {
    VARIABLE_TYPE_STRING,
    VARIABLE_TYPE_INT,
    VARIABLE_TYPE_FLOAT,
    VARIABLE_TYPE_BOOL,
};
