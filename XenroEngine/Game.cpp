/*************************************************************************/
/*                             Game.cpp                                  */
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

#include "Game.h"
#include "Timing.h"
#include "ErrorMessages.h"
#include "ScreenList.h"
#include "IScreen.h"
#include <iostream>
#include "InputManager.h"

namespace Xenro {

Game::Game()
{
	m_InputManager = new InputManager(this);
	m_screenList = std::make_unique<ScreenList>(this);
}


Game::~Game()
{
	delete m_InputManager;
	exitGame();
}

void Game::run() {

	if (!init()) {
		fatalError("Failed to initialize Game!");
	}

	FPSlimiter limiter;
	limiter.setTargetFPS(60.0f);

	while (m_isRunning) {
		limiter.calculateFPS();

		update();

		if(m_isRunning){

			m_InputManager->update();
			draw();
 			m_fps = limiter.limitFPS();
			m_window.swapBuffer();
		}
		
	}
}


void Game::exitGame() {

	if (m_currScreen != nullptr) {
		m_currScreen->onExit();
	}
	
	if (m_screenList != nullptr) {
		m_screenList->destroyScreen();
		m_screenList.reset();
	}

	m_isRunning = false;
}

void Game::update() {

	if (m_currScreen != nullptr) {
		switch (m_currScreen->getScreenState()) {
		case ScreenState::CHANGE_TO_NEXT:
			m_currScreen->onExit();
			m_currScreen = m_screenList->moveNext();
			if (m_currScreen != nullptr) {
				m_currScreen->setRunning();
				m_currScreen->onEntry();
			}
			break;

		case ScreenState::CHANGE_TO_PREVIOUS:
			m_currScreen->onExit();
			m_currScreen = m_screenList->movePrevious();
			if (m_currScreen != nullptr) {
				m_currScreen->setRunning();
				m_currScreen->onEntry();
			}
			break;

		case ScreenState::EXIT_APP:
			exitGame();
			break;

		case ScreenState::NONE:
			//Do nothing.
			break;

		case ScreenState::RUNNING:
			m_currScreen->update();
			break;

		case ScreenState::CHANGE_TO_PARTICULAR:
			m_currScreen->onExit();
			m_currScreen = m_screenList->moveToParticular(m_currScreen->getParticularScreenIndex());
			if (m_currScreen != nullptr) {
				m_currScreen->setRunning();
				m_currScreen->onEntry();
			}
			break;

		default:
			//Do nothing.
			break;
		}
	}
}

void Game::draw() {

	glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());

	if (m_currScreen != nullptr && m_currScreen->getScreenState() == ScreenState::RUNNING) {
		m_currScreen->draw();
	}
}

bool Game::init() {

	//initialize SDL.
	SDL_Init(SDL_INIT_EVERYTHING);

	//Tell SDL we require hardware acceleration.
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	//Tell SDL that we want a double buffered window
	//so we get no flickering.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_window.create();

	onInit();
	addScreens();

	m_currScreen = m_screenList->getCurrScreen();
	m_currScreen->onEntry();
	m_currScreen->setRunning();

	return true;
}

}