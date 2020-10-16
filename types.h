#ifndef TYPES_H
#define TYPES_H

#define color int

#define B_WINDOW_WIDTH 800
#define B_WINDOW_HEIGHT 600
#define B_INTERNAL_HEIGHT 800
#define B_INTERNAL_WIDTH 600

enum solid_op {
    ADD
};

struct ray 
{
    vec pos;
    vec dir;
};

struct camera
{
    vec pos;
    vec rot;
    int dims;
};

struct pixel_info
{
    int flags; /* 0 - no collide */
    color col;
    int iterations;
    double travel_dist;
    double scene_dist;
};



struct object
{
    solid sol;
    color (*col)(struct ray);
};

struct solid {
    enum solid_op op;
    double rotation;
    double scale;
    double (*dist)(struct vec *);
};

#endif
