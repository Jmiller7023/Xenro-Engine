#include "Timing.h"
#include <SDL\SDL.h>
namespace Xenro{

FPSlimiter::FPSlimiter()
	:m_fps(60.0f), m_maxFPS(60.0f), m_lastTime(0.0f), m_frames(0)
{}

FPSlimiter::~FPSlimiter() {

}

void FPSlimiter::setTargetFPS(float targetFPS) {
	m_maxFPS = targetFPS;
}

void FPSlimiter::calculateFPS() {
	
	m_currTicks = (float)SDL_GetTicks();
	m_frames++;
	//if it's been one second since last calculation.
	if (m_currTicks - m_lastTime >= 1000.0f) {
		m_fps =  m_frames;
		m_frames = 1;
		m_lastTime += 1000.0f;
	}
}

float FPSlimiter::limitFPS() {
	float delay = ((float)SDL_GetTicks() - m_currTicks);
	if ( 1000.0f / m_maxFPS > delay) {
		SDL_Delay((Uint32)(1000.f / m_maxFPS - delay));
	}
	return m_fps;
}



}