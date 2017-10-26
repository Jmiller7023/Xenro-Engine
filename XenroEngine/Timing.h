#ifndef XENRO_TIMING_DEFINED
#define XENRO_TIMING_DEFINED

namespace Xenro{
class FPSlimiter {
public:
	FPSlimiter();
	~FPSlimiter();

	//manually set the FPS after construction.
	void setTargetFPS(float targetFPS);

	//counts the number of gameticks every second, then calculates how many frames each second.
	void calculateFPS();

	//returns the fps.
	float limitFPS();

private:

	float m_fps, m_maxFPS, m_lastTime, m_prevTicks, m_currTicks;
	int m_frames;

};

}

#endif //XENRO_TIMING_DEFINED