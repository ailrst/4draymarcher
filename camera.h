
#include "types.h"
#include "math.h"
#include "vect.h"
#include "distfuncs.h"
#include "scene.h"

#ifndef CAMH
#define CAMH

struct pixel_info march(struct ray *r, struct object *scene);
Uint32 process_pixel(int, int);

double solid_dist(struct solid *s, struct vec *v);
void place(struct solid *v);
struct vec * estimateNormal(struct vec *r, struct solid *sol);

#endif
