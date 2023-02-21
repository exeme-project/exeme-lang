// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#if defined(_WIN32) || defined(_WIN64)
#define OS 1
#elif defined(__linux__)
#define OS 2
#elif defined(__APPLE__) || defined(__MACH__)
#define OS 3
#elif defined(__unix) || defined(__unix__)
#define OS 4
#else
#define OS 0
#endif

const auto negative_index = static_cast<size_t>(-1);
