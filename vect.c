#include "vect.h"
#include <math.h>

/**
 * Creates a new vec* struct with the given number of dimenions.
 * All elements are set to 0.
 */
struct vec* 
new_vec(int num_dimensions)
{
    struct vec* new_vector = calloc(1,sizeof(struct vec));
    new_vector->dimension = num_dimensions;

    new_vector->elements = calloc(num_dimensions, sizeof(double));

    return new_vector;
}

struct vec*
new_vec_of(int num_dimensions, double value) {
    struct vec* new_vector = new_vec(num_dimensions);
    for (int i = 0; i < num_dimensions; i++) {
        new_vector->elements[i] = value;
    }

    return new_vector;
}

/**
 * Frees all memory related to the given vec.
 */
void 
free_vec(struct vec* a) 
{
    free(a->elements);
    free(a);
}

/**
 * Takes 2 doubles and creates an vec of 2 dimensions.
 */
struct vec* 
new_vec2(double x, double y) 
{
    struct vec* new_vector = new_vec(2);
    new_vector->elements[0] = x;
    new_vector->elements[1] = y;

    return new_vector;
}

/**
 * Takes 3 doubles and creates an vec of 3 dimensions.
 */
struct vec* 
new_vec3(double x, double y, double z) 
{
    struct vec* new_vector = new_vec(3);
    new_vector->elements[0] = x;
    new_vector->elements[1] = y;
    new_vector->elements[2] = z;
    
    return new_vector;
}

/**
 * Takes 4 doubles and creates an vec of 4 dimensions.
 */
struct vec* 
new_vec4(double w, double x, double y, double z) 
{
    struct vec* new_vector = new_vec(2);
    new_vector->elements[0] = w;
    new_vector->elements[1] = x;
    new_vector->elements[2] = y;
    new_vector->elements[3] = z;
    
    return new_vector;
}

/**
 * Adds together two vecs and returns a reference to the summed vec.
 */
struct vec* 
add_vec(struct vec* a, struct vec* b) 
{
    int smallest_dimension = a->dimension < b->dimension ? a->dimension : b->dimension;
    int largest_dimension = a->dimension > b->dimension ? a->dimension : b->dimension;

    // Perform addition up to where the dimensions of both vectors are equal.
    struct vec* result = new_vec(a->dimension);
    for (int i = 0; i < smallest_dimension; i++) {
        result->elements[i] = a->elements[i] + b->elements[i];
    }

    // Assume the smaller array is all 0s if the dimensions aren't equal
    for (int i = smallest_dimension; i < largest_dimension; i++) {
        if (largest_dimension == a->dimension) {
            result->elements[i] = a->elements[i];
        } else if (largest_dimension == b->dimension) {
            result->elements[i] = b->elements[i];
        }
    }

    return result;
}

/**
 * Add vec a to vec b and return the pointer to vec a.
 */
struct vec* 
add_vec_ip(struct vec* a, struct vec* b) 
{
    int smallest_dimension = a->dimension < b->dimension ? a->dimension : b->dimension;
    int largest_dimension = a->dimension > b->dimension? a->dimension : b->dimension;


    for (int i = 0; i < smallest_dimension; i++) {
        a->elements[i] = a->elements[i] + b->elements[i];
    }

    // Assume the smaller array is all 0s if the dimensions aren't equal
    for (int i = smallest_dimension; i < largest_dimension; i++) {
        if (largest_dimension == a->dimension) {
            a->elements[i] = a->elements[i];
        } else if (largest_dimension == b->dimension) {
            a->elements[i] = b->elements[i];
        }
    }

    return a;
}

/**
 * Add vec a to vec b * scaleFactor and store in vec a. Returns a pointer to vec a.
 */
struct vec*
add_scaled_vec_ip(struct vec* a, struct vec* b, double scaleFactor) {
    int smallest_dimension = a->dimension < b->dimension ? a->dimension : b->dimension;
    int largest_dimension = a->dimension > b->dimension? a->dimension : b->dimension;


    for (int i = 0; i < smallest_dimension; i++) {
        a->elements[i] = a->elements[i] + b->elements[i] * scaleFactor;
    }

    // Assume the smaller array is all 0s if the dimensions aren't equal
    for (int i = smallest_dimension; i < largest_dimension; i++) {
        if (largest_dimension == a->dimension) {
            a->elements[i] = a->elements[i];
        } else if (largest_dimension == b->dimension) {
            a->elements[i] = b->elements[i] * scaleFactor;
        }
    }

    return a;
}


