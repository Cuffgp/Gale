#include "glpch.h"

#include "Timestep.h"

namespace Gale {

	float Timestep::s_Runtime = 0;
	unsigned int Timestep::s_FrameCount = 0;

	Timestep::Timestep(float time) :
		m_Time(time)
	{

	}

	void Timestep::Set(float time)
	{
		m_Time = time;
		s_Runtime += time;
		s_FrameCount += 1;
	}

}