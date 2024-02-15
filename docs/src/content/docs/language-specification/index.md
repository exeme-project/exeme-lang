---
title: Overview
description: A brief overview of common concepts in The Exeme Language.
---

This documentation provides an overview of the *design* of **The Exeme Language**. It is also split into *sub-folders*, each of which contain in-depth **documentation** on the design of a *specific* part of the language.

:::note
This document is **provisional**, and so *subject to change*. This means that any and all information contained within this document is **not** guaranteed to be *accurate* or *up-to-date*.
:::

## The Basics

The Exeme Language is a **statically-typed**, **compiled** programming language. This is a simple function utilising the *basics* of the language:

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

The example starts with an [**`import` statement**](/language-specification/packages#importing-packages), used to import a [*package*](/language-specification/packages). `import` is a **top-level** declaration, and must be the first declaration in a file. The `import` statement is followed by a **string literal**, which is the package identifier. In this case it imports the *`io`* sub-package of the *`std`* package.

The [**`using` statement**](/language-specification/namespaces#using-statements) brings the *`out`* function from the *`io`* package into the current scope. This means that the *`out`* function can be used without having to prefix it with *`io::`*.

This is followed by a [**single-line comment**](/language-specification/comments#single-line-comments). Comments are ignored by the compiler, and are used to provide *context* to the code.
