#include "main.h"
#include "types.h"
#include "math.h"
#include "vect.h"
#include "distfuncs.h"
#include "scene.h"

#define DRAW_DIST 10000.0
#define MAX_ITERATIONS 25
#define EPSILON 0.1

double manidist(struct vec *v) 
{
        return v->elements[3];
}

struct solid manifold = (struct solid) {
        .dist = manidist,
};

double solid_dist(struct solid *s, struct vec *v) {
        subtract_vec_ip(v, &s->pos);
        double out = s->dist(v);
        add_vec_ip(v, &s->pos);
        return out;
}

/* return a malloced vector, normal to sol at r */ 
struct vec * 
estimateNormal(struct vec *r, struct solid *sol) 
{
        struct vec *out = new_vec(r->dimension);
        struct vec *tmp = new_vec(r->dimension);
        for (int i = 0; i < out->dimension; i++) {
                tmp->elements[i] = EPSILON;
                double s1 = solid_dist(sol, tmp);
                tmp->elements[i] = -EPSILON;
                double s2 = solid_dist(sol, tmp);
                
                out->elements[i] = s1 - s2;
                tmp->elements[i] = 0;
        }
        free(tmp->elements);
        return normalise_vec_ip(out);
}

struct vec *
reyeet(struct vec *v, struct vec *k) {
        struct vec *vs [v->dimension - 1];
        struct vec *tmp = new_vec_of(v->dimension, 1);
        vs[0] = k;
        vs[1] = v;
        for (int i = 1; i < v->dimension; i++) vs[i] = k;
        struct vec *out = perpendicular_vec(v->dimension - 1, vs);
        free_vec(tmp);
        return out;
}

void
rotateaxis(struct vec *v, struct vec *k, double a)
{
        double cosa = cos(a);        

        struct vec *p = add_scaled_vec_ip(
                add_scaled_vec_ip(scalar_multiply_vec_ip(reyeet(v, k), sin(a)), v, cosa),
                k, dot_product_vec(k, v)*(1 - cosa));

        free(v->elements);
        v->elements = p->elements;
}


void 
manifoldstep(struct ray *r, double distance)
{

        struct vec *yaxisold = estimateNormal(&r->pos, &manifold);

         /* move the vector foward in euclid */
        add_scaled_vec_ip(&r->pos, &r->dir, distance);

        struct vec *yaxisnew = estimateNormal(&r->pos, &manifold);

        /* stick it to the manifold */
        add_scaled_vec_ip(&r->pos, yaxisnew, manifold.dist(&r->pos));

        double protamtloc = acos(dot_product_vec(yaxisold,yaxisnew));

        struct vec *protaxisloc = normalise_vec_ip(reyeet(yaxisold, yaxisnew));
        rotateaxis(&r->dir, protaxisloc, protamtloc); /* change the direction */

        free_vec(yaxisnew);
        free_vec(yaxisold);
        free_vec(protaxisloc);
}

struct pixel_info 
march(struct ray *r, struct object *scene)
{
        double travel_dist = 0;
        double scene_dist;
        double min_dist = DRAW_DIST;
        int i;
        struct colour out = (struct colour) {};
        for (i = 0; (i < MAX_ITERATIONS) && (travel_dist < DRAW_DIST); i++) {
                //scene_dist = scene->sol.dist(&(r->pos));
                scene_dist = solid_dist(&(scene->sol), &(r->pos));
                if (scene_dist < EPSILON) { /* we've hit an object */
                         out = scene->col(r);
                         break;
                }

                if (min_dist > scene_dist)
                        min_dist = scene_dist;

                manifoldstep(r, scene_dist);
                travel_dist += scene_dist;
        }

        /* no colour reached */
        return (struct pixel_info) {
                .flags = 1, 
                .iterations = i, 
                .col = out, 
                .travel_dist = travel_dist, 
                .scene_dist = min_dist
                }; 
}


union badpixelformat {
    struct {
        Uint8 r;
        Uint8 g;
        Uint8 b;
        Uint8 a;
    };
    Uint32 pixel;
};

Uint32 get_stl_colour(struct colour *cl) {
    struct colour c = get_rgb(*cl);
    union badpixelformat p;
    p.r = c.r;
    p.g = c.g;
    p.b = c.b;
    p.a = c.a;
    return p.pixel;
}

Uint32
process_pixel(int i, int j)
{
        struct vec *pos = new_vec(4);
        struct vec *dir = normalise_vec_ip(new_vec4(i  - B_INTERNAL_WIDTH/2, j - B_INTERNAL_HEIGHT/2, 100, 0));
        struct ray r = (struct ray) {
            .pos = *pos,
            .dir = *dir,
        };
        struct pixel_info p = march(&r, &scene_object);
        p.col.r -= p.iterations*10;
        p.col.b -= p.iterations*10;
        p.col.g -= p.iterations*10;
        // printf("%d, ", p.iterations);
        if (p.col.r < 0) p.col.r = 0;
        if (p.col.g < 0) p.col.g = 0;
        if (p.col.b < 0) p.col.b = 0;
        if (p.col.r > 255) p.col.r = 255;
        if (p.col.g > 255) p.col.g = 255;
        if (p.col.b > 255) p.col.b = 255;
        // p.col.b = 255.0 / p.scene_dist;
        // if (p.col.b > 255) p.col.b = 255;
        free_vec(pos); 
        // free_vec(dir);

        return get_stl_colour(&p.col);
}