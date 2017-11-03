/*************************************************************************/
/*                         ParticleEffect.cpp                            */
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

#include "ParticleEffect.h"
#include "SpriteBatch.h"
#include "ParticleBatch.h"

namespace Xenro{

ParticleEffect::ParticleEffect()
{
	//Empty
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