# ELF-Inspector

A small elf viewer in C that reads the elf headers of an Elf file and prints informations about it.

## Features

- Verifies the file is a valid ELF (checks the magic number)
- Prints the entry point address
- Prints the magic number bytes
- Detects the class (32-bit or 64-bit)
- Identifies the target machine (x86-64, Intel 80386, ARM AArch64)
- Section Headers printing 
- Still adding and LEARNING

## Build

```bash
gcc -o main main.c
```

## Usage

```bash
./main <elf-file>
```

