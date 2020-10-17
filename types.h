#ifndef TYPES_H
#define TYPES_H

#define B_WINDOW_WIDTH 800
#define B_WINDOW_HEIGHT 600
#define B_INTERNAL_HEIGHT 100
#define B_INTERNAL_WIDTH 100

#include "vect.h"
#include "main.h"

enum solid_op {
    B_ADD
};

struct ray 
{
    struct vec pos;
    struct vec dir;
};

struct camera
{
    struct vec pos;
    struct vec rot;
    int dims;
};

struct pixel_info
{
    int flags; /* 0 - no collide */
    struct colour col;
    int iterations;
    double travel_dist;
    double scene_dist;
};

struct solid {
    enum solid_op op;
    struct vec pos;
    double rotation;
    double scale;
    double (*dist)(struct vec *);
};

struct object
{
    struct solid sol;
    struct colour (*col)(struct ray *);
};

#endif
