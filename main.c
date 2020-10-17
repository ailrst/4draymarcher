#include "main.h"
#include "queue.h"
#include "camera.h"
#include "distfuncs.h"
#include "types.h"
#include "vect.h"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

int keyboardstate[322] = {};  // 322 is the number of SDLK_DOWN events
int exitnow = 0;
SDL_Renderer * ren;  

struct SDL_Window* make_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
        printf("error initializing SDL: %s\n", SDL_GetError()); 
    } 

    return SDL_CreateWindow("sdl_tester", 
                                       SDL_WINDOWPOS_CENTERED, 
                                       SDL_WINDOWPOS_CENTERED, 
                                       B_WINDOW_WIDTH, B_WINDOW_HEIGHT, 
                                       0); 
}


void handle_inputs(void) 
{
    if (keyboardstate[SDL_SCANCODE_UP])
        printf("up");
    if (keyboardstate[SDL_SCANCODE_DOWN])
        printf("down");
    if (keyboardstate[SDL_SCANCODE_LEFT])
        printf("left");
    if (keyboardstate[SDL_SCANCODE_RIGHT])
        printf("right");
    if (keyboardstate[SDL_SCANCODE_ESCAPE]) {
        exitnow = 1;
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
                    exitnow = 1;
                    *(int *)0 = 1;
                    break;
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
    return 0;
}


int main(int argc, char **argv) {
    SDL_Window * win = make_window();
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(ren, B_INTERNAL_HEIGHT, B_INTERNAL_HEIGHT);

    SDL_Thread *input_thread = SDL_CreateThread(input_loop, "input", (void *)NULL);
    struct colour c = get_random_color();
    double elapsed;
    Uint64 start, end;


    while (!exitnow) {
        /* clear the view */
        start = SDL_GetPerformanceCounter();
 //       SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
//        SDL_RenderClear(ren);

        if (SDL_GetTicks() % 800 == 0) {
            c = get_random_color();
        }

        /* draw stuff */
        /* march the rays */
        for (int i = 0; i < B_INTERNAL_WIDTH; i++) {
            for (int j = 0; j < B_INTERNAL_HEIGHT; j++) {
                sdlb_draw_col_pixel(process_pixel(i, j), i, j);
            }
        }



        SDL_RenderPresent(ren);

        end = SDL_GetPerformanceCounter();
        double el = (1000 * (end - start) / SDL_GetPerformanceFrequency());
        if (el > 0) {
            elapsed = 1000 / el;
        }
        printf("framerate: %f\r", elapsed);
        fflush(stdout);

    }

    SDL_Quit();
}

/********************************************************************************
 *
 * DRAWING UTILITIES 
 *
 */
void sdlb_set_colour(struct colour col) {
    struct colour c = get_rgb(col);
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);
}

void sdlb_draw_col_pixel(struct colour col, int x, int y) {
      sdlb_set_colour(col);
      SDL_RenderDrawPoint(ren, x, y);
}

