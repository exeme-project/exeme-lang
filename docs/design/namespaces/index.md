<!-- Part of the Exeme Project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

# Namespaces

## Overview

A namespace is a way to group related variables, functions, and structs together. Namespaces are useful for preventing name collisions, and for organizing code.

## Syntax

### Namespace Declaration

To declare a namespace, use the **`namespace`** keyword followed by the namespace name in any file. The namespace name must be a valid identifier. After that, anything declared in the curly braces will be part of the namespace.

```exeme
namespace exeme {
  // ...
}
```

### Referencing Namespaces' Members

To reference a member of a namespace, use the **`::`** operator. The left side of the operator must be a valid namespace identifier, and the right side must be a valid identifier.

::: tip
Namespaces can be nested infinitely. In that case, use the **`::`** operator to reference the nested namespace(s).
:::

### Example

```exeme
namespace github {
  fn auth() {
    ; ...
  }
}

fn main() {
  github::auth()
}
```

## Scoping of Namespaces / Their Members

Namespaces and their members can be brought into the global scope using the **`using`** keyword. This is useful for shortening the code.

::: warning
If there are multiple namespaces with overlapping members, upon bringing the second one into scope the compiler will emit an error.
:::

### Example with Namespace

```exeme
import "github"

using github ; the contents from the github namespace are brought into the global scope

fn main() {
  auth()
}
```

### Example with Namespace Member

```exeme
import "github"

using github::auth ; the auth function from the github namespace is brought into the global scope

fn main() {
  auth()
}
```

## Generic Notes

::: tip
* Whenever you import a package, the compiler will automatically assign the imported code to a namespace with the same name as the package.
:::
