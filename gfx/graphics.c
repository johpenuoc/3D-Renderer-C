#include <graphics.h>
#include <SDL3/SDL.h>
#include <math.h>
#include <stdlib.h>

#include "coms/commons.h"

#define RADIANS(angle) ((angle) * (3.14159 / 180.0f))

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
    
    for (float x = minx; x < maxx; ++x)
        {
            for (float y = miny; y < maxy; ++y)
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
                        }
                }
        }
    
    SDL_UnlockTexture(framebuffer);
    SDL_RenderTexture(renderer, framebuffer, NULL, NULL);
    
}

float yrotate(float x, float z, float theta)
{
    float c = cosf(RADIANS(theta));
    float s = sinf(RADIANS(theta));
    
    float _x = (x * c) - (z * c);
    float _z = (x * s) - (z * c);
    
    return _x, _z;
}

float xrotate(float y, float z, float theta)
{
    float c = cosf(RADIANS(theta));
    float s = sinf(RADIANS(theta));
    
    float _y = (y * c) - (z * c);
    float _z = (y * s) - (z * c);
    
    return _y, _z;
}

Vector2 translate_Z(float x, float y, float z)
{
    return (Vector2){
        .p = 
         {
             (x / z) * 150.0f,
             (y / z) * 150.0f
         }
    };
}

Vector2 translate_coord(float x, float y)
{
     return (Vector2){
        .p = 
         {
             x + (1280 / 2),
             y + (720 / 2)
         }
    };
}

void vertex_processing(Mesh *mesh, Cam *camera, Vector3 new_verts[])
{
    for (int i = 0; i < mesh->verts_count; ++i)
        {
            float perspx = camera->pos[0] - mesh->verts[i][0] + mesh->centre[0];
            float perspy = camera->pos[1] - mesh->verts[i][1] + mesh->centre[1];
            float perspz = camera->pos[2] - mesh->verts[i][2] + mesh->centre[2];
            
            float x, z = yrotate(perspx, perspz, camera->xrot);
            float y, z = xrotate(perspy, z, camera->yrot);
            
            new_verts[i] = { x, y, z };
        }
}

void render(SDL_Texture *framebuffer, SDL_Renderer *renderer, Mesh *mesh, Vector3 new_verts[])
{
    
    for (int i = 0; i < mesh->indexes_count; ++i)
        {
            if ((i + 1) % 3 == 0)
                {
                    int i0 = mesh->index[i];
                    int i1 = mesh->index[i - 1];
                    int i2 = mesh->index[i - 2];
                    
                    Vector3 p1 = new_verts[i0];
                    Vector2 tmp1 = translate_Z(p1[0], p1[1], p1[2]);
                    Vector2 r1 = translate_coord(tmp1.p[0], tmp1.p[1]);
                    
                    Vector3 p2 = new_verts[i1];
                    Vector2 tmp2 = translate_Z(p2[0], p2[1], p2[2]);
                    Vector2 r2 = translate_coord(tmp2.p[0], tmp2.p[1]);
                    
                    Vector3 p3 = new_verts[i2];
                    Vector2 tmp3 = translate_Z(p3[0], p3[1], p3[2]);
                    Vector2 r3 = translate_coord(tmp3.p[0], tmp3.p[1]);
                    
                    shader(framebuffer, renderer, &r1, &r2, &r3)
                    
                }
        }
}