/****************************************************************************
** Copyright (c) 2017, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
**     1. Redistributions of source code must retain the above copyright
**        notice, this list of conditions and the following disclaimer.
**
**     2. Redistributions in binary form must reproduce the above
**        copyright notice, this list of conditions and the following
**        disclaimer in the documentation and/or other materials provided
**        with the distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
****************************************************************************/

#include "utest_assert.h"

#include "../src/gmio_core/memblock.h"
#include "../src/gmio_core/endian.h"
#include "../src/gmio_core/error.h"
#include "../src/gmio_core/stream.h"

#include <stdlib.h>
#include <string.h>

static struct gmio_memblock __tc__buffer_ctor()
{
    return gmio_memblock_calloc(4, 256);
}

static const char* test_core__buffer()
{
    /* gmio_memblock_calloc() */
    {
        const size_t obj_count = 4;
        const size_t obj_size = 256;
        const size_t buff_size = obj_count * obj_size;
        const uint8_t zero_buff[4 * 256] = {0};
        struct gmio_memblock buff = gmio_memblock_calloc(obj_count, obj_size);
        UTEST_ASSERT(buff.ptr != NULL);
        UTEST_ASSERT(buff.size == buff_size);
        UTEST_ASSERT(memcmp(buff.ptr, &zero_buff[0], buff_size) == 0);
        /* TODO: make assert succeed with mingw and gmio as a DLL
         *       In this case free() has not the same address in libgmio.dll and
         *       test_core.exe */
        UTEST_ASSERT(buff.func_deallocate == &free);
        gmio_memblock_deallocate(&buff);
    }
    /* gmio_memblock_malloc() */
    {
        const size_t buff_size = 2 * 1024; /* 2KB */
        struct gmio_memblock buff = gmio_memblock_malloc(buff_size);
        UTEST_ASSERT(buff.ptr != NULL);
        UTEST_ASSERT(buff.size == buff_size);
        UTEST_ASSERT(buff.func_deallocate == &free);
        gmio_memblock_deallocate(&buff);
    }
    /* gmio_memblock_realloc() */
    {
        const size_t buff_size = 1024; /* 1KB */
        struct gmio_memblock buff = gmio_memblock_malloc(buff_size);
        buff = gmio_memblock_realloc(buff.ptr, 2 * buff_size);
        UTEST_ASSERT(buff.ptr != NULL);
        UTEST_ASSERT(buff.size == (2 * buff_size));
        UTEST_ASSERT(buff.func_deallocate == &free);
        gmio_memblock_deallocate(&buff);
    }
    /* default ctor */
    {
        UTEST_ASSERT(gmio_memblock_default_constructor() != NULL);
        gmio_memblock_set_default_constructor(&__tc__buffer_ctor);
        UTEST_ASSERT(gmio_memblock_default_constructor() == &__tc__buffer_ctor);
    }

    return NULL;
}

static const char* test_core__endian()
{
    UTEST_ASSERT(gmio_host_endianness() == GMIO_ENDIANNESS_HOST);
GMIO_PRAGMA_MSVC_WARNING_PUSH_AND_DISABLE(4127)
    UTEST_ASSERT(GMIO_ENDIANNESS_LITTLE != GMIO_ENDIANNESS_BIG);
    UTEST_ASSERT(GMIO_ENDIANNESS_HOST == GMIO_ENDIANNESS_LITTLE
                 || GMIO_ENDIANNESS_HOST == GMIO_ENDIANNESS_BIG);
GMIO_PRAGMA_MSVC_WARNING_POP()
    return NULL;
}

static const char* test_core__error()
{
    UTEST_ASSERT(gmio_no_error(GMIO_ERROR_OK));
    UTEST_ASSERT(!gmio_error(GMIO_ERROR_OK));

    UTEST_ASSERT(!gmio_no_error(GMIO_ERROR_UNKNOWN));
    UTEST_ASSERT(gmio_error(GMIO_ERROR_UNKNOWN));

    return NULL;
}

static const char* test_core__stream()
{
    const struct gmio_stream null_stream = gmio_stream_null();
    const uint8_t null_bytes[sizeof(struct gmio_stream)] = {0};
    UTEST_ASSERT(memcmp(&null_stream, &null_bytes, sizeof(struct gmio_stream))
                 == 0);

    return NULL;
}
