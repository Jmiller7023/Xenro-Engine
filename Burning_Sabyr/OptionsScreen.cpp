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

	//Initialize MainMenu Button.
	m_mainMenuButtion = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("TaharezLook/Button", glm::vec4(0.44f, 0.2f, 0.15f, 0.05f), glm::vec4(0), "MainMenuButton"));
	m_mainMenuButtion->setText("Back");
	m_mainMenuButtion->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::backToMainMenu, this));

	//Initialize Resolution Button Window.
	m_openResolutionWindow = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("TaharezLook/Button", glm::vec4(0.44f, 0.3f, 0.15f, 0.05f), glm::vec4(0), "ResolutionWindowButton"));
	m_openResolutionWindow->setText("Change Resolution");
	m_openResolutionWindow->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::openResolutionWindow, this));

	//Initialize Resolution Window.
	m_resolutionWindow = static_cast<CEGUI::FrameWindow*>(m_GUI.createWidget("TaharezLook/FrameWindow", glm::vec4(0.3f, 0.3f, 0.4f, 0.4f), glm::vec4(0), "ResolutionWindow"));
	m_resolutionWindow->setText("Test");
	m_resolutionWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&OptionsScreen::onExitClicked, this));
	m_resolutionWindow->setDragMovingEnabled(false);

	//Initialize Children of Resolution Window.
	m_resolution1 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "TaharezLook/Button", glm::vec4(0.4f, 0.2f, 0.25f, 0.08f), glm::vec4(0), "800x600"));
	m_resolution1->setText("800x600");
	m_resolution1->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution1, this));

	m_resolution2 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "TaharezLook/Button", glm::vec4(0.4f, 0.3f, 0.25f, 0.08f), glm::vec4(0), "1024X768"));
	m_resolution2->setText("1024x768");
	m_resolution2->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution2, this));

	m_resolution3 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "TaharezLook/Button", glm::vec4(0.4f, 0.4f, 0.25f, 0.08f), glm::vec4(0), "1280x720"));
	m_resolution3->setText("1280x720");
	m_resolution3->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution3, this));

	m_resolution4 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "TaharezLook/Button", glm::vec4(0.4f, 0.5f, 0.25f, 0.08f), glm::vec4(0), "1360x768"));
	m_resolution4->setText("1360x768");
	m_resolution4->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution4, this));

	m_resolution5 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "TaharezLook/Button", glm::vec4(0.4f, 0.6f, 0.25f, 0.08f), glm::vec4(0), "1600x900"));
	m_resolution5->setText("1600x900");
	m_resolution5->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution5, this));

	m_resolution6 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "TaharezLook/Button", glm::vec4(0.4f, 0.7f, 0.25f, 0.08f), glm::vec4(0), "1920x1080"));
	m_resolution6->setText("1920x1080");
	m_resolution6->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution6, this));

	m_resolution7 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "TaharezLook/Button", glm::vec4(0.4f, 0.8f, 0.25f, 0.08f), glm::vec4(0), "3840x2160"));
	m_resolution7->setText("3840x2160");
	m_resolution7->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution7, this));

	//Start Window Disabled.
	m_resolutionWindow->setAlpha(0.0f);
	m_resolutionWindow->disable();

	m_GUI.setMouseCursor("TaharezLook/MouseArrow");
	m_GUI.showCursor();
	
	//Prevent initializing GUI twice.
	m_GUIinitialized = true;
}

bool OptionsScreen::Vsync(const CEGUI::EventArgs& args) {
	return true;
}

bool OptionsScreen::screenResolution1(const CEGUI::EventArgs& args) {
	m_window->setInitScreenWidth(800);
	m_window->setInitScreenHeight(600);
	m_window->modifyWindowSize(800, 600);
	return true;
}

bool OptionsScreen::screenResolution2(const CEGUI::EventArgs& args) {
	m_window->setInitScreenWidth(1024);
	m_window->setInitScreenHeight(768);
	m_window->modifyWindowSize(1024, 768);
	return true;
}

bool OptionsScreen::screenResolution3(const CEGUI::EventArgs& args) {
	m_window->setInitScreenWidth(1280);
	m_window->setInitScreenHeight(720);
	m_window->modifyWindowSize(1280, 720);
	return true;
}

bool OptionsScreen::screenResolution4(const CEGUI::EventArgs& args) {
	m_window->setInitScreenWidth(1360);
	m_window->setInitScreenHeight(768);
	m_window->modifyWindowSize(1360, 768);
	return true;
}

bool OptionsScreen::screenResolution5(const CEGUI::EventArgs& args) {
	m_window->setInitScreenWidth(1600);
	m_window->setInitScreenHeight(900);
	m_window->modifyWindowSize(1600, 900);
	return true;
}

bool OptionsScreen::screenResolution6(const CEGUI::EventArgs& args) {
	m_window->setInitScreenWidth(1920);
	m_window->setInitScreenHeight(1080);
	m_window->modifyWindowSize(1920, 1080);
	return true;
}

bool OptionsScreen::screenResolution7(const CEGUI::EventArgs& args) {
	m_window->setInitScreenWidth(3840);
	m_window->setInitScreenHeight(2160);
	m_window->modifyWindowSize(3840, 2160);
	return true;
}

bool OptionsScreen::backToMainMenu(const CEGUI::EventArgs& args) {

	m_currState = Xenro::ScreenState::CHANGE_TO_PARTICULAR;
	m_changeToParticular = MAINMENU_SCREEN;
	return true;
}

bool OptionsScreen::onExitClicked(const CEGUI::EventArgs& args) {

	m_resolutionWindow->setAlpha(0.0f);
	m_resolutionWindow->disable();

	m_openResolutionWindow->setAlpha(255.0f);
	m_openResolutionWindow->enable();
	return true;
}

bool OptionsScreen::openResolutionWindow(const CEGUI::EventArgs& args) {

	m_openResolutionWindow->setAlpha(0.0f);
	m_openResolutionWindow->disable();

	m_resolutionWindow->enable();
	m_resolutionWindow->setAlpha(255.0f);
	return true;
}