#include "PdS_project.h"

int main() {
	int action = 0;
	double t;
	cout << "Insert the desired speed for the plant\n";
	cin >> t; 
	sharedStructure sharedS;
	setDesired(t);
	setupPlant();
	setupControl(sharedS); 
	startInterface(sharedS);
	vTaskStartScheduler();

	return 0;
}