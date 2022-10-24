#ifndef __SHSTR__
#define __SHSTR__

#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"
#include "message_buffer.h"

#define N_TASK 4

class sharedStructure {
private:
	double avgBuffer;
	SemaphoreHandle_t singleTasksSem;
	EventGroupHandle_t totalTaskStart;
	SemaphoreHandle_t stSem;
	MessageBufferHandle_t totalAvgBuffer;
public:
	sharedStructure();
	void addValue(const double);
	double getValue();
	void sendMessage(const double) const;
	double receiveMessage() const;
	void fault_hang();
	void fault_deelay();


};

#endif