/*************************************************************************/
/*                          InputManager.cpp                             */
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

#include "InputManager.h"
#include "Game.h"
#include<SDL\SDL.h>

namespace Xenro{

InputManager::InputManager()
	:m_mousecoords(0)
{
	//Empty
}
InputManager::InputManager(Game* game)
	:m_mousecoords(0), m_game(game)
{
	//Empty
}


InputManager::~InputManager()
{
	//Empty
}

void InputManager::processInput(SDL_Event& evnt) {

		switch (evnt.type) {
		case SDL_QUIT:
			m_game->exitGame();
			break;
		case SDL_KEYDOWN:
			keyPress(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			keyRelease(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			keyPress(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			keyRelease(evnt.button.button);
			break;
		case SDL_MOUSEMOTION:
			setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_WINDOWEVENT:
			switch (evnt.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
				m_game->modifyWindowScreenWidth(evnt.window.data1);
				m_game->modifyWindowScreenHeight(evnt.window.data2);
				break;
			}
			
		}
}


void InputManager::update() {

	//iterate throught the keymap and set it to prevmap.
	for (auto it = m_keymap.begin(); it != m_keymap.end(); it++) {
		m_prevmap[it->first] = it->second;
	}
}

bool InputManager::isDown(unsigned int keyID) {
	//check if the key is in the map
	auto it = m_keymap.find(keyID);
	if (it != m_keymap.end()) {
		return it->second;
	}
	else {
		//wasn't in the map.
		return false;
	}
}

bool InputManager::isPressed(unsigned int keyID) {
	return (wasPressed(keyID) == false && isDown(keyID) == true);
}

bool InputManager::wasPressed(unsigned int keyID) {
	//check if the key is in the map
	auto it = m_prevmap.find(keyID);
	if (it != m_prevmap.end()) {
		return it->second;
	}
	else {
		//wasn't in the map.
		return false;
	}
}


}