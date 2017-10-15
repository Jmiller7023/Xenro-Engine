#pragma once
#include <unordered_map>
#include<glm\glm.hpp>

namespace Xenro{

class Game;
class InputManager
{
public:
	InputManager(Game* game);
	InputManager();
	~InputManager();

	void update();

	void processInput();

	//Checks if the key is currently down.
	bool isDown(unsigned int keyID);

	//Checks if the key was pressed once in the last frame.
	bool isPressed(unsigned int keyID);

	///Setters.
	void setMouseCoords(float x, float y) { m_mousecoords.x = x; m_mousecoords.y = y; }	
	
	void keyPress(unsigned int keyID) { m_keymap[keyID] = true; }

	void keyRelease(unsigned int keyID) { m_keymap[keyID] = false; }

	///Getters.
	glm::vec2 getMouseCoords() const { return m_mousecoords; }

private:
	bool wasPressed(unsigned int keyID);
	Game* m_game = nullptr;
	std::unordered_map<unsigned int, bool> m_keymap;
	std::unordered_map<unsigned int, bool> m_prevmap;
	glm::vec2 m_mousecoords;
};

}