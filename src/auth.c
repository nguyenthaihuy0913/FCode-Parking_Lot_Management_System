#include <stdio.h>
#include <string.h>
#include "../include/auth.h"
#include "../include/utils.h"

void initDefaultAdmin(Account accounts[], int *accountCount) {
    if (*accountCount == 0) {
        strcpy(accounts[0].username, "admin");
        strcpy(accounts[0].password, "Admin123");
        accounts[0].role = ROLE_ADMIN;
        *accountCount = 1;
    }
}
int login(Account accounts[], int accountCount) {

    char username[30];
    char password[30];
    printf(LINE "================================== " TITLE "LOGIN" RESET LINE " ==================================" RESET "\n\n");
    int index = -1;
    while (index == -1) {
        getString("Enter username: ", username, sizeof(username));
        trim(username);
        index = findAccountByUsername(accounts, accountCount, username);
        if (index == -1) {
            printf(RED "Username '%s' not found. Please try again.\n" RESET, username);
        }
    }
    while (1) {
        printf("Enter password: ");
        inputPassword(password, sizeof(password));
        if (strcmp(password, accounts[index].password) == 0) {
            printf(GREEN "Login successfully!\n" RESET);
            return index;
        } else {
            printf(RED "Invalid password. Please try again.\n" RESET);
        }
    }
}
void printMenuByRole(int role) {
 
    printf("╔════════════════════════════════════════╗\n");
    printf("║        PARKING LOT MANAGEMENT          ║\n");
    printf("╚════════════════════════════════════════╝\n");

    printf("1. Check in a vehicle\n");
    printf("2. Check out a vehicle\n");
    printf("3. View parked vehicles\n");
    printf("4. Search a vehicle\n");
    printf("5. Change my password\n");
    printf("6. Save data\n");
    if (role == ROLE_ADMIN) {
    printf("7. View daily report\n");
    printf("8. Edit price list by vehicle type\n");
    printf("9. Export revenue report (.txt)\n");
    printf("10. Create staff account\n");
    printf("11. Change user's role\n");
    printf("12. View account list\n");
    printf("13. Delete vehicle permanently\n");
    }
    printf("0. Exit\n");
    printf(LINE "-----------------------------------------------------------------------------\n" RESET);
}
void updateOwnAccount(Account accounts[], int accountCount, int currentIndex) {
    char newPassword[30];
    printf(LINE "\n============================== " TITLE "CHANGE PASSWORD" RESET LINE " ==============================" RESET "\n");
    printf("Current account: %s\n", accounts[currentIndex].username);
    while (1) {
        printf("Enter new password: ");
        inputPassword(newPassword, sizeof(newPassword));
        if (isValidPassword(newPassword)) break;
    }
    strcpy(accounts[currentIndex].password, newPassword);
    printf(GREEN "Password updated successfully.\n" RESET);
}
int findAccountByUsername(Account accounts[], int accountCount, char username[]) {
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}
void createStaffAccount(Account accounts[], int *accountCount) {
    if (*accountCount >= MAX_ACCOUNTS) {
        printf(RED "Account list is full.\n" RESET);
        return;
    }
    char username[30];
    char password[30];
    printf(TITLE "\n============================ CREATE STAFF ACCOUNT ===========================\n" RESET);
    while (1) {
        getString("Enter username: ", username, sizeof(username));
        trim(username);
        if (isValidUsername(username)) break;
    }
    if (findAccountByUsername(accounts, *accountCount, username) != -1) {
        printf(RED "Username already exists.\n" RESET);
        return;
    }
    while (1) {
        printf("Enter password: ");
        inputPassword(password, sizeof(password));
        if (isValidPassword(password)) break;
    }
    strcpy(accounts[*accountCount].username, username);
    strcpy(accounts[*accountCount].password, password);
    accounts[*accountCount].role = ROLE_STAFF;
    (*accountCount)++;
    printf(GREEN "Staff account created successfully.\n" RESET);
}
void updateUserRole(Account accounts[], int accountCount) {
    char username[30];
    printf(LINE "\n================================ " TITLE "CHANGE USER ROLE" RESET LINE " ===========================" RESET "\n");
    getString("Enter username: ", username, sizeof(username));
    trim(username);
    int index = findAccountByUsername(accounts, accountCount, username);
    if (index == -1) {
        printf(RED "Account not found.\n" RESET);
        return;
    }
    if (accounts[index].role == ROLE_ADMIN) {
        accounts[index].role = ROLE_STAFF;
        printf(GREEN "Account has been demoted to Staff.\n" RESET);
    }else {
      accounts[index].role = ROLE_ADMIN;
      printf(GREEN "Account has been promoted to Admin.\n" RESET);
    }
}
void listAccounts(Account accounts[], int accountCount) {
    printf(LINE "\n=============================== " TITLE "ACCOUNT LIST" RESET LINE " ================================" RESET "\n");
    for (int i = 0; i < accountCount; i++) {
        printf("Username: %s\n", accounts[i].username);
        if (accounts[i].role == ROLE_ADMIN) printf("Role: ADMIN\n");
        else printf("Role: STAFF\n");
        printf(LINE "-----------------------------------------------------------------------------\n" RESET);
    }
}
void listAccountsNPass(Account accounts[], int accountCount) {
    printf(LINE "\n=============================== " TITLE "ACCOUNT LIST" RESET LINE " ================================" RESET "\n");
    for (int i = 0; i < accountCount; i++) {
        printf("Username: %s\n", accounts[i].username);
        printf("Password: %s\n", accounts[i].password);
        if (accounts[i].role == ROLE_ADMIN) printf( "Role: ADMIN\n" );
        else printf( "Role: STAFF\n" );
        printf(LINE "-----------------------------------------------------------------------------\n" RESET);
    }
}
void saveAccounts(Account accounts[], int accountCount) {
    FILE *file = fopen("data/accounts.dat", "wb");
    if (file == NULL) {
        printf(RED "Cannot open account file.\n" RESET);
        return;
    }
    fwrite(&accountCount, sizeof(int), 1, file);
    fwrite(accounts, sizeof(Account), accountCount, file);
    fclose(file);
}
void loadAccounts(Account accounts[], int *accountCount) {
    FILE *file = fopen("data/accounts.dat", "rb");
    if (file == NULL) {
        *accountCount = 0;
        return;
    }
    fread(accountCount, sizeof(int), 1, file);
    fread(accounts, sizeof(Account), *accountCount, file);
    fclose(file);
}