#ifndef XENRO_HUD_DEFINED
#define XENRO_HUD_DEFINED

#include "SpriteBatch.h"
#include "Vertex.h"
#include <glm\glm.hpp>
#include <string>

namespace Xenro{

class SpriteFont;
class Camera;
class GLSLProgram;

class HUD
{
public:
	HUD(const SpriteBatch& spriteBatch, SpriteFont* spriteFont, GLSLProgram* hudProgram, int screenWidth, int screenHeight);
	HUD();
	~HUD();

	//Sets all the HUD variables so the HUD can be modified for screenchanges, font changes, or any
	//other game changes.
	void initHUD(const SpriteBatch& spriteBatch, SpriteFont* spriteFont, GLSLProgram* hudProgram, int screenWidth, int screenHeight);

	//Draws the HUD to the screen. numObjects is the value for whatever number should be rendered
	//to the right of the message. text is the message the HUD should print to the screen.
	//Cannot exceed more than 512 characters.
	void drawHUD(int numObjects, std::string text);
	//This one can exceed 512 characters.
	void drawHUD(std::string text);

	//By default text will be rendered to lower left hand corner.
	void setTextPos(int x = 0, int y = 0);

	//By default text will be rendered as the color white.
	void setColor(int r, int g, int b, int a = 255);

	//By default depth will be set to 0.0f.
	void setDepth(float depth = 0.0f);

	//By default scale will be set to 1.0.
	void setScale(double scale = 1.0);

private:
	//updates the camera for the HUD.
	void updateCamera();

	GLSLProgram* m_hudProgram;
	SpriteFont* m_spriteFont;
	SpriteBatch m_spriteBatch;
	Camera* m_camera;
	glm::vec2 m_textPos;
	glm::vec2 m_scale;
	float m_depth;
	ColorRGBA m_color;
};

}

#endif //XENRO_HUD_DEFINED