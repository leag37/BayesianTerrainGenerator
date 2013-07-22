/**************************************************************************************************
* Title: SmurfsGlobals.cpp
* Authors: Gael Huber
* Description: SMURFS engine globals
**************************************************************************************************/
#include "SmurfsGlobals.h"

// Fullscreen
static bool fullscreen;
bool getFullscreen(void) { return fullscreen; }
void setFullscreen(bool value) { fullscreen = value; }

// Window width
static int windowWidth;
int getWindowWidth(void) { return windowWidth; }
void setWindowWidth(int value) { windowWidth = value; }

// Window height
static int windowHeight;
int getWindowHeight(void) { return windowHeight; }
void setWindowHeight(int value) { windowHeight = value; }

// Window focus
static bool windowFocus;
bool getWindowFocus(void) { return windowFocus; }
void setWindowFocus(bool value) { windowFocus = value; }

// Game paused
static bool gamePaused = false;
bool getGamePaused(void) { return gamePaused; }
void toggleGamePaused(void) { gamePaused = !gamePaused; }