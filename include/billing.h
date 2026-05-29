#ifndef BILLING_H
#define BILLING_H

#include "parking.h"
#include "types.h"

double calculateFee(Vehicle v, ParkingLot *p);
void   printBill(Vehicle v, ParkingLot *p);
void   updatePrice(ParkingLot *p);

#endif