/*************************************************************************/
/*                          LevelEditor.cpp                              */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Xenro Engine                                */
/*             https://github.com/Jmiller7023/Xenro-Engine               */
/*************************************************************************/
/*                 Copyright  11-3-2017 Joseph Miller.                   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "LevelEditor.h"
#include "Window.h"
#include "Game.h"
#include "InputManager.h"
#include "ResourceManager.h"

namespace Xenro {

//Globals
const int MOUSE_LEFT = 0;
const int MOUSE_RIGHT = 1;
const float LIGHT_SELECT_RADIUS = 5.0f;
const b2Vec2 GRAVITY(0.0f, 0.0f);

void WidgetLabel::draw(SpriteBatch& sb, SpriteFont& sf, Window* w) {
	if (!widget->isVisible()) return;
	glm::vec2 pos;
	pos.x = widget->getXPosition().d_scale * w->getScreenWidth() - w->getScreenWidth() / 2.0f +
		widget->getWidth().d_offset / 2.0f;
	pos.y = w->getScreenHeight() / 2.0f - widget->getYPosition().d_scale * w->getScreenHeight();
	sf.draw(sb, text.c_str(), pos, glm::vec2(scale), 0.0f, color, Justification::MIDDLE);
}

LevelEditor::LevelEditor(Window * window)
	:m_spriteBatch(window, true, glm::vec2(1920,1080)), m_GUI("GUI", window), m_window(window),
	m_outlineRenderer(window, true, glm::vec2(1920, 1080))
{
}

LevelEditor::~LevelEditor()
{
}

int LevelEditor::getNextScreenIndex() const
{
	return 0;
}

int LevelEditor::getPrevScrenIndex() const
{
	return 0;
}

void LevelEditor::create()
{
	initUI();

	//Intitialize the shaders.
	m_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	m_lightProgram.compileShaders("Shaders/lightShading.vert", "Shaders/lightShading.frag");
	m_lightProgram.addAttribute("vertexPosition");
	m_lightProgram.addAttribute("vertexColor");
	m_lightProgram.addAttribute("vertexUV");
	m_lightProgram.linkShaders();
}

void LevelEditor::destroy()
{
}

void LevelEditor::onEntry()
{
	m_spriteFont.init("Fonts/Fipps_Regular.ttf", 16);

	// Set up box2D stuff
	m_world = std::make_unique<b2World>(GRAVITY);

	//Update Camera on re-entry
	m_camera.reset(m_window);
	m_camera.update();

	m_blankTexture = ResourceManager::getTexture("Textures/blank.png");
}

void LevelEditor::onExit()
{
	m_audioEngine.closeEngine();
	m_lightEngine.reset();
}

bool inLightSelect(const Light& l, const glm::vec2& pos) {
	return (glm::length(pos - l.pos) <= LIGHT_SELECT_RADIUS);
}

void LevelEditor::update()
{
	//For testing purposes. Delete afterwards.
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->getInputManager()->processInput(evnt);

		//Determine if mouse inputs should be injected or not.
		if (m_game->isControllerConnected() && evnt.type != SDL_MOUSEMOTION && evnt.type != SDL_MOUSEBUTTONDOWN && evnt.type != SDL_MOUSEBUTTONUP) {
			m_GUI.onEvent(evnt);
		}
		else if (!m_game->isControllerConnected()) {
			m_GUI.onEvent(evnt);
		}
	}

	if (m_game->getInputManager()->isPressed(SDL_BUTTON_LEFT) && !isMouseInUI()) {

		glm::vec2 pos;
		glm::vec4 uvRect;
		static GLTexture texture = ResourceManager::getTexture("Textures/red_bricks.png");

		m_mouseButtons[MOUSE_LEFT] = true;

		if (m_selectMode == SelectionMode::SELECT) {
			// Select mode
			pos = m_camera.convertScreentoWorld(glm::vec2(evnt.button.x, evnt.button.y));
			// Lights have selection priority, so check lights first
			// If a light is already selected, test to see if we just clicked it again
			if (m_selectedLight != NO_LIGHT && inLightSelect(m_lights[m_selectedLight], pos)) {
				// We selected ourself, do nothing.
			}
			else {
				// Unselect
				m_selectedLight = NO_LIGHT;
				// Find the tile that we are selecting
				for (size_t i = 0; i < m_lights.size(); i++) {
					if (inLightSelect(m_lights[i], pos)) {
						m_selectedLight = i;
						break;
					}
				}
			}
			// If we selected a light
			if (m_selectedLight != NO_LIGHT) {
				// Get the offset from the center so we can drag correctly
				m_selectOffset = pos - m_lights[m_selectedLight].pos;
				m_selectedTile = NO_LIGHT;
				m_isDragging = true;
				// Set variables first so refreshing the controls works
				m_colorPickerRed = m_lights[m_selectedLight].color.r;
				m_colorPickerGreen = m_lights[m_selectedLight].color.g;
				m_colorPickerBlue = m_lights[m_selectedLight].color.b;
				m_colorPickerAlpha = m_lights[m_selectedLight].color.a;
				m_lightSize = m_lights[m_selectedLight].size.x;
				// Set widget values
				m_rSlider->setCurrentValue(m_lights[m_selectedLight].color.r);
				m_gSlider->setCurrentValue(m_lights[m_selectedLight].color.g);
				m_bSlider->setCurrentValue(m_lights[m_selectedLight].color.b);
				m_aSlider->setCurrentValue(m_lights[m_selectedLight].color.a);
				m_sizeSpinner->setCurrentValue(m_lights[m_selectedLight].size.x);
				m_lightRadioButton->setSelected(true);
				m_objectMode = ObjectMode::LIGHT;
			}
			
			// If a tile is already selected, test to see if we just clicked it again
			if (m_selectedTile != NO_TILE && m_tiles[m_selectedTile].testPoint(pos.x, pos.y)) {
				// We selected ourself, do nothing.
			}
			else {
				// Unselect
				m_selectedTile = NO_TILE;
				// Find the tile that we are selecting
				for (size_t i = 0; i < m_tiles.size(); i++) {
					if (m_tiles[i].testPoint(pos.x, pos.y)) {
						m_selectedTile = i;
						break;
					}
				}
			}
			// If we selected a tile
			if (m_selectedTile != NO_TILE) {
				// Get the offset from the center so we can drag correctly
				m_selectOffset = pos - m_tiles[m_selectedTile].getPosition();
				m_isDragging = true;
				// Set variables first so refreshing the controls works
				m_rotation = m_tiles[m_selectedTile].getBody()->GetAngle();
				m_tileDims.x = m_tiles[m_selectedTile].getDimensions().x;
				m_tileDims.y = m_tiles[m_selectedTile].getDimensions().y;
				m_colorPickerRed = m_tiles[m_selectedTile].getColor().r;
				m_colorPickerGreen = m_tiles[m_selectedTile].getColor().g;
				m_colorPickerBlue = m_tiles[m_selectedTile].getColor().b;
				// Set widget values
				m_rotationSpinner->setCurrentValue(m_tiles[m_selectedTile].getBody()->GetAngle());
				m_widthSpinner->setCurrentValue(m_tiles[m_selectedTile].getDimensions().x);
				m_heightSpinner->setCurrentValue(m_tiles[m_selectedTile].getDimensions().y);
				m_rSlider->setCurrentValue(m_tiles[m_selectedTile].getColor().r);
				m_gSlider->setCurrentValue(m_tiles[m_selectedTile].getColor().g);
				m_bSlider->setCurrentValue(m_tiles[m_selectedTile].getColor().b);
				if (m_tiles[m_selectedTile].isDynamic()) {
					m_dynamicRadioButton->setSelected(true);
					m_physicsMode = PhysicsMode::DYNAMIC;
				}
				else {
					m_rigidRadioButton->setSelected(true);
					m_physicsMode = PhysicsMode::RIGID;
				}
				m_platformRadioButton->setSelected(true);
				m_objectMode = ObjectMode::PLATFORM;
			}
		}
		else {
			// Place mode
			Tile newTile;
			Light newLight;
			// Place
			switch (m_objectMode) {
				case ObjectMode::PLATFORM:

					pos = m_camera.convertScreentoWorld(glm::vec2(evnt.button.x, evnt.button.y));
					newTile.init(m_world.get(), pos, m_tileDims, texture, ColorRGBA((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, 255),
						false, m_physicsMode == PhysicsMode::DYNAMIC, m_rotation);
					m_tiles.push_back(newTile);
					break;
				case ObjectMode::PLAYER:
					/*
					// Just remove the player, easiest way.
					m_player.destroy(m_world.get());
					// Re-init the player
					pos = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y));
					m_player.init(m_world.get(), pos, glm::vec2(2.0f), glm::vec2(1.0f, 1.8f), Bengine::ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, 255));
					m_hasPlayer = true;
					break;*/
				case ObjectMode::LIGHT:
					newLight.pos = m_camera.convertScreentoWorld(glm::vec2(evnt.button.x, evnt.button.y));
					newLight.size = glm::vec2(m_lightSize);
					newLight.color = ColorRGBA((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, (GLubyte)m_colorPickerAlpha);
					m_lightEngine.addLight(newLight);
					m_lights.push_back(newLight);
					break;
				}
		}
		
	}

	// Platform scaling and rotation from keypress
	if ((m_objectMode == ObjectMode::PLATFORM && m_selectMode == SelectionMode::PLACE) || m_selectedTile != NO_TILE) {
		const double SCALE_SPEED = 4.0;
		const double ROT_SPEED = 0.04;
		// The callbacks will set the member variables for m_tileDims, don't need to set here
		// Scale
		if (m_game->getInputManager()->isDown(SDLK_LEFT)) {
			m_widthSpinner->setCurrentValue(m_widthSpinner->getCurrentValue() - SCALE_SPEED);
		}
		else if (m_game->getInputManager()->isDown(SDLK_RIGHT)) {
			m_widthSpinner->setCurrentValue(m_widthSpinner->getCurrentValue() + SCALE_SPEED);
		}
		if (m_game->getInputManager()->isDown(SDLK_DOWN)) {
			m_heightSpinner->setCurrentValue(m_heightSpinner->getCurrentValue() - SCALE_SPEED);
		}
		else if (m_game->getInputManager()->isDown(SDLK_UP)) {
			m_heightSpinner->setCurrentValue(m_heightSpinner->getCurrentValue() + SCALE_SPEED);
		}
		// Rotation
		if (m_game->getInputManager()->isDown(SDLK_e)) {
			// Have to check wraparound
			double newValue = m_rotationSpinner->getCurrentValue() - ROT_SPEED;
			if (newValue < 0.0) newValue += M_PI * 2.0;
			m_rotationSpinner->setCurrentValue(newValue);
		}
		else if (m_game->getInputManager()->isDown(SDLK_q)) {
			double newValue = m_rotationSpinner->getCurrentValue() + ROT_SPEED;
			if (newValue > M_PI * 2.0) newValue -= M_PI * 2.0;
			m_rotationSpinner->setCurrentValue(newValue);
		}
	}

	// Light scaling from keypress
	if ((m_objectMode == ObjectMode::LIGHT && m_selectMode == SelectionMode::PLACE) || m_selectedLight != NO_LIGHT) {
		const double SCALE_SPEED = 1.0f;
		const float ALPHA_SPEED = 1.0f;
		// Scaling
		if (m_game->getInputManager()->isDown(SDLK_LEFT)) {
			m_sizeSpinner->setCurrentValue(m_sizeSpinner->getCurrentValue() - SCALE_SPEED);
		}
		else if (m_game->getInputManager()->isDown(SDLK_RIGHT)) {
			m_sizeSpinner->setCurrentValue(m_sizeSpinner->getCurrentValue() + SCALE_SPEED);
		}
		// Light intensity
		if (m_game->getInputManager()->isDown(SDLK_DOWN)) {
			m_aSlider->setCurrentValue(m_aSlider->getCurrentValue() - ALPHA_SPEED);
		}
		else if (m_game->getInputManager()->isDown(SDLK_UP)) {
			m_aSlider->setCurrentValue(m_aSlider->getCurrentValue() + ALPHA_SPEED);
		}
	}

	// Check for deleting an object
	if (m_game->getInputManager()->isPressed(SDLK_DELETE)) {
		if (m_selectedLight != NO_LIGHT) {
			m_lights.erase(m_lights.begin() + m_selectedLight);
			m_selectedLight = NO_LIGHT;
		}
		else if (m_selectedTile != NO_TILE) {
			m_tiles[m_selectedTile].destroy(m_world.get());
			m_tiles.erase(m_tiles.begin() + m_selectedTile);
			m_selectedTile = NO_TILE;
		}
	}

	m_GUI.update();
	m_camera.update();
}

