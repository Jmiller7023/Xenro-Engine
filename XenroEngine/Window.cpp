#include "Window.h"
#include "ErrorMessages.h"
#include "SaveManager.h"
#include "LoadManager.h"
#include <iostream>

namespace Xenro{

Window::Window()
{
	m_loadManager = new LoadManager;
	m_saveManager = new SaveManager("Init/initWindow.ini");
	if (m_loadManager->fileExists("Init/initWindow.ini")) {
		m_loadManager->loadData("Init/initWindow.ini");
		m_cachedFile = m_loadManager->getLoadData();
		m_saveManager->replaceFile(m_cachedFile);
		loadValues();
	}
	else {
		createDefaultIni();
	}
}


Window::~Window()
{
	delete m_loadManager;
	delete m_saveManager;
}

int Window::create() {

	Uint32 flags = SDL_WINDOW_OPENGL;

	//Set up flags.
	if (m_hidden) {
		flags |= SDL_WINDOW_HIDDEN;
	}
	if (m_fullscreen) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	if (m_borderless) {
		flags |= SDL_WINDOW_BORDERLESS;
	}
	if (m_resizable) {
		flags |= SDL_WINDOW_RESIZABLE;
	}
	if (m_maximized) {
		flags |= SDL_WINDOW_MAXIMIZED;
	}
	if (m_minimized) {
		flags |= SDL_WINDOW_MINIMIZED;
	}

	//Open an SDL window.
	m_sdlWindow = SDL_CreateWindow(m_windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, flags);

	if (m_sdlWindow == nullptr) {
		fatalError("Failed to create SDL window!");
	}

	//Set up OpenGL context.
	SDL_GLContext GLcontext = SDL_GL_CreateContext(m_sdlWindow);

	if (GLcontext == nullptr) {
		fatalError("GL context could not be created!");
	}

	//set up glew
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Failed to initialize Glew!");
	}

	//Check the OpenGL version.
	std::printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));

	//Set window color to black.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Set VSYNC: 0 turns it off and 1 turns it on.
	SDL_GL_SetSwapInterval(m_vsync);

	//Enable alpha blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Window::swapBuffer() {

	//Swap our buffer and draw everything to the screen.
	SDL_GL_SwapWindow(m_sdlWindow);
}

void Window::createDefaultIni() {

	m_saveManager->addEntry(";Do not delete any entries in this ini file!");

	m_saveManager->addEntry("ScreenWidth:");
	m_saveManager->addEntry("1024");

	m_saveManager->addEntry("ScreenHeight:");
	m_saveManager->addEntry("768");

	m_saveManager->addEntry("VSYNC:");
	m_saveManager->addEntry("true");

	m_saveManager->addEntry(";Window flags");

	m_saveManager->addEntry("windowHidden:");
	m_saveManager->addEntry("false");

	m_saveManager->addEntry("windowFullscreen:");
	m_saveManager->addEntry("false");

	m_saveManager->addEntry("windowBorderless:");
	m_saveManager->addEntry("false");

	m_saveManager->addEntry("windowResizable:");
	m_saveManager->addEntry("false");

	m_saveManager->addEntry("windowMinimized:");
	m_saveManager->addEntry("false");

	m_saveManager->addEntry("windowMaximized:");
	m_saveManager->addEntry("false");

	m_saveManager->addEntry("windowName:");
	m_saveManager->addEntry("Default");

	m_saveManager->saveFiletoDir();
	m_screenWidth = 1024;
	m_screenHeight = 768;
	m_vsync = 1;
	m_borderless = false;
	m_fullscreen = false;
	m_hidden = false;
	m_maximized = false;
	m_minimized = false;
	m_resizable = false;
	m_windowName = "Default";
	
}

 
 
 
 

