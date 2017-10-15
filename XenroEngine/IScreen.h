#pragma once
namespace Xenro{

enum class ScreenState {
		NONE, RUNNING, EXIT_APP, CHANGE_TO_NEXT, CHANGE_TO_PREVIOUS
};

class Game;

class IScreen
{
public:
	friend class ScreenList;
	IScreen()  { /*empty*/ }
	virtual ~IScreen() { /*empty*/ }

	//Return index
	virtual int getNextScreenIndex() const = 0;
	virtual int getPrevScrenIndex() const = 0;

	//Called at beginning and end of application.
	virtual void create() = 0;
	virtual void destroy() = 0;

	//Called whenever a screen closes or opens.
	virtual void onEntry() = 0;
	virtual void onExit() = 0;

	//Called in Gameloop.
	virtual void update() = 0;
	virtual void draw() = 0;

	//Getters.
	int getScreenIndex() const { return m_screenIndex; }
	ScreenState getScreenState() const { return m_currState; }
	
	//Setters.
	void setGame(Game* game) { m_game = game; }
	void setRunning() { m_currState = ScreenState::RUNNING; }

protected:
	ScreenState m_currState = ScreenState::NONE;
	Game* m_game = nullptr;
	int m_screenIndex = -1;
};


}