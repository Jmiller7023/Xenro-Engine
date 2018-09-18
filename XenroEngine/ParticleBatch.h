/*************************************************************************/
/*                          ParticleBatch.h                              */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Xenro Engine                                */
/*             https://github.com/Jmiller7023/Xenro-Engine               */
/*************************************************************************/
/*                 Copyright  11-3-2017 Joseph Miller.                   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef XENRO_PARTICLEBATCH_DEFINED
#define XENRO_PARTICLEBATCH_DEFINED

#include <functional>
#include <glm\glm.hpp>
#include "Vertex.h"
#include "GLTexture.h"
#include "Particle.h"

namespace Xenro{

class SpriteBatch;

class ParticleBatch {
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

#endif //XENRO_PARTICLEBATCH_DEFINED