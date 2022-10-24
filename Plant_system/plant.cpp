#include "plant.h"

// FreeRTOS files
#include "FreeRTOS.h"
#include "task.h"
#include "config.h"

// std files
#include <cstdlib>
#include <iostream>

/*	The external temperature is defined in plant.h and is a fixed parameter.
	Initially, the plant temperature is equal to the external temperature
	The actuator register contains the action to perform on the plant */
static double plantSpeed;
static int actuatorRegister = 0;
static bool crash = false;

double readSensorRegister() {
	return plantSpeed + (rand() % 11 - 5) * 0.05;
}

void writeActuatorRegister(const int action) {
	actuatorRegister = action;
}

void setupPlant() {
	//The plant task is activated
	plantSpeed = 0;
	BaseType_t rReturned = xTaskCreate(
		plantTask,
		"plant",
		configMINIMAL_STACK_SIZE,
		nullptr,
		tskIDLE_PRIORITY,
		nullptr
	);
	rReturned = xTaskCreate(
		actuatorTask,
		"actuator",
		configMINIMAL_STACK_SIZE,
		nullptr,
		tskIDLE_PRIORITY,
		nullptr
	);
	srand(xTaskGetTickCount());
}

void plantTask(void* p) {
	const TickType_t xPeriod = pdMS_TO_TICKS(500);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while (true) {
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		/* The speed of the plant is updated every 50 milliseconds and tends to 0. */
		if (plantSpeed >= 0.1)
			plantSpeed -= 0.1;
	};
}

void actuatorTask(void* p) {
	const TickType_t xPeriod = pdMS_TO_TICKS(100);
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while (true) {
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		/* The actuatore modifies the speed of the plant every 10 milliseconds */
		if (crash) return;
		if (actuatorRegister > 0)
			plantSpeed += 0.5;
		else if (actuatorRegister < 0 && plantSpeed >= 0.5) {
			plantSpeed -= 0.5;
		}
	};
}

void simulateCrash() {
	// in case of crash, instantaneously decreases to 0
	crash = true;
	plantSpeed = 0;
}

void activateAirbag() {
	// The activation of the airbag is simulated with a message
		std::cout << "Airbag activated\n";
}