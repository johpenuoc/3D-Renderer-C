#ifndef GRAPHICS_H
#define GRAPHICS_H

struct SDL_Texture;
struct SDL_Renderer;

float triangle_area(float *p1, float *p2, float *p3);
float min(float *arr, int arrlen);
float max(float *arr, int arrlen);
void shader(struct SDL_Texture *framebuffer, struct SDL_Renderer *render, float *p1, float *p2, float *p3);

#endif