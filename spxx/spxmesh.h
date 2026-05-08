/*

Copyright (c) 2023 Eugenio Arteaga A.

Permission is hereby granted, free of charge, to any 
person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the 
Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice 
shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef SIMPLE_PIXEL_MESH_H
#define SIMPLE_PIXEL_MESH_H

/******************
****** spxmesh ******
 Simple PiXel Mesh
*******************
@Eugenio Arteaga A.
*******************

C89-friendly header-only mesh library for 2D and 3D models.
Depends on spxmath (vec2, vec3, mat4 helpers).

As with other spxx headers, define SPXMESH_APPLICATION in
exactly one translation unit to compile the implementation,
and define SPXM_APPLICATION in that same unit if you pull
in math from spxmath.h (recommended: include spxx.h with
SPXX_APPLICATION once).

****************************************************/

/* Simple PiXel Mesh */

#include <stddef.h>
#include <spxx/spxmath.h>

/* growable vec2 / vec3 buffers */

typedef struct SpxmeshVec2Array {
    vec2 *data;
    size_t bytes;
    size_t capacity;
    size_t size;
} SpxmeshVec2Array;

typedef struct SpxmeshVec3Array {
    vec3 *data;
    size_t bytes;
    size_t capacity;
    size_t size;
} SpxmeshVec3Array;

/* indexed vertex attributes (unique pool + index buffer) */

typedef struct SpxmeshVec2Table {
    size_t *indices;
    vec2 *data;
    size_t bytes;
    size_t capacity;
    size_t size;
} SpxmeshVec2Table;

typedef struct SpxmeshVec3Table {
    size_t *indices;
    vec3 *data;
    size_t bytes;
    size_t capacity;
    size_t size;
} SpxmeshVec3Table;

/* ---------- built-in shapes (triangle lists) ---------- */

const vec2 *spxmesh_shape_quad(void);
const vec3 *spxmesh_shape_cube(void);
const vec2 *spxmesh_shape_cube_uvs(void);
const vec3 *spxmesh_shape_skybox(void);
size_t spxmesh_shape_quad_size(void);
size_t spxmesh_shape_cube_size(void);
size_t spxmesh_shape_cube_uvs_size(void);
size_t spxmesh_shape_skybox_size(void);


/* ---------- 3D mesh ---------- */

typedef struct Mesh3D {
    SpxmeshVec3Array vertices;
    SpxmeshVec3Array normals;
    SpxmeshVec2Array uvs;
} Mesh3D;

typedef struct iMesh3D {
    SpxmeshVec3Table vertices;
    SpxmeshVec3Table normals;
    SpxmeshVec2Table uvs;
} iMesh3D;

/* --- Wavefront OBJ File Format I/O --- */
iMesh3D imesh3D_load_wavefront(const char *path);
void imesh3D_save_wavefront(const iMesh3D *mesh, const char *path);
void mesh3D_save_wavefront(const Mesh3D *mesh, const char *path);

/* --- Indexed 3D Mesh --- */
iMesh3D imesh3D_create(void);
iMesh3D imesh3D_load(const char *path);
iMesh3D imesh3D_shape_quad(const vec3 position);
iMesh3D imesh3D_shape_plane(const size_t width, const size_t height);
iMesh3D imesh3D_shape_cube(const size_t size);
iMesh3D imesh3D_shape_hex(const vec3 size);
iMesh3D imesh3D_shape_sphere(const size_t detail);
void imesh3D_save(const iMesh3D *mesh, const char *path);
void imesh3D_free(iMesh3D *mesh);

void imesh3D_push_quad_z(iMesh3D *mesh, const vec3 position);
void imesh3D_push_quad_xyz(iMesh3D *mesh, const vec3 position, const vec3 xvec, const vec3 yvec);
void imesh3D_push_plane_z(iMesh3D *mesh, const size_t xsize, const size_t ysize);
void imesh3D_push_plane_xyz(iMesh3D *mesh, const size_t xsize, const size_t ysize, const vec3 position, const vec3 xvec, const vec3 yvec);
void imesh3D_normalize_faces(iMesh3D *mesh);
void imesh3D_scale(iMesh3D *mesh, const float f);
void imesh3D_move(iMesh3D *mesh, const vec3 add);
void imesh3D_normalize(iMesh3D *mesh);
void imesh3D_merge(iMesh3D *m1, iMesh3D *m2);

/* --- 3D Mesh --- */
Mesh3D mesh3D_create(void);
Mesh3D mesh3D_load(const char *path);
Mesh3D mesh3D_shape_quad(const vec3 position);
Mesh3D mesh3D_shape_plane(const size_t width, const size_t height);
Mesh3D mesh3D_shape_cube(const size_t size);
Mesh3D mesh3D_shape_hex(const vec3 size);
Mesh3D mesh3D_shape_sphere(const size_t detail);
void mesh3D_save(const Mesh3D *mesh, const char *path);
void mesh3D_save_quick(const Mesh3D *mesh, const char *path);
void mesh3D_free(Mesh3D *mesh);

void mesh3D_push_quad_z(Mesh3D *mesh, const vec3 position);
void mesh3D_push_quad_xyz(Mesh3D *mesh, const vec3 position, const vec3 xvec, const vec3 yvec);
void mesh3D_push_plane_z(Mesh3D *mesh, const size_t xsize, const size_t ysize);
void mesh3D_push_plane_xyz(Mesh3D *mesh, const size_t xsize, const size_t ysize, const vec3 position, const vec3 xvec, const vec3 yvec);
void mesh3D_normalize_faces(Mesh3D *mesh);
void mesh3D_scale(Mesh3D *mesh, const float f);
void mesh3D_move(Mesh3D *mesh, const vec3 add);
void mesh3D_normalize(Mesh3D *mesh);
void mesh3D_merge(Mesh3D *m1, Mesh3D *m2);

/* --- 3D mesh type conversions --- */
Mesh3D imesh3D_to_mesh3D(const iMesh3D *mesh);
iMesh3D mesh3D_to_imesh3D(const Mesh3D *mesh);


/* ---------- 2D mesh ---------- */

typedef struct Mesh2D {
    SpxmeshVec2Array vertices;
    SpxmeshVec2Array uvs;
} Mesh2D;

typedef struct iMesh2D {
    SpxmeshVec2Table vertices;
    SpxmeshVec2Table uvs;
} iMesh2D;

/* --- Indexed 2D Mesh --- */
iMesh2D imesh2D_create(void);
iMesh2D imesh2D_shape_quad(const vec2 position);
iMesh2D imesh2D_shape_plane(const size_t width, const size_t height);
iMesh2D imesh2D_shape_polygon(const size_t sides);
void imesh2D_free(iMesh2D *mesh);

void imesh2D_push_quad(iMesh2D *mesh, const vec2 position);
void imesh2D_push_plane(iMesh2D *mesh, const size_t xsize, const size_t ysize);
void imesh2D_scale(iMesh2D *mesh, const float f);
void imesh2D_move(iMesh2D *mesh, const vec2 add);
void imesh2D_merge(iMesh2D *m1, iMesh2D *m2);

/* --- 2D Mesh --- */
Mesh2D mesh2D_create(void);
Mesh2D mesh2D_shape_quad(const vec2 position);
Mesh2D mesh2D_shape_plane(const size_t width, const size_t height);
Mesh2D mesh2D_shape_polygon(const size_t sides);
void mesh2D_free(Mesh2D *mesh);

