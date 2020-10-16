#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#ifndef H_COLOURS
#define H_COLOURS

enum colour_space {
    CS_INVALID = 0,
    CS_RGB = 0b1, // default to RGB
    CS_HSV = 0b10,
    CS_HSL = 0b100,
};

struct colour {
    enum colour_space sp;
    double h;
    double s;
    double v;
    double l;
    short r;
    short g;
    short b;
    short a; // alpha
};

SDL_Colour get_sdlcolour(struct colour col, int alpha); 

struct colour get_random_color(void); 

// doesn't support hsl-hsv or converse, conversion
struct colour get_hsl(struct colour c);
struct colour get_hsv(struct colour c); 

struct colour get_rgb(struct colour c); 

struct colour *get_adjacent(struct colour base, int deg, int num); 

void print_colour(struct colour c); 

void test_print_wheel(); 

#endif
