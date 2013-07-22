/**************************************************************************************************
* Title: SmurfsShader.h
* Authors: Gael Huber
* Description: Shader used for GLSL and more efficient rendering.
**************************************************************************************************/
#ifndef __SHADER_H__
#define __SHADER_H__

#include "OpenGL/glew-1.5.8/include/GL/glew.h"
#include <GL/GL.h>
#include "OpenGL/glext.h"
#include <string>
#include <iostream>

class Shader {
public:
	Shader(void);	// Constructor
	Shader(const char* vsFile, const char* fsFile);	// Constructor
	~Shader(void);	// Destructor

	void init(const char* vsFile, const char* fsFile);	// Initializer

	void bind(void);	// Enable the shader
	void unbind(void);	// Disable the shader

	void getShaderLog(void);	// Read the shader info log
	
	unsigned int getID();	// Get the number associated with the shader

private:
	char* textFileRead(const char* filename);	// Reads in text from a file and returns it

	unsigned int shader_id;	// ID number associated with the shader
	unsigned int shader_vp;	// Vertex shader
	unsigned int shader_fp;	// Fragment shader
};

#endif