void mesh2D_push_quad(Mesh2D *mesh, const vec2 position);
void mesh2D_push_plane(Mesh2D *mesh, const size_t xsize, const size_t ysize);
void mesh2D_scale(Mesh2D *mesh, const float f);
void mesh2D_move(Mesh2D *mesh, const vec2 add);
void mesh2D_merge(Mesh2D *m1, Mesh2D *m2);

/* --- 2D mesh type conversions --- */
Mesh2D imesh2D_to_mesh2D(const iMesh2D *mesh);
iMesh2D mesh2D_to_imesh2D(const Mesh2D *mesh);

#ifdef SPXMESH_APPLICATION

/******************
****** spxmesh ******
 Simple PiXel Mesh
*******************
* IMPLEMENTATION  *
*******************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SPXMESH_BUCKET_CAP_INDEX 0
#define SPXMESH_BUCKET_SIZE_INDEX 1
#define SPXMESH_BUCKET_DATA_INDEX 2

#define SPXMESH_BUCKET_SIZE(bucket) ((bucket) ? (bucket)[SPXMESH_BUCKET_SIZE_INDEX] : 0)
#define SPXMESH_BUCKET_CAP(bucket) ((bucket) ? (bucket)[SPXMESH_BUCKET_CAP_INDEX] : 0)

#define spxmesh_countof(x) (sizeof(x) / sizeof((x)[0]))

/*--- bucket ---*/

static size_t *spxmesh_bucket_push(size_t *bucket, size_t index)
{
    size_t size = SPXMESH_BUCKET_SIZE(bucket);
    size_t cap = SPXMESH_BUCKET_CAP(bucket);

    if (size >= cap) {
        cap = (!cap + cap) * 2;
        bucket = (size_t *)realloc(bucket, (cap + SPXMESH_BUCKET_DATA_INDEX) * sizeof(size_t));
        bucket[SPXMESH_BUCKET_CAP_INDEX] = cap;
    }

    bucket[SPXMESH_BUCKET_DATA_INDEX + size] = index;
    bucket[SPXMESH_BUCKET_SIZE_INDEX] = size + 1;

    return bucket;
}

/*--- SpxmeshVec2Array ---*/

static SpxmeshVec2Array spxmesh_vec2_array_create(void)
{
    SpxmeshVec2Array v;
    v.data = NULL;
    v.bytes = sizeof(vec2);
    v.capacity = 0;
    v.size = 0;
    return v;
}

static void spxmesh_vec2_array_resize(SpxmeshVec2Array *v, size_t n)
{
    v->capacity = (n > v->size) ? n : v->size;
    if (v->capacity) {
        v->data = (vec2 *)realloc(v->data, v->capacity * sizeof(vec2));
    }
}

static void spxmesh_vec2_array_push(SpxmeshVec2Array *v, const vec2 *x)
{
    vec2 *nd;
    if (v->size == v->capacity) {
        v->capacity = v->capacity * 2 + !v->capacity;
        nd = (vec2 *)realloc(v->data, v->capacity * sizeof(vec2));
        v->data = nd;
    }
    v->data[v->size] = *x;
    v->size++;
}

static void spxmesh_vec2_array_free(SpxmeshVec2Array *v)
{
    if (v->data) {
        free(v->data);
        v->data = NULL;
        v->capacity = 0;
        v->size = 0;
    }
}

static SpxmeshVec2Array spxmesh_vec2_array_wrap(vec2 *data, size_t sz)
{
    SpxmeshVec2Array v;
    v.data = data;
    v.bytes = sizeof(vec2);
    v.capacity = sz;
    v.size = sz;
    return v;
}

/*--- SpxmeshVec3Array ---*/

static SpxmeshVec3Array spxmesh_vec3_array_create(void)
{
    SpxmeshVec3Array v;
    v.data = NULL;
    v.bytes = sizeof(vec3);
    v.capacity = 0;
    v.size = 0;
    return v;
}

static void spxmesh_vec3_array_resize(SpxmeshVec3Array *v, size_t n)
{
    v->capacity = (n > v->size) ? n : v->size;
    if (v->capacity) {
        v->data = (vec3 *)realloc(v->data, v->capacity * sizeof(vec3));
    }
}

static void spxmesh_vec3_array_push(SpxmeshVec3Array *v, const vec3 *x)
{
    vec3 *nd;
    if (v->size == v->capacity) {
        v->capacity = v->capacity * 2 + !v->capacity;
        nd = (vec3 *)realloc(v->data, v->capacity * sizeof(vec3));
        v->data = nd;
    }
    v->data[v->size] = *x;
    v->size++;
}

static void spxmesh_vec3_array_clear(SpxmeshVec3Array *v)
{
    v->size = 0;
}

static void spxmesh_vec3_array_free(SpxmeshVec3Array *v)
{
    if (v->data) {
        free(v->data);
        v->data = NULL;
        v->capacity = 0;
        v->size = 0;
    }
}

static SpxmeshVec3Array spxmesh_vec3_array_wrap(vec3 *data, size_t sz)
{
    SpxmeshVec3Array v;
    v.data = data;
    v.bytes = sizeof(vec3);
    v.capacity = sz;
    v.size = sz;
    return v;
}

/*--- SpxmeshVec2Table ---*/

static SpxmeshVec2Table spxmesh_vec2_table_create(void)
{
    SpxmeshVec2Table t;
    t.indices = NULL;
    t.data = NULL;
    t.bytes = sizeof(vec2);
    t.capacity = 0;
    t.size = 0;
    return t;
}

static size_t spxmesh_vec2_table_search(const SpxmeshVec2Table *t, const vec2 *x)
{
    size_t i;
    for (i = 0; i < t->size; ++i) {
        if (memcmp(&t->data[i], x, sizeof(vec2)) == 0) {
            return i + 1;
        }
    }
    return 0;
}

static void spxmesh_vec2_table_push_index(SpxmeshVec2Table *t, size_t index)
{
    t->indices = spxmesh_bucket_push(t->indices, index);
}

static void spxmesh_vec2_table_push_data(SpxmeshVec2Table *t, const vec2 *x)
{
    vec2 *nd;
    if (t->size == t->capacity) {
        t->capacity = t->capacity * 2 + !t->capacity;
        nd = (vec2 *)realloc(t->data, t->capacity * sizeof(vec2));
        t->data = nd;
    }
    t->data[t->size++] = *x;
}

static size_t spxmesh_vec2_table_push(SpxmeshVec2Table *t, const vec2 *x)
{
    size_t search = spxmesh_vec2_table_search(t, x);
    if (!search) {
        spxmesh_vec2_table_push_data(t, x);
        search = t->size;
    }

    spxmesh_vec2_table_push_index(t, search - 1);
    return search;
}

static SpxmeshVec2Table spxmesh_vec2_table_compress(const vec2 *data, size_t count)
{
    SpxmeshVec2Table table = spxmesh_vec2_table_create();
    size_t i;

    for (i = 0; i < count; ++i) {
        spxmesh_vec2_table_push(&table, data + i);
    }

    return table;
}

static void *spxmesh_vec2_table_decompress(const SpxmeshVec2Table *t, size_t *size_out)
{
    size_t i;
    void *data;
    char *ptr;
    const size_t count = SPXMESH_BUCKET_SIZE(t->indices) + SPXMESH_BUCKET_DATA_INDEX;

    *size_out = SPXMESH_BUCKET_SIZE(t->indices);

    data = malloc((*size_out) * sizeof(vec2));
    ptr = (char *)data;

    for (i = SPXMESH_BUCKET_DATA_INDEX; i < count; ++i) {
        memcpy(ptr, ((char *)t->data) + t->indices[i] * sizeof(vec2), sizeof(vec2));
        ptr += sizeof(vec2);
    }

    return data;
}

