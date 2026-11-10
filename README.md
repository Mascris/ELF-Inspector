# ELF-Inspector

A small command-line tool written in C that reads the header of an ELF file and prints basic information about it.

## Features

- Verifies the file is a valid ELF (checks the magic number)
- Prints the entry point address
- Prints the magic number bytes
- Detects the class (32-bit or 64-bit)
- Identifies the target machine (x86-64, Intel 80386, ARM AArch64)
- Still adding and LEARNING

## Build

```bash
gcc -o main main.c
```

Replace `main.c` with the name of your source file.

## Usage

```bash
./main <elf-file>
```

