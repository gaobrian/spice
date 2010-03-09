/* -*- Mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
   Copyright (C) 2010 Red Hat, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mem.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef MALLOC_ERROR
#define MALLOC_ERROR(format, ...) {                             \
    printf(format "\n", ## __VA_ARGS__);   \
    abort();                                                    \
}
#endif

char *spice_strdup(const char *str)
{
    char *copy;

    copy = (char *)spice_malloc(strlen(str) + 1);
    strcpy(copy, str);
    return copy;
}

void *spice_memdup(const void *mem, size_t n_bytes)
{
    void *copy;

    copy = spice_malloc(n_bytes);
    memcpy(copy, mem, n_bytes);
    return copy;
}

void *spice_malloc(size_t n_bytes)
{
    void *mem;

    if (SPICE_LIKELY(n_bytes)) {
        mem = malloc(n_bytes);

        if (SPICE_LIKELY(mem != NULL)) {
            return mem;
        }

        MALLOC_ERROR("spice_malloc: panic: unable to allocate %lu bytes\n",
                     (unsigned long)n_bytes);
    }
    return NULL;
}

void *spice_malloc0(size_t n_bytes)
{
    void *mem;

    if (SPICE_LIKELY(n_bytes)) {
        mem = calloc(1, n_bytes);

        if (SPICE_LIKELY(mem != NULL)) {
            return mem;
        }

        MALLOC_ERROR("spice_malloc0: panic: unable to allocate %lu bytes\n",
                     (unsigned long)n_bytes);
    }
    return NULL;
}

void *spice_realloc(void *mem, size_t n_bytes)
{
    if (SPICE_LIKELY(n_bytes)) {
        mem = realloc(mem, n_bytes);

        if (SPICE_LIKELY(mem != NULL)) {
            return mem;
        }

        MALLOC_ERROR("spice_realloc: panic: unable to allocate %lu bytes\n",
                     (unsigned long)n_bytes);
    }

    if (mem) {
        free(mem);
    }

    return NULL;
}

#define SIZE_OVERFLOWS(a,b) (SPICE_UNLIKELY ((a) > SIZE_MAX / (b)))

void *spice_malloc_n(size_t n_blocks, size_t n_block_bytes)
{
    if (SIZE_OVERFLOWS (n_blocks, n_block_bytes)) {
        MALLOC_ERROR("spice_malloc_n: overflow allocating %lu*%lu bytes",
                     (unsigned long)n_blocks, (unsigned long)n_block_bytes);
    }

    return spice_malloc(n_blocks * n_block_bytes);
}

void *spice_malloc_n_m(size_t n_blocks, size_t n_block_bytes, size_t extra_size)
{
    size_t size1, size2;
    if (SIZE_OVERFLOWS (n_blocks, n_block_bytes)) {
        MALLOC_ERROR("spice_malloc_n: overflow allocating %lu*%lu + %lubytes",
                     (unsigned long)n_blocks, (unsigned long)n_block_bytes, (unsigned long)extra_size);
    }
    size1 = n_blocks * n_block_bytes;
    size2 = size1 + extra_size;
    if (size2 < size1) {
        MALLOC_ERROR("spice_malloc_n: overflow allocating %lu*%lu + %lubytes",
                     (unsigned long)n_blocks, (unsigned long)n_block_bytes, (unsigned long)extra_size);
    }
    return spice_malloc(size2);
}


void *spice_malloc0_n(size_t n_blocks, size_t n_block_bytes)
{
    if (SIZE_OVERFLOWS (n_blocks, n_block_bytes)) {
        MALLOC_ERROR("spice_malloc0_n: overflow allocating %lu*%lu bytes",
                     (unsigned long)n_blocks, (unsigned long)n_block_bytes);
    }

    return spice_malloc0 (n_blocks * n_block_bytes);
}

void *spice_realloc_n(void *mem, size_t n_blocks, size_t n_block_bytes)
{
    if (SIZE_OVERFLOWS (n_blocks, n_block_bytes)) {
        MALLOC_ERROR("spice_realloc_n: overflow allocating %lu*%lu bytes",
                     (unsigned long)n_blocks, (unsigned long)n_block_bytes);
  }

    return spice_realloc(mem, n_blocks * n_block_bytes);
}
