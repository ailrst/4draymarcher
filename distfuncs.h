#ifndef DISTFU
#define DISTFU

struct object new_sphere(double radius);

double sdf_box(struct vec *);
double sdf_hplane(struct vec *);
double sdf_phat_vert_line(struct vec *);
double sdf_sphere(struct vec *);

struct colour yeet_col(struct ray *);

#endif
