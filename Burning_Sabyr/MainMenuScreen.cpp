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
}

void MainMenuScreen::destroy() {

}


void MainMenuScreen::onEntry() {

	//initialize the Audio Engine.
	m_audioEngine.openEngine();

	//Start level music.
	m_audioEngine.loadSong("Audio/Music/Intro.ogg").play();

	//Set the camera properly.
	m_camera.init(m_window);

	//Intitialize the shaders.
	m_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	//Initialize spritefont
	m_spriteFont = Xenro::SpriteFont("Fonts/Pixel_Bubble.ttf", 80);

	//Initialize the HUD
	m_hud.initHUD(m_HUDspriteBatch, &m_spriteFont, &m_textureProgram, m_window);

	//Update mouse cursor.
	glm::vec2 coords = m_game->getInputManager()->getMouseCoords();
	m_GUI.setMousePos(coords.x, coords.y);


	//Disables normal mouse cursor.
	SDL_ShowCursor(0);
}

void MainMenuScreen::onExit() {

	m_audioEngine.closeEngine();
	//m_GUI.clearGUI();
}

void MainMenuScreen::update() {
	
	updateGUI();
	m_camera.update();

}

void MainMenuScreen::draw() {

	//Set base depth to 1.0
	glClearDepth(1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Endable the shader
	m_textureProgram.use();
	//This is using texture unit 0.
	glActiveTexture(GL_TEXTURE0);

	//Get uniform location.
	GLint textureLocation = m_textureProgram.getUniformLocation("mySampler");
	//Tell the shader that the exture is in texture unit 0.
	glUniform1i(textureLocation, 0);

	//Set the camera matrix.
	glm::mat4 cameraMatrix = m_camera.getcamMatrix();
	GLint pLocation = m_textureProgram.getUniformLocation("P");

	//Pass pointer to openGL
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	glm::vec4 destRect(-m_window->getScreenWidth() / 2.0f, -m_window->getScreenHeight() / 2.0, m_window->getScreenWidth(), m_window->getScreenHeight());
	Xenro::ColorRGBA color(255, 255, 255, 255);
	Xenro::GLTexture texture = Xenro::ResourceManager::getTexture("Textures/BackGround1.png");
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	m_spriteBatch.begin();

	m_spriteBatch.draw(destRect, uvRect, texture.ID, 0.0f, color);

	m_spriteBatch.end();

	m_spriteBatch.renderBatch();

	m_HUDspriteBatch.begin();

	m_hud.setColor(255, 0, 0);

	m_hud.setTextPos(m_window->getScreenWidth() / 13.0f, m_window->getScreenHeight() / 1.0f);

	m_hud.drawHUD("Burning Sabyr");

	m_HUDspriteBatch.end();
	m_HUDspriteBatch.renderBatch();

	//unbind the texture.
	glBindTexture(GL_TEXTURE_2D, 0);
	m_textureProgram.unuse();

	m_GUI.draw();
}

bool MainMenuScreen::startGame(const CEGUI::EventArgs& args) {

	m_currState = Xenro::ScreenState::CHANGE_TO_NEXT;
	return true;
}

bool MainMenuScreen::exitGame(const CEGUI::EventArgs& args) {

	m_currState = Xenro::ScreenState::EXIT_APP;
	return true;
}


bool MainMenuScreen::openOptions(const CEGUI::EventArgs& args) {

	m_currState = Xenro::ScreenState::CHANGE_TO_PARTICULAR;
	m_changeToParticular = OPTIONS_SCREEN;
	return true;
}

void MainMenuScreen::initGUI() {

	m_GUI.loadScheme("TaharezLook.scheme");

	m_GUI.loadFont("Jura-10");
	CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("TaharezLook/Button", glm::vec4(0.44f, 0.4f, 0.15f, 0.05f), glm::vec4(0), "StartGameButton"));
	testButton->setText("New Game");
	testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::startGame, this));

	CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("TaharezLook/Button", glm::vec4(0.44f, 0.6f, 0.15f, 0.05f), glm::vec4(0), "ExitGameButton"));
	exitButton->setText("Exit Game");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::exitGame, this));

	CEGUI::PushButton* optionsButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("TaharezLook/Button", glm::vec4(0.44f, 0.5f, 0.15f, 0.05f), glm::vec4(0), "openOptionsButton"));
	optionsButton->setText("Options");
	optionsButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::openOptions, this));

	m_GUI.setMouseCursor("TaharezLook/MouseArrow");
	m_GUI.showCursor();

	//Prevet=nt initializing GUI twice.
	m_GUIinitialized = true;
}

void MainMenuScreen::updateGUI() {

	//for testing purposes. Delete afterwards.
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->getInputManager()->processInput(evnt);
		m_GUI.onEvent(evnt);
		//Crappy work around. Fix later.
		if (evnt.type == SDL_QUIT)
			return;
	}
}