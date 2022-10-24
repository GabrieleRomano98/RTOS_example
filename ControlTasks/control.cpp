#include "control.h"
#include <iostream>
using namespace std;

static double desiredValue;
void setDesired(const double val) {
	desiredValue = val;
}
double getDesired() {
	return desiredValue;
}

/****************** Task functions ******************/
//Calculate the arithmetic mean of 10 measurements executed by a sensor
void singleAvgTask(void*);
//Reads the results of the other tasks and calculates their arithmetic mean
void totalAvgTask(void*);
//Compares the plant temperature with the desired temperature and sends the correction action to the actuator
void correctionTask(void*);
/****************************************************/


/* The setup function initializes the tasks */
void setupControl(sharedStructure& sharedS) {
	BaseType_t rReturned;
	//N tasks reads 100 values from the sensor and calcualates the arithmetic mean
	for (int i = 0; i < N_TASK; i++)
		rReturned = xTaskCreate(
			singleAvgTask,
			"single Avg",
			configMINIMAL_STACK_SIZE,
			(void*)&sharedS,
			tskIDLE_PRIORITY,
			nullptr
		);
	//This task calculates the arithmetic mean of the previous arithmetic means
	rReturned = xTaskCreate(
		totalAvgTask,
		"total Avg",
		configMINIMAL_STACK_SIZE,
		(void*)&sharedS,
		tskIDLE_PRIORITY,
		nullptr
	);
	//The last task sends the action that the actuator should perform
	rReturned = xTaskCreate(
		correctionTask,
		"correction task",
		configMINIMAL_STACK_SIZE,
		(void*)&sharedS,
		tskIDLE_PRIORITY,
		nullptr
	);
}

void singleAvgTask(void* p) {
	auto P = (sharedStructure*)p;
	double avg = 0;
	const TickType_t xPeriod = pdMS_TO_TICKS(100);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while (true) {
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		for (int i = 0; i < 100; i++)
			avg += readSensorRegister();
		P->addValue(avg/100);
		avg = 0;
	}
}

void totalAvgTask(void* p) {
	bool airbagReady = true;
	const TickType_t xPeriod = pdMS_TO_TICKS(150);
	auto P = (sharedStructure*)p;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	double t, prvSpeed = 0;
	while (true) {
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		t = P->getValue();
		if (prvSpeed - t > 10 && airbagReady) {
			activateAirbag();
			airbagReady = false;
		}
		else
			prvSpeed = t;
		P->sendMessage(t);
	}
}

void correctionTask(void* p) {
	auto P = (sharedStructure*)p;
	const auto xPeriod = pdMS_TO_TICKS(150);
	auto xLastWakeTime = xTaskGetTickCount();
	double t;
	while (true) {
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		t = P->receiveMessage();
		if (t > desiredValue)
			writeActuatorRegister(DECREASE_SPEED);
		else if (t < desiredValue)
			writeActuatorRegister(INCREASE_SPEED);
		else
			writeActuatorRegister(NO_ACTION);
	}
}