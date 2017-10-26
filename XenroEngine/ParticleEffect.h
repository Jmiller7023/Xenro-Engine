#ifndef XENRO_PARTICLEEFFECT_DEFINED
#define XENRO_PARTICLEEFFECT_DEFINED

#include <vector>

namespace Xenro {

class ParticleBatch;
class SpriteBatch;

class ParticleEffect
{
public:
	ParticleEffect();
	~ParticleEffect();

	//After adding a particle batch, the particle Effect
	//class will be responsible for cleaning it up.
	void addParticleBatch(ParticleBatch* particleBatch);

	void update(float deltaTime);

	void draw(SpriteBatch* spriteBatch);

private:
	std::vector<ParticleBatch*> m_batches;
};

}

#endif //XENRO_PARTICLEEFFECT_DEFINED