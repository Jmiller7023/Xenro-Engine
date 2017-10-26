#ifndef XENRO_ISCREEN_DEFINED
#define XENRO_ISCREEN_DEFINED

namespace Xenro{

enum class ScreenState {
		NONE, RUNNING, EXIT_APP, CHANGE_TO_NEXT, CHANGE_TO_PREVIOUS, CHANGE_TO_PARTICULAR
};

class Game;

class IScreen
{
public:
	friend class ScreenList;
	IScreen() { }
	virtual ~IScreen() { }

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
	int getParticularScreenIndex() const { return m_changeToParticular; }
	
	//Setters.
	void setGame(Game* game) { m_game = game; }
	void setRunning() { m_currState = ScreenState::RUNNING; }
	void setParticularScreenIndex(int index) { m_changeToParticular = index; }

protected:
	ScreenState m_currState = ScreenState::NONE;
	Game* m_game = nullptr;
	int m_screenIndex = -1;
	int m_changeToParticular = -1;
};


}

#endif //XENRO_ISCREEN_DEFINED