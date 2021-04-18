#pragma once

#include "Gale/Core/Base.h"
#include "Gale/Core/KeyCodes.h"

namespace Gale {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
