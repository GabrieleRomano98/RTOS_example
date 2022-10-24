#ifndef INTERF_H
#define INTERF_H

#include <iostream>
#include "control.h"

using namespace std;

void startInterface(sharedStructure&);
void interfaceTask(void*);
void fault_injection(sharedStructure&);

#endif