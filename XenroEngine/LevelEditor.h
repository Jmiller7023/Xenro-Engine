/*************************************************************************/
/*                            LevelEditor.h                              */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Xenro Engine                                */
/*             https://github.com/Jmiller7023/Xenro-Engine               */
/*************************************************************************/
/*                 Copyright  9-20-2018 Joseph Miller.                   */
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

#ifndef XENRO_LEVEL_EDITOR_DEFINED
#define XENRO_LEVEL_EDITOR_DEFINED

#include "IScreen.h"
#include "Camera.h"
#include "SpriteBatch.h"
#include "OutlineRenderer.h"
#include "GUI.h"
#include "GLSLProgram.h"
#include "AudioEngine.h"
#include "LightEngine.h"
#include "InputManager.h"
#include "SpriteFont.h"
#include "GLTexture.h"
#include "Tile.h"

namespace Xenro {

class Window;

enum class PhysicsMode {
	RIGID,
	DYNAMIC
};

enum class ObjectMode {
	PLAYER,
	PLATFORM,
	LIGHT
};

enum class SelectionMode {
	SELECT,
	PLACE
};

const int NO_TILE = -1;
const int NO_LIGHT = -1;

class WidgetLabel {
public:
	WidgetLabel() {};
	WidgetLabel(CEGUI::Window* w, const std::string& text, float scale) :
		widget(w), text(text), scale(scale) {
		// Empty
	};
	void draw(SpriteBatch& sb, SpriteFont& sf, Window* w);

	CEGUI::Window* widget = nullptr;
	std::string text = "";
	ColorRGBA color = ColorRGBA(255, 255, 255, 255);
	float scale = 0.7f;
};

class LevelEditor : public IScreen {

public:
	LevelEditor(Xenro::Window* window);
	virtual ~LevelEditor();

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
	//Member Functions
	void initUI();
	void drawUI();
	void drawWorld();
	void setPlatformWidgetVisibility(bool visible);
	void setLightWidgetVisibility(bool visible);
	void refreshSelectedTile();
	void refreshSelectedTile(const glm::vec2& newPosition);
	void refreshSelectedLight();
	void refreshSelectedLight(const glm::vec2& newPosition);

	/************************************************************************/
	/* Event Handlers                                                       */
	/************************************************************************/
	bool onExitClicked(const CEGUI::EventArgs& e);
	bool onColorPickerRedChanged(const CEGUI::EventArgs& e);
	bool onColorPickerGreenChanged(const CEGUI::EventArgs& e);
	bool onColorPickerBlueChanged(const CEGUI::EventArgs& e);
	bool onColorPickerAlphaChanged(const CEGUI::EventArgs& e);
	bool onRigidMouseClick(const CEGUI::EventArgs& e);
	bool onDynamicMouseClick(const CEGUI::EventArgs& e);
	bool onPlayerMouseClick(const CEGUI::EventArgs& e);
	bool onPlatformMouseClick(const CEGUI::EventArgs& e);
	bool onLightMouseClick(const CEGUI::EventArgs& e);
	bool onSelectMouseClick(const CEGUI::EventArgs& e);
	bool onPlaceMouseClick(const CEGUI::EventArgs& e);
	bool onSaveMouseClick(const CEGUI::EventArgs& e);
	bool onLoadMouseClick(const CEGUI::EventArgs& e);
	bool onBackMouseClick(const CEGUI::EventArgs& e);
	bool onRotationValueChange(const CEGUI::EventArgs& e);
	bool onSizeValueChange(const CEGUI::EventArgs& e);
	bool onWidthValueChange(const CEGUI::EventArgs& e);
	bool onHeightValueChange(const CEGUI::EventArgs& e);
	bool onDebugToggleClick(const CEGUI::EventArgs& e);
	bool onSaveCancelClick(const CEGUI::EventArgs& e);
	bool onSave(const CEGUI::EventArgs& e);
	bool onLoadCancelClick(const CEGUI::EventArgs& e);
	bool onLoad(const CEGUI::EventArgs& e);
	bool isMouseInUI();


	//Member variables
	float m_colorPickerRed = 255.0f;
	float m_colorPickerGreen = 255.0f;
	float m_colorPickerBlue = 255.0f;
	float m_colorPickerAlpha = 255.0f;
	float m_rotation = 0.0f;
	float m_lightSize = 200.0f;
	glm::vec2 m_tileDims = glm::vec2(64.0f);
	bool m_mouseButtons[2];
	bool m_debugRender = false;
	//std::vector<Box> m_tiles;
	std::vector<Light> m_lights;
	std::vector<Tile> m_tiles;
	int m_selectedTile = NO_TILE;
	int m_selectedLight = NO_LIGHT;
	bool m_isDragging = false;
	glm::vec2 m_selectOffset;

	CEGUI::GroupBox* m_groupBox = nullptr;
	CEGUI::Slider* m_rSlider = nullptr;
	CEGUI::Slider* m_gSlider = nullptr;
	CEGUI::Slider* m_bSlider = nullptr;
	CEGUI::Slider* m_aSlider = nullptr;
	CEGUI::RadioButton* m_rigidRadioButton = nullptr;
	CEGUI::RadioButton* m_dynamicRadioButton = nullptr;
	CEGUI::RadioButton* m_playerRadioButton = nullptr;
	CEGUI::RadioButton* m_platformRadioButton = nullptr;
	CEGUI::RadioButton* m_lightRadioButton = nullptr;
	CEGUI::RadioButton* m_selectRadioButton = nullptr;
	CEGUI::RadioButton* m_placeRadioButton = nullptr;
	CEGUI::ToggleButton* m_debugToggle = nullptr;
	CEGUI::Spinner* m_rotationSpinner = nullptr;
	CEGUI::Spinner* m_widthSpinner = nullptr;
	CEGUI::Spinner* m_heightSpinner = nullptr;
	CEGUI::Spinner* m_sizeSpinner = nullptr;
	CEGUI::FrameWindow* m_saveWindow = nullptr;
	CEGUI::PushButton* m_saveWindowSaveButton = nullptr;
	CEGUI::Combobox* m_saveWindowCombobox = nullptr;
	CEGUI::FrameWindow* m_loadWindow = nullptr;
	CEGUI::PushButton* m_loadWindowLoadButton = nullptr;
	CEGUI::Combobox* m_loadWindowCombobox = nullptr;
	std::vector<CEGUI::ListboxTextItem*> m_saveListBoxItems;
	std::vector<CEGUI::ListboxTextItem*> m_loadListBoxItems;
	CEGUI::PushButton* m_saveButton = nullptr;
	CEGUI::PushButton* m_loadButton = nullptr;
	CEGUI::PushButton* m_backButton = nullptr;

	std::vector<WidgetLabel> m_widgetLabels;
	PhysicsMode m_physicsMode = PhysicsMode::RIGID;
	ObjectMode m_objectMode = ObjectMode::PLAYER;
	SelectionMode m_selectMode = SelectionMode::SELECT;

	AudioEngine m_audioEngine;
	LightEngine m_lightEngine;
	GLSLProgram m_lightProgram;
	OutlineRenderer m_outlineRenderer;
	GLSLProgram m_textureProgram;
	GLTexture m_blankTexture;
	SpriteBatch m_spriteBatch;
	Camera m_camera;
	Window* m_window;
	GUI m_GUI;
	SpriteFont m_spriteFont;

	///Temporarily have level editor hold world pointer.
	//TODO: implement all physics in the World Class.
	std::unique_ptr<b2World> m_world;
};

}




#endif //XENRO_LEVEL_EDITOR_DEFINED
