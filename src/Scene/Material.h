#include <glm/glm.hpp>
class Material {
public:
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;

};

class DefaultMaterial :public Material
{
private:
	unsigned int uniformblockid;
};