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

typedef struct LineEqu
{
    float m;
    float x1;
    float y1;
    
} LineEqu;

typedef struct Points2
{
    float x1;
    float y1;
    
    float x2;
    float y2;
} Points2;

typedef struct Mesh
{
    Vector3 verts[100];
    int verts_count;
    
    Vector3 centre;
    
    int indexes[300];
    int indexes_count;
    
}Mesh;

Mesh create_cube(float width, float height, float depth, float cenx, float ceny, float cenz);

#endif