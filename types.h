#ifndef TYPES_H
#define TYPES_H

#define B_WINDOW_WIDTH 800
#define B_WINDOW_HEIGHT 600
#define B_INTERNAL_HEIGHT 800
#define B_INTERNAL_WIDTH 600

enum solid_op {
    ADD
};


struct solid {
    enum solid_op op;
    double rotation;
};

#endif
