#pragma once
#include <vector>
#include <GLEW/glew.h>


struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() :m_Stride(0){};



	inline std::vector<VertexBufferElement> GetElements() const& { return m_Elements; };
	inline unsigned int GetStride()const { return m_Stride; };
	template<typename T> void Push(unsigned int count);
};

