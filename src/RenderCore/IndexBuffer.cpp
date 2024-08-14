#include "IndexBuffer.h"
#include "../utils.h"
#include "../Global.h"


/*
count -- the number of indices
*/
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) :m_Count(count), m_RendererID (0)
{
	/*check the consistency of the size of unsigned int of different platforms*/
	ASSERT(sizeof(GLuint) == sizeof(unsigned int));

	
	GLCALL(glGenBuffers(1, &this->m_RendererID));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_RendererID));
	GLCALL	(glBufferData(GL_ELEMENT_ARRAY_BUFFER,count *sizeof(unsigned int ), data, GL_STATIC_DRAW));

}

IndexBuffer::~IndexBuffer()
{
	GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind()const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind()const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
