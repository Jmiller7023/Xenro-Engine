/*************************************************************************/
/*                             IScreen.h                                 */
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

#ifndef XENRO_ISCREEN_DEFINED
#define XENRO_ISCREEN_DEFINED

namespace Xenro{

enum class ScreenState {
		NONE, RUNNING, EXIT_APP, CHANGE_TO_NEXT, CHANGE_TO_PREVIOUS, CHANGE_TO_PARTICULAR
};

class Game;

class IScreen {
public:
	friend class ScreenList;
	IScreen() { }
	virtual ~IScreen() { }

	//Return index
	virtual int getNextScreenIndex() const = 0;
	virtual int getPrevScrenIndex() const = 0;

	//Called at beginning and end of application.
	virtual void create() = 0;
	virtual void destroy() = 0;

	//Called whenever a screen closes or opens.
	virtual void onEntry() = 0;
	virtual void onExit() = 0;

	//Called in Gameloop.
	virtual void update() = 0;
	virtual void draw() = 0;

	//Getters.
	int getScreenIndex() const { return m_screenIndex; }
	ScreenState getScreenState() const { return m_currState; }
	int getParticularScreenIndex() const { return m_changeToParticular; }
	
	//Setters.
	void setGame(Game* game) { m_game = game; }
	void setRunning() { m_currState = ScreenState::RUNNING; }
	void setParticularScreenIndex(int index) { m_changeToParticular = index; }

protected:
	ScreenState m_currState = ScreenState::NONE;
	Game* m_game = nullptr;
	int m_screenIndex = -1;
	int m_changeToParticular = -1;
};

}

#endif //XENRO_ISCREEN_DEFINED