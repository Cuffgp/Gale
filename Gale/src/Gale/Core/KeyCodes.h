#pragma once

namespace Gale
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space               = 32,
		Apostrophe          = 39, /* ' */
		Comma               = 44, /* , */
		Minus               = 45, /* - */
		Period              = 46, /* . */
		Slash               = 47, /* / */

		D0                  = 48, /* 0 */
		D1                  = 49, /* 1 */
		D2                  = 50, /* 2 */
		D3                  = 51, /* 3 */
		D4                  = 52, /* 4 */
		D5                  = 53, /* 5 */
		D6                  = 54, /* 6 */
		D7                  = 55, /* 7 */
		D8                  = 56, /* 8 */
		D9                  = 57, /* 9 */

		Semicolon           = 59, /* ; */
		Equal               = 61, /* = */

		A                   = 65,
		B                   = 66,
		C                   = 67,
		D                   = 68,
		E                   = 69,
		F                   = 70,
		G                   = 71,
		H                   = 72,
		I                   = 73,
		J                   = 74,
		K                   = 75,
		L                   = 76,
		M                   = 77,
		N                   = 78,
		O                   = 79,
		P                   = 80,
		Q                   = 81,
		R                   = 82,
		S                   = 83,
		T                   = 84,
		U                   = 85,
		V                   = 86,
		W                   = 87,
		X                   = 88,
		Y                   = 89,
		Z                   = 90,

		LeftBracket         = 91,  /* [ */
		Backslash           = 92,  /* \ */
		RightBracket        = 93,  /* ] */
		GraveAccent         = 96,  /* ` */

		World1              = 161, /* non-US #1 */
		World2              = 162, /* non-US #2 */

		/* Function keys */
		Escape              = 256,
		Enter               = 257,
		Tab                 = 258,
		Backspace           = 259,
		Insert              = 260,
		Delete              = 261,
		Right               = 262,
		Left                = 263,
		Down                = 264,
		Up                  = 265,
		PageUp              = 266,
		PageDown            = 267,
		Home                = 268,
		End                 = 269,
		CapsLock            = 280,
		ScrollLock          = 281,
		NumLock             = 282,
		PrintScreen         = 283,
		Pause               = 284,
		F1                  = 290,
		F2                  = 291,
		F3                  = 292,
		F4                  = 293,
		F5                  = 294,
		F6                  = 295,
		F7                  = 296,
		F8                  = 297,
		F9                  = 298,
		F10                 = 299,
		F11                 = 300,
		F12                 = 301,
		F13                 = 302,
		F14                 = 303,
		F15                 = 304,
		F16                 = 305,
		F17                 = 306,
		F18                 = 307,
		F19                 = 308,
		F20                 = 309,
		F21                 = 310,
		F22                 = 311,
		F23                 = 312,
		F24                 = 313,
		F25                 = 314,

		/* Keypad */
		KP0                 = 320,
		KP1                 = 321,
		KP2                 = 322,
		KP3                 = 323,
		KP4                 = 324,
		KP5                 = 325,
		KP6                 = 326,
		KP7                 = 327,
		KP8                 = 328,
		KP9                 = 329,
		KPDecimal           = 330,
		KPDivide            = 331,
		KPMultiply          = 332,
		KPSubtract          = 333,
		KPAdd               = 334,
		KPEnter             = 335,
		KPEqual             = 336,

		LeftShift           = 340,
		LeftControl         = 341,
		LeftAlt             = 342,
		LeftSuper           = 343,
		RightShift          = 344,
		RightControl        = 345,
		RightAlt            = 346,
		RightSuper          = 347,
		Menu                = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}

}

