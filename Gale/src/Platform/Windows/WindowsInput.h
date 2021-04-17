#pragma once

#include "Gale/Core/Base.h"

namespace Gale {

	class WindowsInput
	{
	protected:
		static bool IsKeyPressedImpl(int keycode);

		static bool IsMouseButtonPressedImpl(int button);
		static std::pair<float, float> GetMousePositionImpl();
		static float GetMouseXImpl();
		static float GetMouseYImpl();
	private:
		static WindowsInput* s_Instance;
	};

}
