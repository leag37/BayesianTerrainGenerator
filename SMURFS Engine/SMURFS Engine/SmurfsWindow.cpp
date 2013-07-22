/**************************************************************************************************
* Title: Windows.cpp
* Authors: Gael Huber
* Description: Defines a standard win32 window.
**************************************************************************************************/
#include "SmurfsWindow.h"

/**************************************************************************************************
* Windows process
*
* HWND	Handle for this window
* UINT	Message for this window
* WPARAM	Additional message information
* LPARAM	Additional message information
**************************************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// Check for Windows messages
	switch(uMsg) {
	case WM_ACTIVATE:	// Watch for Window activate message
		// Check minimization state
		if( !HIWORD(wParam) ) {
			setWindowFocus(true);
		} else {
			setWindowFocus(false);
		}

		return 0;	// Return to the message loop

	case WM_SYSCOMMAND:	// Intercept system commands
		// Check system calls
		switch(wParam) {
		case SC_SCREENSAVE:	// Screensaver trying to start?
		case SC_MONITORPOWER:	// Monitor trying to enter powersave?
			return 0;	// Prevent from happening
		}
		break;

	case WM_CLOSE:	// Did we receive a close message?
		PostQuitMessage(0);	// Quit
		break;
	}

	// Pass all unhandled messages to DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/**************************************************************************************************
* Constructor
**************************************************************************************************/
Window::Window(void) : hRC(NULL), hDC(NULL), hWnd(NULL) {
	// Do nothing
}

/**************************************************************************************************
* Destructor
**************************************************************************************************/
Window::~Window(void) {
	// Do nothing!
}

/**************************************************************************************************
* Initialize the window
**************************************************************************************************/
bool Window::startUp(wchar_t* windowName, int width, int height) {
	WNDCLASS wc;	// Window classs
	DWORD dwExStyle;	// Window extended style
	DWORD dwStyle;	// Window style
	RECT windowRect;	// Grabs rectangle upper left/lower right values

	// Set the window's dimensions
	if(!setWindowDimensions(&windowRect, width, height))
		return false;

	// Define and register the window class
	if(!registerWindow(&wc))
		return false;

	// Attempt fullscreen mode
	//if(!attemptFullscreen(&dwExStyle, &dwStyle))
		//return false;
	ShowCursor(FALSE);
	
	// Adjust window to true requested size
	//AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle = WS_OVERLAPPEDWINDOW;

	// Create the window
	if(!createWindow(dwExStyle, dwStyle, windowRect, windowName))
		return false;

	// Create the rendering context for the window
	if(!createRenderingContext())
		return false;

	ShowWindow(hWnd, SW_SHOW);	// Show the window
	UpdateWindow(hWnd);	// Update the window once
	SetForegroundWindow(hWnd);	// Slighly higher priority
	SetFocus(hWnd);	// Sets keyboard focus to the window
	resize();	// Set up our perspective GL screen

	// Initialization successful
	return true;
}

