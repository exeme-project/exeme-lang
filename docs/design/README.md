# Design

- [Design](#design)
  - [Introduction](#introduction)
  - [Important Notice](#important-notice)
  - [The Basics](#the-basics)
    - [**`import`** declaration - *Packages*](#import-declaration---packages)

## Introduction

This documentation is an overview of the Exeme project in its current state, and is written for the maintainers of Exeme, and for those interested in learning more about Exeme. It is a description of the design of the Exeme language, and the rationale behind the design.

## Important Notice

> [!IMPORTANT]\
> This document is **not** a complete programming manual. It does not provide a detailed and comprehensive justification for design decisions. See the sub-folders and linked pages for more detailed justifications.

## The Basics

> [!NOTE]\
> This section does not provide in-depth knowledge into the language's constructs, but instead provides an overview. If you want more to know more about certain constructs, there are links to more detailed descriptions.

```exeme
import "std.io"

using io::out

; Returns whether `n` itself is prime.
fn isPrime(n: i32) -> bool {
    for _, d = range(2, n) { ; '_' is the index, but it is not needed
        if (n % d == 0) { ; Can be divided without a remainder
            return false
        }
    }

    return true
}

fn main() {
    out(isPrime(100))
}
```

### **`import`** declaration - [*Packages*](packages/README.md)

The *`import`* declaration is used to import packages. The *`import`* declaration is a top-level declaration, and must be the first declaration in a file (after the optional package declaration). In this example it imports the *`io`* subpackage from the *`std`* package.

> [!NOTE]\
> To find out more about packages, see **[`packages/README.md`](packages/README.md)**.