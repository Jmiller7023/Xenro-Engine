#pragma once
#include <glm\glm.hpp>
#include "vertex.h"
#include "SpriteBatch.h"
#include <vector>

namespace Xenro{

struct Light {
	Light(ColorRGBA Color, glm::vec2 Pos, glm::vec2 Size);
	ColorRGBA color;
	glm::vec2 pos;
	glm::vec2 size;
};

class LightEngine
{
public:
	LightEngine();
	~LightEngine();

	//Draws the light to the spritebatch.
	void drawLight(Light light);

	//Render the lights to the screen.
	void renderAllLights();
	void addLights(std::vector<Light>& lights);

	//Returns the index of the light object.
	int addLight(const Light& light);
	int addLight(ColorRGBA color, glm::vec2 pos, glm::vec2 size);

	//Modify aspects of a particular light index.
	void modifyLight(int index, ColorRGBA color, glm::vec2 pos, glm::vec2 size);
	void modifyLightColor(int index, ColorRGBA color);
	void modifyLightPos(int index, glm::vec2 pos);
	void modifyLightSize(int index, glm::vec2 size);

	//Clear all lights.
	void reset();


private:
	std::vector<Light> m_lights;
	SpriteBatch m_spriteBatch;
};

}