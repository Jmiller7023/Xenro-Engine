#include "Game.h"
#include "Timing.h"
#include "ErrorMessages.h"
#include "ScreenList.h"
#include "IScreen.h"
#include <iostream>
#include "InputManager.h"

namespace Xenro{

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