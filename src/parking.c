#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include "../include/types.h"
#include "../include/parking.h"
#include "../include/billing.h"
#include "../include/fileio.h"
#include "../include/utils.h"

void initParkingLot(ParkingLot *p) {
    p->count = 0;
    p->prices[0] = (PriceConfig){MOTO, "Motorbike", 2000, 2000};
    p->prices[1] = (PriceConfig){CAR,  "Car",       5000, 5000};
    p->prices[2] = (PriceConfig){BUS,  "Bus",      10000, 10000};
}

int findVehicleIndex(ParkingLot *p, const char *plate) {
    int i;
    for (i = 0; i < p->count; i++) {
        if (p->list[i].status == PARKING &&
            strcmp(p->list[i].licensePlate, plate) == 0) {
            return i;
        }
    }
    return -1;
}

void addVehicle(ParkingLot *p) {
    if (p->count >= MAX_VEHICLES) {
        printf(RED "Parking lot is full!\n" RESET);
        return;
    }

    char plate[20];
    int type;

    printf(LINE "\n===================== " TITLE "ADD VEHICLE" RESET LINE " =====================" RESET "\n");

    getString("Enter license plate: ", plate, sizeof(plate));
    toUpperCase(plate);

    if (!isValidLicensePlate(plate)) {
        printf("Invalid license plate! Format: SSCC-SSSSS (S: number, C: letter)\n");
        return;
    }

    if (isDuplicateLicensePlate(p->list, p->count, plate)) {
        printf(RED "Vehicle already exists!\n" RESET);
        return;
    }

    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);
    int hour = localTime->tm_hour;

    if (hour >= 23 || hour < 3) {
        printf("Parking lot is closed from 23:00 to 03:00!\n");
        return;
    }

    type = getInt(
        "Vehicle type (0: motorbike, 1: car, 2: bus): ",
        "Vehicle type must be 0, 1, or 2.",
        "Please enter a valid number.",
        0, 2
    );

    Vehicle *v = &p->list[p->count];
    strncpy(v->licensePlate, plate, sizeof(v->licensePlate) - 1);
    v->licensePlate[sizeof(v->licensePlate) - 1] = '\0';
    v->type      = type;
    v->entryTime = time(NULL);
    v->exitTime  = 0;
    v->fee       = 0;
    v->status    = PARKING;
    p->count++;

    if (saveData(p) == 1) {
        printf(GREEN "Vehicle added and data saved successfully!\n" RESET);
        printf("Entry time: %s", ctime(&v->entryTime));
    } else {
        printf(RED "Warning: Vehicle added to memory but failed to save to disk!\n" RESET);
    }
}

void removeVehicle(ParkingLot *p) {
    char plate[20];

    printf("\n--- CHECK OUT VEHICLE ---\n");
    printf("\nCurrent vehicles in yard:\n");
    printf("\033[1;36m%-5s | %-15s | %-15s\033[0m\n", "No", "LICENSE PLATE", "VEHICLE TYPE");
    printf("--------------------------------------------------\n");

    int count = 0;
    for (int i = 0; i < p->count; i++) {
        if (p->list[i].status == PARKING) {
            count++;
            const char *typeStr;
            if      (p->list[i].type == MOTO) typeStr = "Motorbike";
            else if (p->list[i].type == CAR)  typeStr = "Car";
            else if (p->list[i].type == BUS)  typeStr = "Bus";
            else                              typeStr = "Other";

            printf("%-5d | %-15s | %-15s\n", count, p->list[i].licensePlate, typeStr);
        }
    }

    printf("--------------------------------------------------\n");    
    if (p->count == 0) {
         printf("No vehicles in parking lot.\n");
         return;
    }

    getString("Enter license plate: ", plate, sizeof(plate));
    toUpperCase(plate);

    if (!isValidLicensePlate(plate)) {
        printf(RED "Invalid license plate!\n" RESET);
        return;
    }

    int idx = findVehicleIndex(p, plate);
    if (idx == -1) {
        printf(RED "Vehicle not found or already exited!\n" RESET);
        return;
    }

    Vehicle *v = &p->list[idx];
    v->exitTime = time(NULL);

    if (v->exitTime <= v->entryTime) {
        printf(RED "Time error!\n" RESET);
        return;
    }

    v->fee    = calculateFee(*v, p);
    v->status = EXITED;

    printBill(*v, p);
    
    if (saveData(p) == 1) {
        printf(GREEN "Data saved successfully!\n" RESET);
    } else {
        printf(RED "Warning: Checkout successful but failed to save to disk!\n" RESET);
    }
}

