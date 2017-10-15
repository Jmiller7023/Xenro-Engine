#include "HUD.h"
#include "SpriteFont.h"
#include "Camera.h"
#include "GLSLProgram.h"
#include <SDL\SDL.h>
#include <glm\glm.hpp>

namespace Xenro{
HUD::HUD(const SpriteBatch& spriteBatch, SpriteFont* spriteFont, GLSLProgram* hudProgram, int screenWidth, int screenHeight)
{
	initHUD(spriteBatch, spriteFont, hudProgram, screenWidth, screenHeight);
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
}

HUD::~HUD()
{
	if (m_camera != nullptr) {
		delete m_camera;
	}
}

void HUD::initHUD(const SpriteBatch& spriteBatch, SpriteFont* spriteFont, GLSLProgram* hudProgram, int screenWidth, int screenHeight) {
	m_spriteBatch = spriteBatch;
	m_spriteFont = spriteFont;
	m_camera = new Camera(screenWidth, screenHeight);
	m_camera->setPosition(glm::vec2(screenWidth / 2, screenHeight / 2));
	m_hudProgram = hudProgram;
}

void HUD::drawHUD(int numObjects, std::string text) {
	updateCamera();

	char buffer[512];

	m_spriteBatch.begin();

	text += "%d";
	sprintf_s(buffer, text.c_str(), numObjects);

	m_spriteFont->draw(m_spriteBatch, buffer, m_textPos, m_scale, m_depth,
		m_color);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
}

void HUD::drawHUD(std::string text) {
	updateCamera();

	m_spriteBatch.begin();

	m_spriteFont->draw(m_spriteBatch, text.c_str(), m_textPos, m_scale, m_depth,
		m_color);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
}

void HUD::setTextPos(int x, int y) {
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

void HUD::updateCamera() {
	m_camera->update();

	//Set the camera matrix.
	GLint pLocation = m_hudProgram->getUniformLocation("P");
	glm::mat4 cameraMatrix = m_camera->getcamMatrix();

	//Pass pointer to openGL
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
}

}
