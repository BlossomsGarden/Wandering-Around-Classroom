

#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "particles.h"
#include "../RenderCore/Shader.h"
#include "../../vendor/include/stb/stb_image.h"
using namespace std;

void ParticleSystem::update1(GLfloat deltaTime) {
	Particle particle;
	int newNum = randInt(1, 20);
	if (newNum == 1) {
		if (particles.size() > totnum) {
			cout << "already max limit" << endl;
		}
		else {
			particle.position = glm::vec3(randFloat(35.0f, 60.0f), randFloat(15.0f, 20.0f), randFloat(-35.0f, -25.0f));
			particle.velocity = glm::vec3(randFloat(-1.0f, 1.0f), randFloat(-1.0f, -2.0f), randFloat(-0.1f, 1.0f));
			particle.rotation = glm::vec3(randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f));
			particle.rotation_i = randFloat(0.0f, 180);
			particle.statical = false;
			particle.life = maxlifespan;
			particles.push_back(particle);
		}
	}

	for (int i = 0; i < particles.size(); i++) {
		particles[i].life--;

		if (particles[i].statical == false && particles[i].position.y <= 0.0f) {
			particles[i].position.y = 0.1f;
			particles[i].velocity = glm::vec3(0, 0, 0);
			particles[i].rotation_i = 0;
			particles[i].rotation = glm::vec3(0.0, 90.0, 0.0);
			particles[i].statical = true;
			particles[i].rotation_y = randInt(0, 180);
			if (particles[i].position.z <= -20.0f || particles[i].life <= 0) {
				particles.erase(particles.begin() + i);
			}
			continue;
		}
		particles[i].position += deltaTime * particles[i].velocity;
		particles[i].position.z += 0.2 * F * deltaTime * particles[i].velocity.z;
		particles[i].position.y -= 0.5 * gravity * deltaTime * deltaTime * particles[i].velocity.y;

	}
}

void ParticleSystem::Draw1()
{
	particleShader.Bind();

	for (const auto& particle : particles) {
		glm::mat4 model = glm::mat4(1.0f);
		if (particle.statical) {
			model = glm::translate(model, particle.position);
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
			model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		}
		else {
			model = glm::translate(model, particle.position);
			model = glm::rotate(model, particle.rotation_i, particle.rotation);
			model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		}

		particleShader.SetMat4("model", model);

		particlePlane.Draw(particleShader, particleTexture);
	}
}

void ParticleSystem::update2(GLfloat deltaTime) {
	Particle particle;
	int newNum = randInt(1, 100);
	if (newNum == 1) {
		if (particles.size() > totnum) {
			cout << "already max limit" << endl;
		}
		else {
			particle.position = glm::vec3(randFloat(-5.0f, 5.0f), randFloat(15.0f, 20.0f), randFloat(-35.0f, -25.0f));
			particle.velocity = glm::vec3(randFloat(-1.0f, 1.0f), randFloat(-1.0f, -2.0f), randFloat(-0.1f, 1.0f));
			particle.rotation = glm::vec3(randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f));
			particle.rotation_i = randFloat(0.0f, 180);
			particle.statical = false;
			particle.life = maxlifespan;
			particles.push_back(particle);
		}
	}

	for (int i = 0; i < particles.size(); i++) {
		particles[i].life--;

		if (particles[i].statical == false && particles[i].position.y <= 0.0f) {
			particles[i].position.y = 0.1f;
			particles[i].velocity = glm::vec3(0, 0, 0);
			particles[i].rotation_i = 0;
			particles[i].rotation = glm::vec3(0.0, 90.0, 0.0);
			particles[i].statical = true;
			particles[i].rotation_y = randInt(0, 180);
			if (particles[i].position.z <= -20.0f || particles[i].life <= 0) {
				particles.erase(particles.begin() + i);
			}
			continue;
		}
		particles[i].position += deltaTime * particles[i].velocity;
		particles[i].position.z += 0.2 * F * deltaTime * particles[i].velocity.z;
		particles[i].position.y -= 0.5 * gravity * deltaTime * deltaTime * particles[i].velocity.y;

	}
}

void ParticleSystem::Draw2()
{
	particleShader.Bind();

	for (const auto& particle : particles) {
		glm::mat4 model = glm::mat4(1.0f);
		if (particle.statical) {
			model = glm::translate(model, particle.position);
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		}
		else {
			model = glm::translate(model, particle.position);
			model = glm::rotate(model, particle.rotation_i, particle.rotation);
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		}

		particleShader.SetMat4("model", model);

		particlePlane.Draw(particleShader, particleTexture);
	}
}


void ParticleSystem::DrawGenerateZone() {
	{
		const float Pointx1 = -10.0f;
		const float Pointx2 = 10.0f;
		const float Pointy1 = 20.0f;
		const float Pointy2 = 25.0f;
		const float Pointz1 = -10.0f;
		const float Pointz2 = 10.0f;

		glLineWidth(10.0f); 
		glColor3f(1.0f, 0.0f, 0.0f); 
		glBegin(GL_LINES);
		glVertex3f(Pointx1, Pointy1, Pointz1); 
		glVertex3f(Pointx2, Pointy1, Pointz1); 

		glVertex3f(Pointx1, Pointy1, Pointz1);
		glVertex3f(Pointx1, Pointy1, Pointz2); 

		glVertex3f(Pointx1, Pointy1, Pointz2); 
		glVertex3f(Pointx2, Pointy1, Pointz2); 

		glVertex3f(Pointx2, Pointy1, Pointz1); 
		glVertex3f(Pointx2, Pointy1, Pointz2); 


		glVertex3f(Pointx1, Pointy2, Pointz1); 
		glVertex3f(Pointx2, Pointy2, Pointz1);

		glVertex3f(Pointx1, Pointy2, Pointz1); 
		glVertex3f(Pointx1, Pointy2, Pointz2); 

		glVertex3f(Pointx1, Pointy2, Pointz2); 
		glVertex3f(Pointx2, Pointy2, Pointz2); 

		glVertex3f(Pointx2, Pointy2, Pointz1); 
		glVertex3f(Pointx2, Pointy2, Pointz2); 

		glVertex3f(Pointx1, Pointy1, Pointz1); 
		glVertex3f(Pointx1, Pointy2, Pointz1); 

		glVertex3f(Pointx2, Pointy1, Pointz1); 
		glVertex3f(Pointx2, Pointy2, Pointz1); 

		glVertex3f(Pointx1, Pointy1, Pointz2); 
		glVertex3f(Pointx1, Pointy2, Pointz2); 

		glVertex3f(Pointx2, Pointy1, Pointz2); 
		glVertex3f(Pointx2, Pointy2, Pointz2); 

		glEnd();
	}
}

