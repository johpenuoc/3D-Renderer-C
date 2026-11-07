#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "cam/camera.h"
#include "gfx/graphics.h"

int main(int argc, char* argv[])
{
    
    if (!SDL_Init(SDL_INIT_VIDEO))
        {
            SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
            return 1;
        }
    
        SDL_Window *window = SDL_CreateWindow(
            "SDL3 Window",
            1280,
            720,
            0
        );
        
        if (window == NULL)
            {
                SDL_Log("Unable to create window: %s", SDL_GetError());
                SDL_Quit();
                return 1;
            }
            
        SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
        if (renderer == NULL)
            {
                SDL_Log("Couldn't create renderer: %s", SDL_GetError());
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 1;
            }
            
        SDL_Texture *framebuffer = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ABGR8888,
            SDL_TEXTUREACCESS_STREAMING,
            1280, 720
        );
        
        
        // Initialize camera
        Cam camera;
        camera.pos.x = 200.0f;
        camera.pos.y = 0.0f;
        camera.pos.z = 100.0f;
        camera.xrot = 0.0f;
        camera.yrot = 0.0f;
        camera.speed = 150.0f;
        
        // Initialize cube
        float width = 50.0f;
        float height = 50.0f;
        float depth = 50.0f;
        Mesh cube = create_cube(width, height, depth, 200.0f, 0.0f, 200.0f);
        Vector3 verts[cube.verts_count];
        Vector3 *verts_ptr = verts;
        
        Uint64 prevt = SDL_GetTicks();
        bool is_running = true;
        SDL_Event event;
        
        SDL_SetWindowFullscreen(window, true);
        SDL_SetWindowRelativeMouseMode(window, true);
        
        while (is_running)
            {
                Uint64 currt = SDL_GetTicks();
                
                float dt = (float)(currt - prevt) / 1000.0f;
                prevt = currt;
                
                while (SDL_PollEvent(&event))
                    {
                        if (event.type == SDL_EVENT_QUIT)
                            {
                              is_running = false;  
                            }
                        
                        else if (event.type == SDL_EVENT_KEY_DOWN)
                        {
                            if (event.key.scancode == SDL_SCANCODE_ESCAPE)
                                {
                                    is_running = false;
                                }
                        }
                    }
                    
                // const bool *keystate = SDL_GetKeyboardState(NULL);
                
                camera_movement(&camera, dt);
                vertex_processing(&cube, &camera, verts_ptr);
                //SDL_WarpMouseInWindow(window, (float)1280 / 2, (float)720 / 2);
                
                    
                SDL_RenderClear(renderer);
                    
                SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
                SDL_RenderClear(renderer);
                
                render(framebuffer, renderer, &cube, verts_ptr);
                
                
                //shader(framebuffer, renderer, p1_ptr, p2_ptr, p3_ptr);
            
                SDL_RenderPresent(renderer);
                
                float fps = (dt > 0) ? 1000.0f / dt : 0.0f;
                printf("%f\n", fps);
            }
        
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        
        printf("SDL window closed.\n");
        return 0;
}