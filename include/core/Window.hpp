#ifndef ETO_WINDOW_HPP
#define ETO_WINDOW_HPP

#include <GLFW/glfw3.h>
#include <queue> 
#include <string>
#include <core/GLFWevent.hpp>

namespace eto {

struct WinSize{
	unsigned int x;
	unsigned int y;
};

struct WinPos {
	int x;
	int y;
};

namespace glfw {
	bool GetInit();
	bool GetWillTerminateOnExit();
}

// =====================================================================================
//        Class:  Window
//  Description:  Class for handling glfw window
// =====================================================================================
class Window
{
public:
	Window ();                            
	Window (const Window &other) = delete;
	Window& operator = (const Window &other) = delete; 
	~Window ();                            
	
	//--------------------------------------------------------------------------------------
	//      Method:  Window :: create
	// Description:  Creates glfw window
	// 		 Returns 0 if fails otherwise 1
	//--------------------------------------------------------------------------------------
	///int create(int w, int h, std::string &title);
	int create(int w, int h, std::string &&title);

	//--------------------------------------------------------------------------------------
	//      Method:  Window :: shouldClose
	// Description:  Returns true if window is to be closed otherwise returns false
	//--------------------------------------------------------------------------------------
	bool shouldClose() const;

	//--------------------------------------------------------------------------------------
	//      Method:  Window :: setShouldClose
	// Description:  Sets the ShouldClose flag to the specified value
	//--------------------------------------------------------------------------------------
	void setShouldClose(int value);

	//--------------------------------------------------------------------------------------
	//      Method:  Window :: pollEvents
	// Description:  Poll events from the system
	//--------------------------------------------------------------------------------------
	void pollEvents();

	//--------------------------------------------------------------------------------------
	//      Method:  Window :: setWindowHint
	// Description:  Sets the specified hint to specified value
	//--------------------------------------------------------------------------------------
	void setWinHint(int code, int value);

	void setPos(WinPos position);
	void setPos(int x, int y);
	WinPos getPos() const;

	void setSize(WinSize size);
	void setSize(int w, int h);
	WinSize getSize() const;
	
	//--------------------------------------------------------------------------------------
	//      Method:  Window :: getEvent
	// Description:  Writes the last event from the event queue
	// 		 Returns 0 if there are no events in the queue othrewise returns 1
	//--------------------------------------------------------------------------------------
	bool getEvent(GLFWevent &event);

	/*
	 * Callback functions 
	  *All these functions create a corresponding event and add it to the event queue
	*/ 
	//--------------------------------------------------------------------------------------
	//      Method:  Window :: onKey
	// Description:  Callback function for handling keyboard input
	//--------------------------------------------------------------------------------------
	void onKey(int key, int scancode, int action, int mods);

	//--------------------------------------------------------------------------------------
	//      Method:  Window :: onMouseButton
	// Description:  Callback function for handling mouse button input  
	//--------------------------------------------------------------------------------------
	void onMouseButton(int button, int action, int mods);

	//--------------------------------------------------------------------------------------
	//      Method:  Window :: onCursorPosition
	// Description:  Callback function for hanling cursor position
	//--------------------------------------------------------------------------------------
	void onCursorPosition(double x, double y);
private:
	void prepareCallbacks();
	void addEvent(GLFWevent &event);

	GLFWwindow	     *m_window;
	WinSize     	      m_size;
	WinPos  	      m_pos;
	std::queue<GLFWevent> m_eventQueue;

}; // -----  end of class Window  ----- 
} // namespace eto

#endif // ETO_WINDOW_HPP
