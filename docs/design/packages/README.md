# Packages

- [Packages](#packages)
  - [Overview](#overview)
  - [Syntax](#syntax)
    - [Package Declaration](#package-declaration)
  - [Importing Packages](#importing-packages)
    - [Example](#example)
      - [File Structure](#file-structure)
  - [Implicit Package Declaration](#implicit-package-declaration)

## Overview

A package is a collection of related resources that provide functionality for a specific purpose. Packages may contain sub-packages, which may contain sub-packages, infinitely.

## Syntax

### Package Declaration

To declare a folder is a package, use the **`package`** keyword followed by the package name in the **`package.exl`** file. The package name must be a valid identifier.

```exeme
package exeme
```

By default, no functions or variables are exported from a package. To export a function or variable, it has to be declared in the **`package.exl`** file. See [Example](#example) for more information.

> [!NOTE]\
> Package declarations must be the first statement in a file. They are however optional (see [Implicit Package Declaration](#implicit-package-declaration)).

> [!IMPORTANT]\
> If a file is declared to be part of a package, all files in the same directory **must** be part of the same package. This does not mean you have to redeclare the package in each file, but it does mean that you cannot declare a file to be part of a different package.

## Importing Packages

To use a package, use the **`import`** keyword followed by the package identifier. The package identifier must be a valid string literal.

### Example

#### File Structure

```
- exeme
  - main.exl
  - github
    - package.exl
```

```exeme
import "github"

// 'using github::auth' would bring **`auth`** into the global scope

fn main() {
  github::auth()
}
```

> [!NOTE]\
> Use '.' to represent folders in the package identifier. For example, if you have a package called **`github`** with a sub-package called **`auth`**, you would use **`github.auth`** to import the **`auth`** package.

## Implicit Package Declaration

If a file does not contain a package declaration, it is implicitly declared to be the root package. The root package contains the code that will be executed when the program is run. The **`main.exl`** file is the entry point of the program.