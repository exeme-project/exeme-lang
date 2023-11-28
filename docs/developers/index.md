<!-- Part of the Exeme Project, under the MIT license. See '/LICENSE' for license information. SPDX-License-Identifier: MIT License. -->

# 🛠️ Developer Documentation

- [🛠️ Developer Documentation](#️-developer-documentation)
  - [🖥️ Environment Setup](#️-environment-setup)
    - [Cloning the Repository](#cloning-the-repository)
    - [Setting up *pre-commit*](#setting-up-pre-commit)
  - [Making Changes](#making-changes)

## 🖥️ Environment Setup

::: info
GitHub Codespaces uses devcontainers to setup development environments. If you are using GitHub Codespaces, you can skip this section. This is because we have a devcontainer setup ready.
:::

Requirements to setup the development environment for The Exeme Language:

- [**CMake**](https://cmake.org/download/) - For building the project.
- [**Git**](https://git-scm.com/downloads) - For cloning the repository.
- [**LLVM**](https://llvm.org/) - For compiling the compiler.

### Cloning the Repository

To clone the repository, run the following command:

```bash
git clone https://github.com/revical/revical
cd revical
```

### Setting up *pre-commit*

To setup *pre-commit*, run the following command:

```bash
python -m pip -r requirements.txt && pre-commit install
```

::: tip
If the command fails, try replacing **`python`** with **`python3`**.
:::

## Making Changes