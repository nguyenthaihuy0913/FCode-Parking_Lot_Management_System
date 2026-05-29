#ifndef UTILS_H
#define UTILS_H
#include "types.h"
#define GREEN "\033[0;32m" 
#define YELLOW "\033[0;33m" 
#define RED "\033[0;31m" 
#define TITLE "\033[1;36m" 
#define LINE "\033[38;2;70;130;180m" 
#define RESET   "\033[0m"
int getInt(const char *messageInfo,const char *messageErrorOutOfRange,const char *messageErrorNumber,int min, int max);

int getIntUpdate(const char *messageInfo,const char *messageErrorOutOfRange,const char *messageErrorNumber,int min, int max,int oldValue);

double getDouble(const char *messageInfo,const char *messageError,double min);

void getString(const char *messageInfo,char *buffer,int maxLen);

void getStringUpdate(const char *messageInfo,char *buffer,int maxLen,const char *oldValue);
void toUpperCase(char *str);
void trim(char *str);
int isEmpty(const char *str);

int isValidLicensePlate(const char *plate);
int isDuplicateLicensePlate(Vehicle vehicles[], int count, const char *plate);

int isValidVehicleType(int type);
int isValidStatus(int status);

int parseDateTime(const char *dateStr, time_t *result);
int isValidEntryExitTime(time_t entryTime, time_t exitTime);
int isValidEntryTime(time_t entryTime);
int isValidUsername(const char *username);
int isValidPassword(const char *password);
void printMoney(double amount);
void inputPassword(char *password, int maxSize);
#endif