void listVehicles(ParkingLot *p) {
    int choice;
    while (1) {
        printf(LINE "\n============================== " TITLE "DISPLAY OPTIONS" RESET LINE " ==============================" RESET "\n");
        printf("1. Vehicles currently in yard\n");
        printf("2. Vehicles already exited\n");
        printf("3. All transaction history\n");
        printf("4. Deleted vehicles history (Recycle Bin)\n");
        printf(LINE "------------------------------------------------------------------------------\n");
        printf(YELLOW "Enter your choice: " RESET);
        
        if (scanf("%d", &choice) != 1) 
        {
            printf(RED "Error: Invalid input! Please enter a number (1, 2, 3, or 4).\n" RESET);
            while(getchar() != '\n'); 
            continue;                
        } 
        while(getchar() != '\n'); 

        if (choice < 1 || choice > 4) 
        {
            printf(RED "Error: Choice must be 1, 2, 3, or 4. Please try again.\n" RESET);
            continue; 
        } 
        break;
    }

    int count_in_yard = 0;
    time_t currentTime = time(NULL); 

    printf(LINE "\n================================ " TITLE "VEHICLE LIST" RESET LINE " ================================" RESET "\n\n");
    if (choice == 3 || choice == 4) 
    {
        printf("\033[1;36m%-5s | %-15s | %-15s | %-10s | %-25s\033[0m\n", 
               "STT", "LICENSE PLATE", "VEHICLE TYPE", "STATUS", "ENTRY TIME");
    } 
    else 
    {
        printf("\033[1;36m%-5s | %-15s | %-15s | %-25s\033[0m\n", 
               "STT", "LICENSE PLATE", "VEHICLE TYPE", "ENTRY TIME");
    }
    printf(LINE "------------------------------------------------------------------------------\n");
    if (choice == 4) {
        FILE *f = fopen("data/deleted_vehicles.dat", "r");
        if (f == NULL) {
            printf(RED "No deleted vehicles history file found!\n" RESET);
        } else {
            char line[150];
            while (fgets(line, sizeof(line), f)) {
                char plate[15];
                int type, status;
                long entryTime;
                float fee;
                long outTime;

                if (sscanf(line, "%[^|]|%d|%ld|%ld|%f|%d", plate, &type, &entryTime, &outTime, &fee, &status) == 6) {
                    count_in_yard++;

                    const char *typeStr;
                    if      (type == MOTO) typeStr = "Motorbike";
                    else if (type == CAR)  typeStr = "Car";
                    else if (type == BUS)  typeStr = "Bus";
                    else                   typeStr = "Other";

                    char timeStr[26];
                    time_t tempTime = (time_t)entryTime;
                    char *rawTime = ctime(&tempTime);
                    if (rawTime != NULL) {
                        strncpy(timeStr, rawTime, 24);
                        timeStr[24] = '\0';
                    } else { 
                        strcpy(timeStr, "N/A"); 
                    }

                    printf(RESET "%-5d | %-15s | %-15s | %-10s | %-25s\n", 
                           count_in_yard, plate, typeStr, "Deleted", timeStr);
                }
            }
            fclose(f);
        }
    } 
    else 
    {
        int i;
        for (i = 0; i < p->count; i++) 
        {
            bool shouldPrint = false;
            if (choice == 1 && p->list[i].status == PARKING) shouldPrint = true;
            else if (choice == 2 && p->list[i].status == EXITED) shouldPrint = true;
            else if (choice == 3) shouldPrint = true;

            if (shouldPrint) 
            {
                count_in_yard++;

                bool isOver72h = false;
                if (p->list[i].status == PARKING) 
                {
                    double seconds = difftime(currentTime, p->list[i].entryTime);
                    if (seconds >= 72 * 3600) isOver72h = true;
                }

                const char *typeStr;
                if      (p->list[i].type == MOTO) typeStr = "Motorbike";
                else if (p->list[i].type == CAR)  typeStr = "Car";
                else if (p->list[i].type == BUS)  typeStr = "Bus";
                else                              typeStr = "Other";

                char timeStr[26];
                char *rawTime = ctime(&p->list[i].entryTime);
                if (rawTime != NULL) 
                {
                    strncpy(timeStr, rawTime, 24);
                    timeStr[24] = '\0';
                } 
                else 
                { 
                    strcpy(timeStr, "N/A"); 
                }

                const char *rowColor = isOver72h ? RED : RESET;

                if (choice == 3) 
                {
                    const char *statusStr = (p->list[i].status == PARKING) ? "In Yard" : "Exited";
                    printf("%s%-5d | %-15s | %-15s | %-10s | %-25s%s\n", 
                    rowColor, count_in_yard, p->list[i].licensePlate, typeStr, statusStr, timeStr, (isOver72h ? " (Over 72h!)" : ""));
                } 
                else 
                {
                    printf("%s%-5d | %-15s | %-15s | %-25s%s\n", 
                    rowColor, count_in_yard, p->list[i].licensePlate, typeStr, timeStr, (isOver72h ? " (Over 72h!)" : ""));
                }
                printf(RESET); 
            }
        }
    }

    printf(LINE "----------------------------------------------------------------------\n");
    if (choice == 1)
    {
        float ratio = (float)(count_in_yard * 100) / MAX_VEHICLES;
        if (count_in_yard == 0) printf(RED "Empty!\n" RESET);
        printf(LINE "------------------------------------------------------------------------------\n");
        if (ratio < 80) 
        {
            printf(YELLOW "Total: %d/3636 " RESET "\n", count_in_yard);
            printf(GREEN "Status: %0.2f%% Available\n" RESET, ratio);
        }
        else if (ratio < 100) 
        {
            printf(YELLOW "Total: %d/3636 " RESET "\n", count_in_yard);
            printf(YELLOW "Status: %0.2f%% Nearly full\n" RESET, ratio);
        }
        else 
        {
            printf(YELLOW "Total: %d/3636 " RESET "\n", count_in_yard);
            printf(RED "Status: %0.2f%% Full\n" RESET, ratio);
        }
    }
}

