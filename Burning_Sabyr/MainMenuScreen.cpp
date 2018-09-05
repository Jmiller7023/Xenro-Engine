#include "MainMenuScreen.h"
#include "globals.h"
#include <XenroEngine\Globals.h>
#include <XenroEngine\ScreenList.h>
#include <XenroEngine\Game.h>
#include <iostream>
#include <XenroEngine\ResourceManager.h>
#include <XenroEngine\GLTexture.h>
#include <XenroEngine\InputManager.h>

MainMenuScreen::MainMenuScreen(Xenro::Window* window)
	:m_window(window), m_GUI("GUI", window)
{
	m_screenIndex = MAINMENU_SCREEN;
}


MainMenuScreen::~MainMenuScreen()
{
	//Empty
}


int MainMenuScreen::getNextScreenIndex() const {

	return GAMEPLAY_SCREEN;
}

int MainMenuScreen::getPrevScrenIndex() const {

	return NO_CURRENT_SCREEN_INDEX;
}

void MainMenuScreen::create() {
	
	initGUI();

	//Intitialize the shaders.
	m_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	//Cache textures that will be used on creation.
	Xenro::ResourceManager::getTexture("Textures/BackGround.png");
	Xenro::ResourceManager::getTexture("Textures/BackGround1.png");
}

void MainMenuScreen::destroy() {

}


void MainMenuScreen::onEntry() {

	//initialize the Audio Engine.
	m_audioEngine.openEngine();

	//Start level music.
	m_audioEngine.loadSong("Audio/Music/mm2.ogg", 64).play();

	//Set the camera properly.
	m_camera.reset(m_window);

	//Initialize spritefont
	m_spriteFont = Xenro::SpriteFont("Fonts/Pixel_Bubble.ttf", 128);

	//Initialize the HUD
	m_hud.initHUD(m_HUDspriteBatch, &m_spriteFont, &m_textureProgram, m_window, Xenro::Justification::MIDDLE);
	m_hud.setScale(0.75);

	//Update mouse cursor.
	glm::vec2 coords = m_game->getInputManager()->getMouseCoords();

	//Decide whether to show mouse cursor or not.
	if (m_game->isControllerConnected()) {
		m_GUI.hideCursor();
		calculateMousePos();
	}
	else {
		m_GUI.showCursor();
		m_GUI.setMousePos(coords.x, coords.y);
	}

	//Disables normal mouse cursor.
	m_window->showCursor(false);

	//Update the camera
	m_camera.update();
}

void MainMenuScreen::onExit() {

	m_audioEngine.closeEngine();
	m_currButtonIndex = 0;

}

void MainMenuScreen::calculateMousePos() {

	if (m_currButtonIndex == START_BUTTON) {
		m_GUI.setMousePos(m_startButton->getPixelPosition().d_x, m_startButton->getPixelPosition().d_y);
	}
	else if(m_currButtonIndex == OPTIONS_BUTTON) {
		m_GUI.setMousePos(m_optionsButton->getPixelPosition().d_x, m_optionsButton->getPixelPosition().d_y);
	}
	else if (m_currButtonIndex == EXIT_BUTTON) {
		m_GUI.setMousePos(m_exitButton->getPixelPosition().d_x, m_exitButton->getPixelPosition().d_y);
	}
	else {
		printf("Error with the screen Indices!");
	}

}

void MainMenuScreen::update() {
	
	updateGUI();

	if (!m_exitGame) {

		m_camera.update();

		if (m_game->getInputManager()->isPressed(Xenro::Button::DPAD_DOWN)) {
			m_audioEngine.loadSFX("Audio/SFX/Move_Button.wav").play();
			if (++m_currButtonIndex == 3) {
				m_currButtonIndex = 0;
			}
		}

		if (m_game->getInputManager()->isPressed(Xenro::Button::DPAD_UP)) {
			m_audioEngine.loadSFX("Audio/SFX/Move_Button.wav").play();
			if (--m_currButtonIndex == -1) {
				m_currButtonIndex = 2;
			}
		}

		if (m_game->getInputManager()->isPressed(Xenro::Button::A)) {
			m_GUI.mouseClick();
		}
	}
}

