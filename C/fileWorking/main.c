#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define UNDEFINED -1
#define STOP_PROGRAM 0
#define EXIT_VALUE 0
#define MAX_INPUT_LENGTH 30
#define MAX_FILE_NAME_LENGTH 256
#define FILE_EXT ".region"
#define INPUT_SCANF_HOLDER "%29s"

#define SORT_BY_NAME 1
#define SORT_BY_POPULATION 2
#define SORT_BY_AREA 3

#define REGION_OPERATIONS_MODE 1
#define CITY_OPERATIONS_MODE 2

#define CREATE_CITY_MODE 1
#define EDIT_CITY_MODE 2
#define DELETE_CITY_MODE 3

#define CREATE_REGION_MODE 1
#define READ_REGION_MODE 2
#define RENAME_REGION_MODE 3
#define SORT_REGION_MODE 4
#define DELETE_REGION_MODE 5


FILE *file;

typedef struct {
    char name[MAX_INPUT_LENGTH];
    int population;
    float area;
} city_t;


void createCity(char* fileName, city_t* newCity) {
    file = fopen(fileName, "ab");
    if (file == NULL) {
        perror("Unable to create this region");
        return;
    }

    fwrite(newCity, sizeof(city_t), 1, file);
    fclose(file);
}

void deleteCity(char* fileName, char* cityNameToDelete) {
    file = fopen(fileName, "rb");
    if (file == NULL) {
        perror("Unable to read this region");
        return;
    }

    int cityNum = 0;
    city_t currentCity = {"", 0, 0.0};

    while (fread(&currentCity, sizeof(city_t), 1, file) && ++cityNum);

    city_t cities[cityNum];
    rewind(file);

    int i = 0;
    while (fread(&currentCity, sizeof(city_t), 1, file))
        if (strcmp(currentCity.name, cityNameToDelete))
            cities[i++] = currentCity;

    fclose(file);

    file = fopen(fileName, "wb");
    if (file == NULL) {
        perror("Unable to edit this region");
        return;
    }

    fwrite(cities, sizeof(city_t), i, file);

    fclose(file);
}

void editCity(char* fileName, char* cityNameToEdit, city_t newCity) {
    file = fopen(fileName, "rb");
    if (file == NULL) {
        perror("Unable to read this region");
        return;
    }

    int cityNum = 0;
    city_t currentCity = {"", 0, 0.0};

    while (fread(&currentCity, sizeof(city_t), 1, file) && ++cityNum);

    city_t cities[cityNum];
    rewind(file);

    int i = 0;
    while (fread(&currentCity, sizeof(city_t), 1, file))
        cities[i++] = strcmp(currentCity.name, cityNameToEdit) ? currentCity : newCity;

    fclose(file);

    file = fopen(fileName, "wb");
    if (file == NULL) {
        perror("Unable to edit this region");
        return;
    }

    fwrite(cities, sizeof(city_t), i, file);

    fclose(file);
}



void displayRegions(const char* folderPath) {
    DIR* directory = opendir(folderPath);

    if (directory == NULL) {
        perror("Error opening directory");
        return;
    }

    int cityNum = 0;
    struct dirent* entry;

    printf("\n=-=-= Regions =-=-=\n");

    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_type != DT_REG)
            continue;

        char filePath[MAX_FILE_NAME_LENGTH];
        snprintf(filePath, sizeof(filePath), "%s/%s", folderPath, entry->d_name);

        struct stat fileStat;
        const char* fileExt = strrchr(entry->d_name, '.');
        if (stat(filePath, &fileStat) || fileExt == NULL || strcmp(fileExt, FILE_EXT))
            continue;

        const int citiesAmount = fileStat.st_size / sizeof(city_t);
        printf(" [%d] %.*s \t [%d cities]\n", ++cityNum, MAX_INPUT_LENGTH, entry->d_name, citiesAmount);
    }

    closedir(directory);
}



void createRegion(char* regionName) {
    file = fopen(regionName, "w");
    printf(file == NULL ? "Unable to create region" : "Region was created");
    fclose(file);
}

void readRegion(char* regionName) {
    file = fopen(regionName, "rb");
    if (file == NULL) {
        perror("Unable to read this region");
        return;
    }

    printf("\n=-=-= Cities =-=-=\n");

    int cityNum = 0;
    city_t currentCity = {"", 0, 0.0};

    while (fread(&currentCity, sizeof(city_t), 1, file))
        printf(" [%d] %.*s \t [%d habitants, %g km^2]\n", 
            ++cityNum, MAX_INPUT_LENGTH, currentCity.name, currentCity.population, currentCity.area
        );

    fclose(file);
}