static size_t *spxmesh_vec2_table_indices(const SpxmeshVec2Table *t)
{
    return t->indices ? (t->indices + SPXMESH_BUCKET_DATA_INDEX) : NULL;
}

static size_t spxmesh_vec2_table_indices_size(const SpxmeshVec2Table *t)
{
    return SPXMESH_BUCKET_SIZE(t->indices);
}

static void spxmesh_vec2_table_free(SpxmeshVec2Table *t)
{
    if (t->data) {
        free(t->data);
        free(t->indices);
        t->data = NULL;
        t->indices = NULL;
        t->size = 0;
        t->capacity = 0;
    }
}

/*--- SpxmeshVec3Table ---*/

static SpxmeshVec3Table spxmesh_vec3_table_create(void)
{
    SpxmeshVec3Table t;
    t.indices = NULL;
    t.data = NULL;
    t.bytes = sizeof(vec3);
    t.capacity = 0;
    t.size = 0;
    return t;
}

static size_t spxmesh_vec3_table_search(const SpxmeshVec3Table *t, const vec3 *x)
{
    size_t i;
    for (i = 0; i < t->size; ++i) {
        if (memcmp(&t->data[i], x, sizeof(vec3)) == 0) {
            return i + 1;
        }
    }
    return 0;
}

static void spxmesh_vec3_table_push_index(SpxmeshVec3Table *t, size_t index)
{
    t->indices = spxmesh_bucket_push(t->indices, index);
}

static void spxmesh_vec3_table_push_data(SpxmeshVec3Table *t, const vec3 *x)
{
    vec3 *nd;
    if (t->size == t->capacity) {
        t->capacity = t->capacity * 2 + !t->capacity;
        nd = (vec3 *)realloc(t->data, t->capacity * sizeof(vec3));
        t->data = nd;
    }
    t->data[t->size++] = *x;
}

static size_t spxmesh_vec3_table_push(SpxmeshVec3Table *t, const vec3 *x)
{
    size_t search = spxmesh_vec3_table_search(t, x);
    if (!search) {
        spxmesh_vec3_table_push_data(t, x);
        search = t->size;
    }

    spxmesh_vec3_table_push_index(t, search - 1);
    return search;
}

static SpxmeshVec3Table spxmesh_vec3_table_compress(const vec3 *data, size_t count)
{
    SpxmeshVec3Table table = spxmesh_vec3_table_create();
    size_t i;

    for (i = 0; i < count; ++i) {
        spxmesh_vec3_table_push(&table, data + i);
    }

    return table;
}

static void *spxmesh_vec3_table_decompress(const SpxmeshVec3Table *t, size_t *size_out)
{
    size_t i;
    void *data;
    char *ptr;
    const size_t count = SPXMESH_BUCKET_SIZE(t->indices) + SPXMESH_BUCKET_DATA_INDEX;

    *size_out = SPXMESH_BUCKET_SIZE(t->indices);

    data = malloc((*size_out) * sizeof(vec3));
    ptr = (char *)data;

    for (i = SPXMESH_BUCKET_DATA_INDEX; i < count; ++i) {
        memcpy(ptr, ((char *)t->data) + t->indices[i] * sizeof(vec3), sizeof(vec3));
        ptr += sizeof(vec3);
    }

    return data;
}

static size_t *spxmesh_vec3_table_indices(const SpxmeshVec3Table *t)
{
    return t->indices ? (t->indices + SPXMESH_BUCKET_DATA_INDEX) : NULL;
}

static size_t spxmesh_vec3_table_indices_size(const SpxmeshVec3Table *t)
{
    return SPXMESH_BUCKET_SIZE(t->indices);
}

static void spxmesh_vec3_table_free(SpxmeshVec3Table *t)
{
    if (t->data) {
        free(t->data);
        free(t->indices);
        t->data = NULL;
        t->indices = NULL;
        t->size = 0;
        t->capacity = 0;
    }
}

/*--- Common shapes ---*/

static const vec2 spxmesh_quad[] = {
    {1.0f, 1.0f},
    {1.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 0.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f}
};

static const vec3 spxmesh_cube[] = {
    {-1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f},
    {-1.0f, 1.0f, -1.0f},
    {1.0f, -1.0f, 1.0f},
    {-1.0f, -1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},
    {1.0f, 1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f},
    {-1.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f},
    {1.0f, -1.0f, 1.0f},
    {-1.0f, -1.0f, 1.0f},
    {-1.0f, -1.0f, -1.0f},
    {-1.0f, 1.0f, 1.0f},
    {-1.0f, -1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, -1.0f, -1.0f},
    {1.0f, 1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, -1.0f},
    {-1.0f, 1.0f, -1.0f},
    {1.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f},
    {-1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f}
};

