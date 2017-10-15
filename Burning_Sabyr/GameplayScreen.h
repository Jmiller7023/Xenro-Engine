#pragma once
#include <XenroEngine\IScreen.h>
#include <XenroEngine\GLSLProgram.h>
#include <XenroEngine\AudioEngine.h>
#include <XenroEngine\Camera.h>
#include <XenroEngine\Window.h>
#include <XenroEngine\HUD.h>
#include <XenroEngine\SpriteBatch.h>
#include <XenroEngine\OutlineRenderer.h>
#include <XenroEngine\LightEngine.h>
#include <vector>
#include "Bullet.h"
#include "LevelLoader.h"
#include <XenroEngine\GUI.h>

class Player;
class SpriteFont;

class GameplayScreen : public Xenro::IScreen
{
public:
	GameplayScreen(Xenro::Window* window);
	~GameplayScreen();


	//Return index
	virtual int getNextScreenIndex() const override;
	virtual int getPrevScrenIndex() const override;

	//Called at beginning and end of application.
	virtual void create() override;
	virtual void destroy() override;

	//Called whenever a screen closes or opens.
	virtual void onEntry() override;
	virtual void onExit() override;

	//Called in Gameloop.
	virtual void update() override;
	virtual void draw() override;

private:
	Xenro::GLSLProgram m_textureProgram;
	Xenro::GLSLProgram m_lightProgram;
	Xenro::Camera m_camera;
	Xenro::AudioEngine m_audioEngine;
	Xenro::Window* m_window = nullptr;
	Xenro::SpriteBatch m_spriteBatch;
	Xenro::SpriteBatch m_HUDSpriteBatch;
	Xenro::SpriteFont* m_spriteFont;
	Xenro::HUD m_hud;
	Player* m_player;
	LevelLoader m_levelLoader;
	std::vector<Bullet> m_bullets;
	Xenro::OutlineRenderer m_outlineRenderer;
	Xenro::LightEngine m_lightEngine;
	Xenro::GUI m_GUI;

	int m_mouselightIndex;
	int m_playerLightIndex;
};

