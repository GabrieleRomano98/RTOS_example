#include "interface.h"
#include "semphr.h"

static int fault = 0;

void fault_injection(sharedStructure& s) {
	int n = 0, sec;
	srand((int)&s);
	while (n < 1 || n>5) {
		cout << "\nInsert the number of fault to generate (from 1 to 5)\n";
		cin >> n;
	}
	cout << "\nInsert the maximum number of seconds to wait for the fault\n";
	cin >> sec;
	int bit;
	for (int i=0; i < n; i++) {
		cout << "\nSelect the fault\n";
		if(!(fault & 1))
			cout << "\t1) Masked error\n";
		if (!(fault & 2))
			cout << "\t2) Silence data corruption\n";
		if (!(fault & 4))
			cout << "\t3) Introduce deelay\n";
		if (!(fault & 8))
			cout << "\t4) Hang\n";
		if (!(fault & 16))
			cout << "\t5) Crash\n";
		cin >> bit;
		if (bit < 0 || bit > 5 || ((1 << bit) & fault)) {
			i--;
			cout << "Insert a value between 1 and 5 that you don't have already inserted";
		}
		else
			fault |= (1 << (bit-1));
	}
	if (sec) {
		sec = (rand() % sec + 1) * 1000;
		vTaskDelay(sec);
	}

	// Masked error
	/*A bit in the actuator register is set to 0, in the
	  worst case the difference in the output is of [-]0.5	*/
	if (fault & 1)
		writeActuatorRegister(0);
	// Silence data corruption
	/* The bit associated with the signum of the double
	   value is modified, the output will change			*/
	if (fault & 2)
		setDesired(-1 * getDesired());
	// Deelay
	/* The first bit of a mutex semaphore count is set 
	   to 0 for 5 seconds, then its value is restored*/
	if(fault & 4)
		BaseType_t rReturned = xTaskCreate(
			[](void* p) {((sharedStructure*)p)->fault_deelay();},
			"deelay",
			configMINIMAL_STACK_SIZE,
			(void*)&s,
			tskIDLE_PRIORITY,
			nullptr
		);
	// Hang event
	/* The first bit of a mutex semaphore count is set
	   to 0, causing a permanent deadlock					*/
	if (fault & 8)
		BaseType_t rReturned = xTaskCreate(
			[](void* p) {((sharedStructure*)p)->fault_hang();},
			"hang",
			configMINIMAL_STACK_SIZE,
			(void*)&s,
			tskIDLE_PRIORITY,
			nullptr
		);
	// RTOS crash
	/* The bit for the boolean value that memorizes if 
	   the scheduler is runnig is set to 0, causing the 
	   crash of the entire application						*/
	if (fault & 16)
		fault_crash();

	/*	For the hang event and the deelay the injection possibilities can 
		be further expanded by allowing the injection process to select 
		dinamically the semaphore that will cause the deelay/deadlock		*/
}