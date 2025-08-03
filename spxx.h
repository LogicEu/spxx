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

#ifndef SPXF_APPLICATION
#define SPXF_APPLICATION
#endif /* SPXF_APPLICATION */

#endif /* SPXX_APPLICATION */

#include <spxx/spxe.h>
#include <spxx/spxmath.h>
#include <spxx/spxplot.h>
#include <spxx/spxfont.h>
#include <spxx/spximg.h>

#define spxxRun(tex) spxeRun((tex).pixbuf)
#define spxxEnd(tex) spxeEnd((tex).pixbuf)
#define spxxStart(title, ww, wh, sw, sh) (Tex2D){spxeStart(title, ww, wh, sw, sh), sw, sh}

#ifdef SPXX_APPLICATION

/*********************
******** spxx ********
Simple PiXel Extension
**********************
*  IMPLEMENTATION    *
*********************/

#endif /* SPXX_APPLICATION */
#endif /* SIMPLE_PIXEL_EXTENSION_H */

