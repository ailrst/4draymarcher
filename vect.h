#ifndef VECT_H
#define VECT_H

#include <stdlib.h>
#include <stdio.h>

struct vec {
    int dimension;
    double *elements;
};

struct vec* new_vec(int num_dimensions);
void free_vec(struct vec*);

struct vec* new_vec2(double x, double y);
struct vec* new_vec3(double x, double y, double z);
struct vec* new_vec4(double w, double x, double y, double z);

struct vec* add_vec(struct vec* a, struct vec* b);
struct vec* add_vec_ip(struct vec* a, struct vec* b);

struct vec* subtract_vec(struct vec* a, struct vec* b);
struct vec* subtract_vec_ip(struct vec* a, struct vec* b);

struct vec* normalise_vec(struct vec* a);
struct vec* normalise_vec_ip(struct vec* a);

double magnitude_vec(struct vec* a);
double distance_vec(struct vec* a, struct vec* b);

struct vec* dot_product_vec(struct vec* a, struct vec* b);

struct vec* scalar_multiply_vec(struct vec* a, double multiplier);
struct vec* scalar_multiply_vec_ip(struct vec* a, double multiplier);

struct vec* perpendicular_vec(struct vec* a, struct vec* b);

#endif