void LevelEditor::draw()
{
	drawUI();
	drawWorld();
}

void LevelEditor::initUI() {
	m_GUI.loadScheme("GWEN.scheme");
	m_GUI.loadFont("DejaVuSans-10");

	// Add group box back panel
		m_groupBox = static_cast<CEGUI::GroupBox*>(m_GUI.createWidget("GWEN/GroupBox", glm::vec4(0.001f, 0.0f, 0.18f, 0.72f), glm::vec4(0.0f), "GroupBox"));
	// Group box should be behind everything.
	m_groupBox->setAlwaysOnTop(false);
	m_groupBox->moveToBack();
	m_groupBox->disable(); // If you don't disable it, clicking on it will move it to the foreground and it will steal events from other widgets.

	{ // Add the color picker
		const float X_POS = 0.01f;
		const float X_DIM = 0.015f, Y_DIM = 0.1f;
		const float Y_POS = 0.05f;
		const float PADDING = 0.005f;

		m_rSlider = static_cast<CEGUI::Slider*>(m_GUI.createWidget("GWEN/Slider", glm::vec4(X_POS, Y_POS, X_DIM, Y_DIM), glm::vec4(0.0f), "RedSlider"));
		m_rSlider->setMaxValue(255.0f);
		m_rSlider->setCurrentValue(m_colorPickerRed);
		m_rSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&LevelEditor::onColorPickerRedChanged, this));
		m_rSlider->setClickStep(1.0f);

		m_gSlider = static_cast<CEGUI::Slider*>(m_GUI.createWidget("GWEN/Slider", glm::vec4(X_POS + X_DIM + PADDING, Y_POS, X_DIM, Y_DIM), glm::vec4(0.0f), "GreenSlider"));
		m_gSlider->setMaxValue(255.0f);
		m_gSlider->setCurrentValue(m_colorPickerGreen);
		m_gSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&LevelEditor::onColorPickerGreenChanged, this));
		m_gSlider->setClickStep(1.0f);

		m_bSlider = static_cast<CEGUI::Slider*>(m_GUI.createWidget("GWEN/Slider", glm::vec4(X_POS + (X_DIM + PADDING) * 2, Y_POS, X_DIM, Y_DIM), glm::vec4(0.0f), "BlueSlider"));
		m_bSlider->setMaxValue(255.0f);
		m_bSlider->setCurrentValue(m_colorPickerBlue);
		m_bSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&LevelEditor::onColorPickerBlueChanged, this));
		m_bSlider->setClickStep(1.0f);

		m_aSlider = static_cast<CEGUI::Slider*>(m_GUI.createWidget("GWEN/Slider", glm::vec4(X_POS + (X_DIM + PADDING) * 3, Y_POS, X_DIM, Y_DIM), glm::vec4(0.0f), "AlphaSlider"));
		m_aSlider->setMaxValue(255.0f);
		m_aSlider->setCurrentValue(m_colorPickerAlpha);
		m_aSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&LevelEditor::onColorPickerAlphaChanged, this));
		m_aSlider->setClickStep(1.0f);
	}

	{ // Add Object type radio buttons
		const float X_POS = 0.02f;
		const float Y_POS = 0.20f;
		const float DIMS_PIXELS = 20.0f;
		const float PADDING = 0.042f;
		const float TEXT_SCALE = 0.6f;
		const int GROUP_ID = 1;
		m_playerRadioButton = static_cast<CEGUI::RadioButton*>(m_GUI.createWidget("GWEN/RadioButton", glm::vec4(X_POS, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS), "PlayerButton"));
		m_playerRadioButton->setSelected(true);
		m_playerRadioButton->subscribeEvent(CEGUI::RadioButton::EventMouseClick, CEGUI::Event::Subscriber(&LevelEditor::onPlayerMouseClick, this));
		m_playerRadioButton->setGroupID(GROUP_ID);
		m_widgetLabels.emplace_back(m_playerRadioButton, "Player", TEXT_SCALE);

		m_platformRadioButton = static_cast<CEGUI::RadioButton*>(m_GUI.createWidget("GWEN/RadioButton", glm::vec4(X_POS + PADDING, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS), "PlatformButton"));
		m_platformRadioButton->setSelected(false);
		m_platformRadioButton->subscribeEvent(CEGUI::RadioButton::EventMouseClick, CEGUI::Event::Subscriber(&LevelEditor::onPlatformMouseClick, this));
		m_platformRadioButton->setGroupID(GROUP_ID);
		m_widgetLabels.emplace_back(m_platformRadioButton, "Platform", TEXT_SCALE);

		m_lightRadioButton = static_cast<CEGUI::RadioButton*>(m_GUI.createWidget("GWEN/RadioButton", glm::vec4(X_POS + PADDING * 2.0, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS), "LightButton"));
		m_lightRadioButton->setSelected(false);
		m_lightRadioButton->subscribeEvent(CEGUI::RadioButton::EventMouseClick, CEGUI::Event::Subscriber(&LevelEditor::onLightMouseClick, this));
		m_lightRadioButton->setGroupID(GROUP_ID);
		m_widgetLabels.emplace_back(m_lightRadioButton, "Light", TEXT_SCALE);

		m_objectMode = ObjectMode::PLAYER;
	}

	{ // Add the physics mode radio buttons as well as rotation and size spinner
		const float X_POS = 0.02f;
		const float Y_POS = 0.28f;
		const float DIMS_PIXELS = 20.0f;
		const float PADDING = 0.04f;
		const float TEXT_SCALE = 0.7f;
		const int GROUP_ID = 2;
		m_rigidRadioButton = static_cast<CEGUI::RadioButton*>(m_GUI.createWidget("GWEN/RadioButton", glm::vec4(X_POS, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS), "RigidButton"));
		m_rigidRadioButton->setSelected(true);
		m_rigidRadioButton->subscribeEvent(CEGUI::RadioButton::EventMouseClick, CEGUI::Event::Subscriber(&LevelEditor::onRigidMouseClick, this));
		m_rigidRadioButton->setGroupID(GROUP_ID);
		m_widgetLabels.emplace_back(m_rigidRadioButton, "Rigid", TEXT_SCALE);

		m_dynamicRadioButton = static_cast<CEGUI::RadioButton*>(m_GUI.createWidget("GWEN/RadioButton", glm::vec4(X_POS + PADDING, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS), "DynamicButton"));
		m_dynamicRadioButton->setSelected(false);
		m_dynamicRadioButton->subscribeEvent(CEGUI::RadioButton::EventMouseClick, CEGUI::Event::Subscriber(&LevelEditor::onDynamicMouseClick, this));
		m_dynamicRadioButton->setGroupID(GROUP_ID);
		m_widgetLabels.emplace_back(m_dynamicRadioButton, "Dynamic", TEXT_SCALE);

		// Rotation spinner
		m_rotationSpinner = static_cast<CEGUI::Spinner*>(m_GUI.createWidget("GWEN/Spinner", glm::vec4(X_POS + PADDING * 2.0, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS * 5, DIMS_PIXELS * 2), "RotationSpinner"));
		m_rotationSpinner->setMinimumValue(0.0);
		m_rotationSpinner->setMaximumValue(M_PI * 2.0);
		m_rotationSpinner->setCurrentValue(m_rotation);
		m_rotationSpinner->setStepSize(0.01);
		m_rotationSpinner->setTextInputMode(CEGUI::Spinner::FloatingPoint);
		m_rotationSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&LevelEditor::onRotationValueChange, this));
		m_widgetLabels.emplace_back(m_rotationSpinner, "Rotation", TEXT_SCALE);

		// Light size
		m_sizeSpinner = static_cast<CEGUI::Spinner*>(m_GUI.createWidget("GWEN/Spinner", glm::vec4(X_POS + PADDING * 2.0, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS * 5, DIMS_PIXELS * 2), "SizeSpinner"));
		m_sizeSpinner->setMinimumValue(0.0);
		m_sizeSpinner->setMaximumValue(1000.0);
		m_sizeSpinner->setCurrentValue(m_lightSize);
		m_sizeSpinner->setStepSize(0.1);
		m_sizeSpinner->setTextInputMode(CEGUI::Spinner::FloatingPoint);
		m_sizeSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&LevelEditor::onSizeValueChange, this));
		m_widgetLabels.emplace_back(m_sizeSpinner, "Size", TEXT_SCALE);

		m_physicsMode = PhysicsMode::RIGID;
	}

	{ // Add platform dimension spinners
		const float X_POS = 0.02f;
		const float Y_POS = 0.35f;
		const float DIMS_PIXELS = 20.0f;
		const float PADDING = 0.04f;
		const float TEXT_SCALE = 0.7f;
		m_widthSpinner = static_cast<CEGUI::Spinner*>(m_GUI.createWidget("GWEN/Spinner", glm::vec4(X_POS, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS * 5, DIMS_PIXELS * 2), "WidthSpinner"));
		m_widthSpinner->setMinimumValue(0.0);
		m_widthSpinner->setMaximumValue(10000.0);
		m_widthSpinner->setCurrentValue(m_tileDims.x);
		m_widthSpinner->setStepSize(1.0);
		m_widthSpinner->setTextInputMode(CEGUI::Spinner::FloatingPoint);
		m_widthSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&LevelEditor::onWidthValueChange, this));
		m_widgetLabels.emplace_back(m_widthSpinner, "Width", TEXT_SCALE);

		m_heightSpinner = static_cast<CEGUI::Spinner*>(m_GUI.createWidget("GWEN/Spinner", glm::vec4(X_POS + PADDING * 2.0, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS * 5, DIMS_PIXELS * 2), "HeightSpinner"));
		m_heightSpinner->setMinimumValue(0.0);
		m_heightSpinner->setMaximumValue(10000.0);
		m_heightSpinner->setCurrentValue(m_tileDims.y);
		m_heightSpinner->setStepSize(1.0);
		m_heightSpinner->setTextInputMode(CEGUI::Spinner::FloatingPoint);
		m_heightSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&LevelEditor::onHeightValueChange, this));
		m_widgetLabels.emplace_back(m_heightSpinner, "Height", TEXT_SCALE);
	}

	{ // Add Selection mode radio buttons and debug render toggle
		const float X_POS = 0.03f;
		const float Y_POS = 0.45f;
		const float DIMS_PIXELS = 20.0f;
		const float PADDING = 0.04f;
		const float TEXT_SCALE = 0.7f;
		const int GROUP_ID = 3;
		m_selectRadioButton = static_cast<CEGUI::RadioButton*>(m_GUI.createWidget("GWEN/RadioButton", glm::vec4(X_POS, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS), "SelectButton"));
		m_selectRadioButton->setSelected(true);
		m_selectRadioButton->subscribeEvent(CEGUI::RadioButton::EventMouseClick, CEGUI::Event::Subscriber(&LevelEditor::onSelectMouseClick, this));
		m_selectRadioButton->setGroupID(GROUP_ID);
		m_widgetLabels.emplace_back(m_selectRadioButton, "Select", TEXT_SCALE);

		m_placeRadioButton = static_cast<CEGUI::RadioButton*>(m_GUI.createWidget("GWEN/RadioButton", glm::vec4(X_POS + PADDING, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS), "PlaceButton"));
		m_placeRadioButton->setSelected(false);
		m_placeRadioButton->subscribeEvent(CEGUI::RadioButton::EventMouseClick, CEGUI::Event::Subscriber(&LevelEditor::onPlaceMouseClick, this));
		m_placeRadioButton->setGroupID(GROUP_ID);
		m_widgetLabels.emplace_back(m_placeRadioButton, "Place", TEXT_SCALE);

		m_debugToggle = static_cast<CEGUI::ToggleButton*>(m_GUI.createWidget("GWEN/Checkbox", glm::vec4(X_POS + PADDING * 2.5, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS), "DebugToggle"));
		m_debugToggle->setSelected(false);
		m_debugToggle->subscribeEvent(CEGUI::ToggleButton::EventMouseClick, CEGUI::Event::Subscriber(&LevelEditor::onDebugToggleClick, this));
		m_widgetLabels.emplace_back(m_debugToggle, "Debug", TEXT_SCALE);
		m_debugRender = false;

		m_selectMode = SelectionMode::SELECT;
	}

	{ // Add save and back buttons
		m_saveButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("GWEN/Button", glm::vec4(0.03f, 0.5f, 0.1f, 0.05f), glm::vec4(0.0f), "SaveButton"));
		m_saveButton->setText("Save");
		m_saveButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&LevelEditor::onSaveMouseClick, this));

		m_saveButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("GWEN/Button", glm::vec4(0.03f, 0.57f, 0.1f, 0.05f), glm::vec4(0.0f), "LoadButton"));
		m_saveButton->setText("Load");
		m_saveButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&LevelEditor::onLoadMouseClick, this));

		m_backButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget("GWEN/Button", glm::vec4(0.03f, 0.64f, 0.1f, 0.05f), glm::vec4(0.0f), "BackButton"));
		m_backButton->setText("Back");
		m_backButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&LevelEditor::onBackMouseClick, this));
	}

	{ // Add save window widgets
		m_saveWindow = static_cast<CEGUI::FrameWindow*>(m_GUI.createWidget("GWEN/FrameWindow", glm::vec4(0.3f, 0.3f, 0.4f, 0.4f), glm::vec4(0.0f), "SaveWindow"));
		m_saveWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&LevelEditor::onSaveCancelClick, this));
		m_saveWindow->setText("Save Level");
		// Don't let user drag window around
		m_saveWindow->setDragMovingEnabled(false);

		// Children of saveWindow
		m_saveWindowCombobox = static_cast<CEGUI::Combobox*>(m_GUI.createWidget(m_saveWindow, "GWEN/Combobox", glm::vec4(0.1f, 0.1f, 0.8f, 0.4f), glm::vec4(0.0f), "SaveCombobox"));
		m_saveWindowSaveButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_saveWindow, "GWEN/Button", glm::vec4(0.35f, 0.8f, 0.3f, 0.1f), glm::vec4(0.0f), "SaveCancelButton"));
		m_saveWindowSaveButton->setText("Save");
		m_saveWindowSaveButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&LevelEditor::onSave, this));

		// Start disabled
		m_saveWindow->setAlpha(0.0f);
		m_saveWindow->disable();
	}

	{ // Add load window widgets
		m_loadWindow = static_cast<CEGUI::FrameWindow*>(m_GUI.createWidget("GWEN/FrameWindow", glm::vec4(0.3f, 0.3f, 0.4f, 0.4f), glm::vec4(0.0f), "LoadWindow"));
		m_loadWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&LevelEditor::onLoadCancelClick, this));
		m_loadWindow->setText("Load Level");
		// Don't let user drag window around
		m_loadWindow->setDragMovingEnabled(false);

		// Children of loadWindow
		m_loadWindowCombobox = static_cast<CEGUI::Combobox*>(m_GUI.createWidget(m_loadWindow, "GWEN/Combobox", glm::vec4(0.1f, 0.1f, 0.8f, 0.4f), glm::vec4(0.0f), "LoadCombobox"));
		m_loadWindowLoadButton = static_cast<CEGUI::PushButton*>(m_GUI.createWidget(m_loadWindow, "GWEN/Button", glm::vec4(0.35f, 0.8f, 0.3f, 0.1f), glm::vec4(0.0f), "LoadCancelButton"));
		m_loadWindowLoadButton->setText("Load");
		m_loadWindowLoadButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&LevelEditor::onLoad, this));

		// Start disabled
		m_loadWindow->setAlpha(0.0f);
		m_loadWindow->disable();
	}

	setLightWidgetVisibility(false);
	setPlatformWidgetVisibility(false);
}

