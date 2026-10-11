# Bachelor Thesis - RSA Implementation

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

TODO: Usage der Executable

## Build

### 1. Install build dependencies

#### CMake

To build this project, you will need at least CMake version 3.14.
CMake is a powerful build system generator that will compile and build this project for you.
If you don't have CMake installed or need to update your installation you can download it from
[https://cmake.org/](https://cmake.org/).

#### Clang

Since this project requires a special compiler extension for some modular 128-bit calculations as of right now,
it is recommended to use the Clang compiler from the LLVM project.
You can download Clang from TODO.

#### Ninja

We recommend [Ninja](https://ninja-build.org/) as your build system.
Follow the link and download the binary or simply run one of the following commands within your terminal:

`TODO` on Windows

`TODO` on MacOS

`TODO` on Ubuntu / Debian

### 2. Clone this repository

If you have git installed, simply clone this repository into a directory of your choice with

`git clone https://github.com/nicolatesula/thesis.git` # TODO fix URL

Else you can press the green `Code` Button, download the project ZIP and unpack it in a directory of your choice.

Next, navigate to the project's root directory within a terminal. If you just used the git command, you can simply use

`cd thesis`

### 3. Build the library and executable

First, create a `build` directory by executing `mkdir build` on Windows or TODO on Linux / Mac.

Enter the directory with `cd build` and run `cmake ..` to configure the build.

Finally, build both the library as well as the executable with `cmake --build .`.

### 4. Run the executable

The executable will be located in TODO

## Stuff

In src/lib_rsa/square_and_multiply.cpp we calculate:

```C++
unsigned __int128 product = static_cast<unsigned __int128>(a) * b;
return static_cast<std::uint64_t>(product % mod);
```

Since x86_64 CPUs do not come with an integrated 128 bit modulo operation,
the Clang compiler calls a compiler intern helper function called
`__umodti3` (Unsigned Modulo Tetra-Integer).

The linker needs to be passed this function, which is contained in `clang_rt.builtins-x86_64.lib`.
TODO: explain further.

## TODOs

- Indistinguishability Game
- Factorization attacks on weak prime numbers
- General Factorization algorithms (incl. Shor's algorithm? would need Qiskit maybe)

## Credits

Directory structure inspired by [Coding with Mat](https://www.youtube.com/watch?v=mdkVnmj32ZQ)
