#include "LightEngine.h"
#include "Errormessages.h"

namespace Xenro{

Light::Light(ColorRGBA Color, glm::vec2 Pos, glm::vec2 Size)
	:color(Color), pos(Pos), size(Size)
{
	//Empty
}

LightEngine::LightEngine()
{
	//Empty
}


LightEngine::~LightEngine()
{
	//Empty
}

void LightEngine::drawLight(Light light) {

	glm::vec4 DrawRect;

	DrawRect.x = light.pos.x - light.size.x / 2.0f;
	DrawRect.y = light.pos.y - light.size.y / 2.0f;
	DrawRect.z = light.size.x;
	DrawRect.w = light.size.y;

	//Add DrawRect to be drawn.
	m_spriteBatch.draw(DrawRect,glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, light.color);
}

void LightEngine::renderAllLights() {

	m_spriteBatch.begin();

	for (size_t i = 0; i < m_lights.size(); i++) {
		drawLight(m_lights[i]);
	}

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
}

void LightEngine::addLights(std::vector<Light>& lights) {

	for (size_t i = 0; i < m_lights.size(); i++) {
		m_lights.push_back(lights[i]);
	}
}

int LightEngine::addLight(const Light& light) {

	m_lights.push_back(light);
	return m_lights.size() - 1;
}

int LightEngine::addLight(ColorRGBA color, glm::vec2 pos, glm::vec2 size) {

	m_lights.emplace_back(color, pos, size);
	return m_lights.size() - 1;
}

void LightEngine::modifyLight(int index, ColorRGBA color, glm::vec2 pos, glm::vec2 size) {

	//Error checking.
	if (index >= (int)m_lights.size()) {
		errorMessage("index is out of array size");
	}

	//Modify all the lights.
	m_lights[index].color = color;
	m_lights[index].pos = pos;
	m_lights[index].size = size;
}

void LightEngine::modifyLightColor(int index, ColorRGBA color) {

	//Error checking.
	if (index >= (int)m_lights.size()) {
		errorMessage("index is out of array size");
	}

	m_lights[index].color = color;
}

void LightEngine::modifyLightPos(int index, glm::vec2 pos) {

	//Error checking.
	if (index >= (int)m_lights.size()) {
		errorMessage("index is out of array size");
	}

	//Modify the light's position.
	m_lights[index].pos = pos;
}

void LightEngine::modifyLightSize(int index, glm::vec2 size) {

	//Error checking.
	if (index >= (int)m_lights.size()) {
		errorMessage("index is out of array size");
	}

	m_lights[index].size = size;
}

void LightEngine::reset() {

	//Empty the vector.
	m_lights.clear();
}

}