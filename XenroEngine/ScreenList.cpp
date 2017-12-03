/*************************************************************************/
/*                          ScreenList.cpp                               */
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

#include "ScreenList.h"
#include "IScreen.h"
#include "Errormessages.h"

namespace Xenro{

ScreenList::ScreenList(Game* game) 
	:m_game(game)
{
	//Empty
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

	warning("Passed in invalid screen index");
}

IScreen* ScreenList::getCurrScreen() {

	if(m_currScreenIndex != NO_CURRENT_SCREEN_INDEX){
		return m_screens[m_currScreenIndex];
	}

	fatalError("Forgot to initialize screen!");
	return nullptr;
}

}