static const vec2 spxmesh_cube_uvs[] = {
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},
    {0.0f, 0.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},
    {0.0f, 0.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {0.0f, 1.0f},
    {1.0f, 0.0f},
    {0.0f, 0.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {0.0f, 0.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 0.0f},
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {0.0f, 0.0f},
    {1.0f, 1.0f},
};

static const vec3 spxmesh_skybox[] = {
    {-1.0f, 1.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},
    {1.0f, 1.0f, -1.0f},
    {-1.0f, 1.0f, -1.0f},
    {-1.0f, -1.0f, 1.0f},
    {-1.0f, -1.0f, -1.0f},
    {-1.0f, 1.0f, -1.0f},
    {-1.0f, 1.0f, -1.0f},
    {-1.0f, 1.0f, 1.0f},
    {-1.0f, -1.0f, 1.0f},
    {1.0f, -1.0f, -1.0f},
    {1.0f, -1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f},
    {-1.0f, -1.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f},
    {1.0f, 1.0f, -1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f, 1.0f},
    {1.0f, -1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f}
};

const vec2 *spxmesh_shape_quad(void)
{
    return spxmesh_quad;
}

size_t spxmesh_shape_quad_size(void)
{
    return spxmesh_countof(spxmesh_quad);
}

const vec3 *spxmesh_shape_cube(void)
{
    return spxmesh_cube;
}

size_t spxmesh_shape_cube_size(void)
{
    return spxmesh_countof(spxmesh_cube);
}

const vec2 *spxmesh_shape_cube_uvs(void)
{
    return spxmesh_cube_uvs;
}

size_t spxmesh_shape_cube_uvs_size(void)
{
    return spxmesh_countof(spxmesh_cube_uvs);
}

const vec3 *spxmesh_shape_skybox(void)
{
    return spxmesh_skybox;
}

size_t spxmesh_shape_skybox_size(void)
{
    return spxmesh_countof(spxmesh_skybox);
}


/* --- Wavefront ---*/

iMesh3D imesh3D_load_wavefront(const char *path)
{
    static const char symbols[] = "/\n ";
    iMesh3D mesh = imesh3D_create();
    FILE *file;
    size_t line_count;
    char *token;
    char line[BUFSIZ];

    file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "spxmesh could not open file '%s'.\n", path);
        return mesh;
    }

    line_count = 0;

    while (fgets(line, (int)sizeof(line), file)) {

        ++line_count;

        token = strtok(line, symbols);
        if (!token || *token == '#') {
            continue;
        }

        if (!strcmp("vt", token)) {
            vec2 vt;
            float *fp = (float *)&vt;
            const size_t nfloats = sizeof(vec2) / sizeof(float);
            size_t ii;
            for (ii = 0; ii < nfloats; ++ii) {
                if (!(token = strtok(NULL, symbols))) {
                    fprintf(stderr,
                        "spxmesh detected a problem in the .obj file '%s' at line %lu.\n",
                        path, (unsigned long)line_count);
                    break;
                }
                sscanf(token, "%f", fp++);
            }
            spxmesh_vec2_table_push_data(&mesh.uvs, &vt);
        }
        else if (!strcmp("vn", token)) {
            vec3 vn;
            float *fp = (float *)&vn;
            const size_t nfloats = sizeof(vec3) / sizeof(float);
            size_t ii;
            for (ii = 0; ii < nfloats; ++ii) {
                if (!(token = strtok(NULL, symbols))) {
                    fprintf(stderr,
                        "spxmesh detected a problem in the .obj file '%s' at line %lu.\n",
                        path, (unsigned long)line_count);
                    break;
                }
                sscanf(token, "%f", fp++);
            }
            spxmesh_vec3_table_push_data(&mesh.normals, &vn);
        }
        else if (!strcmp("v", token)) {
            vec3 vtx;
            float *fp = (float *)&vtx;
            const size_t nfloats = sizeof(vec3) / sizeof(float);
            size_t ii;
            for (ii = 0; ii < nfloats; ++ii) {
                if (!(token = strtok(NULL, symbols))) {
                    fprintf(stderr,
                        "spxmesh detected a problem in the .obj file '%s' at line %lu.\n",
                        path, (unsigned long)line_count);
                    break;
                }
                sscanf(token, "%f", fp++);
            }
            spxmesh_vec3_table_push_data(&mesh.vertices, &vtx);
        }
        else if (!strcmp("f", token)) {

            size_t indices[9];
            size_t count = 0;
            size_t ii;

            for (ii = 0; ii < 9; ++ii) {
                if (!(token = strtok(NULL, symbols))) {
                    break;
                }
                sscanf(token, "%lu", &indices[count++]); /* %lu with size_t */
            }

            if (count == 9) {
                for (ii = 0; ii < 3; ++ii) {
                    spxmesh_vec3_table_push_index(&mesh.vertices, indices[ii * 3 + 0] - 1);
                    spxmesh_vec2_table_push_index(&mesh.uvs, indices[ii * 3 + 1] - 1);
                    spxmesh_vec3_table_push_index(&mesh.normals, indices[ii * 3 + 2] - 1);
                }
            }
            else if (count == 6) {
                for (ii = 0; ii < 3; ++ii) {
                    spxmesh_vec3_table_push_index(&mesh.vertices, indices[ii * 2 + 0] - 1);
                    spxmesh_vec3_table_push_index(&mesh.normals, indices[ii * 2 + 1] - 1);
                }
            }
            else if (count == 3) {
                for (ii = 0; ii < 3; ++ii) {
                    spxmesh_vec3_table_push_index(&mesh.vertices, indices[ii] - 1);
                }
            }
            else {
                fprintf(stderr, "spxmesh won't recognize format of file '%s'.\n", path);
                break;
            }
        }
    }

    fclose(file);
    return mesh;
}

void imesh3D_save_wavefront(const iMesh3D *mesh, const char *path)
{
    FILE *file;
    vec3 *v;
    vec3 *n;
    vec2 *uv;
    size_t vsize;
    size_t nsize;
    size_t uvsize;
    size_t *vindex;
    size_t *nindex;
    size_t *uvindex;
    size_t isize;
    size_t i;
    size_t j;

    file = fopen(path, "w");
    if (!file) {
        fprintf(stderr, "spxmesh could not write file '%s'.\n", path);
        return;
    }

    fprintf(file, "# spxmesh wavefront export\n");

    v = mesh->vertices.data;
    vsize = mesh->vertices.size;
    for (i = 0; i < vsize; ++i) {
        fprintf(file, "v %f %f %f\n", v[i].x, v[i].y, v[i].z);
    }

    n = mesh->normals.data;
    nsize = mesh->normals.size;
    for (i = 0; i < nsize; ++i) {
        fprintf(file, "vn %f %f %f\n", n[i].x, n[i].y, n[i].z);
    }

    uv = mesh->uvs.data;
    uvsize = mesh->uvs.size;
    for (i = 0; i < uvsize; ++i) {
        fprintf(file, "vt %f %f\n", uv[i].x, uv[i].y);
    }

    vindex = spxmesh_vec3_table_indices(&mesh->vertices);
    nindex = spxmesh_vec3_table_indices(&mesh->normals);
    uvindex = spxmesh_vec2_table_indices(&mesh->uvs);

    isize = spxmesh_vec3_table_indices_size(&mesh->vertices) / 3;

    for (i = 0; i < isize; ++i) {
        fprintf(file, "f");
        for (j = i * 3; j < i * 3 + 3; ++j) {
            if (nsize && uvsize) {
                fprintf(file, " %lu/%lu/%lu",
                    (unsigned long)(vindex[j] + 1),
                    (unsigned long)(uvindex[j] + 1),
                    (unsigned long)(nindex[j] + 1));
            }
            else if (nsize) {
                fprintf(file, " %lu//%lu",
                    (unsigned long)(vindex[j] + 1),
                    (unsigned long)(nindex[j] + 1));
            }
            else {
                fprintf(file, " %lu", (unsigned long)(vindex[j] + 1));
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void mesh3D_save_wavefront(const Mesh3D *mesh, const char *path)
{
    FILE *file;
    vec3 *v;
    vec3 *vn;
    vec2 *uv;
    size_t vsize;
    size_t nsize;
    size_t uvsize;
    size_t isize;
    size_t i;
    size_t j;
    size_t vnidx;

    file = fopen(path, "w");
    if (!file) {
        fprintf(stderr, "spxmesh could not write file '%s'.\n", path);
        return;
    }

    fprintf(file, "# spxmesh wavefront export\n");

    v = mesh->vertices.data;
    vsize = mesh->vertices.size;
    for (i = 0; i < vsize; ++i) {
        fprintf(file, "v %f %f %f\n", v[i].x, v[i].y, v[i].z);
    }

    vn = mesh->normals.data;
    nsize = mesh->normals.size;
    for (i = 0; i < nsize; ++i) {
        fprintf(file, "vn %f %f %f\n", vn[i].x, vn[i].y, vn[i].z);
    }

    uv = mesh->uvs.data;
    uvsize = mesh->uvs.size;
    for (i = 0; i < uvsize; ++i) {
        fprintf(file, "vt %f %f\n", uv[i].x, uv[i].y);
    }

    isize = mesh->vertices.size / 3;

    for (i = 0; i < isize; ++i) {
        fprintf(file, "f");
        for (j = i * 3; j < i * 3 + 3; ++j) {
            vnidx = j + 1;
            if (nsize && uvsize) {
                fprintf(file, " %lu/%lu/%lu",
                    (unsigned long)vnidx, (unsigned long)vnidx, (unsigned long)vnidx);
            }
            else if (nsize) {
                fprintf(file, " %lu//%lu", (unsigned long)vnidx, (unsigned long)vnidx);
            }
            else {
                fprintf(file, " %lu", (unsigned long)vnidx);
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

/* --- Indexed 3D Mesh --- */

iMesh3D imesh3D_create(void)
{
    iMesh3D mesh;
    mesh.vertices = spxmesh_vec3_table_create();
    mesh.normals = spxmesh_vec3_table_create();
    mesh.uvs = spxmesh_vec2_table_create();
    return mesh;
}

iMesh3D imesh3D_load(const char *path)
{
    return imesh3D_load_wavefront(path);
}

iMesh3D imesh3D_shape_quad(const vec3 position)
{
    iMesh3D mesh = imesh3D_create();
    imesh3D_push_quad_z(&mesh, position);
    return mesh;
}

iMesh3D imesh3D_shape_plane(const size_t width, const size_t height)
{
    iMesh3D mesh = imesh3D_create();
    size_t x;
    size_t z;

    for (x = 0; x < width; x++) {
        for (z = 0; z < height; z++) {
            imesh3D_push_quad_z(&mesh, vec3_new((float)(int)x, 0.0F, (float)(int)z));
        }
    }

    return mesh;
}

iMesh3D imesh3D_shape_cube(const size_t size)
{
    iMesh3D mesh = imesh3D_create();

    imesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0F, (float)size, 0.0F), vec3_new(1.0F, 0.0F, 0.0F), vec3_new(0.0F, 0.0F, 1.0F));
    imesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0F, 0.0F, 0.0F), vec3_new(0.0F, 0.0F, 1.0F), vec3_new(1.0F, 0.0F, 0.0F));
    imesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0F, 0.0F, 0.0F), vec3_new(1.0F, 0.0F, 0.0F), vec3_new(0.0F, 1.0F, 0.0F));
    imesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0F, 0.0F, (float)size), vec3_new(0.0F, 1.0F, 0.0F), vec3_new(1.0F, 0.0F, 0.0F));
    imesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0F, 0.0F, 0.0F), vec3_new(0.0F, 1.0F, 0.0F), vec3_new(0.0F, 0.0F, 1.0F));
    imesh3D_push_plane_xyz(&mesh, size, size, vec3_new((float)size, 0.0F, 0.0F), vec3_new(0.0F, 0.0F, 1.0F), vec3_new(0.0F, 1.0F, 0.0F));

    imesh3D_move(&mesh, vec3_uni(-(float)size * 0.5F));

    return mesh;
}