void searchVehicle(ParkingLot *p) {
    char key[15];
    int found_count = 0;

    printf(LINE "============================== " TITLE "SEARCH VEHICLE" RESET LINE " ==============================" RESET "\n\n");
    getString("Enter license plate keyword: ", key, sizeof(key));

    char upperKey[15];
    int k = 0;
    while (key[k] != '\0') 
    {
        upperKey[k] = toupper((unsigned char)key[k]);
        k++;
    }
    upperKey[k] = '\0'; 
    printf("\n\n");
    printf(LINE "============================= " TITLE "SUGGESTED RESULTS" RESET LINE " =============================" RESET "\n");
    printf("\n\033[1;36m%-5s | %-15s | %-15s | %-10s | %-25s\033[0m\n",  "STT", "LICENSE PLATE", "VEHICLE TYPE", "STATUS", "ENTRY TIME");    
    printf(LINE "------------------------------------------------------------------------------------\n" RESET);

    int i;
    for (i = 0; i < p->count; i++) 
    {
        if (strstr(p->list[i].licensePlate, upperKey) != NULL) 
        {
            found_count++;

            const char *typeStr;
            if      (p->list[i].type == MOTO) typeStr = "Motorbike";
            else if (p->list[i].type == CAR)  typeStr = "Car";
            else if (p->list[i].type == BUS)  typeStr = "Bus";
            else                              typeStr = "Other";

            char timeStr[26];
            char *rawTime = ctime(&p->list[i].entryTime);
            if (rawTime != NULL) 
            {
                strncpy(timeStr, rawTime, 24);
                timeStr[24] = '\0';
            } 
            else 
            {
                strcpy(timeStr, "N/A");
            }

            printf(RESET "%-5d | %-15s | %-15s | %-10s | %-25s\n",
                   found_count,
                   p->list[i].licensePlate, 
                   typeStr,
                   (p->list[i].status == PARKING ? "In Yard" : "Exited"),
                   timeStr);
        }
    }

    if (found_count == 0) {
        printf(RED "No vehicle found matching '%s'.\n" RESET, key);
    }
}

