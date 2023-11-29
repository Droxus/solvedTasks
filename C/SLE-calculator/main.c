#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#define MAX_ITERATIONS 1e3
#define MIN_TOLERANCE 1e-30
#define MAX_TOLERANCE 1
#define MIN_MATRIX_SIZE 2
#define MAX_MATRIX_SIZE 100
#define MIN_FLOAT -1e+38
#define MAX_FLOAT 1e+38

void printVector(double *vector, int size) {
    for (int i = 0; i < size; i++)
        printf("x%d: %g, ", i+1, vector[i]);

    printf("\n");
}

int isDiagonallyDominant(int size, double **matrix) {
    for (int i = 0; i < size; i++) {
        double diagonal = fabs(matrix[i][i]);
        double rowSum = 0.0;

        for (int j = 0; j < size; j++)
            if (j != i)
                rowSum += fabs(matrix[i][j]);

        if (diagonal <= rowSum)
            return 0;
    }

    return 1;
}

void getValue(char* msg, char* scanfHolder, void *value, ...) {
    int scanfResult = 0;
    va_list args;
    va_start(args, value);

    do {
        vprintf(msg, args);
        scanfResult = scanf(scanfHolder, value);
        fflush(stdin);
    } while (scanfResult != 1);

    va_end(args);
}

int isInRange(double value, double min, double max) {
    if (value >= min && value <= max)
        return 1;
    return 0;
}

void solveSLE(int size, double tolerance, double **A, double *B, double *X) {
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double *newX = (double *)malloc(size * sizeof(double));

        for (int i = 0; i < size; i++) {
            newX[i] = B[i];
            for (int j = 0; j < size; j++)
                if (i != j)
                    newX[i] -= A[i][j] * X[j];

            newX[i] /= A[i][i];
        }

        double maxDiff = 0.0;
        for (int i = 0; i < size; i++) {
            double diff = fabs(newX[i] - X[i]);
            X[i] = newX[i];
            if (diff > maxDiff)
                maxDiff = diff;
        }

        free(newX);

        printf("Iteration %d: ", iter + 1);
        printVector(X, size);

        if (maxDiff < tolerance) {
            printf("Converged.\n");
            break;
        }
    }
}

int inputSettings(int *size, double *tolerance, double *stopValue) {
    char* sizeMsg = "Input amount of unknows (%d, %d): ";
    do getValue(sizeMsg, "%d", size, MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
    while (!isInRange(*size, MIN_MATRIX_SIZE, MAX_MATRIX_SIZE) && *size != 0);
    if (!*size) return 0;

    char* toleranceMsg = "Input tolerance (%.0e, %d): ";
    do getValue(toleranceMsg, "%lf", tolerance, MIN_TOLERANCE, MAX_TOLERANCE);
    while (!isInRange(*tolerance, MIN_TOLERANCE, MAX_TOLERANCE) && *tolerance != 0.0);
    if (!*tolerance) return 0;

    char* stopValueMsg = "Enter a stop value by which you can stop the program at any time (%.0e, %.0e): ";
    do getValue(stopValueMsg, "%lf", stopValue, MIN_FLOAT, MAX_FLOAT);
    while (!isInRange(*stopValue, MIN_FLOAT, MAX_FLOAT) && *size != *stopValue);

    return 1;
}

void clearMatrixes(int size, double **A, double *B, double *X) {
    free(X);
    free(B);
    
    for (int i = 0; i < size; i++)
        free(A[i]);

    free(A);
}

int matrixInput(int size, double stopValue, double **A, double *B) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            do getValue("Enter A[%d, %d]: ", "%lf", &A[i][j], i+1, j+1);
            while (!isInRange(A[i][j], MIN_FLOAT, MAX_FLOAT));
            if (A[i][j] == stopValue) 
                return 0;
        }
        do getValue("Enter B[%d]: ", "%lf", &B[i], i+1);
        while (!isInRange(B[i], MIN_FLOAT, MAX_FLOAT));
        if (B[i] == stopValue) 
            return 0;
    }

    return 1;
}

int runProgram() {
    int size = 0;
    double tolerance = 0.0, stopValue = 0.0;
    double **A, *B, *X;

    printf("\n\nSLE CALCULARTOR\n Enter 0 to stop program\n\n");

    if (!inputSettings(&size, &tolerance, &stopValue))
        return 0;

    A = (double **)malloc(size * sizeof(double *));
    B = (double *)malloc(size * sizeof(double));
    X = (double *)malloc(size * sizeof(double));

    for (int i = 0; i < size; i++) {
        A[i] = (double *)malloc(size * sizeof(double));
        X[i] = 0.0;
    }

    if (!matrixInput(size, stopValue, A, B))
        return 0;

    if (!isDiagonallyDominant(size, A)) {
        printf("The matrix is not diagonally dominant. Iterative methods may not converge reliably.\n");
        clearMatrixes(size, A, B, X);
        return 1;
    }

    solveSLE(size, tolerance, A, B, X);

    printf("\nResult: ");
    const int decimalPlaces = log10(1.0 / tolerance);
    for (int i = 0; i < size; i++)
        printf("x%d: %.*g, ", i+1, decimalPlaces, X[i]);

    clearMatrixes(size, A, B, X);

    return 1;
}

int main() {
    while (runProgram());

    return 0;
}
