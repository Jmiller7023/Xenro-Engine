#include "MainGame.h"
#include <SDL/SDL.h>
#include <XenroEngine\Xenro.h>
#include <iostream>
const float CAMERA_SPEED = 2.0f;
const float SCALE_SPEED = 0.0002f;

MainGame::MainGame()
	:m_screenWidth(1024), m_screenHeight(768), m_player(nullptr), m_level(0), m_gameState(GameState::PLAY)
{
    // Empty
}

MainGame::~MainGame() 
{
	for (size_t i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}
}

void MainGame::run() {
	initSystems();

	initLevel();

	gameLoop();
}

void MainGame::initSystems() {
	Xenro::init();
	
	m_window.create("Zombie Hunt", m_screenWidth, m_screenHeight, 0);
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	initShaders();

	m_spriteBatch.init();

	m_camera.init(m_screenWidth, m_screenHeight);


}

void MainGame::initLevel() {
	//Level 1
	m_levels.push_back(new Level ("Levels/level1.txt"));
	m_spriteBatch.init();
	m_fpsLimiter.setTargetFPS(m_maxFPS);
	m_player = new Player;
	m_player->init(1.0f, m_levels[m_level]->getStartPlayerPos(), &m_inputManager);

	m_humans.push_back(m_player);
}

void MainGame::initShaders() {
    // Compile our color shader
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	while (m_gameState != GameState::EXIT) {

		//Used for frame time measuring.
		m_fpsLimiter.calculateFPS();

		processInput();

		updateAgents();

		m_camera.setPosition(m_player->getPos());


		m_camera.update();

		drawGame();
		
		m_fps = m_fpsLimiter.limitFPS();
	}
	
}

void MainGame::updateAgents() {
	for (int i = 0; i < m_humans.size(); i++) {
		m_humans[i]->update(m_levels[m_level]->getLevelData(), m_humans, m_zombies);
	}
	//Zombies.

}

void MainGame::processInput() {
    SDL_Event evnt;

    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
				m_gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
				m_inputManager.keyPress(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
				m_inputManager.keyRelease(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
				m_inputManager.keyPress(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
				m_inputManager.keyRelease(evnt.button.button);
                break;
        }
    
	}
	if (m_inputManager.isDown(SDLK_w)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}

	if (m_inputManager.isDown(SDLK_s)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}

	if (m_inputManager.isDown(SDLK_a)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}

	if (m_inputManager.isDown(SDLK_d)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}

	if (m_inputManager.isDown(SDLK_q)) {
		m_camera.setScale(m_camera.getScale() + SCALE_SPEED);
	}

	if (m_inputManager.isDown(SDLK_e)) {
		m_camera.setScale(m_camera.getScale() - SCALE_SPEED);
	}

	if (m_inputManager.isDown(SDLK_ESCAPE)) {
		m_gameState = GameState::EXIT;
	}

	if (m_inputManager.isDown(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = m_inputManager.getMouseCoords();
		mouseCoords = m_camera.convertScreentoWorld(mouseCoords);
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);

		m_bullets.emplace_back(playerPosition, direction, 1.00f, 1000);
	}
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);

	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = m_camera.getcamMatrix();

	GLint pUniform = m_textureProgram.getUniformLocation("P");

	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//draw the level.
	m_levels[m_level]->draw();

	m_spriteBatch.begin();

	//Draw the Player
	for (int i = 0; i < m_humans.size(); i++) {
		m_humans[i]->draw(m_spriteBatch);
	}

	m_spriteBatch.end();

	m_spriteBatch.renderBatch();

	m_textureProgram.unuse(); 

	//Swap our buffer and draw everything to the screen!
	m_window.swapBuffer();
}