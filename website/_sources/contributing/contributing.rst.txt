.. raw:: html

   <!-- Part of the Exeme language project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

##############
 Contributing
##############

*********
 Welcome
*********

Thank you for your interest in contributing to Exeme. There are many
different ways to contribute, and they are all appreciated. If you have
any questions, feel free to ask on the `GitHub
<https://github.com/exeme-project/exeme-lang/discussions>`_.

Everyone who contributes to Exeme is expected to:

-  Read the :doc:`Code of Conduct <code_of_conduct>`. We expect everyone
   in our community to be welcoming and friendly.

***********************
 Where to make changes
***********************

The core libraries and documentation are the areas that are in most need
of input. Any enhancements to parts of the compiler are welcome.

*************
 Adding docs
*************

If you add new functionality, or change the way current functionality
works, add or edit the docs to reflect this. This is so that users have
documentation of the new changes.

*******************
 Contributors file
*******************

Remember to add your name, GitHub proile, and email address to the
official :doc:`contributors file <contributors>`.

*******
 Style
*******

Code style and syntax rules
===========================

You can easily follow the same coding style and syntax rules by looking
at the Exeme source code. It is also recommended to install
clang-format, which can format your code using the `src/.clang-format
<https://github.com/exeme-project/exeme-lang/blob/main/src/.clang-format>`_
file.

*********
 License
*********

All license is required at the top of all non-trivial files.

reStructuredText
================

reStructuredText files always have at the top:

.. code::

   .. raw:: html

      <!-- Part of the Exeme language project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

For example, see the top of `this file's
<https://github.com/exeme-project/exeme-lang/blob/main/docs/contributing/contributing.rst>`_
raw contents.

C++ source code
===============

C++ source code files always have at the top:

.. code:: c++

   // Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
   // license information. SPDX-License-Identifier: MIT License.

For example, see the top of `src/main.cpp
<https://github.com/exeme-project/exeme-lang/blob/main/src/main.cpp>`_
file's contents.

Other files
===========

Every file type uses a variation of the above license text (**MIT
License**) with similar formatting.
