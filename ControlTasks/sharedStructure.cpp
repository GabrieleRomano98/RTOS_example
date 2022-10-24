#include "sharedStructure.h"
#include "queue.h"

sharedStructure::sharedStructure() : avgBuffer(0) {
	totalAvgBuffer = xMessageBufferCreate(100);			/* The the message buffer used to send/receive the calculated temperature			*/
	totalTaskStart = xEventGroupCreate();				/* The total task must wait untill the last single task has added its contribute	*/
	singleTasksSem  = xSemaphoreCreateMutex();			/* The single tasks cannot access the avg buffer in parallel						*/
	stSem = xSemaphoreCreateCounting(N_TASK, N_TASK);	/* Counts how many contributes have been added by the single tasks					*/
}

void sharedStructure::addValue(const double singleAvg) {
	xSemaphoreTake(singleTasksSem, INFINITE);			/* Enter the critial section														*/
	xSemaphoreTake(stSem, INFINITE);					/* If all the N_TASK contributes have already been added, the task waits			*/
	avgBuffer += singleAvg;								/* Sums its contribute																*/
	if (!uxSemaphoreGetCount(stSem))					/* If the task is the last one to add its contribute,								*/
		xEventGroupSetBits(totalTaskStart, 1);			/* 	unlocks the total task															*/
	xSemaphoreGive(singleTasksSem);						/* Exits the critical section														*/
}

double sharedStructure::getValue() {
	double t;
	xEventGroupWaitBits(								/* Waits for the the last task to add its contribute*/
		totalTaskStart, 								/* The event group being tested.													*/
		1,												/* The bit within the event group to wait for.										*/
		pdTRUE,											/* The event group is cleared before returning.										*/
		pdFALSE,										/* There is only one bit to wait for.												*/
		INFINITE										/* Waits indefinitely.																*/
	);
	t = avgBuffer;										/* Reads the shared resource														*/
	avgBuffer = 0;										/* Reset the sum of the avg and the number of contributes							*/
	for(int i=0; i<N_TASK; i++)							/* Releases all the single tasks waiting to add contributes							*/
		xSemaphoreGive(stSem);
	return t/N_TASK;
}


void sharedStructure::sendMessage(const double t) const {
	xMessageBufferSend(
		totalAvgBuffer,
		(void*)&t,
		sizeof(&t),
		INFINITE
	);
}

double sharedStructure::receiveMessage() const {
	double t;
	xMessageBufferReceive(
		totalAvgBuffer,
		(void*)&t,
		sizeof(&t),
		INFINITE
	); 
	xMessageBufferReset(totalAvgBuffer); 
	return t;
}


void sharedStructure::fault_hang() {
	/* This function is defined in queue.c */
	faultSem(singleTasksSem);
}

void sharedStructure::fault_deelay() {
	/* This function is defined in queue.c */
	faultSem_temp(singleTasksSem);
}