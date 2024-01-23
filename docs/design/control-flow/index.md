<!-- Part of the Exeme Project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

# Control Flow

[`Blocks`](../functions/index.md#blocks) of [`statements`](../functions/index.md#statements) are executed sequentially, from top to bottom. This can be changed using control-flow statements, which can be used to change the order of execution, or to execute code conditionally. Some control-flow statements include blocks which are always written in curly brackets.

```exeme
import "std.io"

using io::out

func main() {
    if true {
        out("Hello, world!")
    }
}
```

## Conditionals

### `if`, `else`, and `elif`

`if`, `else`, and `elif` are used to execute code conditionally. `if` and `elif` are followed by a condition, and `else` is not. If the condition is true, the code in the block is executed. If the condition is false, the code in the block is not executed. `elif` is short for "else if". The syntax is as follows:

> `if condition { statements }`
>
> `elif condition { statements }`
>
> `else { statements }`

A block is executed if:

* `if` is followed by a condition which is true.
* `elif` is followed by a condition which is true, and the previous `if` or `elif(s)` was not executed.
* `else` is not followed by a condition, and the previous `if` or `elif(s)` was not executed.

In other words, when a boolean expression is true, other branches are not evaluated and so not executed.

::: info
`if` and `else` can only be used once in a block, but `elif` can be used multiple times.
:::

An example is as follows:

```exeme
import "std.io"
import "exeme"

using io::out

func main() {
    if exeme.isStable() {
        out("Stable!")
    } elif exeme.isBeta() {
        out("Beta.")
    } else {
        out("Alpha.")
    }

    out("Done!")
}
```

The code will:

* Evaluate `exeme.isStable()`
  * When `True`, it will print `Stable!` and then jump to `out("Done!")`.
  * When `False`, it will evaluate `exeme.isBeta()`.
    * When `True`, it will print `Beta.` and then jump to `out("Done!")`.
    * When `False`, it will print `Alpha.` and then jump to `out("Done!")`.

## Loops

Loops are provided using the `while` and `for` statements. Within a loop, the `break` and `continue` statements can be used for control flow.

### `while`

`while` statements are used to execute a block of code repeatedly, until a condition is false. The syntax is as follows:

> `while (condition) { statements }`

The following example prints the numbers 0 to 9 (inclusive):

```exeme
import "std.io"

using io::out

func main() {
    i = 0

    while i < 10 {
        out(i)

        i += 1
    }
}
```

### `for`

`for` statements are used to execute a block of code repeatedly, for each item in a iterable. The syntax is as follows:

> `for index, item = range(collection) { statements }`

The following example prints the numbers 0 to 9 (inclusive):

```exeme
import "std.io"

using io::out

func main() {
    for _, num = range(0, 10) {
        out(num)
    }
}
```

### `break`

The `break` statement is used to immediately exit a loop, and resume execution at the end of the loop's scope. The syntax is as follows:

> `break`

The following example prints the numbers 0 to 9 (inclusive), but stops when it reaches 5:

```exeme
import "std.io"

using io::out

func main() {
    for _, num = range(0, 10) {
        if num == 5 {
            break
        }

        out(num)
    }
}
```

### `continue`

The `continue` statement is used to immediately jump to the next iteration of a loop. The syntax is as follows:

> `continue`

The following example prints the numbers 0 to 9 (inclusive), but skips 5:

```exeme
import "std.io"

using io::out

func main() {
    for _, num = range(0, 10) {
        if num == 5 {
            continue
        }

        out(num)
    }
}
```

## Misc

### `return`

The `return` statement is used to return a variable from a function to the caller. The syntax is as follows:

> `return expression`

::: info
If the `return` statement is not followed by an expression, it will return `null`. This value is also returned implicitly if the function ends without a `return` statement.
:::

The following example prints the numbers 0 to 9 (inclusive), but stops when it reaches 5:

```exeme
import "std.io"

using io::out

func printNumbers() {
    for _, num = range(0, 9) {
        if num == 5 {
            return
        }

        out(num)
    }
}

func main() {
    printNumbers()
}
```

### `match`

A
