#include "interface.h"

void interfaceTask(void* p) {
	int action = 0;
	double t;
	while (true) {

		cout << "Select the desired action\n";
		cout << "\t1) Show speed\n";
		cout << "\t2) Change speed\n";
		cout << "\t3) Simulate crash\n";
		cout << "\t4) Fault injection\n";
		
		cin >> action;
		switch (action) {
		case 1:
			cout << "\nThe current speed of the plant is: " << readSensorRegister() << "\n\n";
			break;
		case 2:
			cout << "Insert the desired speed for the plant\n";
			cin >> t;
			setDesired(t);
			break;
		case 3:
			simulateCrash();
			cout << "Crash simulated\n\n";
			break;
		case 4:
			fault_injection(*(sharedStructure*)p);
			break;
		default: break;
		}
	}
}

void startInterface(sharedStructure& s) {
	BaseType_t rReturned = xTaskCreate(
		interfaceTask,
		"interface",
		configMINIMAL_STACK_SIZE,
		(void*)&s,
		tskIDLE_PRIORITY,
		nullptr
	);
}