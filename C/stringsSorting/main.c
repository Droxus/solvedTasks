#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define maxInputSize 100

typedef char* string;

void swapStringElements(string *a, string *b) {
    string temp = *a;
    *a = *b;
    *b = temp;
}

void sortStringArray(string *array, int length) {
    for (int i = 0; i < length-1; i++)
        for (int j = i+1; j < length; j++)
            if (strcmp(array[i], array[j]) > 0)
                swapStringElements(&array[i], &array[j]);
}

void outputSortedStringArray(string *array, int length) {
    printf("\n---after sorting---\n\n");

    for (int i = 0; i < length; i++)
        printf("%i -> %s\n", i+1, array[i]);
}

void inputStringsArray(int stringsNumber, int stringLength, char array[stringsNumber][stringLength+1]) {
    char input[maxInputSize + 1];

    printf("\n---before sorting---\n\n");

    for (int i = 0; i < stringsNumber; i++) {
        printf("%i ->", i+1);

        int scanfResult = scanf("%s", input);
        int inputLength = strlen(input);

        if (inputLength == 1 && input[0] == '0')
            exit(0);

        if (inputLength > stringLength) {
            printf("Input is too long. Please enter no more than %i characters.\n", stringLength);
            i--;
        } else {
            strncpy(array[i], input, stringLength);
            array[i][stringLength] = '\0';
        }

        fflush(stdin);
    }
}

string* replicateArray(int size, int length, char original[size][length+1]) {
    string* replicated = (string*)malloc(size * sizeof(string));

    for (int i = 0; i < size; i++) {
        replicated[i] = (string)malloc(length * sizeof(char));
        strcpy(replicated[i], original[i]);
    }

    return replicated;
}

int inputSettings(string msg) {
    int scanfResult = 0, result = 0;

    do {
        printf(msg, maxInputSize);
        scanfResult = scanf("%d", &result);
        fflush(stdin);
        if (scanfResult != 1 || result > maxInputSize || result < 0)
            printf("Error: Wrong value\n");
    } while (scanfResult != 1 || result > maxInputSize || result < 0);

    return result;
}

int runProgram() {
    printf("STRINGS SORTING\n");
    printf("Enter 0 to stop program\n");
    
    int stringsNumber = inputSettings("Enter the number of strings [max %i]: ");
    if (!stringsNumber)
        return 0;

    int stringLength = inputSettings("Enter the length of each string [max %i]: ");
    if (!stringLength)
        return 0;

    char list[stringsNumber][stringLength+1];
    inputStringsArray(stringsNumber, stringLength, list);

    string* sortedArray = replicateArray(stringsNumber, stringLength, list);
    sortStringArray(sortedArray, stringsNumber);

    outputSortedStringArray(sortedArray, stringsNumber);

    return 1;
}

int main() {
    while(runProgram());

    return 0;
}