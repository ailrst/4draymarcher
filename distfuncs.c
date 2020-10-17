#include "main.h"
#include "types.h"
#include "vect.h"
#include "camera.h"

double sdf_sphere(struct vec *x) {
    static const double r = 1.4;
    struct vec *v = copy_vec(x);
    
    //v->elements[2] -= 5;
//    v->elements[1] += (SDL_GetTicks()/1000.0) - 5;
    v->elements[2] -= 2;
   // v->elements[1] += (SDL_GetTicks()/1000.0) - 5;
    
    double res = magnitude_vec(v) - r;
    free_vec(v);

    // return magnitude_vec(x - centre) - r 
    //
//    printf("v %f %f\n", magnitude_vec(x) - r);
    return res;
}

double sdf_hplane(struct vec *x) {
    static const double h = -5;
    return -(x->e->y) - h ;
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

double clamp_positive(double a) {
    if (a > 0) {
        return a;
    }
    return 0;
}

double sdf_fat_vert_line(struct vec *x) {
    static const double h = 10;
    static const double r = 1;

    struct vec *v = copy_vec(x);
    v->elements[2] -= 5;
    
    v->e->y -= clamp(v->e->y, 0.0, h);
    double val = magnitude_vec(v) - r;
    free_vec(v);
    return val;
}

/* https://www.alanzucconi.com/2016/07/01/signed-distance-functions/#part3 */
/* http://mercury.sexy/hg_sdf/ */
double sdf_box(struct vec *x) {

    struct vec *v = copy_vec(x);
    v->elements[2] -= 50;
    do_on_vec_ip(v, fabs);

    struct vec *box = new_vec3(-1, -1, -1);

    add_vec_ip(v, box);

    do_on_vec_ip(v, clamp_positive);
    
    double result = magnitude_vec(v);

    free_vec(v);
    free_vec(box);

    return result;
}

struct colour dlight(struct ray *ray, struct object *o) {
    // normal

    /* richals code doesnt work for when veca is < vecb */
    struct vec *cam = new_vec4(999,999,999, 999);
//    struct vec *nray = scalar_multiply_vec(&ray->dir, -1);
    struct vec *norm = estimateNormal(&ray->pos,&o->sol);

    struct vec *tolight = subtract_vec(&ray->pos, cam);

    double v = dot_product_vec(tolight, norm) * 200;

    struct colour c = {.a = 255, .r = v, .g = v, .b = v};

    free(norm);
 //   free(nray);
    free_vec(cam);
    return c;
}

struct colour yeet(struct ray *ray) {

    struct colour c = {.r = 255, .g = 0, .b = 255, .a = 255, .sp=CS_RGB};
    return (c);
}

struct object new_sphere(double radius) {
    struct object s;
    
    struct vec * v = new_vec4(0,0,5,0);
    s.sol.pos = *v;
    s.sol.op = B_ADD;
    s.col = dlight;
    s.sol.dist = sdf_sphere;

    return s;
}

