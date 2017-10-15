#pragma once

#include <XenroEngine\Window.h>
#include <XenroEngine\GLSLProgram.h>
#include <XenroEngine\Camera.h>
#include <XenroEngine\InputManager.h>
#include <XenroEngine\SpriteBatch.h>
#include <XenroEngine\Timing.h>
#include <XenroEngine\ResourceManager.h>
#include <XenroEngine\GLTexture.h>
#include <vector>

#include "Bullet.h"
#include "Level.h"
#include "Human.h"
#include "Agent.h"
#include "Player.h"
#include "Zombie.h"

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();
    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();
    /// Handles input processing
    void processInput();
    /// Renders the game
    void drawGame();

	void updateAgents();

	void initLevel();

private:
    /// Member Variables
    Xenro::Window m_window; ///< The game window
	Xenro::GLSLProgram m_textureProgram; ///< The shader program
	Xenro::InputManager m_inputManager; ///< Handles input
	Xenro::Camera m_camera; ///< Main Camera
	Xenro::SpriteBatch m_spriteBatch;
	Xenro::FPSlimiter m_fpsLimiter;

	GameState m_gameState;

	std::vector<Bullet> m_bullets;
	std::vector<Level*> m_levels;
	std::vector<Human*> m_humans;
	std::vector<Zombie*> m_zombies;

	Player* m_player;

	int m_screenWidth, m_screenHeight, m_level;
	float m_maxFPS, m_fps, m_time;
};