void Window::loadValues() {
	
	//ignores all lines that begin with ; to allow for commenting.
	std::vector<std::string> processedInput;
	for (size_t i = 0; i < m_cachedFile.size(); i++) {
		if (m_cachedFile[i][0] != ';' && m_cachedFile[i] != "") {
			processedInput.push_back(m_cachedFile[i]);
		}
	}

	//Flags
	bool nameModified = false;
	bool widthModified = false;
	bool heightModified = false;
	bool vsyncModified = false;
	bool hiddenModified = false;
	bool fullscreenModified = false;
	bool borderlessModified = false;
	bool resizableModified = false;
	bool minimizedModified = false;
	bool maximizedModified = false;

	//Initialize all variables.
	size_t max = processedInput.size();
	for (size_t i = 0; i < max; i++) {

		if (processedInput[i] == "windowName:" && i + 1 < max) {
			m_windowName = processedInput[i + 1];
			nameModified = true;
			continue;
		}

		if (processedInput[i] == "ScreenWidth:" && i + 1 < max) {
			m_screenWidth = std::stoi(processedInput[i + 1]);
			widthModified = true;
			continue;
		}

		if (processedInput[i] == "ScreenHeight:" && i + 1 < max) {
			m_screenHeight = std::stoi(processedInput[i + 1]);
			heightModified = true;
			continue;
		}

		if (processedInput[i] == "VSYNC:" && i + 1 < max) {
			if (processedInput[i + 1] == "true") 
				m_vsync = 1;
			else 
				m_vsync = 0;
			vsyncModified = true;
			continue;
		}

		if (processedInput[i] == "windowHidden:" && i + 1 < max) {
			if (processedInput[i + 1] == "true")
				m_hidden = true;
			else
				m_hidden = false;
			hiddenModified = true;
			continue;
		}

		if (processedInput[i] == "windowFullscreen:" && i + 1 < max) {
			if (processedInput[i + 1] == "true")
				m_fullscreen = true;
			else
				m_fullscreen = false;
			fullscreenModified = true;
			continue;
		}

		if (processedInput[i] == "windowBorderless:" && i + 1 < max) {
			if (processedInput[i + 1] == "true")
				m_borderless = true;
			else
				m_borderless = false;
			borderlessModified = true;
			continue;
		}

		if (processedInput[i] == "windowResizable:" && i + 1 < max) {
			if (processedInput[i + 1] == "true")
				m_resizable = true;
			else
				m_resizable = false;
			resizableModified = true;
			continue;
		}

		if (processedInput[i] == "windowMinimized:" && i + 1 < max) {
			if (processedInput[i + 1] == "true")
				m_minimized = true;
			else
				m_minimized = false;
			minimizedModified = true;
			continue;
		}

		if (processedInput[i] == "windowMaximized:" && i + 1 < max) {
			if (processedInput[i + 1] == "true")
				m_maximized = true;
			else
				m_maximized = false;
			maximizedModified = true;
			continue;
		}
	}

	//If any object was left unmodified.
	if (!(nameModified && widthModified && heightModified &&
		vsyncModified && hiddenModified && fullscreenModified &&
		borderlessModified && resizableModified && minimizedModified &&
		maximizedModified)) {
		fatalError("Problem in Init/initWindow.ini! delete file to obtain new one.");
	}
}

void Window::setInitScreenName(const std::string& name) {

	modifyInit("windowName:", name);
}

void Window::setInitScreenWidth(int screenWidth) {

	modifyInit("ScreenWidth:", std::to_string(screenWidth));
}

void Window::setInitScreenHeight(int screenHeight) {

	modifyInit("ScreenHeight:", std::to_string(screenHeight));
}

void Window::setInitHidden(bool state) {
	
	if (state) 
		modifyInit("windowHidden:", "true");
	else
		modifyInit("windowHidden:", "false");
}

void Window::setInitFullscreen(bool state) {

	if (state)
		modifyInit("windowFullscreen:", "true");
	else
		modifyInit("windowFullscreen:", "false");
}

void Window::setInitBorderless(bool state) {

	if (state)
		modifyInit("windowBorderless:", "true");
	else
		modifyInit("windowBorderless:", "false");
}

void Window::setInitResizable(bool state){

	if (state)
		modifyInit("windowResizable:", "true");
	else
		modifyInit("windowResizable:", "false");
}

void Window::setInitMaximized(bool state) {

	if (state)
		modifyInit("windowMaximized:", "true");
	else
		modifyInit("windowMaximized:", "false");
}

void Window::setInitMinimized(bool state) {

	if (state)
		modifyInit("windowMinimized:", "true");
	else
		modifyInit("windowMinimized:", "false");
}

void Window::setInitVSYNC(bool state) {

	if (state)
		modifyInit("VSYNC:", "true");
	else
		modifyInit("VSYNC:", "false");
}

void Window::modifyInit(const std::string& line, const std::string& text) {

	for (size_t i = 0; i < m_cachedFile.size(); i++) {
		if (m_cachedFile[i] == line) {
			m_saveManager->replaceEntry(text, i + 1);
			m_saveManager->saveFiletoDir();
			return;
		}
	}

	//screenWidth was renamed or deleted from .ini
	fatalError("Initialization File invalid! Missing parameter " + line);
}

}