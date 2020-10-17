#include "main.h"
#include "queue.h"
#include "camera.h"
#include "distfuncs.h"
#include "scene.h"
#include "types.h"
#include "vect.h"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>

int keyboardstate[322] = {};  // 322 is the number of SDLK_DOWN events
int exitnow = 0;
SDL_Renderer * ren;  
Uint32 pixels[B_INTERNAL_HEIGHT][B_INTERNAL_WIDTH];

struct object scene_object;

struct SDL_Window* make_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
        printf("error initializing SDL: %s\n", SDL_GetError()); 
    } 

    return SDL_CreateWindow("sdl_tester", 
                                       SDL_WINDOWPOS_CENTERED, 
                                       SDL_WINDOWPOS_CENTERED, 
                                       B_WINDOW_WIDTH, B_WINDOW_HEIGHT, 
                                       SDL_WINDOW_RESIZABLE); 
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
        //            *(int *)0 = 1; // segfault
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
        SDL_Delay(50);
    }
    return 0;
}

static int raymarch_threadfn(void *data) {
    int id = *(int *)data;

    /* draw stuff */
    /* march the rays */
    for (int i = 0; i < B_INTERNAL_WIDTH; i++) {
        for (int j = 0; j < B_INTERNAL_HEIGHT; j++) {
            /* checkerboard rendering */
            if ((i + j * B_INTERNAL_WIDTH) % B_NUM_RAYMARCH_THREADS != id) {
                continue;
            }
            //sdlb_draw_col_pixel(p.col, j, i);
            pixels[j][i] = process_pixel(i, j);
        }
    }

    free(data);
    return 0;

}

void setup_camera_scene()
{
    struct object white_sphere = new_sphere(10);
    white_sphere.sol.pos.elements[0] = -1.75;
    white_sphere.sol.pos.elements[1] = -1;
    white_sphere.sol.pos.elements[2] = 7;

    struct object other_white_sphere = new_sphere(10);
    other_white_sphere.sol.pos.elements[0] = -1.75;
    other_white_sphere.sol.pos.elements[1] = 0;
    other_white_sphere.sol.pos.elements[2] = 7;

    struct object* scene_objects = malloc(2 * sizeof(struct object));
    scene_objects[0] = white_sphere;
    scene_objects[1] = other_white_sphere;
    
    scene_object = new_scene(2, scene_objects);
}

int main(int argc, char **argv) {
    SDL_Window * win = make_window();
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(ren, B_INTERNAL_HEIGHT, B_INTERNAL_HEIGHT);

    // use this to turn on antristroptic filtering
    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    SDL_Thread *input_thread = SDL_CreateThread(input_loop, "input", (void *)NULL);

    double elapsed;
    int framct = 0;
    Uint64 start, end;

    /* texture for drawing into */
    SDL_Rect view = {.w = B_INTERNAL_WIDTH, .h = B_INTERNAL_HEIGHT, .x = 0, .y = 0};
    SDL_Texture *texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, 
            SDL_TEXTUREACCESS_STATIC, B_INTERNAL_WIDTH, B_INTERNAL_HEIGHT);

    SDL_Thread ** threads = calloc(B_NUM_RAYMARCH_THREADS, sizeof(SDL_Thread *));

    setup_camera_scene();

    while (!exitnow) {
        /* clear the view */
        start = SDL_GetPerformanceCounter();
        SDL_RenderClear(ren);

        for (int i = 0; i < B_NUM_RAYMARCH_THREADS; i++) {
            int *tid = malloc(sizeof(int));
            *tid = i;
            threads[i] = SDL_CreateThread(raymarch_threadfn, "raymarch", tid);
            // tid is freed in raymarch_threadfn
        }

        for (int i = 0; i < B_NUM_RAYMARCH_THREADS; i++) {
            int status;
            SDL_WaitThread(threads[i], &status);
        }

        SDL_UpdateTexture(texture, &view, pixels, B_INTERNAL_WIDTH * sizeof(Uint32));

        SDL_RenderCopy(ren, texture, NULL, NULL);

        SDL_RenderPresent(ren);

        end = SDL_GetPerformanceCounter();
        double el = (1000 * (end - start) / SDL_GetPerformanceFrequency());
        if (el > 0) {
            elapsed = 1000 / el;
        }
        printf("\rframerate: %f", elapsed);
        fflush(stdout);
        framct++;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
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

