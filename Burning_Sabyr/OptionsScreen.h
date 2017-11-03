#ifndef OPTIONS_SCREEN_DEFINED
#define OPTIONS_SCREEN_DEFINED

#include <XenroEngine\GUI.h>
#include <XenroEngine\Window.h>
#include <XenroEngine\Camera.h>
#include <XenroEngine\SpriteBatch.h>
#include <XenroEngine\IScreen.h>
#include <XenroEngine\AudioEngine.h>
#include <XenroEngine\GLSLProgram.h>

class OptionsScreen : public Xenro::IScreen
{
public:
	OptionsScreen(Xenro::Window* window);
	~OptionsScreen();

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
	Xenro::GUI m_GUI;
	Xenro::AudioEngine m_audioEngine;

	void initGUI();
	void updateGUI();

	bool Vsync(const CEGUI::EventArgs& args);
	bool ScreenResolution(const CEGUI::EventArgs& args);
	bool Windowmode(const CEGUI::EventArgs& args);
	bool backToMainMenu(const CEGUI::EventArgs& args);
	bool m_GUIinitialized = false;
};

#endif //OPTIONS_SCREEN_DEFINED