void LevelEditor::drawUI() {
	// Outlines
	if (m_selectMode == SelectionMode::PLACE && !isMouseInUI()) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		glm::vec2 pos = m_camera.convertScreentoWorld(glm::vec2(x, y));
		// Draw the object placement outlines
		if (m_objectMode == ObjectMode::PLATFORM) {
			m_outlineRenderer.drawBox(glm::vec4(pos - m_tileDims * 0.5f, m_tileDims), ColorRGBA(255, 255, 255, 200), m_rotation);
			m_outlineRenderer.end();
			m_outlineRenderer.render(m_camera.getcamMatrix(), 2.0f);
		}
		else if (m_objectMode == ObjectMode::LIGHT) {
			// Make temporary light to render
			Light tmpLight(ColorRGBA((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, (GLubyte)m_colorPickerAlpha), pos, glm::vec2(m_lightSize));

			// Draw light
			m_lightProgram.use();
			m_camera.updateUniform(&m_textureProgram, "P");

			m_lightEngine.renderLight(tmpLight);
			m_lightProgram.unuse();

			// Selection radius
			m_outlineRenderer.drawCircle(pos, ColorRGBA(255, 255, 255, 255), LIGHT_SELECT_RADIUS);
			// Outer radius
			m_outlineRenderer.drawCircle(pos, tmpLight.color, m_lightSize);
			m_outlineRenderer.end();
			m_outlineRenderer.render(m_camera.getcamMatrix(), 2.0f);
		}
	}
	else {
		// Draw selected light
		if (m_selectedLight != NO_LIGHT) {
			// Selection radius
			m_outlineRenderer.drawCircle(m_lights[m_selectedLight].pos, ColorRGBA(255, 255, 0, 255), LIGHT_SELECT_RADIUS);
			// Outer radius
			m_outlineRenderer.drawCircle(m_lights[m_selectedLight].pos, m_lights[m_selectedLight].color, m_lights[m_selectedLight].size.x);
			m_outlineRenderer.end();
			m_outlineRenderer.render(m_camera.getcamMatrix(), 2.0f);
		}
		else {
			// Draw selected tile

			if (m_selectedTile != NO_TILE) {
				const Tile& b = m_tiles[m_selectedTile];
				glm::vec4 destRect;
				destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
				destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
				destRect.z = b.getDimensions().x;
				destRect.w = b.getDimensions().y;

				m_outlineRenderer.drawBox(destRect, ColorRGBA(255, 255, 0, 255), b.getBody()->GetAngle());

				m_outlineRenderer.end();
				m_outlineRenderer.render(m_camera.getcamMatrix(), 2.0f);
			}
		}
	}
	//Enable the shader.
	m_textureProgram.use("mySampler");
	m_camera.updateUniform(&m_textureProgram, "P");

	m_spriteBatch.begin();

	{ // Draw the color picker quad
		const float QUAD_SIZE = 75.0f;

		glm::vec4 destRect;
		// Sorry for this
		destRect.x = m_aSlider->getXPosition().d_scale * m_window->getScreenWidth() + 10.0f - m_window->getScreenWidth() / 2.0f + QUAD_SIZE / 2.0f;
		destRect.y = m_window->getScreenHeight() / 2.0f - m_aSlider->getYPosition().d_scale * m_window->getScreenHeight() -
			m_aSlider->getHeight().d_scale * m_window->getScreenHeight() * 0.5f - QUAD_SIZE / 4.0f;
		destRect.z = QUAD_SIZE;
		destRect.w = QUAD_SIZE;
		m_spriteBatch.draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_blankTexture.ID, 0.0f, ColorRGBA((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, 255));

		// Draw color text
		std::string colorText;
		if (m_objectMode == ObjectMode::LIGHT) {
			colorText = "RGBA [ " + std::to_string((int)m_colorPickerRed) + " , " + std::to_string((int)m_colorPickerGreen) + " , " + std::to_string((int)m_colorPickerBlue) + " , " + std::to_string((int)m_colorPickerAlpha) + " ]";
		}
		else {
			colorText = "RGB [ " + std::to_string((int)m_colorPickerRed) + " , " + std::to_string((int)m_colorPickerGreen) + " , " + std::to_string((int)m_colorPickerBlue) + " ]";
		}
		m_spriteFont.draw(m_spriteBatch, colorText.c_str(), glm::vec2(destRect.x + destRect.z * 0.5f, destRect.y + destRect.w), glm::vec2(0.55f), 0.0f, ColorRGBA(255, 255, 255, 255), Justification::MIDDLE);
	}

	// Draw custom labels for widgets
	for (auto& l : m_widgetLabels) l.draw(m_spriteBatch, m_spriteFont, m_window);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();
	m_GUI.draw();
}

