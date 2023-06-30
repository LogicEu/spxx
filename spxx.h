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

#ifndef SIMPLE_PIXEL_EXTENSION_H
#define SIMPLE_PIXEL_EXTENSION_H

/******************
****** spxe *******
Simple PiXel Engine
*******************
@Eugenio Arteaga A.
*******************

Simple Pixel Engine extension. Provides useful structures
and functions to easily render and plot primitive shapes
right out of the box.

****************************************************/

/* Simple Pixel Extension */

#ifdef SPXE_APPLICATION
#define SPXM_APPLICATION
#endif /* SPXE_APPLICATION */

#include <spxe.h>
#include <spxmath.h>

#define pxSign(n) ((n) < 0) ? -1 : !(n)
#define pxAbs(n) (((n) < 0) ? -(n) : (n))
#define pxMax(a, b) (((a) > (b)) ? (a) : (b))
#define pxMin(a, b) (((a) < (b)) ? (a) : (b))
#define pxSwap(a, b, type) do { type _tmp = a; a = b; b = _tmp; } while (0)
#define pxClamp(n, min, max) (((n) < (min)) ? (min) : (((n) > (max)) ? (max) : (n)))

typedef struct Vert2D {
    vec2 pos;
    vec2 uv;
} Vert2D;

typedef struct Vert3D {
    vec3 pos;
    vec2 uv;
} Vert3D;

typedef struct Tex2D {
    Px* fb;
    int width;
    int height;
} Tex2D;

typedef struct Glyph {
    unsigned char* pixmap;
    ivec2 size;
    ivec2 bearing;
    unsigned int advance;
} Glyph;

typedef struct Font2D {
    struct FT_FaceRec_* face;
    Glyph* glyphs;
} Font2D;

#define pxAt(tex, px, py) tex.fb[(tex.width * (py)) + (px)]
#define pxInside(tex, px, py) ((px) >= 0 && (px) < tex.width &&\
                                (py) >= 0 && (py) < tex.height)

Tex2D spxxStart(const char* title, int ww, int wh, int sw, int sh);
int spxxEnd(Tex2D texture);

Px pxLerp(Px a, Px b, float t);
void pxPlot(const Tex2D texture, int x, int y, Px color);
void pxBlend(const Tex2D texture, int x, int y, float t, Px color);
void pxPlotLine(const Tex2D texture, ivec2 p, ivec2 q, const Px color);
void pxPlotLine2(const Tex2D texture, ivec2 p, ivec2 q, const Px color);
void pxPlotLineSmooth(const Tex2D texture, vec2 p, vec2 q, const Px color);
void pxPlotBezier2(const Tex2D texture, vec2 a, vec2 b, vec2 c, const Px col);
void pxPlotBezier2Smooth(const Tex2D texture, vec2 a, vec2 b, vec2 c, const Px col);
void pxPlotBezier3(const Tex2D texture, vec2 a, vec2 b, vec2 c, vec2 d, const Px col);
void pxPlotRect(const Tex2D texture, ivec2 p, ivec2 q, const Px color);
void pxPlotTri(const Tex2D texture, ivec2 p0, ivec2 p1, ivec2 p2, const Px color);
void pxPlotTriSmooth(const Tex2D texture, vec2 p0, vec2 p1, vec2 p2, const Px c);
void pxPlotCircle(const Tex2D texture, ivec2 p, float r, const Px color);
void pxPlotCircleSmooth(const Tex2D texture, ivec2 p, float r, const Px color);

Font2D spxxFontLoad(const char* path);
void spxxFontFree(Font2D* font);
void spxxFontSize(Font2D* font, size_t size);
void spxxFontDrawGlyph(Tex2D texture, const Glyph glyph, ivec2 p, const Px color);
void spxxFontDrawText(Tex2D texture, const Font2D*, const char*, ivec2, const Px);

#ifdef SPXE_APPLICATION

/*********************
******** spxx ********
Simple PiXel Extension
**********************
*  IMPLEMENTATION    *
*********************/

#include <math.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#ifndef SPXE_SUBSAMPLES 
#define SPXE_SUBSAMPLES 2
#endif /* SPXE_SUBSAMPLES */

