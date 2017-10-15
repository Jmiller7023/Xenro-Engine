#include "Application.h"
#include <XenroEngine\ScreenList.h>
#include "GamePlayScreen.h"

Application::Application()

{
}


Application::~Application()
{
	m_currScreen = nullptr;
	//don't delete m_GameplayScreen since the desructor for ScreenList deletes it.
}

void Application::onInit() {

	return;
}

void Application::addScreens() {
	m_gameplayScreen = new GameplayScreen(&m_window);

	m_screenList->addScreen(m_gameplayScreen);
	m_screenList->setScreen(m_gameplayScreen->getScreenIndex());
}

void Application::onExit() {
	return;
}