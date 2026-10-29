# RSA Playground

## About

This is a simple library (`lib_rsa`) containing functionality relevant to RSA such as

- the Miller-Rabin primality test
- a prime number generator (using Miller-Rabin)
- the Square and Multiply algorithm for fast modular exponentiation
- an RSA encryptor to handle encryption and
- an RSA decryptor to handle decryption
- a simple version of Fermat factorization
- Pollard's p-1 factorization algorithm

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
> Its RSA implementation does NOT provide padding or authenticated encryption as of right now.

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

## Build

Before being able to build you will need to install the following dependencies:

- Git is a version control software system mostly used for collaborative coding.

- CMake is a build system generator that configures the project, fetches dependencies, and builds the targets automatically for you.
For this project you will require CMake 3.20 or newer.

- Clang (Apple Clang or LLVM Clang) or GCC is the compiler currently recommended for this project, as it uses the `__int128` extension for modular arithmetic.
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

macOS includes Apple's Clang compiler and Git out of the box via Xcode Command Line Tools. Apple Clang natively supports the `unsigned __int128` extension used by this project, so **you do not need to install LLVM**.

First, install Apple's command-line tools if they are not already installed:

```sh
xcode-select --install
```

Next, you will need **CMake** (and optionally **Ninja** for faster parallel builds). Choose the method that best matches your system:

> [!WARNING]
> **For Intel Macs (macOS 14 Sonoma or older):**
> Homebrew has moved Intel macOS to Tier 3 (unsupported) and discontinued pre-compiled binary bottles for Intel chips. Do **not** run `brew install llvm`, as Homebrew will attempt to compile LLVM from source (which can take 6+ hours and usually fails). If `brew install cmake ninja` gives bottle errors, use **Option A (pip)** or **Option C (official installer)** below.

##### Option A: Using `pip` (Fastest, zero compilation — recommended for Intel Macs)

If Python is available on your machine, pre-compiled wheels install in seconds without any compilation:

```sh
pip3 install cmake ninja
```

> [!IMPORTANT]
> **Fixing "command not found: cmake" after pip install:**
> On macOS, `pip` installs executable binaries into Python's user bin directory (e.g. `~/Library/Python/3.x/bin`), which is **not** on `$PATH` by default.
>
> Add it to your `PATH` by running:
> ```sh
> echo 'export PATH="$(python3 -m site --user-base)/bin:$PATH"' >> ~/.zshrc
> source ~/.zshrc
> ```
> *(Or if you use bash instead of zsh: `echo 'export PATH="$(python3 -m site --user-base)/bin:$PATH"' >> ~/.bash_profile && source ~/.bash_profile`)*
>
> **Alternative (Zero PATH setup):**
> You can also run CMake and Ninja directly as Python modules without modifying your `PATH`:
> ```sh
> python3 -m cmake --version
> python3 -m cmake -S . -B build -G Ninja
> python3 -m cmake --build build
> ```

##### Option B: Using Homebrew (Apple Silicon or Intel when bottles are available)

```sh
brew install cmake ninja
```

##### Option C: Official Installer (No package manager needed)

1. Download CMake for macOS (`.dmg`) from [cmake.org/download](https://cmake.org/download/).
2. Open the DMG and drag `CMake.app` to `/Applications`.
3. Enable command-line access via:
   ```sh
   sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install
   ```
*(Ninja is optional; if not installed, CMake will automatically use Apple's built-in `make` tool.)*

##### Option D: Using MacPorts (Alternative package manager for Intel Macs)

If you use MacPorts, pre-built binary packages for macOS 14 Intel are maintained:

```sh
sudo port install cmake ninja
```

Verify that the tools are available:

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
> Run the following within the project's root directory.

If you have Ninja installed:

```sh
cmake -S . -B build -G Ninja
cmake --build build
```

Or without Ninja (using macOS's built-in `make`):

```sh
cmake -S . -B build
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

Check that the installed CMake version is at least 3.20 and that all tools are
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

```sh
./build/bin/bin_playground
```

## Testing

Run the registered tests from the repository root with:

```sh
ctest --test-dir build --output-on-failure
```

You can also add your own tests by adding them either in the existing "*_test.cpp"s in '/tests' or by adding your own 'cpp' files containing tests.
You can use the following skeleton to add tests:

```C++
#include <gtest/gtest.h>

// This checks whether the expected output matches the result
TEST(TestSuit, TestName) {
    const auto expected{val1, val2, ...};
    const auto actual = function_to_test(in1, in2, ...);

    EXPECT_EQ(expected, actual);
}

// This checks for expected errors and compares error messages
TEST(TestSuite, OtherTestName) {
    EXPECT_THROW({
        try {
            function_to_throw(invalid_argument);
        } catch(const std::invalid_argument& e) {
            // This tests that the exception has the correct message
            EXPECT_STREQ("Passed in invalid argument", e.what());
            throw;
        }
    }, std::invalid_argument);
}
```

## Usage

Start the executable from the repository root using the platform-specific command
listed below. Generate a key pair first, then enter a message in the Encrypt
panel and select **Encrypt**. In the Decrypt panel, enable **Use Encrypted** to
copy the generated cipher blocks into its input field, then select **Decrypt**.
The primality-check panel accepts an integer and reports whether it is composite
or probably prime.

## Additional info

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

- Integrate Indistinguishability Game into the TUI
- Add logging feature to the TUI / library
- Write tests for 'prime_generator.cpp'
