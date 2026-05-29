#ifndef PARKING_H
#define PARKING_H

#include <time.h>
#include "types.h"

int findVehicleIndex(ParkingLot *p, const char *plate);
void initParkingLot(ParkingLot *p);
void addVehicle(ParkingLot *p);
void removeVehicle(ParkingLot *p);
void deleteVehicle(ParkingLot *p); 
void listVehicles(ParkingLot *p);
void searchVehicle(ParkingLot *p);

#endif