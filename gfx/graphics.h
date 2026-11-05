#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "coms/commons.h"

struct SDL_Texture;
struct SDL_Renderer;
struct Mesh;
struct Cam;

float triangle_area(Vector2 *p1, Vector2 *p2, Vector2 *p3);
float min(float *arr, int arrlen);
float max(float *arr, int arrlen);
void shader(struct SDL_Texture *framebuffer, struct SDL_Renderer *render, Vector2 *p1, Vector2 *p2, Vector2 *p3);
Vector2 yrotate(float x, float z, float theta);
Vector2 xrotate(float y, float z, float theta);
Vector2 translate_Z(float x, float y, float z);
Vector2 translate_coord(float x, float y);
void vertex_processing(struct Mesh *mesh, struct Cam *camera, Vector3 new_verts[]);
void render(struct SDL_Texture *framebuffer, struct SDL_Renderer *renderer, struct Mesh *mesh, Vector3 new_verts[]);

#endif