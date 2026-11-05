#include <graphics.h>
#include <SDL3/SDL.h>
#include <math.h>
#include <stdlib.h>

#include "coms/commons.h"

float triangle_area(float *p1, float *p2, float *p3)
{
    return fabsf((p1[0] * (p2[1] - p3[1]) + p2[0] * (p3[1] - p1[1]) + p3[0] * (p1[1] - p2[1])) / 2);
}

float min(float *arr, int arrlen)
{
    float curr = arr[0];
    for (int i = 1; i < arrlen; ++i)
        {
            if (arr[i] < curr)
                {
                    curr = arr[i];
                }
        }
    
    return curr;
}

float max(float *arr, int arrlen)
{
    float curr = arr[0];
    for (int i = 1; i < arrlen; ++i)
        {
            if (arr[i] > curr)
                {
                    curr = arr[i];
                }
        }
    
    return curr;
}

void shader(SDL_Texture *framebuffer, SDL_Renderer *renderer, float *p1, float *p2, float *p3)
{
    void *pixels;
    int p;
    SDL_LockTexture(framebuffer, NULL, &pixels, &p);
    
    uint32_t *pix_array = (uint32_t *)pixels;
    
    float xs[3] = { p1[0], p2[0], p3[0] };
    float ys[3] = { p1[1], p2[1], p3[1] };
    float *xs_ptr;
    float *ys_ptr;
    xs_ptr = &xs[0];
    ys_ptr = &ys[0];
    float minx = min(xs_ptr, 3);
    float maxx = max(xs_ptr, 3);
    float miny = min(ys_ptr, 3);
    float maxy = max(ys_ptr, 3);
    int premx = (int)maxx - minx;
    int premy = (int)maxy - miny;
    
    float a1;
    float a2;
    float a3;
    float a4;
    
    //for (float y = miny; y < (maxy + 1.0f); ++y)
    for (float y = miny; y < maxy; ++y)
        {
            for (float x = minx; x < maxx; ++x)
                {
                    float point[2] = { x, y };
                    float *ptr;
                    ptr = &point[0];
                    a1 = triangle_area(p1, p2, p3);
                    a2 = triangle_area(ptr, p1, p2);
                    a3 = triangle_area(ptr, p2, p3);
                    a4 = triangle_area(ptr, p1, p3);
                    
                    if (fabsf((a2 + a3 + a4) - a1) < 0.1f)
                        {
                            pix_array[((int)y * 1280) + (int)x] = 0xFF0000FF;
                            //pix_array[((((int)y) + 1) * 1280) + (int)x] //= 0xFF0000FF;
                        }
                }
        }
    
    SDL_UnlockTexture(framebuffer);
    SDL_RenderTexture(renderer, framebuffer, NULL, NULL);
    
}