void LevelEditor::drawWorld() {
	//Endable the shader.
	m_textureProgram.use("mySampler");
	m_camera.updateUniform(&m_textureProgram, "P");


	{ // Draw all the tiles
		m_spriteBatch.begin();
		for (auto& b : m_tiles) {
			b.draw(m_spriteBatch);
		}

		m_spriteBatch.end();
		m_spriteBatch.renderBatch();
		m_textureProgram.unuse();
	}

	{ // Draw lights
		
		m_lightProgram.use();
		m_spriteBatch.begin();

		m_camera.updateUniform(&m_textureProgram, "P");
		m_lightEngine.renderAllLights();

		m_spriteBatch.end();
		m_spriteBatch.renderBatch();

		m_lightProgram.unuse();

	}

	// Debug rendering
	if (m_debugRender) {
		// Render dynamic tile borders
		glm::vec4 destRect;
		for (auto& b : m_tiles) {
			destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
			destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
			destRect.z = b.getDimensions().x;
			destRect.w = b.getDimensions().y;
			// Dynamic is green, static is red.
			if (b.isDynamic()) {
				m_outlineRenderer.drawBox(destRect, ColorRGBA(0, 255, 0, 255), b.getBody()->GetAngle());
			}
			else {
				m_outlineRenderer.drawBox(destRect, ColorRGBA(255, 0, 0, 255), b.getBody()->GetAngle());
			}
		}

		// Render magenta light selection radius
		for (auto& l : m_lights) {
			m_outlineRenderer.drawCircle(l.pos, ColorRGBA(255, 0, 255, 255), LIGHT_SELECT_RADIUS);
		}
		// Draw axis
		// +X axis
		m_outlineRenderer.drawLine(glm::vec2(0.0f), glm::vec2(100000.0f, 0.0f), ColorRGBA(255, 255, 255, 200));
		// -X axis
		m_outlineRenderer.drawLine(glm::vec2(0.0f), glm::vec2(-100000.0f, 0.0f), ColorRGBA(255, 255, 255, 100));
		// +Y axis
		m_outlineRenderer.drawLine(glm::vec2(0.0f), glm::vec2(0.0f, 100000.0f), ColorRGBA(255, 255, 255, 200));
		// -Y axis
		m_outlineRenderer.drawLine(glm::vec2(0.0f), glm::vec2(0.0f, -100000.0f), ColorRGBA(255, 255, 255, 100));

		m_outlineRenderer.end();
		m_outlineRenderer.render(m_camera.getcamMatrix(), 2.0f);
	}
}

