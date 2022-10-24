#ifndef PLANT__H
#define PLANT__H

#define INCREASE_SPEED 1
#define DECREASE_SPEED -1
#define NO_ACTION 0

//Plant functions
void setupPlant();						//intializes the plant task
void plantTask(void*);					//simulates the effect of the external temperature on the plant
void actuatorTask(void*);				//simulates the effect of the actuator on the plant
void activateAirbag();
void simulateCrash();
void stopPlant();

//Plant interface function
double readSensorRegister();				//reads the register of the sensor
void writeActuatorRegister(const int);	//writes the register of the actuator

#endif