/**************************************************************************************************
* Title: SmurfsTimer.h
* Authors: Gael Huber, Erin Mooney
* Description: Keeps time.
**************************************************************************************************/
#ifndef __SMURFSTIMER_H__
#define __SMURFSTIMER_H__

#include <Windows.h>

class Timer {
public:
	Timer(void);	// Constructor
	~Timer(void);	// Destructor

	void setupTimer(void);	// Setup the timer
	void setStartTime(void);	// Sets the starting time for the timer

	float getDeltaTime(void);	// Return the time elapsed between the last call
	float getNumTicksPerMs(void);	// Get the number of timer ticks per millisecond

private:
	INT64 frequency;	// Number of ticks per millisecond
	INT64 startTime;	// Time at which the time started
	INT64 currentTime;	// Current time elapsed
};

#endif