iMesh3D imesh3D_shape_hex(const vec3 size)
{
    iMesh3D mesh = imesh3D_create();

    imesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0F, size.y, 0.0F), vec3_new(size.x, 0.0F, 0.0F), vec3_new(0.0F, 0.0F, size.z));
    imesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0F, 0.0F, 0.0F), vec3_new(0.0F, 0.0F, size.z), vec3_new(size.x, 0.0F, 0.0F));
    imesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0F, 0.0F, 0.0F), vec3_new(size.x, 0.0F, 0.0F), vec3_new(0.0F, size.y, 0.0F));
    imesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0F, 0.0F, size.z), vec3_new(0.0F, size.y, 0.0F), vec3_new(size.x, 0.0F, 0.0F));
    imesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0F, 0.0F, 0.0F), vec3_new(0.0F, size.y, 0.0F), vec3_new(0.0F, 0.0F, size.z));
    imesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(size.x, 0.0F, 0.0F), vec3_new(0.0F, 0.0F, size.z), vec3_new(0.0F, size.y, 0.0F));

    imesh3D_move(&mesh, vec3_mult(size, -0.5F));

    return mesh;
}

iMesh3D imesh3D_shape_sphere(const size_t size)
{
    iMesh3D mesh = imesh3D_shape_cube(size);
    imesh3D_normalize(&mesh);
    imesh3D_normalize_faces(&mesh);
    return mesh;
}

void imesh3D_save(const iMesh3D *mesh, const char *path)
{
    imesh3D_save_wavefront(mesh, path);
}

void imesh3D_free(iMesh3D *mesh)
{
    spxmesh_vec3_table_free(&mesh->vertices);
    spxmesh_vec3_table_free(&mesh->normals);
    spxmesh_vec2_table_free(&mesh->uvs);
}

void imesh3D_push_quad_z(iMesh3D *mesh, const vec3 position)
{
    const vec2 *quad = spxmesh_shape_quad();
    size_t i;

    for (i = 0; i < 6; i++) {
        vec3 pos;

        pos.x = quad[i].x + position.x;
        pos.y = position.y;
        pos.z = quad[i].y + position.z;

        spxmesh_vec3_table_push(&mesh->vertices, &pos);
    }
}

void imesh3D_push_quad_xyz(iMesh3D *mesh, const vec3 position, const vec3 xvec, const vec3 yvec)
{
    const vec2 *quad = spxmesh_shape_quad();
    size_t i;

    for (i = 0; i < 6; i++) {
        vec3 pos;

        pos.x = quad[i].x * xvec.x + quad[i].y * yvec.x + position.x;
        pos.y = quad[i].x * xvec.y + quad[i].y * yvec.y + position.y;
        pos.z = quad[i].x * xvec.z + quad[i].y * yvec.z + position.z;

        spxmesh_vec3_table_push(&mesh->vertices, &pos);
    }
}

void imesh3D_push_plane_z(iMesh3D *mesh, const size_t xsize, const size_t ysize)
{
    size_t x;
    size_t y;

    for (x = 0; x < xsize; x++) {
        for (y = 0; y < ysize; y++) {
            imesh3D_push_quad_z(mesh, vec3_new((float)(int)x, 0.0F, (float)(int)y));
        }
    }
}

void imesh3D_push_plane_xyz(iMesh3D *mesh, const size_t xsize, const size_t ysize,
    const vec3 position, const vec3 xvec, const vec3 yvec)
{
    size_t x;
    size_t y;

    for (x = 0; x < xsize; x++) {
        for (y = 0; y < ysize; y++) {
            float fx = (float)(int)x;
            float fy = (float)(int)y;
            vec3 offset;

            offset.x = fx * xvec.x + fy * yvec.x + position.x;
            offset.y = fx * xvec.y + fy * yvec.y + position.y;
            offset.z = fx * xvec.z + fy * yvec.z + position.z;

            imesh3D_push_quad_xyz(mesh, offset, xvec, yvec);
        }
    }
}

void imesh3D_normalize_faces(iMesh3D *mesh)
{
    vec3 *pos = mesh->vertices.data;
    size_t *indices = spxmesh_vec3_table_indices(&mesh->vertices);

    spxmesh_vec3_table_free(&mesh->normals);

    mesh->normals = spxmesh_vec3_table_create();

    {
        const size_t triangles = spxmesh_vec3_table_indices_size(&mesh->vertices) / 3;
        size_t i;

        for (i = 0; i < triangles; i++) {

            size_t p1 = indices[i * 3 + 0];
            size_t p2 = indices[i * 3 + 1];
            size_t p3 = indices[i * 3 + 2];

            vec3 vu = vec3_sub(pos[p2], pos[p1]);
            vec3 vv = vec3_sub(pos[p3], pos[p1]);
            vec3 nc = vec3_cross(vu, vv);
            vec3 n = vec3_norm(nc);

            {
                size_t search = spxmesh_vec3_table_push(&mesh->normals, &n) - 1;
                spxmesh_vec3_table_push_index(&mesh->normals, search);
                spxmesh_vec3_table_push_index(&mesh->normals, search);
            }
        }
    }
}

