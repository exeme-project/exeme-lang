.. raw:: html

   <!-- Part of the Exeme language project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

######
 Home
######

******
 Why?
******

C++ remains the dominant programming language for performance-critical
software. However, it is struggling to meet the needs of modern
developers, in no small part due to the accumulation of technical debt
over decades. Improvement of C++ is extremely difficult [1]_, not just
because of decades of technical debt, but also due to challenges in its
evolution process. The best way to address these problems is by avoiding
inheriting the legacy of C and C++, and instead start with solid
language foundations.

There are many modern programming languages that provide an excellent
developer experience - **Go** and **Kotlin** are examples, among many
others. Developers that *can* use them *should* use them. However, the
design of these languages presents significant barriers to migrating
from C++. These barriers range from the design of the language, to
performance overheads.

Exeme aims to be what **Kotlin** was for **Java** - a programming
language that has a gentle learning curve for C++ developers, but
provides tools to accelerate and safen the devlopment process.

*******
 Goals
*******

Exeme is designed to have:

-  Performance parity with C++,
-  A simple but powerful syntax; high quality code merits easy
   development,
-  Built-in practical code security and testing mechanisms,
-  A fast and scalable development process, and
-  Support for all modern OS platforms, and OS-independent APIs.

While many programming languages share subsets of these goals, their
combination is what sets Exeme apart. There are also explicit
*non-goals* for Exeme, the most notable being perfect backwards /
forwards compatability.

.. note::

   The :doc:`goals <project/goals>` document provides a deeper view into
   the language's goals.

****************
 Project status
****************

Exeme is **not** ready for use - it is a work in progress, and there is
no working compiler / toolchain **yet**. If you're interested in
contributing, we would love help with completing the compiler.

.. note::

   You can see the :doc:`roadmap <project/roadmap>` for more details on
   the expected progress of the Exeme programming language.

*********
 Join us
*********

Exeme is committed to maintaining a welcoming and inclusive environment
where everyone can contribute. For more information about the Exeme
development community, see our :doc:`code of conduct
<contributing/code_of_conduct>` and :doc:`contributing guidelines
<contributing/contributing>`.

----

.. [1]

   The :doc:`Issues with improving C++ <project/cpp_improvement_issues>`
   document outlines the reasons we believe improvement of C++ to be a
   challenge.

.. toctree::
   :hidden:
   :glob:

   self
   *
   contributing/index.rst
   design/index.rst
   project/index.rst
   setup/index.rst
