#include "VertexBuffer.h"
#include "../Global.h"
#include "../utils.h"
/*data--pointer to data array
  size--total size of the data array
*/
VertexBuffer::VertexBuffer(const void* data, unsigned int size):m_RendererID(0)
{
	/*check the consistency of the size of unsigned int of different platforms*/
	ASSERT(sizeof(GLuint) == sizeof(unsigned int));

	GLCALL(glGenBuffers(1, &this->m_RendererID));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

/*delete the buffer*/
VertexBuffer::~VertexBuffer()
{
	GLCALL(glDeleteBuffers(1, &this->m_RendererID));
}


void VertexBuffer::Bind()const 
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID));
}



void VertexBuffer::Unbind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER,0));
}

void VertexBuffer::SetData(const void* data, unsigned int size)
{
	if (m_RendererID == 0) {
		ASSERT(sizeof(GLuint) == sizeof(unsigned int));

		GLCALL(glGenBuffers(1, &this->m_RendererID));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}
}
