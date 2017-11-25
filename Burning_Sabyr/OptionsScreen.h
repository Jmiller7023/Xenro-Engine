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

	//GUI Event Handlers
	bool Vsync(const CEGUI::EventArgs& args);
	bool openResolutionWindow(const CEGUI::EventArgs& args);
	bool screenResolution1(const CEGUI::EventArgs& args);
	bool screenResolution2(const CEGUI::EventArgs& args);
	bool screenResolution3(const CEGUI::EventArgs& args);
	bool screenResolution4(const CEGUI::EventArgs& args);
	bool screenResolution5(const CEGUI::EventArgs& args);
	bool screenResolution6(const CEGUI::EventArgs& args);
	bool screenResolution7(const CEGUI::EventArgs& args);
	bool backToMainMenu(const CEGUI::EventArgs& args);
	bool onExitClicked(const CEGUI::EventArgs& args);

	//Member Variables
	bool m_GUIinitialized = false;
	bool m_exitGame = false;
	bool m_changedRes = false;

	CEGUI::FrameWindow* m_resolutionWindow = nullptr;
	CEGUI::PushButton* m_mainMenuButtion = nullptr;
	CEGUI::RadioButton* m_VSYNCCheckBox = nullptr;
	CEGUI::PushButton* m_openResolutionWindow = nullptr;
	CEGUI::PushButton* m_resolution1 = nullptr;
	CEGUI::PushButton* m_resolution2 = nullptr;
	CEGUI::PushButton* m_resolution3 = nullptr;
	CEGUI::PushButton* m_resolution4 = nullptr;
	CEGUI::PushButton* m_resolution5 = nullptr;
	CEGUI::PushButton* m_resolution6 = nullptr;
	CEGUI::PushButton* m_resolution7 = nullptr;
};

#endif //OPTIONS_SCREEN_DEFINED