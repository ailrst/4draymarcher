#include "main.h"
#include "vect.h"

double sdf_sphere(struct vec *v, double r) {
    return magnitude_vec(v) - r;
}


