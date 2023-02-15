.. raw:: html

   <!-- Part of the Exeme language project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

###########################
 Issues with improving C++
###########################

C++ is the dominant programming language for performance critical
software. However, improving C++ to deliver an experience that
developers expect from a modern programming language is difficult, due
to the fact that it has **decades** of technical debt accumulated in its
design. C++ inherited the legacy of C; at the time this was essential to
its success, by giving it instant and high-quality access to a large C
ecosystem. However over time this has led to significant technical debt,
ranging from `confusing integer promotion rules
<https://shafik.github.io/c++/2021/12/30/usual_arithmetic_confusions.html>`_
to complex syntax with `the most vexing parse
<https://en.wikipedia.org/wiki/Most_vexing_parse>`_.

C++ also prioritizes backwards compatibility, in both its syntax and
`ABI <https://en.wikipedia.org/wiki/Application_binary_interface>`_.
This preserves its access to *existing* C and C++ ecosystems, and forms
one of the foundations of common Linux package management approaches.
However, this has resulted in features being overwhelmingly added over
time, instead of changing or replacing language designs to simplify and
improve the language. This creates technical debt due to complicated
feature interaction, and means C++ fails to benefit from cleanup
opportunities in the form of replacing or removing legacy features.

Another challenge to improving C++ is the current evolution process and
direction. A key example of this is the C++ committee's struggle to
converge on a clear set of long-term goals and priorities. When pushed
to address the technical debt caused by not braking the ABI, **C++'s
process** `did not reach any definitive conclusion
<https://cor3ntin.github.io/posts/abi/#abi-discussions-in-prague>`_.
This both failed to change C++'s direction and priorities towards
improvements (rather than backwards compatibility), and demonstrates how
the process can be made to fail to allow biased decisions.

Beyond C++'s evolution direction, the mechanics of the process also make
improvement of C++ difficult. **C++'s process is oriented around
standardization rather than design**; it uses a multiyear waterfall
committee process. Access to the committee and standard is restricted
and expensive, attendance in-person is necessary to have a voice, and
decisions are made by live votes of *those present* (and only *those
present*). The committee structure is designed to ensure representations
of *nations and companies*, rather than building an inclusive team and
community of experts and contributors.
