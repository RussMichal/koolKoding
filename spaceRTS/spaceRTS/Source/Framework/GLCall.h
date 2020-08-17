#pragma once

#include <iostream>

//my macros
#define ASSERT(x) if(!x) __debugbreak();

#ifdef _DEBUG
#define GLCall(x)\
		ClearOpenGLErrors();\
		x;\
		ASSERT(GLFetchError(__FILE__, #x, __LINE__));
#else
#define GLCall(x) x;
#endif

void ClearOpenGLErrors();
bool GLFetchError(const char * file, const char* function, int line);