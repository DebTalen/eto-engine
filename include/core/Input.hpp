#ifndef ETO_INPUT_HPP
#define ETO_INPUT_HPP

#include <GLFW/glfw3.h>
#include <core/Window.hpp>
#include <core/GLFWevent.hpp>
#include <core/Assert.hpp>
#include <functional>
#include <algorithm>
#include <vector>
#include <map>

using eto::GLFWevent;
using std::vector;
using std::pair;

namespace eto
{

/**
 *  @brief  Hander for the raw input data
 */
class Input 
{
public:
	using CallbackType = std::function<void(const GLFWevent&)>;
	using CallbackId = unsigned long;

	/** Returns the single instance of the Input class */
	static Input &get_instance();

	/**
	 *  @brief  Configures callbacks from specified window
	 *
	 *  You should always call this function before using the Input
	 */
	void set_window(const Window &w);

	/**
	 *  @brief  Adds a callback function to the specified event type
	 *
	 *  The specified callback will be called whenever the specified event type will pop up.
	 *  The callback function must take const reference to the GLFWevent and return void.
	 *  Use lambda to wrap the needed function.
	 *  @param  type Event type to subscribe 
	 *  @param  callback The callback function
	 */
	CallbackId add_callback(GLFWevent::Type type, CallbackType callback);

	/**
	 *  @brief  Removes the specified callback
	 *
	 *  @param  type Event type to unsubscribe 
	 *  @param  callback The callback function
	 */
	void remove_callback(GLFWevent::Type type, CallbackId id);

	/**
	 *  @brief  Indicates if the key was pressed
	 *
	 *  @param  key The key code
	 *  @return True if was pressd and false if not
	 */
	bool is_key_press(input::Key key) const;

	/**
	 *  @brief  Indicates if the key was released
	 *
	 *  @param  key The key code
	 *  @return True if was released and false if not
	 */
	bool is_key_release(input::Key key) const;

	/**
	 *  @brief  Indicates if the key was held
	 *
	 *  @param  key The key code
	 *  @return True if was held and false if not
	 */
	bool is_key_held(input::Key key) const;

	/**
	 *  @brief  Returns the last cursor position
	 *
	 *  @return The cursor coordinates on the screen
	 */
	GLFWevent::CursorPositionEvent get_cursor_position() const; 

	/** Callback function for handling keyboard input */
	void on_key(int key, int scancode, int action, int mods);

	/** Callback function for handling mouse buttons input */
	void on_mouse_button(int button, int action, int mods);

	/** Callback function for handling mouse cursor position */
	void on_cursor_position(double x, double y);
private:
	Input(const Window &w);
	Input();

	void setup_callbacks();

	void notify(GLFWevent::Type t, const GLFWevent &e);


	std::map<GLFWevent::Type, vector<pair<CallbackId, CallbackType>> > m_observers;
	GLFWwindow *m_window;
	CallbackId next_callback_id;
};	

}

#endif //ETO_INPUT_HPP
