#include "InputManager.h"
#include "Game.h"
#include<SDL\SDL.h>

namespace Xenro{


InputManager::InputManager()//Xenro::Game& game)
	:m_mousecoords(0)//, m_game(&game)
{
}
InputManager::InputManager(Game* game)
	:m_mousecoords(0), m_game(game)
{

}


InputManager::~InputManager()
{
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