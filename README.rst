.. raw:: html

   <!-- Part of the Exeme language project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

############################
 Exeme Programming Language
############################

|Licensed under the MIT License| |PR's Welcome| |Website Status|

An introduction to Exeme can be found on the `website
<https://exeme-project.github.io/exeme-lang/>`__.

**************
 Example Code
**************

This is a Prime Number checker written in Exeme. This code is analyzed
in the `Design Specification
<https://exeme-project.github.io/exeme-lang/design/index.html#the-basics>`_.

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

.. |Licensed under the MIT License| image:: https://img.shields.io/badge/License-MIT-blue.svg
   :target: https://github.com/exeme-project/exeme-lang/blob/master/LICENSE

.. |PR's Welcome| image:: https://img.shields.io/badge/PRs%20-welcome-brightgreen.svg
   :target: https://github.com/exeme-project/exeme-lang/pulls

.. |Website Status| image:: https://img.shields.io/website?down_message=offline&up_message=online&url=https%3A%2F%2Fexeme-project.github.io%2Fexeme-lang
   :target: https://exeme-project.github.io/exeme-lang
