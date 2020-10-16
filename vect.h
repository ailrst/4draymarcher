#ifndef VECT_H
#define VECT_H

#include <stdio.h>
#include "types.h"

struct nvec* new_nvec(int num_dimensions);
struct nvec* new_vec2(double x, double y);
struct nvec* new_vec3(double x, double y, double z);
struct nvec* new_vec4(double w, double x, double y, double z);

struct nvec* add_nvec(struct nvec* a, struct nvec* b);
struct nvec* subtract_nvec(struct nvec* a, struct nvec* b);
double magnitude_nvec(struct nvec* a);
struct nvec* normalise_nvec(struct nvec* a, struct nvec* b);
struct nvec* dot_product_nvec(struct nvec* a, struct nvec* b);
struct nvec* scalar_multiply_nvec(struct nvec* a, double multiplier);
struct nvec* distance_nvec(struct nvec* a, struct nvec* b);
struct nvec* perpendicular_nvec(struct nvec* a, struct nvec* b);

#endif