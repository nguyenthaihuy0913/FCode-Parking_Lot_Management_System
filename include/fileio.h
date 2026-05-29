#ifndef FILEIO_H
#define FILEIO_H

#include "parking.h"

void loadData(ParkingLot *p);
int saveData(ParkingLot *p);
void logDeletedVehicle(const Vehicle *v);

#endif
