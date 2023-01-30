<!-- Part of the Exeme language project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

# Assignment

- [Assignment](#assignment)
  - [Overview](#overview)
  - [Syntax](#syntax)
  - [Compund assignment semantics](#compund-assignment-semantics)
  - [Built-in types](#built-in-types)

## Overview

Values can be assigned to variables using the `=` operator.

```
i = 1 # Or i: i32 = 1
```

For each [arithmetic](expressions/arithmetic.md) operator `$`, a corresponding compund assignment `$=` is provided, that performs the same operation *in-place*.

```
i += 1 # Same as `a = a + 1`
```

## Syntax

The operands of the `=` operator can be any [expression](expressions/).

## Compund assignment semantics

The syntax `x $= y` is intended to be a shortened form of `x = x $ y`, except in the following cicrumstances:

* A type may be able to provide a more efficient implementation for the compound assignment than for the uncombined assignment.
* A type may not be able to provide the longer form at all, for example if it does not have additional resources that are not avilable.

## Built-in types

Integers, floats, etc support simple assignment with `=`. The right-hand operand is implicitly converted into the type of the left-hand operand, and replaces the value of the original operand. For example:

```
i: f32 = 1
```

will set `i` to `1.0`, not `1`.