#ifndef SPXX_GLYPH_COUNT
#define SPXX_GLYPH_COUNT 128
#endif /* SPXX_GLYPH_COUNT */

#ifndef SPXX_FONT_SIZE 
#define SPXX_FONT_SIZE 12
#endif /* SPXX_FONT_SIZE */

static struct spxx {
    FT_Library ft;
} spxx;

Tex2D spxxStart(const char* title, int ww, int wh, int sw, int sh)
{
    Tex2D t = {NULL, 0, 0};
    if (FT_Init_FreeType(&spxx.ft)) {
        fprintf(stderr, "spxx could not init freetype\n");
        return t;
    }

    t.fb = spxeStart(title, ww, wh, sw, sh);
    if (!t.fb) {
        fprintf(stderr, "spxx coult not init spxe\n");
        return t;
    }

    t.width = sw;
    t.height = sh;
    return t;
}

int spxxEnd(Tex2D texture)
{
    FT_Done_FreeType(spxx.ft);
    return spxeEnd(texture.fb);
}

Px pxLerp(Px a, Px b, float t)
{
    return (Px) {
        (uint8_t)lerpf((float)a.r, (float)b.r, t),
        (uint8_t)lerpf((float)a.g, (float)b.g, t),
        (uint8_t)lerpf((float)a.b, (float)b.b, t),
        (uint8_t)lerpf((float)a.a, (float)b.a, t)
    };
}

void pxPlot(const Tex2D texture, int x, int y, Px color)
{ 
    if (x >= 0 && x < texture.width && y >= 0 && y < texture.height)
        pxAt(texture, x, y) = color;
}

void pxBlend(const Tex2D texture, int x, int y, float t, Px color)
{
    if (x >= 0 && x < texture.width && y >= 0 && y < texture.height)
        pxAt(texture, x, y) = pxLerp(pxAt(texture, x, y), color, t);
}

void pxPlotLine(const Tex2D texture, ivec2 p, ivec2 q, const Px color)
{
    const ivec2 dif = {q.x - p.x, q.y - p.y};
    const ivec2 d = {pxAbs(dif.x), -pxAbs(dif.y)};
    const ivec2 s = {p.x < q.x ? 1 : -1, p.y < q.y ? 1 : -1};
    int error = d.x + d.y;
    while (!(p.x == q.x && p.y == q.y)) {
        int e2 = error * 2;
        pxPlot(texture, p.x, p.y, color);
        if (e2 >= d.y) {
            error = error + d.y;
            p.x += s.x;
        }
        if (e2 <= d.x) {
            error = error + d.x;
            p.y +=s.y;
        }
    }
    pxPlot(texture, p.x, p.y, color);
}

void pxPlotLine2(const Tex2D texture, ivec2 p, ivec2 q, const Px color)
{
    const ivec2 dif = {q.x - p.x, q.y - p.y};
    const ivec2 d = {pxAbs(dif.x), pxAbs(dif.y)};
    const ivec2 s = {p.x < q.x ? 1 : -1, p.y < q.y ? 1 : -1};
    int error = d.x - d.y;
    while (!(p.x == q.x && p.y == q.y)) {
        int e2 = error * 2;
        pxPlot(texture, p.x, p.y, color);
        if (e2 > -d.y) {
            error -= d.y;
            p.x += s.x;
        }
        if (e2 < d.x) {
            error +=  d.x;
            p.y += s.y;
        }
    }
    pxPlot(texture, p.x, p.y, color);
}

