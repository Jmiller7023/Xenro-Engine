#ifndef XENRO_GAME_INCLUDED
#define XENRO_GAME_INCLUDED

#include <memory>
#include "Window.h"


namespace Xenro{

class ScreenList;
class IScreen;
class InputManager;

class Game
{
public:
	Game();
	virtual ~Game();

	//Runs the game.
	void run();

	//Exits the game.
	void exitGame();

	//Called on initialization.
	virtual void onInit() = 0;

	//Used to add screens.
	virtual void addScreens() = 0;

	//Called upon exiting.
	virtual void onExit() = 0;

	//Setters
	void modifyWindowScreenHeight(int height) { m_window.setScreenHeight(height); }
	void modifyWindowScreenWidth(int width) { m_window.setScreenWidth(width); }

	//Getters
	const float getFPS() const { return m_fps; }
 	InputManager* getInputManager() const { return m_InputManager; }
	int getScreenWidth() const { return m_window.getScreenWidth(); }
	int getScreenHeight() const { return m_window.getScreenHeight(); }
	bool isRunning() const { return m_isRunning; }

protected:

	//custom update function.
	virtual void update();

	//Custom draw function.
	virtual void draw();

	bool init();

	std::unique_ptr<ScreenList> m_screenList = nullptr;
	IScreen* m_currScreen = nullptr;
	bool m_isRunning = true;
	float m_fps = 0.0f;
	Window m_window;
	InputManager* m_InputManager;
};

}

#endif //XENRO_GAME_INCLUDED