void imesh3D_merge(iMesh3D *m1, iMesh3D *m2)
{
    size_t i;
    const size_t vsize = spxmesh_vec3_table_indices_size(&m2->vertices);
    size_t *vindices = spxmesh_vec3_table_indices(&m2->vertices);
    vec3 *v = m2->vertices.data;

    for (i = 0; i < vsize; ++i) {
        spxmesh_vec3_table_push(&m1->vertices, v + vindices[i]);
    }

    {
        const size_t nsize = spxmesh_vec3_table_indices_size(&m2->normals);
        size_t *nindices = spxmesh_vec3_table_indices(&m2->normals);
        vec3 *n = m2->normals.data;

        for (i = 0; i < nsize; ++i) {
            spxmesh_vec3_table_push(&m1->normals, n + nindices[i]);
        }
    }

    {
        const size_t uvsize = spxmesh_vec2_table_indices_size(&m2->uvs);
        size_t *uvindices = spxmesh_vec2_table_indices(&m2->uvs);
        vec2 *uv = m2->uvs.data;

        for (i = 0; i < uvsize; ++i) {
            spxmesh_vec2_table_push(&m1->uvs, uv + uvindices[i]);
        }
    }

    imesh3D_free(m2);
}

void imesh3D_scale(iMesh3D *mesh, const float f)
{
    vec3 *v = mesh->vertices.data;
    size_t vsize = mesh->vertices.size;
    size_t i;

    for (i = 0; i < vsize; i++) {
        v[i] = vec3_mult(v[i], f);
    }
}

void imesh3D_move(iMesh3D *mesh, const vec3 add)
{
    vec3 *v = mesh->vertices.data;
    size_t vsize = mesh->vertices.size;
    size_t i;

    for (i = 0; i < vsize; i++) {
        v[i] = vec3_add(v[i], add);
    }
}

void imesh3D_normalize(iMesh3D *mesh)
{
    vec3 *v = mesh->vertices.data;
    size_t vsize = mesh->vertices.size;
    size_t i;

    for (i = 0; i < vsize; i++) {
        v[i] = vec3_norm(v[i]);
    }
}

iMesh3D mesh3D_to_imesh3D(const Mesh3D *mesh)
{
    iMesh3D m;
    m.vertices = spxmesh_vec3_table_compress(mesh->vertices.data, mesh->vertices.size);
    m.normals = spxmesh_vec3_table_compress(mesh->normals.data, mesh->normals.size);
    m.uvs = spxmesh_vec2_table_compress(mesh->uvs.data, mesh->uvs.size);
    return m;
}


/* --- 3D Mesh ---*/

Mesh3D mesh3D_create(void)
{
    Mesh3D mesh;

    mesh.vertices = spxmesh_vec3_array_create();
    mesh.normals = spxmesh_vec3_array_create();
    mesh.uvs = spxmesh_vec2_array_create();

    return mesh;
}

Mesh3D mesh3D_load(const char *path)
{
    iMesh3D m = imesh3D_load(path);
    Mesh3D mesh = imesh3D_to_mesh3D(&m);
    imesh3D_free(&m);
    return mesh;
}

Mesh3D mesh3D_shape_quad(const vec3 position)
{
    Mesh3D mesh = mesh3D_create();
    spxmesh_vec3_array_resize(&mesh.vertices, 6);
    mesh3D_push_quad_z(&mesh, position);
    return mesh;
}

Mesh3D mesh3D_shape_plane(const size_t width, const size_t height)
{
    Mesh3D mesh = mesh3D_create();
    size_t x;
    size_t z;

    spxmesh_vec3_array_resize(&mesh.vertices, width * height * 6);

    for (x = 0; x < width; x++) {
        for (z = 0; z < height; z++) {
            mesh3D_push_quad_z(&mesh, vec3_new((float)(int)x, 0.0F, (float)(int)z));
        }
    }

    return mesh;
}

Mesh3D mesh3D_shape_cube(const size_t size)
{
    Mesh3D mesh = mesh3D_create();

    spxmesh_vec3_array_resize(&mesh.vertices, 36 * size * size);

    mesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0F, (float)size, 0.0F), vec3_new(1.0F, 0.0F, 0.0F), vec3_new(0.0F, 0.0F, 1.0F));
    mesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0F, 0.0F, 0.0F), vec3_new(0.0F, 0.0F, 1.0F), vec3_new(1.0F, 0.0F, 0.0F));
    mesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0F, 0.0F, 0.0F), vec3_new(1.0F, 0.0F, 0.0F), vec3_new(0.0F, 1.0F, 0.0F));
    mesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0F, 0.0F, (float)size), vec3_new(0.0F, 1.0F, 0.0F), vec3_new(1.0F, 0.0F, 0.0F));
    mesh3D_push_plane_xyz(&mesh, size, size, vec3_new(0.0F, 0.0F, 0.0F), vec3_new(0.0F, 1.0F, 0.0F), vec3_new(0.0F, 0.0F, 1.0F));
    mesh3D_push_plane_xyz(&mesh, size, size, vec3_new((float)size, 0.0F, 0.0F), vec3_new(0.0F, 0.0F, 1.0F), vec3_new(0.0F, 1.0F, 0.0F));

    mesh3D_move(&mesh, vec3_uni(-(float)size * 0.5F));

    return mesh;
}

Mesh3D mesh3D_shape_hex(const vec3 size)
{
    Mesh3D mesh = mesh3D_create();
    spxmesh_vec3_array_resize(&mesh.vertices, 36);

    mesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0F, size.y, 0.0F), vec3_new(size.x, 0.0F, 0.0F), vec3_new(0.0F, 0.0F, size.z));
    mesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0F, 0.0F, 0.0F), vec3_new(0.0F, 0.0F, size.z), vec3_new(size.x, 0.0F, 0.0F));
    mesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0F, 0.0F, 0.0F), vec3_new(size.x, 0.0F, 0.0F), vec3_new(0.0F, size.y, 0.0F));
    mesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0F, 0.0F, size.z), vec3_new(0.0F, size.y, 0.0F), vec3_new(size.x, 0.0F, 0.0F));
    mesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(0.0F, 0.0F, 0.0F), vec3_new(0.0F, size.y, 0.0F), vec3_new(0.0F, 0.0F, size.z));
    mesh3D_push_plane_xyz(&mesh, 1, 1, vec3_new(size.x, 0.0F, 0.0F), vec3_new(0.0F, 0.0F, size.z), vec3_new(0.0F, size.y, 0.0F));

    mesh3D_move(&mesh, vec3_mult(size, -0.5F));

    return mesh;
}

Mesh3D mesh3D_shape_sphere(const size_t size)
{
    Mesh3D mesh = mesh3D_shape_cube(size);
    mesh3D_normalize(&mesh);
    mesh3D_normalize_faces(&mesh);
    return mesh;
}

void mesh3D_save(const Mesh3D *mesh, const char *path)
{
    iMesh3D m = mesh3D_to_imesh3D(mesh);
    imesh3D_save(&m, path);
    imesh3D_free(&m);
}

void mesh3D_save_quick(const Mesh3D *mesh, const char *path)
{
    mesh3D_save_wavefront(mesh, path);
}

void mesh3D_free(Mesh3D *mesh)
{
    spxmesh_vec3_array_free(&mesh->vertices);
    spxmesh_vec3_array_free(&mesh->normals);
    spxmesh_vec2_array_free(&mesh->uvs);
}

