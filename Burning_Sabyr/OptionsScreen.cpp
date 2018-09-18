#include "OptionsScreen.h"
#include "globals.h"
#include <XenroEngine\ResourceManager.h>
#include <XenroEngine\ScreenList.h>
#include <XenroEngine\InputManager.h>
#include <XenroEngine\Game.h>


OptionsScreen::OptionsScreen(Xenro::Window* window)
	:m_GUI("GUI", window), m_window(window), m_spriteBatch(window, true, glm::vec2(1920, 1080))
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
	m_audioEngine.loadSong("Audio/Music/Options.ogg", 64).play();

	//Set the camera properly.
	m_camera.reset(m_window);

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
	m_window->showCursor(true);
	
	//Update the camera
	m_camera.update();
}

void OptionsScreen::onExit() {

	m_audioEngine.closeEngine();

	//Disable resolution window and make it invisisble.
	m_resolutionWindow->setAlpha(0.0f);
	m_resolutionWindow->disable();

	//Enable resolution window button and make it visible.
	m_openResolutionWindow->setAlpha(255.0f);
	m_openResolutionWindow->enable();

	m_currButtonIndex = 0;
	m_resWindowOpen = false;
}

void OptionsScreen::update() {

	updateGUI();
	
	if (!m_exitGame) {
		m_camera.update();

		//Controller handling.
		if (m_game->getInputManager()->isPressed(Xenro::Button::DPAD_DOWN)) {
			m_audioEngine.loadSFX("Audio/SFX/Move_Button.wav").play();
			m_currButtonIndex++;
			if(!m_resWindowOpen){
				if (m_currButtonIndex == RESOLUTION1) {
					m_currButtonIndex = BACK_TO_MAIN_MENU;
				}				printf("%d", m_currButtonIndex);
			} 
			else {
				if (m_currButtonIndex == UNDEFINED_BUTTON_INDEX_POSTIVE) {
					m_currButtonIndex = RESOLUTION1;
				} 				printf("%d", m_currButtonIndex);
			}
		}

		if (m_game->getInputManager()->isPressed(Xenro::Button::DPAD_UP)) {
			m_audioEngine.loadSFX("Audio/SFX/Move_Button.wav").play();
			m_currButtonIndex--;
			if (!m_resWindowOpen) {
				if (m_currButtonIndex == UNDEFINED_BUTTON_INDEX_NEGATIVE) {
					m_currButtonIndex = OPEN_RESOLUTION_WINDOW;
				}
			}
			else {
				if (m_currButtonIndex == OPEN_RESOLUTION_WINDOW) {
					m_currButtonIndex = RESOLUTION7;
				}
			}
		}

		if (m_game->getInputManager()->isPressed(Xenro::Button::A)) {
			m_GUI.mouseClick();
		}

		if (m_resWindowOpen) {
			if (m_game->getInputManager()->isPressed(Xenro::Button::B)) {
				CEGUI::EventArgs temp;
				onExitClicked(temp);
			}
		}
	}
}

void OptionsScreen::draw() {

	//Reset camera on resolution change.
	if (m_changedRes) {
		m_camera.reset(m_window);
		m_camera.update();
		m_changedRes = false;
	}

	//Endable the shader.
	m_textureProgram.use("mySampler");
	m_camera.updateUniform(&m_textureProgram, "P");

	//Prepare dimensions to be drawn.
	glm::vec4 destRect(-m_spriteBatch.getDefaultWindowSize().x / 2.0f, -m_spriteBatch.getDefaultWindowSize().y / 2.0, m_spriteBatch.getDefaultWindowSize().x, m_spriteBatch.getDefaultWindowSize().y);
	Xenro::ColorRGBA color(255, 255, 255, 255);
	Xenro::GLTexture texture = Xenro::ResourceManager::getTexture("Textures/BackGround.png");
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	//Draw sprites.
	m_spriteBatch.begin();
	m_spriteBatch.draw(destRect, uvRect, texture.ID, 0.0f, color);
	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	//unbind the texture.
	m_textureProgram.unuse();

	//Draw GUI.
	m_GUI.draw();
}


