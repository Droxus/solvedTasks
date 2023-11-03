#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define maxIterations 1e+4
#define defaultTolerance 1e-5
#define MIN_POSITIVE_FLOAT 1e-18
#define EXIT_NUMBER_NEGATIVE -1

void swapElements(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

void sortArray(float *array, int length) {
    for (int i = 0; i < length-1; i++)
        for (int j = i+1; j < length; j++)
            if (array[i] > array[j])
                swapElements(&array[i], &array[j]);
}

void outputArray(float *array, int length, int isReversed) {
    printf("[ ");

    if (isReversed) {
        for (int i = length-1; i >= 0; i--)
            printf(" %.3g ", array[i]);
    } else {
        for (int i = 0; i < length; i++)
            printf(" %.3g ", array[i]);
    }

    printf(" ]\n\n");
}

float* getGeneratedArray(int length) {
    float* array = (float*)malloc(length * sizeof(float));
    const int maxPossibleNum = 100;

    rand(); // to prevent first same value

    for (int i = 0; i < length; i++)
        array[i] = (((float)rand() / RAND_MAX) * maxPossibleNum) - (maxPossibleNum/2);

    return array;
}

bool arraySortingMode() {
    int length = 0;

    printf("\nARRAY SORTING MODE\n");
    printf("\nEnter length of array\n");
    printf("(value must be integer from %i to %g)\n", 0, maxIterations);
    printf(" -- Enter 0 - to stop progam, -1 - to main menu --\n");

    printf("Your input: ");
    const int scanfResult = scanf("%i", &length);
    fflush(stdin);
    
    if (scanfResult != 1) {
        printf("Error: invalid format\n");
        return arraySortingMode();
    }

    if (!length) {
        return false;
    } else if (length == EXIT_NUMBER_NEGATIVE) {
        return true;
    } else if (length < 0 || length > maxIterations) {
        printf("Error: invalid format\n");
        return arraySortingMode();
    }

    srand((unsigned)time(NULL));

    float* array = getGeneratedArray(length);

    printf("\nRandomly generated array: \t");
    outputArray(array, length, 0);

    printf("Sorted array: \t\t\t");
    sortArray(array, length);
    outputArray(array, length, 0);

    printf("Sorted Reversed array: \t\t");
    outputArray(array, length, 1);

    free(array);

    return arraySortingMode();
}

double normalizeAngle(double angle) {
    return fabs(angle) - (int)(fabs(angle) / (2 * M_PI)) * (2 * M_PI);
}

double mathFactorial(int number) {
    number = (number < 0) ? -number : number;
    return (number == 0) ? 1 : number * mathFactorial(number - 1);
}

double customCos(double angle, double tolerance) {
    angle = normalizeAngle(angle);
    double result = 1.0, currentTolerance = 1.0;
    int sign = 1;

    if (fabs(angle) > M_PI) {
        sign = -1;
        angle = (angle / fabs(angle)) * (angle - M_PI);
    }

    for (int i = 2; i <= maxIterations && fabs(currentTolerance) > tolerance; i += 2) {
        currentTolerance = pow(angle, i) / mathFactorial(i);
        result += (i % 4 == 0 ? 1 : -1) * currentTolerance;
    }

    return result * sign;
}

void outputCosinusTable(double min, double max, double step, double tolerance) {
    printf("\t---degree---\t\t\t-----cos(x)-----\t-----cos(Taylor)-----\t\t---difference---\n");

    for (double deg = min, i = 0; i <= maxIterations &&  deg <= max; deg += step, i++) {
        const double angle = deg * M_PI / 180;
        const double mathCos = cos(angle);
        const double myCos = customCos(angle, tolerance);
        const double difference =  mathCos - myCos;

        const int decimalPlaces = log10((float)1.0 / tolerance);
        printf(
            "\t%lf\t\t\t%.*lf\t\t\t%.*lf\t\t\t\t%.*lf\n",
            deg, decimalPlaces, mathCos, decimalPlaces, myCos, decimalPlaces, difference
        );
    }
}

int isValidCosCalculating(double scanfResult, double x1, double x2, double step, double tolerance, double min, double max) {
    if (
        !(x1 || x2 || step)
        && (scanfResult == 1)
    ) {
        return 0;
    }

    if (
        (x1 == EXIT_NUMBER_NEGATIVE)
        && !(x2 || step)
        && (scanfResult == 1)
    ) {
        return EXIT_NUMBER_NEGATIVE;
    }

    if (scanfResult < 3) {
        printf("Error: invalid format\n");
        return 2;
    }

    if (tolerance < MIN_POSITIVE_FLOAT || tolerance > 1) {
        printf("Error: tolerance must be between %.1e and %d\n", MIN_POSITIVE_FLOAT, 1);
        return 2;
    }

    if (step < 0) {
        step = -step;
        printf("Warning: step can be positive only\n");
    } else if (step == 0) {
        printf("Error: step can not be zero\n");
        return 2;
    }

    if (fabs((max - min) / step) > maxIterations) {
        printf("Error: too much iterations. Try to increase step");
        return 2;
    }

    return 1;
}

bool cosCalculatingMode() {
    double x1 = 0.0, x2 = 0.0, step = 0.0, tolerance = defaultTolerance;

    printf("\nCOSINUS CALCULATION MODE\n");
    printf("\nEnter value in format: 0, 180, 15, 1e-5\n1st - x1; 2nd - x2; 3rd - step; 4th - tolerance(optional)\n");
    printf("Warning: default tolerance = 1e-5\n");
    printf(" -- Enter 0 - to stop progam, -1 - to main menu --\n");

    printf("Your input: ");
    const int scanfResult = scanf("%lf, %lf, %lf, %lf", &x1, &x2, &step, &tolerance);
    fflush(stdin);

    double min = fmin(x1, x2);
    double max = fmax(x1, x2);

    const int validationResult = isValidCosCalculating(scanfResult, x1, x2, step, tolerance, min, max);

    if (!validationResult) {
        return false;
    } else if (validationResult == EXIT_NUMBER_NEGATIVE) {
        return true;
    } else if (validationResult == 2) {
        return cosCalculatingMode();
    }

    outputCosinusTable(min, max, step, tolerance);

    return cosCalculatingMode();
}

bool stopProgram() {
    return false;
}

bool runProgram() {
    bool (*modes[])() = {stopProgram, cosCalculatingMode, arraySortingMode};
    size_t totalSize = sizeof(modes);
    const int modesNumber = totalSize ? totalSize / sizeof(*modes) : totalSize;
    int modeIndex = 0;

    printf("\nSelect program mode:\n");
    printf("-- Enter number 0 to stop program --\n");
    printf(" 1) Enter number 1 to select COS CALCULATING MODE\n");
    printf(" 2) Enter number 2 to select ARRAY SORTING MODE\n");

    printf("Your input: ");
    const int scanfResult = scanf("%i", &modeIndex);
    fflush(stdin);
    if (!scanfResult) {
        printf("Invalid value: enter integer number (0, 1, 2)\n");
        return true;
    }

    const int modeLastIndex = modesNumber - 1;
    const int modeFirstIndex = 0;
    if (modeFirstIndex > modeIndex || modeIndex > modeLastIndex) {
        printf("\nError: this mode does not exist\n");
        return true;
    }

    return modes[modeIndex]();
}

int main() {
    while (runProgram());

    return 0;
}