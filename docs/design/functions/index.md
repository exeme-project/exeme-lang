<!-- Part of the Exeme Project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

# Functions

- [Functions](#functions)
  - [Overview](#overview)
  - [Syntax](#syntax)
    - [Function Declaration](#function-declaration)
    - [Calling Functions](#calling-functions)

## Overview

A function is a block of code that can be called from anywhere in the program. Functions are useful for code reuse and for organizing code.

## Syntax

### Function Declaration

To declare a function, use the **`func`** keyword followed by the function name. The function name must be a valid identifier. Then, in parentheses, declare the function's parameters. The parameters must be valid identifiers with types, akin to variable declarations. They may also optionally have a default value, in the same syntax as a variable declarations. Then optionally, after the parentheses, declare the function's return type. The return type must be a valid type, and must appear after the **`->`** operator. After that, anything declared in the curly braces will be part of the function.

::: info
The default return type is **`null`**, and if the return type is not specified, this will be the inferred return type.
:::

```exeme
func add(a: i32, b: i32) -> int {
  return a + b
}
```

### Calling Functions

To call a function, use the the function's name followed by parentheses. Inside the parentheses, pass the function's arguments. The arguments must be valid expressions.

```exeme
func main() {
  add(1, 2)
}
```