void OptionsScreen::calculateMousePos() {

	if (m_currButtonIndex == BACK_TO_MAIN_MENU) {
		m_GUI.setMousePos(m_mainMenuButton->getPixelPosition().d_x, m_mainMenuButton->getPixelPosition().d_y);
	}
	else if (m_currButtonIndex == OPEN_RESOLUTION_WINDOW) {
		m_GUI.setMousePos(m_openResolutionWindow->getPixelPosition().d_x, m_openResolutionWindow->getPixelPosition().d_y);
	}
	else if (m_currButtonIndex == RESOLUTION1) {
		m_GUI.setMousePos(m_resolution1->getPixelPosition().d_x, m_resolution1->getPixelPosition().d_y);
	}
	else if (m_currButtonIndex == RESOLUTION2) {
		m_GUI.setMousePos(m_resolution2->getPixelPosition().d_x, m_resolution2->getPixelPosition().d_y);
	}
	else if (m_currButtonIndex == RESOLUTION3) {
		m_GUI.setMousePos(m_resolution3->getPixelPosition().d_x, m_resolution3->getPixelPosition().d_y);
	}
	else if (m_currButtonIndex == RESOLUTION4) {
		m_GUI.setMousePos(m_resolution4->getPixelPosition().d_x, m_resolution4->getPixelPosition().d_y);
	}
	else if (m_currButtonIndex == RESOLUTION5) {
		m_GUI.setMousePos(m_resolution5->getPixelPosition().d_x, m_resolution5->getPixelPosition().d_y);
	}
	else if (m_currButtonIndex == RESOLUTION6) {
		m_GUI.setMousePos(m_resolution6->getPixelPosition().d_x, m_resolution6->getPixelPosition().d_y);
	}
	else if (m_currButtonIndex == RESOLUTION7) {
		m_GUI.setMousePos(m_resolution7->getPixelPosition().d_x, m_resolution7->getPixelPosition().d_y);
	}
	else {
		printf("Error with the screen Indices! button index is %d", m_currButtonIndex);
	}

}

void OptionsScreen::updateGUI() {

	//for testing purposes. Delete afterwards.
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->getInputManager()->processInput(evnt);

		//Determine if mouse inputs should be injected or not.
		if (m_game->isControllerConnected() && evnt.type != SDL_MOUSEMOTION && evnt.type != SDL_MOUSEBUTTONDOWN && evnt.type != SDL_MOUSEBUTTONUP) {
			m_GUI.onEvent(evnt);
		}
		else if (!m_game->isControllerConnected()) {
			m_GUI.onEvent(evnt);
		}

		//Determine if mouse or controller should be used.
		if (m_game->isControllerConnected()) {
			calculateMousePos();
			m_GUI.hideCursor();
		}
		else {
			glm::vec2 coords = m_game->getInputManager()->getMouseCoords();
			m_GUI.setMousePos(coords.x, coords.y);
			m_GUI.showCursor();
		}
	}
}

void OptionsScreen::initGUI() {

	m_GUI.loadScheme("GWEN.scheme");

	m_GUI.loadFont("Jura-10");

	//Initialize MainMenu Button.
	m_mainMenuButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("GWEN/Button", glm::vec4(0.44f, 0.2f, 0.15f, 0.05f), glm::vec4(0), "MainMenuButton"));
	m_mainMenuButton->setText("Back");
	m_mainMenuButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::backToMainMenu, this));

	//Initialize Resolution Button Window.
	m_openResolutionWindow = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("GWEN/Button", glm::vec4(0.44f, 0.3f, 0.15f, 0.05f), glm::vec4(0), "ResolutionWindowButton"));
	m_openResolutionWindow->setText("Change Resolution");
	m_openResolutionWindow->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::openResolutionWindow, this));
	m_openResolutionWindow->setAlwaysOnTop(true);

	//Initialize Resolution Window.
	m_resolutionWindow = static_cast<CEGUI::FrameWindow*>(m_GUI.createWidget("GWEN/FrameWindow", glm::vec4(0.3f, 0.3f, 0.4f, 0.4f), glm::vec4(0), "ResolutionWindow"));
	m_resolutionWindow->setText("Test");
	m_resolutionWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&OptionsScreen::onExitClicked, this));
	m_resolutionWindow->setDragMovingEnabled(false);
	m_resolutionWindow->setRollupEnabled(false);

	//Initialize Children of Resolution Window.
	m_resolution1 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "GWEN/Button", glm::vec4(0.4f, 0.2f, 0.25f, 0.08f), glm::vec4(0), "800x600"));
	m_resolution1->setText("800x600");
	m_resolution1->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution1, this));

	m_resolution2 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "GWEN/Button", glm::vec4(0.4f, 0.3f, 0.25f, 0.08f), glm::vec4(0), "1024X768"));
	m_resolution2->setText("1024x768");
	m_resolution2->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution2, this));

	m_resolution3 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "GWEN/Button", glm::vec4(0.4f, 0.4f, 0.25f, 0.08f), glm::vec4(0), "1280x720"));
	m_resolution3->setText("1280x720");
	m_resolution3->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution3, this));

	m_resolution4 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "GWEN/Button", glm::vec4(0.4f, 0.5f, 0.25f, 0.08f), glm::vec4(0), "1360x768"));
	m_resolution4->setText("1360x768");
	m_resolution4->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution4, this));

	m_resolution5 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "GWEN/Button", glm::vec4(0.4f, 0.6f, 0.25f, 0.08f), glm::vec4(0), "1600x900"));
	m_resolution5->setText("1600x900");
	m_resolution5->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution5, this));

	m_resolution6 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "GWEN/Button", glm::vec4(0.4f, 0.7f, 0.25f, 0.08f), glm::vec4(0), "1920x1080"));
	m_resolution6->setText("1920x1080");
	m_resolution6->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution6, this));

	m_resolution7 = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_resolutionWindow, "GWEN/Button", glm::vec4(0.4f, 0.8f, 0.25f, 0.08f), glm::vec4(0), "3840x2160"));
	m_resolution7->setText("3840x2160");
	m_resolution7->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsScreen::screenResolution7, this));

	//Start Window Disabled.
	m_resolutionWindow->setAlpha(0.0f);
	m_resolutionWindow->disable();

	if (m_game->isControllerConnected()) {
		m_GUI.showCursor();
	}
	else {
		m_GUI.hideCursor();
	}
	
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
	m_audioEngine.loadSFX("Audio/SFX/Select_Button.wav").play();
	m_changedRes = true;

	return true;
}

