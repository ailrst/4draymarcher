#include "scene.h"

struct scene_objects {
    int num_objects;
    struct object* objects;
};

struct scene_objects scene;

struct colour 
colour_function(struct ray* ray, struct object* obj) 
{
    if (scene.num_objects == 0) {
        return (struct colour) {};
    }

    struct colour scene_colour = scene.objects[0].col(ray, &scene.objects[0]);
    double min = DBL_MAX;
    for (int i = 0; i < scene.num_objects; i++) {
        double distance = solid_dist(&(scene.objects[i].sol), ray->pos);
        if (distance < min) {
            min = distance;
            scene_colour = scene.objects[i].col(ray, &scene.objects[i]);
        }
    }

    return scene_colour;
}

double 
distance_function(struct vec* position) 
{
    double min = DBL_MAX;
    for (int i = 0; i < scene.num_objects; i++) {
        double distance = solid_dist(&(scene.objects[i].sol), position);
        if (distance < min) {
            min = distance;
        }
    }

    return min;
}

struct object *
new_scene(int num_scene_objects, struct object* scene_objects) 
{
    scene.num_objects = num_scene_objects;
    scene.objects = scene_objects;

    // for (int i = 0; i < num_scene_objects; i++) {
    //     place(&(scene.objects[i].sol));
    // }

    struct object *scene_rep = malloc(sizeof(struct object));
    scene_rep->sol.dist = distance_function;
    scene_rep->sol.op = B_ADD;
    scene_rep->sol.rotation = 0;
    scene_rep->sol.scale = 1;

    scene_rep->col = colour_function;

    return scene_rep;
}


