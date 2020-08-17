#include "MEpch.h"
#include "GLCall.h"

#include <glew-2.1.0/include/GL/glew.h>

void ClearOpenGLErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLFetchError(const char * file, const char* function, int line)
{
	bool isNoError = true;
	while (unsigned int error = glGetError())
	{
		std::cout << "OpenGL Error: " << error << std::endl <<
			file << ": " << function << ": line:" << line << ": error:" << error;
		isNoError = false;
	}
	return isNoError;
}