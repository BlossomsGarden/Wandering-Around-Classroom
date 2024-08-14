#pragma once
#include "EntityObject3D.h"
class Cube :
    public EntityObject3D
{
private:
    int m_Width;
    int m_Height;
    int m_Depth;
    void Init()override;
public:
    Cube(glm::vec3 Pos,int width ,int height ,int depth)
        :EntityObject3D(Pos),m_Width(width),m_Height(height),m_Depth(depth)
    {
        this->Init();
    }


	Cube(glm::vec3 Pos, int width, int height, int depth,unsigned int slot)
		:EntityObject3D(Pos,slot), m_Width(width), m_Height(height), m_Depth(depth)
	{
		this->Init();
	}


};

