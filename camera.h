struct pixel_info march(struct ray *r, struct object *scene);
Uint32 process_pixel(int, int);

double solid_dist(struct solid *s, struct vec *v);