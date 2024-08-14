#include "VertexBufferLayout.h"
#include "../utils.h"
#include "../Global.h"
/*undefined type*/
template<typename T>
void VertexBufferLayout::Push(unsigned int count) {
	ASSERT(false);
}

/*specialize*/
//float
template<>
void VertexBufferLayout::Push<float>(unsigned int count) {
	m_Elements.push_back({ GL_FLOAT,count,GL_FALSE });
	m_Stride += GetSizeOfType(GL_FLOAT) * count;
}

//unsigned int
template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
	m_Elements.push_back({ GL_UNSIGNED_INT,count,GL_FALSE });
	m_Stride += GetSizeOfType(GL_UNSIGNED_INT) * count;
}

//char
template<>
void VertexBufferLayout::Push<unsigned char>(unsigned int count) {
	m_Elements.push_back({ GL_UNSIGNED_BYTE,count,GL_TRUE });
	m_Stride += GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}