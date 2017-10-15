#pragma once
#include <functional>
#include <glm\glm.hpp>
#include "Vertex.h"
#include "GLTexture.h"
#include "Particle.h"

namespace Xenro{
class SpriteBatch;

class ParticleBatch
{
public:

	ParticleBatch(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle&, float)> updateFunc = defaultPArticleupdate);
	~ParticleBatch();

	void addParticle(const glm::vec2 & position, const ColorRGBA & color, const glm::vec2 & velocity, float width);

	void update(float deltaTime);

	void draw(SpriteBatch* spriteBatch);

private:
	int findFreeParticle();

	std::function<void(Particle&, float)> m_updateFunc;
	float m_decayRate;
	Particle* m_particles;
	int m_maxParticles;
	int m_lastFreeParticle;
	GLTexture m_texture;
};

}