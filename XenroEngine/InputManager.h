/*************************************************************************/
/*                           InputManager.h                              */
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

#ifndef XENRO_INPUTMANAGER_DEFINED
#define XENRO_INPUTMANAGER_DEFINED

#include <unordered_map>
#include<glm\glm.hpp>
#include <SDL/SDL_events.h>

namespace Xenro{

class Game;

enum Axis {
	LEFTANALOGUP = 255, LEFTANALOGUPRIGHT, LEFTANALOGRIGHT, LEFTANALOGDOWNRIGHT,
	LEFTANALOGDOWN, LEFTANALOGDOWNLEFT, LEFTANALOGLEFT, LEFTANALOGUPLEFT,
	RIGHTANALOGUP, RIGHTANALOGUPRIGHT, RIGHTANALOGRIGHT, RIGHTANALOGDOWNRIGHT,
	RIGHTANALOGDOWN, RIGHTANALOGDOWNLEFT, RIGHTANALOGLEFT, RIGHTANALOGUPLEFT,
};

class InputManager {

public:
	InputManager(Game* game);
	InputManager();
	~InputManager();

	void update();

	void processInput(SDL_Event& evnt);

	//Checks if the key is currently down.
	bool isDown(unsigned int keyID);

	//Checks if the key was pressed once in the last frame.
	bool isPressed(unsigned int keyID);

	//Setters.
	void setMouseCoords(float x, float y) { m_mousecoords.x = x; m_mousecoords.y = y; }		
	void keyPress(unsigned int keyID) { m_keymap[keyID] = true; }
	void keyRelease(unsigned int keyID) { m_keymap[keyID] = false; }

	//Getters.
	glm::vec2 getMouseCoords() const { return m_mousecoords; }
	float getLeftAnalogAngle() const { return m_leftAnalogAngle; }
	float getRightAnalogAngle() const { return m_rightAnalogAngle; }

private:

	//Member functions.
	void processJoyAxis(SDL_Event& evnt);
	void angleToKeyRightAnalog();
	void angleToKeyLeftAnalog();
	bool wasPressed(unsigned int keyID);

	//Member variables.
	Game* m_game = nullptr;
	std::unordered_map<unsigned int, bool> m_keymap;
	std::unordered_map<unsigned int, bool> m_prevmap;
	glm::vec2 m_mousecoords;
	int m_leftxDir = 0, m_leftyDir = 0, m_rightxDir = 0, m_rightyDir = 0;
	int m_rightAnalogAngle = -1, m_leftAnalogAngle = -1;
	Axis m_lastLeftAnalogKey = Axis::LEFTANALOGUP, m_lastRightAnalogKey = Axis::RIGHTANALOGUP;
};

}

#endif //XENRO_INPUTMANAGER_DEFINED