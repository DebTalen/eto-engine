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
	
	/*!
	 *  \brief  Creates window 
	 *
	 *  Creates glfw window using the specified title and window hints
	 *  \param  w, h width and height of the window
	 *  \return 0 on fail otherwise 1
	 */
	int create(int w, int h, const std::string &title);
	//int create(int w, int h, std::string &&title);

	/*!
	 *  \brief  Indicates whether the window should be closed or not
	 *
	 *  \return 1 if window is to be closed otherwise 0
	 */
	bool shouldClose() const;

	/*!
	 *  \brief  Sets the shouldClose flag to the specified value
	 */
	void setShouldClose(int value);

	/*!
	 *  \brief  Polls events form the operating system
	 */
	void pollEvents();

	/*!
	 *  \brief  Sets the spesified hint to the specified value 
	 *
	 *  Hint codes and values are identical to those of GLFW
	 *  \param  code hint code 
	 *  \param  value hint value
	 */
	void setWinHint(int code, int value);

	
	/*!
	 *  \brief  Gives the las evnet from the event queue
	 *
	 *  Any input events are added to the event queue and can be obitained from this function.
	 *  The recived event is no longer stored in the queue
	 *  \param  event variable for writting the last event
	 *  \return 0 if there is no events int the queue otherwise 1
	 */
	bool getEvent(GLFWevent &event);

	void setPos(WinPos position);
	void setPos(int x, int y);
	WinPos getPos() const;

	void setSize(WinSize size);
	void setSize(int w, int h);
	WinSize getSize() const;

	/*!
	 *  \brief  Callback function for handling keyboard input
	 *
	 *  Creates Key event and adds it to the event queue
	 *  Do not call it manually
	 */
	void onKey(int key, int scancode, int action, int mods);

	/*!
	 *  \brief  Callback function for handling mouse buttons input
	 *
	 *  Creates MouseButton event and adds it to the event queue
	 *  Do not call it manually
	 */
	void onMouseButton(int button, int action, int mods);

	/*!
	 *  \brief  Callback function for handling mouse cursor position
	 *
	 *  Creates CursorPosition event and adds it to the event queue
	 *  Do not call it manually
	 */
	void onCursorPosition(double x, double y);

	GLFWwindow *getRawPointer() const { return m_window; }
private:
	void prepareCallbacks();
	void addEvent(GLFWevent &event);

	GLFWwindow	     *m_window;
	WinSize     	      m_size;
	WinPos  	      m_pos;
	std::queue<GLFWevent> m_eventQueue;

}; // end of class Window  
} // namespace eto

#endif // ETO_WINDOW_HPP
