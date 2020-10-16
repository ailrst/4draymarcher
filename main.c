#include "main.h"
#include "queue.h"

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
                    goto leave;
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
leave:
    exitnow = 1;
    return 0;
}


int main(int argc, char **argv) {

    SDL_Window * win = make_window();
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(ren, B_INTERNAL_HEIGHT, B_INTERNAL_HEIGHT);

    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.h = 300;
    r.w = 500;

    SDL_Thread *input_thread = SDL_CreateThread(input_loop, "input", (void *)NULL);
    struct colour c = get_random_color();

    while (!exitnow) {
        /* clear the view */
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        if (SDL_GetTicks() % 800 == 0) {
            c = get_random_color();
        }

        /* draw stuff */
        sdlb_set_colour(c);
        SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);
        SDL_RenderFillRect(ren, &r);

        /* update the view */
        SDL_RenderPresent(ren);
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

