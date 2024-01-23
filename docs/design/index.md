<!-- Part of the Exeme Project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

# Design

This folder contains all documentation related to the design of the Exeme language. This includes the language specification, and the language design. It is split into sub-folders, each of which contains documentation related to a specific topic. For example, the [`packages/`](packages/) folder contains documentation related to packages.

## Introduction

This documentation is an overview of the Exeme project in its current state, and is written for the maintainers of Exeme, and for those interested in learning more about Exeme. It is a description of the design of the Exeme language, and the rationale behind the design.

::: info
This document is **not** a complete programming manual. It does not provide a detailed and comprehensive justification for design decisions. See the sub-folders and linked pages for more detailed justifications.
:::

## The Basics

::: tip
This section does not provide in-depth knowledge into the language's constructs, but instead provides an overview. If you want more to know more about certain constructs, there are links to more detailed descriptions.
:::

```exeme
import "std.io"

using io::out

; Returns whether `n` itself is prime.
func isPrime(n: i32) -> bool {
    for _, d = range(2, n) { ; '_' is the index, but it is not needed
        if n % d == 0 { ; Can be divided without a remainder
            return false
        }
    }

    return true
}

func main() {
    out(isPrime(100))
}
```

The examples starts with an [`import` declaration](packages/index.md#importing-packages), used to import packages. The `import` declaration is a top-level declaration, and must be the first declaration in a file (after the optional package declaration). In this example it imports the `io` sub-package from the `std` package.

::: tip
To find out more about packages, see [`packages/`](packages/).
:::

The [`using` declaration](namespaces/#scoping-of-namespaces-their-members) brings the `out` function from the `io` namespace into the global scope. This means that it can be used without having to use the `io::` prefix.

::: tip
To find out more about namespaces, see [`namespaces/`](namespaces/).
:::

This is followed by a [`comment`](comments/), more specifically a single-line comment. Comments are ignored by the compiler, and are used to document code.

Next is a [`function declaration`](functions/#function-declaration), consisting of:

* The `func` keyword, introducing the function declaration.
* The function name, `isPrime`.
* A list of parameters enclosed in round brackets. In this case, the function takes a single parameter, `n`, of type `i32`.
* An optional return type, `bool`, and the associated `->` token.
* A body enclosed in curly brackets.

The body is a [`block`](functions/index.md#blocks), containing [`statements`](functions/index.md#statements).

## Code Encoding

### Comments

a
