#include "PhysicsScreen.h"
#include <XenroEngine\ScreenList.h>
#include <XenroEngine\InputManager.h>
#include "Player.h"
#include <XenroEngine\ResourceManager.h>
#include <iostream>
#include <XenroEngine\SpriteFont.h>
#include <XenroEngine\Game.h>
#include <XenroEngine\Globals.h>
#include <XenroEngine\Vertex.h>
#include <XenroEngine\SaveManager.h>
#include <iostream>
#include <XenroEngine\LoadManager.h>
#include <random>
#include "globals.h"


PhysicsScreen::PhysicsScreen(Xenro::Window* window)
	:m_window(window)
{
	m_screenIndex = PHYSICS_SCREEN;
}


PhysicsScreen::~PhysicsScreen()
{
	destroy();
}

//Return index
int PhysicsScreen::getNextScreenIndex() const {

	return NO_CURRENT_SCREEN_INDEX;
}

int PhysicsScreen::getPrevScrenIndex() const {

	return MAINMENU_SCREEN;
}

void PhysicsScreen::create() {

}

void PhysicsScreen::destroy() {

}

//Called whenever a screen closes or opens.
void PhysicsScreen::onEntry() {

	b2Vec2 gravity(0.0f, -9.81f);
	m_world = std::make_unique<b2World>(gravity);

	//Make the ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -25.0f);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);
	//Make the ground fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	// Make a bunch of boxes
	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xPos(-10.0, 10.0f);
	std::uniform_real_distribution<float> yPos(-10.0, 25.0f);
	std::uniform_real_distribution<float> size(0.5, 2.5f);
	std::uniform_int_distribution<int> color(50, 255);
	const int NUM_BOXES = 100;

	for (int i = 0; i < NUM_BOXES; i++) {
		Xenro::ColorRGBA randColor;
		randColor.r = color(randGenerator);
		randColor.g = color(randGenerator);
		randColor.b = color(randGenerator);
		randColor.a = 255;
		Box newBox;
		newBox.init(m_world.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(size(randGenerator), size(randGenerator)), randColor);
		m_boxes.push_back(newBox);
	}

	//Intitialize the shaders.
	m_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	//Load texture
	m_texture = Xenro::ResourceManager::getTexture("Textures/red_bricks.png");

	// Init camera
	m_camera.reset(m_window);
	m_camera.setScale(32.0f);

	//Update the camera
	m_camera.update();

}

void PhysicsScreen::onExit() {

}

//Called in Gameloop update function.
void PhysicsScreen::update() {

	m_world->Step(1.0f / 60.0f, 6, 2);
	m_camera.update();

	//For testing purposes. Delete afterwards.
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->getInputManager()->processInput(evnt);
	}
}

void PhysicsScreen::draw() {

	//Enable the shader.
	m_textureProgram.use("mySampler");
	m_camera.updateUniform(&m_textureProgram, "P");

	m_spriteBatch.begin();

	//draw all boxes
	for (auto& b : m_boxes) {
		glm::vec4 destRect;
		destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
		destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
		destRect.z = b.getDimensions().x;
		destRect.w = b.getDimensions().y;
		m_spriteBatch.draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_texture.ID, 0.0f, Xenro::ColorRGBA(255, 0, 0, 255), b.getBody()->GetAngle());
	}

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();
}

