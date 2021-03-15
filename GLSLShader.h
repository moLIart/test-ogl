
#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include <glad/glad.h>

enum class GLSLCONSTANTS {
	MAT4F
};

class GLSLShader
{
public:
	GLuint programID;
public:
	GLSLShader(const char* const PathVS, const char* const PathPS);

	void SetConstant(enum GLSLCONSTANTS type, const char* Name,  void* Data);

	void Use();
};

#endif