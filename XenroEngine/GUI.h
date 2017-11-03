#ifndef XENRO_GUI_DEFINED
#define XENRO_GUI_DEFINED

#include <CEGUI/CEGUI.h>
#include<CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <SDL\SDL_events.h>
#include <glm/glm.hpp>

namespace Xenro{

class Window;

class GUI
{
public:
	GUI(const std::string& resourceFilePath, Window* window);
	~GUI();

	void draw();

	void setMouseCursor(const std::string& imageFile);
	void setMousePos(float x, float y);
	void showCursor();
	void hideCursor();

	void onEvent(SDL_Event& evnt);

	void update();

	void loadScheme(const std::string& scheme);
	void loadFont(const std::string& font);
	CEGUI::Window* createWidget(const std::string& type, const glm::vec4& drawRectPercent, const glm::vec4& drawRectPixels, const std::string& name = "");
	static void setWidgetDrawRect(CEGUI::Window* widget, const glm::vec4& drawRectPercent, const glm::vec4& drawRectPixels);

	void clearGUI();

	//Getters.
	CEGUI::OpenGL3Renderer* getRenderer() const { return m_renderer; }
	const CEGUI::GUIContext* getContext() const { return m_context; }

private:
	static CEGUI::OpenGL3Renderer* m_renderer;
	CEGUI::GUIContext* m_context = nullptr;
	CEGUI::Window* m_rootWindow = nullptr;
	Window* m_window = nullptr;
	
	unsigned int m_time = 0;
};

}

#endif //XENRO_GUI_DEFINED