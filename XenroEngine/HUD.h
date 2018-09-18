/*************************************************************************/
/*                               HUD.h                                   */
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

#ifndef XENRO_HUD_DEFINED
#define XENRO_HUD_DEFINED

#include "SpriteBatch.h"
#include "Vertex.h"
#include "SpriteFont.h"
#include <glm\glm.hpp>
#include <string>

namespace Xenro{

class Window;
class SpriteFont;
class Camera;
class GLSLProgram;

class HUD {
public:
	HUD(const SpriteBatch& spriteBatch, SpriteFont* spriteFont, GLSLProgram* hudProgram, Window* window, Justification justification = Justification::LEFT);
	HUD();
	~HUD();

	//Sets all the HUD variables so the HUD can be modified for screenchanges, font changes, or any
	//other game changes.
	void initHUD(const SpriteBatch& spriteBatch, SpriteFont* spriteFont, GLSLProgram* hudProgram, Window* window, Justification justification = Justification::LEFT);

	//Draws the HUD to the screen. numObjects is the value for whatever number should be rendered
	//to the right of the message. text is the message the HUD should print to the screen.
	//Cannot exceed more than 512 characters.
	void drawHUD(int numObjects, std::string text);
	//This one can exceed 512 characters.
	void drawHUD(std::string text);

	//By default text will be rendered to lower left hand corner.
	void setTextPos(float x = 0.0f, float y = 0.0f);

	//By default text will be rendered as the color white.
	void setColor(int r, int g, int b, int a = 255);

	//By default depth will be set to 0.0f.
	void setDepth(float depth = 0.0f);

	//By default scale will be set to 1.0.
	void setScale(double scale = 1.0);

	//By default justification will be left.
	void setJustification(Justification justification = Justification::LEFT);

private:
	//updates the camera for the HUD.
	void updateCamera();
	void draw(const char* msg);

	GLSLProgram* m_hudProgram;
	SpriteFont* m_spriteFont;
	SpriteBatch m_spriteBatch;
	Camera* m_camera;
	glm::vec2 m_textPos;
	glm::vec2 m_scale;
	float m_depth;
	ColorRGBA m_color;
	Justification m_justification;
	Window* m_window;
};

}

#endif //XENRO_HUD_DEFINED