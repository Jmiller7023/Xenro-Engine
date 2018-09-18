/*************************************************************************/
/*                           ScreenList.h                                */
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

#ifndef XENRO_SCREENLIST_DEFINED
#define XENRO_SCREENLIST_DEFINED

#include <vector>

#define NO_CURRENT_SCREEN_INDEX -1

namespace Xenro{

class Game;
class IScreen;

class ScreenList {
public:
	ScreenList(Game* game);
	~ScreenList();

	IScreen* moveNext();
	IScreen* movePrevious();
	IScreen* moveToParticular(int nextScreen);

	void setScreen(int nextScreen);
	void addScreen(IScreen* newScreen);

	void destroyScreen();
	void destroyScreen(int screen);

	IScreen* getCurrScreen();

protected:
	std::vector<IScreen*> m_screens;
	int m_currScreenIndex = NO_CURRENT_SCREEN_INDEX;
	Game* m_game;
};

}

#endif //XENRO_SCREENLIST_DEFINED