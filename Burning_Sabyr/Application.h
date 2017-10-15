#pragma once
#include <XenroEngine\Game.h>

class GameplayScreen;

class Application :public Xenro::Game
{
public:
	Application();
	~Application();

	//Called on initialization.
	virtual void onInit() override;

	//Used to add screens.
	virtual void addScreens() override;

	//Called upon exiting.
	virtual void onExit() override;

private:
	GameplayScreen* m_gameplayScreen;
};

