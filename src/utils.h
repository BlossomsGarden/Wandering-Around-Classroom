#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
const  float RectVertices[] = {
	-1.0f,  1.0f,0.0f,	0.0f,0.0f,-1.0f,	0.0f,1.0f,
	 1.0f, -1.0f,0.0f,	0.0f,0.0f,-1.0f,	1.0f,0.0f,
	-1.0f, -1.0f,0.0f,	0.0f,0.0f,-1.0f,	0.0f,0.0f,
	

	 1.0f, -1.0f,0.0f,	0.0f,0.0f,-1.0f,	1.0f,0.0f,
	-1.0f,  1.0f,0.0f,	0.0f,0.0f,-1.0f,	0.0f,1.0f,
	 1.0f,  1.0f,0.0f,	0.0f,0.0f,-1.0f,	1.0f,1.0f
};

const float CubeVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};


#define ASSERT(x)    if(!(x)) __debugbreak();
/*注意函数复制操作和if语句*/
#define GLCALL(x)    GLClearError();\
                     x;\
                     ASSERT(GLLogCall(#x,__FILE__,__LINE__))
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
unsigned int GetSizeOfType(unsigned int type);

