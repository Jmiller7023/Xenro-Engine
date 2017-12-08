#include "GameplayScreen.h"
#include <XenroEngine\ScreenList.h>
#include <XenroEngine\InputManager.h>
#include "Player.h"
#include <XenroEngine\ResourceManager.h>
#include <iostream>
#include <XenroEngine\SpriteFont.h>
#include <XenroEngine\Game.h>
#include <XenroEngine\Globals.h>
#include <XenroEngine\Vertex.h>
#include <XenroEngine\SaveManager.h>
#include <iostream>
#include <XenroEngine\LoadManager.h>
#include "globals.h"

GameplayScreen::GameplayScreen(Xenro::Window* window)
	:m_window(window), m_GUI("GUI", window)
{
	m_screenIndex = GAMEPLAY_SCREEN;
}


GameplayScreen::~GameplayScreen()
{
	destroy();
}

//Return index
int GameplayScreen::getNextScreenIndex() const {

	return NO_CURRENT_SCREEN_INDEX;
}

int GameplayScreen::getPrevScrenIndex() const {

	return MAINMENU_SCREEN;
}

void GameplayScreen::create() {
	
	//Init GUI.
	m_GUI.loadScheme("TaharezLook.scheme");
	m_GUI.loadFont("Jura-10");

	m_GUI.setMouseCursor("TaharezLook/MouseArrow");
	m_GUI.showCursor();

	m_mainMenuButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("TaharezLook/Button", glm::vec4(0.44f, 0.4f, 0.15f, 0.05f), glm::vec4(0), "MainMenubutton"));
	m_mainMenuButton->setText("Back");
	m_mainMenuButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::BacktoMainMenu, this));
	m_mainMenuButton->disable();
	m_mainMenuButton->setAlpha(0);

	//Intitialize the shaders.
	m_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

}

void GameplayScreen::destroy() {

	delete m_player;
	delete m_spriteFont;
	m_player = nullptr;
	m_spriteFont = nullptr;
}

//Called whenever a screen closes or opens.
void GameplayScreen::onEntry() {

	//initialize the Audio Engine.
	m_audioEngine.openEngine();

	//Start level music.
	m_audioEngine.loadSong("Audio/Music/GoodZelda.ogg").play();

	//Set the camera properly.
	m_camera.reset(m_window);

	//Initialize spritefont
	m_spriteFont = new Xenro::SpriteFont("Fonts/Pixel_Bubble.ttf", 64);

	//Initialize the HUD
	m_hud.initHUD(m_spriteBatch, m_spriteFont, &m_textureProgram, m_window);

	//Read in level data.
	m_levelLoader.updateGameWorld();

	//Allocate the player.
	m_player = new Player(8.0f, m_levelLoader.getStartPlayerPos(), m_game->getInputManager());
	Xenro::ColorRGBA color(255, 255, 255, 255);
	glm::vec2 SpriteSheetDims(4, 9);

	m_player->initializeActor("Textures/aqua.png", color, SpriteSheetDims, m_levelLoader.getStartPlayerPos(), glm::vec2(60.0f), glm::vec2(30.0f, 40.0f));

	//Update mouse cursor.
	glm::vec2 coords = m_game->getInputManager()->getMouseCoords();
	m_GUI.setMousePos(coords.x, coords.y);

	//Disables normal mouse cursor.
	SDL_ShowCursor(0);
	m_GUI.hideCursor();

	//Update the camera
	m_camera.update();
}

void GameplayScreen::onExit() {

	m_audioEngine.closeEngine();
	m_bullets.clear();
	destroy();
	m_mainMenuButton->disable();
	m_mainMenuButton->setAlpha(0);
	m_mainMenuButtonShown = false;
}

