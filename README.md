# RSA Playground

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [About](#about)
- [Build](#build)
  - [Windows (native)](#windows-native)
    - [1. Install dependencies](#1-install-dependencies)
    - [2. Clone this repository](#2-clone-this-repository)
    - [3. Build the library and executable](#3-build-the-library-and-executable)
  - [macOS](#macos)
    - [1. Install dependencies](#1-install-dependencies-1)
    - [2. Clone this repository](#2-clone-this-repository-1)
    - [3. Build the library and executable](#3-build-the-library-and-executable-1)
  - [Ubuntu / Debian / Windows (WSL)](#ubuntu--debian--windows-wsl)
    - [1. Install dependencies](#1-install-dependencies-2)
    - [2. Clone this repository](#2-clone-this-repository-2)
    - [3. Build the library and executable](#3-build-the-library-and-executable-2)
- [Run](#run)
- [Testing](#testing)
- [Usage](#usage)
- [Stuff](#stuff)
- [TODOs](#todos)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## About

This is a simple library `lib_rsa` containing functionality relevant to RSA such as

- the Miller-Rabin primality test
- a prime number generator (using Miller-Rabin)
- the Square and Multiply algorithm for fast modular exponentiation
- an RSA encryptor to handle encryption and
- an RSA decryptor to handle decryption

The en- and decryption follows this pipeline:

1. Pack the message string data into a vector of numbers smaller than the public module
2. Encrypt each vector element to receive a vector of ciphers
3. Decrypt the vector of ciphers back into a vector of numbers
4. Unpack the vector of numbers into a message string

This project also includes a sample executable `bin_playground`, showcasing the library's functionality.
`bin_playground` is a simple TUI using the [FTXUI](https://github.com/ArthurSonzogni/FTXUI) library to render to the terminal.
To see how to use it, please see the [Usage](#usage) section.

> [!NOTE]
> This is an educational playground, not production-ready cryptography.
> Its RSA implementation does not provide padding or authenticated encryption as of right now.

## Build

Before being able to build you will need to install the following dependencies:

- Git is a version control software system mostly used for collaborative coding.

- CMake is a build system generator that configures the project, fetches dependencies, and builds the targets automatically for you.
For this project you will require CMake 4.4 or newer.

- LLVM's Clang is the compiler currently recommended for this project, as it uses Clang's `__int128` extension for modular arithmetic.
A MSVC build will not successfully compile.

- Ninja is a build system specifically designed with CMake and speed in mind.

> [!IMPORTANT]
> If you decide to install these with dedicated installer programs instead of the instructions below, make sure to select `ADD TO PATH` or similar to guarantee that they are available system-wide.
> The build process will not succeed otherwise.

### Windows (native)

#### 1. Install dependencies

First, if you are on Windows and do not already have Visual Studio setup with the necessary C++ development tools, install the [Build Tools for Visual Studio](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2026).
When being prompted, install the `Desktop development for C++` workload.

Afterwards open a Windows PowerShell and install the following packages:

```powershell
winget install --id Git.Git -e
winget install --id Kitware.CMake -e
winget install --id LLVM.LLVM -e
winget install --id Ninja-build.Ninja -e
```

After installation is complete, reopen PowerShell and verify the installations:

```powershell
git --version
cmake --version
ninja --version
."C:\Program Files\LLVM\bin\clang-cl.exe" --version
```

#### 2. Clone this repository

Clone the GitHub repository into a directory of your choice:

```sh
git clone https://github.com/NicolaTesula/rsa-playground.git
```

and then enter it with

```sh
cd rsa-playground
```

#### 3. Build the library and executable

> [!NOTE]
> Run the following within the project's root directory.

On Windows, configure the build with the MSVC-compatible Clang driver:

```powershell
cmake -S . -B build -G Ninja `
  -DCMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang-cl.exe" `
  -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang-cl.exe" `
  -DCMAKE_RC_COMPILER="C:/Program Files/LLVM/bin/llvm-rc.exe"
```

Then build the library, tests and executable with

```sh
cmake --build build
```

---

### macOS

#### 1. Install dependencies

> [!NOTE]
> These instructions are AI generated and have not yet been tested.
> Please follow them with caution.

Install Apple's command-line tools and Homebrew if they are not already installed.
Follow Homebrew's official instructions at [brew.sh](https://brew.sh/), then install the build tools:

```sh
xcode-select --install
brew install git cmake ninja llvm
```

Homebrew does not always put its LLVM binaries directly on `PATH`.
Use Homebrew's reported prefix to select its Clang explicitly:

```sh
LLVM_PREFIX="$(brew --prefix llvm)"
export PATH="$LLVM_PREFIX/bin:$PATH"
git --version
cmake --version
ninja --version
clang++ --version
```

#### 2. Clone this repository

Clone the GitHub repository into a directory of your choice:

```sh
git clone https://github.com/NicolaTesula/rsa-playground.git
```

and then enter it with

```sh
cd rsa-playground
```

#### 3. Build the library and executable

> [!NOTE]
> Run the following within the project's root directory.

Select Homebrew's LLVM compiler explicitly if it is not already on `PATH`:

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_CXX_COMPILER="$(brew --prefix llvm)/bin/clang++"
```

Then build the library, tests and executable with

```sh
cmake --build build
```

---

### Ubuntu / Debian / Windows (WSL)

#### 1. Install dependencies

Open your terminal and run

```sh
sudo apt update
sudo apt install git cmake ninja-build clang
```

Check that the installed CMake version is at least 4.4 and that all tools are
available:

```sh
git --version
cmake --version
ninja --version
clang++ --version
```

#### 2. Clone this repository

Clone the GitHub repository into a directory of your choice:

```sh
git clone https://github.com/NicolaTesula/rsa-playground.git
```

and then enter it with

```sh
cd rsa-playground
```

#### 3. Build the library and executable

> [!NOTE]
> Run the following from the project's root directory.

Configure the project with Ninja and Clang:

```sh
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++
```

Then build the library, tests and executable with

```sh
cmake --build build
```

## Run

The executable is located in `build/bin`. To execute it, run

```powershell
.\build\bin\bin_playground.exe
```

on Windows or

```sh
./build/bin/bin_playground
```

on macOS and Ubuntu / Debian.

## Testing

TODO: How to add tests

Run the registered tests from the repository root with:

```sh
ctest --test-dir build --output-on-failure
```

## Usage

Start the executable from the repository root using the platform-specific command
listed below. Generate a key pair first, then enter a message in the Encrypt
panel and select **Encrypt**. In the Decrypt panel, enable **Use Encrypted** to
copy the generated cipher blocks into its input field, then select **Decrypt**.
The primality-check panel accepts an integer and reports whether it is composite
or probably prime.

## Stuff

In src/lib_rsa/square_and_multiply.cpp we calculate:

```C++
unsigned __int128 product = static_cast<unsigned __int128>(a) * b;
return static_cast<std::uint64_t>(product % mod);
```

Since x86_64 CPUs do not come with an integrated 128 bit modulo operation,
the Clang compiler calls a compiler intern helper function called
`__umodti3` (unsigned modulo of a 128-bit integer). This helper is part of
compiler-rt, Clang's compiler runtime library. On Windows x64, Clang's
`clang_rt.builtins-x86_64.lib` provides the helper; the CMake configuration
locates Clang's resource directory and links that library to `lib_rsa`.

This explicit library path is specific to the Windows x64 LLVM distribution.
On macOS and Linux, the compiler driver normally links the appropriate runtime
for its platform, so CMake does not add the Windows `.lib` there. The use of
`unsigned __int128` is a compiler extension rather than a portable C++ type.

## TODOs

- Indistinguishability Game
- Factorization attacks on weak prime numbers
- General Factorization algorithms (incl. Shor's algorithm? would need Qiskit maybe)
