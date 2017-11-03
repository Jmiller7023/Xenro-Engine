#ifndef XENRO_WINDOW_DEFINED
#define XENRO_WINDOW_DEFINED

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>
#include <vector>

namespace Xenro {

	class SaveManager;
	class LoadManager;

	class Window
	{
	public:
		Window();
		~Window();

		//Creates window based on values from window initialization file.
		int create();

		//Swaps buffer.
		void swapBuffer();

		//init manipulators
		void setInitScreenName(const std::string& name);
		void setInitScreenWidth(int screenWidth);
		void setInitScreenHeight(int screenWidth);
		void setInitHidden(bool state);
		void setInitFullscreen(bool state);
		void setInitBorderless(bool state);
		void setInitResizable(bool state);
		void setInitMaximized(bool state);
		void setInitMinimized(bool state);
		void setInitVSYNC(bool state);

		//Setters
		void setScreenWidth(int width) { m_screenWidth = width; }
		void setScreenHeight(int height) { m_screenHeight = height; }

		//Getters
		int getScreenWidth() const { return m_screenWidth; }
		int getScreenHeight() const { return m_screenHeight; }

		//Window modifiers.
		void modifyWindowSize();
		void modifyWindowSize(int width, int height);
		void setFullscreen();
		void setFullscreen(int width, int height);
		void setWindowBorder(bool isBordered);
		void setVSYNC(bool on);
		
	private:
		void createDefaultIni();
		void loadValues();
		void modifyInit(const std::string& line, const std::string& text);

		LoadManager* m_loadManager;
		SaveManager* m_saveManager;
		SDL_Window* m_sdlWindow;
		int m_screenWidth = 1920, m_screenHeight = 1080, m_vsync = 1;
		std::string m_windowName;
		bool m_hidden = false, m_fullscreen = false, m_borderless = false,
			m_resizable = false, m_maximized = false, m_minimized = false;
		std::vector<std::string> m_cachedFile;
	};

}

#endif //XENRO_WINDOW_DEFINED