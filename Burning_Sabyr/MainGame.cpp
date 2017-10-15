#include "MainGame.h"
#include <iostream>
#include <string>
#include <XenroEngine/ErrorMessages.h>
#include <XenroEngine\ResourceManager.h>
#include <glm/gtx/rotate_vector.hpp>
#include <GL\glew.h>
#include <algorithm>
#include <random>
#include <ctime>
#include "Player.h"

void updateBloodParticles(Xenro::Particle& particle, float deltaTime) {
	particle.position += particle.velocity * deltaTime;
	particle.color.a = (GLubyte)(particle.lifeTime * 255.0f);
}

MainGame::MainGame()
	
	:m_screenwidth(1000), m_screenheight(648), m_gamestate(GameState::PLAY), m_maxFPS(60.0f)
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	//Tell SDL that we want a double buffered window
	//so we get no flickering.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Initialize the camera.
	m_camera.init(m_screenwidth, m_screenheight);

	//Create window.
	m_window.create("Burning Sabyr", m_screenwidth, m_screenheight, 0);

	//Initialize shaders.
	initShaders();

	//Initialize Music.
	m_audioEngine.loadSong("Audio/Music/Rain.ogg").play();

	//Initialize spritefont
 	m_spriteFont = new Xenro::SpriteFont("Fonts/Pixel_Bubble.ttf", 64);

	//Initialize particles.
	m_bloodParticleBatch = new Xenro::ParticleBatch(1000, 0.05f, Xenro::ResourceManager::getTexture("Textures/particle.png"), updateBloodParticles);
	m_particleEffect.addParticleBatch(m_bloodParticleBatch);

	//Set FPS limit.
	m_fpsLimiter.setTargetFPS(m_maxFPS);

	//Initialize the HUD
	m_hud.initHUD(m_spriteBatch, m_spriteFont, &m_colorProgram, m_screenwidth, m_screenheight);

	//Read in level data.
	m_levelLoader.updateGameWorld();

	//Allocate the player.
	m_player = new Player;
	m_player->init(64.0f, m_levelLoader.getStartPlayerPos(), &m_inputManager);
	Xenro::ColorRGBA color(255, 255, 255, 255);
	glm::vec2 SpriteSheetDims(4, 4);

	m_player->initializeActor("Textures/cursor.png", color, SpriteSheetDims, m_levelLoader.getStartPlayerPos(), glm::vec2(2.0f), glm::vec2(20.0f));
	
}

MainGame::~MainGame()
{
	delete m_player;
	delete m_spriteFont;
	delete m_bloodParticleBatch;
}

//this runs the game
void MainGame::run() {
	//This only returns when the game ends.
	while (m_gamestate != GameState::EXIT) {

		//Used for frame time measuring.
		m_fpsLimiter.calculateFPS();

		processInput();

		m_player->update(m_levelLoader.getLevelData());

		m_camera.setPosition(m_player->getPos());

		m_camera.update();

		m_inputManager.update();

		for (size_t i = 0; i < m_bullets.size();) {
			if (m_bullets[i].updatePos() == true) {
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
			}
			else {
				i++;
			}
		}

		m_particleEffect.update(1.0f);
		drawGame();

		m_fps = m_fpsLimiter.limitFPS();

		//print only once every thousand frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 60) {
			std::cout << "Framerate: " << m_fps << " FPS" << std::endl;
			frameCounter = 0;
		}
	}
}

void MainGame::initShaders() {
	m_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_colorProgram.addAttribute("vertexPosition");
	m_colorProgram.addAttribute("vertexColor");
	m_colorProgram.addAttribute("vertexUV");
	m_colorProgram.linkShaders();
}

//needs some serious work.
void MainGame::pauseLoop() {
	SDL_Event evnt;
	while (m_gamestate == GameState::PAUSE) {
		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) {
			switch (evnt.type) {
			case SDL_QUIT:
				m_gamestate = GameState::EXIT;
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
			case SDL_MOUSEMOTION:
				m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			}
		}
		if (evnt.key.keysym.sym == SDLK_p) {
			m_gamestate = GameState::PLAY;
		}
		drawGame();
	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_gamestate = GameState::EXIT;
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
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
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
	if (m_inputManager.isDown(SDLK_q)){
		m_camera.setScale(m_camera.getScale() + SCALE_SPEED);
}
	if (m_inputManager.isDown(SDLK_e)) {
		m_camera.setScale(m_camera.getScale() - SCALE_SPEED);
	} 

	if (m_inputManager.isDown(SDLK_ESCAPE)) {
		m_gamestate = GameState::EXIT;
	}

	if (m_inputManager.isPressed(SDLK_o)) {

		m_gamestate = GameState::PAUSE;

		m_audioEngine.loadSong("Audio/Music/Persona.ogg").stop();
		m_audioEngine.loadSong("Audio/Music/song.ogg").play();

		//Remember to release the key.
		m_inputManager.keyRelease(evnt.key.keysym.sym);

		pauseLoop();

		m_audioEngine.loadSong("Audio/Music/song.ogg").stop();
		m_audioEngine.loadSong("Audio/Music/Persona.ogg").play();
	}

	if (m_inputManager.isPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = m_inputManager.getMouseCoords();
		mouseCoords = m_camera.convertScreentoWorld(mouseCoords);
		glm::vec2 playerPosition(m_player->getPos());
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);
		addBlood(m_player->getPos(), 5);
		m_bullets.emplace_back(playerPosition, direction, 10.0f, 500);
		m_audioEngine.loadSFX("Audio/SFX/shot.wav").play();
	}
}

void MainGame::drawGame() {
	//Set base depth to 1.0
	glClearDepth(1.0);

	//Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Endable the shader
	m_colorProgram.use();
	//This is using texture unit 0.
	glActiveTexture(GL_TEXTURE0);

	//Get uniform location.
	GLint textureLocation = m_colorProgram.getUniformLocation("mySampler");
	//Tell the shader that the exture is in texture unit 0.
	glUniform1i(textureLocation, 0);

	//Set the camera matrix.
	glm::mat4 cameraMatrix = m_camera.getcamMatrix();
	GLint pLocation = m_colorProgram.getUniformLocation("P");

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

	m_particleEffect.draw(&m_spriteBatch);

	m_hud.setColor(255, 0, 255);

	m_hud.setTextPos();

	m_hud.drawHUD((int)m_bullets.size(), "Num Bullets: ");

	if (m_gamestate == GameState::PAUSE) {
		m_hud.setTextPos(m_screenwidth / 2 - 150, m_screenheight / 2);
		m_hud.drawHUD("PAUSED");
	}

	//unbind the texture.
	glBindTexture(GL_TEXTURE_2D, 0);
	m_colorProgram.unuse();
	
	//Swap our buffer and draw everything to the screen.
	m_window.swapBuffer();
}

void MainGame::addBlood(const glm::vec2& pos, int numParticles) {
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randangle(0.0f, 2 * M_PI);

	glm::vec2 vel(1.0f, 0.0f);
	glm::rotate(vel, randangle(randEngine));

	for(int i =0; i < numParticles; i++){
		m_bloodParticleBatch->addParticle(pos,Xenro::ColorRGBA(255, 0, 0, 255), glm::rotate(vel, randangle(randEngine)), 20.0f);
	}
}