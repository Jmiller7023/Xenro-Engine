#pragma once
#include <glm\glm.hpp>
#include "Vertex.h"

namespace Xenro{

class Particle
{
public:
	Particle();
	~Particle();

	glm::vec2 position;
	glm::vec2 velocity;
	ColorRGBA color;
	float width = 0.0f;
	float lifeTime;

};

inline void defaultPArticleupdate(Particle& particle, float deltaTime) {
	particle.position += particle.velocity * deltaTime;
}

}