void renameRegion(char* oldName, char* newName) {
    printf(!rename(oldName, newName) ? "Renamed successfully" : "Unable to rename this region");
}

void sortRegion(char* regionName, int mode, int isReversed) {
    file = fopen(regionName, "rb");
    if (file == NULL) {
        perror("Unable to read this region");
        return;
    }

    int cityNum = 0;
    city_t currentCity = {"", 0, 0.0};

    while (fread(&currentCity, sizeof(city_t), 1, file) && ++cityNum);

    int i = 0;
    city_t cities[cityNum];
    rewind(file);

    while (fread(&currentCity, sizeof(city_t), 1, file))
        cities[i++] = currentCity;

    fclose(file);

    int needToSwap = 0;
    isReversed = isReversed == 1 ? 0 : 1;

    for (i = 0; i < cityNum; i++) {
        for (int j = i+1; j < cityNum; j++) {
            switch (mode) {
                case SORT_BY_NAME: 
                    needToSwap = isReversed ? 
                                    strcmp(cities[i].name, cities[j].name) > 0 : 
                                    strcmp(cities[i].name, cities[j].name) < 0;
                    break;
                case SORT_BY_POPULATION: 
                    needToSwap = isReversed ? 
                                    cities[i].population < cities[j].population :
                                    cities[i].population > cities[j].population;
                    break;
                case SORT_BY_AREA: 
                    needToSwap = isReversed ? 
                                    cities[i].area < cities[j].area :
                                    cities[i].area > cities[j].area;
                    break;
            }
            if (needToSwap) {
                city_t temp = cities[i];
                cities[i] = cities[j];
                cities[j] = temp;
            }
        }
    }

    file = fopen(regionName, "wb");
    if (file == NULL) {
        perror("Unable to edit this region");
        return;
    }

    fwrite(cities, sizeof(city_t), i, file);
    fclose(file);
}

void deleteRegion(char* regionName) {
    printf(!remove(regionName) ? "Deleted successfully" : "Unable to delete this region");
}

struct {
    void (*create)(char*);
    void (*read)(char*);
    void (*rename)(char*, char*);
    void (*sort)(char*, int, int);
    void (*delete)(char*);
} region = {createRegion, readRegion, renameRegion, sortRegion, deleteRegion};

struct {
    void (*create)(char*, city_t*);
    void (*edit)(char*, char*, city_t);
    void (*delete)(char*, char*);
} city = {createCity, editCity, deleteCity};

int fileExists(const char *filename) {
    if (access(filename, F_OK) == -1) {
        printf("\nThis region does not exist yet\n");
        return 0;
    } else {
        printf("\nThis region already exists\n");
        return 1;
    }
}

void input(void *value, char* scanfHolder, char* msg) {
    const int amountOfParams = 1;
    int scanfResult = 0;

    do {
        printf("%s", msg);
        scanfResult = scanf(scanfHolder, value);
        fflush(stdin);
    } while (scanfResult != amountOfParams);
}

void displayMenu() {
    printf("\n\n=== Main Menu ===\n\n 1. Region Operations\n 2. City Operations\n 0. Exit\n");
}

