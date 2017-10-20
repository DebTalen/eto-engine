#ifndef ETO_GLFWEVENT_HPP
#define ETO_GLFWEVENT_HPP

#include <GLFW/glfw3.h>

namespace eto 
{

namespace input
{
enum Modifier 
{
	Shift = GLFW_MOD_SHIFT,
	Control = GLFW_MOD_CONTROL,
	Alt = GLFW_MOD_ALT,
	Super = GLFW_MOD_SUPER
};

enum Action 
{
	Press,
	Release,
	Repeat
};

enum MouseButton
{
	Button1 = GLFW_MOUSE_BUTTON_1,
	Button2,
	Button3,
	Button4,
	Button5,
	Button6,
	Button7,
	Button8,
	ButtonLast = Button8,

	ButtonLeft = Button1,
	ButtonRight = Button2,
	ButtonMiddle = Button3
};

enum Key // well...
{
	Unknown = GLFW_KEY_UNKNOWN,
	Space = GLFW_KEY_SPACE,
	Apostrophe = GLFW_KEY_APOSTROPHE,
	Comma = GLFW_KEY_COMMA,
	Minus,
	Period,
	Slash,
	Number0,
	Number1,
	Number2,
	Number3,
	Number4,
	Number5,
	Number6,
	Number7,
	Number8,
	Number9,
	Semicolon,
	Equal,
	A = GLFW_KEY_A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	LeftBracket,
	Backslash,
	RightBracket,
	GraveAccent = GLFW_KEY_GRAVE_ACCENT,
	World1 = GLFW_KEY_WORLD_1,
	World2,
	Escape = GLFW_KEY_ESCAPE,
	Enter,
	Tab,
	Backspace,
	Insert,
	Delete,
	Right,
	Left,
	Down,
	Up,
	PageUp,
	PageDown,
	Home,
	End,
	CapsLock,
	ScrollLock,
	NumLock,
	PrintScreen,
	Pause,
	F1 = GLFW_KEY_F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	F20,
	F21,
	F22,
	F23,
	F24,
	F25,
	Keypad0 = GLFW_KEY_KP_0,
	Keypad1,
	Keypad2,
	Keypad3,
	Keypad4,
	Keypad5,
	Keypad6,
	Keypad7,
	Keypad8,
	Keypad9,
	KeypadDecimal,
	KeypadDivide,
	KeypadMultiply,
	KeypadSubtract,
	KeypadAdd,
	KeypadEnter,
	KeypadEqual,
	LeftShift = GLFW_KEY_LEFT_SHIFT,
	LeftControl,
	LeftAlt,
	LeftSuper,
	RightShift,
	RightControl,
	RightAlt,
	RightSuper,
	Menu,
	Last = Menu
};

}

using namespace eto::input;

/**
 *  @brief  Represents input event 
 */
class GLFWevent
{
public:
	/** Enum of event types */
	enum Type  {
		Key = 0, 		/*!< Keyboard key presed/releaded/held */
		MouseButton,    /*!< Mouse utton pressed/released/held */
		CursorPosition, /*!< Cursor moved */
		Char,		/*!< Character entered */
		WindowPosition, /*!< Window moved */
		WindowSize, 	/*!< Window resized */
		WindowClose     /*!< Window closed */
	}  type;

	/** Structure that represents keyboard event */
	struct KeyEvent {
		input::Key key; /*!< Button key */
		int scancode;   /*!< System specific scancode */
		Action action;  /*!< Produced action */
		int modifier; 	/*!< Additional keys modifier (ctrl, alt etc.) */
	};

	/** Structure that represents mouse button event */
	struct MouseButtonEvent {
		input::MouseButton button; /*!< Button key */
		Action action;             /*!< Produced action */
		int modifier;              /*!< Additional keys modifier (ctrl, alt etc.) */
	};

	/** Structure that represents cursor position */
	struct CursorPositionEvent {
		double x;
		double y;
	};

	union {
		KeyEvent key;
		MouseButtonEvent mouseButton;
		CursorPositionEvent cursorPos;
	} data;

}; // end of class GLFWevent  


} // namespace eto

#endif // ETO_GLFWEVENT_HPP
