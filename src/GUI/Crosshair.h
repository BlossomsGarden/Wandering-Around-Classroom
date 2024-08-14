#pragma once
#include <glm/glm.hpp>
class Crosshair {
private:
	glm::vec3 color;
	float size;
	float thickness;
public:
	Crosshair(glm::vec3 color, float size, float thickness)
	{
		this->color = color;
		this->size = size;
		this->thickness = thickness;

	};
	void SetColor(glm::vec3 color) { this->color = color; };
	void SetSize(float size) { this->size = size; };
	void SetThickness(float thickness) { this->thickness = thickness; };

	float GetSize()const { return size; };
	glm::vec3 GetColor()const { return color; };
	float GetThickness()const { return thickness; };




};