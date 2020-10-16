#ifndef TYPES_H
#define TYPES_H

struct nvec {
    int dimension;
    double elements[];
};

enum solid_op {
    ADD
};


struct solid {
    enum solid_op op;
    double rotation;
    double scale;
    struct nvec distfunc;
};

#endif
