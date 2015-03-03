/****************************************************************************
** GeomIO Library
** Copyright FougSys (2 Mar. 2015)
** contact@fougsys.fr
**
** This software is a reusable library whose purpose is to provide complete
** I/O support for various CAD file formats (eg. STL)
**
** This software is governed by the CeCILL-B license under French law and
** abiding by the rules of distribution of free software.  You can  use,
** modify and/ or redistribute the software under the terms of the CeCILL-B
** license as circulated by CEA, CNRS and INRIA at the following URL
** "http://www.cecill.info".
****************************************************************************/

#include "stl_format.h"

#include "stl_triangle.h"

#include "../gmio_core/endian.h"
#include "../gmio_core/internal/byte_codec.h"
#include "../gmio_core/internal/byte_swap.h"

#include <ctype.h>
#include <string.h>

enum { _INTERNAL_GMIO_FIXED_BUFFER_SIZE = 512 };

gmio_stl_format_t gmio_stl_get_format(gmio_stream_t *stream, size_t data_size)
{
    char fixed_buffer[_INTERNAL_GMIO_FIXED_BUFFER_SIZE];
    size_t read_size = 0;

    if (stream == NULL || data_size == 0)
        return GMIO_STL_UNKNOWN_FORMAT;

    /* Read a chunk of bytes from stream, then try to find format from that */
    memset(fixed_buffer, 0, _INTERNAL_GMIO_FIXED_BUFFER_SIZE);
    read_size = gmio_stream_read(stream, &fixed_buffer, 1, _INTERNAL_GMIO_FIXED_BUFFER_SIZE);
    read_size = read_size < _INTERNAL_GMIO_FIXED_BUFFER_SIZE ? read_size :
                                                               _INTERNAL_GMIO_FIXED_BUFFER_SIZE;

    /* Binary STL ? */
    if (read_size >= (GMIO_STLB_HEADER_SIZE + 4)) {
        /* Try with little-endian format */
        uint32_t facet_count = gmio_decode_uint32_le((const uint8_t*)fixed_buffer + 80);

        if ((GMIO_STLB_HEADER_SIZE + 4 + facet_count*GMIO_STLB_TRIANGLE_RAWSIZE) == data_size)
            return GMIO_STL_BINARY_LE_FORMAT;

        /* Try with byte-reverted facet count */
        facet_count = gmio_uint32_bswap(facet_count);
        if ((GMIO_STLB_HEADER_SIZE + 4 + facet_count*GMIO_STLB_TRIANGLE_RAWSIZE) == data_size)
            return GMIO_STL_BINARY_BE_FORMAT;
    }

    /* ASCII STL ? */
    {
        /* Skip spaces at beginning */
        size_t pos = 0;
        while (pos < read_size && isspace(fixed_buffer[pos]))
            ++pos;

        /* Next token (if exists) must match "solid " */
        if (pos < _INTERNAL_GMIO_FIXED_BUFFER_SIZE
                && strncmp(fixed_buffer + pos, "solid ", 6) == 0)
        {
            return GMIO_STL_ASCII_FORMAT;
        }
    }

    /* Fallback case */
    return GMIO_STL_UNKNOWN_FORMAT;
}