void mesh3D_push_quad_z(Mesh3D *mesh, const vec3 position)
{
    const vec2 *quad = spxmesh_shape_quad();
    size_t i;

    for (i = 0; i < 6; i++) {
        vec3 pos;

        pos.x = quad[i].x + position.x;
        pos.y = position.y;
        pos.z = quad[i].y + position.z;

        spxmesh_vec3_array_push(&mesh->vertices, &pos);
    }
}

void mesh3D_push_quad_xyz(Mesh3D *mesh, const vec3 position, const vec3 xvec, const vec3 yvec)
{
    const vec2 *quad = spxmesh_shape_quad();
    size_t i;

    for (i = 0; i < 6; i++) {
        vec3 pos;

        pos.x = quad[i].x * xvec.x + quad[i].y * yvec.x + position.x;
        pos.y = quad[i].x * xvec.y + quad[i].y * yvec.y + position.y;
        pos.z = quad[i].x * xvec.z + quad[i].y * yvec.z + position.z;

        spxmesh_vec3_array_push(&mesh->vertices, &pos);
    }
}

void mesh3D_push_plane_z(Mesh3D *mesh, const size_t xsize, const size_t ysize)
{
    size_t x;
    size_t y;

    for (x = 0; x < xsize; x++) {
        for (y = 0; y < ysize; y++) {
            mesh3D_push_quad_z(mesh, vec3_new((float)(int)x, 0.0F, (float)(int)y));
        }
    }
}

void mesh3D_push_plane_xyz(Mesh3D *mesh, const size_t xsize, const size_t ysize,
    const vec3 position, const vec3 xvec, const vec3 yvec)
{
    size_t x;
    size_t y;

    for (x = 0; x < xsize; x++) {
        for (y = 0; y < ysize; y++) {
            float fx = (float)(int)x;
            float fy = (float)(int)y;
            vec3 offset;

            offset.x = fx * xvec.x + fy * yvec.x + position.x;
            offset.y = fx * xvec.y + fy * yvec.y + position.y;
            offset.z = fx * xvec.z + fy * yvec.z + position.z;

            mesh3D_push_quad_xyz(mesh, offset, xvec, yvec);
        }
    }
}

void mesh3D_normalize_faces(Mesh3D *mesh)
{
    vec3 *pos = mesh->vertices.data;
    const size_t triangles = mesh->vertices.size / 3;
    size_t i;

    spxmesh_vec3_array_clear(&mesh->normals);

    for (i = 0; i < triangles; i++) {

        vec3 vu = vec3_sub(pos[i * 3 + 1], pos[i * 3]);
        vec3 vv = vec3_sub(pos[i * 3 + 2], pos[i * 3]);
        vec3 nc = vec3_cross(vu, vv);
        vec3 n = vec3_norm(nc);

        spxmesh_vec3_array_push(&mesh->normals, &n);
        spxmesh_vec3_array_push(&mesh->normals, &n);
        spxmesh_vec3_array_push(&mesh->normals, &n);
    }
}

void mesh3D_merge(Mesh3D *m1, Mesh3D *m2)
{
    size_t i;
    const size_t vsize = m2->vertices.size;
    vec3 *v = m2->vertices.data;

    const size_t nsize = m2->normals.size;
    vec3 *n = m2->normals.data;

    const size_t uvsize = m2->uvs.size;
    vec2 *uv = m2->uvs.data;

    for (i = 0; i < vsize; i++) {
        spxmesh_vec3_array_push(&m1->vertices, v + i);
    }

    for (i = 0; i < nsize; i++) {
        spxmesh_vec3_array_push(&m1->normals, n + i);
    }

    for (i = 0; i < uvsize; i++) {
        spxmesh_vec2_array_push(&m1->uvs, uv + i);
    }

    mesh3D_free(m2);
}

void mesh3D_scale(Mesh3D *mesh, const float f)
{
    const size_t vsize = mesh->vertices.size;
    vec3 *v = mesh->vertices.data;
    size_t i;

    for (i = 0; i < vsize; i++) {
        v[i] = vec3_mult(v[i], f);
    }
}

void mesh3D_move(Mesh3D *mesh, const vec3 add)
{
    const size_t vsize = mesh->vertices.size;
    vec3 *v = mesh->vertices.data;
    size_t i;

    for (i = 0; i < vsize; i++) {
        v[i] = vec3_add(v[i], add);
    }
}

void mesh3D_normalize(Mesh3D *mesh)
{
    const size_t vsize = mesh->vertices.size;
    vec3 *v = mesh->vertices.data;
    size_t i;

    for (i = 0; i < vsize; i++) {
        v[i] = vec3_norm(v[i]);
    }
}

Mesh3D imesh3D_to_mesh3D(const iMesh3D *mesh)
{
    Mesh3D m;
    size_t vertcount;
    size_t normcount;
    size_t uvcount;
    void *vertdata;
    void *normdata;
    void *uvdata;

    vertdata = spxmesh_vec3_table_decompress(&mesh->vertices, &vertcount);
    normdata = spxmesh_vec3_table_decompress(&mesh->normals, &normcount);
    uvdata = spxmesh_vec2_table_decompress(&mesh->uvs, &uvcount);

    m.vertices = spxmesh_vec3_array_wrap((vec3 *)vertdata, vertcount);
    m.normals = spxmesh_vec3_array_wrap((vec3 *)normdata, normcount);
    m.uvs = spxmesh_vec2_array_wrap((vec2 *)uvdata, uvcount);
    return m;
}


/* --- 2D Mesh --- */

Mesh2D mesh2D_create(void)
{
    Mesh2D mesh;

    mesh.vertices = spxmesh_vec2_array_create();
    mesh.uvs = spxmesh_vec2_array_create();

    return mesh;
}

Mesh2D mesh2D_shape_quad(const vec2 position)
{
    Mesh2D mesh = mesh2D_create();
    spxmesh_vec2_array_resize(&mesh.vertices, 6);
    mesh2D_push_quad(&mesh, position);
    return mesh;
}

Mesh2D mesh2D_shape_plane(const size_t width, const size_t height)
{
    Mesh2D mesh = mesh2D_create();
    size_t x;
    size_t y;

    spxmesh_vec2_array_resize(&mesh.vertices, width * height * 6);

    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            mesh2D_push_quad(&mesh, vec2_new((float)(int)x, (float)(int)y));
        }
    }

    return mesh;
}

Mesh2D mesh2D_shape_polygon(const size_t sides)
{
    Mesh2D mesh = mesh2D_create();

    {
        const float offset = 2.0F * (float)M_PI / (float)sides;

        vec2 center;
        vec2 orig;
        vec2 p1;
        size_t i;

        center.x = 0.0F;
        center.y = 0.0F;
        orig.x = cosf(0.0F);
        orig.y = sinf(0.0F);
        p1 = orig;

        for (i = 1; i < sides; ++i) {

            float rot = (float)i * offset;
            vec2 p2;

            p2.x = cosf(rot);
            p2.y = sinf(rot);

            spxmesh_vec2_array_push(&mesh.vertices, &center);
            spxmesh_vec2_array_push(&mesh.vertices, &p1);
            spxmesh_vec2_array_push(&mesh.vertices, &p2);

            p1 = p2;
        }

        spxmesh_vec2_array_push(&mesh.vertices, &center);
        spxmesh_vec2_array_push(&mesh.vertices, &p1);
        spxmesh_vec2_array_push(&mesh.vertices, &orig);
    }

    return mesh;
}

