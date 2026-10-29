/*
 * Copyright LWJGL. All rights reserved.
 * License terms: https://www.lwjgl.org/license
 */
#include "common_tools.h"
#include <ffi.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

/* The generic bare-metal libffi allocator uses malloc, whose pages are not
 * executable on Scarlet. Allocate closures from anonymous executable mappings.
 * Each closure owns its mapping; no file-backed alias or DSO TLS is required. */
typedef union ClosureMapping {
    size_t length;
    max_align_t alignment;
} ClosureMapping;

void *ffi_closure_alloc(size_t size, void **code) {
    if (code == NULL || size > SIZE_MAX - sizeof(ClosureMapping)) {
        errno = EINVAL;
        return NULL;
    }
    long page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0) {
        return NULL;
    }
    size_t length = size + sizeof(ClosureMapping);
    size_t page = (size_t)page_size;
    if (length > SIZE_MAX - (page - 1)) {
        errno = ENOMEM;
        return NULL;
    }
    length = (length + page - 1) / page * page;
    ClosureMapping *mapping = mmap(NULL, length, PROT_READ | PROT_WRITE | PROT_EXEC,
                                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mapping == MAP_FAILED) {
        return NULL;
    }
    mapping->length = length;
    *code = mapping + 1;
    return mapping + 1;
}

void ffi_closure_free(void *pointer) {
    if (pointer != NULL) {
        ClosureMapping *mapping = (ClosureMapping *)pointer - 1;
        munmap(mapping, mapping->length);
    }
}

void *ffi_data_to_code_pointer(void *pointer) {
    return pointer;
}

int ffi_tramp_is_present(void *pointer) {
    UNUSED_PARAM(pointer)
    return 0; /* This allocator does not use libffi's static trampoline pool. */
}