void handleRegionOperations() {
    const char* folderPath = "./";
    char regionName[MAX_INPUT_LENGTH] = "";
    
    int choice = UNDEFINED;
    int needToDeleteRegion = 0;
    char newName[MAX_INPUT_LENGTH];
    int sortOption, isReversedSorting, deleteOption;

    do {
        displayRegions(folderPath);
        printf("\n\n=== Region Operations ===\n\n");
        printf(" 1. Create Region\n 2. Read Region\n 3. Rename Region\n 4. Sort Region\n 5. Delete Region\n 0. Back to Main Menu\n");
        input(&choice, "%d", "Input: ");

        switch (choice) {
            case CREATE_REGION_MODE:
                do {
                    input(&regionName, INPUT_SCANF_HOLDER, "Enter the name of the region to create: ");
                    strcat(regionName, FILE_EXT);
                } while (fileExists(regionName));

                region.create(regionName);
                break;
            case READ_REGION_MODE:
                do {
                    input(&regionName, INPUT_SCANF_HOLDER, "Enter the name of the region to read: ");
                    strcat(regionName, FILE_EXT);
                } while (!fileExists(regionName));

                region.read(regionName);
                break;
            case RENAME_REGION_MODE:
                do {
                    input(&regionName, INPUT_SCANF_HOLDER, "Enter the current name of the region: ");
                    strcat(regionName, FILE_EXT);
                } while (!fileExists(regionName));

                do {
                    input(&newName, INPUT_SCANF_HOLDER, "Enter the new name of the region: ");
                    strcat(newName, FILE_EXT);
                } while (fileExists(newName));

                region.rename(regionName, newName);
                break;
            case SORT_REGION_MODE:
                do {
                    input(&regionName, INPUT_SCANF_HOLDER, "Enter the name of the region to sort: ");
                    strcat(regionName, FILE_EXT);
                } while (!fileExists(regionName));

                input(&sortOption, "%d", "Enter sorting options, sort by:\n 1. Name\n 2. Population\n 3. Area\n Input: ");
                input(&isReversedSorting, "%d", "Enter sorting options: \n 1. Ascending (ASC)\n 2. Descending (DESC)\n Input: ");

                region.sort(regionName, sortOption, isReversedSorting);
                region.read(regionName);
                break;
            case DELETE_REGION_MODE:
                input(&regionName, INPUT_SCANF_HOLDER, "Enter the name of the region to delete: ");
                printf("Are you sure you want to delete the region %s?", regionName);
                input(&needToDeleteRegion, "%d", "\n 1. Yes\n 2. No\nInput: ");
                if (needToDeleteRegion == 1) {
                    strcat(regionName, FILE_EXT);
                    region.delete(regionName);
                }
                break;
            case EXIT_VALUE:
                printf("Returning to the main menu.\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != EXIT_VALUE);
}

void handleCityOperations() {
    int choice = UNDEFINED;
    char cityName[MAX_INPUT_LENGTH];
    city_t newCity;

    char regionName[MAX_INPUT_LENGTH] = "";
    char fileName[MAX_INPUT_LENGTH] = "";


    do {
        input(&regionName, INPUT_SCANF_HOLDER, "Enter the name of the region: ");
        strcpy(fileName, regionName);
        strcat(fileName, FILE_EXT);
    } while (!fileExists(fileName));

    do {
        region.read(fileName);
        printf("\n\n=== City Operations ===\n");
        printf("-> Region: %s\n", regionName);
        input(&choice, "%d", " 1. Create City\n 2. Edit City\n 3. Delete City\n 0. Back to Main Menu\n Input: ");

        switch (choice) {
            case CREATE_CITY_MODE:
                input(&newCity.name, INPUT_SCANF_HOLDER, "Enter the Name of the city to create: ");
                input(&newCity.population, "%d", "Enter the Population of the city to create: ");
                input(&newCity.area, "%f", "Enter the Area of the city to create: ");

                city.create(fileName, &newCity);
                break;
            case EDIT_CITY_MODE:
                input(&cityName, INPUT_SCANF_HOLDER, "Enter the current name of the city: ");
                input(&newCity.name, INPUT_SCANF_HOLDER, "Enter the new Name of the city: ");
                input(&newCity.population, "%d", "Enter the new Population of the city: ");
                input(&newCity.area, "%f", "Enter the new Area of the city: ");

                city.edit(fileName, cityName, newCity);
                break;
            case DELETE_CITY_MODE:
                input(&cityName, INPUT_SCANF_HOLDER, "Enter the name of the city to delete: ");

                city.delete(fileName, cityName);
                break;
            case EXIT_VALUE:
                printf("Returning to the main menu.\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != EXIT_VALUE);
}

int main() {
    const char* folderPath = "./";
    int choice = UNDEFINED;
    printf("\nWith this program, you can create regions and cities.\n");

    do {
        displayRegions(folderPath);
        displayMenu();
        input(&choice, "%d", "Input: ");

        switch (choice) {
            case REGION_OPERATIONS_MODE:
                handleRegionOperations();
                break;
            case CITY_OPERATIONS_MODE:
                handleCityOperations();
                break;
            case STOP_PROGRAM:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid input. Please enter a valid option.\n");
        }
    } while (choice != STOP_PROGRAM);

    return EXIT_SUCCESS;
}