//Called in Gameloop update function.
void GameplayScreen::update() {

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

		//Crappy work around. Fix later.
		if (evnt.type == SDL_QUIT)
			return;
	}

	//endtest

	//Fix the screen in case the window was resized.
	if (evnt.type == SDL_WINDOWEVENT) {
		if (evnt.window.event == SDL_WINDOWEVENT_RESIZED) {
			m_camera.reset(m_window);
		}
	}
	
	m_player->update(m_levelLoader.getLevelData());

	m_camera.setPosition(m_player->getPos());

	m_camera.update();
	
	for (size_t i = 0; i < m_bullets.size();) {
		if (m_bullets[i].updatePos() == true) {
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	if (m_game->getInputManager()->isDown(SDLK_q) || m_game->getInputManager()->isDown(Xenro::Button::LEFT_TRIGGER)) {
		if(m_camera.getScale() < 2.0)
			m_camera.setScale(m_camera.getScale() + Xenro::SCALE_SPEED);
	}
	if (m_game->getInputManager()->isDown(SDLK_e) || m_game->getInputManager()->isDown(Xenro::Button::RIGHT_TRIGGER)) {
		if(m_camera.getScale() > 0.5)
			m_camera.setScale(m_camera.getScale() - Xenro::SCALE_SPEED);
	}
	if (m_game->getInputManager()->isDown(SDLK_ESCAPE) || m_game->getInputManager()->isDown(Xenro::Button::BACK)) {
		m_currState = Xenro::ScreenState::EXIT_APP;
	}

	if (m_game->getInputManager()->isPressed(Xenro::Button::RIGHTSHOULDER)) {
		m_game->rumbleController(0.75, 500);
	}

	if (m_game->getInputManager()->isPressed(Xenro::Button::START) || m_game->getInputManager()->isPressed(SDLK_RSHIFT)) {
		if (!m_mainMenuButtonShown) {
			m_mainMenuButton->enable();
			m_mainMenuButton->setAlpha(255);
			m_mainMenuButtonShown = true;
			m_GUI.setMousePos(m_mainMenuButton->getPixelPosition().d_x, m_mainMenuButton->getPixelPosition().d_y);
		}
		else {
			m_mainMenuButton->disable();
			m_mainMenuButton->setAlpha(0);
			m_mainMenuButtonShown = false;
		}
	}

	if (m_game->getInputManager()->isPressed(Xenro::Button::A)) {
		m_GUI.mouseClick();
	}

	if ((m_game->getInputManager()->isPressed(SDL_BUTTON_LEFT) && !m_game->isControllerConnected())|| m_game->getInputManager()->isPressed(Xenro::Button::A)) {

		m_bullets.emplace_back(m_player->getPos(), glm::normalize(m_player->getDirection()), 10.0f, 500);
		m_audioEngine.loadSFX("Audio/SFX/shot.wav").play();
	}

}

void GameplayScreen::draw() {

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

	m_levelLoader.draw();

	m_spriteBatch.begin();

	for (size_t i = 0; i < m_bullets.size(); i++) {
		m_bullets[i].draw(m_spriteBatch);
	}

	m_player->draw(m_spriteBatch);

	m_spriteBatch.end();

	m_spriteBatch.renderBatch();

	m_hud.setColor(255, 0, 255);

	m_hud.setTextPos();

	m_hud.drawHUD((int)m_bullets.size(), "Num Bullets: ");

	//unbind the texture.
	glBindTexture(GL_TEXTURE_2D, 0);
	m_textureProgram.unuse();

	//Render outlines.
	if(m_game->getInputManager()->isDown(SDLK_p) ||  m_game->getInputManager()->isDown(Xenro::Button::Y)){
		glm::vec4 destRect;

		destRect.x = m_player->getPos().x + m_player->getDrawDims().x / 4.0f;
		destRect.y = m_player->getPos().y;
		destRect.z = m_player->getHboxDims().x;
		destRect.w = m_player->getHboxDims().y;
		m_outlineRenderer.drawBox(destRect, Xenro::ColorRGBA(255, 255, 255, 255), 0.0f);
		m_outlineRenderer.end();
		m_outlineRenderer.render(cameraMatrix, 2.0f);
	}

	pLocation = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	m_GUI.draw();
}

bool GameplayScreen::BacktoMainMenu(const CEGUI::EventArgs& args) {

	m_currState = Xenro::ScreenState::CHANGE_TO_PARTICULAR;
	m_changeToParticular = MAINMENU_SCREEN;
	m_audioEngine.loadSFX("Audio/SFX/Select_Button.wav").playUntilEffectFinishes();
	return true;
}