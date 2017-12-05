/*************************************************************************/
/*                          InputManager.cpp                             */
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

#include "InputManager.h"
#include "Game.h"
#include "globals.h"
#include <cmath>
#include<SDL\SDL.h>
#include <SDL\SDL_gamecontroller.h>

namespace Xenro{

InputManager::InputManager()
	:m_mousecoords(0)
{
	//Empty
}
InputManager::InputManager(Game* game)
	:m_mousecoords(0), m_game(game)
{
	//Empty
}


InputManager::~InputManager()
{
	//Empty
}

unsigned int InputManager::SDLKtoButton(unsigned int keyID) {

	switch (keyID) {
		case SDL_CONTROLLER_BUTTON_A: return Button::A;
		case SDL_CONTROLLER_BUTTON_B: return Button::B;
		case SDL_CONTROLLER_BUTTON_X: return Button::X;
		case SDL_CONTROLLER_BUTTON_Y: return Button::Y;
		case SDL_CONTROLLER_BUTTON_BACK: return Button::BACK;
		case SDL_CONTROLLER_BUTTON_GUIDE: return Button::GUIDE;
		case SDL_CONTROLLER_BUTTON_START: return Button::START;
		case SDL_CONTROLLER_BUTTON_LEFTSTICK: return Button::LEFTSTICK;
		case SDL_CONTROLLER_BUTTON_RIGHTSTICK: return Button::RIGHTSTICK;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return Button::LEFTSHOULDER;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return Button::RIGHTSHOULDER;
		case SDL_CONTROLLER_BUTTON_DPAD_UP: return Button::DPAD_UP;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return Button::DPAD_DOWN;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return Button::DPAD_LEFT;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return Button::DPAD_RIGHT;
		case SDL_CONTROLLER_BUTTON_MAX: return Button::MAX;
		default: return Button::INVALID;
	}

}

void InputManager::angleToKeyRightAnalog() {

	//Release old key.
	keyRelease(m_lastRightAnalogKey);

	//Press new key.
	if(m_rightAnalogAngle == 0) {
		keyPress(Axis::RIGHTANALOGRIGHT);
		m_lastRightAnalogKey = Axis::RIGHTANALOGRIGHT;
	}
	else if(m_rightAnalogAngle == -45) {
		keyPress(Axis::RIGHTANALOGUPRIGHT);
		m_lastRightAnalogKey = Axis::RIGHTANALOGUPRIGHT;
	}
	else if(m_rightAnalogAngle == -90) {
		keyPress(Axis::RIGHTANALOGUP);
		m_lastRightAnalogKey = Axis::RIGHTANALOGUP;
	}
	else if(m_rightAnalogAngle == -135) {
		keyPress(Axis::RIGHTANALOGUPLEFT);
		m_lastRightAnalogKey = Axis::RIGHTANALOGUPLEFT;
	}
	else if(m_rightAnalogAngle == 180) {
		keyPress(Axis::RIGHTANALOGLEFT);
		m_lastRightAnalogKey = Axis::RIGHTANALOGLEFT;
	}
	else if(m_rightAnalogAngle == 135) {
		keyPress(Axis::RIGHTANALOGDOWNLEFT);
		m_lastRightAnalogKey = Axis::RIGHTANALOGDOWNLEFT;
	}
	else if(m_rightAnalogAngle == 90) {
		keyPress(Axis::RIGHTANALOGDOWN);
		m_lastRightAnalogKey = Axis::RIGHTANALOGDOWN;
	}
	else if(m_rightAnalogAngle == 45) {
		keyPress(Axis::RIGHTANALOGDOWNRIGHT);
		m_lastRightAnalogKey = Axis::RIGHTANALOGDOWNRIGHT;
	}
}

void InputManager::angleToKeyLeftAnalog() {

	//Release old key.
	keyRelease(m_lastLeftAnalogKey);
 
	//Press new key.
	if(m_leftAnalogAngle == 0) {
		keyPress(Axis::LEFTANALOGRIGHT);
		m_lastLeftAnalogKey = Axis::LEFTANALOGRIGHT;
	}
	else if(m_leftAnalogAngle == -45) {
		keyPress(Axis::LEFTANALOGUPRIGHT);
		m_lastLeftAnalogKey = Axis::LEFTANALOGUPRIGHT;
	}
	else if(m_leftAnalogAngle  == -90) {
		keyPress(Axis::LEFTANALOGUP);
		m_lastLeftAnalogKey = Axis::LEFTANALOGUP;
	}
	else if(m_leftAnalogAngle  == -135) {
		keyPress(Axis::LEFTANALOGUPLEFT);
		m_lastLeftAnalogKey = Axis::LEFTANALOGUPLEFT;
	}
	else if(m_leftAnalogAngle == 180) {
		keyPress(Axis::LEFTANALOGLEFT);
		m_lastLeftAnalogKey = Axis::LEFTANALOGLEFT;
	}
	else if(m_leftAnalogAngle == 135) {
		keyPress(Axis::LEFTANALOGDOWNLEFT);
		m_lastLeftAnalogKey = Axis::LEFTANALOGDOWNLEFT;
	}
	else if(m_leftAnalogAngle  == 90) {
		keyPress(Axis::LEFTANALOGDOWN);
		m_lastLeftAnalogKey = Axis::LEFTANALOGDOWN;
	}
	else if(m_leftAnalogAngle == 45) {
		keyPress(Axis::LEFTANALOGDOWNRIGHT);
		m_lastLeftAnalogKey = Axis::LEFTANALOGDOWNRIGHT;
	}
}

void InputManager::processJoyAxis(SDL_Event& evnt) {

	bool leftAnalogMotion = false;
	bool rightAnalogMotion = false;

	///RIGHT TRIGGER PULLED
	if (evnt.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
	{
		if (evnt.caxis.value > TRIGGER_DEAD_ZONE) {
			keyPress(Button::RIGHT_TRIGGER);
		}
		else {
			keyRelease(Button::RIGHT_TRIGGER);
		}
		m_rightTriggerValue = evnt.caxis.value;
	}

	///LEFT TRIGGER PULLED
	if (evnt.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
	{
		if (evnt.caxis.value > TRIGGER_DEAD_ZONE) {
			keyPress(Button::LEFT_TRIGGER);
		}
		else {
			keyRelease(Button::LEFT_TRIGGER);
		}
		m_leftTriggerValue = evnt.caxis.value;
	}

	///LEFT ANALOG STICK
	//X axis motion
	if (evnt.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
	{
		//Left of dead zone
		if (evnt.caxis.value < -CONTROLLER_DEAD_ZONE)
		{
			m_leftxDir = -1;
		}
		//Right of dead zone
		else if (evnt.caxis.value > CONTROLLER_DEAD_ZONE)
		{
			m_leftxDir = 1;
		}
		else
		{
			m_leftxDir = 0;
		}
		leftAnalogMotion = true;
	}
	//Y axis motion
	else if (evnt.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
	{
		//Below of dead zone
		if (evnt.caxis.value < -CONTROLLER_DEAD_ZONE)
		{
			m_leftyDir = -1;
		}
		//Above of dead zone
		else if (evnt.caxis.value > CONTROLLER_DEAD_ZONE)
		{
			m_leftyDir = 1;
		}
		else
		{
			m_leftyDir = 0;
		}
		leftAnalogMotion = true;
	}
	
	///RIGHT ANALOG STICK
	//X axis motion
	else if (evnt.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
	{
		//Left of dead zone
		if (evnt.caxis.value < -CONTROLLER_DEAD_ZONE)
		{
			m_rightxDir = -1;
		}
		//Right of dead zone
		else if (evnt.caxis.value > CONTROLLER_DEAD_ZONE)
		{
			m_rightxDir = 1;
		}
		else
		{
			m_rightxDir = 0;
		}
		rightAnalogMotion = true;
	}
	//Y axis motion
	else if (evnt.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
	{
		//Below of dead zone
		if (evnt.caxis.value < -CONTROLLER_DEAD_ZONE)
		{
			m_rightyDir = -1;
		}
		//Above of dead zone
		else if (evnt.caxis.value > CONTROLLER_DEAD_ZONE)
		{
			m_rightyDir = 1;
		}
		else
		{
			m_rightyDir = 0;
		}
		rightAnalogMotion = true;
	}

	if(leftAnalogMotion) {

		//Calculate angle
		m_leftAnalogAngle = atan2( (double)m_leftyDir, (double)m_leftxDir ) * ( 180.0 / M_PI );

		//Take into account NaN case.
		if (m_leftyDir == 0 && m_leftxDir == 0) {
			m_leftAnalogAngle = -1;
		}

		angleToKeyLeftAnalog();
	}

	if(rightAnalogMotion){

		//Calculate angle
		m_rightAnalogAngle = atan2((double)m_rightyDir, (double)m_rightxDir) * (180.0 / M_PI);

		//Take into account NaN case.
		if (m_rightyDir == 0 && m_rightxDir == 0) {
			m_rightAnalogAngle = -1;
		}

		angleToKeyRightAnalog();
	}
}

void InputManager::processInput(SDL_Event& evnt) {

		switch (evnt.type) {

			case SDL_QUIT:
				m_game->exitGame();
				break;
			case SDL_KEYDOWN:
				keyPress(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				keyRelease(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				keyPress(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				keyRelease(evnt.button.button);
				break;
			case SDL_CONTROLLERAXISMOTION:
				//Motion on controller 0.
				if (evnt.caxis.which == 0)
				{
					processJoyAxis(evnt);
				}
				//Additional controller support not yet implemented
				break;
			case SDL_CONTROLLERBUTTONDOWN:
				keyPress(SDLKtoButton(evnt.cbutton.button));
				break;
			case SDL_CONTROLLERBUTTONUP:
				keyRelease(SDLKtoButton(evnt.cbutton.button));
				break;
			case SDL_MOUSEMOTION:
				setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			case SDL_CONTROLLERDEVICEADDED:
				m_game->addGameController();
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				m_game->RemoveGameController();
				break;
			case SDL_WINDOWEVENT:
				switch (evnt.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						m_game->modifyWindowScreenWidth(evnt.window.data1);
						m_game->modifyWindowScreenHeight(evnt.window.data2);
						break;
				}
			
		}
}


void InputManager::update() {

	//iterate throught the keymap and set it to prevmap.
	for (auto it = m_keymap.begin(); it != m_keymap.end(); it++) {
		m_prevmap[it->first] = it->second;
	}
}

bool InputManager::isDown(unsigned int keyID) {

	//check if the key is in the map
	auto it = m_keymap.find(keyID);
	if (it != m_keymap.end()) {
		return it->second;
	}
	else {
		//wasn't in the map.
		return false;
	}
}

bool InputManager::isPressed(unsigned int keyID) {
	return (wasPressed(keyID) == false && isDown(keyID) == true);
}

bool InputManager::wasPressed(unsigned int keyID) {
	//check if the key is in the map
	auto it = m_prevmap.find(keyID);
	if (it != m_prevmap.end()) {
		return it->second;
	}
	else {
		//wasn't in the map.
		return false;
	}
}


}