# Namespaces

- [Namespaces](#namespaces)
  - [Overview](#overview)
  - [Syntax](#syntax)
    - [Namespace Declaration](#namespace-declaration)
  - [Referencing Namespaces' Members](#referencing-namespaces-members)
    - [Example](#example)
  - [Using Namespaces / Their Members](#using-namespaces--their-members)

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

## Referencing Namespaces' Members

To reference a member of a namespace, use the **`::`** operator. The left side of the operator must be a valid namespace identifier, and the right side must be a valid identifier.

> [!NOTE]\
> Namespaces can be nested infinitely. In that case, use the **`::`** operator to reference the nested namespace(s).

### Example

```exeme
namespace github {
  fn auth() {
    // ...
  }
}

fn main() {
  github::auth()
}
```


## Using Namespaces / Their Members