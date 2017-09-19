#ifndef ETO_GLFWEVENT_HPP
#define ETO_GLFWEVENT_HPP

#include <core/Input.hpp>

namespace eto 
{

using namespace eto::Input;

/*!
 *  \brief  Represents input event 
 */
class GLFWevent
{
public:
	GLFWevent () {}
	
	/*! \brief Enum of event types */
	enum Type  {
		Key, 		/*!< Keyboard key presed/releaded/held */
		MouseButton,    /*!< Mouse utton pressed/released/held */
		CursorPosition, /*!< Cursor moved */
		Char,		/*!< Character entered */
		WindowPosition, /*!< Window moved */
		WindowSize, 	/*!< Window resized */
		WindowClose     /*!< Window closed */
	}  type;

	/*! \brief Structure that represents keyboard event */
	struct KeyEvent {
		Input::Key key; /*!< Button key */
		int scancode;   /*!< System specific scancode */
		Action action;  /*!< Produced action */
		int modifier; 	/*!< Additional keys modifier (ctrl, alt etc.) */
	};

	/*! \brief Structure that represents mouse button event */
	struct MouseButtonEvent {
		Input::MouseButton button; /*!< Button key */
		Action action;             /*!< Produced action */
		int modifier;              /*!< Additional keys modifier (ctrl, alt etc.) */
	};

	/*! \brief Structure that represents cursor position */
	struct CursorPositionEvent {
		double x;
		double y;
	};

	union {
		KeyEvent key;
		MouseButtonEvent mouseButton;
		CursorPositionEvent cursorPos;
	};

}; // end of class GLFWevent  


} // namespace eto

#endif // ETO_GLFWEVENT_HPP
