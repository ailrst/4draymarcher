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
struct camera *camera;

struct SDL_Window* make_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
        printf("error initializing SDL: %s\n", SDL_GetError()); 
    } 

    return SDL_CreateWindow("𝕧 𝕒 𝕡 𝕠 𝕣 𝕨 𝕒 𝕧 𝕖", 
                                       SDL_WINDOWPOS_CENTERED, 
                                       SDL_WINDOWPOS_CENTERED, 
                                       B_WINDOW_WIDTH, B_WINDOW_HEIGHT, 
                                       SDL_WINDOW_RESIZABLE); 
}

void handle_inputs(void) 
{
    /* this is so broken it leaks memory everywhere and crashes and idek why 
     * Pressing UP will crash immediately and anything else will crash
     * eventually 
     *
     * It also makes the object disappear off into the 4th dimension
     */

    struct ray r;
    r.dir = new_vec(4); 
    r.pos = new_vec(4);

    const double dist = 0.1;
    double done = 0;

    if (keyboardstate[SDL_SCANCODE_UP]) {
        struct ray cameraray = {.pos = copy_vec(camera->pos), .dir = camera->y};
        manifoldturn(&cameraray, camera->x, dist);
        free_vec(cameraray.pos);

        cameraray.pos = copy_vec(camera->pos);
        manifoldturn(&cameraray, camera->z, dist);

        cameraray.pos = camera->pos;
        manifoldstep(&cameraray, dist);
        done = dist;
    }
    if (keyboardstate[SDL_SCANCODE_DOWN]) {
        struct ray cameraray = {.pos = copy_vec(camera->pos), .dir = camera->y};
        manifoldturn(&cameraray, camera->x, dist);

        free_vec(cameraray.pos);
        cameraray.pos = copy_vec(camera->pos);
        manifoldturn(&cameraray, camera->z, -dist);

        free_vec(cameraray.pos);
        cameraray.pos = camera->pos;
        manifoldstep(&cameraray, -dist);
        done = -dist;
    }
    if (keyboardstate[SDL_SCANCODE_LEFT])  {
        struct ray cameraray = {.pos = copy_vec(camera->pos), .dir = camera->x};
        manifoldturn(&cameraray, camera->z, dist);

        free_vec(cameraray.pos);
        cameraray.pos = copy_vec(camera->pos);

        manifoldturn(&cameraray, camera->y, dist);
        free_vec(cameraray.pos);
        cameraray.pos = camera->pos;
        manifoldstep(&cameraray, -dist);
        done = -dist;
    }                                      
    if (keyboardstate[SDL_SCANCODE_RIGHT]) {
        struct ray cameraray = {.pos = copy_vec(camera->pos), .dir = camera->x};
        manifoldturn(&cameraray, camera->z, dist);

        free_vec(cameraray.pos);
        cameraray.pos = copy_vec(camera->pos);

        manifoldturn(&cameraray, camera->y, dist);
        free_vec(cameraray.pos);
        cameraray.pos = camera->pos;
        manifoldstep(&cameraray, dist);
        done = -dist;
    }                                      
        r.dir->elements[0] = -1;
    if (keyboardstate[SDL_SCANCODE_ESCAPE]) {
        exitnow = 1;
    }

    if (fabs(done) > 0.00001)
        manifoldstep(&r, done);

    free_vec(r.dir);
    free_vec(r.pos);
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
        SDL_Delay(50);
        handle_inputs();
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
    //struct object white_sphere = new_object(new_vec3(0.75, 0, 8), 0, 1, sdf_box, yeet_whit);

    camera = calloc(1, sizeof(struct camera));
    camera->pos = new_vec(4);
    camera->x = new_vec(4);
    camera->y = new_vec(4);
    camera->z = new_vec(4);
    camera->x->elements[0] = 1;
    camera->y->elements[1] = 1;
    camera->z->elements[2] = 1;

    // struct object white_sphere = new_box(new_vec3(-0.75, 0, 8), 0, 1);
    // struct object other_white_sphere = new_sphere(new_vec3(0.75, 0, 8), 0, 1);

    struct object* tree = new_tree(new_vec3(0, 0, 5), 0, 1);

    struct object* scene_objects = malloc(2 * sizeof(struct object));
    scene_objects[0] = tree[0];
    scene_objects[1] = tree[1];
    
    scene_object = new_scene(2, scene_objects);
    scene_object.sol.pos.dimension = 3;
    scene_object.sol.pos.elements = camera->pos->elements;

}

int main(int argc, char **argv) {


    SDL_Window * win = make_window();
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(ren, B_INTERNAL_HEIGHT, B_INTERNAL_HEIGHT);

    // use this to turn on antristroptic filtering
//    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");


    double elapsed;
    int framct = 0;
    Uint64 start, end;

    /* texture for drawing into */
    SDL_Texture *texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, 
            SDL_TEXTUREACCESS_STATIC, B_INTERNAL_WIDTH, B_INTERNAL_HEIGHT);

    SDL_Thread ** threads = calloc(B_NUM_RAYMARCH_THREADS, sizeof(SDL_Thread *));

    SDL_Thread *input_thread = SDL_CreateThread(input_loop, "input", (void *)NULL);
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

        SDL_UpdateTexture(texture, NULL, pixels, B_INTERNAL_WIDTH * sizeof(Uint32));

        SDL_RenderCopy(ren, texture, NULL, NULL);

        SDL_RenderPresent(ren);

        end = SDL_GetPerformanceCounter();
        double el = (1000 * (end - start) / SDL_GetPerformanceFrequency());
        if (el > 0) {
            elapsed = 1000 / el;
        }
        printf("\rframerate: %f", elapsed);
//        fflush(stdout);
        framct++;
        if (framct++ > 2) {
        }

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

