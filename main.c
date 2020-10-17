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
#include <SDL2/SDL_video.h>

int keyboardstate[322] = {};  // 322 is the number of SDLK_DOWN events
int exitnow = 0;
SDL_Renderer * ren;  
Uint32 pixels[B_INTERNAL_HEIGHT][B_INTERNAL_WIDTH];


union badpixelformat {
    struct {
        Uint8 r;
        Uint8 g;
        Uint8 b;
        Uint8 a;
    };
    Uint32 pixel;
};

Uint32 get_colour(struct colour *cl) {
    struct colour c = get_rgb(*cl);
    union badpixelformat p;
    p.r = c.r;
    p.g = c.g;
    p.b = c.b;
    p.a = c.a;
    return p.pixel;
}


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

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    SDL_Thread *input_thread = SDL_CreateThread(input_loop, "input", (void *)NULL);
    struct colour c = get_random_color();
    double elapsed;
    Uint64 start, end;
    struct object white_sphere = new_sphere(100);

    /* texture for drawing into */
    SDL_Rect view = {.w = B_INTERNAL_WIDTH, .h = B_INTERNAL_HEIGHT, .x = 0, .y = 0};
    SDL_Texture *texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, 
            SDL_TEXTUREACCESS_STATIC, B_INTERNAL_WIDTH, B_INTERNAL_HEIGHT);


    while (!exitnow) {
        /* clear the view */
        start = SDL_GetPerformanceCounter();
 //       SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
//        SDL_RenderClear(ren);
//

        if (SDL_GetTicks() % 800 == 0) {
            c = get_random_color();
        }

        /* draw stuff */
        /* march the rays */
        for (int i = 0; i < B_INTERNAL_WIDTH; i++) {
            for (int j = 0; j < B_INTERNAL_HEIGHT; j++) {
                struct vec *pos = new_vec(4);
                struct vec *dir = new_vec4(i  - B_INTERNAL_WIDTH/2, j - B_INTERNAL_HEIGHT/2, 100, 0);
                struct ray r = (struct ray) {
                    .pos = *pos,
                    .dir = *normalise_vec_ip(dir),
                };
                struct pixel_info p = march(&r, &white_sphere);
                p.col.r += p.iterations;
                p.col.b += 255 *  p.travel_dist / 100;

                //sdlb_draw_col_pixel(p.col, j, i);
                pixels[j][i] = get_colour(&p.col);
            //    free_vec(pos);
            //    free_vec(dir);
            }
        }


        SDL_UpdateTexture(texture, &view, pixels, B_INTERNAL_WIDTH * sizeof(Uint32));

        SDL_RenderCopy(ren, texture, NULL, NULL);

        SDL_RenderPresent(ren);

        end = SDL_GetPerformanceCounter();
        double el = (1000 * (end - start) / SDL_GetPerformanceFrequency());
        if (el > 0) {
            elapsed = 1000 / el;
        }
        printf("framerate: %f\r", elapsed);
        fflush(stdout);

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

