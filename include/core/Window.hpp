#ifndef ETO_WINDOW_HPP
#define ETO_WINDOW_HPP

//#include <GLFW/glfw3.h>
#include <queue> 
#include <string>
#include <core/GLFWevent.hpp>

namespace eto {

namespace glfw {
	bool get_init();
	bool get_will_terminate_on_exit();
}

/*!
 *  \brief Represents GLFW winodw
 */
class Window
{
public:
	struct WinSize{
		unsigned int x;
		unsigned int y;
	};

	struct WinPos {
		int x;
		int y;
	};

	Window ();                            
	Window (const Window &other) = delete;
	Window& operator = (const Window &other) = delete; 
	~Window ();                            
	
	/**
	 *  @brief  Creates window 
	 *
	 *  Creates glfw window using the specified title and window hints
	 *  @param  w, h width and height of the window
	 *  @return 0 on fail otherwise 1
	 */
	int create(int w, int h, const std::string &title, bool fullscreen = 0);

	/** @brief  Indicates whether the window should be closed or not
	 *  @return 1 if window is to be closed otherwise 0
	 */
	bool should_close() const;

	/** Sets the shouldClose flag to the specified value */
	void set_should_close(int value);

	/** Polls events form the operating system */
	void poll_events();

	/*!
	 *  \brief  Sets the spesified hint to the specified value 
	 *
	 *  Hint codes and values are identical to those of GLFW
	 *  \param  code hint code 
	 *  \param  value hint value
	 */
	void set_win_hint(int code, int value);
	
	void set_input_mode(int code, int value);
	
	void set_pos(WinPos position);
	void set_pos(int x, int y);
	WinPos get_pos() const;

	void set_size(WinSize size);
	void set_size(int w, int h);
	WinSize get_size() const;

	void swap_buffers() { glfwSwapBuffers(m_window); }

	// temporary measure
	GLFWwindow *get_raw_pointer() const { return m_window; }
private:
	friend class Input;

	GLFWwindow	     *m_window;
	WinSize     	      m_size;
	WinPos  	      m_pos;

}; // end of class Window  
} // namespace eto

#endif // ETO_WINDOW_HPP
