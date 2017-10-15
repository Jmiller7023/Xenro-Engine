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


GameplayScreen::GameplayScreen(Xenro::Window* window)
	:m_window(window), m_GUI("GUI")
{

}


GameplayScreen::~GameplayScreen()
{
	delete m_player;
	delete m_spriteFont;
}

//Return index
int GameplayScreen::getNextScreenIndex() const {
	return NO_CURRENT_SCREEN_INDEX;
}

int GameplayScreen::getPrevScrenIndex() const {
	return NO_CURRENT_SCREEN_INDEX;
}

//Called at beginning and end of application.
void GameplayScreen::create() {
}

void GameplayScreen::destroy() {

}

//Called whenever a screen closes or opens.
void GameplayScreen::onEntry() {

	//Start level music.
	m_audioEngine.loadSong("Audio/Music/GoodZelda.ogg").play();

	//Set the camera properly.
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());

	//Intitialize the shaders.
	m_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	m_lightProgram.compileShaders("Shaders/lightShading.vert", "Shaders/lightShading.frag");
	m_lightProgram.addAttribute("vertexPosition");
	m_lightProgram.addAttribute("vertexColor");
	m_lightProgram.addAttribute("vertexUV");
	m_lightProgram.linkShaders();

	//Initialize spritefont
	m_spriteFont = new Xenro::SpriteFont("Fonts/Pixel_Bubble.ttf", 64);

	//Initialize the HUD
	m_hud.initHUD(m_spriteBatch, m_spriteFont, &m_textureProgram, m_window->getScreenWidth(), m_window->getScreenHeight());

	//Read in level data.
	m_levelLoader.updateGameWorld();

	//Allocate the player.
	m_player = new Player;
	m_player->init(8.0f, m_levelLoader.getStartPlayerPos(), m_game->getInputManager());
	Xenro::ColorRGBA color(255, 255, 255, 255);
	glm::vec2 SpriteSheetDims(4, 9);

	m_player->initializeActor("Textures/aqua.png", color, SpriteSheetDims, m_levelLoader.getStartPlayerPos(), glm::vec2(60.0f), glm::vec2(30.0f, 40.0f));

	//Initialize the lights.
	Xenro::Light playerLight(Xenro::ColorRGBA(255, 0, 255, 128),
		glm::vec2(m_player->getPos().x + m_player->getDrawDims().x / 2.0f, m_player->getPos().y + m_player->getDrawDims().y / 2.0f),
		glm::vec2(150.0f));

	Xenro::Light mouseLight(Xenro::ColorRGBA(0, 0, 255, 255),
		m_camera.convertScreentoWorld(m_game->getInputManager()->getMouseCoords()),
		glm::vec2(300.0f));

	m_playerLightIndex = m_lightEngine.addLight(playerLight);
	m_mouselightIndex = m_lightEngine.addLight(mouseLight);

	//Init GUI.
	m_GUI.loadScheme("AlfiskoSkin.scheme");
	m_GUI.loadFont("DejaVuSans-10");
	CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("AlfiskoSkin/Button", glm::vec4(0.5f, 0.5f, 0.15f, 0.05f), glm::vec4(0), "TestButton"));
	testButton->setText("3 hours for this...");

}

void GameplayScreen::onExit() {

}

//Called in Gameloop update function.
void GameplayScreen::update() {

	m_player->update(m_levelLoader.getLevelData());

	m_camera.setPosition(m_player->getPos());

	m_camera.update();
	
	m_lightEngine.modifyLightPos(m_mouselightIndex, m_camera.convertScreentoWorld(m_game->getInputManager()->getMouseCoords()));
	m_lightEngine.modifyLightPos(m_playerLightIndex, glm::vec2(m_player->getPos().x + m_player->getDrawDims().x / 2.0f, m_player->getPos().y + m_player->getDrawDims().y / 2.0f));
		
	for (size_t i = 0; i < m_bullets.size();) {
		if (m_bullets[i].updatePos() == true) {
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	if (m_game->getInputManager()->isDown(SDLK_w)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (m_game->getInputManager()->isDown(SDLK_s)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (m_game->getInputManager()->isDown(SDLK_a)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (m_game->getInputManager()->isDown(SDLK_d)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (m_game->getInputManager()->isDown(SDLK_q)) {
		m_camera.setScale(m_camera.getScale() + SCALE_SPEED);
	}
	if (m_game->getInputManager()->isDown(SDLK_e)) {
		m_camera.setScale(m_camera.getScale() - SCALE_SPEED);
	}


	/*
	if (m_game->getInputManager()->isPressed(SDLK_o)) {

		m_gamestate = GameState::PAUSE;

		m_audioEngine.loadSong("Audio/Music/Persona.ogg").stop();
		m_audioEngine.loadSong("Audio/Music/song.ogg").play();

		//Remember to release the key.
		m_game->getInputManager()->keyRelease(SDLK_o);

		pauseLoop();

		m_audioEngine.loadSong("Audio/Music/song.ogg").stop();
		m_audioEngine.loadSong("Audio/Music/Persona.ogg").play();
	}
*/
	if (m_game->getInputManager()->isPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = m_game->getInputManager()->getMouseCoords();
		mouseCoords = m_camera.convertScreentoWorld(mouseCoords);
		glm::vec2 playerPosition(m_player->getPos());
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);
		m_bullets.emplace_back(playerPosition, direction, 10.0f, 500);
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

	/*
	if (m_gamestate == GameState::PAUSE) {
		m_hud.setTextPos(m_screenwidth / 2 - 150, m_screenheight / 2);
		m_hud.drawHUD("PAUSED");
	}*/

	//unbind the texture.
	glBindTexture(GL_TEXTURE_2D, 0);
	m_textureProgram.unuse();

	//Render outlines.
	if(m_game->getInputManager()->isDown(SDLK_p)){
		glm::vec4 destRect;

		destRect.x = m_player->getPos().x + m_player->getDrawDims().x / 4.0f;
		destRect.y = m_player->getPos().y;
		destRect.z = m_player->getHboxDims().x;
		destRect.w = m_player->getHboxDims().y;
		m_outlineRenderer.drawBox(destRect, Xenro::ColorRGBA(255, 255, 255, 255), 0.0f);
		m_outlineRenderer.end();
		m_outlineRenderer.render(cameraMatrix, 2.0f);
	}

	//Render lights.
	m_lightProgram.use();

	pLocation = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	m_lightEngine.renderAllLights();

	m_lightEngine.renderAllLights();

	m_lightProgram.unuse();

	m_GUI.draw();
}