void pxPlotLineSmooth(const Tex2D texture, vec2 p, vec2 q, const Px color)
{
    const int steep = pxAbs(q.y - p.y) > pxAbs(q.x - p.x);
    if (steep) {
        pxSwap(p.x, p.y, float);
        pxSwap(q.x, q.y, float);
     }
     if (p.x > q.x) {
        pxSwap(p.x, q.x, float);
        pxSwap(p.y, q.y, float);
     }

     const float dx = q.x - p.x;
     const float dy = q.y - p.y;
     const float g = (dx != 0.0F) ? dy / dx : 1.0F;

     float xend = floorf(p.x);
     float yend = p.y + g * (xend - p.x);
     float xgap = 1.0F - ((p.x + 0.5F) - floorf(p.x + 0.5F));
     const int xpxl1 = xend;
     const int ypxl1 = floorf(yend);
     float fpart = yend - (float)(ypxl1);
     float rfpart = 1.0F - fpart;

     if (steep) {
        pxBlend(texture, ypxl1, xpxl1, rfpart * xgap, color);
        pxBlend(texture, ypxl1 + 1, xpxl1, fpart * xgap, color);
     } else {
        pxBlend(texture, xpxl1, ypxl1, rfpart * xgap, color);
        pxBlend(texture, xpxl1, ypxl1 + 1, fpart * xgap, color);
     }

     float intery = yend + g;
     xend = floorf(q.x);
     yend = q.y + g * (xend - q.x);
     //xgap = 1.0F - (q.x + 0.5F - xend);
     xgap = 1.0F - ((q.x + 0.5F) - floorf(q.x + 0.5F));
     const int xpxl2 = xend;
     const int ypxl2 = floorf(yend);
     fpart = yend - (float)ypxl2;
     rfpart = 1.0F - fpart;

     if (steep) {
        pxBlend(texture, ypxl2, xpxl2, rfpart * xgap, color);
        pxBlend(texture, ypxl2 + 1, xpxl2, fpart * xgap, color);
        for (int x = xpxl1 + 1; x < xpxl2; ++x) {
            const int y = floorf(intery);
            fpart = intery - (float)y;
            rfpart = 1.0F - fpart;
            pxBlend(texture, y, x, rfpart, color);
            pxBlend(texture, y + 1, x, fpart, color);
            intery += g;
        }
     } else {
        pxBlend(texture, xpxl2, ypxl2, rfpart * xgap, color);
        pxBlend(texture, xpxl2, ypxl2 + 1, fpart * xgap, color);
        for (int x = xpxl1 + 1; x < xpxl2; ++x) {
            const int y = floorf(intery);
            fpart = intery - (float)y;
            rfpart = 1.0F - fpart;
            pxBlend(texture, x, y, rfpart, color);
            pxBlend(texture, x, y + 1, fpart, color);
            intery += g;
        }
    }
}

void pxPlotBezier2(const Tex2D texture, vec2 a, vec2 b, vec2 c, const Px col)
{
    const vec2 d = {pxAbs(b.x - a.x), pxAbs(b.y - a.y)};
    const float delta = 1.0F / (d.x + d.y);
    for (float t = 0.0; t < 1.0; t += delta) {
        vec2 p1 = vec2_lerp(a, c, t);
        vec2 p2 = vec2_lerp(c, b, t);
        ivec2 p = vec2_to_ivec2(vec2_lerp(p1, p2, t));
        pxPlot(texture, p.x, p.y, col);
    }
}

void pxPlotBezier3(const Tex2D texture, vec2 a, vec2 b, vec2 c, vec2 d, const Px col)
{
    const vec2 D = {pxAbs(d.x - a.x), pxAbs(d.y - a.y)};
    const float delta = 1.0F / (D.x + D.y);
    vec2 P[2] = {a};
    for (float t = delta; t < 1.0; t += delta) {
        float u = 1.0F - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;
        P[1] = (vec2) {
            uuu * a.x + 3.0F * uu * t * b.x + 3.0F * u * tt * c.x + ttt * d.x,
            uuu * a.y + 3.0F * uu * t * b.y + 3.0F * u * tt * c.y + ttt * d.y
        };

        pxPlotLineSmooth(texture, P[0], P[1], col);
        P[0] = P[1];
    }
    
    P[1] = d;
    pxPlotLineSmooth(texture, P[0], P[1], col);
}

void pxPlotBezier2Smooth(const Tex2D texture, vec2 a, vec2 b, vec2 c, const Px col)
{
    const vec2 d = {pxAbs(b.x - a.x), pxAbs(b.y - a.y)};
    const float delta = 1.0F / pxMax(d.x, d.y);

    vec2 P[2] = {a};
    for (float t = delta; t < 1.0; t += delta) {
        vec2 p1 = vec2_lerp(a, c, t);
        vec2 p2 = vec2_lerp(c, b, t);
        P[1] = vec2_lerp(p1, p2, t);
        pxPlotLineSmooth(texture, P[0], P[1], col);
        P[0] = P[1];
    }
    P[1] = b;
    pxPlotLineSmooth(texture, P[0], P[1], col);
}