void mesh2D_free(Mesh2D *mesh)
{
    spxmesh_vec2_array_free(&mesh->vertices);
    spxmesh_vec2_array_free(&mesh->uvs);
}

void mesh2D_push_quad(Mesh2D *mesh, const vec2 position)
{
    const vec2 *quad = spxmesh_shape_quad();
    size_t i;

    for (i = 0; i < 6; i++) {
        vec2 pos;

        pos.x = quad[i].x + position.x;
        pos.y = quad[i].y + position.y;

        spxmesh_vec2_array_push(&mesh->vertices, &pos);
    }
}

void mesh2D_push_plane(Mesh2D *mesh, const size_t xsize, const size_t ysize)
{
    size_t x;
    size_t y;

    for (x = 0; x < xsize; x++) {
        for (y = 0; y < ysize; y++) {
            mesh2D_push_quad(mesh, vec2_new((float)(int)x, (float)(int)y));
        }
    }
}

void mesh2D_merge(Mesh2D *m1, Mesh2D *m2)
{
    size_t i;

    const size_t vsize = m2->vertices.size;
    vec2 *v = m2->vertices.data;

    const size_t uvsize = m2->uvs.size;
    vec2 *uv = m2->uvs.data;

    for (i = 0; i < vsize; i++) {
        spxmesh_vec2_array_push(&m1->vertices, v + i);
    }

    for (i = 0; i < uvsize; i++) {
        spxmesh_vec2_array_push(&m1->uvs, uv + i);
    }

    mesh2D_free(m2);
}

void mesh2D_scale(Mesh2D *mesh, const float f)
{
    const size_t vsize = mesh->vertices.size;
    vec2 *v = mesh->vertices.data;
    size_t i;

    for (i = 0; i < vsize; i++) {
        v[i] = vec2_mult(v[i], f);
    }
}

void mesh2D_move(Mesh2D *mesh, const vec2 add)
{
    const size_t vsize = mesh->vertices.size;
    vec2 *v = mesh->vertices.data;
    size_t i;

    for (i = 0; i < vsize; i++) {
        v[i] = vec2_add(v[i], add);
    }
}

Mesh2D imesh2D_to_mesh2D(const iMesh2D *mesh)
{
    Mesh2D m;
    size_t vertcount;
    size_t uvcount;
    void *vertdata = spxmesh_vec2_table_decompress(&mesh->vertices, &vertcount);
    void *uvdata = spxmesh_vec2_table_decompress(&mesh->uvs, &uvcount);

    m.vertices = spxmesh_vec2_array_wrap((vec2 *)vertdata, vertcount);
    m.uvs = spxmesh_vec2_array_wrap((vec2 *)uvdata, uvcount);
    return m;
}


/* --- Indexed 2D Mesh ---*/

iMesh2D imesh2D_create(void)
{
    iMesh2D mesh;

    mesh.vertices = spxmesh_vec2_table_create();
    mesh.uvs = spxmesh_vec2_table_create();

    return mesh;
}

iMesh2D imesh2D_shape_quad(const vec2 position)
{
    iMesh2D mesh = imesh2D_create();
    imesh2D_push_quad(&mesh, position);
    return mesh;
}

iMesh2D imesh2D_shape_plane(const size_t width, const size_t height)
{
    iMesh2D mesh = imesh2D_create();
    size_t x;
    size_t y;

    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            imesh2D_push_quad(&mesh, vec2_new((float)(int)x, (float)(int)y));
        }
    }

    return mesh;
}

iMesh2D imesh2D_shape_polygon(const size_t sides)
{
    iMesh2D mesh = imesh2D_create();

    if (sides >= 3) {
        const float offset = 2.0F * (float)M_PI / (float)sides;

        vec2 center;
        vec2 orig;
        vec2 p1;
        size_t i;

        center.x = 0.0F;
        center.y = 0.0F;
        orig.x = cosf(0.0F);
        orig.y = sinf(0.0F);
        p1 = orig;

        for (i = 1; i < sides; ++i) {

            float rot = (float)i * offset;
            vec2 p2;

            p2.x = cosf(rot);
            p2.y = sinf(rot);

            spxmesh_vec2_table_push(&mesh.vertices, &center);
            spxmesh_vec2_table_push(&mesh.vertices, &p1);
            spxmesh_vec2_table_push(&mesh.vertices, &p2);

            p1 = p2;
        }

        spxmesh_vec2_table_push(&mesh.vertices, &center);
        spxmesh_vec2_table_push(&mesh.vertices, &p1);
        spxmesh_vec2_table_push(&mesh.vertices, &orig);
    }

    return mesh;
}

void imesh2D_free(iMesh2D *mesh)
{
    spxmesh_vec2_table_free(&mesh->vertices);
    spxmesh_vec2_table_free(&mesh->uvs);
}

void imesh2D_push_quad(iMesh2D *mesh, const vec2 position)
{
    const vec2 *quad = spxmesh_shape_quad();
    size_t i;

    for (i = 0; i < 6; i++) {
        vec2 pos;

        pos.x = quad[i].x + position.x;
        pos.y = quad[i].y + position.y;

        spxmesh_vec2_table_push(&mesh->vertices, &pos);
    }
}

void imesh2D_push_plane(iMesh2D *mesh, const size_t xsize, const size_t ysize)
{
    size_t x;
    size_t y;

    for (x = 0; x < xsize; x++) {
        for (y = 0; y < ysize; y++) {
            imesh2D_push_quad(mesh, vec2_new((float)(int)x, (float)(int)y));
        }
    }
}

void imesh2D_merge(iMesh2D *m1, iMesh2D *m2)
{
    size_t i;

    const size_t vsize = spxmesh_vec2_table_indices_size(&m2->vertices);
    size_t *vindices = spxmesh_vec2_table_indices(&m2->vertices);
    vec2 *v = m2->vertices.data;

    const size_t uvsize = spxmesh_vec2_table_indices_size(&m2->uvs);
    size_t *uvindices = spxmesh_vec2_table_indices(&m2->uvs);
    vec2 *uv = m2->uvs.data;

    for (i = 0; i < vsize; ++i) {
        spxmesh_vec2_table_push(&m1->vertices, v + vindices[i]);
    }

    for (i = 0; i < uvsize; ++i) {
        spxmesh_vec2_table_push(&m1->uvs, uv + uvindices[i]);
    }

    imesh2D_free(m2);
}

void imesh2D_scale(iMesh2D *mesh, const float f)
{
    vec2 *v = mesh->vertices.data;
    size_t vsize = mesh->vertices.size;
    size_t i;

    for (i = 0; i < vsize; i++) {
        v[i] = vec2_mult(v[i], f);
    }
}

void imesh2D_move(iMesh2D *mesh, const vec2 add)
{
    vec2 *v = mesh->vertices.data;
    size_t vsize = mesh->vertices.size;
    size_t i;

    for (i = 0; i < vsize; i++) {
        v[i] = vec2_add(v[i], add);
    }
}

iMesh2D mesh2D_to_imesh2D(const Mesh2D *mesh)
{
    iMesh2D m;
    m.vertices = spxmesh_vec2_table_compress(mesh->vertices.data, mesh->vertices.size);
    m.uvs = spxmesh_vec2_table_compress(mesh->uvs.data, mesh->uvs.size);
    return m;
}

#endif /* SPXMESH_APPLICATION */

#endif /* SIMPLE_PIXEL_MESH_H */
