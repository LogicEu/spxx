/*

Copyright (c) 2026 Eugenio Arteaga A.

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

#ifndef SIMPLE_PIXEL_UI_H
#define SIMPLE_PIXEL_UI_H

/******************
****** spxui ******
  Simple PiXel UI
*******************
 @Eugenio Arteaga A.
*******************

Simple pixel UI toolkit built on top of spxplot, spxfont
and spxe input. It provides immediate mode widgets that
render directly to a Tex2D framebuffer.

****************************************************/

/* Simple PiXel UI */

#include <spxx/spxplot.h>
#include <spxx/spxfont.h>
#include <spxx/spxe.h>

typedef struct SpxUITheme {
    Px window_bg;
    Px panel_bg;
    Px text;
    Px widget;
    Px widget_hot;
    Px widget_active;
    Px widget_mark;
    Px slider_track;
} SpxUITheme;

typedef struct SpxUIContext {
    Tex2D target;
    const Font2D* font;
    SpxUITheme theme;
    ivec2 mouse;
    int mouse_down;
    int mouse_pressed;
    int mouse_released;
    unsigned int hot_id;
    unsigned int active_id;
} SpxUIContext;

SpxUITheme  spxuiThemeDefault(void);
void        spxuiBegin(SpxUIContext* ui, Tex2D target, const Font2D* font);
void        spxuiClear(SpxUIContext* ui);
void        spxuiPanel(SpxUIContext* ui, ivec2 p, ivec2 size);
void        spxuiLabel(SpxUIContext* ui, const char* text, ivec2 p);
int         spxuiButton(SpxUIContext* ui, const char* id, const char* text, ivec2 p, ivec2 size);
int         spxuiCheckbox(SpxUIContext* ui, const char* id, const char* text, ivec2 p, int* value);
int         spxuiSlider(SpxUIContext* ui, const char* id, ivec2 p, int width, float min, float max, float* value);

#ifdef SPXUI_APPLICATION

/******************
****** spxui ******
  Simple PiXel UI
*******************
* IMPLEMENTATION  *
*******************/

#include <string.h>

static unsigned int spxuiHash(const char* text)
{
    unsigned int hash = 2166136261u;
    while (*text) {
        hash ^= (unsigned int)(unsigned char)(*text);
        hash *= 16777619u;
        ++text;
    }
    return hash ? hash : 1u;
}

static int spxuiContains(ivec2 p, ivec2 pos, ivec2 size)
{
    return p.x >= pos.x && p.x < pos.x + size.x &&
           p.y >= pos.y && p.y < pos.y + size.y;
}

static void spxuiFillRect(Tex2D tex, ivec2 p, ivec2 size, Px color)
{
    ivec2 center;
    ivec2 half;
    center.x = p.x + (size.x >> 1);
    center.y = p.y + (size.y >> 1);
    half.x = size.x >> 1;
    half.y = size.y >> 1;
    pxPlotRect(tex, center, half, color);
}

SpxUITheme spxuiThemeDefault(void)
{
    SpxUITheme theme;
    theme.window_bg.r = 18; theme.window_bg.g = 21; theme.window_bg.b = 26; theme.window_bg.a = 255;
    theme.panel_bg.r = 28; theme.panel_bg.g = 33; theme.panel_bg.b = 40; theme.panel_bg.a = 255;
    theme.text.r = 235; theme.text.g = 235; theme.text.b = 235; theme.text.a = 255;
    theme.widget.r = 52; theme.widget.g = 63; theme.widget.b = 76; theme.widget.a = 255;
    theme.widget_hot.r = 72; theme.widget_hot.g = 89; theme.widget_hot.b = 105; theme.widget_hot.a = 255;
    theme.widget_active.r = 95; theme.widget_active.g = 125; theme.widget_active.b = 155; theme.widget_active.a = 255;
    theme.widget_mark.r = 145; theme.widget_mark.g = 208; theme.widget_mark.b = 118; theme.widget_mark.a = 255;
    theme.slider_track.r = 40; theme.slider_track.g = 45; theme.slider_track.b = 55; theme.slider_track.a = 255;
    return theme;
}

void spxuiBegin(SpxUIContext* ui, Tex2D target, const Font2D* font)
{
    ui->target = target;
    ui->font = font ? font : &spxFontDefault;
    ui->theme = spxuiThemeDefault();
    spxeMousePos(&ui->mouse.x, &ui->mouse.y);
    ui->mouse_down = spxeMouseDown(MOUSE_LEFT);
    ui->mouse_pressed = spxeMousePressed(MOUSE_LEFT);
    ui->mouse_released = spxeMouseReleased(MOUSE_LEFT);
    ui->hot_id = 0u;
}

