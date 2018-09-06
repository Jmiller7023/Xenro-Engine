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
#include <SDL/SDL.h>


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

	init();

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
	//SDL is no longer needed.
	SDL_Quit();
}


void Game::exitGame() {

	//Call current screens exit function.
	if (m_currScreen != nullptr) {
		m_currScreen->onExit();
	}
	
	//Destroy screens in screenlist.
	if (m_screenList != nullptr) {
		m_screenList->destroyScreen();
		m_screenList.reset();
	}

	//Close game controller.
	RemoveGameController();

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
		//Set base depth to 1.0
		glClearDepth(1.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_currScreen->draw();
	}
}

void Game::RemoveGameController() {

	//Close game controller.
	if (m_gameController != nullptr) {
		printf("%s: disconnected.\n", SDL_GameControllerName(m_gameController));
		SDL_GameControllerClose(m_gameController);
		m_gameController = nullptr;
	}

	//Close Haptic device
	if (m_controllerHaptic != nullptr) {
		SDL_HapticClose(m_controllerHaptic);
		m_controllerHaptic = nullptr;
	}
}

void Game::addGameController() {
	//Check for controllers
	if (SDL_NumJoysticks() > 0) {
		//Load controller
		m_gameController = SDL_GameControllerOpen(0);
		printf("%s: connected.\n", SDL_GameControllerName(m_gameController));
		if (m_gameController == nullptr) {
			warning("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
		}
		else {
			SDL_GameControllerAddMappingsFromFile("Controllers/gamecontrollerdb_205.txt");
			m_controllerHaptic = SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(m_gameController));
			if (m_controllerHaptic == nullptr) {
				warning("Warning: Controller does not support haptics! SDL Error: %s\n", SDL_GetError());
			}
			else {
				//Initialize the rumble.
				if (SDL_HapticRumbleInit(m_controllerHaptic) < 0) {
					warning("Warning: Unable to initialize the rumble! SDL Error: %s\n", SDL_GetError());
				}
			}
		}

	}
	else {
		warning("Warning: No controller connected!\n");
	}
}

void Game::init() {

	//initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fatalError("Failed to initialize SDL!\nSDL Error: %s\n", SDL_GetError());
	}
	
	//Set texture filtering to linear
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
		warning("Warning: Linear texture filtering not enabled!"); 
	}

	//Tell SDL we require hardware acceleration.
	if (SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) < 0) {
		fatalError("Failed to set attribute: SDL_GL_ACCELERATED_VISUAL!\nSDL Error: %s\n", SDL_GetError());
	}

	//Tell SDL that we want a double buffered window
	if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0) {
		fatalError("Failed to set attribute: SDL_GL_DOUBLEBUFFER!\nSDL Error: %s\n", SDL_GetError());
	}

	m_window.create();

	onInit();
	addScreens();

	m_currScreen = m_screenList->getCurrScreen();
	m_currScreen->onEntry();
	m_currScreen->setRunning();
}

void Game::rumbleController(double strength, int milliseconds) {
	
	if (strength > 1.0 || strength < 0.0) {
		warning("Rumble strength ranges from 0.0 to 1.0!");
		return;
	}

	if (SDL_HapticRumblePlay(m_controllerHaptic, (float)strength, milliseconds) != 0) { 
		warning("Warning: Unable to play rumble! %s\n", SDL_GetError()); 
	}
}

void Game::stopGame() {
	m_isRunning = false;
}

}