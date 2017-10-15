#include "GUI.h"

namespace Xenro{

GUI::GUI(const std::string& resourceFilePath)
{
	//Initialize renderer and all the CEGUI subsystems.
	m_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
	
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

void GUI::loadScheme(const std::string& scheme) {

	CEGUI::SchemeManager::getSingleton().createFromFile(scheme);
}

void GUI::loadFont(const std::string& font) {
	CEGUI::FontManager::getSingleton().createFromFile(font + ".font");
	m_context->setDefaultFont(font);
}

CEGUI::Window* GUI::createWidget(const std::string& type, const glm::vec4& drawRectPercent, const glm::vec4& drawRectPixels, const std::string& name) {

	CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);
	m_rootWindow->addChild(newWindow);
	setWidgetDrawRect(newWindow, drawRectPercent, drawRectPixels);
	return newWindow;
}

void GUI::setWidgetDrawRect(CEGUI::Window* widget, const glm::vec4& drawRectPercent, const glm::vec4& drawRectPixels) {

	widget->setPosition(CEGUI::UVector2(CEGUI::UDim(drawRectPercent.x, drawRectPixels.x), CEGUI::UDim(drawRectPercent.y, drawRectPixels.y)));
	widget->setSize(CEGUI::USize(CEGUI::UDim(drawRectPercent.z, drawRectPixels.z), CEGUI::UDim(drawRectPercent.w, drawRectPixels.w)));
}

}