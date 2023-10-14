#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define maxIterations 1e+4
#define defaultTolerance 1e-5
#define MATH_PI 3.141592653589793
#define EXIT_NUMBER_NEGATIVE -1
#define MAX_SAFE_NUMBER 3e+38
#define MIN_INT_NUMBER 1

double mathPow(double base, int exponent) {
    return (exponent == 0) ? 1 : base * mathPow(base, exponent - 1);
}
double mathFactorial(int number) {
    return (number == 0) ? 1 : number * mathFactorial(number-1);
}
double mathAbs(double value) {
    return (value < 0) ? -value : value;
}
double normalizeAngle(double angle) {
    return mathAbs(angle) - (int)(mathAbs(angle) / (2 * MATH_PI)) * (2 * MATH_PI);
}
double mathSqrt(float base, int exponent, double tolerance) {
    const bool isSqrtParamsValid = (exponent != 0 && base != 0) && ((exponent % 2 == 0 && base > 0) || exponent % 2 != 0);
    if (!isSqrtParamsValid) {
        return EXIT_NUMBER_NEGATIVE;
    }

    double result = 1, currentTolerance = 1;
    for (int i = 0; i < maxIterations && mathAbs(currentTolerance) > tolerance; i++) {
        currentTolerance = (1 / (double)exponent) * (base / mathPow(result, exponent-1) - result);
        result += currentTolerance;
    }

    if (mathAbs(currentTolerance) > tolerance) {
        printf("Sorry, but we can't achive pointed tolerance\n");
    }
    return result;
}
double mathCos(double angle) {
    angle = normalizeAngle(angle);
    double result = 1.0;
    int sign = 1;

    if (mathAbs(angle) > MATH_PI) {
        sign = -1;
        angle = (angle / mathAbs(angle)) * (angle - MATH_PI);
    }

    for (int i = 2; i <= 10; i += 2) {
        result += (i % 4 == 0) ? mathPow(angle, i) / mathFactorial(i) : -(mathPow(angle, i) / mathFactorial(i));
    }
    return result * sign;
}
bool sqrtCalculationMode() {
    float base = 0;
    int exponent = 0;
    double tolerance = 0;

    printf("\nSQRT CALCULATION MODE\n");
    printf("\nEnter value in format: 10, 5, 3\n1st - base; 2nd - exponent; 3rd - tolerance(optional)\n");
    printf(" -- Enter 0 - to stop progam, -1 - to main menu --\n");
    printf("Your input: ");
    const int scanfResult = scanf("%f, %d, %lf", &base, &exponent, &tolerance);
    fflush(stdin);
    if (!base) {
        return false;
    }
    if (base == EXIT_NUMBER_NEGATIVE) {
        return true;
    }
    if (scanfResult < 2) {
        printf("Error: invalid format\n");
        return sqrtCalculationMode();
    }

    if (tolerance == 0) {
        tolerance = defaultTolerance;
    }

    double mathSqrtResult = mathSqrt(base, exponent, tolerance);

    printf("\nMath Sqrt result: %g\n", mathSqrtResult);
    return sqrtCalculationMode();
}
bool isValidDate(int day, int month, int year) {
    const float maxDaysInMonth = 31;
    const float maxMonthsInYear = 12;

    const bool isDayInRange = (day >= MIN_INT_NUMBER && day <= maxDaysInMonth);
    const bool isMonthInRange = (month >= MIN_INT_NUMBER && month <= maxMonthsInYear);
    const bool isYearInRange = (year >= MIN_INT_NUMBER && year <= MAX_SAFE_NUMBER);

    if (!(isDayInRange && isMonthInRange && isYearInRange)) {
        printf("Invalid date format: day(%i; %g).month(%i; %g).year(%i; %g)\n",
            MIN_INT_NUMBER, maxDaysInMonth, MIN_INT_NUMBER, maxMonthsInYear, MIN_INT_NUMBER, MAX_SAFE_NUMBER);
        return false;
    }
    return true;
}
int getDayOfWeekIndex(int day, int month, int year) {
    const float daysInYear = 365.25;
    const float daysInMonth = 30.56;
    const int daysInWeek = 7;

    if (!isValidDate(day, month, year)) {
        return EXIT_NUMBER_NEGATIVE;
    }
    
    const bool isLeapYear = (year % 100 != 0 && year % 4 == 0) || (year % 100 && year % 400 == 0); // according to the formula
    const int fixer = (month < 3) ? ((isLeapYear) ?  1 : 2) : 0; // according to the formula
    const int dayOfWeekIndex = ((int)(daysInYear * year) + (int)(daysInMonth * month) + day + fixer) % daysInWeek;

    return dayOfWeekIndex;
}
bool dayOfWeekMode() {
    int day = 1, month = 1, year = 1;
    const int maxElementLength = 10;
    const char days[][maxElementLength] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    printf("\nDAY OF WEEK MODE\n");
    printf("\nEnter date in format: DAY.MONTH.YEAR\n");
    printf("-- Enter 0 - to stop progam, -1 - to main menu --\n");
    printf("Your input: ");
    const int scanfResult = scanf("%d.%d.%d", &day, &month, &year);
    fflush(stdin);
    if (day == 0) {
        return false;
    }
    if (day == EXIT_NUMBER_NEGATIVE) {
        return true;
    }
    if (scanfResult != 3) {
        printf("Invalid date format: it must be like DAY.MONTH.YEAR\n");
        return dayOfWeekMode();
    }
    int dayOfWeekIndex = getDayOfWeekIndex(day, month, year);
    if (dayOfWeekIndex == EXIT_NUMBER_NEGATIVE) {
        return dayOfWeekMode();
    }
    printf("\nOutput: %s\n", days[dayOfWeekIndex]);
    return dayOfWeekMode();
}
const int rootsAmount = 3;
void getRootsOfCubicEqutions(float a, float b, float c, double roots[rootsAmount], double *complexPart) {
    double p = 0, q = 0, d = 0, r = 0, f = 0, u = 0, v = 0;

    p = b - (mathPow(a, 2) / 3);
    q = ((2 * mathPow(a, 3)) / 27) - ((a * b) / 3) + c;
    d = (mathPow(p, 3) / 27) + (mathPow(q, 2) / 4);

    u = mathSqrt(-(q / 2) + mathSqrt(d, 2, defaultTolerance), 3, defaultTolerance);
    v = -(p / (3 * u));

    if (d > 0) {
        roots[0] = u + v;
        roots[1] = -((u + v) / 2);
        roots[2] = roots[1];
        *complexPart = (mathSqrt(3, 2, defaultTolerance) * (u - v) / 2);
    } else if (d == 0) {
        roots[0] = (3 * q) / p;
        roots[1] = -((3 * q) / (2 * p));
        roots[2] = roots[1];
    } else {
        r = mathSqrt(-mathPow(p, 3) / 27, 2, defaultTolerance);
        f = acos(-q / (2 * r));

        roots[0] = 2 * mathAbs(mathSqrt(r, 3, defaultTolerance)) * mathCos(f/3);
        roots[1] = 2 * mathAbs(mathSqrt(r, 3, defaultTolerance)) * mathCos((f + 2 * MATH_PI)/3);
        roots[2] = 2 * mathAbs(mathSqrt(r, 3, defaultTolerance)) * mathCos((f + 4 * MATH_PI)/3);
    }
}
bool solveCubeEquationMode() {
    float a = 1, b = 1, c = 1;
    double complexPart = 0;
    double roots[rootsAmount];

    printf("\nCUBE EQUATION CALCULATOR MODE\n");
    printf("\nEnter coefficient of cube equation: a, b, c\n");
    printf("Valid format sample: 4, 5, 6\n");
    printf("-- Enter 0 - to stop progam, -1 - to go main menu --\n");
    printf("Your input: ");

    const int scanfResult = scanf("%f, %f, %f", &a, &b, &c);
    fflush(stdin);
    if (!a) {
        return false;
    }
    if (a == EXIT_NUMBER_NEGATIVE) {
        return true;
    }
    const int scanfArgumentsAmount = 3;
    if (scanfResult != scanfArgumentsAmount) {
        printf("Error: invalid format\n");
        return solveCubeEquationMode();
    }

    const bool isAValid = (a < MAX_SAFE_NUMBER && a > -MAX_SAFE_NUMBER);
    const bool isBValid = (b < MAX_SAFE_NUMBER && b > -MAX_SAFE_NUMBER);
    const bool isCValid = (c < MAX_SAFE_NUMBER && c > -MAX_SAFE_NUMBER);
    const bool isValuesValid = (isAValid && isBValid && isCValid);
    if (!isValuesValid) {
        return solveCubeEquationMode();
    }

    getRootsOfCubicEqutions(a, b, c, roots, &complexPart);
    
    printf("\nRoots: ");
    const bool noComplexPart = (complexPart == 0);
    if (noComplexPart) {
        printf("y1 = %lf, y2 = %lf, y3 = %lf \n", roots[0], roots[1], roots[2]);
    } else {
        printf("y1 = %lf, y2 = %lf+%lfi, y3 = %lf-%lfi \n", roots[0], roots[1], complexPart, roots[2], complexPart);
    }
    return solveCubeEquationMode();
}
bool stopProgram() {
    return false;
}
bool runProgram() {
    bool (*modes[])() = {stopProgram, sqrtCalculationMode, dayOfWeekMode, solveCubeEquationMode};
    int modeIndex = 0;

    printf("\nSelect program mode:\n");
    printf("-- Enter number 0 to stop program --\n");
    printf(" 1) Enter number 1 to select Sqrt Calculation Mode\n");
    printf(" 2) Enter number 2 to select Day Of Week Mode\n");
    printf(" 3) Enter number 3 to select Solve Cube Equation Mode\n");

    printf("Your input: ");
    const int scanfResult = scanf("%i", &modeIndex);
    fflush(stdin);
    if (!scanfResult) {
        printf("Invalid value: enter integer number (0, 1, 2, 3)\n");
        return true;
    }
    const int modeLastIndex = 3;
    const int modeFirstIndex = 0;
    if (modeFirstIndex > modeIndex || modeIndex > modeLastIndex) {
        return true;
    }

    return modes[modeIndex]();
}

int main() {
    while(runProgram());
    return 0;
}