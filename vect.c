#include "vect.h"
#include <math.h>
#include <float.h>

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

struct vec* do_on_vec_ip(struct vec * v, double (*func)(double)) {

    for (int i = 0; i < v->dimension; i++) {
        v->elements[i] = func(v->elements[i]);
    }

    return v;
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
 * Copies the given vector into the newly malloc'd vector.
 */
struct vec* 
copy_vec(struct vec* to_copy) 
{
    struct vec* new_vector = new_vec(to_copy->dimension);
    for (int i = 0; i < to_copy->dimension; i++) {
        new_vector->elements[i] = to_copy->elements[i];
    }

    return new_vector;
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
    struct vec* new_vector = new_vec(4);
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
        sum_of_squares +=a->elements[i] * a->elements[i];
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
 * Calculate the dot product of vec a and vec b.
 */
double
dot_product_vec(struct vec* a, struct vec* b) 
{
    double result = 0;
    for (int i = 0; i < a->dimension; i++) {
        result += a->elements[i] * b->elements[i];
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

double 
vec_max(const struct vec *v) 
{
    double max = -DBL_MAX;

    for (int i = 0; i < v->dimension; i++) {
        if (i > max)
            max = v->elements[i];
    }

    return max;
}

double 
vec_min(const struct vec *v) 
{
    double min = DBL_MAX;

    for (int i = 0; i < v->dimension; i++) {
        if (i < min)
            min = v->elements[i];
    }

    return min;
}

/**
 * Creates a new matrix with the given number of rows and columns
 */
struct mat2*
new_mat(int num_rows, int num_cols) 
{
    struct mat2* new_matrix = calloc(1, sizeof(struct mat2));
    new_matrix->num_rows = num_rows;
    new_matrix->num_cols = num_cols;
    new_matrix->elements = calloc(num_rows, sizeof(double*));
    for (int r = 0; r < num_rows; r++) {
        new_matrix->elements[r] = calloc(num_cols, sizeof(double));
    }

    return new_matrix;
}

void
free_mat(struct mat2* to_free) 
{
    for (int r = 0; r < to_free->num_rows; r++) {
        free(to_free->elements[r]);
    }
    free(to_free->elements);
    free(to_free);
}

/**
 * Creates a matrix from the given vectors.
 * Each vector should have the same number of elements.
 */
struct mat2*
new_mat_from_vecs(int num_vectors, struct vec** vectors) 
{
    if (num_vectors == 0 || vectors[0]->dimension == 0) {

    }

    struct mat2* new_matrix = new_mat(num_vectors, num_vectors + 1);
    for (int r = 0; r < new_matrix->num_rows; r++) {
        for (int c = 0; c < new_matrix->num_cols; c++) {
            new_matrix->elements[r][c] = vectors[r]->elements[c];
        }
    }

    return new_matrix;
}

/**
 * Returns a copy of the given matrix ignoring the given row and column.
 * This is useful for the Laplace expansion where a certain cell in the matrix
 * is multiplied by the matrix given by ignoring the row and column it's in.
 */
struct mat2*
get_determinant_sub_mat(int ignore_col, int ignore_row, struct mat2* matrix) 
{
    int new_num_rows = matrix->num_rows - 1;
    int new_num_cols = matrix->num_cols - 1;
    if (ignore_row == -1) {
        new_num_rows = matrix->num_rows;
    }

    if (ignore_col == -1) {
        new_num_cols = matrix->num_cols;
    }

    struct mat2* new_matrix = new_mat(new_num_rows, new_num_cols);
    int row_ignored = 0;
    int col_ignored = 0;
    for (int r = 0; r < matrix->num_rows; r++) {
        for (int c = 0; c < matrix->num_cols; c++) {
            // Very dodgy stuff going on here
            if (c == ignore_col) {
                col_ignored = 1;
            }

            if (r == ignore_row) {
                row_ignored = 1;
            }

            int next_col = c;
            int next_row = r;

            if (col_ignored == 1) {
                next_col = c - 1;
            }

            if (row_ignored == 1) {
                next_row = r - 1;
            }

            if (c != ignore_col && r != ignore_row) {
                new_matrix->elements[next_row][next_col] = matrix->elements[r][c];
            }
        }

        col_ignored = 0;
    }

    return new_matrix;
}

/**
 * Finds the determinant of an n*n matrix using the Laplace expansion 
 * https://en.wikipedia.org/wiki/Laplace_expansion
 */
double 
calc_determinant_mat2(struct mat2* matrix) 
{
    if (matrix->num_rows != matrix->num_cols) {
        // Do something here bc this is impossible.
    }

    if (matrix->num_rows == 2 && matrix->num_cols == 2) {
        return matrix->elements[0][0] * matrix->elements[1][1] -
                matrix->elements[0][1] * matrix->elements[1][0];
    }

    double det = 0;
    for (int c = 0; c < matrix->num_cols; c++) {
        // Even is added and odd is subtracted according to Laplace expansion
        struct mat2* sub_mat = get_determinant_sub_mat(c, 0, matrix);
        if (c % 2 == 0) {
            det += matrix->elements[0][c] * calc_determinant_mat2(sub_mat); 
        } else {
            det -= matrix->elements[0][c] * calc_determinant_mat2(sub_mat); 
        }

        free_mat(sub_mat);
    }

    return det;
}

/**
 * Takes an array of n vectors and returns a vector perpendicular to all of them.
 * 
 * Creates an array of vectors (v1, v2, ..., vn) and creates a determinant
 * matrix to find the cross-product between all of them. 
 * 
 * That is for 2 vectors of dimension 3:
 * |i   j   k  |
 * |v11 v12 v13|
 * |v21 v22 v23|
 * 
 * The perpendicular vector = (i, j, k, etc.) where 
 * i = |v12 v13|
 *     |v22 v23|,
 *  
 * j = |v11 v13| 
 *     |v21 v23|
 *                                                           
 * and so on.
 */
struct vec*
perpendicular_vec(int num_vectors, struct vec** vectors)
{
    if (num_vectors == 0 || vectors[0] == NULL) {
        // This shouldnt happen
    }
    if (num_vectors + 1 != vectors[0]->dimension) {
        // This shouldnt happen
    }

    struct mat2* matrix = new_mat_from_vecs(num_vectors, vectors);
    struct vec* perpendicular = new_vec(vectors[0]->dimension);
    for (int i = 0; i < num_vectors + 1; i++) {
        struct mat2* sub_mat = get_determinant_sub_mat(i, -1, matrix);
        if (i % 2 == 0) {
            perpendicular->elements[i] = calc_determinant_mat2(sub_mat);
        } else {
            perpendicular->elements[i] = -calc_determinant_mat2(sub_mat);
        }

        free_mat(sub_mat);
    }
    
    free_mat(matrix);
    return perpendicular;
}


