#include <graphics.h>
#include <SDL3/SDL.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "coms/commons.h"
#include "cam/camera.h"\

#define RADIANS(angle) ((angle) * (3.14159 / 180.0f))

float triangle_area(Vector2 *p1, Vector2 *p2, Vector2 *p3)
{
    return fabsf((p1->p[0] * (p2->p[1] - p3->p[1]) + p2->p[0] * (p3->p[1] - p1->p[1]) + p3->p[0] * (p1->p[1] - p2->p[1])) / 2);
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

void shader(SDL_Texture *framebuffer, SDL_Renderer *renderer, Vector2 *p1, Vector2 *p2, Vector2 *p3, uint32_t *pix_array)
{
    
    float xs[3] = { p1->p[0], p2->p[0], p3->p[0] };
    float ys[3] = { p1->p[1], p2->p[1], p3->p[1] };
    float *xs_ptr;
    float *ys_ptr;
    xs_ptr = &xs[0];
    ys_ptr = &ys[0];
    float minx = min(xs_ptr, 3);
    float maxx = max(xs_ptr, 3);
    float miny = min(ys_ptr, 3);
    float maxy = max(ys_ptr, 3);
    
    //a;lksdflkajfl;kajdfkl;jasl;fkd
    
    //if (minx < 0.0f || maxx > 1280.0f || miny < 0.0f || maxy > 720.0f) return;
    if (minx != minx || maxx != maxx || miny != miny || maxy != maxy) return;
    //if ((xs[0] < 0.0f || xs[0] > 1280.0f || ys[0] < 0.0f || ys[0] > 720.0f) && (xs[1] < 0.0f || xs[1] > 1280.0f || ys[1] < 0.0f || ys[1] > 720.0f) && (xs[2] < 0.0f || xs[2] > 1280.0f || ys[2] < 0.0f || ys[2] > 720.0f)) return;
    
    if (maxx < 0.0f || minx > 1280.0f || maxy < 0.0f || miny > 1280.f) return;
    
    if (minx < 0.0f) minx = 0.0f;
    if (maxx > 1280.0f) maxx = 1280.0f;
    if (miny < 0.0f) miny = 0.0f;
    if (maxy > 720.0f) maxy = 720.0f;
    
    float a1;
    float a2;
    float a3;
    float a4;
    
    a1 = triangle_area(p1, p2, p3);
    
    for (int x = (int)minx; x < (int)maxx + 1; ++x)
        {
            for (int y = (int)miny; y < (int)maxy + 1; ++y)
                {
                    
                    int pix_pos = (y * 1280) + x;
                    if (y >= 0 && y < 720 && x >= 0 && x < 1280)
                        {
                           Vector2 point = { .p = { x, y } };
                           a2 = triangle_area(&point, p1, p2);
                           a3 = triangle_area(&point, p2, p3);
                           a4 = triangle_area(&point, p1, p3);
                        
                           if (fabsf((a2 + a3 + a4) - a1) <= 0.1f)
                               {
                                   pix_array[pix_pos] = 0xFF0000FF;
                               } 
                        }
                }
        }
    
}

Vector2 yrotate(float x, float z, float theta)
{
    float c = cosf(RADIANS(theta));
    float s = sinf(RADIANS(theta));
    
    float _x = (x * c) - (z * s);
    float _z = (x * s) + (z * c);
    
    return (Vector2){ .p = { _x, _z } };
}

Vector2 xrotate(float y, float z, float theta)
{
    float c = cosf(RADIANS(theta));
    float s = sinf(RADIANS(theta));
    
    float _y = (y * c) - (z * s);
    float _z = (y * s) + (z * c);
    
    return (Vector2){ .p = { _y, _z } };
}

Vector2 translate_Z(float x, float y, float z)
{
    return (Vector2){
        .p = 
         {
             (x / z) * 250.0f,
             (y / z) * 250.0f
         }
    };
}

Vector2 translate_coord(float x, float y)
{
     return (Vector2){
        .p = 
         {
             x + (float)(1280 / 2),
             y + (float)(720 / 2)
         }
    };
}

void vertex_processing(Mesh *mesh, Cam *camera, Vector3 new_verts[])
{
    for (int i = 0; i < mesh->verts_count; ++i)
        {
            float perspx = (mesh->verts[i].x + mesh->centre.x) - (camera->pos.x);
            float perspy = (mesh->verts[i].y + mesh->centre.y) - (camera->pos.y);
            float perspz = (mesh->verts[i].z + mesh->centre.z) - (camera->pos.z);
            
            Vector2 _yrot = yrotate(perspx, perspz, camera->xrot);
            Vector2 _xrot = xrotate(perspy, _yrot.p[1], camera->yrot);
            
            new_verts[i].x = _yrot.p[0];
            new_verts[i].y = _xrot.p[0];
            new_verts[i].z = _xrot.p[1];
        }
}

LineEqu line_equation(float x1, float y1, float x2, float y2)
{
    LineEqu le;
    le.m = (y2 - y1) / (x2 - x1);
    
    le.x1 = x1;
    le.y1 = y1;
    
    return le;
}

float find_x(LineEqu *le, float y)
{    
    return (float)((y - le->y1) / le->m) + le->x1;
}

float find_y(LineEqu *le, float x)
{
    return (float)(le->m * (x - le->x1)) + le->y1;
}

Points2 get_points(Vector2 *p1, Vector2 *p2, Vector2 *op3)
{
    
    LineEqu p1_le = line_equation(p1->p[0], p1->p[1], op3->p[0], op3->p[1]);
    LineEqu p2_le = line_equation(p2->p[0], p2->p[1], op3->p[0], op3->p[1]);

    float xp1;
    float yp1;
    
    float xp2;
    float yp2;
    
    bool yset = false;
    
    if (op3->p[0] < 0.0f)
        {
            xp1 = 0.0f;
            xp2 = 0.0f;
            
            yp1 = find_y(&p1_le, xp1);
            yp2 = find_y(&p2_le, xp2);
            
            yset = true;
        }
    else if (op3->p[0] > 1280.0f)
        {
            xp1 = 1280.0f;
            xp2 = 1280.0f;
            
            yp1 = find_y(&p1_le, xp1);
            yp2 = find_y(&p2_le, xp2);
            
            yset = true;
        }
    if (op3->p[1] < 0.0f && !yset)
        {
            yp1 = 0.0f;
            yp2 = 0.0f;
            
            xp1 = find_x(&p1_le, yp1);
            xp2 = find_x(&p2_le, yp2);
        }
    else if (op3->p[1] > 720.0f && !yset)
        {
            yp1 = 720.0f;
            yp2 = 720.0f;
            
            xp1 = find_x(&p1_le, yp1);
            xp2 = find_x(&p2_le, yp2);
        }
        
    return (Points2){
        .x1 = xp1,
        .y1 = yp1,
        
        .x2 = xp2,
        .y2 = yp2  
    };
}

void render(SDL_Texture *framebuffer, SDL_Renderer *renderer, Mesh *mesh, Vector3 new_verts[])
{
    
    void *pixels;
    int p;
    SDL_LockTexture(framebuffer, NULL, &pixels, &p);
    
    uint32_t *pix_array = (uint32_t *)pixels;
    
    for (int i = 0; i < mesh->indexes_count; ++i)
        {
            if ((i + 1) % 3 == 0)
                {
                    int i0 = mesh->indexes[i];
                    int i1 = mesh->indexes[i - 1];
                    int i2 = mesh->indexes[i - 2];
                    
                    Vector3 p1 = new_verts[i0];
                    Vector3 p2 = new_verts[i1];
                    Vector3 p3 = new_verts[i2];
                    
                    Vector2 tmp1 = translate_Z(p1.x, p1.y, p1.z);
                    Vector2 r1 = translate_coord(tmp1.p[0], tmp1.p[1]);
                    
                    Vector2 tmp2 = translate_Z(p2.x, p2.y, p2.z);
                    Vector2 r2 = translate_coord(tmp2.p[0], tmp2.p[1]);
                    
                    Vector2 tmp3 = translate_Z(p3.x, p3.y, p3.z);
                    Vector2 r3 = translate_coord(tmp3.p[0], tmp3.p[1]);
                    
                    bool oob_p1 = p1.z < 0.01f;
                    bool oob_p2 = p2.z < 0.01f;
                    bool oob_p3 = p3.z < 0.01f;
                    
                    if (!(oob_p1 || oob_p2 || oob_p3))
                        {
                            shader(framebuffer, renderer, &r1, &r2, &r3, pix_array);
                        }
                    else 
                        {
                            // two points are in-bounds
                            int cond = oob_p1 + oob_p2 + oob_p3;
                            if (cond == 1)
                                {
                                    if (!oob_p1 && !oob_p2)
                                        {
                                             float t1 = (0.01f - p1.z) / (p3.z - p1.z);
                                             float t2 = (0.01f - p2.z) / (p3.z - p2.z);
                                             
                                             Vector3 inters1;
                                             Vector3 inters2;
                                             
                                             inters1.x = p1.x + t1 * (p3.x - p1.x);
                                             inters1.y = p1.y + t1 * (p3.y - p1.y);
                                             inters1.z = 0.01f;
                                             
                                             inters2.x = p2.x + t2 * (p3.x - p2.x);
                                             inters2.y = p2.y + t2 * (p3.y - p2.y);
                                             inters2.z = 0.01f;
                                             
                                             Vector2 tmp_i1 = translate_Z(inters1.x, inters1.y, inters1.z); 
                                             Vector2 i_r1 = translate_coord(tmp_i1.p[0], tmp_i1.p[1]);
                                             
                                             Vector2 tmp_i2 = translate_Z(inters2.x, inters2.y, inters2.z); 
                                             Vector2 i_r2 = translate_coord(tmp_i2.p[0], tmp_i2.p[1]);
                                             
                                             shader(framebuffer, renderer, &r1, &r2, &i_r1, pix_array);
                                             shader(framebuffer, renderer, &r2, &i_r2, &i_r1, pix_array);
                                        }
                                    else if (!oob_p2 && !oob_p3)
                                        {
                                            float t1 = (0.01f - p2.z) / (p1.z - p2.z);
                                            float t2 = (0.01f - p3.z) / (p1.z - p3.z);
                                            
                                            Vector3 inters1;
                                            Vector3 inters2;
                                         
                                            inters1.x = p2.x + t1 * (p1.x - p2.x);
                                            inters1.y = p2.y + t1 * (p1.y - p2.y);
                                            inters1.z = 0.01f;
                                             
                                            inters2.x = p3.x + t2 * (p1.x - p3.x);
                                            inters2.y = p3.y + t2 * (p1.y - p3.y);
                                            inters2.z = 0.01f;
                                             
                                            Vector2 tmp_i1 = translate_Z(inters1.x, inters1.y, inters1.z); 
                                            Vector2 i_r1 = translate_coord(tmp_i1.p[0], tmp_i1.p[1]);
                                             
                                            Vector2 tmp_i2 = translate_Z(inters2.x, inters2.y, inters2.z); 
                                            Vector2 i_r2 = translate_coord(tmp_i2.p[0], tmp_i2.p[1]);
                                             
                                            shader(framebuffer, renderer, &r2, &r3, &i_r1, pix_array);
                                            shader(framebuffer, renderer, &r2, &i_r2, &i_r1, pix_array); 
                                        }
                                    else if (!oob_p1 && !oob_p3)
                                        {
                                            float t1 = (0.01f - p1.z) / (p2.z - p1.z);
                                            float t2 = (0.01f - p3.z) / (p2.z - p3.z);
                                             
                                            Vector3 inters1;
                                            Vector3 inters2;
                                             
                                            inters1.x = p1.x + t1 * (p2.x - p1.x);
                                            inters1.y = p1.y + t1 * (p2.y - p1.y);
                                            inters1.z = 0.01f;
                                             
                                            inters2.x = p3.x + t2 * (p2.x - p3.x);
                                            inters2.y = p3.y + t2 * (p2.y - p3.y);
                                            inters2.z = 0.01f;
                                             
                                            Vector2 tmp_i1 = translate_Z(inters1.x, inters1.y, inters1.z); 
                                            Vector2 i_r1 = translate_coord(tmp_i1.p[0], tmp_i1.p[1]);
                                             
                                            Vector2 tmp_i2 = translate_Z(inters2.x, inters2.y, inters2.z); 
                                            Vector2 i_r2 = translate_coord(tmp_i2.p[0], tmp_i2.p[1]);
                                             
                                            shader(framebuffer, renderer, &r1, &r3, &i_r1, pix_array);
                                            shader(framebuffer, renderer, &r1, &i_r2, &i_r2, pix_array); 
                                        }
                                }
                            else if (cond == 2)
                                {
                                    if (!oob_p1)
                                        {
                                             float t1 = (0.01f - p1.z) / (p2.z - p1.z);
                                             float t2 = (0.01f - p1.z) / (p3.z - p1.z);
                                             
                                             Vector3 inters1;
                                             Vector3 inters2;
                                             
                                             inters1.x = p1.x + t1 * (p2.x - p1.x);
                                             inters1.y = p1.y + t1 * (p2.y - p1.y);
                                             inters1.z = 0.01f;
                                             
                                             inters2.x = p1.x + t2 * (p3.x - p1.x);
                                             inters2.y = p1.y + t2 * (p3.y - p1.y);
                                             inters2.z = 0.01f;
                                             
                                             Vector2 tmp_i1 = translate_Z(inters1.x, inters1.y, inters1.z); 
                                             Vector2 i_r1 = translate_coord(tmp_i1.p[0], tmp_i1.p[1]);
                                             
                                             Vector2 tmp_i2 = translate_Z(inters2.x, inters2.y, inters2.z); 
                                             Vector2 i_r2 = translate_coord(tmp_i2.p[0], tmp_i2.p[1]);
                                             
                                             shader(framebuffer, renderer, &r1, &i_r1, &i_r2, pix_array);
                                        }
                                    else if (!oob_p2)
                                        {
                                            float t1 = (0.01f - p2.z) / (p1.z - p2.z);
                                            float t2 = (0.01f - p2.z) / (p3.z - p2.z);
                                             
                                            Vector3 inters1;
                                            Vector3 inters2;
                                             
                                            inters1.x = p2.x + t1 * (p1.x - p2.x);
                                            inters1.y = p2.y + t1 * (p1.y - p2.y);
                                            inters1.z = 0.01f;
                                             
                                            inters2.x = p2.x + t2 * (p3.x - p2.x);
                                            inters2.y = p2.y + t2 * (p3.y - p2.y);
                                            inters2.z = 0.01f;
                                             
                                            Vector2 tmp_i1 = translate_Z(inters1.x, inters1.y, inters1.z); 
                                            Vector2 i_r1 = translate_coord(tmp_i1.p[0], tmp_i1.p[1]);
                                             
                                            Vector2 tmp_i2 = translate_Z(inters2.x, inters2.y, inters2.z); 
                                            Vector2 i_r2 = translate_coord(tmp_i2.p[0], tmp_i2.p[1]);
                                             
                                            shader(framebuffer, renderer, &r2, &i_r1, &i_r2, pix_array); 
                                        }
                                    else if (!oob_p3)
                                        {
                                            float t1 = (0.01f - p3.z) / (p1.z - p3.z);
                                            float t2 = (0.01f - p3.z) / (p2.z - p3.z);
                                             
                                            Vector3 inters1;
                                            Vector3 inters2;
                                             
                                            inters1.x = p3.x + t1 * (p1.x - p3.x);
                                            inters1.y = p3.y + t1 * (p1.y - p3.y);
                                            inters1.z = 0.01f;
                                             
                                            inters2.x = p3.x + t2 * (p2.x - p3.x);
                                            inters2.y = p3.y + t2 * (p2.y - p3.y);
                                            inters2.z = 0.01f;
                                             
                                            Vector2 tmp_i1 = translate_Z(inters1.x, inters1.y, inters1.z); 
                                            Vector2 i_r1 = translate_coord(tmp_i1.p[0], tmp_i1.p[1]);
                                             
                                            Vector2 tmp_i2 = translate_Z(inters2.x, inters2.y, inters2.z); 
                                            Vector2 i_r2 = translate_coord(tmp_i2.p[0], tmp_i2.p[1]);
                                             
                                            shader(framebuffer, renderer, &r3, &i_r1, &i_r2, pix_array); 
                                        }
                                }
                        }
                }
        }
        
    SDL_UnlockTexture(framebuffer);
    SDL_RenderTexture(renderer, framebuffer, NULL, NULL);
}