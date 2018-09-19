/*************************************************************************/
/*                             GUI.cpp                                   */
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

#include "GUI.h"
#include "Window.h"
#include <SDL/SDL_timer.h>
#include "InputManager.h"
#include "utf8.h"

namespace Xenro {

CEGUI::Key::Scan SDLKeyToCEGUIKey(SDL_Keycode key) {
	using namespace CEGUI;
	switch (key) {
		case SDLK_BACKSPACE:    return Key::Backspace;
		case SDLK_TAB:          return Key::Tab;
		case SDLK_RETURN:       return Key::Return;
		case SDLK_PAUSE:        return Key::Pause;
		case SDLK_ESCAPE:       return Key::Escape;
		case SDLK_SPACE:        return Key::Space;
		case SDLK_COMMA:        return Key::Comma;
		case SDLK_MINUS:        return Key::Minus;
		case SDLK_PERIOD:       return Key::Period;
		case SDLK_SLASH:        return Key::Slash;
		case SDLK_0:            return Key::Zero;
		case SDLK_1:            return Key::One;
		case SDLK_2:            return Key::Two;
		case SDLK_3:            return Key::Three;
		case SDLK_4:            return Key::Four;
		case SDLK_5:            return Key::Five;
		case SDLK_6:            return Key::Six;
		case SDLK_7:            return Key::Seven;
		case SDLK_8:            return Key::Eight;
		case SDLK_9:            return Key::Nine;
		case SDLK_COLON:        return Key::Colon;
		case SDLK_SEMICOLON:    return Key::Semicolon;
		case SDLK_EQUALS:       return Key::Equals;
		case SDLK_LEFTBRACKET:  return Key::LeftBracket;
		case SDLK_BACKSLASH:    return Key::Backslash;
		case SDLK_RIGHTBRACKET: return Key::RightBracket;
		case SDLK_a:            return Key::A;
		case SDLK_b:            return Key::B;
		case SDLK_c:            return Key::C;
		case SDLK_d:            return Key::D;
		case SDLK_e:            return Key::E;
		case SDLK_f:            return Key::F;
		case SDLK_g:            return Key::G;
		case SDLK_h:            return Key::H;
		case SDLK_i:            return Key::I;
		case SDLK_j:            return Key::J;
		case SDLK_k:            return Key::K;
		case SDLK_l:            return Key::L;
		case SDLK_m:            return Key::M;
		case SDLK_n:            return Key::N;
		case SDLK_o:            return Key::O;
		case SDLK_p:            return Key::P;
		case SDLK_q:            return Key::Q;
		case SDLK_r:            return Key::R;
		case SDLK_s:            return Key::S;
		case SDLK_t:            return Key::T;
		case SDLK_u:            return Key::U;
		case SDLK_v:            return Key::V;
		case SDLK_w:            return Key::W;
		case SDLK_x:            return Key::X;
		case SDLK_y:            return Key::Y;
		case SDLK_z:            return Key::Z;
		case SDLK_DELETE:       return Key::Delete;
		case SDLK_KP_PERIOD:    return Key::Decimal;
		case SDLK_KP_DIVIDE:    return Key::Divide;
		case SDLK_KP_MULTIPLY:  return Key::Multiply;
		case SDLK_KP_MINUS:     return Key::Subtract;
		case SDLK_KP_PLUS:      return Key::Add;
		case SDLK_KP_ENTER:     return Key::NumpadEnter;
		case SDLK_KP_EQUALS:    return Key::NumpadEquals;
		case SDLK_UP:           return Key::ArrowUp;
		case SDLK_DOWN:         return Key::ArrowDown;
		case SDLK_RIGHT:        return Key::ArrowRight;
		case SDLK_LEFT:         return Key::ArrowLeft;
		case SDLK_INSERT:       return Key::Insert;
		case SDLK_HOME:         return Key::Home;
		case SDLK_END:          return Key::End;
		case SDLK_PAGEUP:       return Key::PageUp;
		case SDLK_PAGEDOWN:     return Key::PageDown;
		case SDLK_F1:           return Key::F1;
		case SDLK_F2:           return Key::F2;
		case SDLK_F3:           return Key::F3;
		case SDLK_F4:           return Key::F4;
		case SDLK_F5:           return Key::F5;
		case SDLK_F6:           return Key::F6;
		case SDLK_F7:           return Key::F7;
		case SDLK_F8:           return Key::F8;
		case SDLK_F9:           return Key::F9;
		case SDLK_F10:          return Key::F10;
		case SDLK_F11:          return Key::F11;
		case SDLK_F12:          return Key::F12;
		case SDLK_F13:          return Key::F13;
		case SDLK_F14:          return Key::F14;
		case SDLK_F15:          return Key::F15;
		case SDLK_RSHIFT:       return Key::RightShift;
		case SDLK_LSHIFT:       return Key::LeftShift;
		case SDLK_RCTRL:        return Key::RightControl;
		case SDLK_LCTRL:        return Key::LeftControl;
		case SDLK_RALT:         return Key::RightAlt;
		case SDLK_LALT:         return Key::LeftAlt;
		case SDLK_SYSREQ:       return Key::SysRq;
		case SDLK_MENU:         return Key::AppMenu;
		case SDLK_POWER:        return Key::Power;
		default:                return Key::Unknown;
	}
}

CEGUI::MouseButton SDLButtonToCEGUIButton(Uint8 sdlButton) {
	switch (sdlButton) {
		case SDL_BUTTON_LEFT: return CEGUI::MouseButton::LeftButton;
		case SDL_BUTTON_MIDDLE: return CEGUI::MouseButton::MiddleButton;
		case SDL_BUTTON_RIGHT: return CEGUI::MouseButton::RightButton;
		case SDL_BUTTON_X1: return CEGUI::MouseButton::X1Button;
		case SDL_BUTTON_X2: return CEGUI::MouseButton::X2Button;
	}
	return CEGUI::MouseButton::NoButton;
}


CEGUI::OpenGL3Renderer* GUI::m_renderer = nullptr;

GUI::GUI(const std::string& resourceFilePath, Window* window)
{
	m_window = window;
	m_windowSize = glm::vec2(window->getScreenWidth(), window->getScreenHeight());

	//Check and initialize renderer and all the CEGUI subsystems.
	if (m_renderer == nullptr) {
		m_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
	}

	//Get resource provider to point to the resource folders.
	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());

