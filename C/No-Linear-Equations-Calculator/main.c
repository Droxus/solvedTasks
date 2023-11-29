#include <stdio.h>
#include <math.h>
#include <stdarg.h>

#define MAX_ITERATIONS 1e5
#define ZERO_LIMIT 1e-10
#define MIN_FLOAT -1e+38
#define MAX_FLOAT 1e+38
#define MIN_TOLERANCE 1e-30
#define MAX_TOLERANCE 1

double func1(double x, double t) {
    return cos(t / x) - 2.0 * sin(1.0 / x) + 1.0 / x;
}

double func2(double x, double t) {
    return sin(log(x)) - cos(log(x)) + t * log(x);
}

double derivative(double (*func)(double, double), double x, double t) {
    return (func(x + ZERO_LIMIT, t) - func(x, t)) / ZERO_LIMIT;
}

double bisection(double (*func)(double, double), double a, double b, double t, double e) {
    double x = a;

    if (func(a, t) * func(b, t) >= 0)
        return NAN;

    for (int i = 0; i < MAX_ITERATIONS; ++i) {
        if ((b - a) < e)
            return x;

        x = (a + b) / 2.0;

        if (func(x, t) == 0.0)
            return NAN;

        if (func(x, t) * func(a, t) > 0)
            a = x;
        else
            b = x;
    }

    return NAN;
}

double newtonRaphson(double (*func)(double, double), double x0, double t, double e) {
    double x = x0;

    for (int i = 0; i < MAX_ITERATIONS; ++i) {
        double x1 = x - func(x, t) / derivative(func, x, t);

        if (fabs(x1 - x) < e)
            return x1;

        x = x1;
    }
    
    return NAN;
}

void getValue(char* msg, int argsAmount, char* scanfHolder, ...) {
    int scanfResult = 0;
    va_list value;
    va_start(value, scanfHolder);

    do {
        printf("%s", msg);
        scanfResult = vscanf(scanfHolder, value);
        fflush(stdin);
    } while (scanfResult != argsAmount);

    va_end(value);
}

int isInRange(double value, double min, double max) {
    if (value >= min && value <= max)
        return 1;
    return 0;
}

int inputSettings(int* equation, int* method, float* a1, float* a2, float* t, double* e) {
    char* msg = "";

    printf("\nTip: Enter 0 to stop program at any time\n\n");

    msg = "Select equation (1, 2):\n [1] cos(t/x) - 2sin(1/x) + 1/x\n [2] sin(ln(x)) - cos(ln(x)) + tln(x)\nInput: ";
    do getValue(msg, 1, "%d", equation);
    while (!isInRange(*equation, 1, 2) && *equation != 0);
    if (*equation == 0) 
        return 0;
    
    msg = "Select solving method (1, 2):\n [1] Bisection\n [2] Newton-Raphson\nInput: ";
    do getValue(msg, 1, "%d", method);
    while (!isInRange(*method, 1, 2) && *method != 0);
    if (*method == 0) 
        return 0;
    
    msg = "Enter interval [a1, a2]: ";
    do getValue(msg, 2, "%f, %f", a1, a2);
    while (!isInRange(*a1, MIN_FLOAT, MAX_FLOAT) && *a1 != 0.0 && !isInRange(*a2, MIN_FLOAT, MAX_FLOAT) && *a2 != 0.0);
    if (*a1 == 0 || *a2 == 0) 
        return 0;

    msg = "Enter value of t: ";
    do getValue(msg, 1, "%f", t);
    while (!isInRange(*t, MIN_FLOAT, MAX_FLOAT) && *t != 0.0);
    if (*t == 0) 
        return 0;

    msg = "Enter tolerance: ";
    do getValue(msg, 1, "%lf", e);
    while (!isInRange(*e, MIN_TOLERANCE, MAX_TOLERANCE) && *e != 0.0);
    if (*e == 0) 
        return 0;

    return 1;
}

int runProgram() {
    int equation = 0, method = 0;
    float a1 = 0.0, a2 = 0.0, t = 0.0;
    double e = 0.0, root = 0.0;

    if (!inputSettings(&equation, &method, &a1, &a2, &t, &e))
        return 0;

    double (*func)(double, double) = (equation == 1) ? func1 : func2;
    root = (method == 1) ? bisection(func, a1, a2, t, e) : newtonRaphson(func, (a1 + a2) / 2.0, t, e);

    if (!isnan(root))
        printf("\nRoot of equation: %lf\n\n", root);
    else
        printf("This method doesn't work on this interval\n\n");

    return 1;
}

int main() {
    while(runProgram());

    return 0;
}

