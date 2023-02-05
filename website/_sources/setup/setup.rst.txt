.. raw:: html

   <!-- Part of the Exeme language project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

#######
 Setup
#######

*********
 Windows
*********

-  Install `Visual Studio
   <https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSFeaturesPage&passive=true&tailored=cplus&cid=2031#cplusplus>`__
   with support for C++ development. If you used the above link then it
   should automatically choose the required options to install.

-  Install the latest version of `Clang
   <https://github.com/llvm/llvm-project/releases/latest>`__ from LLVM.
   Choose the binary ending with **``-win64.exe``** (unless your system
   is 32-Bit).

.. note::

   Make sure you select **Add to Path** when installing clang.

*******
 macOS
*******

-  Install the latest XCode (from the App Store or the `Xcode website
   <https://developer.apple.com/xcode/>`__).

-  Install XCode command-line tools ``xcode-select --install``.

-  Install `Homebrew <https://brew.sh/>`__.

-  Install LLVM through Homebrew with: ``brew install llvm``.

-  Run ``echo 'export PATH="/usr/local/opt/llvm/bin:$PATH"' >>
   ~/.zshrc_profile`` to add LLVM to your PATH.

-  Run ``source ~/.bash_profile`` or ``source ~/.zshrc`` to update your
   PATH variable in the current terminal session, depending on your
   shell.

.. note::

   On newer versions of macOS, some headers are not installed by
   default. Open
   ``Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_*.pkg``.

*************************
 GNU/Linux and other Nix
*************************

.. note::

   Make sure the LLVM binaries and the linker are added to your $PATH
   environmental variable.

Linux
=====

Install clang and llvm (using your distro's package manager).

FreeBSD
=======

For FreeBSD: ``pkg install bash git llvm``.
