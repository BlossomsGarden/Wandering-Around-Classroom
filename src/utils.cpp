#include "utils.h"
void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::cout << "[OPENGL ERROR](0x" << std::hex << (int)error << std::dec << ")" << function << ' ' << file << ":line " << line << std::endl;
		return false;
	}
	return true;
}

unsigned int GetSizeOfType(unsigned int type)
{
	switch (type) {
		case GL_FLOAT:				return 4;
		case GL_UNSIGNED_INT:		return 4;
		case GL_UNSIGNED_BYTE:		return 1;
	}
	ASSERT(false);//invalid type
	return 0;
}