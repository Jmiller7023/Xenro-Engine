#ifndef MAINMENUSCREEN_DEFINED
#define MAINMENUSCREEN_DEFINED

#include <XenroEngine\IScreen.h>
#include <XenroEngine\GLSLProgram.h>
#include <XenroEngine\AudioEngine.h>
#include <XenroEngine\Camera.h>
#include <XenroEngine\Window.h>
#include <XenroEngine\HUD.h>
#include <XenroEngine\SpriteFont.h>
#include <XenroEngine\SpriteBatch.h>
#include <XenroEngine\GUI.h>

class MainMenuScreen : public Xenro::IScreen
{
public:
	MainMenuScreen(Xenro::Window* window);
	virtual ~MainMenuScreen();

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
	Xenro::Window* m_window = nullptr;
	Xenro::GLSLProgram m_textureProgram;
	Xenro::Camera m_camera;
	Xenro::SpriteBatch m_spriteBatch;
	Xenro::SpriteBatch m_HUDspriteBatch;
	Xenro::GUI m_GUI;
	Xenro::AudioEngine m_audioEngine;
	Xenro::SpriteFont m_spriteFont;
	Xenro::HUD m_hud;
	bool startGame(const CEGUI::EventArgs& args);
	bool exitGame(const CEGUI::EventArgs& args);
};

#endif //MAINMENUSCREEN_DEFINED