.. raw:: html

   <!-- Part of the Exeme language project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

########
 Design
########

**************
 Introduction
**************

This documentation is an overview of the Exeme project in its current
state, and is written for the maintainers of Exeme, and for those
interested in learning more about Exeme. It is a description of the
design of the Exeme language, and the rationale behind the design.

This document is *not* a complete programming manual. It does not
provide a detailed and comprehensive justification for design decisions.
See the linked pages for more detailed justifications.

************
 The Basics
************

Here is a simple function showing some Exeme code:

.. note::

   This section does not provide in-depth knowledge into the language's
   constructs, but instead provides an overview. If you want more to
   know more about certain constructs, there are links to more detailed
   descriptions.

.. code::

   import "std.io"

   using io::out

   # Returns whether `n` itself is prime.
   fn isPrime(n: i32) -> bool {
       for _, d = range(2, n) { # '_' is the index, but it is not needed
           if (n % d == 0) { # Can be divided without a remainder
               return false
           }
       }

       return true
   }

   fn main() {
       out(isPrime(100))
   }

``import`` declaration
======================

The example starts with an :doc:`import declaration <index>`. Exeme
imports are akin to `C++ modules
<https://en.cppreference.com/w/cpp/language/modules>`_ rather than
`source file inclusion during preprocessing
<https://en.cppreference.com/w/cpp/preprocessor/include>`_. The
``import`` declaration imports the ``io`` library from the ``std``
package (see :doc:`what are files, libraries, and packages <index>`). It
must appear at the top of an Exeme source file, the first line after the
*optional* :doc:`package declaration <index>`. Libraries can be
optionally split into :doc:`api and implementation files <index>`,
similar to C++'s header and source files. The import from the example:

``import "std.io"``

imports the default library for the package ``std.io``. The names from
this library are accessible as members of ``io``, such as ``io::out``,
and ``io::in``. Unlike C++, different packages are automatically
assigned their own namespace upon import, so there are no name
conflicts.

``using`` declaration
=====================

In the next line, a familiar declaration to C++ developers can be found
- :doc:`using <index>`. ``using`` introduces a name that is defined
elsewhere into the scope where the using-declaration appears. This
allows us to use the ``out`` function from the ``io`` namespace in the
global namespace.

Comments
========

Exeme comments can be placed anywhere (on a line by themselves, at the
end of lines, etc), and have to start with ``#``.

.. code::

   # Returns whether `n` itself is prime.
   ...
       for _, d = range(2, n) { # '_' is the index, but it is not needed.
           if (n % d == 0) { # Can be divided without a remainder.

Function Declaration
====================

A :doc:`function declaration <index>` consists of:

-  The ``fn`` keyword introducer,
-  The function's name,
-  A parameters list in round parenthesis ``(``\ …\ ``)``,
-  An optional ``->`` followed by return type(s), and
-  A body inside curly braces ``{``\ …\ ``}``.

.. code::

   fn isPrime(n: i32) -> bool {
       ...
           ...
               return false
           ...
       ...
       return true
   }

The body of the function is a sequence of :doc:`statements <index>` and
:doc:`declarations <index>` executed in chronological order. Function
execution ends when it reaches a ``return`` statement, or the end of the
function body. ``return`` statements can also specify an expression
whose value(s) are returned.

Here ``i32`` refers to a signed :doc:`integer type <index>`, with
32-Bits, and ``bool`` is the :doc:`boolean type <index>`. Exeme also has
:doc:`floating-point types <index>` such as ``f32``, and :doc:`string
types <index>`.

Variable Declaration
====================

A :doc:`variable declaration <index>` has 2 parts:

-  The name (optionally followed by ``:`` and the data type, declared in
   the same way as a parameter in a function signature), and
-  The initial value for the variable.

.. code::

   i: i32 = 1

Exeme has short variable declaration, which means the compile can guess
the value of your variable. This means the code can be shortened to:

.. code::

   i = 1

..

.. note::

   This may not work well with more complex data structures.

You can modify the value of a variable with an :doc:`assignment
statement <assignment>`:

.. code::

   i = 1
   i += 1
   i -= 1

Constant Variable Declaration
-----------------------------

Constants are declared by subscripting the ``const`` type with the
variable's type. Constants are effectively a container for their inner
data type that do not allow overwriting of the inner value.

.. code::

   i: const[i32] = 1

Control Flow Statements
=======================

These statements (including ``if``, ``while``, ``for``, ``break``, and
``continue``), change the order that statement are executed, like in
C++:

.. code::

   for _, d = range(2, n) {
       if (n % d == 0) {
           ...
       }
   }

Every code block in curly braces defines a scope. Names are visible from
their declaration until the end of the scope they were declared into
(unless they are a :doc:`global variable <index>`). In this example,
``d`` is only defined until the end of the for loop.

******
 Code
******

All source code should be encoded in ``utf-8``. Comments, identifiers,
and strings are allowed to have non-ASCII (Unicode) characters.

.. code::

   cómo = "no mágica" # In English, `how = "non-magical"`

**********
 Comments
**********

Comments start with a hashtag ``#``, and span the rest of the line. They
are completely ignored by the compiler.

.. code::

   # Can be divided without a remainder

*************
 Build Modes
*************

The behavior of the Exeme compiler, and the performance of the resultant
executable depends on the *build mode*:

Development Build
=================

The priority is diagnosing problems, so build times are the fastest.

Performance Build
=================

The priority is a highly optimized binary, with faster execution times.

*****************
 Primitive Types
*****************

Primitive types fall into the following categories:

-  The boolean type ``bool``,
-  Signed and unsigned integer types (e.g., ``i32``, ``ui32``),
-  Floating-point types (e.g, ``f32``, ``f64``), and
-  String types.

These types are available by default in any Exeme code due to the
`prelude <index>`_.

``bool``
========

The type ``bool`` is a boolean with two possible values - ``true`` or
``false``. ``bool``, ``true``, and ``false`` are keywords.
:doc:`Comparison expressions <index>` produce values of the ``bool``
type. The arguments in :doc:`control-flow statements <index>` (such as
:doc:`if <index>` and :doc:`while <index>`) take ``bool`` values.

Integer Types
=============

.. note::

   ``X`` has to be a positive multiple of 8. ``0`` is neither positive
   nor negative, and so is not valid.

Signed Integer Types
--------------------

The signed-integer type with bit width ``X`` may be written as ``iX``.
For example, ``i32`` is a signed 32-Bit integer.

Unsigned Integer Types
----------------------

The unsigned-integer type with bit width ``X`` may be written as
``uiX``. For example, ``ui32`` is an unsigned 32-Bit integer.

.. toctree::
   :hidden:
   :glob:

   expressions/index.rst
   *