void LevelEditor::setPlatformWidgetVisibility(bool visible)
{
	m_rigidRadioButton->setVisible(visible);
	m_dynamicRadioButton->setVisible(visible);
	m_rotationSpinner->setVisible(visible);
	m_widthSpinner->setVisible(visible);
	m_heightSpinner->setVisible(visible);
}

void LevelEditor::setLightWidgetVisibility(bool visible)
{
	m_aSlider->setVisible(visible);
	m_sizeSpinner->setVisible(visible);
}

void LevelEditor::refreshSelectedTile()
{
	if (m_selectedTile == NO_TILE) return;
	refreshSelectedTile(m_tiles[m_selectedTile].getPosition());
}

void LevelEditor::refreshSelectedTile(const glm::vec2 & newPosition)
{
	if (m_selectedTile == NO_TILE) return;
	// Texture for tiles. Its here because lazy.
	static GLTexture texture = ResourceManager::getTexture("Textures/red_bricks.png.png");
	Tile newTile;

	newTile.init(m_world.get(), newPosition, m_tileDims, texture, ColorRGBA((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, 255),
		false, m_physicsMode == PhysicsMode::DYNAMIC, m_rotation);
	// Destroy old tile and replace with new one
	m_tiles[m_selectedTile].destroy(m_world.get());
	m_tiles[m_selectedTile] = newTile;
}

void LevelEditor::refreshSelectedLight()
{
	if (m_selectedLight == NO_LIGHT) return;
	refreshSelectedTile(m_lights[m_selectedLight].pos);
}

void LevelEditor::refreshSelectedLight(const glm::vec2 & newPosition)
{
	if (m_selectedLight == NO_LIGHT) return;
	Light newLight(ColorRGBA((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, (GLubyte)m_colorPickerAlpha),newPosition, glm::vec2(m_lightSize));
	m_lights[m_selectedLight] = newLight;
}

bool LevelEditor::onExitClicked(const CEGUI::EventArgs & e)
{
	m_game->stopGame();
	return true;
}

bool LevelEditor::onColorPickerRedChanged(const CEGUI::EventArgs & e)
{
	m_colorPickerRed = m_rSlider->getCurrentValue();
	refreshSelectedTile();
	refreshSelectedLight();
	return true;
}

bool LevelEditor::onColorPickerGreenChanged(const CEGUI::EventArgs & e)
{
	m_colorPickerGreen = m_gSlider->getCurrentValue();
	refreshSelectedTile();
	refreshSelectedLight();
	return true;
}

bool LevelEditor::onColorPickerBlueChanged(const CEGUI::EventArgs & e)
{
	m_colorPickerBlue = m_bSlider->getCurrentValue();
	refreshSelectedTile();
	refreshSelectedLight();
	return true;
}

bool LevelEditor::onColorPickerAlphaChanged(const CEGUI::EventArgs & e)
{
	m_colorPickerAlpha = m_aSlider->getCurrentValue();
	refreshSelectedTile();
	refreshSelectedLight();
	return true;
}

bool LevelEditor::onRigidMouseClick(const CEGUI::EventArgs & e)
{
	m_physicsMode = PhysicsMode::RIGID;
	refreshSelectedTile();
	return true;
}

bool LevelEditor::onDynamicMouseClick(const CEGUI::EventArgs & e)
{
	m_physicsMode = PhysicsMode::DYNAMIC;
	refreshSelectedTile();
	return true;
}

bool LevelEditor::onPlayerMouseClick(const CEGUI::EventArgs & e)
{
	m_objectMode = ObjectMode::PLAYER;
	setPlatformWidgetVisibility(false);
	setLightWidgetVisibility(false);
	return true;
}

bool LevelEditor::onPlatformMouseClick(const CEGUI::EventArgs & e)
{
	m_objectMode = ObjectMode::PLATFORM;
	setPlatformWidgetVisibility(true);
	setLightWidgetVisibility(false);
	return true;
}

bool LevelEditor::onLightMouseClick(const CEGUI::EventArgs & e)
{
	m_objectMode = ObjectMode::LIGHT;
	setPlatformWidgetVisibility(false);
	setLightWidgetVisibility(true);
	return true;
}

bool LevelEditor::onSelectMouseClick(const CEGUI::EventArgs & e)
{
	m_selectMode = SelectionMode::SELECT;
	return true;
}

bool LevelEditor::onPlaceMouseClick(const CEGUI::EventArgs & e)
{
	m_selectMode = SelectionMode::PLACE;
	m_selectedTile = NO_TILE;
	m_selectedLight = NO_LIGHT;
	if (m_objectMode == ObjectMode::LIGHT) {
		setLightWidgetVisibility(true);
	}
	else if (m_objectMode == ObjectMode::PLATFORM) {
		setPlatformWidgetVisibility(true);
	}
	return true;
}

bool LevelEditor::onSaveMouseClick(const CEGUI::EventArgs & e)
{
	return true;
}

bool LevelEditor::onLoadMouseClick(const CEGUI::EventArgs & e)
{
	return true;
}

bool LevelEditor::onBackMouseClick(const CEGUI::EventArgs & e)
{
	m_currState = ScreenState::CHANGE_TO_PARTICULAR;
	m_changeToParticular = 0;
	return true;
}

bool LevelEditor::onRotationValueChange(const CEGUI::EventArgs & e)
{
	m_rotation = (float)m_rotationSpinner->getCurrentValue();
	refreshSelectedTile();
	return true;
}

bool LevelEditor::onSizeValueChange(const CEGUI::EventArgs & e)
{
	m_lightSize = (float)m_sizeSpinner->getCurrentValue();
	refreshSelectedLight();
	return true;
}

bool LevelEditor::onWidthValueChange(const CEGUI::EventArgs & e)
{
	m_tileDims.x = (float)m_widthSpinner->getCurrentValue();
	refreshSelectedTile();
	return true;
}

bool LevelEditor::onHeightValueChange(const CEGUI::EventArgs & e)
{
	m_tileDims.y = (float)m_heightSpinner->getCurrentValue();
	refreshSelectedTile();
	return true;
}

bool LevelEditor::onDebugToggleClick(const CEGUI::EventArgs & e)
{
	m_debugRender = m_debugToggle->isSelected();
	return true;
}

bool LevelEditor::onSaveCancelClick(const CEGUI::EventArgs & e)
{
	m_saveWindow->disable();
	m_saveWindow->setAlpha(0.0f);
	return true;
}

bool LevelEditor::onSave(const CEGUI::EventArgs & e)
{
	return true;
}

bool LevelEditor::onLoadCancelClick(const CEGUI::EventArgs & e)
{
	m_loadWindow->disable();
	m_loadWindow->setAlpha(0.0f);
	return true;
}

bool LevelEditor::onLoad(const CEGUI::EventArgs & e)
{
	return true;
}

bool LevelEditor::isMouseInUI()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	const float SW = (float)m_window->getScreenWidth();
	const float SH = (float)m_window->getScreenHeight();
	// First check save window
	if (!m_saveWindow->isDisabled() &&
		x >= m_saveWindow->getXPosition().d_scale * SW && x <= m_saveWindow->getXPosition().d_scale * SW + m_saveWindow->getWidth().d_scale  * SW &&
		y >= m_saveWindow->getYPosition().d_scale * SH && y <= m_saveWindow->getYPosition().d_scale * SH + m_saveWindow->getHeight().d_scale * SH) return true;

	// Notice we aren't converting to world space, we are staying in screen space because UI.
	return (x >= m_groupBox->getXPosition().d_scale * SW && x <= m_groupBox->getXPosition().d_scale * SW + m_groupBox->getWidth().d_scale  * SW &&
		y >= m_groupBox->getYPosition().d_scale * SH && y <= m_groupBox->getYPosition().d_scale * SH + m_groupBox->getHeight().d_scale * SH);

}

}
