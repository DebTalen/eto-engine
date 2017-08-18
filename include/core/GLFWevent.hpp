#ifndef ETO_GLFWEVENT_HPP
#define ETO_GLFWEVENT_HPP

#include <core/Input.hpp>

namespace eto 
{
/*
 * TODO: 
 * - Rewrite class so that it won't use union
 */

using namespace eto::Input;
// =====================================================================================
//        Class:  GLFWevent
//  Description:  Class for representing input event
// =====================================================================================
class GLFWevent
{
public:
	GLFWevent () {}
	
	// Types of events
	enum Type  {
		Key, 		// keyboard key presed/releaded/held
		MouseButton,    // mouse button pressed/released/held
		CursorPosition, // cursor moved
		Char,		// character entered
		WindowPosition, // window moved
		WindowSize, 	// window resized
		WindowClose     // window closed
	}  type;

	struct KeyEvent {
		Input::Key key;
		int scancode;
		Action action;
		int modifier;
	};
	struct MouseButtonEvent {
		Input::MouseButton button;
		Action action;
		int modifier;
	};

	struct CursorPositionEvent {
		double x;
		double y;
	};

	union {
		KeyEvent key;
		MouseButtonEvent mouseButton;
		CursorPositionEvent cursorPos;
	};

}; // -----  end of class GLFWevent  ----- 


} // namespace eto

#endif // ETO_GLFWEVENT_HPP
