#include "Window.h"
#include "ErrorMessages.h"
#include "SaveManager.h"
#include "LoadManager.h"
#include <iostream>

namespace Xenro{

Window::Window()
{
	LoadManager lmanager;
	if (lmanager.fileExists("Init/initWindow.ini")) {
		lmanager.loadData("Init/initWindow.ini");
		std::vector<std::string> setup = lmanager.getLoadData();
		loadValues(setup);
	}
	else {
		createDefaultIni();
	}
}


Window::~Window()
{
	//Empty.
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

	SaveManager smanager;
	smanager.addEntry(";Do not delete any entries in this ini file!");
	smanager.addEntry("ScreenWidth=1024");
	smanager.addEntry("ScreenHeight=768");
	smanager.addEntry("VSYNC=true");
	smanager.addEntry(";Window flags");
	smanager.addEntry("windowHidden=false");
	smanager.addEntry("windowFullscreen=false");
	smanager.addEntry("windowBorderless=false");
	smanager.addEntry("windowResizable=false");
	smanager.addEntry("windowMinimized=false");
	smanager.addEntry("windowMaximized=false");
	smanager.addEntry("windowName=Burning Sabyr");
	smanager.saveFiletoDir("Init/initWindow.ini");
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

void Window::loadValues(const std::vector<std::string>& setup) {
	
	//ignores all lines that begin with ; to allow for commenting.
	std::vector<std::string> processedInput;
	for (size_t i = 0; i < setup.size(); i++) {
		if (setup[i][0] != ';') {
			processedInput.push_back(setup[i]);
		}
	}
	std::string screenWidth, screenHeight, VSYNC, windowHidden, windowFullscreen, windowBorderless,
		windowResizable, windowMinimized, windowMaximized, windowName;

	if (processedInput.size() != 10) {
		fatalError("Error in initWindow.ini! delete it for new one.");
	}

	//Grab strings for all the input.
	screenWidth = processedInput[0].substr(12, processedInput[0].size() - 12);
	m_screenWidth = std::stoi(screenWidth);
	screenHeight = processedInput[1].substr(13, processedInput[1].size() - 13);
	m_screenHeight = std::stoi(screenHeight);

	//Hard coded without error checking. 
	//TODO: implement error checking in here.
	VSYNC = processedInput[2].substr(6, processedInput[2].size() - 6);
	if (VSYNC == "true") {
		m_vsync = 1;
	}
	else {
		m_vsync = 0;
	}
	
	windowHidden = processedInput[3].substr(13, processedInput[3].size() - 13);
	if (windowHidden == "true") {
		m_hidden = true;
	}
	else {
		m_hidden = false;
	}

	windowFullscreen = processedInput[4].substr(17, processedInput[4].size() - 17);
	if (windowFullscreen == "true") {
		m_fullscreen = true;
	}
	else {
		m_fullscreen = false;
	}

	windowBorderless = processedInput[5].substr(17, processedInput[5].size() - 17);
	if (windowBorderless == "true") {
		m_borderless = true;
	}
	else {
		m_borderless = false;
	}

	windowResizable = processedInput[6].substr(16, processedInput[6].size() - 16);
	if (windowResizable == "true") {
		m_resizable = true;
	}
	else {
		m_resizable = false;
	}

	windowMinimized = processedInput[7].substr(16, processedInput[7].size() - 16);
	if (windowMinimized == "true") {
		m_minimized = true;
	}
	else {
		m_minimized = false;
	}

	windowMaximized = processedInput[8].substr(16, processedInput[8].size() - 16);
	if (windowMaximized == "true") {
		m_maximized = true;
	}
	else {
		m_maximized = false;
	}

	m_windowName = processedInput[9].substr(11, processedInput[9].size() - 11);

}

}