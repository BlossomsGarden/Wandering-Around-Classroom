#pragma once
#include "../../../Global.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
class EntityObject3D
{
protected:
	glm::vec3 m_Pos;
	glm::vec3 m_Scale;
	glm::vec3 m_RotateAxis;
	float m_RotateAngle = 0.0f;
	unsigned int m_TextureSlot = 0;


	int m_IndexNum;
	int m_VertexNum;
	std::vector<float> m_TexCoords;
	std::vector<float> m_Vertices;
	std::vector<float> m_Normals;
	std::vector<unsigned int> m_Indices;

	virtual void Init() {};

public:

	EntityObject3D(glm::vec3 pos)
		:m_Pos(pos), m_IndexNum(0), m_VertexNum(0), m_Scale(glm::vec3(1.0f)), m_RotateAxis(glm::vec3(0.0f, 1.0f, 0.0f)), m_TextureSlot(0) {};
	EntityObject3D(glm::vec3 pos, unsigned int slot)
		:m_Pos(pos), m_IndexNum(0), m_VertexNum(0), m_Scale(glm::vec3(1.0f)), m_RotateAxis(glm::vec3(0.0f, 1.0f, 0.0f)), m_TextureSlot(slot) {};
	EntityObject3D()
		:m_Pos(glm::vec3(0.0f, 0.0f, 0.0f)), m_IndexNum(0), m_VertexNum(0), m_Scale(glm::vec3(1.0f)), m_RotateAxis(glm::vec3(0.0f, 1.0f, 0.0f)),m_TextureSlot(0)  {};
	std::vector<float> GetTexCoords()const { return m_TexCoords; };
	std::vector<float> GetVertices() const { return m_Vertices; };
	std::vector<unsigned int> GetIndices()		 const { return m_Indices; };
	std::vector<float> GetNormals() const { return m_Normals; };

	glm::vec3 GetPos() const { return m_Pos; }
	void SetPos(glm::vec3 pos) { this->m_Pos = pos; }

	int GetVertexNum() const { return m_VertexNum; }

	int GetIndexNum() const { return m_IndexNum; }
	;
	virtual void Finish()const {};





	glm::mat4 GetModel() { return glm::translate(glm::mat4(), m_Pos); };
	glm::mat4 GetModel(const float angle, const glm::vec3 axis, const glm::vec3 scaler) { return glm::translate(glm::rotate(glm::scale(glm::mat4(), scaler), angle, axis), m_Pos); };


	glm::vec3 GetScale() const { return m_Scale; }
	void SetScale(glm::vec3 val) { m_Scale = val; }



	glm::vec3 GetRotateAxis() const { return m_RotateAxis; }
	void SetRotateAxis(glm::vec3 val) { m_RotateAxis = val; }

	float GetRotateAngle() const { return m_RotateAngle; }
	void SetRotateAngle(float val) {
		if (val > 360.0f) {
			val -= 360.0f;
		}
		else if (val < -360.0f) {
			val += 360.0f;
		}
		m_RotateAngle = val;
	}



	unsigned int GetTextureSlot() const  { return m_TextureSlot; }
	void SetTextureSlot(unsigned int val) { m_TextureSlot = val; }
};
