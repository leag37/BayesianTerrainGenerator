/**************************************************************************************************
* Title: Windows.h
* Authors: Gael Huber
* Description: Defines a standard win32 window.
**************************************************************************************************/
#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "SmurfsGlobals.h"
#include "OpenGL/glew-1.5.8/include/GL/glew.h"
#include "OpenGL/glew-1.5.8/include/GL/wglew.h"
#include <GL/GL.h>
#include <GL/GLU.h>

class Window {
public: 
	Window(void);	// Constructor
	~Window(void);	// Destructor

	bool startUp(wchar_t* windowName, int width, int height);	// Initialize the window
	void shutDown(void);	// Shutdown the window
	void resize(void);	// Setup the window to utilize OpenGL

	// Getters
	HDC getHDC(void) { return hDC; }
	HWND getHWND(void) { return hWnd; }

private:
	bool setWindowDimensions(RECT* windowRect, int width, int height);	// Set the window's dimensions
	bool attemptFullscreen(DWORD* dwExStyle, DWORD* dwStyle);	// Attempt to render in fullscreen if the user requests it
	bool registerWindow(WNDCLASS* wc);	// Attempt to define and register the window class
	bool createWindow(DWORD dwExStyle, DWORD dwStyle, RECT windowRect, wchar_t* windowName);	// Create the viewable window
	bool createRenderingContext(void);	// Create the rendering context for the window
	bool releaseRenderingContext(void);	// Release the rendering context for the window

	HGLRC hRC;	// Permanent rendering context
	HDC hDC;	// Private GDI device context
	HWND hWnd;	// Window handle
	HINSTANCE hInstance;	// Window instance
};

#endif