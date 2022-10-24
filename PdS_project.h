// PdS_project.h: include files of the project
#ifndef PdSProject___H
#define PdSProject___H

/* Standard includes. */
#include <stdlib.h>
#include <conio.h>

#include <iostream>
using namespace std;

/* Visual studio intrinsics used so the __debugbreak() function is available
should an assert get hit. */
#include <intrin.h>

// FreeRTOS files
#include "FreeRTOS.h"
#include "task.h"

// Specific files
#include "plant.h"
#include "control.h"
#include "sharedStructure.h"
#include "interface.h"

#include "config.h"

#endif