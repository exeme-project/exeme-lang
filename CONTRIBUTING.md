<!-- Part of the Exeme language project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

# Contributing Guidelines

- [Contributing Guidelines](#contributing-guidelines)
  - [Welcome](#welcome)
  - [Where to make Changes](#where-to-make-changes)
  - [Adding Docs](#adding-docs)
  - [Contributors File](#contributors-file)
  - [Style](#style)
    - [Code Style and Syntax Rules](#code-style-and-syntax-rules)
  - [License](#license)
    - [Markdown](#markdown)
    - [C Source Code](#c-source-code)
    - [Other Files](#other-files)

## Welcome

Thank you for your interest in contributing to Exeme. There are many different ways to contribute, and they are all appreciated. If you have any questions, feel free to ask on the [**GitHub**](https://github.com/exeme-project/exeme-lang/discussions).

Everyone who contributes to Exeme is expected to:

* Read the [**Code of Conduct**](https://github.com/exeme-project/exeme-lang/blob/main/CODE_OF_CONDUCT.md). We expect everyone in our community to be welcoming and friendly.

## Where to make Changes

The core libraries and documentation are the areas that are in most need of input. Any enhancements to parts of the compiler are welcome.

## Adding Docs

If you add new functionality, or change the way current functionality works, add or edit the docs to reflect this. This is so that users have documentation of the new changes.

## Contributors File

Remember to add your name, GitHub profile, and email address to the official [**Contributors File**](https://github.com/exeme-project/exeme-lang/blob/main/CONTRIBUTORS.md).

## Style

### Code Style and Syntax Rules

You can easily follow the same coding style and syntax rules by looking at the Exeme source code. It is also recommended to install clang-format, which can format your code using the [**src/.clang-format**](https://github.com/exeme-project/exeme-lang/blob/main/src/.clang-format) file.

## License

licenses are required at the top of all non-trivial files.

### Markdown

Markdown files always have at the top:

```md
<!-- Part of the Exeme language project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->
```

For example, see the top of [**this file's**](https://raw.githubusercontent.com/exeme-project/exeme-lang/main/CONTRIBUTING.md) raw contents.

### C Source Code

C source code files always have at the top:

``` c
// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.
```

For example, see the top of [**src/main.c**](https://github.com/exeme-project/exeme-lang/blob/main/src/main.c) file's contents.

### Other Files

Every file type uses a variation of the above license text (*MIT License*) with similar formatting.