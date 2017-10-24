#include "ScreenList.h"
#include "IScreen.h"
#include "Errormessages.h"

namespace Xenro{

ScreenList::ScreenList(Game* game) 
	:m_game(game)
{
	
}

ScreenList::~ScreenList() {
	destroyScreen();
}

IScreen* ScreenList::moveNext() {

	IScreen* currScreen = getCurrScreen();
	if (currScreen->getScreenIndex() != NO_CURRENT_SCREEN_INDEX) {
		m_currScreenIndex = currScreen->getNextScreenIndex();
		return getCurrScreen();
	}
	//There was no next screen so return nullptr.
	return nullptr;
}

IScreen* ScreenList::movePrevious() {

	IScreen* currScreen = getCurrScreen();
	if (currScreen->getScreenIndex() != NO_CURRENT_SCREEN_INDEX) {
		m_currScreenIndex = currScreen->getPrevScrenIndex();
		return getCurrScreen();
	}

	//There was no next screen so return nullptr.
	return nullptr;
}

IScreen* ScreenList::moveToParticular(int nextScreen) {

	IScreen* currScreen = getCurrScreen();
	if (currScreen->getScreenIndex() != NO_CURRENT_SCREEN_INDEX) {
		m_currScreenIndex = nextScreen;
		return getCurrScreen();
	}

	//There was no next screen so return nullptr.
	return nullptr;

}

void ScreenList::setScreen(int nextScreen) {
	m_currScreenIndex = nextScreen;
}

void ScreenList::addScreen(IScreen* newScreen) {

	//Error checking
	if (newScreen == nullptr) {
		fatalError("Forgot to initialize screen!");
	}

	newScreen->m_screenIndex = m_screens.size();
	m_screens.push_back(newScreen);
	newScreen->create();
	newScreen->setGame(m_game);
}

void ScreenList::destroyScreen() {
	for (size_t i = 0; i < m_screens.size(); i++) {
		m_screens[i]->destroy();
		delete m_screens[i];
		m_screens[i] = nullptr;
	}
	m_screens.resize(0);
	m_currScreenIndex = NO_CURRENT_SCREEN_INDEX;
}

void ScreenList::destroyScreen(int screen) {
	if (screen > 0 && (size_t)screen < m_screens.size()) {
		std::swap(m_screens[screen], m_screens.back());
		delete m_screens[(int)m_screens.size() - 1];
		m_screens.pop_back();
	}

	errorMessage("Passed in invalid screen index");
}

IScreen* ScreenList::getCurrScreen() {

	if(m_currScreenIndex != NO_CURRENT_SCREEN_INDEX){
		return m_screens[m_currScreenIndex];
	}

	fatalError("Forgot to initialize screen!");
	return nullptr;
}

}