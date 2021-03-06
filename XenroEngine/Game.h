/*************************************************************************/
/*                              Game.h                                   */
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

#ifndef XENRO_GAME_INCLUDED
#define XENRO_GAME_INCLUDED

#include <memory>
#include <SDL/SDL_gamecontroller.h>
#include <SDL/SDL_haptic.h>
#include "Window.h"

namespace Xenro{

class ScreenList;
class IScreen;
class InputManager;

class Game {
public:
	Game();
	virtual ~Game();

	//Runs the game.
	void run();

	//Stops the game.
	void stopGame();

	//Called on initialization.
	virtual void onInit() = 0;

	//Used to add screens.
	virtual void addScreens() = 0;

	//Called upon exiting.
	virtual void onExit() = 0;

	//Causes Game Controller to rumble.
	//Strength ranges from 0.0 to 1.0.
	void rumbleController(double strength, int milliseconds);

	//Setters
	void modifyWindowScreenHeight(int height) { m_window.setScreenHeight(height); }
	void modifyWindowScreenWidth(int width) { m_window.setScreenWidth(width); }
	void RemoveGameController();
	void addGameController();

	//Getters
	const float getFPS() const { return m_fps; }
 	InputManager* getInputManager() const { return m_InputManager; }
	int getScreenWidth() const { return m_window.getScreenWidth(); }
	int getScreenHeight() const { return m_window.getScreenHeight(); }
	bool isRunning() const { return m_isRunning; }
	SDL_GameController* getGameController() const { return m_gameController; }
	bool isControllerConnected() const { return m_gameController != nullptr; }

protected:

	//custom update function.
	virtual void update();

	//Custom draw function.
	virtual void draw();

	void init();

	std::unique_ptr<ScreenList> m_screenList = nullptr;
	IScreen* m_currScreen = nullptr;
	bool m_isRunning = true;
	float m_fps = 0.0f;
	Window m_window;
	InputManager* m_InputManager;

	//Game Controller handler
	SDL_GameController* m_gameController = nullptr;
	SDL_Haptic* m_controllerHaptic = nullptr;
private:
	//Exits the game.
	void exitGame();
};

}

#endif //XENRO_GAME_INCLUDED