void spxuiClear(SpxUIContext* ui)
{
    ivec2 p;
    ivec2 s;
    p.x = 0;
    p.y = 0;
    s.x = ui->target.width;
    s.y = ui->target.height;
    spxuiFillRect(ui->target, p, s, ui->theme.window_bg);
}

void spxuiPanel(SpxUIContext* ui, ivec2 p, ivec2 size)
{
    spxuiFillRect(ui->target, p, size, ui->theme.panel_bg);
}

void spxuiLabel(SpxUIContext* ui, const char* text, ivec2 p)
{
    spxFontDrawText(ui->target, ui->font, text, p, ui->theme.text);
}

int spxuiButton(SpxUIContext* ui, const char* id, const char* text, ivec2 p, ivec2 size)
{
    unsigned int wid;
    int hovered;
    int clicked;
    Px color;
    ivec2 text_pos;

    wid = spxuiHash(id);
    hovered = spxuiContains(ui->mouse, p, size);
    clicked = 0;
    color = ui->theme.widget;

    if (hovered) {
        ui->hot_id = wid;
        color = ui->theme.widget_hot;
        if (ui->mouse_pressed || (ui->mouse_down && ui->active_id == wid)) {
            ui->active_id = wid;
            color = ui->theme.widget_active;
        }
    }

    if (!ui->mouse_down && ui->active_id == wid) {
        clicked = hovered;
        ui->active_id = 0u;
    }

    spxuiFillRect(ui->target, p, size, color);
    text_pos.x = p.x + 4;
    text_pos.y = p.y + (size.y >> 1) - 3;
    spxFontDrawText(ui->target, ui->font, text, text_pos, ui->theme.text);
    return clicked;
}

int spxuiCheckbox(SpxUIContext* ui, const char* id, const char* text, ivec2 p, int* value)
{
    ivec2 box_size;
    ivec2 label_pos;
    ivec2 button_size;
    int changed;
    box_size.x = 10;
    box_size.y = 10;
    button_size = box_size;
    changed = spxuiButton(ui, id, "", p, button_size);

    if (changed) {
        *value = !(*value);
    }

    if (*value) {
        ivec2 mark_p;
        ivec2 mark_s;
        mark_p.x = p.x + 2;
        mark_p.y = p.y + 2;
        mark_s.x = 6;
        mark_s.y = 6;
        spxuiFillRect(ui->target, mark_p, mark_s, ui->theme.widget_mark);
    }

    label_pos.x = p.x + box_size.x + 4;
    label_pos.y = p.y + 2;
    spxFontDrawText(ui->target, ui->font, text, label_pos, ui->theme.text);
    return changed;
}

int spxuiSlider(SpxUIContext* ui, const char* id, ivec2 p, int width, float min, float max, float* value)
{
    unsigned int wid;
    ivec2 area_size;
    int hovered;
    int changed;
    float n;
    float range;
    int local_x;
    float t;
    int handle_x;
    ivec2 handle_p;
    ivec2 handle_s;

    wid = spxuiHash(id);
    area_size.x = width;
    area_size.y = 10;
    if (width < 8) {
        width = 8;
        area_size.x = width;
    }
    hovered = spxuiContains(ui->mouse, p, area_size);
    changed = 0;

    if (hovered) {
        ui->hot_id = wid;
        if (ui->mouse_pressed || (ui->mouse_down && ui->active_id == wid)) {
            ui->active_id = wid;
        }
    }

    if (ui->active_id == wid && ui->mouse_down) {
        local_x = ui->mouse.x - p.x;
        if (local_x < 0) {
            local_x = 0;
        }
        if (local_x > (width - 1)) {
            local_x = width - 1;
        }
        t = (float)local_x / (float)(width - 1);
        *value = min + t * (max - min);
        changed = 1;
    }

    if (!ui->mouse_down && ui->active_id == wid) {
        ui->active_id = 0u;
    }

    if (max == min) {
        n = 0.0F;
    } else {
        n = (*value - min) / (max - min);
    }
    if (n < 0.0F) {
        n = 0.0F;
    }
    if (n > 1.0F) {
        n = 1.0F;
    }

    range = (float)(width - 8);
    handle_x = p.x + (int)(n * range);

    spxuiFillRect(ui->target, p, area_size, ui->theme.slider_track);

    handle_p.x = handle_x;
    handle_p.y = p.y;
    handle_s.x = 8;
    handle_s.y = 10;

    if (ui->active_id == wid) {
        spxuiFillRect(ui->target, handle_p, handle_s, ui->theme.widget_active);
    } else if (hovered) {
        spxuiFillRect(ui->target, handle_p, handle_s, ui->theme.widget_hot);
    } else {
        spxuiFillRect(ui->target, handle_p, handle_s, ui->theme.widget);
    }

    return changed;
}

#endif /* SPXUI_APPLICATION */
#endif /* SIMPLE_PIXEL_UI_H */
