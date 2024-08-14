#pragma once
#include "EntityObject3D.h"

class Sphere :public  EntityObject3D
{
private:
	
	float m_Radius;
	int m_Precision;
	void Init()override;
public:
	Sphere(glm::vec3 pos, float m_Radius, int m_Precision) :EntityObject3D(pos), m_Precision(m_Precision), m_Radius(m_Radius)
	{

		this->Init();
	};

	Sphere(glm::vec3 pos, float m_Radius, int m_Precision,unsigned int slot) :EntityObject3D(pos,slot), m_Precision(m_Precision), m_Radius(m_Radius)
	{

		this->Init();
	};

	

};

