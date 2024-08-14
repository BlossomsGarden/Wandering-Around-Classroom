#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
using namespace std;

#include "../RenderCore/Shader.h"
#include "../RenderCore/Texture.h"
#include "../Scene/SimpleObject/2D/Plane.h"

struct Particle {
    glm::vec3 position;
    glm::vec3 rotation;
    float rotation_i;
    float rotation_y;
    glm::vec3 velocity;
    bool statical;
    int life;
};

class ParticleSystem {
private:
    Texture particleTexture;
    Plane particlePlane;
    vector<Particle> particles;
    int maxlifespan;
    Shader& particleShader;

    int totnum;

    const float gravity = 9.87f;
    const float F = 10.0f;
    const float pi = 3.1416;

    float randFloat(float from, float to) {
        return from + (to - from) * 1.0 * rand() / RAND_MAX;
    }

    int randInt(int from, int to) {
        return from + rand() % (to - from);
    }

public:
    ParticleSystem(Shader& shader, const string& texturePath)
        : particleShader(shader), particleTexture(texturePath), particlePlane(glm::vec3(0.0f), 1.0f, 1.0f) {
        init(500);
    }

    void init(int totnum) {
        this->totnum = totnum;
        maxlifespan = 6000;
    }

    // for window near TDesk
    void update1(GLfloat deltaTime);
    void Draw1();

    // For Window far from TDesk
    void update2(GLfloat deltaTime);
    void Draw2();

    void DrawGenerateZone();

};

