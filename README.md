# Exeme Programming Language

[![Licensed under the MIT License](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/skifli/exeme/blob/master/LICENSE)
[![PR's Welcome](https://img.shields.io/badge/PRs%20-welcome-brightgreen.svg)](https://github.com/skifli/exeme/pulls)
[![Website Status](https://img.shields.io/website?down_message=offline&up_message=online&url=https%3A%2F%2Fskifli.github.io%2Fexeme)](https://skifli.github.io/exeme)

- [Exeme Programming Language](#exeme-programming-language)
  - [The Exeme Project](#the-exeme-project)
  - [Goals](#goals)
  - [Project status](#project-status)
  - [Join us](#join-us)


## The Exeme Project

C++ remains the predoimnant language for performance-critical software. However, it is struggling to keep up with the needs of modern developers, in no small part due to accumulating decades of technical debt. Incrementally improving C++ is difficult, not just due to the technical complexities, but also due to challenges with its evolution process. The best way to address these problems is to avoid inheriting the legacy of C / C++, and instead start with solid language foundations like modular code orgainisation, and a simple syntax.

Many modern languages already provide an excellent developer experience - **Go**, **Rust**, and **Kotlin**, are examples among many others. Developers that ***can*** use one of these languages ***should***. However, the design of these languages present significant barriers to migration from C++. These barries range from the language's design, to performance overheads.

Exeme is fundamentally a **successor language**, rather than an attempt to incrementally improve C++, and is designed around interoperability with C++. In fact, the standard library will be implemented in C++ for higher performance. A successor language to C++ requires:

* **Performance matching C++**, an essential property required for developers who are migrating.
* **Seamless interoperability with C++**, such that a library written in C++ can be easily integrated into an Exeme project, without porting it in its entirety.
* **A gentle learning curve**, with *reasonable* familiarity for C++ developers.

With this approach we can build upon C++'s existing ecosystem, and bring along countless existing investments, codebases, and developers populations. There are few languages that have followed this model, and Exeme aims to fill the role for C++.

* JavaScript -> TypeScript
* Java -> Kotlin
* C++ -> ***Exeme***

## Goals

Exeme is designed to:

* Run as fast as C++.
* Be written in a way that is easy to read, understand, and write. High quality code merits easy development, and Exeme embodies this.
* Support practical code safety / testing mechanisms.
* Support fast and scalable development.
* Support all modern OS platforms, and provide APIs that are OS independent.
* Have interoperability with existing C++ code, leading to easy migration.

While many languages share subsets of these goals, the combination of them together into one programming language is what sets Exeme apart.

> **Note** If you are interested in a deeper understanding of our goals, the [goals](docs/project/goals.md) document fleshes out these ideas, and provides a deeper view into the goals for the Exeme project.

## Project status

Exeme is ***not*** ready for use. It is currently a work in progress, but there is no working compiler / toolchain ***yet***. If you're interested in contributing, we would love help with completing the compiler. You can see the [full roadmap](/docs/project/roadmap.md) for more details.

## Join us

Exeme is commited to having a welcoming and inclusive environment where everyone can contribute.

* Most of the activity relating to Exeme happens on our [Discord](https://discord.gg/9SCfXMZKSf).
* See our [code of conduct](CODE_OF_CONDUCT.md) and [contributing guidelines](CONTRIBUTING.md) for information about the Exeme development community.