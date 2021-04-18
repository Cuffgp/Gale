#pragma once

namespace Gale
{


	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

}

// Mouse (TODO: move into separate file probably)
#define GL_MOUSE_BUTTON_LEFT    0
#define GL_MOUSE_BUTTON_RIGHT   1
#define GL_MOUSE_BUTTON_MIDDLE  2