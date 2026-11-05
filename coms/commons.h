#ifndef COMMONS_H
#define COMMONS_H

typedef struct Vector3
{
    float x;
    float y;
    float z;
} Vector3;

typedef struct Vector2
{
    float p[2];
} Vector2;

typedef struct Mesh
{
    Vector3 verts[8];
    int indexes[36];
}Mesh;

Mesh create_cube(float width, float height, float depth, float cenx, float ceny, float cenz);

#endif