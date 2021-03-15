#include "GLSLShader.h"
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <string>

GLSLShader::GLSLShader(const char* const PathVS, const char* const PathFS)
{
	std::string vsCode = "", fsCode = "";
	FILE *fpVS = std::fopen(PathVS, "rb"), *fpFS = std::fopen(PathFS, "rb");
	if (!fpVS) { 
		fprintf(stdout, "cant open vs");
		return;
	}
	if (!fpFS) {
		fprintf(stdout, "cant open Fs");
		return;
	}

	char code[1024];
	while (true) {
		size_t realsize = std::fread(code, 1, sizeof(code), fpVS);
		vsCode.append(code, realsize);
		if (feof(fpVS))  break;
	}
	fclose(fpVS);
	while (true) {
		size_t realsize = std::fread(code, 1, sizeof(code), fpFS);
		fsCode.append(code, realsize);
		if (feof(fpFS))  break;
	}
	fclose(fpFS);
	
	const GLchar *szCodeVS = vsCode.c_str(), *szCodeFS = fsCode.c_str();
	
	GLint success;
	GLchar info[1024];

	GLuint vsID, fsID;
	vsID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1, &szCodeVS, NULL);
	glCompileShader(vsID);
	glGetShaderiv(vsID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vsID, 1024, NULL, info);
		fprintf(stdout, "vs compilation error: %s\n", info);
	}

	fsID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsID, 1, &szCodeFS, NULL);
	glCompileShader(fsID);
	glGetShaderiv(fsID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fsID, 1024, NULL, info);
		fprintf(stdout, "ps compilation error: %s\n", info);
	}

	programID = glCreateProgram();
	glAttachShader(programID, vsID);
	glAttachShader(programID, fsID);
	glLinkProgram(programID);
	glGetShaderiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fsID, 1024, NULL, info);
		fprintf(stdout, "program compilation error: %s\n", info);
	}

	glDeleteShader(vsID);
	glDeleteShader(fsID);
}

void GLSLShader::SetConstant(enum GLSLCONSTANTS type, const char* Name, void* Data)
{
	GLint location = glGetUniformLocation(this->programID, Name);
	switch (type)
	{
	case GLSLCONSTANTS::MAT4F:
		glUniformMatrix4fv(location, 1, GL_FALSE, reinterpret_cast<const float*>(Data));
		break;
	default:
		break;
	}
}

void GLSLShader::Use()
{
	glUseProgram(this->programID);
}