// From glfw3.h
#define GL_KEY_SPACE           ::Gale::Key::Space
#define GL_KEY_APOSTROPHE      ::Gale::Key::Apostrophe    /* ' */
#define GL_KEY_COMMA           ::Gale::Key::Comma         /* , */
#define GL_KEY_MINUS           ::Gale::Key::Minus         /* - */
#define GL_KEY_PERIOD          ::Gale::Key::Period        /* . */
#define GL_KEY_SLASH           ::Gale::Key::Slash         /* / */
#define GL_KEY_0               ::Gale::Key::D0
#define GL_KEY_1               ::Gale::Key::D1
#define GL_KEY_2               ::Gale::Key::D2
#define GL_KEY_3               ::Gale::Key::D3
#define GL_KEY_4               ::Gale::Key::D4
#define GL_KEY_5               ::Gale::Key::D5
#define GL_KEY_6               ::Gale::Key::D6
#define GL_KEY_7               ::Gale::Key::D7
#define GL_KEY_8               ::Gale::Key::D8
#define GL_KEY_9               ::Gale::Key::D9
#define GL_KEY_SEMICOLON       ::Gale::Key::Semicolon     /* ; */
#define GL_KEY_EQUAL           ::Gale::Key::Equal         /* = */
#define GL_KEY_A               ::Gale::Key::A
#define GL_KEY_B               ::Gale::Key::B
#define GL_KEY_C               ::Gale::Key::C
#define GL_KEY_D               ::Gale::Key::D
#define GL_KEY_E               ::Gale::Key::E
#define GL_KEY_F               ::Gale::Key::F
#define GL_KEY_G               ::Gale::Key::G
#define GL_KEY_H               ::Gale::Key::H
#define GL_KEY_I               ::Gale::Key::I
#define GL_KEY_J               ::Gale::Key::J
#define GL_KEY_K               ::Gale::Key::K
#define GL_KEY_L               ::Gale::Key::L
#define GL_KEY_M               ::Gale::Key::M
#define GL_KEY_N               ::Gale::Key::N
#define GL_KEY_O               ::Gale::Key::O
#define GL_KEY_P               ::Gale::Key::P
#define GL_KEY_Q               ::Gale::Key::Q
#define GL_KEY_R               ::Gale::Key::R
#define GL_KEY_S               ::Gale::Key::S
#define GL_KEY_T               ::Gale::Key::T
#define GL_KEY_U               ::Gale::Key::U
#define GL_KEY_V               ::Gale::Key::V
#define GL_KEY_W               ::Gale::Key::W
#define GL_KEY_X               ::Gale::Key::X
#define GL_KEY_Y               ::Gale::Key::Y
#define GL_KEY_Z               ::Gale::Key::Z
#define GL_KEY_LEFT_BRACKET    ::Gale::Key::LeftBracket   /* [ */
#define GL_KEY_BACKSLASH       ::Gale::Key::Backslash     /* \ */
#define GL_KEY_RIGHT_BRACKET   ::Gale::Key::RightBracket  /* ] */
#define GL_KEY_GRAVE_ACCENT    ::Gale::Key::GraveAccent   /* ` */
#define GL_KEY_WORLD_1         ::Gale::Key::World1        /* non-US #1 */
#define GL_KEY_WORLD_2         ::Gale::Key::World2        /* non-US #2 */

