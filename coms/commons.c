#include <stdlib.h>

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
    Vector3 verts[100];
    int verts_count;
    
    Vector3 centre;
    
    int indexes[300];
    int indexes_count;
    
}Mesh;

Mesh create_cube(float width, float height, float depth, float cenx, float ceny, float cenz)
{
    float width2 = width / 2.0f;
    float height2 = height / 2.0f;
    float depth2 = depth / 2.0f;
    
    Mesh cube = {
        .verts = {
            {-width2, -height2, -depth2},
            {-width2, height2, -depth2},
            {width2, -height2, -depth2},
            {width2, height2, -depth2},
            {-width2, -height2, depth2},
            {-width2, height2, depth2},
            {width2, -height2, depth2},
            {width2, height2, depth2}
        },
        
        .indexes = {
            0, 1, 2,
            3, 2, 1,
            
            0, 2, 6,
            6, 4, 0,
            
            4, 5, 7,
            7, 6, 4,
            
            1, 3, 7,
            7, 5, 1,
            
            0, 1, 5,
            5, 4, 0,
            
            2, 3, 7,
            7, 6, 2
        }
    
    };
    
    cube.verts_count = 8;
    cube.indexes_count = 36;
    cube.centre.x = cenx;
    cube.centre.y = ceny;
    cube.centre.z = cenz;
    
    return cube;
}