	//Point to all of the resource directories.
	rp->setResourceGroupDirectory("fonts", resourceFilePath + "/fonts/");
	rp->setResourceGroupDirectory("imagesets", resourceFilePath + "/imagesets/");
	rp->setResourceGroupDirectory("layouts", resourceFilePath + "/layouts/");
	rp->setResourceGroupDirectory("looknfeel", resourceFilePath + "/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", resourceFilePath + "/lua_scripts/");
	rp->setResourceGroupDirectory("schemes", resourceFilePath + "/schemes/");

	// set the default resource groups to be used
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	

	//Set Context and Window
	m_context = &CEGUI::System::getSingleton().createGUIContext(m_renderer->getDefaultRenderTarget());
	m_rootWindow = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
	m_context->setRootWindow(m_rootWindow);
}

GUI::~GUI()
{
	CEGUI::System::getSingleton().destroyGUIContext(*m_context);
}

void GUI::draw() {

	//Take into account changes in screenSize.
	if (m_autoscalingEnabled) {
		rescaleGUI(m_window->getScreenWidth(), m_window->getScreenHeight());
	}

	//Begin Rendering.
	glDisable(GL_DEPTH_TEST);
	m_renderer->beginRendering();
	m_context->draw();
	m_renderer->endRendering();

	// Clean up after CEGUI
	glBindVertexArray(0);
	glDisable(GL_SCISSOR_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void GUI::setMouseCursor(const std::string& imageFile) {

	m_context->getMouseCursor().setDefaultImage(imageFile);
}

void GUI::setMousePos(float x, float y) {

	m_context->injectMousePosition(x, y);
}

void GUI::showCursor() {

	m_context->getMouseCursor().show();
}

void GUI::hideCursor() {

	m_context->getMouseCursor().hide();
}

void GUI::mouseClick() {
	m_context->injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
	m_context->injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
}

void GUI::loadScheme(const std::string& scheme) {

	CEGUI::SchemeManager::getSingleton().createFromFile(scheme);
}

void GUI::onEvent(SDL_Event& evnt) {

	CEGUI::utf32 codePoint;
	std::string evntText = std::string(evnt.text.text);
	std::vector<int> utf32result;

	switch (evnt.type) {

		case SDL_MOUSEMOTION:
			m_context->injectMouseMove(evnt.motion.xrel, evnt.motion.yrel);
			m_context->injectMousePosition((float)evnt.motion.x, (float)evnt.motion.y);
			break;

		case SDL_KEYDOWN:
			m_context->injectKeyDown(SDLKeyToCEGUIKey(evnt.key.keysym.sym));
			break;

		case SDL_KEYUP:
			m_context->injectKeyUp(SDLKeyToCEGUIKey(evnt.key.keysym.sym));
			break;

		case SDL_TEXTINPUT:
			//Decode the utf-8 into utf-32.
			utf8::utf8to32(evnt.text.text, evnt.text.text + evntText.size(), std::back_inserter(utf32result));
			codePoint = (CEGUI::utf32)utf32result[0];
			m_context->injectChar(codePoint);
			break;

		case SDL_MOUSEBUTTONDOWN:
			m_context->injectMouseButtonDown(SDLButtonToCEGUIButton(evnt.button.button));
			break;

		case SDL_MOUSEBUTTONUP:
			m_context->injectMouseButtonUp(SDLButtonToCEGUIButton(evnt.button.button));
			break;
	}
}

void GUI::update() {
	unsigned int timeElapsed, currTime;

	if (m_time == 0) {
		timeElapsed = 0;
		m_time = SDL_GetTicks();

	} else {
		currTime = SDL_GetTicks();
		timeElapsed = m_time - currTime;
		m_time = currTime;
	}

	m_context->injectTimePulse((float)timeElapsed / 1000.0f);
}

void GUI::loadFont(const std::string& font) {

	CEGUI::FontManager::getSingleton().createFromFile(font + ".font");

	//Error checking.
	if (m_context->getDefaultFont()->getName() == font) {
		return;
	}
	
	m_context->setDefaultFont(font);
}

CEGUI::Window* GUI::createWidget(const std::string& type, const glm::vec4& drawRectPercent, const glm::vec4& drawRectPixels, const std::string& name) {

	CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);
	m_rootWindow->addChild(newWindow);
	setWidgetDrawRect(newWindow, drawRectPercent, drawRectPixels);
	return newWindow;
}

CEGUI::Window* GUI::createWidget(CEGUI::Window* parent, const std::string& type, const glm::vec4& drawRectPercent, const glm::vec4& drawRectPixels, const std::string& name) {

	CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);
	parent->addChild(newWindow);
	setWidgetDrawRect(newWindow, drawRectPercent, drawRectPixels);
	return newWindow;
}

void GUI::setWidgetDrawRect(CEGUI::Window* widget, const glm::vec4& drawRectPercent, const glm::vec4& drawRectPixels) {

	widget->setPosition(CEGUI::UVector2(CEGUI::UDim(drawRectPercent.x, drawRectPixels.x), CEGUI::UDim(drawRectPercent.y, drawRectPixels.y)));
	widget->setSize(CEGUI::USize(CEGUI::UDim(drawRectPercent.z, drawRectPixels.z), CEGUI::UDim(drawRectPercent.w, drawRectPixels.w)));
}

void GUI::clearGUI() {

	CEGUI::System::getSingleton().destroyGUIContext(*m_context);
}

void GUI::rescaleGUI(int width, int height) {
	if (m_windowSize.x != width || m_windowSize.y != height) {
		//Notify CEGUI system about display change.
		CEGUI::Sizef size;
		size.d_height = (float)height;
		size.d_width = (float)width;
		CEGUI::System::getSingleton().notifyDisplaySizeChanged(size);

		//Update m_GUI to know what the new screen size is.
		m_windowSize.x = (float)width;
		m_windowSize.y = (float)height;
	}
}

}