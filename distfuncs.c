#include "main.h"
#include "types.h"
#include "vect.h"

double sdf_sphere(struct vec *x) {
    const double r = 1.4;
    struct vec *v = copy_vec(x);
    v->elements[2] -= 4;
    double res = magnitude_vec(v) - r;
    free_vec(v);

    // return magnitude_vec(x - centre) - r 
    //
//    printf("v %f %f\n", magnitude_vec(x) - r);
    return res;
}


/* https://www.alanzucconi.com/2016/07/01/signed-distance-functions/#part3 */
/* http://mercury.sexy/hg_sdf/ */
double sdf_box(struct vec *v) {
    const double s = 10;

    /* the NEGATED box shape */
    static struct vec * box_shape = NULL;
    struct vec * radius_vec = NULL;

    if (!box_shape) {
        box_shape = new_vec(v->dimension);
        for (int i = 0; i < box_shape->dimension; i++) {
            box_shape->elements[i] = 1;
        }
        scalar_multiply_vec_ip(box_shape, -1);
        radius_vec= new_vec_of(v->dimension, -s);;
    }
    
    struct vec * p = add_vec_ip(copy_vec(v), box_shape); // v - box_shape
    do_on_vec_ip(p, fabs);
    add_vec_ip(p, radius_vec);

    double result = vec_max(p);
    free_vec(p);

    return result;
}
                     

struct colour simple_col(struct ray *ray) {
    struct colour c = {.r = 0, .g = 255, .b = 0, .a = 255, .sp=CS_RGB};
    return (c);
}

struct object new_sphere(double radius) {
    struct object s;

    struct vec * v = new_vec4(0,0,5,0);
    s.sol.pos = *v;
    s.sol.op = B_ADD;
    s.col = simple_col;
    s.sol.dist = sdf_sphere;

    return s;
}
