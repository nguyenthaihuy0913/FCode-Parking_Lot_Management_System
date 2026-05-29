#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/parking.h"
#include "../include/fileio.h"
#include "../include/utils.h"

#define DATA_FILE "data/vehicles.dat"

void loadData(ParkingLot *p) {
    FILE *fp = fopen(DATA_FILE, "r");

    if (fp == NULL) {
        p->count = 0;
        printf(LINE "========================================\n" RESET);
        printf(RED "  No saved data found in vehicles.dat. Starting fresh.\n" RESET);
        printf(LINE "========================================\n" RESET);
        return;
    }

    p->count = 0;

    while (p->count < MAX_VEHICLES) {
        int type, status;
        long entry, exit_t; 
        double fee;
        char plate[20];

        int result = fscanf(fp, "%19[^|]|%d|%ld|%ld|%lf|%d\n",
                            plate, &type, &entry, &exit_t, &fee, &status);

        if (result == EOF || result != 6) {
            break;
        }

        strncpy(p->list[p->count].licensePlate, plate, sizeof(p->list[p->count].licensePlate) - 1);
        p->list[p->count].licensePlate[sizeof(p->list[p->count].licensePlate) - 1] = '\0';
        
        p->list[p->count].type = (VehicleType)type;
        p->list[p->count].entryTime = (time_t)entry;
        p->list[p->count].exitTime = (time_t)exit_t;
        p->list[p->count].fee = fee;
        p->list[p->count].status = status;

        p->count++;
    }

    fclose(fp);

    printf(GREEN "  System Data loaded: %d vehicle(s) found.\n" RESET, p->count);
}

int saveData(ParkingLot *p) {
    FILE *fp = fopen(DATA_FILE, "w");

    if (fp == NULL) {
        printf(RED "ERROR: Cannot open %s for saving!\n" RESET, DATA_FILE);
        return 0; 
    }

    int i;
    for (i = 0; i < p->count; i++) {
        fprintf(fp, "%s|%d|%ld|%ld|%.2f|%d\n",
                p->list[i].licensePlate,
                p->list[i].type,
                (long)p->list[i].entryTime,
                (long)p->list[i].exitTime,
                p->list[i].fee,
                p->list[i].status);
    }

    fclose(fp);
    
    return 1; 
}

void logDeletedVehicle(const Vehicle *v) {
    FILE *fp = fopen("data/deleted_vehicles.dat", "a");
    if (fp == NULL) {
        printf(RED "Cannot open deleted_vehicles.dat for logging!\n" RESET);
        return;
    }

    fprintf(fp, "%s|%d|%ld|%ld|%.2f|%d\n",
            v->licensePlate,
            v->type,
            (long)v->entryTime,
            (long)v->exitTime,
            v->fee,
            v->status);

    fclose(fp);
}