/**************************************************************************************************
* Shutdown the window
**************************************************************************************************/
void Window::shutDown(void) {
	// Are we in fullscreen?
	if ( getFullscreen() ) {
		ChangeDisplaySettings(NULL, 0);		// If so, switch back to the desktop
		ShowCursor(TRUE);	// Show mouse pointer
	}

	// Do we have a rendering context?
	if ( hRC ) {
		// Are we able to release the DC and RC contexts?
		if ( !wglMakeCurrent(NULL, NULL) ) {
			MessageBoxA(NULL, "Release of DC and RC failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		// Are we able to delete the rendering context?
		if ( !wglDeleteContext(hRC) ) {
			MessageBoxA(NULL, "Release rendering context failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		hRC = NULL;	// Set rendering context to NULL

		// Are we able to release the device context?
		if ( hDC && !ReleaseDC(hWnd, hDC) ) {
			MessageBoxA(NULL, "Release device context failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
			hDC = NULL;	// Set device context to NULL
		}

		// Are we able to destroy the window?
		if ( hWnd && !DestroyWindow(hWnd) ) {
			MessageBoxA(NULL, "Could not release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
			hWnd = NULL;	// Set window handle to NULL
		}

		// Are we able to unregister class?
		if ( !UnregisterClassA("OpenGL", hInstance) ) {
			MessageBoxA(NULL, "Could not unregister class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
			hInstance = NULL;
		}
	}
}

/**************************************************************************************************
* Resize the window by changing the viewport
**************************************************************************************************/
void Window::resize(void) {
	if ( getWindowHeight() == 0 ) {	// Prevent a divide by zero by making height equal 1
		setWindowHeight(1);
	}

	glViewport(0, 0, getWindowWidth(), getWindowHeight());	// Reset the current viewport
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)getWindowWidth()/(GLfloat)getWindowHeight(),0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
}

/**************************************************************************************************
* Attempt to define and register the window class
**************************************************************************************************/
bool Window::registerWindow(WNDCLASS* wc) {
	// Grab a window instance and declare the window class
	hInstance = GetModuleHandle(NULL);	// Grab an instance for our window
	wc->style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw on move and own device context for window
	wc->lpfnWndProc = (WNDPROC) WndProc;	// WndProc handles messages
	wc->cbClsExtra = 0;	// No extra class data
	wc->cbWndExtra = 0;	// No extra window data
	wc->hInstance = hInstance;	// Set the instance
	wc->hIcon = LoadIcon(NULL, IDI_WINLOGO);	// Load the default icon
	wc->hCursor = LoadCursor(NULL, IDC_ARROW);	// Load the arrow pointer
	wc->hbrBackground = NULL;	// No background required for GL
	wc->lpszMenuName = NULL;	// We don't want a menu
	wc->lpszClassName = L"OpenGL";	// Set the class name

	// Attempt to register the window class
	if ( !RegisterClass(&*wc) ) {
		MessageBoxA(NULL, "Failed to register the window class.", "ERROR", MB_OK | MB_ICONINFORMATION);
		return false;	// exit and return FALSE
	}

	// Everything succeeded, return true
	return true;
}

/**************************************************************************************************
* Attempt to render in fullscreen if the user requests it
**************************************************************************************************/
bool Window::attemptFullscreen(DWORD* dwExStyle, DWORD* dwStyle) {
	// Ask the user which screen mode they prefer
	if( MessageBoxA(NULL, "Would you like to run in fullscreen mode?", "Start fullscreen?", MB_YESNO | MB_ICONQUESTION) == IDNO ) {
		setFullscreen(false); // Windowed mode
	} else {
		setFullscreen(true); // Fullscreen
	}

	// Attempt fullscreen mode?
	if(getFullscreen()) {
		DEVMODE dmScreenSettings;	// Device mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Make sure memory is cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);	// Size of the devmode structure
		dmScreenSettings.dmPelsWidth = getWindowWidth();	// Selected screen width
		dmScreenSettings.dmPelsHeight = getWindowHeight();	// Selected screen height
		dmScreenSettings.dmBitsPerPel = 32;	// Selected bits per pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try to set selected mode and get results.
		// *NOTE: CDS_FULLSCREEN gets rid of start bar
		if ( ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL ) {
			// If the mode fails, offer two options: quit or run in a window
			if( MessageBoxA(NULL, "The requested fullscreen mode is not supported by\nyour video card. Use windowed mode instead?", 
				"FULLSCREEN FAILED", MB_YESNO | MB_ICONEXCLAMATION) == IDYES ) {
					setFullscreen(false);	// Select windowed mode
			} else {
				// Pop up a message box letting user know the program is closing
				MessageBoxA(NULL, "Program will now close.", "ERROR", MB_OK | MB_ICONSTOP);
				return false;	// Exit and return false
			}
		}
	}

	// Are we still in fullscreen mode?
	if(getFullscreen()) {
		*dwExStyle = WS_EX_APPWINDOW;	// Window extended style
		*dwStyle = WS_POPUP;	// Windows style
		ShowCursor(FALSE);	// Hide mouse pointer
	} else {
		*dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// Window extended style
		*dwStyle = WS_OVERLAPPEDWINDOW;	// Windows style
		ShowCursor(FALSE);
	}

	// Regardless of whether fullscreen was set or not, the attempt was successful so return true
	return true;
}

/**************************************************************************************************
* Set window dimensions
**************************************************************************************************/
bool Window::setWindowDimensions(RECT* windowRect, int width, int height) {
	setWindowWidth(width);
	setWindowHeight(height);
	windowRect->left = (long) 0;
	windowRect->right = (long) width;
	windowRect->top = (long) 0;
	windowRect->bottom = (long) height;

	return true;
}
/**************************************************************************************************
* Create the viewable window
**************************************************************************************************/
bool Window::createWindow(DWORD dwExStyle, DWORD dwStyle, RECT windowRect, wchar_t* windowName) {	
	if ( !(hWnd = CreateWindowEx(	dwExStyle,	// Extended style for the window
									L"OpenGL", // Class name
									windowName, // Window title
									WS_CLIPSIBLINGS |	// Required window style
									WS_CLIPCHILDREN |	// Required window style
									dwStyle,	// Selected window style
									0, 0,	// Window position
									windowRect.right - windowRect.left,	// Calculate adjusted window width
									windowRect.bottom - windowRect.top,	// Calculate adjusted window height
									NULL,	// No parent window
									NULL,	// No menu
									hInstance,	// Instance
									NULL)) )	// Don't pass anything to WM_CREATE
	{
			shutDown();	// Reset the display
			MessageBoxA(NULL, "Window creation error.", "ERROR", MB_OK | MB_ICONINFORMATION);
			
			return false;	// Return FALSE
	}

	// Window creation was successful, so return true
	return true;
}

/**************************************************************************************************
* Create the rendering context for the window
**************************************************************************************************/
bool Window::createRenderingContext(void) {
	// Did we get a device context?
	if( !(hDC = GetDC(hWnd)) ) {
		shutDown();	// Reset the display
		MessageBoxA(NULL, "Can't create a GL device context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	
	// pfd tells windows how we want things to be
	PIXELFORMATDESCRIPTOR pfd;	// Create a new PIXELFORMATDESCRIPTOR (PFD) to help define how our window draws
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));	// Clear the PFD of any data
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);		// Size of this pixel format descriptor
	pfd.dwFlags =	PFD_DRAW_TO_WINDOW |			// Format must support window
					PFD_SUPPORT_OPENGL |			// Format must support OpenGL
					PFD_DOUBLEBUFFER;				// Must support double buffering
	pfd.iPixelType = PFD_TYPE_RGBA;		// Set our application to use RGBA pixels  
	pfd.cColorBits = 32;	// Give us 32 bits of color information (the higher, the more colors)  
	pfd.cDepthBits = 32;	// Give us 32 bits of depth information (the higher, the more depth levels)
	
	// Did Windows find a matching pixel format?
	GLuint pixelFormat;	// Holds the results after searching for a match
	if( !(pixelFormat = ChoosePixelFormat(hDC, &pfd)) ) {
		shutDown();	// Reset the display
		MessageBoxA(NULL, "Can't find a suitable pixel format.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// Are we able to set the pixel format?
	if( !SetPixelFormat(hDC, pixelFormat, &pfd) ) {
		shutDown();	// Reset the display
		MessageBoxA(NULL, "Can't set the pixel format.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// Setup the rendering context, we want to create a temporary rendering context first to ensure 
	// compatibility with older versions of OpenGL (default to 2.1 if unsuccessful at newer version)
	HGLRC tempContext = wglCreateContext(hDC);	// Create an OpenGL 2.1 context
	wglMakeCurrent(hDC, tempContext);	// Make OpenGL 2.1 the current context

	// Initialize GLEW
	if(glewInit() != GLEW_OK)
		return false;

	// Define attributes for a newer version of OpenGL, attempt to use OpenGL 4.1
	int attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,	// Set the MAJOR version of OpenGL to 4
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,	// Set the MINOR version of OpenGL to 1
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,	// Set our OpenGL context to be forward compatible
		0
	};

	if(wglewIsSupported("WGL_ARB_create_context") == 1) {
		hRC = wglCreateContextAttribsARB(hDC, NULL, attributes);	// Create an OpenGL context based on the defined attributes
		wglMakeCurrent(NULL, NULL);	// Remove the temporary context from being active
		wglDeleteContext(tempContext);	// Delete the temporary context
		wglMakeCurrent(hDC, hRC);	// Make our OpenGL 4.1 context current
	} else {
		// No support for OpenGL 4.1 and above, so use OpenGL 2.1 context
		hRC = tempContext;
	}

	int glVersion[2] = {-1, -1};
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL MAJOR version we are using  
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

	return true;
}