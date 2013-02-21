#ifndef FOUG_C_LIBSTL_STLB_READ_H
#define FOUG_C_LIBSTL_STLB_READ_H

#include "stl_global.h"
#include "stlb_triangle.h"
#include "../stream.h"
#include "../task_control.h"

/* foug_stlb_geom_input : opaque structure */
typedef struct _internal_foug_stlb_geom_input foug_stlb_geom_input_t;

/* foug_stlb_geom_input_manip */
typedef struct foug_stlb_geom_input_manip
{
  void (*process_header_func)(foug_stlb_geom_input_t*, const uint8_t*);
  void (*begin_triangles_func)(foug_stlb_geom_input_t*, uint32_t);
  void (*process_next_triangle_func)(foug_stlb_geom_input_t*, const foug_stlb_triangle_t*);
  void (*end_triangles_func)(foug_stlb_geom_input_t*);
} foug_stlb_geom_input_manip_t;

/* foug_stlb_geom_input : services */

FOUG_DATAEX_LIBSTL_EXPORT
foug_stlb_geom_input_t* foug_stlb_geom_input_create(foug_malloc_func_t func,
                                                    void* data,
                                                    foug_stlb_geom_input_manip_t manip);

FOUG_DATAEX_LIBSTL_EXPORT
void* foug_stlb_geom_input_get_cookie(const foug_stlb_geom_input_t* geom);

FOUG_DATAEX_LIBSTL_EXPORT
void foug_stlb_geom_input_set_cookie(foug_stlb_geom_input_t* geom, void* data);

/* foug_stlb_read_args */
typedef struct foug_stlb_read_args
{
  foug_stlb_geom_input_t* geom_input;
  foug_stream_t* stream;
  foug_task_control_t* task_control;
  uint8_t* buffer;
  uint32_t buffer_size;
} foug_stlb_read_args_t;

FOUG_DATAEX_LIBSTL_EXPORT
int foug_stlb_read(foug_stlb_read_args_t args);

/* Error codes returned by foug_stlb_read() */
#define FOUG_STLB_READ_NO_ERROR                  0
#define FOUG_STLB_READ_NULL_GEOM_INPUT_ERROR     1
#define FOUG_STLB_READ_NULL_STREAM_ERROR         2
#define FOUG_STLB_READ_NULL_BUFFER               3
#define FOUG_STLB_READ_INVALID_BUFFER_SIZE_ERROR 4
#define FOUG_STLB_READ_HEADER_WRONG_SIZE_ERROR   5
#define FOUG_STLB_READ_FACET_COUNT_ERROR         6
#define FOUG_STLB_READ_STREAM_ERROR              7
#define FOUG_STLB_READ_TASK_STOPPED_ERROR        8

#endif /* FOUG_C_LIBSTL_STLB_READ_H */
