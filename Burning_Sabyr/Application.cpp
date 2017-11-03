#include "Application.h"
#include <XenroEngine\ScreenList.h>
#include "GamePlayScreen.h"
#include "MainMenuScreen.h"
#include "OptionsScreen.h"

Application::Application()
{
	//Empty
}


Application::~Application()
{
	m_currScreen = nullptr;
	//don't delete m_GameplayScreen since the destructor for ScreenList deletes it.
}

void Application::onInit() {

	//Nothing for now.
}

void Application::addScreens() {

	m_mainMenuScreen = new MainMenuScreen(&m_window);
	m_gameplayScreen = new GameplayScreen(&m_window);
	m_optionsScreen = new OptionsScreen(&m_window);

	m_screenList->addScreen(m_mainMenuScreen);
	m_screenList->addScreen(m_gameplayScreen);
	m_screenList->addScreen(m_optionsScreen);
	m_screenList->setScreen(m_mainMenuScreen->getScreenIndex());
}

void Application::onExit() {
	return;
}