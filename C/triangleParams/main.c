#include<stdio.h>
#include<math.h>

struct object {
    const char key;
    float value;
};

float getTrianglePerimeter(float a, float b, float c) {
    float perimeter = a + b + c;
    return perimeter;
}
float getTriangleArea(float a, float b, float c, float p) {
    float area = sqrt(p * (p - a) * (p - b) * (p - c));
    return area;
}
float getTriangleHeight(float area, float side) {
    float height = 2 * area / side;
    return height;
}
float getTriangleMedian(float a, float b, float c) {
    float median = 0.5 * sqrt(2 * pow(b, 2) + 2 * pow(c, 2) - pow(a, 2));
    return median;
}
float getTriangleBisection(float a, float b, float c, float p) {
    float bisection = (2 / (b + c)) * sqrt(b * c * p * (p - a));
    return bisection;
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
void clearInput() {
    int input;
    while ((input = getchar()) != '\n' && input != EOF);
}
float onTriangleSideValue() {
    float sideValue = 0;
    const float maxFloatNumber = 3.4e38;
    if (scanf("%f", &sideValue) != 1) {
        printf("Error: Invalid value, please enter a valid number\n");
        clearInput();
        return 0;
    }
    if (sideValue == 0) {
        printf("Program stopped\n");
        return -1;
    } else if (sideValue < 0) {
        printf("Error: Number must be positive only\n");
        return 0;
    } else if (sideValue > maxFloatNumber) {
        printf("Error: Number must be less than %f\n", maxFloatNumber);
        return 0;
    }
    return sideValue;
}
int inputTriangleSides(float* a, float* b, float* c) {
    struct object triangleSides[] = {
        {'a', 0},
        {'b', 0},
        {'c', 0},
    };
    int triangleSidesLength = 0;
    if (sizeof(triangleSides) > 0) {
        triangleSidesLength = sizeof(triangleSides) / sizeof(triangleSides[0]);
    } else {
        return 0;
    }
    for (int i = 0; i < triangleSidesLength; ++i) {
       float side = 0; 
        while (side == 0) {
            printf(" Input side %c: ", triangleSides[i].key);
            side = onTriangleSideValue();
            if (side == -1) {
                return -1;
            }
            triangleSides[i].value = side;
        }
    }
    *a = triangleSides[0].value;
    *b = triangleSides[1].value;
    *c = triangleSides[2].value;
    return 1;
}
int isThisTriangleExists(float a, float b, float c) {
    if (a + b <= c || a + c <= b || b + c <= a) {
        printf("Error: This triangle does not exist (sum of two sides should be more than third side)\n");
        return 0;
    }
    return 1;
}
int startProgram() {
    float a, b, c = 0; // a, b, c - triangle sides

    printf("\nInput triangle sides (from 0 to 3.4e38) to get triangle parameters. Input 0 to stop program\n");
    if (inputTriangleSides(&a, &b, &c) == -1) {
        return -1;
    }

    if (isThisTriangleExists(a, b, c) == 0) {
        return 0;
    }

    printf("\nTriangle Parameters\n");
    outputTriangleParameters(a, b, c);

    return 0;
}
int main() {
    while (1) {
        if (startProgram() == -1) {
            break;
        }
    }
    return 0;
}