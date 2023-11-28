<!-- Part of the Exeme Project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

# Packages

- [Packages](#packages)
  - [Overview](#overview)
  - [Syntax](#syntax)
    - [Package Declaration](#package-declaration)
    - [Package Requirements](#package-requirements)
    - [Importing Packages](#importing-packages)
    - [Example](#example)
      - [File Structure](#file-structure)
        - [`main.exl`](#mainexl)
        - [`github/package.exlp`](#githubpackageexlp)
        - [`github/github.exl`](#githubgithubexl)

## Overview

A package is a collection of related resources that provide functionality for a specific purpose. Packages may contain sub-packages, which may contain sub-packages, infinitely.

## Syntax

### Package Declaration

To declare a folder is a package, use the **`package`** keyword followed by the package name in the **`package.exlp`** file. The package name must be a valid identifier.

```exeme
; package.exlp

package github
```

By default, no functions or variables are exported from a package. To export a function or variable, it has to be declared with the **`extern`** keyword. See [Example](#example) for more information.

### Package Requirements

Packages may have external requirements. To declare a package requirement, use the **`require`** keyword followed by the package identifier, and the package version. The package identifier must be a valid link to a valid repository (that can be understood by **`git`**), that is a valid package. The package version must be a valid tag.

```exeme
; package.exlp

package github

require (
  github.com/exeme-project/example-package v1.0.0
)
```

### Importing Packages

To use a package, use the **`import`** keyword followed by the package identifier. The package identifier must be a valid string literal.

::: tip
Use `.` to represent folders in the package identifier. For example, if you have a package called **`github`** with a sub-package called **`auth`**, you would use **`github.auth`** to import the **`auth`** package.

When you import a package, it automatically is assigned its own namespace. Please note that this is based on the package's name, and is irrespective of whether it is a subpackage. For example, if you import the **`github.auth`** package, it will be assigned the namespace **`auth`**, not **`github_auth`**.

The standard library takes precedence over user-defined packages. For example if there is a package in a sub-folder of the current folder called **`std`**, it will not be imported if you use **`import "std"`**.
:::

::: info
You can use the **`using`** keyword to change the scope of the imported package / its members. To learn more, see [**`../namespaces/`**](../namespaces/).
:::

### Example

#### File Structure

```
- main.exl
- github
  - package.exlp
  - github.exl
```

##### `main.exl`

```exeme
import "github"

fn main() {
  github::auth()
}
```

##### `github/package.exlp`

```exeme
package github

require (
  github.com/exeme-project/example-package v1.0.0
)
```

##### `github/github.exl`

```exeme
extern fn auth() {
  ; ...
}
```
