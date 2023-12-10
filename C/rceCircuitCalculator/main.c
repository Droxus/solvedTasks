#include <stdio.h>
#include <math.h>

#define MIN_FLOAT -1e+38
#define MAX_FLOAT 1e+38
#define STOP_PROGRAM 0
#define MAX_ITERATIONS 1e+4

struct rlcCircuit {float R, R1, R2, L, C;};
struct streamFreq {float min, max, step, current;};
struct complex {float real, imag;};

typedef struct complex cmplx;
typedef struct rlcCircuit rlcCircuit;
typedef struct streamFreq streamFreq;

cmplx addCmplx(cmplx a, cmplx b) {
    cmplx result;
    result.real = a.real + b.real;
    result.imag = a.imag + b.imag;

    return result;
}

cmplx divideCmplx(cmplx a, cmplx b) {
    cmplx result;
    double denominator = b.real * b.real + b.imag * b.imag;
    result.real = (a.real * b.real + a.imag * b.imag) / denominator;
    result.imag = (a.imag * b.real - a.real * b.imag) / denominator;
    
    return result;
}

cmplx rlcCircuit1(rlcCircuit param, float omega) {
    cmplx numerator = {param.L / param.C, -param.R / (omega * param.C)};
    cmplx denominator = {param.R, omega * param.L - 1 / (omega * param.C)};

    return divideCmplx(numerator, denominator);
}

cmplx rlcCircuit2(rlcCircuit param, float omega) {
    cmplx numerator = {param.L / param.C, param.R / (omega * param.C)};
    cmplx denominator = {param.R, omega * param.L - 1 / (omega * param.C)};

    return divideCmplx(numerator, denominator);
}

cmplx rlcCircuit3(rlcCircuit param, float omega) {
    cmplx numerator = {param.R1 * param.R2, param.R1 * (omega * param.L - 1 / (omega * param.C))};
    cmplx denominator = {param.R1 + param.R2, omega * param.L - 1 / (omega * param.C)};

    return divideCmplx(numerator, denominator);
}

cmplx rlcCircuit4(rlcCircuit param, float omega) {
    cmplx numerator = {param.R1 * param.R2 + param.L / param.C, omega * param.L * param.R1 - param.R2 / (omega * param.C)};
    cmplx denominator = {param.R1 + param.R2, omega * param.L - 1 / (omega * param.C)};

    return divideCmplx(numerator, denominator);
}

void displayTable(cmplx (*circuit)(rlcCircuit, float), rlcCircuit params, streamFreq freq) {
    printf("Resonant Freq\t\tFreq\t\tResistance\n");
    for (freq.current = freq.min; freq.current < freq.max; freq.current += freq.step) {
        float omega = 2.0 * M_PI * freq.current;
        cmplx resistance = circuit(params, omega);
        float resonantFrequency = 1.0 / (2.0 * M_PI * sqrt(params.L * params.C));

        printf("%f\t\t%f\t%.1f%+.1fi\n", resonantFrequency, freq.current, resistance.real, resistance.imag);
    }
}

int isInRange(float value, float min, float max) {
    if (value >= min && value <= max)
        return 1;
    return 0;
}

void getValue(char* msg, char* scanfHolder, float *value) {
    int scanfResult = 0;

    do {
        printf(msg, MIN_FLOAT, MAX_FLOAT);
        scanfResult = scanf(scanfHolder, value);
        fflush(stdin);
    } while (scanfResult != 1 && !isInRange(*value, MIN_FLOAT, MAX_FLOAT) && *value != STOP_PROGRAM);
}

int inputSettings(int *mode, rlcCircuit *input, streamFreq *freq) {
    int scanfResult = 0;

    printf("\nModes:\n [1] {L / C, -R / (omega * C)} / {R, omega * L - 1 / (omega * C)}\n");
    printf(" [2] {L / C, R / (omega * C)} / {R, omega * L - 1 / (omega * C)}\n");
    printf(" [3] {R1 * R2, R1 * (omega * L - 1 / (omega * C))} / {R1 + R2, omega * L - 1 / (omega * C)}\n");
    printf(" [4] {R1 * R2 + L / C, omega * L * R1 - R2 / (omega * C)} / {R1 + R2, omega * L - 1 / (omega * C)}\n");

    do {
        printf("Input mode [mode] (%d, %d): ", 0, 4);
        scanfResult = scanf("%d", mode);
        fflush(stdin);
    } while (scanfResult != 1 && *mode >= 0 && *mode <= 4 && *mode != 0);
    if (!*mode) return STOP_PROGRAM;

    if (*mode == 1 || *mode == 2) {
        getValue("Input resistance [R] (%.0e, %.0e): ", "%f", &input->R);
        if (!input->R) return STOP_PROGRAM;
    } else {
        getValue("Input first resistance [R1] (%.0e, %.0e): ", "%f", &input->R1);
        if (!input->R1) return STOP_PROGRAM;

        getValue("Input second resistance [R2] (%.0e, %.0e): ", "%f", &input->R2);
        if (!input->R2) return STOP_PROGRAM;
    }

    getValue("Input inductor [L] (%.0e, %.0e): ", "%f", &input->L);
    if (!input->L) return STOP_PROGRAM;

    getValue("Input capacitor [C] (%.0e, %.0e): ", "%f", &input->C);
    if (!input->C) return STOP_PROGRAM;

    getValue("Input min stream frequency [min] (%.0e, %.0e): ", "%f", &freq->min);
    if (!freq->min) return STOP_PROGRAM;

    getValue("Input max stream frequency [max] (%.0e, %.0e): ", "%f", &freq->max);
    if (!freq->max) return STOP_PROGRAM;

    printf("*Step should be more than %f\n", fabs(freq->max - freq->min) / MAX_ITERATIONS);
    do getValue("Input step of stream frequency [step] (%.0e, %.0e): ", "%f", &freq->step);
    while (fabs(freq->max - freq->min) / MAX_ITERATIONS > freq->step);
    if (!freq->step) return STOP_PROGRAM;

    return 1;
}

int runProgram() {
    rlcCircuit input = {0.0, 0.0, 0.0, 0.0, 0.0};
    streamFreq freq = {0.0, 0.0, 0.0, 0.0};

    int mode = 0;

    cmplx (*circuits[4])(rlcCircuit, float) = {rlcCircuit1, rlcCircuit2, rlcCircuit3, rlcCircuit4};

    if (!inputSettings(&mode, &input, &freq))
        return STOP_PROGRAM;

    displayTable(circuits[mode-1], input, freq);

    return 1;
}

int main() {
    while(runProgram());

    return STOP_PROGRAM;
}