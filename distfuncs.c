#include "main.h"
#include "types.h"
#include "vect.h"

double sdf_sphere(struct vec *x) {
    static const double r = 1.4;
    struct vec *v = copy_vec(x);
    
    // v->elements[2] -= 5;
//    v->elements[1] += (SDL_GetTicks()/1000.0) - 5;
    // v->elements[2] -= 2;
   // v->elements[1] += (SDL_GetTicks()/1000.0) - 5;
    
    double res = magnitude_vec(v) - r;
    free_vec(v);

    return res;
}

double sdf_hplane(struct vec *x) {
    static const double h = -0.5;
    return -(x->elements[1] + h);
}

double clamp(double val, double min, double max) {
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
}

/*
double sdf_cone(struct vec *x) {
    double height = 3;
    struct vec *v = copy_vec(x);
    double cx = 1;
    double cy = 1;
    static struct vec *cone = NULL;
    static struct vec *w, *a, *b;

    if (!cone) {
        cone = scalar_multiply_vec_ip(new_vec2(cx/cy, -1), height); 
    }
    struct vec *temp;

    double cone2 = 

    struct vec * temp = new_vec2(v->e->x, v->e->z);
    double aaa = magnitude_vec(temp);
    free_vec(temp);
    w = new_vec2(aaa, v->e->y);

    double t2 = clamp((dot_product_vec(w, v) / dot_product_vec(cone, cone)), 0.0, 1.0);
    temp = scalar_multiply_vec(cone, t2);
    a = subtract_vec(w, temp);
    free_vec(temp);

    temp = new_vec2(clamp(w->e->x / cone->e->x, 0.0, 1.0), 1.0);
    scalar_multiply_vec_ip(temp, )



    

    double result = -1;
    free_vec(v);
    return result;
}
        */

double is_pos(double a) {
    if (a > 0) {
        return a;
    }
    return 0;
}

double sdf_phat_vert_line(struct vec *x) {
    static const double h = 10;
    static const double r = 1;

    struct vec *v = copy_vec(x);
    
    v->e->y -= clamp(v->e->y, 0.0, h);
    double val = magnitude_vec(v) - r;
    free_vec(v);
    return val;
}

/* https://www.alanzucconi.com/2016/07/01/signed-distance-functions/#part3 */
/* http://mercury.sexy/hg_sdf/ */
double sdf_box(struct vec *x) {

    struct vec *v = copy_vec(x);
    do_on_vec_ip(v, fabs);

    static struct vec * box_shape = NULL;

    if (!box_shape) {
        box_shape = new_vec_of(v->dimension, -0.5);
    }

    do_on_vec_ip(v, is_pos);
    
    double result = magnitude_vec(v);

    free_vec(v);

    return result;
}

struct colour yeet_col(struct ray *ray) {
    struct vec *l = new_vec4(1,1,1,1);
    struct vec *n = subtract_vec_ip(l, ray->pos);
    struct vec *nl = normalise_vec(l);

    struct colour c = {.r = nl->e->x * 555, .g = nl->e->y * 555, .b = nl->e->z * 555, .a = 255, .sp=CS_RGB};

    free_vec(n);
    return (c);
}

struct object new_sphere(double radius) {
    struct object s;
    
    struct vec * v = new_vec4(0,0,5,0);
    s.sol.pos = *v;
    s.sol.op = B_ADD;
    s.col = yeet_col;
    s.sol.dist = sdf_sphere;

    return s;
}

