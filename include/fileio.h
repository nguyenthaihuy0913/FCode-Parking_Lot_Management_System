#ifndef FILEIO_H
#define FILEIO_H

#include "parking.h"
#include "auth.h"

void loadData(ParkingLot *p);
int saveData(ParkingLot *p);
void logDeletedVehicle(const Vehicle *v);

void loadAccounts(Account accounts[], int *accountCount);
int saveAccounts(Account accounts[], int accountCount);

#endif
