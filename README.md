# Bachelor Thesis

## About

The en- and decryption follows this pipeline:

1. Pack the message string data into a vector of numbers smaller than the public module
2. Encrypt each vector entry and return a vector of ciphers
3. Decrypt the vector of ciphers into a vector of numbers
4. Unpack the vector of numbers into a message string

## Build

### Windows

TODO

### MacOS

Instructions have only been tested for ...
TODO

### Linux

Instructions have only been tested for Ubuntu.
TODO

## Stuff

Was ist __umodti3? In src/lib_rsa/square_and_multiply.cpp berechnest du:

```C++
unsigned __int128 product = static_cast<unsigned __int128>(a) * b;
return static_cast<std::uint64_t>(product % mod);
```

Da x86_64-CPUs keinen Maschinenbefehl für eine 128-Bit-Modulo-Operation besitzen, generiert der Clang-Compiler einen Aufruf an eine Compiler-interne Hilfsfunktion namens __umodti3 (Unsigned Modulo Tetra-Integer).

Warum konnte der Linker sie nicht finden? Unter Windows ruft CMake Clang standardmäßig mit -nostdlib auf und bindet nur die MSVC-Standardbibliotheken ein. Die Clang-eigene Runtime-Bibliothek (clang_rt.builtins-x86_64.lib), die __umodti3 bereitstellt, wurde daher nicht automatisch mitgelinkt.

## TODO

- Primality Test
- Decrypt
- Indistinguishability Game
- Info Bar unten für Public Key, Private Key, TODO was noch?
- Build Anleitungen schreiben
- Herausfinden, ob man zu Clipboard kopieren bzw von Clipboard pasten kann

## Credits

Directory structure inspired by [Coding with Mat](https://www.youtube.com/watch?v=mdkVnmj32ZQ)
