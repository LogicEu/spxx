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

/*********************
******** spxx ********
Simple PiXel Extension
**********************
   @Eugenio Arteaga A.
**********************

Simple Pixel Engine extension. Provides access to the
entire collection of simple pixel headers in a single
include statement.

****************************************************/

/* Simple Pixel Extension */

#ifdef SPXX_APPLICATION

#ifndef SPXM_APPLICATION
#define SPXM_APPLICATION
#endif /* SPXM_APPLICATION */

#ifndef SPXE_APPLICATION
#define SPXE_APPLICATION
#endif /* SPXE_APPLICATION */

#ifndef SPXI_APPLICATION
#define SPXI_APPLICATION
#endif /* SPXI_APPLICATION */

#ifndef SPXP_APPLICATION
#define SPXP_APPLICATION
#endif /* SPXP_APPLICATION */

#endif /* SPXX_APPLICATION */

#include <spxmath.h>
#include <spxplot.h>
#include <spximg.h>
#include <spxe.h>

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

#define spxxRun(tex) spxeRun((tex).pixbuf)

Tex2D   spxxStart(const char* title, int ww, int wh, int sw, int sh);
int     spxxEnd(Tex2D texture);

Font2D  spxxFontLoad(const char* path);
void    spxxFontFree(Font2D* font);
void    spxxFontSize(Font2D* font, size_t size);
int     spxxFontDrawGlyph(Tex2D texture, const Glyph glyph, ivec2 p, const Px color);
int     spxxFontDrawText(Tex2D texture, const Font2D*, const char*, ivec2, const Px);

#ifdef SPXX_APPLICATION

/*********************
******** spxx ********
Simple PiXel Extension
**********************
*  IMPLEMENTATION    *
*********************/

#include <ft2build.h>
#include FT_FREETYPE_H

#ifndef SPXX_GLYPH_COUNT
#define SPXX_GLYPH_COUNT 128
#endif /* SPXX_GLYPH_COUNT */

#ifndef SPXX_FONT_SIZE 
#define SPXX_FONT_SIZE 12
#endif /* SPXX_FONT_SIZE */

/* Core engine functions */

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

    t.pixbuf = spxeStart(title, ww, wh, sw, sh);
    if (!t.pixbuf) {
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
    return spxeEnd(texture.pixbuf);
}

void spxxFontSize(Font2D* font, size_t size)
{
    size_t len;
    unsigned char c;
    FT_Set_Pixel_Sizes(font->face, 0, size);

    for (c = 0; c < SPXX_GLYPH_COUNT; ++c) {
        if (FT_Load_Char(font->face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "spxx failed to load glyph: %c - $%d\n", c, (int)c);
            continue;
        }

        len = font->face->glyph->bitmap.width * font->face->glyph->bitmap.rows;
        font->glyphs[c].pixmap = (unsigned char*)realloc(font->glyphs[c].pixmap, len);
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
        unsigned char c;
        for (c = 0; c < SPXX_GLYPH_COUNT; ++c) {
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

    font.glyphs = (Glyph*)calloc(sizeof(Glyph), SPXX_GLYPH_COUNT);
    spxxFontSize(&font, SPXX_FONT_SIZE);
    return font;
}

int spxxFontDrawGlyph(Tex2D texture, const Glyph glyph, ivec2 p, const Px color)
{
    int x, y, ret = 0;
    const int resx = texture.width - 1, resy = texture.height - 1;
    const int endx = p.x + glyph.size.x, endy = p.y + glyph.size.y;
    ret = p.y < 0 || p.x < 0;
    for (y = p.y; y < endy; ++y) {
        ret += y > resy;
        for (x = p.x; x < endx; ++x) {
            Px* px = &pxAt(texture, x, y);
            int index = (glyph.size.y - 1 - (y - p.y)) * glyph.size.x + (x - p.x);
            *px = pxLerp(*px, color, (float)glyph.pixmap[index] / 255.0F);
            ret += x > resx;
        }
    }
    return ret;
}

int spxxFontDrawText(  Tex2D texture,  const Font2D* font, const char* text, 
                        ivec2 p,        const Px color)
{
    int i, ret = 0;
    for (i = 0; text[i]; i++) {
        ret += spxxFontDrawGlyph(texture, font->glyphs[(int)text[i]], p, color);
        p.x += (font->glyphs[(int)text[i]].advance >> 6);
        if (p.x >= texture.width) {
            ++ret;
            break;
        }
    }
    return ret;
}

#endif /* SPXE_APPLICATION */
#endif /* SIMPLE_PIXEL_EXTENSION_H */

