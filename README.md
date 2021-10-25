Kazama OS is a UNIX-like 64-bit operating system written in C.

## About Kazama OS
Kazama OS includes its own modular kernel with SMP and networking, and collection of software ports.

If you have any questions or concerns feel free to open a GitHub issue or email me at novabolt.kaza@gmail.com.


## Features

- Modular Kernel
- Symmetric Multiprocessing (SMP)
- Terminal Emulator

## Work In Progress
- UNIX/BSD Sockets
- Network Stack (UDP, TCP, DHCP)
- A small HTTP client/downloader called steal
- Writable Ext2 Filesystem
- IDE, AHCI and NVMe Driver
- Dynamic Linking
- mlibc C Library Port
- LLVM/Clang Port
- DOOM Port
- XHCI Driver

## Third Party

Kazama OS depends on mlibc, Freetype, zlib, libressl and libpng.

Optional ports include LLVM/Clang, DOOM, Binutils and Python 3.8/3.9


## System requirements

- 256 MB RAM (512 is more optimal)
- x86_64 Processor supporting [x86_64-v2 instructions](https://en.wikipedia.org/wiki/X86-64#Microarchitecture_levels) including SSE4.2
    - For QEMU/KVM use `-cpu host` or at least `-cpu Nehalem` see [this page](https://qemu-project.gitlab.io/qemu/system/target-i386.html)
- 2 or more CPU cores recommended
- I/O APIC
- ATA, NVMe or AHCI disk (AHCI *strongly* recommended)

## Compiling
- 'make' to compile the Kernel

## Note

- Dont use directly in Hardware Device , Test with QEMU or VM Ware / VirtualBox
