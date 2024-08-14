#include "Sphere.h"

#include "../../../Global.h"

void Sphere::Init()
{
	m_Scale = glm::vec3(m_Radius, m_Radius, m_Radius);
	m_VertexNum = (m_Precision + 1) * (m_Precision + 1);
	m_IndexNum = m_Precision * m_Precision * 6;


	for (int i = 0; i <= m_Precision; i++)
	{
		for (int  j= 0; j <= m_Precision; j++)
		{
			float xSegment = (float)i / (float)m_Precision;
			float ySegment = (float)j / (float)m_Precision;
			float x = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float y = std::cos(ySegment * PI);
			float z = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

		
			m_Vertices.push_back(x);
			m_Vertices.push_back(y);
			m_Vertices.push_back(z);
			
		
			m_Vertices.push_back(x);
			m_Vertices.push_back(y);
			m_Vertices.push_back(z);


			m_Vertices.push_back(j / (float)m_Precision);
			m_Vertices.push_back(i / (float)m_Precision);
		}
	}
	for (int i = 0; i < m_Precision; i++) {
		for (int j = 0; j < m_Precision; j++) {
			
			m_Indices.push_back(i * (m_Precision + 1) + j);
			m_Indices.push_back(i * (m_Precision + 1) + j + 1);
			m_Indices.push_back((i + 1) * (m_Precision + 1) + j);
			m_Indices.push_back(i * (m_Precision + 1) + j + 1);
			m_Indices.push_back((i + 1) * (m_Precision + 1) + j + 1);
			m_Indices.push_back((i + 1) * (m_Precision + 1) + j);
		}
	}
	

}




