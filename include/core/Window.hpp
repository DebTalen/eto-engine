#ifndef ETO_WINDOW_HPP
#define ETO_WINDOW_HPP

//#include <GLFW/glfw3.h>
#include <queue> 
#include <string>
#include <core/GLFWevent.hpp>

namespace eto {

namespace glfw {
	bool GetInit();
	bool GetWillTerminateOnExit();
};

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
	bool shouldClose() const;

	/** Sets the shouldClose flag to the specified value */
	void setShouldClose(int value);

	/** Polls events form the operating system */
	void pollEvents();

	/*!
	 *  \brief  Sets the spesified hint to the specified value 
	 *
	 *  Hint codes and values are identical to those of GLFW
	 *  \param  code hint code 
	 *  \param  value hint value
	 */
	void setWinHint(int code, int value);
	
	void setInputMode(int code, int value);
	
	void setPos(WinPos position);
	void setPos(int x, int y);
	WinPos getPos() const;

	void setSize(WinSize size);
	void setSize(int w, int h);
	WinSize getSize() const;

	void swapBuffers() { glfwSwapBuffers(m_window); }

	// temporary measure
	GLFWwindow *getRawPointer() const { return m_window; }
private:
	friend class Input;
	void prepareCallbacks();

	GLFWwindow	     *m_window;
	WinSize     	      m_size;
	WinPos  	      m_pos;

}; // end of class Window  
} // namespace eto

#endif // ETO_WINDOW_HPP
