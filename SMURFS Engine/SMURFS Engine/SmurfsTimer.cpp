/**************************************************************************************************
* Title: SmurfsTimer.cpp
* Authors: Gael Huber, Erin Mooney
* Description: Keeps time.
**************************************************************************************************/
#include "SmurfsTimer.h"

/**************************************************************************************************
* Constructor
**************************************************************************************************/
Timer::Timer(void) {
	frequency = 0;
	startTime = 0;
	currentTime = 0;
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
Timer::~Timer(void) {
	// Nothing!
}

/**************************************************************************************************
* Setup the timer
**************************************************************************************************/
void Timer::setupTimer(void) {
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
}

/**************************************************************************************************
* Sets the starting time for the timer
**************************************************************************************************/
void Timer::setStartTime(void) {
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
}

/**************************************************************************************************
* Return the time elapsed between the last call
**************************************************************************************************/
float Timer::getDeltaTime(void) {
	// The count made in the last time this function was invoked
	static INT64 lastCount = 0;
	INT64 nCount = 0;

	// Query the current number of cou nts
	QueryPerformanceCounter((LARGE_INTEGER*)&nCount);

	// The difference, used to find deltaTime
	INT64 difference = 0;

	// Calculate the difference
	difference = nCount - lastCount;

	// Set the last count to the current count
	lastCount = nCount;

	// Base deltaTime on the count difference divided by the frequency per second. This will give 
	// you the deltaTime in seconds.
	return (float) difference / frequency;
}

/**************************************************************************************************
* Get the number of timer ticks per millisecond	
**************************************************************************************************/
float Timer::getNumTicksPerMs(void) {
	return (float) frequency / 1000.0f;
}