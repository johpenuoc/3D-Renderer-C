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
            800,
            600,
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
        camera.pos.x = 50;
        camera.pos.y = 0;
        camera.pos.z = 50;
        camera.xrot = 0;
        camera.yrot = 0;
        camera.speed = 70;
        
        float p1[2] = { 50, 50 };
        float p2[2] = { 100, 100 };
        float p3[2] = { 25, 150 };
        
        float *p1_ptr = &p1[0];
        float *p2_ptr = &p2[0];
        float *p3_ptr = &p3[0];
        
        Uint64 prevt = SDL_GetTicks();
        bool is_running = true;
        SDL_Event event;
        
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
                    }
                    
                // const bool *keystate = SDL_GetKeyboardState(NULL);
                
                    
                        
                    
                SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
                SDL_RenderClear(renderer);
                
                shader(framebuffer, renderer, p1_ptr, p2_ptr, p3_ptr);
            
                SDL_RenderPresent(renderer);
            }
        
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        
        printf("SDL window closed.\n");
        return 0;
}