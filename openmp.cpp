#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MATRIX_SIZE 1000000
#define BATCH_SIZE 100     

typedef struct {
    int *data;  
    int rows;  
    int columns; 
} Matrix;

Matrix create_matrix(int rows, int columns) {
    Matrix matrix;
    matrix.rows = rows;
    matrix.columns = columns;
    matrix.data = (int *)malloc(rows * columns * sizeof(int));
    return matrix;
}
void free_matrix(Matrix matrix) {
    free(matrix.data);
}
void print_matrix(Matrix matrix) {
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.columns; j++) {
            printf("%d ", matrix.data[i * matrix.columns + j]);
        }
        printf("\n");
    }
}
void initialize_matrix(Matrix matrix) {
    for (int i = 0; i < matrix.rows * matrix.columns; i++) {
        matrix.data[i] = rand() % 10;
    }
}
int is_empty_row(int *row, int size) {
    for (int i = 0; i < size; i++) {
        if (row[i] != 0) {
            return 0;
        }
    }
    return 1;
}
void eliminate_rows(int batch_start, int batch_end, Matrix matrix) {
    for (int i = batch_start; i < batch_end; i++) {
        int first_non_zero = i % matrix.columns;
        if (first_non_zero >= batch_start && first_non_zero < batch_end) {

            if (!is_empty_row(matrix.data + i * matrix.columns, matrix.columns)) {
                int *eliminating_row = matrix.data + i * matrix.columns;
                for (int j = i + 1; j < matrix.rows; j++) {
                    int *current_row = matrix.data + j * matrix.columns;
                    if (current_row[first_non_zero] == 1) {
                        for (int k = 0; k < matrix.columns; k++) {
                            current_row[k] = current_row[k] ^ eliminating_row[k];
                        }
                    }
                }
            }
        } else {
            int *leading_term_row = matrix.data + i * matrix.columns;
            leading_term_row[first_non_zero] = 1;
        }
    }
}
int main() {
    Matrix matrix = create_matrix(MATRIX_SIZE, MATRIX_SIZE);
    initialize_matrix(matrix);
    int num_batches = MATRIX_SIZE / BATCH_SIZE;
    for (int i = 0; i < num_batches; i++) {
        int batch_start = i * BATCH_SIZE;
        int batch_end = (i + 1) * BATCH_SIZE;
        if (batch_end > MATRIX_SIZE) {
            batch_end = MATRIX_SIZE;
        }

        eliminate_rows(batch_start, batch_end, matrix);
    }
    free_matrix(matrix);

    return 0;
}




