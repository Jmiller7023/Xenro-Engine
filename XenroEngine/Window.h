#ifndef XENRO_WINDOW_DEFINED
#define XENRO_WINDOW_DEFINED

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>
#include <vector>

namespace Xenro {

	class Window
	{
	public:
		Window();
		~Window();

		//Creates window based on values from window initialization file.
		int create();

		//Swaps buffer.
		void swapBuffer();

		//Getters
		int getScreenWidth() { return m_screenWidth; }
		int getScreenHeight() { return m_screenHeight; }

	private:
		void createDefaultIni();
		void loadValues(const std::vector<std::string>& setup);
		SDL_Window* m_sdlWindow;
		int m_screenWidth = 1920, m_screenHeight = 1080, m_vsync = 1;
		std::string m_windowName;
		bool m_hidden = false, m_fullscreen = false, m_borderless = false,
			m_resizable = false, m_maximized = false, m_minimized = false;
	};

}

#endif //XENRO_WINDOW_DEFINED