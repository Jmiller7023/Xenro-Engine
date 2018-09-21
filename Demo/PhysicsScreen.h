#ifndef PHYSICSSCREEN_DEFINED
#define PHYSICSSCREEN_DEFINED

#include <XenroEngine\IScreen.h>
#include <XenroEngine\Window.h>
#include <XenroEngine\Camera.h>
#include <XenroEngine/GLSLProgram.h>
#include <XenroEngine\SpriteBatch.h>
#include <XenroEngine/GLTexture.h>
#include <Box2D/Box2D.h>
#include "Box.h"
#include <vector>

class Player;

class PhysicsScreen
	: public Xenro::IScreen
{
public:
	PhysicsScreen(Xenro::Window* window);
	virtual ~PhysicsScreen();

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
	void checkInput();

	Xenro::GLTexture m_texture;
	Xenro::GLSLProgram m_textureProgram;
	Xenro::SpriteBatch m_spriteBatch;
	Xenro::Camera m_camera;
	std::vector<Box> m_boxes;
	Xenro::Window* m_window;
	std::unique_ptr<b2World> m_world;
};

#endif //PHYSICSSCREEN_DEFINED