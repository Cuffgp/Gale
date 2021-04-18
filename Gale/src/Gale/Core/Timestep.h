#pragma once

namespace Gale {

	class Timestep 
	{
	public:
		Timestep() {}
		Timestep(float time);
		void Set(float time);

		operator float() const { return m_Time; }
		inline float GetSeconds() const { return m_Time; }
		inline float GetMilliseconds() const { return m_Time * 1000; }
		inline float GetRuntime() const { return s_Runtime; }
		inline unsigned int GetFrameCount() const { return s_FrameCount; }

	private:
		float m_Time = 0.0f;
		static float s_Runtime;
		static unsigned int s_FrameCount;
	};

}