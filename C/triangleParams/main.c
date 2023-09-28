#include<stdio.h>
#include<math.h>
#include<stdbool.h>
#include<float.h> 

#define EXIT_NUMBER_NEGATIVE -1
#define EXIT_FAILURE 0
#define FLT_MIN_POSITIVE 0

float getTrianglePerimeter(float a, float b, float c) {
    return a + b + c;
}
float getTriangleArea(float a, float b, float c, float semiPerimeter) {
    return sqrt(semiPerimeter * (semiPerimeter - a) * (semiPerimeter - b) * (semiPerimeter - c));
}
float getTriangleHeight(float area, float side) {
    return 2 * area / side;
}
float getTriangleMedian(float a, float b, float c) {
    return 0.5 * sqrt(2 * pow(b, 2) + 2 * pow(c, 2) - pow(a, 2));
}
float getTriangleBisection(float a, float b, float c, float semiPerimeter) {
    return (2 / (b + c)) * sqrt(b * c * semiPerimeter * (semiPerimeter - a));
}
void outputTriangleParameters(float a, float b, float c) {
    float perimeter = getTrianglePerimeter(a, b, c);
    printf(" Perimeter: %g\n", perimeter);

    float area = getTriangleArea(a, b, c, perimeter/2);
    printf(" Area: %g\n", area);

    float heightA = getTriangleHeight(area, a);
    float heightB = getTriangleHeight(area, b);
    float heightC = getTriangleHeight(area, c);
    printf(" Height to side: {a: %g, b: %g, c: %g}\n", heightA, heightB, heightC);

    float medianA = getTriangleMedian(a, b, c);
    float medianB = getTriangleMedian(b, a, c);
    float medianC = getTriangleMedian(c, b, a);
    printf(" Median to side: {a: %g, b: %g, c: %g}\n", medianA, medianB, medianC);

    float bisectionA = getTriangleBisection(a, b, c, perimeter/2);
    float bisectionB = getTriangleBisection(b, a, c, perimeter/2);
    float bisectionC = getTriangleBisection(c, b, a, perimeter/2);
    printf(" Bisection to side: {a: %g, b: %g, c: %g}\n", bisectionA, bisectionB, bisectionC);
}
float getValidSideValue(float sideValue) {
    if (!sideValue) {
        printf("Program stopped\n");
        return EXIT_FAILURE;
    } else if (sideValue < FLT_MIN_POSITIVE) {
        printf("Error: Number must be positive only\n");
        return EXIT_NUMBER_NEGATIVE;
    } else if (sideValue > FLT_MAX) {
        printf("Error: Number must be less than %e\n", FLT_MAX);
        return EXIT_NUMBER_NEGATIVE;
    }
    return sideValue;
}
float getTriangleSideValue(char sideName) {
    float sideValue = 0;
    printf(" Input side %c: ", sideName);
    const int scanfResult = scanf("%f", &sideValue);
    fflush(stdin);
    if (!scanfResult) {
        printf("Error: Invalid value, please enter a valid number\n");
        return EXIT_NUMBER_NEGATIVE;
    }
    return getValidSideValue(sideValue);
}
bool inputTriangleSides(float* a, float* b, float* c) {
    struct { const char key; float **value; } triangleSides[] = { {'a', &a}, {'b', &b}, {'c', &c} };
    size_t totalSize = sizeof(triangleSides);
    int triangleSidesLength = totalSize ? totalSize / sizeof(*triangleSides) : totalSize;
    for (
        int i = 0;
        i < triangleSidesLength && **triangleSides[i].value;
        **triangleSides[i].value = getTriangleSideValue(triangleSides[i].key), i += **triangleSides[i].value > 0 ? 1 : 0
    );
    return *a && *b && *c;
}
bool startProgram() {
    float a = 1, b = 1, c = 1; // a, b, c - triangle sides

    printf("\nInput triangle sides (from %i to %.1e) to get triangle parameters", FLT_MIN_POSITIVE, FLT_MAX);
    printf("\n--- Input 0 to stop program ---\n");
    if (!inputTriangleSides(&a, &b, &c)) {
        return false;
    }

    const bool isTriangleValid = a + b > c && a + c > b && b + c > a;
    if (!isTriangleValid) {
        printf("Error: This triangle does not exist (sum of two sides should be more than third side)\n");
        return true;
    }

    printf("\nTriangle Parameters\n");
    outputTriangleParameters(a, b, c);

    return true;
}
int main() {
    while (startProgram());
    return EXIT_FAILURE;
}