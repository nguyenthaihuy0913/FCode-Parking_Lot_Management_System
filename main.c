#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "include/types.h"
#include "include/parking.h"
#include "include/billing.h"
#include "include/fileio.h"
#include "include/report.h"
#include "include/utils.h"
#include "include/auth.h"


int main() {
    system("chcp 65001 > nul"); 
    ParkingLot parkingLot;
    Account accounts[MAX_ACCOUNTS];

    int accountCount = 0;
    int currentIndex = -1;
    int choice;

    initParkingLot(&parkingLot);
    loadData(&parkingLot);

    loadAccounts(accounts, &accountCount);
    initDefaultAdmin(accounts, &accountCount);

    while (currentIndex == -1) {
        currentIndex = login(accounts, accountCount);
    }

    do {

        int role = accounts[currentIndex].role;

        printf(YELLOW "\n\n      User: %s | Role: %s" RESET "\n",
               accounts[currentIndex].username,
               role == ROLE_ADMIN ? "Admin" : "Staff");

        printMenuByRole(role);

        choice = getInt(YELLOW "Choose: " RESET,
                        "Invalid menu option.",
                        "Invalid input.",
                        0, (role == ROLE_ADMIN ? 13 : 6));

        switch (choice) {
            case 1:
                addVehicle(&parkingLot);
                break;

            case 2:
                removeVehicle(&parkingLot);
                break;

            case 3:
                listVehicles(&parkingLot);
                break;

            case 4:
                searchVehicle(&parkingLot);
                break;
            case 5: 
                updateOwnAccount(accounts, accountCount, currentIndex);
                saveAccounts(accounts, accountCount);
                break;
            case 6: 
                if (saveData(&parkingLot) == 1) {
                    printf(GREEN "Data saved successfully.\n" RESET);
                } else {
                    printf(RED "Failed to save data!\n" RESET);
                }
                break;
            case 7: 
                viewDailyRevenue(&parkingLot);
                break;
            
            case 8:
                updatePrice(&parkingLot);
                break;

            case 9: 
                exportRevenueReport(&parkingLot);
                break;
            case 10:
                createStaffAccount(accounts, &accountCount);
                saveAccounts(accounts, accountCount);
                break;

            case 11: 
                listAccounts(accounts, accountCount);
                updateUserRole(accounts, accountCount);
                saveAccounts(accounts, accountCount);
                break;

            case 12: 
                listAccountsNPass(accounts, accountCount);
                break;

            case 13:
                deleteVehicle(&parkingLot);
                break;

            case 0:
                saveData(&parkingLot);
                saveAccounts(accounts, accountCount);
                printf(GREEN "Exiting program. Goodbye!\n" RESET);
                break;
        }

    } while (choice != 0);

    return 0;
}