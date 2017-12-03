/*************************************************************************/
/*                             Window.h                                  */
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
	void create();

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