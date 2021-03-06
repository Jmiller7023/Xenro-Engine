/*************************************************************************/
/*                               GUI.h                                   */
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

#ifndef XENRO_GUI_DEFINED
#define XENRO_GUI_DEFINED

#include <CEGUI/CEGUI.h>
#include<CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <SDL\SDL_events.h>
#include <glm/glm.hpp>

#include <CEGUI/CEGUI.h>

/// Disable logging in CEGUI for Release build
#ifndef _DEBUG
class CeguiNonLogger : public CEGUI::Logger {
	void logEvent(const CEGUI::String&, CEGUI::LoggingLevel) {}
	void setLogFilename(const CEGUI::String&, bool) {}
};

#endif

namespace Xenro{

class Window;

class GUI {
public:
	GUI(const std::string& resourceFilePath, Window* window);
	~GUI();

	void draw();

	void setMouseCursor(const std::string& imageFile);
	void setMousePos(float x, float y);
	void showCursor();
	void hideCursor();
	void mouseClick();

	void onEvent(SDL_Event& evnt);

	void update();

	void loadScheme(const std::string& scheme);
	void loadFont(const std::string& font);
	CEGUI::Window* createWidget(const std::string& type, const glm::vec4& drawRectPercent, const glm::vec4& drawRectPixels, const std::string& name = "");
	CEGUI::Window* GUI::createWidget(CEGUI::Window* parent, const std::string& type, const glm::vec4& drawRectPercent, const glm::vec4& drawRectPixels, const std::string& name);
	static void setWidgetDrawRect(CEGUI::Window* widget, const glm::vec4& drawRectPercent, const glm::vec4& drawRectPixels);

	void clearGUI();

	void setAutoScaling(bool enabled) { m_autoscalingEnabled = enabled; }

	//Getters.
	CEGUI::OpenGL3Renderer* getRenderer() const { return m_renderer; }
	const CEGUI::GUIContext* getContext() const { return m_context; }

private:
	//Functions
	void rescaleGUI(int width, int height);
	//Variables
	static CEGUI::OpenGL3Renderer* m_renderer;
	CEGUI::GUIContext* m_context = nullptr;
	CEGUI::Window* m_rootWindow = nullptr;
	Window* m_window = nullptr;
	bool m_screenResized = false;
	bool m_autoscalingEnabled = true;
	glm::vec2 m_windowSize;
	unsigned int m_time = 0;

	/// Disable logging in CEGUI for Release build
	#ifndef _DEBUG
		CeguiNonLogger m_ceguiNonLogger;
	#endif
};

}

#endif //XENRO_GUI_DEFINED