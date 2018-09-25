/*************************************************************************/
/*                          LightEngine.cpp                              */
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

void LightEngine::renderLight(const Light & light)
{
	//Use Additive blending for the lights.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	m_spriteBatch.begin();

	glm::vec4 DrawRect;

	DrawRect.x = light.pos.x - light.size.x / 2.0f;
	DrawRect.y = light.pos.y - light.size.y / 2.0f;
	DrawRect.z = light.size.x;
	DrawRect.w = light.size.y;

	//Add DrawRect to be drawn.
	m_spriteBatch.draw(DrawRect, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, light.color);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	//Reset to regular alpha blending.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	//Use Additive blending for the lights.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	m_spriteBatch.begin();

	for (size_t i = 0; i < m_lights.size(); i++) {
		drawLight(m_lights[i]);
	}

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	//Reset to regular alpha blending.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		warning("index is out of array size");
	}

	//Modify all the lights.
	m_lights[index].color = color;
	m_lights[index].pos = pos;
	m_lights[index].size = size;
}

void LightEngine::modifyLightColor(int index, ColorRGBA color) {

	//Error checking.
	if (index >= (int)m_lights.size()) {
		warning("index is out of array size");
	}

	m_lights[index].color = color;
}

void LightEngine::modifyLightPos(int index, glm::vec2 pos) {

	//Error checking.
	if (index >= (int)m_lights.size()) {
		warning("index is out of array size");
	}

	//Modify the light's position.
	m_lights[index].pos = pos;
}

void LightEngine::modifyLightSize(int index, glm::vec2 size) {

	//Error checking.
	if (index >= (int)m_lights.size()) {
		warning("index is out of array size");
	}

	m_lights[index].size = size;
}

void LightEngine::reset() {

	//Empty the vector.
	m_lights.clear();
}

}