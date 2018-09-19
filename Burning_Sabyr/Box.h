#ifndef BOX_DEFINED
#define BOX_DEFINED

#include <glm/glm.hpp>
#include <Box2D/Box2D.h>
#include <XenroEngine/Color.h>

class Box
{
public:
	Box();
	~Box();

	void init(b2World * world, glm::vec2 position, glm::vec2 dimensions, Xenro::ColorRGBA color);

	b2Body* getBody() const { return m_body; }
	b2Fixture* getFixture() const { return m_fixture; }
	const glm::vec2& getDimensions() const { return m_dimensions; }
	Xenro::ColorRGBA getColor() const { return m_color; }
	
private:
	Xenro::ColorRGBA m_color;
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dimensions;
};

#endif //BOX_DEFINED