bool OptionsScreen::screenResolution2(const CEGUI::EventArgs& args) {

	m_window->setInitScreenWidth(1024);
	m_window->setInitScreenHeight(768);
	m_window->modifyWindowSize(1024, 768);
	m_audioEngine.loadSFX("Audio/SFX/Select_Button.wav").play();
	m_changedRes = true;

	return true;
}

bool OptionsScreen::screenResolution3(const CEGUI::EventArgs& args) {

	m_window->setInitScreenWidth(1280);
	m_window->setInitScreenHeight(720);
	m_window->modifyWindowSize(1280, 720);
	m_audioEngine.loadSFX("Audio/SFX/Select_Button.wav").play();
	m_changedRes = true;

	return true;
}

bool OptionsScreen::screenResolution4(const CEGUI::EventArgs& args) {

	m_window->setInitScreenWidth(1360);
	m_window->setInitScreenHeight(768);
	m_window->modifyWindowSize(1360, 768);
	m_audioEngine.loadSFX("Audio/SFX/Select_Button.wav").play();
	m_changedRes = true;

	return true;
}

bool OptionsScreen::screenResolution5(const CEGUI::EventArgs& args) {

	m_window->setInitScreenWidth(1600);
	m_window->setInitScreenHeight(900);
	m_window->modifyWindowSize(1600, 900);
	m_audioEngine.loadSFX("Audio/SFX/Select_Button.wav").play();
	m_changedRes = true;

	return true;
}

bool OptionsScreen::screenResolution6(const CEGUI::EventArgs& args) {

	m_window->setInitScreenWidth(1920);
	m_window->setInitScreenHeight(1080);
	m_window->modifyWindowSize(1920, 1080);
	m_audioEngine.loadSFX("Audio/SFX/Select_Button.wav").play();
	m_changedRes = true;

	return true;
}

bool OptionsScreen::screenResolution7(const CEGUI::EventArgs& args) {

	m_window->setInitScreenWidth(3840);
	m_window->setInitScreenHeight(2160);
	m_window->modifyWindowSize(3840, 2160);
	m_audioEngine.loadSFX("Audio/SFX/Select_Button.wav").play();
	m_changedRes = true;

	return true;
}

bool OptionsScreen::backToMainMenu(const CEGUI::EventArgs& args) {

	m_currState = Xenro::ScreenState::CHANGE_TO_PARTICULAR;
	m_changeToParticular = MAINMENU_SCREEN;
	m_audioEngine.loadSFX("Audio/SFX/Select_Button.wav").playUntilEffectFinishes();
	m_audioEngine.loadSong("Audio/Music/Options.ogg").fadeOutSong(50);
	return true;
}

bool OptionsScreen::onExitClicked(const CEGUI::EventArgs& args) {

	m_resolutionWindow->setAlpha(0.0f);
	m_resolutionWindow->disable();

	m_openResolutionWindow->setAlpha(255.0f);
	m_openResolutionWindow->enable();

	m_audioEngine.loadSFX("Audio/SFX/Move_Button.wav").play();
	
	m_resWindowOpen = false;
	m_currButtonIndex = 1;

	return true;
}

bool OptionsScreen::openResolutionWindow(const CEGUI::EventArgs& args) {

	m_openResolutionWindow->setAlpha(0.0f);
	m_openResolutionWindow->disable();

	m_resolutionWindow->enable();
	m_resolutionWindow->setAlpha(255.0f);

	m_audioEngine.loadSFX("Audio/SFX/Select_Button.wav").play();

	m_resWindowOpen = true;
	m_currButtonIndex = 2;

	return true;
}