/**************************************************************************************************
* Title: SmurfsGlobals.h
* Authors: Gael Huber
* Description: SMURFS engine globals
**************************************************************************************************/
#ifndef __SMURFSGLOBALS_H__
#define __SMURFSGLOBALS_H__

#include <Windows.h>

// Fullscreen
bool getFullscreen(void);
void setFullscreen(bool value);

// Window width
int getWindowWidth(void);
void setWindowWidth(int value);

// Window height
int getWindowHeight(void);
void setWindowHeight(int value);

// Window focus
bool getWindowFocus(void);
void setWindowFocus(bool value);

// Game paused
bool getGamePaused(void);
void toggleGamePaused(void);

#endif