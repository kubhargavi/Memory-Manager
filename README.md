# Memory-Manager

This project implements a dynamic memory management system with bitmap-based allocation. It provides efficient allocation and deallocation of memory slots, ensuring proper alignment and expandability.

Features:
- Memory allocation (mem_mngr_alloc): Allocates memory slots based on a first fit policy.
- Memory deallocation (mem_mngr_free): Frees allocated memory and prevents double freeing.
- Memory initialization (mem_mngr_init): Initializes the memory manager with default settings.
- Memory cleanup (mem_mngr_leave): Releases all allocated memory on program termination.
- Bitmap-based tracking: Uses a dynamic bitmap for tracking memory slots efficiently.

Files:
- memory_manager.c: Core memory management implementation.
- memory_manager.h: Header file defining memory manager structures and functions.
- bitmap.c: Implements bitmap operations for tracking allocated/free slots.
- common.h: Common definitions used across the project.
- interposition.h: Handles function interposition (if applicable).
- Makefile: Build script to compile the project.

To compile the project, run:
make
This will generate the necessary executables.
