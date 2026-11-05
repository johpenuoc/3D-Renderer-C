#include <SDL3/SDL.h>
#include <math.h>
#include <stdio.h>

#include "coms/commons.h"

#define RADIANS(angle) ((angle) * (3.14159 / 180.0f))

typedef struct Cam
{
    Vector3 pos;
    float xrot;
    float yrot;
    int speed;
}Cam;

void camera_movement(Cam *camera, float dt)
{
    float mdx, mdy;
    SDL_GetRelativeMouseState(&mdx, &mdy);
    camera->xrot += mdx / 10.0f;
    camera->yrot += mdy / 10.0f;
    
    //printf("x: %f   ||   y: %f   || z: %f\n", camera->pos.x, camera->pos.y, camera->pos.z);
    
    
    const bool *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_W])
        {
            camera->pos.x -= cosf(RADIANS(camera->xrot + 90.0f)) * 25.0f * dt;
            camera->pos.z += sinf(RADIANS(camera->xrot + 90.0f)) * 25.0f * dt;
        }
    if (keystate[SDL_SCANCODE_S])
        {
           camera->pos.x += cosf(RADIANS(camera->xrot + 90.0f)) * 25.0f * dt;
            camera->pos.z -= sinf(RADIANS(camera->xrot + 90.0f)) * 25.0f * dt;
        }
        
    if (keystate[SDL_SCANCODE_D])
        {
            camera->pos.x -= cosf(RADIANS(camera->xrot + 180.0f)) * 25.0f * dt;
            camera->pos.z += sinf(RADIANS(camera->xrot + 180.0f)) * 25.0f * dt;
        }
    if (keystate[SDL_SCANCODE_A])
        {
            camera->pos.x += cosf(RADIANS(camera->xrot + 180.0f)) * 25.0f * dt;
            camera->pos.z -= sinf(RADIANS(camera->xrot + 180.0f)) * 25.0f * dt;
        }
    
    camera->xrot = fmodf(camera->xrot, 360.0f);
    camera->yrot = fmodf(camera->yrot, 360.0f);
}