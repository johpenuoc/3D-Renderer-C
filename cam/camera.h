#ifndef CAMERA_H
#define CAMERA_H

#include "coms/commons.h"

typedef struct Cam
{
    Vector3 pos;
    float xrot;
    float yrot;
    int speed;
}Cam;

#endif