void MainMenuScreen::draw() {

	//Endable the shader.
	m_textureProgram.use("mySampler");
	m_camera.updateUniform(&m_textureProgram, "P");
	
	//Prepare dimensions to be drawn.
	glm::vec4 destRect(-m_window->getScreenWidth() / 2.0f, -m_window->getScreenHeight() / 2.0, m_window->getScreenWidth(), m_window->getScreenHeight());
	Xenro::ColorRGBA color(255, 255, 255, 255);
	Xenro::GLTexture texture = Xenro::ResourceManager::getTexture("Textures/BackGround1.png");
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	//Draw sprites.
	m_spriteBatch.begin();
	m_spriteBatch.draw(destRect, uvRect, texture.ID, 0.0f, color);
	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	//Draw HUD
	m_hud.setTextPos(m_window->getScreenWidth()*0.5f, m_window->getScreenHeight()*0.8f);
	m_hud.drawHUD("Burning Sabyr");

	//Disable the shader.
	m_textureProgram.unuse();
	
	//Draw GUI.
	m_GUI.draw();
}

bool MainMenuScreen::startGame(const CEGUI::EventArgs& args) {

	m_currState = Xenro::ScreenState::CHANGE_TO_NEXT;
	m_audioEngine.loadSFX("Audio/SFX/Select_Button.wav").playUntilEffectFinishes();
	m_audioEngine.loadSong("Audio/Music/mm2.ogg").fadeOutSong(50);
	return true;
}

bool MainMenuScreen::exitGame(const CEGUI::EventArgs& args) {

	m_currState = Xenro::ScreenState::EXIT_APP;
	m_audioEngine.loadSFX("Audio/SFX/Select_Button.wav").playUntilEffectFinishes();
	m_audioEngine.loadSong("Audio/Music/mm2.ogg").fadeOutSong(50);
	return true;
}


bool MainMenuScreen::openOptions(const CEGUI::EventArgs& args) {

	m_currState = Xenro::ScreenState::CHANGE_TO_PARTICULAR;
	m_changeToParticular = OPTIONS_SCREEN;
	m_audioEngine.loadSFX("Audio/SFX/Select_Button.wav").playUntilEffectFinishes();
	m_audioEngine.loadSong("Audio/Music/mm2.ogg").fadeOutSong(50);
	return true;
}

void MainMenuScreen::initGUI() {

	m_GUI.loadScheme("WindowsLook.scheme");

	m_GUI.loadFont("Jura-10");

	m_startButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("WindowsLook/Button", glm::vec4(0.44f, 0.4f, 0.15f, 0.05f), glm::vec4(0), "StartGameButton"));
	m_startButton->setText("New Game");
	m_startButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::startGame, this));

	m_exitButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("WindowsLook/Button", glm::vec4(0.44f, 0.6f, 0.15f, 0.05f), glm::vec4(0), "ExitGameButton"));
	m_exitButton->setText("Exit Game");
	m_exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::exitGame, this));

	m_optionsButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("WindowsLook/Button", glm::vec4(0.44f, 0.5f, 0.15f, 0.05f), glm::vec4(0), "openOptionsButton"));
	m_optionsButton->setText("Options");
	m_optionsButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::openOptions, this));


	m_GUI.setMouseCursor("WindowsLook/MouseArrow");

	//Prevent initializing GUI twice.
	m_GUIinitialized = true;
}

void MainMenuScreen::updateGUI() {

	//for testing purposes. Delete afterwards.
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->getInputManager()->processInput(evnt);

		//Prevent a crash on SDL_QUIT
		if (evnt.type == SDL_QUIT) {
			m_exitGame = true;
			return;
		}

		//determine if mouse or controller should be used.
		if (m_game->isControllerConnected()) {
			calculateMousePos();
			m_GUI.hideCursor();
		}
		else {
			glm::vec2 coords = m_game->getInputManager()->getMouseCoords();
			m_GUI.setMousePos(coords.x, coords.y);
			m_GUI.showCursor();
		}

		//Determine if mouse inputs should be injected or not.
		if (m_game->isControllerConnected() && evnt.type != SDL_MOUSEMOTION && evnt.type != SDL_MOUSEBUTTONDOWN && evnt.type != SDL_MOUSEBUTTONUP) {
			m_GUI.onEvent(evnt);
		}
		else if (!m_game->isControllerConnected()) {
			m_GUI.onEvent(evnt);
		}
	}
}