/* Function keys */
#define GL_KEY_ESCAPE          ::Gale::Key::Escape
#define GL_KEY_ENTER           ::Gale::Key::Enter
#define GL_KEY_TAB             ::Gale::Key::Tab
#define GL_KEY_BACKSPACE       ::Gale::Key::Backspace
#define GL_KEY_INSERT          ::Gale::Key::Insert
#define GL_KEY_DELETE          ::Gale::Key::Delete
#define GL_KEY_RIGHT           ::Gale::Key::Right
#define GL_KEY_LEFT            ::Gale::Key::Left
#define GL_KEY_DOWN            ::Gale::Key::Down
#define GL_KEY_UP              ::Gale::Key::Up
#define GL_KEY_PAGE_UP         ::Gale::Key::PageUp
#define GL_KEY_PAGE_DOWN       ::Gale::Key::PageDown
#define GL_KEY_HOME            ::Gale::Key::Home
#define GL_KEY_END             ::Gale::Key::End
#define GL_KEY_CAPS_LOCK       ::Gale::Key::CapsLock
#define GL_KEY_SCROLL_LOCK     ::Gale::Key::ScrollLock
#define GL_KEY_NUM_LOCK        ::Gale::Key::NumLock
#define GL_KEY_PRINT_SCREEN    ::Gale::Key::PrintScreen
#define GL_KEY_PAUSE           ::Gale::Key::Pause
#define GL_KEY_F1              ::Gale::Key::F1
#define GL_KEY_F2              ::Gale::Key::F2
#define GL_KEY_F3              ::Gale::Key::F3
#define GL_KEY_F4              ::Gale::Key::F4
#define GL_KEY_F5              ::Gale::Key::F5
#define GL_KEY_F6              ::Gale::Key::F6
#define GL_KEY_F7              ::Gale::Key::F7
#define GL_KEY_F8              ::Gale::Key::F8
#define GL_KEY_F9              ::Gale::Key::F9
#define GL_KEY_F10             ::Gale::Key::F10
#define GL_KEY_F11             ::Gale::Key::F11
#define GL_KEY_F12             ::Gale::Key::F12
#define GL_KEY_F13             ::Gale::Key::F13
#define GL_KEY_F14             ::Gale::Key::F14
#define GL_KEY_F15             ::Gale::Key::F15
#define GL_KEY_F16             ::Gale::Key::F16
#define GL_KEY_F17             ::Gale::Key::F17
#define GL_KEY_F18             ::Gale::Key::F18
#define GL_KEY_F19             ::Gale::Key::F19
#define GL_KEY_F20             ::Gale::Key::F20
#define GL_KEY_F21             ::Gale::Key::F21
#define GL_KEY_F22             ::Gale::Key::F22
#define GL_KEY_F23             ::Gale::Key::F23
#define GL_KEY_F24             ::Gale::Key::F24
#define GL_KEY_F25             ::Gale::Key::F25

/* Keypad */
#define GL_KEY_KP_0            ::Gale::Key::KP0
#define GL_KEY_KP_1            ::Gale::Key::KP1
#define GL_KEY_KP_2            ::Gale::Key::KP2
#define GL_KEY_KP_3            ::Gale::Key::KP3
#define GL_KEY_KP_4            ::Gale::Key::KP4
#define GL_KEY_KP_5            ::Gale::Key::KP5
#define GL_KEY_KP_6            ::Gale::Key::KP6
#define GL_KEY_KP_7            ::Gale::Key::KP7
#define GL_KEY_KP_8            ::Gale::Key::KP8
#define GL_KEY_KP_9            ::Gale::Key::KP9
#define GL_KEY_KP_DECIMAL      ::Gale::Key::KPDecimal
#define GL_KEY_KP_DIVIDE       ::Gale::Key::KPDivide
#define GL_KEY_KP_MULTIPLY     ::Gale::Key::KPMultiply
#define GL_KEY_KP_SUBTRACT     ::Gale::Key::KPSubtract
#define GL_KEY_KP_ADD          ::Gale::Key::KPAdd
#define GL_KEY_KP_ENTER        ::Gale::Key::KPEnter
#define GL_KEY_KP_EQUAL        ::Gale::Key::KPEqual

#define GL_KEY_LEFT_SHIFT      ::Gale::Key::LeftShift
#define GL_KEY_LEFT_CONTROL    ::Gale::Key::LeftControl
#define GL_KEY_LEFT_ALT        ::Gale::Key::LeftAlt
#define GL_KEY_LEFT_SUPER      ::Gale::Key::LeftSuper
#define GL_KEY_RIGHT_SHIFT     ::Gale::Key::RightShift
#define GL_KEY_RIGHT_CONTROL   ::Gale::Key::RightControl
#define GL_KEY_RIGHT_ALT       ::Gale::Key::RightAlt
#define GL_KEY_RIGHT_SUPER     ::Gale::Key::RightSuper
#define GL_KEY_MENU            ::Gale::Key::Menu