/**
 * Subtracts vec b from vec a and returns a reference to the difference vec.
 */
struct vec* 
subtract_vec(struct vec* a, struct vec* b) 
{
    int smallest_dimension = a->dimension < b->dimension ? a->dimension : b->dimension;
    int largest_dimension = a->dimension > b->dimension? a->dimension : b->dimension;

    // Perform subtraction up to where the dimensions of both vectors are equal.
    struct vec* result = new_vec(a->dimension);
    for (int i = 0; i < smallest_dimension; i++) {
        result->elements[i] = a->elements[i] - b->elements[i];
    }

    // Assume the smaller array is all 0s if the dimensions aren't equal
    for (int i = smallest_dimension; i < largest_dimension; i++) {
        if (largest_dimension == a->dimension) {
            result->elements[i] = a->elements[i];
        } else if (largest_dimension == b->dimension) {
            result->elements[i] = - b->elements[i];
        }
    }

    return result;
}

/**
 * Subtracts vec b from vec a and stores the result in vec a, returning a 
 * pointer to it as well.
 */
struct vec* 
subtract_vec_ip(struct vec* a, struct vec* b) 
{
    int smallest_dimension = a->dimension < b->dimension ? a->dimension : b->dimension;
    int largest_dimension = a->dimension > b->dimension? a->dimension : b->dimension;

    // Perform subtraction up to where the dimensions of both vectors are equal.
    for (int i = 0; i < smallest_dimension; i++) {
        a->elements[i] = a->elements[i] - b->elements[i];
    }

    // Assume the smaller array is all 0s if the dimensions aren't equal
    for (int i = smallest_dimension; i < largest_dimension; i++) {
        if (largest_dimension == a->dimension) {
            break; // the elements of a don't need to be changed 
        } else if (largest_dimension == b->dimension) {
            a->elements[i] = - b->elements[i];
        }
    }

    return a;
}

/**
 * Calculates the magnitude of vec a.
 */
double 
magnitude_vec(struct vec* a) 
{
    double sum_of_squares = 0;
    for (int i = 0; i < a->dimension; i++) {
        sum_of_squares += pow(a->elements[i], 2.0);
    }

    return sqrt(sum_of_squares);
}

/**
 * Normalise the given vec by dividing all components by the magnitude.
 */
struct vec* 
normalise_vec(struct vec* a) 
{
    struct vec* result = new_vec(a->dimension);
    double magnitude = magnitude_vec(a);
    for (int i = 0; i < a-> dimension; i++) {
        result->elements[i] = a->elements[i] / magnitude;
    }

    return result;
}

/**
 * Normalise the given vec and store in vec a.
 */
struct vec* 
normalise_vec_ip(struct vec* a) 
{
    double magnitude = magnitude_vec(a);
    for (int i = 0; i < a-> dimension; i++) {
        a->elements[i] = a->elements[i] / magnitude;
    }

    return a;
}

/**
 * Calculate the dot product of vec a and vec b and return a reference to
 * the result.
 */
struct vec* 
dot_product_vec(struct vec* a, struct vec* b) 
{
    struct vec* result = new_vec(a->dimension);
    for (int i = 0; i < a->dimension; i++) {
        result->elements[i] = a->elements[i] * b->elements[i];
    }

    return result;
}

/**
 * Multiply vec a by a scalarFactor and return a new vec with the result.
 */
struct vec* 
scalar_multiply_vec(struct vec* a, double scalarFactor) 
{
    struct vec* result = new_vec(a->dimension);
    for (int i = 0; i < a->dimension; i++) {
        result->elements[i] = a->elements[i] * scalarFactor;
    }

    return result;
}

/**
 * Multiply vec a by a scalarFactor and return vec a with the result.
 */
struct vec* 
scalar_multiply_vec_ip(struct vec* a, double scalarFactor) 
{
    for (int i = 0; i < a->dimension; i++) {
        a->elements[i] *= scalarFactor;
    }

    return a;
}

/**
 * Calculate the distance between vec a and vec b by summing the square of
 * the differences of each component.
 */
double 
distance_vec(struct vec* a, struct vec* b) 
{
    double sum_of_differences = 0;
    for (int i = 0; i < a->dimension; i++) {
        sum_of_differences = pow(a->elements[i] - b->elements[i], 2);
    }

    return sqrt(sum_of_differences);
}
