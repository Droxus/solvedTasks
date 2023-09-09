#include <stdio.h>
#include <math.h>

struct RomanNumeralMapping {
    const char key;
    int value;
};
struct RomanNumeralMapping romanNumerals[] = {
    {'M', 1000},
    {'D', 500},
    {'C', 100},
    {'L', 50},
    {'X', 10},
    {'V', 5},
    {'I', 1},
};

void getRoman(int value) {
    int romanNumeralsLength = 0;
    if (sizeof(romanNumerals) > 0) {
        romanNumeralsLength = sizeof(romanNumerals) / sizeof(romanNumerals[0]);
    }
    for (int romanNumeralIndex = 0; romanNumeralIndex < romanNumeralsLength; romanNumeralIndex++) {
        char roman = romanNumerals[romanNumeralIndex].key;
        int arabic = romanNumerals[romanNumeralIndex].value;
        while (value >= arabic) {
            putchar(roman);
            value -= arabic;
        }
        const int maxRomansInARow = 3;
        for (int i = romanNumeralIndex; i < romanNumeralsLength; i++) {
            int num = romanNumerals[i].value;
            if (value >= arabic - num && value > num * maxRomansInARow) {
                putchar(romanNumerals[i].key);
                putchar(romanNumerals[romanNumeralIndex].key);
                value -= arabic - num;
                break;
            }
        }
    }
}
void clearInput() {
    int input;
    while ((input = getchar()) != '\n' && input != EOF);
}
int errorHandler(float value) {
    if (scanf("%f", &value) != 1) {
        printf("Error: Invalid value, please enter a valid number.\n");
        clearInput();
        return 0;
    }
    if (value != floor(value) && value != ceil(value)) {
        printf("Error: Number must be integer only\n");
        return 0;
    } else if (value == 0) {
        printf("Program stopped\n");
        return -1;
    } else if (value >= 4000) {
        printf("Error: Number should be less than 4000\n");
        return 0;
    } else if (value < 1) {
        printf("Error: Number must be positive only\n");
        return 0;
    }
    return value;
}
int arabicToRoman() {
    float input;
    printf("\nENTER INTEGER NUMBER FROM 1 TO 3999. Input 0 to stop program\n");

    printf("Input: ");
    input = errorHandler(input);
    if (input == 0) {
        return 1; 
    }
    if (input == -1) {
        return 0; 
    }

    printf("Result: ");
    getRoman(input);
    printf("\n");
    return 1;
}
int main() {
    while (1) {
        if (arabicToRoman() == 0) {
            break;
        }
    }
    return 0;
}
