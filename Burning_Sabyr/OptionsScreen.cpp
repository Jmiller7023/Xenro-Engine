#include "OptionsScreen.h"
#include "globals.h"
#include <XenroEngine\ResourceManager.h>
#include <XenroEngine\ScreenList.h>
#include <XenroEngine\InputManager.h>
#include <XenroEngine\Game.h>


OptionsScreen::OptionsScreen(Xenro::Window* window)
	:m_GUI("GUI", window), m_window(window)
{
	//Empty
}


OptionsScreen::~OptionsScreen()
{
	//Empty
}

int OptionsScreen::getNextScreenIndex() const {

	return NO_CURRENT_SCREEN_INDEX;
}

int OptionsScreen::getPrevScrenIndex() const {

	return MAINMENU_SCREEN;
}

void OptionsScreen::create() {

	initGUI();

	//Intitialize the shaders.
	m_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}

void OptionsScreen::destroy() {

}


void OptionsScreen::onEntry() {

	//initialize the Audio Engine.
	m_audioEngine.openEngine();

	//Start level music.
	m_audioEngine.loadSong("Audio/Music/FF1.ogg").play();

	//Set the camera properly.
	m_camera.init(m_window);

	//Update mouse cursor.
	glm::vec2 coords = m_game->getInputManager()->getMouseCoords();
	m_GUI.setMousePos(coords.x, coords.y);

	//Disables normal mouse cursor.
	SDL_ShowCursor(0);
}

void OptionsScreen::onExit() {

	m_audioEngine.closeEngine();
}

void OptionsScreen::update() {

	updateGUI();
	
	if (!m_exitGame) {
		m_camera.update();
	}

}

void OptionsScreen::draw() {

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
	Xenro::GLTexture texture = Xenro::ResourceManager::getTexture("Textures/BackGround.png");
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	m_spriteBatch.begin();

	m_spriteBatch.draw(destRect, uvRect, texture.ID, 0.0f, color);

	m_spriteBatch.end();

	m_spriteBatch.renderBatch();

	//unbind the texture.
	glBindTexture(GL_TEXTURE_2D, 0);
	m_textureProgram.unuse();

	m_GUI.draw();
}

void OptionsScreen::updateGUI() {

	//for testing purposes. Delete afterwards.
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->getInputManager()->processInput(evnt);

		//Prevent a crash on SDL_QUIT
		if (evnt.type == SDL_QUIT) {
			m_exitGame = true;
			return;
		}

		m_GUI.onEvent(evnt);
	}
}

void OptionsScreen::initGUI() {

	m_GUI.loadScheme("TaharezLook.scheme");

	m_GUI.loadFont("Jura-10");
	CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("TaharezLook/Button", glm::vec4(0.44f, 0.4f, 0.15f, 0.05f), glm::vec4(0), "MainMenuButton"));
	testButton->setText("Back");
	testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::backToMainMenu, this));
	/*
	CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("TaharezLook/Button", glm::vec4(0.44f, 0.5f, 0.15f, 0.05f), glm::vec4(0), "ExitGameButton"));
	exitButton->setText("Exit Game");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::exitGame, this));
	*/
	m_GUI.setMouseCursor("TaharezLook/MouseArrow");
	m_GUI.showCursor();
	
	//Prevent initializing GUI twice.
	m_GUIinitialized = true;
}

bool OptionsScreen::Vsync(const CEGUI::EventArgs& args) {
	return true;
}

bool OptionsScreen::ScreenResolution(const CEGUI::EventArgs& args) {
	return true;
}

bool OptionsScreen::Windowmode(const CEGUI::EventArgs& args) {
	return true;
}

bool OptionsScreen::backToMainMenu(const CEGUI::EventArgs& args) {

	m_currState = Xenro::ScreenState::CHANGE_TO_PARTICULAR;
	m_changeToParticular = MAINMENU_SCREEN;
	return true;
}