#ifndef CONTROL__H
#define CONTROL__H

// FreeRTOS files
#include "FreeRTOS.h"
#include "task.h"
#include "config.h"

#include "plant.h"
#include "sharedStructure.h"

void setDesired(const double);			//changes the desired value for the plant
void setupControl(sharedStructure&);	//inizializes the tasks of the controller
double getDesired();						//returns the desired temperature

#endif