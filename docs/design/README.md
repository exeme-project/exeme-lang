<!-- Part of the Exeme language project, under the MIT license. See '/LICENSE' for
license information. SPDX-License-Identifier: MIT License. -->

# Language design

- [Language design](#language-design)
  - [Introduction](#introduction)
  - [The basics](#the-basics)
    - [`import` declaration](#import-declaration)
    - [`using` declaration](#using-declaration)
    - [Comments](#comments)
    - [Funtion declaration](#funtion-declaration)
    - [Variable declaration](#variable-declaration)
    - [Control flow statements](#control-flow-statements)
  - [Code](#code)
  - [Comments](#comments-1)
  - [Build modes](#build-modes)
  - [Primitive types](#primitive-types)
    - [`bool`](#bool)
    - [Integer types](#integer-types)
      - [Signed integer types](#signed-integer-types)
      - [Unsigned integer types](#unsigned-integer-types)

## Introduction

This documentation is an overview of the Exeme project in its current state, and is written for the maintainers of Exeme, and for those interested in learning more about Exeme. It is a description of the design of the Exeme language, and the rationale behind the design.

This document is *not* a complete programming manual. It does not provide a detailed and comprehensive justification for design decisions. See the linked pages for more detailed justifications.

## The basics

Here is a simple function showing some Exeme code:

> **Note** This section does not provide in-depth knowledge into the language's constructs, but instead provides an overview. If you want more to know more about certain constructs, there are links to more detailed descriptions.

```
import "std.io"

using io::out

# Returns whether `n` itself is prime.
fn isPrime(n: i32) -> bool {
    for _, d = range(2, n) { # '_' is the index, but it is not needed
        if n % d == 0 { # Can be divided without a remainder
            return false
        }
    }

    return true
}

fn main() {
    out(isPrime(100))
}
```

### `import` declaration

The example starts with an [`import` declaration](). Exeme imports are akin to [C++ modules](https://en.cppreference.com/w/cpp/language/modules) rather than [source file inclusing during preprocessing](https://en.cppreference.com/w/cpp/preprocessor/include). The `import` declaration imports the `io` library from the `std` package (see [what are files, libraries, and packages]()). It must appear at the top of an Exeme source file, the first line after the [optional `package` declaration]().  Libraries can be optionally split into [api and implementation files](), similar to C++'s header and source files. The import from the example:

```import "std.io"```

imports the default library for the package `std.io`. The names from this library are accessible as members of `io`, such as `io::out`, and `io::in`. Unlike C++, different packages are automatically assigned their own namespace upon import, so there are no name conflicts.

### `using` declaration

In the next line, a familiar declaration to C++ developers can be found - [`using`](). `using` introduces a name that is defined elsewhere into the scope where the using-declaration appears. This allows us to use the `out` function in the `io` namespace in the global namespace.

### Comments

Exeme comments can be placed anywhere (on a line by themselves, at the end of lines, etc), and have to start with `#`.

```
# Returns whether `n` itself is prime.
...
    for _, d = range(2, n) { # '_' is the index, but it is not needed.
        if n % d == 0 { # Can be divided without a remainder.
```

### Funtion declaration

A [function declaration]() consits of:

* The `fn` keyword introducer,
* The function's name,
* A parameters list in round parenthesis `(`...`)`,
* An optional `->` followed by return type(s),
* A body inside curly braces `{`...`}`.

```
fn isPrime(n: i32) -> bool {
    ...
        ...
            return false
        ...
    ...
    return true
}
```

The body of the function is a sequence of [statements]() and [declarations]() executed in chronological order. Function execution ends when it reaches a `return` statement, or the end of the function body. `return` statements can also specify an expression whose value(s) are returned.

Here `i32` refers to a signed [integer type](), with 32-Bits, and `bool` is the [boolean type](). Exeme also has [floating-point types]() such as `f32`, and [string types]().

### Variable declaration

A [variable decleration]() has 2 parts:

*  The name (optionally followed by `:` and the data type, declared in the same way as a parameter in a function signature), and
*  The initial value for the variable.

```
i: i32 = 1
```

Exeme has short variable decleration, which means the compile can guess the value of your variable. This means the code can be shortened to:

```
i = 1
```
> **Note** This may not work well with more complex data structures.

You can modify the value of a variable with an [assignment statement](assignment.md):

```
i = 1
i += 1
i -= 1
```

Constatants are declared with the `const` data type. Constants are effectively a container for their inner data type, that do not allow overwriting of the inner value.

```
i: const(i32) = 1
```

### Control flow statements

These statements (including `if`, `while`, `for`, `break`, and `continue`), change the order that statement are executed, like in C++:

```
    for _, d = range(2, n) {
        if n % d == 0 {
            ...
        }
    }
```

Every code block in curly braces defines a scope. Names are visible from their decleration until the end of the scope they were declared into (unless they are a [global variable]()). In this example, `d` is only defined until the end of the for loop.

## Code

All source code should be encoded in `utf-8`. Comments, identifiers, and strings are allowed to have non-ASCII (Unicode) characters.

```
cómo = "no mágica" # In English, `how = "non-magical"`
```

## Comments

Comments start with a hashtag `#`, and span the rest of the line. They are completely ignored by the compiler.

```
# Can be divided without a remainder
```

## Build modes

The behavoior of the Exeme compiler, and the performance of the resultant executable depends on the *build mode*:

* In a ***development build***, the priority is diagnosing problems, so build times are the fastest.
* In a ***performance build***, the priority is a highly optimised binary, with faster execution times.

## Primitive types

Primitive types fall into the following categories:

* The boolean type `bool`
* Signed and unsigned integer types (e.g., `i32`, `ui32`)
* Floating-point types (e.g, `f32`, `f64`)
* String types

These types are available by default in any Exeme code due to the [prelude]().

### `bool`

The type `bool` is a boolean with two possible values - `true` or `false`. `bool`, `true`, and `false` are keywords. [Comparison expressions]() produce values of the `bool` type. The arguments in [control-flow statements]() (such as [`if`]() and [`while`]()) take `bool` values.

### Integer types

> **Note** `X` has to be a positive multiple of 8. `0` is neither positive nor negative, and so is not valid.

#### Signed integer types

The signed-integer type with bit width `X` may be written as `iX`. For example, `i32` is a signed 32-Bit integer.

#### Unsigned integer types

The unsigned-integer type with bit width `X` may be written as `uiX`. For example, `ui32` is an unsigned 32-Bit integer.