/*************************************************************************/
/*                            LevelEditor.h                              */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Xenro Engine                                */
/*             https://github.com/Jmiller7023/Xenro-Engine               */
/*************************************************************************/
/*                 Copyright  9-20-2018 Joseph Miller.                   */
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


#ifndef XENRO_TILE_DEFINED
#define XENRO_TILE_DEFINED

#include "GLTexture.h"
#include "SpriteBatch.h"
#include <glm/glm.hpp>
#include <Box2D/Box2D.h>
#include "Color.h"

namespace Xenro {

class Tile {
public:
	Tile();
	Tile(b2World* world,
		const glm::vec2& position,
		const glm::vec2& dimensions,
		GLTexture texture,
		ColorRGBA color,
		bool fixedRotation,
		bool isDynamic,
		float angle = 0.0f,
		glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	~Tile();

	void init(b2World* world,
		const glm::vec2& position,
		const glm::vec2& dimensions,
		GLTexture texture,
		ColorRGBA color,
		bool fixedRotation,
		bool isDynamic,
		float angle = 0.0f,
		glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	void destroy(b2World* world);

	void draw(SpriteBatch& spriteBatch);

	bool isDynamic() const { return m_body->GetType() == b2_dynamicBody; }

	// Test if a point is inside the box
	bool testPoint(float x, float y) const { return m_fixture->TestPoint(b2Vec2(x, y)); }
	
	// Beautiful accessors
	b2Body*                    getBody()          const { return m_body; }
	b2Fixture*                 getFixture()       const { return m_fixture; }
	const glm::vec2&           getDimensions()    const { return m_dimensions; }
	glm::vec2                  getPosition()      const { return glm::vec2(m_body->GetPosition().x, m_body->GetPosition().y); }
	glm::vec4                  getUvRect()        const { return m_uvRect; }
	ColorRGBA                  getColor()         const { return m_color; }
	float                      getAngle()         const { return m_body->GetAngle(); }
	const GLTexture&           getTexture()       const { return m_texture; }
	const bool&                getFixedRotation() const { return m_fixedRotation; }
	const bool&                getIsDynamic()     const { return m_isDynamic; }

private:
	glm::vec4 m_uvRect;
	GLTexture m_texture;
	Xenro::ColorRGBA m_color;
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dimensions;
	bool m_fixedRotation;
	bool m_isDynamic;
	b2World* m_world = nullptr;
};

}
#endif