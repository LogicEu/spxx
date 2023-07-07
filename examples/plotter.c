#define SPXE_APPLICATION
#include <spxx.h>
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

static ivec2 pxMousePos(void)
{
    ivec2 m;
    spxeMousePos(&m.x, &m.y);
    return m;
}

void pxPlotLineSmooth2(const Tex2D texture, vec2 p, vec2 q, const Px color)
{
    int x, y;
    for (y = -1; y <= 1; ++y) {
        for (x = -1; x <= 1; ++x) {
            vec2 P, Q;
            P.x = p.x + (float)x;
            P.y = p.y + (float)y;
            Q.x = q.x + (float)x;
            Q.y = q.y + (float)y;
            pxPlotLineSmooth(texture, P, Q, color);
        }
    }
}

void pxPlotBezier2Smooth2(const Tex2D texture, vec2 a, vec2 b, vec2 c, const Px col)
{
    float t;
    vec2 P[2];
    const float dx = SPXM_ABS(b.x - a.x), dy = SPXM_ABS(b.y - a.y);
    const float dxy = SPXM_MAX(dx, dy);
    const float delta = dxy != 0.0F ? 4.0F / dxy : 1.0F;

    P[0] = a;

    for (t = delta; t < 1.0; t += delta) {
        vec2 p1, p2;
        p1 = vec2_lerp(a, c, t);
        p2 = vec2_lerp(c, b, t);
        P[1] = vec2_lerp(p1, p2, t);
        pxPlotLineSmooth2(texture, P[0], P[1], col);
        P[0] = P[1];
    }
    
    P[1] = b;
    pxPlotLineSmooth2(texture, P[0], P[1], col);
}

int main(const int argc, const char** argv)
{
    const Px red = {255, 0, 0, 255}, green = {0, 255, 0, 255};
    const Px blue = {0, 0, 255, 255}, purple = {255, 0, 255, 255};
    
    ivec2 m, p, q = {0, 0};
    vec2 c, mf;
    Tex2D fb;
    size_t size;

    int w = WIDTH, h = HEIGHT, s = 0, t = 1, i;
    for (i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            int *n = NULL;
            if (argv[i][1] == 'w' && !argv[i][2]) {
                n = &w;
            } else if (argv[i][1] == 'h' && !argv[i][2]) {
                n = &h;
            } else if (argv[i][1] == 's' && !argv[i][2]) {
                n = &s;
            } else {
                fprintf(stderr, "unknown option %s\n", argv[i]);
                return EXIT_FAILURE;
            }

            if (n != NULL) {
                if (i + 1 == argc) {
                    fprintf(stderr, "missing argument for option %s\n", argv[i]);
                    return EXIT_FAILURE;
                }
                
                if ((*n = atoi(argv[++i])) < 1) {
                    fprintf(
                        stderr,
                        "invalid value of %s for option %s\n",
                        argv[i],
                        argv[i - 1]
                    );
                    return EXIT_FAILURE;
                }
            }
        }
    }

    if (s) {
        w = WIDTH / s;
        h = HEIGHT / s;
    }

    fb = spxxStart("spxext", WIDTH, HEIGHT, w, h);
    p.x = fb.width / 2;
    p.y = fb.height / 2;
    c.x = (float)spxe.scrres.width * 0.5F;
    c.y = (float)spxe.scrres.height * 0.5F;
    size = spxe.scrres.width * spxe.scrres.height * 4;
    
    while (spxeRun(fb.fb)) {
        
        if (spxeKeyPressed(ESCAPE)) {
            break;
        }
        if (spxeKeyPressed(SPACE)) {
            t = !t;
        }
        
        m = pxMousePos();
        memset(fb.fb, 0, size);
        
        if (t) {
            pxPlotTriSmooth(
                fb, 
                vec2_from_ivec2(p), 
                vec2_from_ivec2(q),
                vec2_from_ivec2(m),
                red
            );
        } else {
            pxPlotTri(fb, p, q, m, red);
        }
        
        mf = vec2_from_ivec2(m);
        pxPlotLineSmooth(fb, c, mf, green);
        pxPlotCircleSmooth(fb, m, 20.0F, blue);
        pxPlotRect(fb, p, m, purple);
        pxPlotBezier2Smooth2(fb, c, vec2_from_ivec2(q), mf, green);
        pxPlotLineSmooth2(fb, c, mf, blue);
    }

    return spxxEnd(fb);
}
