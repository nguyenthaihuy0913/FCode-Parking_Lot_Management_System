#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/types.h"
#include "../include/parking.h"
#include "../include/report.h"
#include "../include/utils.h"

static const char* getVehicleTypeName(VehicleType type) {
    switch (type) {
        case MOTO: return "Motorbike";
        case CAR:  return "Car";
        case BUS:  return "Bus";
        default:   return "Unknown";
    }
}

static int isSameDay(time_t t1, time_t t2) {
    struct tm *tm1 = localtime(&t1);
    int d1 = tm1->tm_mday, m1 = tm1->tm_mon, y1 = tm1->tm_year;

    struct tm *tm2 = localtime(&t2);
    int d2 = tm2->tm_mday, m2 = tm2->tm_mon, y2 = tm2->tm_year;

    return (d1 == d2 && m1 == m2 && y1 == y2);
}

void viewDailyRevenue(ParkingLot *p) {
    time_t now = time(NULL);
    struct tm *tm_ptr = localtime(&now);
    struct tm today = *tm_ptr; 
    
    double totalRevenue = 0.0;
    int totalVehiclesOut = 0;
    int i;

    printf(LINE "\n===========================================================================\n" RESET);
    printf(TITLE "[                             DAILY REVENUE REPORT                        ]" "\n" RESET);
    printf(LINE "===========================================================================\n"  RESET);
    printf(YELLOW "                              Date: %02d/%02d/%04d\n" RESET,
           today.tm_mday, today.tm_mon + 1, today.tm_year + 1900);
    printf(LINE "---------------------------------------------------------------------------\n" RESET);

    for (i = 0; i < p->count; i++) {
        if (p->list[i].status != 1) { 
            continue;
        }

        if (!isSameDay(p->list[i].exitTime, now)) {
            continue;
        }

        totalRevenue += p->list[i].fee;
        totalVehiclesOut++;
    }

    if (totalVehiclesOut == 0) {
        printf("  No revenue data for today.\n" );
        printf("  No vehicles have checked out yet.\n" );
    } else {
        printf("  Vehicles checked out : " YELLOW "%d\n" RESET, totalVehiclesOut);
        printf("  Total revenue        : " YELLOW "%.0f VND\n" RESET, totalRevenue);
    }

    printf(LINE "===========================================================================\n"  RESET);
}

void exportRevenueReport(ParkingLot *p) {
    time_t now = time(NULL);
    struct tm *tm_ptr = localtime(&now);
    struct tm today = *tm_ptr;
    
    double totalRevenue = 0.0;
    int totalVehiclesOut = 0;
    char filename[100]; 
    int i;

    sprintf(filename, "data/report_%02d%02d%04d_%02d%02d%02d.txt",
            today.tm_mday, today.tm_mon + 1, today.tm_year + 1900,
            today.tm_hour, today.tm_min, today.tm_sec);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("ERROR: Cannot create report file!\n");
        return;
    }

    fprintf(fp, "===================================================\n");
    fprintf(fp, "       DAILY REVENUE REPORT - F-CODE CLUB\n");
    fprintf(fp, "       FPT University Ho Chi Minh City\n");
    fprintf(fp, "===================================================\n");
    fprintf(fp, "  Date       : %02d/%02d/%04d\n",
            today.tm_mday, today.tm_mon + 1, today.tm_year + 1900);
    fprintf(fp, "  Generated  : %02d:%02d:%02d\n",
            today.tm_hour, today.tm_min, today.tm_sec);
    fprintf(fp, "===================================================\n\n");

    fprintf(fp, "%-5s %-15s %-14s %12s\n", "No.", "License Plate", "Vehicle Type", "Parking Fee");
    fprintf(fp, "---------------------------------------------------\n");

    int rowNum = 0;
    for (i = 0; i < p->count; i++) {
        if (p->list[i].status != 1) { 
            continue;
        }
        if (!isSameDay(p->list[i].exitTime, now)) {
            continue;
        }

        rowNum++;
        totalRevenue += p->list[i].fee;
        totalVehiclesOut++;

        fprintf(fp, "%-5d %-15s %-14s %12.0f\n",
                rowNum,
                p->list[i].licensePlate,
                getVehicleTypeName(p->list[i].type),
                p->list[i].fee);
    }

    if (totalVehiclesOut == 0) {
        fprintf(fp, "  No vehicles checked out today.\n");
    }

    fprintf(fp, "---------------------------------------------------\n");
    fprintf(fp, "  Total vehicles checked out : %d\n", totalVehiclesOut);
    fprintf(fp, "  Total revenue              : %.0f VND\n", totalRevenue);
    fprintf(fp, "===================================================\n");
    fprintf(fp, "       End of Report - Parking Lot System\n");
    fprintf(fp, "===================================================\n");

    fclose(fp);
    printf("\n");
    printf(LINE "============================================================================\n"  RESET);
    printf(GREEN "  Report exported successfully!\n" RESET);
    printf(YELLOW "  File: %s\n" RESET, filename);
    printf(YELLOW "  Vehicles: %d | Revenue: %.0f VND\n" RESET, totalVehiclesOut, totalRevenue);
    printf(LINE "============================================================================\n"  RESET);
}