void deleteVehicle(ParkingLot *p) {
    printf("\n" LINE "=================== " TITLE "DELETE VEHICLE (ADMIN ONLY)" RESET LINE " ===================" RESET "\n");

    if (p->count == 0) {
        printf(YELLOW "  System is currently empty. No vehicles to delete.\n" RESET);
        return;
    }

    printf("\n" TITLE "--- AVAILABLE VEHICLES IN SYSTEM ---" RESET "\n");
    printf("\033[1;36m %-5s | %-15s | %-15s | %-18s\033[0m\n", "STT", "LICENSE PLATE", "VEHICLE TYPE", "STATUS");
    printf(LINE "-------------------------------------------------------------------\n" RESET);

    for (int i = 0; i < p->count; i++) {
        const char *typeStr;
        if (p->list[i].type == MOTO) typeStr = "Motorbike";
        else if (p->list[i].type == CAR) typeStr = "Car";
        else if (p->list[i].type == BUS) typeStr = "Bus";
        else typeStr = "Other";

        printf(RESET " %-5d | %-15s | %-15s | %-18s\n",
               i + 1,
               p->list[i].licensePlate,
               typeStr,
               p->list[i].status == PARKING ? "Currently parked" : "Already exited");
    }
    printf(LINE "-------------------------------------------------------------------\n" RESET);

    char plate[20];
    int targetIdx = -1;

    while (1) {
        getString("\nEnter license plate to permanently delete (or '0' to cancel): ", plate, sizeof(plate));
        
        if (strcmp(plate, "0") == 0) {
            printf(YELLOW " Deletion cancelled. Returning to main menu...\n" RESET);
            return;
        }

        targetIdx = -1;
        for (int i = 0; i < p->count; i++) {
            int j = 0;
            int isMatch = 1;
            while (p->list[i].licensePlate[j] != '\0' || plate[j] != '\0') {
                if (toupper((unsigned char)p->list[i].licensePlate[j]) != toupper((unsigned char)plate[j])) {
                    isMatch = 0;
                    break;
                }
                j++;
            }
            
            if (isMatch) {
                targetIdx = i;
                break;
            }
        }

        if (targetIdx != -1) {
            break; 
        }
        
        printf(RED " Oops! Vehicle '%s' was not found. Please double-check the list and try again.\n" RESET, plate);
    }

    logDeletedVehicle(&p->list[targetIdx]);
    
    char deletedPlate[20];
    strcpy(deletedPlate, p->list[targetIdx].licensePlate);

    for (int i = targetIdx; i < p->count - 1; i++) {
        p->list[i] = p->list[i + 1];
    }
    p->count--;

    if (saveData(p) == 1) {
        printf(GREEN " Success! Vehicle '%s' has been completely removed from the system.\n" RESET, deletedPlate);
    } else {
        printf(RED " Warning: Vehicle '%s' removed from memory but failed to save to disk!\n" RESET, deletedPlate);
    }
}
