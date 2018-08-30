/*************************************************************************/
/*                             HUD.cpp                                   */
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

#include "HUD.h"
#include "SpriteFont.h"
#include "Camera.h"
#include "Window.h"
#include "GLSLProgram.h"
#include <SDL\SDL.h>
#include <glm\glm.hpp>

namespace Xenro {

HUD::HUD(const SpriteBatch& spriteBatch, SpriteFont* spriteFont, GLSLProgram* hudProgram, Window* window, Justification justification)
{
	initHUD(spriteBatch, spriteFont, hudProgram, window, justification);
	m_textPos = glm::vec2(0, 0);
	m_color.r = 255;
	m_color.g = 255;
	m_color.b = 255;
	m_color.a = 255;
	m_depth = 0.0f;
	m_scale = glm::vec2(1.0);
}

HUD::HUD()
	:m_spriteFont(nullptr), m_camera(nullptr)
{
	m_textPos = glm::vec2(0, 0);
	m_color.r = 255;
	m_color.g = 255;
	m_color.b = 255;
	m_color.a = 255;
	m_depth = 0.0f;
	m_scale = glm::vec2(1.0);
	m_justification = Justification::LEFT;
}

HUD::~HUD()
{
	if (m_camera != nullptr) {
		delete m_camera;
	}
}

void HUD::initHUD(const SpriteBatch& spriteBatch, SpriteFont* spriteFont, GLSLProgram* hudProgram, Window* window, Justification justification) {
	m_spriteBatch = spriteBatch;
	m_spriteFont = spriteFont;
	m_camera = new Camera(window);
	m_camera->setPosition(glm::vec2(window->getScreenWidth() / 2, window->getScreenHeight() / 2));
	m_hudProgram = hudProgram;
	m_justification = justification;
}

void HUD::drawHUD(int numObjects, std::string text) {
	updateCamera();

	char buffer[512];

	m_spriteBatch.begin();

	text += "%d";
	sprintf_s(buffer, text.c_str(), numObjects);

	m_spriteFont->draw(m_spriteBatch, buffer, m_textPos, m_scale, m_depth,
		m_color, m_justification);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
}

void HUD::drawHUD(std::string text) {
	updateCamera();

	m_spriteBatch.begin();

	m_spriteFont->draw(m_spriteBatch, text.c_str(), m_textPos, m_scale, m_depth,
		m_color, m_justification);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
}

void HUD::setTextPos(float x, float y) {
	m_textPos = glm::vec2(x, y);
}
 
void HUD::setColor(int r, int g, int b, int a) {
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
	m_color.a = a;
}

void HUD::setDepth(float depth) {
	m_depth = depth;
}

void HUD::setScale(double scale) {
	m_scale = glm::vec2(scale);
}

void HUD::setJustification(Justification justification) {
	m_justification = justification;
}

void HUD::updateCamera() {
	
	m_camera->update();

	//Set the camera matrix.
	GLint pLocation = m_hudProgram->getUniformLocation("P");
	glm::mat4 cameraMatrix = m_camera->getcamMatrix();

	//Pass pointer to openGL
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
}

}
