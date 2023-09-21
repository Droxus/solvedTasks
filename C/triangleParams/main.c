#include<stdio.h>
#include<math.h>
#include<stdbool.h>
#include<float.h> 

#define EXIT_NUMBER_NEGATIVE -1
#define EXIT_FAILURE 0
#define FLT_MIN_POSITIVE 0

struct object {
    const char key;
    float value;
};

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
    float area, perimeter,
    heightA, medianA, bisectionA,
    heightB, medianB, bisectionB,
    heightC, medianC, bisectionC;

    perimeter = getTrianglePerimeter(a, b, c);
    printf(" Perimeter: %g\n", perimeter);

    area = getTriangleArea(a, b, c, perimeter/2);
    printf(" Area: %g\n", area);

    heightA = getTriangleHeight(area, a);
    heightB = getTriangleHeight(area, b);
    heightC = getTriangleHeight(area, c);
    printf(" Height to side: {a: %g, b: %g, c: %g}\n", heightA, heightB, heightC);

    medianA = getTriangleMedian(a, b, c);
    medianB = getTriangleMedian(b, a, c);
    medianC = getTriangleMedian(c, b, a);
    printf(" Median to side: {a: %g, b: %g, c: %g}\n", medianA, medianB, medianC);

    bisectionA = getTriangleBisection(a, b, c, perimeter/2);
    bisectionB = getTriangleBisection(b, a, c, perimeter/2);
    bisectionC = getTriangleBisection(c, b, a, perimeter/2);
    printf(" Bisection to side: {a: %g, b: %g, c: %g}\n", bisectionA, bisectionB, bisectionC);
}
float onTriangleSideValue() {
    float sideValue = 0;
    if (!scanf("%f", &sideValue)) {
        printf("Error: Invalid value, please enter a valid number\n");
        fflush(stdin);
        return EXIT_NUMBER_NEGATIVE;
    }
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
bool inputTriangleSides(float* a, float* b, float* c) {
    struct object triangleSides[] = {
        {'a', 0},
        {'b', 0},
        {'c', 0},
    };
    size_t totalSize = sizeof(triangleSides);
    int triangleSidesLength = totalSize ? totalSize / sizeof(*triangleSides) : totalSize;
    for (int i = 0; i < triangleSidesLength; ++i) {
       float side;
       do {
            printf(" Input side %c: ", triangleSides[i].key);
            side = onTriangleSideValue();
            if (!side) {
                return false;
            }
            triangleSides[i].value = side;
       } while (side == EXIT_NUMBER_NEGATIVE);
    }
    *a = triangleSides[0].value;
    *b = triangleSides[1].value;
    *c = triangleSides[2].value;
    return true;
}
bool startProgram() {
    float a = 0, b = 0, c = 0; // a, b, c - triangle sides

    printf("\nInput triangle sides (from %i to %.1e) to get triangle parameters. Input 0 to stop program\n",
    FLT_MIN_POSITIVE, FLT_MAX);
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
    while (startProgram()) {}
    return EXIT_FAILURE;
}