# RSA Playground

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
`bin_playground` is a simple TUI using the FTXUI library to render to the terminal.
To see how to use it, please see the [Usage](#usage) section.

## Build

### 1. Install build dependencies

#### 1.1 The Dependencies

##### CMake

CMake is a build system generator that configures the project, fetches dependencies, and builds the targets automatically for you.
For this project you will need CMake 3.20 or newer.
If you don't have CMake installed or need to update it, download it from [cmake.org](https://cmake.org/) or use your platform's package manager.

##### Clang

You will need a compiler with C++23 support.
LLVM's Clang is recommended here, since this project currently uses Clang's `__int128` extension for modular arithmetic.
Donwload and install Clang from the [LLVM project](https://github.com/llvm/llvm-project/releases) or via your platform's package manager.

> [!NOTE]
> On Windows, use `clang-cl` with the MSVC C++ Build Tools and Windows SDK;
> the GNU-style `clang++` driver can select an incompatible CMake link mode for the MSVC ABI.

##### Ninja

We recommend Ninja as the build system.
Install it from
[ninja-build.org](https://ninja-build.org/) or with your platform's package manager.

> [!IMPORTANT]
> If you are using the official installers, make sure to select `ADD TO PATH` or similar to guarantee that it is found system-wide.
> The build process will not succeed otherwise.

#### 1.2 Package Manager Installation

##### Windows (PowerShell)

If `winget` is available, install the standalone tools:

```powershell
winget install --id Kitware.CMake -e
winget install --id LLVM.LLVM -e
winget install --id Ninja-build.Ninja -e
```

Install Visual Studio Build Tools from the official Visual Studio downloads page if they are not already present, and select the **Desktop development
with C++** workload in Visual Studio Installer. Open a new PowerShell window
after installations, then verify:

```powershell
cmake --version
clang-cl --version
ninja --version
```

If `clang-cl` is not found, add LLVM's default install directory
(`C:\Program Files\LLVM\bin`) to the Windows `Path` environment variable in
**Settings > System > About > Advanced system settings > Environment Variables**,
then open a new terminal and retry the checks.

##### macOS

Install Apple's command-line tools and Homebrew if they are not already installed.
Follow Homebrew's official instructions at [brew.sh](https://brew.sh/), then install the build tools:

```sh
xcode-select --install
brew install cmake ninja llvm
```

Homebrew does not always put its LLVM binaries directly on `PATH`. Use
Homebrew's reported prefix to select its Clang explicitly:

```sh
LLVM_PREFIX="$(brew --prefix llvm)"
export PATH="$LLVM_PREFIX/bin:$PATH"
cmake --version
clang++ --version
ninja --version
git --version
```

##### Ubuntu / Debian

Open your terminal and run

```sh
sudo apt update
sudo apt install cmake ninja-build clang
```

Check that the installed CMake version is at least 3.20 and that all tools are
available:

```sh
cmake --version
clang++ --version
ninja --version
```

### 2. Clone this repository

If you have [Git](https://git-scm.com/) installed, simply clone this GitHub repository into a directory of your choice:

```sh
git clone https://github.com/NicolaTesula/rsa-playground.git
```

and then enter the repository with

```sh
cd rsa-playground
```

If you do not have Git installed, use GitHub's **Code > Download ZIP** option instead, unpack it in a directory of your choice and navigate to the project's root directory in a terminal.

### 3. Build the library and executable

Run the following from the project's root directory.

#### Windows Build

On Windows, configure the build with the MSVC-compatible Clang driver:

```powershell
cmake -S . -B build-clangcl -G Ninja `
  -DCMAKE_C_COMPILER="C:\Program Files\LLVM\bin\clang-cl.exe" `
  -DCMAKE_CXX_COMPILER="C:\Program Files\LLVM\bin\clang-cl.exe"
```

> [!IMPORTANT]
> Run this from **Developer PowerShell for Visual Studio 2022** (or an x64 Visual Studio Developer Command Prompt) so the Windows SDK resource compiler and MSVC libraries are available.

Then build the library, tests and executable with

```sh
cmake --build build-clangcl
```

#### macOS Build

On macOS, select Homebrew's LLVM compiler explicitly if it is not already on `PATH`:

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_CXX_COMPILER="$(brew --prefix llvm)/bin/clang++"
```

Then build the library, tests and executable with

```sh
cmake --build build
```

#### Ubuntu / Debian Build

Configure the project with Ninja and Clang:

```sh
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++
```

Then build the library, tests and executable with

```sh
cmake --build build
```

### 4. Run the executable

The executable is written to `build/bin` on macOS/Linux and
`build-clangcl/bin` on Windows.

On Windows you can run it from the repository rootw with

```powershell
.\build-clangcl\bin\bin_playground.exe
```

On macOS or Linux, use:

```sh
./build/bin/bin_playground
```

### 5. Run tests (optional)

Run the registered tests from the repository root with:

```sh
ctest --test-dir build --output-on-failure
```

on macOS or Linux and with

```sh
ctest --test-dir build-clangcl --output-on-failure
```

on Windows.

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

## Usage

Start the executable from the repository root using the platform-specific command
listed below. Generate a key pair first, then enter a message in the Encrypt
panel and select **Encrypt**. In the Decrypt panel, enable **Use Encrypted** to
copy the generated cipher blocks into its input field, then select **Decrypt**.
The primality-check panel accepts an integer and reports whether it is composite
or probably prime.

This is an educational playground, not production-ready cryptography. Its RSA
implementation does not provide padding or authenticated encryption.

## TODOs

- Indistinguishability Game
- Factorization attacks on weak prime numbers
- General Factorization algorithms (incl. Shor's algorithm? would need Qiskit maybe)

## Credits

Directory structure inspired by [Coding with Mat](https://www.youtube.com/watch?v=mdkVnmj32ZQ)
