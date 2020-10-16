#include "main.h"

#define DRAW_DIST 1000.0
#define MAX_ITERATIONS 255
#define EPSILON 0.1

struct solid manifold;

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
        free_vec(tmp->elements);
        return normalise_vec_ip(out);
}

void
rotateaxis(struct vec *v, struct vec *k, double a)
{
        double cosa = cos(a);

        struct vec *comp1 = scalar_multiply_vec_ip(copy_vec(v), cosa);
        struct vec *comp2 = scalar_multiply_vec_ip(perpendicular_vec(k, v), sin(a));
        struct vec *comp3 = scalar_multiply_vec_ip(copy_vec(k), dot(k, v)*(1 - cosa));
        add_vec_ip(comp1, add_vec_ip(comp2, comp3));
        free(v->)
}

void 
manifoldstep(struct ray *r, double distance)
{
        struct vec *yaxisold = estimateNormal(r->pos, &manifold);
        add_vec_ip(&r->pos, scale_vec(r->dir, distance)); /* move the vector foward in euclid */
        struct vec *yaxisnew = estimateNormal(r->pos, &manifold);

        struct vec *temp = scale_vec(yaxisnew, manifold.dist(r->pos));
        add_vec_ip(&r->pos, temp); /* stick it to the manifold */
        free_vec(temp);

        double protamtloc = acos(dot(yaxisold,yaxisnew));
        struct vec *protaxisloc = normalise_vec_ip(cross(yaxisold, yaxisnew));
        rotateaxis(&r->dir, protaxisloc, protamtloc); /* change the direction */
}

struct pixel_info 
march(struct ray *r, struct object *scene)
{
        double travel_dist = 0;
        double scene_dist;
        int i;
        color out = 0;
        for (i = 0; (i < MAX_ITERATIONS) && (travel_dist < DRAW_DIST); i++) {
                scene_dist = scene->sol->dist(&r->pos);

                if (scene_dist < EPSILON) { /* we've hit an object */
                         out = scene->col(*r);
                         break;
                }
        }

        /* no colour reached */
        return (struct pixel_info) {
                .flags = 1, 
                .iterations = i, 
                .col = out, 
                .travel_dist = travel_dist, 
                .scene_dist = scene_dist
                }; 
}
