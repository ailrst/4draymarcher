#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "camera.h"

extern struct scene_objects scene;

struct object new_scene(int, struct object*);

#endif