#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "../include/billing.h"
#include "../include/types.h"
#include "../include/parking.h"
#include "../include/utils.h"

double calculateFee(Vehicle v, ParkingLot *p) {
    double seconds = difftime(v.exitTime, v.entryTime);
    double hours   = seconds / 3600.0;

    if (hours < 1) hours = 1;

    int parkingHours = (int)ceil(hours);
    double fee = hours * p->prices[v.type].price;

    if (hours > 72)
        printf("\n[WARNING] Vehicle parked over 72 hours!\n");

    return fee;
}

void printBill(Vehicle v, ParkingLot *p) {

    const char *typeStr;

    if (v.type == MOTO) typeStr = "Motorbike";
    else if (v.type == CAR) typeStr = "Car";
    else typeStr = "Bus";


    double seconds = difftime(v.exitTime, v.entryTime);
    double hours = seconds / 3600.0;

    if (hours < 1) hours = 1;

    int parkingHours = (int)ceil(hours);


    double baseFee = parkingHours * p->prices[v.type].price;


    double nightSurchargeFee = 0;
    int nightSurchargeHours = 0;

    time_t temp = v.entryTime;

    while (temp < v.exitTime) {

        struct tm *t = localtime(&temp);
        int hour = t->tm_hour;

        if (hour >= 23 || hour < 3) {
            nightSurchargeFee += 5000;
            nightSurchargeHours++;
        }

        temp += 3600;
    }


    double overtimeSurchargeFee = 0;

    if (hours > 72) {
        overtimeSurchargeFee = 50000;
    }

    double totalFee = baseFee + nightSurchargeFee + overtimeSurchargeFee;

    printf("\n=================================================\n");
    printf("                    PARKING BILL                \n");
    printf("=================================================\n");

    printf("Plate                 : %s\n", v.licensePlate);
    printf("Vehicle Type          : %s\n", typeStr);

    printf("Entry Time            : %s", ctime(&v.entryTime));
    printf("Exit Time             : %s", ctime(&v.exitTime));

    printf("-------------------------------------------------\n");

    printf("Parking Hours         : %d hours \n", parkingHours);

    printf("Base Fee              : ");
    printMoney(baseFee);
    printf("\n");

    printf("-------------------------------------------------\n");

    printf("Night Surcharge Hours : %d hours (23h - 3h)\n", nightSurchargeHours);
    printf("Night Surcharge Fee   : ");
    printMoney(nightSurchargeFee);
    printf("\n");

    printf("-------------------------------------------------\n");

    printf("Overtime Surcharge    : ");
    printMoney(overtimeSurchargeFee);
    printf("\n");

    printf("=================================================\n");

    printf("TOTAL BILL            : ");
    printMoney(totalFee);
    printf("\n");

    printf("=================================================\n\n");
}

void updatePrice(ParkingLot *p) {
    printf("\n===== UPDATE PRICE =====\n");

    printf("1. Motorbike (current: %.0f VND/hour)\n", p->prices[MOTO].price);
    printf("2. Car       (current: %.0f VND/hour)\n", p->prices[CAR].price);
    printf("3. Bus       (current: %.0f VND/hour)\n", p->prices[BUS].price);

    int choice = getInt(
        "Choose vehicle type (1-3): ",
        "Invalid choice! Enter 1, 2 or 3.",
        "Input must be a number!",
        1, 3
    );

    double newPrice = getDouble(
        "Enter new price (1000 - 100000 VND/hour): ",
        "Invalid price! Must be a number >= 1000",
        0
    );


    if (newPrice < 1000 || newPrice > 100000) {
        printf("Price must be between 1000 and 100000 VND/hour!\n");
        return;
    }

    int i = choice - 1;
    p->prices[i].price = newPrice;

    printf("\n===== PRICE UPDATED SUCCESSFULLY =====\n");
    printf("Motorbike : %.0f VND/hour\n", p->prices[MOTO].price);
    printf("Car       : %.0f VND/hour\n", p->prices[CAR].price);
    printf("Bus       : %.0f VND/hour\n", p->prices[BUS].price);
    printf("=======================================\n");
}