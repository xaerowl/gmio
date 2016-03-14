/****************************************************************************
** gmio
** Copyright Fougue (2 Mar. 2015)
** contact@fougue.pro
**
** This software is a reusable library whose purpose is to provide complete
** I/O support for various CAD file formats (eg. STL)
**
** This software is governed by the CeCILL-B license under French law and
** abiding by the rules of distribution of free software.  You can  use,
** modify and/ or redistribute the software under the terms of the CeCILL-B
** license as circulated by CEA, CNRS and INRIA at the following URL
** "http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html".
****************************************************************************/

#include "utest_assert.h"

#include "../src/gmio_core/internal/numeric_utils.h"
#include "../src/gmio_stl/stl_constants.h"
#include "../src/gmio_stl/stl_triangle.h"

#include <stddef.h>
#include <string.h>

/* Disable MSVC warning "conditional expression is constant" */
GMIO_PRAGMA_MSVC_WARNING_PUSH_AND_DISABLE(4127)

const char* test_stl_coords_packing()
{
    UTEST_COMPARE_UINT(0, offsetof(struct gmio_vec3f, x));
    UTEST_COMPARE_UINT(4, offsetof(struct gmio_vec3f, y));
    UTEST_COMPARE_UINT(8, offsetof(struct gmio_vec3f, z));
    UTEST_COMPARE_UINT(GMIO_STL_COORDS_RAWSIZE, sizeof(struct gmio_vec3f));
    return NULL;
}

const char* test_stl_triangle_packing()
{
    UTEST_COMPARE_UINT(0, offsetof(struct gmio_stl_triangle, n));
    UTEST_COMPARE_UINT(GMIO_STL_COORDS_RAWSIZE, offsetof(struct gmio_stl_triangle, v1));
    UTEST_COMPARE_UINT(2*GMIO_STL_COORDS_RAWSIZE, offsetof(struct gmio_stl_triangle, v2));
    UTEST_COMPARE_UINT(3*GMIO_STL_COORDS_RAWSIZE, offsetof(struct gmio_stl_triangle, v3));
    UTEST_COMPARE_UINT(4*GMIO_STL_COORDS_RAWSIZE, offsetof(struct gmio_stl_triangle, attribute_byte_count));
    UTEST_ASSERT(sizeof(struct gmio_stl_triangle) >= GMIO_STLB_TRIANGLE_RAWSIZE);
    return NULL;
}

const char* test_stl_triangle_compute_normal()
{
    const unsigned udiff = 5;
    { /* Doesn't fail on invalid facet */
        struct gmio_stl_triangle tri = {0};
        gmio_stl_triangle_compute_normal(&tri);
        UTEST_ASSERT(gmio_float32_ulp_equals(tri.n.x, 0.f, udiff));
        UTEST_ASSERT(gmio_float32_ulp_equals(tri.n.y, 0.f, udiff));
        UTEST_ASSERT(gmio_float32_ulp_equals(tri.n.z, 0.f, udiff));
    }
    return NULL;
}

GMIO_PRAGMA_MSVC_WARNING_POP()
