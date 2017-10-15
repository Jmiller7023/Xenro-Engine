#include "ParticleBatch.h"
#include "Particle.h"
#include "SpriteBatch.h"

namespace Xenro {

	void updateBloodParticles(Xenro::Particle& particle, float deltaTime) {
		particle.position += particle.velocity * deltaTime;
		particle.color.a = (GLubyte)(particle.lifeTime * 255.0f);
	}

	ParticleBatch::ParticleBatch(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle&, float)> updateFunc)
		:m_decayRate(decayRate), m_maxParticles(maxParticles), m_lastFreeParticle(0), m_particles(nullptr), m_texture(texture), m_updateFunc(updateFunc)
	{
		m_particles = new Particle[maxParticles];
	}


	ParticleBatch::~ParticleBatch()
	{
		delete[] m_particles;
	}

	void ParticleBatch::addParticle(const glm::vec2 & position, const ColorRGBA & color, const glm::vec2 & velocity, float width)
	{
		int i = findFreeParticle();

		m_particles[i].lifeTime = 1.0f;
		m_particles[i].position = position;
		m_particles[i].color = color;
		m_particles[i].velocity = velocity;
		m_particles[i].width = width;
	}

	void ParticleBatch::update(float deltaTime) {
		for (int i = 0; i < m_maxParticles; i++) {
			//check if it's active.
			if (m_particles[i].lifeTime > 0.0f) {
				m_updateFunc(m_particles[i], deltaTime);
				m_particles[i].lifeTime -= m_decayRate;
			}
		}
	}

	void ParticleBatch::draw(SpriteBatch* spriteBatch) {
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < m_maxParticles; i++) {
			//check if it's active.
			if (m_particles[i].lifeTime > 0.0f) {
				glm::vec4 destRect(m_particles[i].position.x, m_particles[i].position.y, m_particles[i].width, m_particles[i].width);
				spriteBatch->draw(destRect, uvRect, m_texture.ID, 0.0f, m_particles[i].color);
			}
		}
	}

	int ParticleBatch::findFreeParticle() {

		for( int i = m_lastFreeParticle; i < m_maxParticles; i++){
			if (m_particles[i].lifeTime <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}

		for (int i = 0; i < m_lastFreeParticle; i++) {
			if (m_particles[i].lifeTime <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}

		//No particles are free so overwrite first one.
		return 0;
}

}