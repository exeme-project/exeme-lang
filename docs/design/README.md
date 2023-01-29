# Language design

> **Note** Last updated 28.01.2023. This document's contents are subject to change at any time.

- [Language design](#language-design)
  - [Introduction](#introduction)
  - [The basics](#the-basics)

## Introduction

This documentation is an overview of the Exeme project in its current state, and is written for the maintainers of Exeme, and for those interested in learning more about Exeme. It is a description of the design of the Exeme language, and the rationale behind the design.

This document is *not* a complete programming manual. It does not provide a detailed and comprehensive justification for design decisions. See the linked pages for more detailed justifications.

## The basics

Here is a simple function showing some Exeme code:

```
package main

import "std.io"

using std::in, std::out

fn main() {
    for _, num = range(101) {
        if num % 15 == 0 {
            out("FizzBuzz")
        } elif num % 5 == 0 {
            out("Buzz")
        } elif num % 3 == 0 {
            out("Fizz")
        } else {
            out(num) # num gets converted to a string in the out function.
        }
    }
}
```