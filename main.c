#include "types.h"
#include "vect.h"
#include "main.h"
#include "math.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>

int keyboardstate[322] = {};  // 322 is the number of SDLK_DOWN events
int exitnow = 0;


struct SDL_Window* make_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
        printf("error initializing SDL: %s\n", SDL_GetError()); 
    } 

    return SDL_CreateWindow("sdl_tester", 
                                       SDL_WINDOWPOS_CENTERED, 
                                       SDL_WINDOWPOS_CENTERED, 
                                       0, 0, 
                                       SDL_WINDOW_FULLSCREEN_DESKTOP); 
}

void handle_inputs(void) {
    if (keyboardstate[SDL_SCANCODE_UP])
        printf("up");
    if (keyboardstate[SDL_SCANCODE_DOWN])
        printf("down");
    if (keyboardstate[SDL_SCANCODE_LEFT])
        printf("left");
    if (keyboardstate[SDL_SCANCODE_RIGHT])
        printf("right");
    if (keyboardstate[SDL_SCANCODE_ESCAPE]) {
        SDL_Quit();
    }

    return;
};


int input_loop(void *ptr) {
    int close = 0;
    while (!close)  {
        SDL_Event event; 
        while(SDL_PollEvent(&event))  {
            switch (event.type) {
                case SDL_QUIT:
                    close = 1;
                    return 0;
                case SDL_KEYDOWN:
                    keyboardstate[event.key.keysym.scancode] = 1;
                    break;
                case SDL_KEYUP:
                    keyboardstate[event.key.keysym.scancode] = 0;
                    break;
            }
        }
        handle_inputs();
        SDL_Delay(100);
    }
    SDL_Quit();
    exitnow = 1;
    return 0;
}


int main(int argc, char **argv) {
    

    SDL_Window * win = make_window();
    SDL_Renderer * ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Thread *input_thread = SDL_CreateThread(input_loop, "input", (void *)NULL);

    while (!exitnow) {
        ;
    }

}
