#include "ParticleEffect.h"
#include "SpriteBatch.h"
#include "ParticleBatch.h"
namespace Xenro{

ParticleEffect::ParticleEffect()
{
}

ParticleEffect::~ParticleEffect()
{
	for (size_t i = 0; i < m_batches.size(); i++) {
		delete m_batches[i];
	}
}

void ParticleEffect::addParticleBatch(ParticleBatch* particleBatch) {
	m_batches.push_back(particleBatch);
}

void ParticleEffect::update(float deltaTime) {
	for (size_t i = 0; i < m_batches.size(); i++) {
		m_batches[i]->update(deltaTime);
	}
}

void ParticleEffect::draw(SpriteBatch* spriteBatch) {
	for (size_t i = 0; i < m_batches.size(); i++) {
		spriteBatch->begin();

		m_batches[i]->draw(spriteBatch);

		spriteBatch->end();
		spriteBatch->renderBatch();
	}

}

}