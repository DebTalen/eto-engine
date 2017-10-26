#ifndef ETO_INPUT_HPP
#define ETO_INPUT_HPP

#include <GLFW/glfw3.h>
#include <core/Window.hpp>
#include <core/GLFWevent.hpp>
#include <core/Assert.hpp>
#include <functional>
#include <vector>
#include <map>

using eto::GLFWevent;

namespace eto
{

/**
 *  @brief  Hander for the raw input data
 */
class Input 
{
public:
	typedef std::function<void(const GLFWevent&)> CallbackType;

	/** Returns the single instance of the Input class */
	static Input &getInstance();

	/**
	 *  @brief  Configures callbacks from specified window
	 *
	 *  You should always call this function before using the Input
	 */
	void setWindow(const Window &w);

	/**
	 *  @brief  Adds a callback function to the specified event type
	 *
	 *  The specified callback will be called whenever the specified event type will pop up.
	 *  The callback function must take const reference to the GLFWevent and return void.
	 *  Use lambda to wrap the needed function.
	 *  @param  type Event type to subscribe 
	 *  @param  callback The callback function
	 */
	void addCallback(GLFWevent::Type type, CallbackType callback);

	/**
	 *  @brief  Removes the specified callback
	 *
	 *  @param  type Event type to unsubscribe 
	 *  @param  callback The callback function
	 */
	void removeCallback(GLFWevent::Type type, CallbackType callback);

	/**
	 *  @brief  Indicates if the key was pressed
	 *
	 *  @param  key The key code
	 *  @return True if was pressd and false if not
	 */
	bool isKeyPress(input::Key key) const;

	/**
	 *  @brief  Indicates if the key was released
	 *
	 *  @param  key The key code
	 *  @return True if was released and false if not
	 */
	bool isKeyRelese(input::Key key) const;

	/**
	 *  @brief  Indicates if the key was held
	 *
	 *  @param  key The key code
	 *  @return True if was held and false if not
	 */
	bool isKeyHeld(input::Key key) const;

	/**
	 *  @brief  Returns the last cursor position
	 *
	 *  @return The cursor coordinates on the screen
	 */
	GLFWevent::CursorPositionEvent getCursorPosition() const; 

	/** Callback function for handling keyboard input */
	void onKey(int key, int scancode, int action, int mods);

	/** Callback function for handling mouse buttons input */
	void onMouseButton(int button, int action, int mods);

	/** Callback function for handling mouse cursor position */
	void onCursorPosition(double x, double y);
private:
	Input(const Window &w);
	Input();

	void setupCallbacks();

	void notify(GLFWevent::Type t, const GLFWevent &e);

	std::map<GLFWevent::Type, std::vector<CallbackType>> m_observers;
	GLFWwindow *m_window;
};	

}

#endif //ETO_INPUT_HPP