void pxPlotRect(const Tex2D texture, ivec2 p, ivec2 q, const Px color)
{
    const ivec2 res = {texture.width - 1, texture.height - 1};
    const int starty = pxClamp(p.y - q.y, 0, res.y);
    const int endy = pxClamp(p.y + q.y, 0, res.y);
    const int startx = pxClamp(p.x - q.x, 0, res.x);
    const int endx = pxClamp(p.x + q.x, 0, res.x);
    for (int y = starty; y <= endy; ++y) {
        for (int x = startx; x <= endx; ++x) {
            pxAt(texture, x, y) = color;
        }
    }
}

static float pxEdge(vec2 a, vec2 b, vec2 c)
{
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

static void pxSortTri(ivec2 t[3])
{
    if (t[2].y < t[0].y || (t[2].y == t[0].y && t[2].x < t[0].x))
        pxSwap(t[2], t[0], ivec2);
    if (t[1].y < t[0].y || (t[1].y == t[0].y && t[1].x < t[0].x))
        pxSwap(t[1], t[0], ivec2);
    if (t[2].y < t[1].y || (t[2].y == t[1].y && t[2].x < t[1].x))
        pxSwap(t[2], t[1], ivec2);
}

static void pxSortTrif(vec2 t[3])
{
    if (t[2].y < t[0].y || (t[2].y == t[0].y && t[2].x < t[0].x))
        pxSwap(t[2], t[0], vec2);
    if (t[1].y < t[0].y || (t[1].y == t[0].y && t[1].x < t[0].x))
        pxSwap(t[1], t[0], vec2);
    if (t[2].y < t[1].y || (t[2].y == t[1].y && t[2].x < t[1].x))
        pxSwap(t[2], t[1], vec2);
}

static void pxSortTriVert2D(Vert2D t[3])
{
    if (t[2].pos.y < t[0].pos.y || (t[2].pos.y == t[0].pos.y && t[2].pos.x < t[0].pos.x))
        pxSwap(t[2], t[0], Vert2D);
    if (t[1].pos.y < t[0].pos.y || (t[1].pos.y == t[0].pos.y && t[1].pos.x < t[0].pos.x))
        pxSwap(t[1], t[0], Vert2D);
    if (t[2].pos.y < t[1].pos.y || (t[2].pos.y == t[1].pos.y && t[2].pos.x < t[1].pos.x))
        pxSwap(t[2], t[1], Vert2D);
}

void pxPlotTri(const Tex2D texture, ivec2 p0, ivec2 p1, ivec2 p2, const Px color)
{
    ivec2 t[3] = {p0, p1, p2};
    pxSortTri(t);

    const ivec2 res = {texture.width - 1, texture.height - 1};
    const int starty = pxMax(t[0].y, 0);
    const int endy = pxMin(t[2].y, res.y);
    const int difx[3] = {(t[2].x - t[0].x), (t[1].x - t[0].x), (t[2].x - t[1].x)};
    const float steps[3] = {
        t[2].y - t[0].y ? 1.0F / (float)(t[2].y - t[0].y) : 0.0F,
        t[1].y - t[0].y ? 1.0F / (float)(t[1].y - t[0].y) : 0.0F,
        t[2].y - t[1].y ? 1.0F / (float)(t[2].y - t[1].y) : 0.0F
    };

    for (int y = starty, n = 1; y <= endy; ++y) {
        n += n == 1 && y >= t[1].y; 
        float dx = steps[0] != 0.0F ? steps[0] * (y - t[0].y) : 1.0F;
        float dy = steps[n] != 0.0F ? steps[n] * (y - t[n - 1].y) : 1.0F;
        int x0 = t[0].x + (float)difx[0] * dx; 
        int x1 = t[n - 1].x + (float)difx[n] * dy;
        if (x1 < x0)
            pxSwap(x0, x1, int);
        
        const int startx = pxMax(x0, 0);
        const int endx = pxMin(x1, res.x);
        for (int x = startx; x <= endx; ++x)
            pxAt(texture, x, y) = color;
    }
}

void pxPlotTriSmooth(const Tex2D texture, vec2 p0, vec2 p1, vec2 p2, const Px color)
{
    static const vec2 P[] = {
        {0.0F, 0.0F}, {1.0F, 0.0F}, {0.0F, -1.0F}, {-1.0F, 0.0F}, {0.0F, 1.0F}
        ,{1.0F, 1.0F}, {1.0F, -1.0F}, {-1.0F, -1.0F}, {-1.0F, 1.0F}
    };
    
    const float ni = 1.0F / (sizeof(P) / sizeof(P[0]));
    vec2 t[3] = {p0, p1, p2}, T[3] = {p0, p1, p2};
    pxSortTrif(t);

    const ivec2 res = {texture.width - 1, texture.height - 1};
    const int starty = pxMax(t[0].y, 0);
    const int endy = pxMin(t[2].y, res.y);
    const float difx[3] = {(t[2].x - t[0].x), (t[1].x - t[0].x), (t[2].x - t[1].x)};
    const float steps[3] = {
        t[2].y - t[0].y ? 1.0F / (t[2].y - t[0].y) : 0.0F,
        t[1].y - t[0].y ? 1.0F / (t[1].y - t[0].y) : 0.0F,
        t[2].y - t[1].y ? 1.0F / (t[2].y - t[1].y) : 0.0F
    };

    for (int y = starty, n = 1; y <= (int)endy; ++y) {
        n += n == 1 && y >= t[1].y; 
        float d0 = steps[0] != 0.0F ? steps[0] * ((float)y - t[0].y) : 1.0F;
        float d1 = steps[n] != 0.0F ? steps[n] * ((float)y - t[n - 1].y) : 1.0F;
        float x0 = t[0].x + difx[0] * pxMax(d0, 0.0F); 
        float x1 = t[n - 1].x + difx[n] * pxMax(d1, 0.0F);
        if (x1 < x0)
            pxSwap(x0, x1, float);
        
        const int startx = pxMax(x0, 0);
        const int endx = pxMin(x1 + 1, res.x);
        for (int x = startx; x < endx; ++x) {
            float sum = 0.0F;
            vec2 p = {(float)x, (float)y};
            for (unsigned long j = 0; j < sizeof(P) / sizeof(P[0]); ++j) {
                vec2 q = {p.x + 0.5F * P[j].x, p.y + 0.5F * P[j].y};
                sum += (float)( pxEdge(T[1], T[2], q) >= 0.0F &&
                                pxEdge(T[2], T[0], q) >= 0.0F &&
                                pxEdge(T[0], T[1], q) >= 0.0F);
            }
            pxAt(texture, x, y) = pxLerp(pxAt(texture, x, y), color, sum * ni);
        }
    }
}

Px pxTexMap(const Tex2D texture, vec2 uv)
{
    float u = uv.x - (float)(int)uv.x, v = uv.y - (float)(int)uv.y;
    float x = (float)texture.width * u, y = (float)texture.height * v;
    return pxAt(texture, (int)x, (int)y);
}

void pxMapTri2D(const Tex2D fb, const Tex2D texture, Vert2D p0, Vert2D p1, Vert2D p2)
{
    static const vec2 P[] = {
        {0.0F, 0.0F}, {1.0F, 0.0F}, {0.0F, -1.0F}, {-1.0F, 0.0F}, {0.0F, 1.0F}
        ,{1.0F, 1.0F}, {1.0F, -1.0F}, {-1.0F, -1.0F}, {-1.0F, 1.0F}
    };
    
    const float ni = 1.0F / (sizeof(P) / sizeof(P[0]));
    Vert2D t[3] = {p0, p1, p2}, T[3] = {p0, p1, p2};
    pxSortTriVert2D(t);

    const ivec2 res = {fb.width - 1, fb.height - 1};
    const int starty = pxMax(t[0].pos.y, 0);
    const int endy = pxMin(t[2].pos.y, res.y);
    const float difx[3] = {
        (t[2].pos.x - t[0].pos.x),
        (t[1].pos.x - t[0].pos.x),
        (t[2].pos.x - t[1].pos.x)
    };
    
    const vec2 difuv[3] = {
        {(t[2].uv.x - t[0].uv.x), (t[2].uv.y - t[0].uv.y)},
        {(t[1].uv.x - t[0].uv.x), (t[1].uv.y - t[0].uv.y)},
        {(t[2].uv.x - t[1].uv.x), (t[2].uv.y - t[1].uv.y)}
    };

    const float steps[3] = {
        t[2].pos.y - t[0].pos.y ? 1.0F / (t[2].pos.y - t[0].pos.y) : 0.0F,
        t[1].pos.y - t[0].pos.y ? 1.0F / (t[1].pos.y - t[0].pos.y) : 0.0F,
        t[2].pos.y - t[1].pos.y ? 1.0F / (t[2].pos.y - t[1].pos.y) : 0.0F
    };

    for (int y = starty, n = 1; y <= (int)endy; ++y) {
        n += n == 1 && y >= t[1].pos.y; 
        float d0 = steps[0] != 0.0F ? steps[0] * ((float)y - t[0].pos.y) : 1.0F;
        float d1 = steps[n] != 0.0F ? steps[n] * ((float)y - t[n - 1].pos.y) : 1.0F;
        float x0 = t[0].pos.x + difx[0] * pxMax(d0, 0.0F); 
        float x1 = t[n - 1].pos.x + difx[n] * pxMax(d1, 0.0F);
        vec2 uv0 = {
            t[0].uv.x + difuv[0].x * pxMax(d0, 0.0F),
            t[0].uv.y + difuv[0].y * pxMax(d0, 0.0F)
        };

        vec2 uv1 = {
            t[n - 1].uv.x + difuv[n].x * pxMax(d1, 0.0F),
            t[n - 1].uv.y + difuv[n].y * pxMax(d1, 0.0F)
        };

        if (x1 < x0) {
            pxSwap(x0, x1, float);
            pxSwap(uv0, uv1, vec2);
        }
        
        const int startx = pxMax(x0, 0);
        const int endx = pxMin(x1 + 1, res.x);
        for (int x = startx; x < endx; ++x) {
            float t = ilerpf(x0, x1, (float)x);
            vec2 uv = vec2_lerp(uv0, uv1, t);
            float sum = 0.0F;
            vec2 p = {(float)x, (float)y};
            for (unsigned long j = 0; j < sizeof(P) / sizeof(P[0]); ++j) {
                vec2 q = {p.x + 0.5F * P[j].x, p.y + 0.5F * P[j].y};
                sum += (float)( pxEdge(T[1].pos, T[2].pos, q) >= 0.0F &&
                                pxEdge(T[2].pos, T[0].pos, q) >= 0.0F &&
                                pxEdge(T[0].pos, T[1].pos, q) >= 0.0F);
            }
            const Px color = pxTexMap(texture, uv);
            pxBlend(fb, x, y, ni * sum, color);
        }
    }
}

void pxPlotCircle(const Tex2D texture, ivec2 p, float r, const Px color)
{
    const float sqr = r * r;
    const ivec2 res = {texture.width - 1, texture.height - 1};
    const int startx = pxClamp(p.x - r, 0, res.x);
    const int starty = pxClamp(p.y - r, 0, res.y);
    const int endx = pxClamp(p.x + r + 1.0F, 0, res.x);
    const int endy = pxClamp(p.y + r + 1.0F, 0, res.y);
    for (int y = starty; y <= endy; ++y) {
        float dy = p.y - y + 0.5F;
        dy *= dy;
        for (int x = startx + 1; x <= endx; ++x) {
            float dx = p.x - x + 0.5F;
            if (dx * dx + dy <= sqr)
                pxAt(texture, x, y) = color;
        }
    }
}

void pxPlotCircleSmooth(const Tex2D texture, ivec2 p, float r, const Px color)
{
    static const float subscale = 1.0F / (float)SPXE_SUBSAMPLES;
    const float sqr = r * r;
    const ivec2 res = {texture.width - 1, texture.height - 1};
    const int startx = pxClamp(p.x - r - 1.0F, 0, res.x);
    const int starty = pxClamp(p.y - r - 1.0F, 0, res.y);
    const int endx = pxClamp(p.x + r + 1.0F, 0, res.x);
    const int endy = pxClamp(p.y + r + 1.0F, 0, res.y);
    for (int y = starty; y <= endy; ++y) {
        const float dy = p.y - (float)y + 0.5F;
        for (int x = startx + 1; x <= endx; ++x) {
            int count = 0;
            const float dx = p.x - (float)x + 0.5F;
            for (int sy = 0; sy < SPXE_SUBSAMPLES; ++sy) {
                float sdy = dy + (float)sy * subscale;
                sdy *= sdy;
                for (int sx = 0; sx < SPXE_SUBSAMPLES; ++sx) {
                    float sdx = dx + (float)sx * subscale;
                    count += (sdx * sdx + sdy) <= sqr;
                }
            }
            float n = (float)count / (float)(SPXE_SUBSAMPLES * SPXE_SUBSAMPLES);
            pxBlend(texture, x, y, n, color);
        }
    }
}

void spxxFontSize(Font2D* font, size_t size)
{
    FT_Set_Pixel_Sizes(font->face, 0, size);
    for (unsigned char c = 0; c < SPXX_GLYPH_COUNT; ++c) {
        if (FT_Load_Char(font->face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "spxx failed to load glyph: %c - $%d\n", c, (int)c);
            continue;
        }

        size_t len = font->face->glyph->bitmap.width * font->face->glyph->bitmap.rows;
        font->glyphs[c].pixmap = realloc(font->glyphs[c].pixmap, len);
        font->glyphs[c].size.x = font->face->glyph->bitmap.width;
        font->glyphs[c].size.y = font->face->glyph->bitmap.rows;
        font->glyphs[c].bearing.x = font->face->glyph->bitmap_left;
        font->glyphs[c].bearing.y = font->face->glyph->bitmap_top;
        font->glyphs[c].advance = font->face->glyph->advance.x;
        memcpy(font->glyphs[c].pixmap, font->face->glyph->bitmap.buffer, len);
    }
}

void spxxFontFree(Font2D* font)
{
    if (font->glyphs) {
        for (unsigned char c = 0; c < SPXX_GLYPH_COUNT; ++c) {
            if (font->glyphs[c].pixmap) {
                free(font->glyphs[c].pixmap);
                font->glyphs[c].pixmap = NULL;
            }
        }
        free(font->glyphs);
        font->glyphs = NULL;
    }
    FT_Done_Face(font->face);
}

Font2D spxxFontLoad(const char* path)
{ 
    Font2D font;
    font.glyphs = NULL;
    if (FT_New_Face(spxx.ft, path, 0, &font.face)) {
        fprintf(stderr, "spxx failed to load font: %s\n", path);
        return font;
    }

    font.glyphs = calloc(sizeof(Glyph), SPXX_GLYPH_COUNT);
    spxxFontSize(&font, SPXX_FONT_SIZE);
    return font;
}

void spxxFontDrawGlyph(Tex2D texture, const Glyph glyph, ivec2 p, const Px color)
{
    const ivec2 end = {p.x + glyph.size.x, p.y + glyph.size.y};
    for (int y = p.y; y < end.y; ++y) {
        for (int x = p.x; x < end.x; ++x) {
            Px* px = &pxAt(texture, x, y);
            int index = (glyph.size.y - 1 - (y - p.y)) * glyph.size.x + (x - p.x);
            *px = pxLerp(*px, color, (float)glyph.pixmap[index] / 255.0F);
        }
    }
}

void spxxFontDrawText(  Tex2D texture,  const Font2D* font, const char* text, 
                        ivec2 p,        const Px color)
{
    for (int i = 0; text[i]; i++) {
        spxxFontDrawGlyph(texture, font->glyphs[(int)text[i]], p, color);
        p.x += (font->glyphs[(int)text[i]].advance >> 6);
    }
}

#endif /* SPXE_APPLICATION */
#endif /* SIMPLE_PIXEL_EXTENSION_H */

