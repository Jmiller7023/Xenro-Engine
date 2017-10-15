#include "Window.h"
#include "ErrorMessages.h"

namespace Xenro{
Window::Window()
	:m_screenWidth(0), m_screenHeight(0)
{
}


Window::~Window()
{
}

int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

	//Store the screen info.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	Uint32 flags = SDL_WINDOW_OPENGL;

	if (currentFlags & INVISIBLE) {
		flags |= SDL_WINDOW_HIDDEN;
	}
	if (currentFlags & FULLSCREEN) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	if (currentFlags & BORDERLESS) {
		flags |= SDL_WINDOW_BORDERLESS;
	}

	//Open an SDL window.
	m_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

	if (m_sdlWindow == nullptr) {
		fatalError("Failed to create SDL window!");
	}

	//Set up OpenGL context.
	SDL_GLContext GLcontext = SDL_GL_CreateContext(m_sdlWindow);

	if (GLcontext == nullptr) {
		fatalError("GL context could not be created!");
	}

	//set up glew (optional but recommended)
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Failed to initialize Glew!");
	}

	//Check the OpenGL version.
	std::printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));

	//Set window color to black.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Set VSYNC: 0 turns it off and 1 turns it on.
	SDL_GL_SetSwapInterval(0);

	//Enable alpha blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Window::swapBuffer() {
	//Swap our buffer and draw everything to the screen.
	SDL_GL_SwapWindow(m_sdlWindow);
	}
}