#include <SDL3/SDL.h>

#include "coms/commons.h"


typedef struct Cam
{
    Vector3 pos;
    float xrot;
    float yrot;
    int speed;
}Cam;

void camera_movement(Cam *camera)
{
    float mdx, mdy;
    SDL_GetRelativeMouseState(&mdx, &mdy);
    
    camera->xrot += mdx / 10;
    camera->yrot += mdy / 10;
}