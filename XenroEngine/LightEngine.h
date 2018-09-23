/*************************************************************************/
/*                           LightEngine.h                               */
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

#ifndef XENRO_LIGHTENGINE_DEFINED
#define XENRO_LIGHTENGINE_DEFINED

#include <glm\glm.hpp>
#include "vertex.h"
#include "SpriteBatch.h"
#include <vector>

namespace Xenro{

struct Light {
	Light() {}
	Light(ColorRGBA Color, glm::vec2 Pos, glm::vec2 Size);
	ColorRGBA color = ColorRGBA(255,255,255,255);
	glm::vec2 pos = glm::vec2(0,0);
	glm::vec2 size = glm::vec2(5.0f);
};

class LightEngine {
public:
	LightEngine();
	~LightEngine();

	//Renders a given light.
	void renderLight(const Light& light);

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

	//Draws the light to the spritebatch.
	void drawLight(Light light);

	std::vector<Light> m_lights;
	SpriteBatch m_spriteBatch;
};

}

#endif //XENRO_LIGHTENGINE_DEFINED