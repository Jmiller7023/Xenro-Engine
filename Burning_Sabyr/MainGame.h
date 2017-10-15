#pragma once

#include<SDL\SDL.h>
#include <XenroEngine/Sprite.h>
#include <XenroEngine/GLSLProgram.h>
#include <XenroEngine/GLTexture.h>
#include <XenroEngine\Window.h>
#include <XenroEngine\Camera.h>
#include <XenroEngine\SpriteBatch.h>
#include<XenroEngine\InputManager.h>
#include <XenroEngine\Timing.h>
#include <XenroEngine\SpriteFont.h>
#include <XenroEngine\AudioEngine.h>
#include <XenroEngine\HUD.h>
#include <XenroEngine\ParticleEffect.h>
#include <XenroEngine\ParticleBatch.h>
#include <vector>
#include "Bullet.h"
#include "LevelLoader.h"

class Player;
enum class GameState { PLAY, EXIT, PAUSE, MENU };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	//private functions
	void processInput();
	void pauseLoop();
	void drawGame();
	void initShaders();
	void addBlood(const glm::vec2& pos, int numParticles);

	//private variables
	int m_screenwidth;
	int m_screenheight;

	GameState m_gamestate;

	Xenro::GLSLProgram m_colorProgram;
	Xenro::Camera m_camera;
	Xenro::Window m_window;
	Xenro::SpriteBatch m_spriteBatch;
	Xenro::SpriteBatch m_hudSpriteBatch;
	Xenro::InputManager m_inputManager;
	Xenro::FPSlimiter m_fpsLimiter;
	Xenro::SpriteFont* m_spriteFont;
	Xenro::AudioEngine m_audioEngine;
	Xenro::HUD m_hud;
	Xenro::ParticleEffect m_particleEffect;
	Xenro::ParticleBatch* m_bloodParticleBatch;

	LevelLoader m_levelLoader;

	std::vector<Bullet> m_bullets;

	float m_fps;
	float m_maxFPS;
	bool m_pause = false;

	Player* m_player;
};

