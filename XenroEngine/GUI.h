#pragma once
#include <CEGUI/CEGUI.h>
#include<CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <glm/glm.hpp>

namespace Xenro{

class GUI
{
public:
	GUI(const std::string& resourceFilePath);
	~GUI();

	void draw();

	void loadScheme(const std::string& scheme);
	void loadFont(const std::string& font);
	CEGUI::Window* createWidget(const std::string& type, const glm::vec4& drawRectPercent, const glm::vec4& drawRectPixels, const std::string& name = "");
	static void setWidgetDrawRect(CEGUI::Window* widget, const glm::vec4& drawRectPercent, const glm::vec4& drawRectPixels);

	//Getters.
	CEGUI::OpenGL3Renderer* getRenderer() const { return m_renderer; }
	const CEGUI::GUIContext* getContext() const { return m_context; }

private:
	CEGUI::OpenGL3Renderer* m_renderer;
	CEGUI::GUIContext* m_context = nullptr;
	CEGUI::Window* m_rootWindow = nullptr;
};

}