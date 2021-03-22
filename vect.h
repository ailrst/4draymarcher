#ifndef VECT_H
#define VECT_H

#include <stdlib.h>
#include <stdio.h>


void free_vec(struct vec* a);

struct yeetVec {
    double x;
    double y;
    double z;
    double w;
    double q;
};

struct vec {
    int dimension;
    union {
        struct yeetVec e;
        double elements[5];
    };
    bool active;
    /*

    ~vec() {
        free_vec(this);
    }
    */
};

struct vec5 {
    int dimension;
    union {
        double elements[5];
        struct yeetVec e;
    };
};

struct vec4 {
    int dimension;
    union {
        double elements[4];
        struct yeetVec e;
    };
};

struct vec3 {
    int dimension;
    union {
        double elements[3];
        struct yeetVec e;
    };
};


struct vec2 {
    int dimension;
    union {
        double elements[2];
        struct yeetVec e;
    };
};


struct mat2 {
    int num_rows;
    int num_cols;
    double **elements;
};




struct vec* new_vec(int num_dimensions);
struct vec* new_random_vec(int num_dimensions, double min, double max);
struct vec* new_vec_of(int num_dimensions, double value);


struct vec* new_vec2(double x, double y);
struct vec* new_vec3(double x, double y, double z);
struct vec* new_vec4(double w, double x, double y, double z);

struct vec* add_vec(struct vec* a, struct vec* b);
struct vec* add_vec_ip(struct vec* a, struct vec* b);
struct vec* add_scaled_vec_ip(struct vec* a, struct vec* b, double scaleFactor);

struct vec* subtract_vec(struct vec* a, struct vec* b);
struct vec* subtract_vec_ip(struct vec* a, struct vec* b);

struct vec* normalise_vec(struct vec* a);
struct vec* normalise_vec_ip(struct vec* a);

double magnitude_vec(struct vec* a);
double distance_vec(struct vec* a, struct vec* b);

double dot_product_vec(struct vec* a, struct vec* b);

struct vec* scalar_multiply_vec(struct vec* a, double multiplier);
struct vec* scalar_multiply_vec_ip(struct vec* a, double multiplier);

struct vec* copy_vec(struct vec*);

struct vec* perpendicular_vec(int num_vectors, struct vec** vectors);

/* do something to every value in a vector */
struct vec* do_on_vec_ip(struct vec * v, double (*func)(double));

/* get the smallest value in the vector */
double vec_min(const struct vec *v); 

/* get the largest value in the vector */
double vec_max(const struct vec *v); 

#endif
