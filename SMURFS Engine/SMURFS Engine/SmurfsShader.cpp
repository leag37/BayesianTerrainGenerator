/**************************************************************************************************
* Title: SmurfsShader.cpp
* Authors: Gael Huber
* Description: Shader used for GLSL and more efficient rendering.
**************************************************************************************************/
#include "SmurfsShader.h"
#include <string>

/**************************************************************************************************
* Constructor
**************************************************************************************************/
Shader::Shader() {  
}

/**************************************************************************************************
* Constructor
**************************************************************************************************/
Shader::Shader(const char* vsFile, const char* fsFile) {
    init(vsFile, fsFile);
} 

/**************************************************************************************************
* Destructor
**************************************************************************************************/
Shader::~Shader(void) {
	glDetachShader(shader_id, shader_fp);
    glDetachShader(shader_id, shader_vp);
    
    glDeleteShader(shader_fp);
    glDeleteShader(shader_vp);
    glDeleteProgram(shader_id);
}

/**************************************************************************************************
* Initialize the shader
**************************************************************************************************/
void Shader::init(const char* vsFile, const char* fsFile) {
	// Tell OpenGL to create a vertex shader and a fragment shader
	shader_vp = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	shader_fp = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	// Read the text from the shader files
	const char* vsText = textFileRead(vsFile);
	const char* fsText = textFileRead(fsFile);

	// Ensure that files were loaded correctly
	if(vsText == NULL || fsText == NULL) {
		return;
	}

	// Pass the file contents to the OpenGL and attach it to the shader
	glShaderSourceARB(shader_vp, 1, &vsText, 0);
	glShaderSourceARB(shader_fp, 1, &fsText, 0);
	
	// Compile our shaders
	glCompileShaderARB(shader_vp);
	glCompileShaderARB(shader_fp);

	// Create our shader_id as a shader program
	shader_id = glCreateProgramObjectARB();

	// Attach the shaders to the shader program
	glAttachObjectARB(shader_id, shader_vp);
	glAttachObjectARB(shader_id, shader_fp);

	glBindAttribLocationARB(shader_id, 0, "in_Position");	// Bind a constant attribute location for positions of vertices
	glBindAttribLocationARB(shader_id, 1, "in_Color");	// Bind another constant attribute location, this time for color

	// Link the program
	glLinkProgramARB(shader_id);
}

/**************************************************************************************************
* Enable the shader
**************************************************************************************************/
void Shader::bind(void) {
	glUseProgram(shader_id);
}

/**************************************************************************************************
* Disable the shader
**************************************************************************************************/
void Shader::unbind(void) {
	glUseProgram(0);
}

/**************************************************************************************************
* Get the number associated with the shader
**************************************************************************************************/
unsigned int Shader::getID(void) {
	return shader_id;
}

/**************************************************************************************************
* Reads in text from a file and returns it
**************************************************************************************************/
char* Shader::textFileRead(const char *fileName) {
    char* text;
    
    if (fileName != NULL) {
        FILE* file;
		fopen_s(&file, fileName, "rt");
        
        if (file != NULL) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);
            
            if (count > 0) {
                text = (char*)malloc(sizeof(char) * (count + 1));
                count = fread(text, sizeof(char), count, file);
                text[count] = '\0';
            }
            fclose(file);
        }
    }
    return text;
}

/**************************************************************************************************
* Read the shader info log
**************************************************************************************************/
void Shader::getShaderLog(void) {
	// Log the shader
	int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetObjectParameterivARB(shader_id, GL_OBJECT_INFO_LOG_LENGTH_ARB,
					 &infologLength);

    if (infologLength > 0)
    {
		infoLog = (char *)malloc(infologLength);
		glGetInfoLogARB(shader_id, infologLength, &charsWritten, infoLog);
		//printf("%s\n",infoLog);
		free(infoLog);
    }
}