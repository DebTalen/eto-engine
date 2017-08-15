#include <GLFW/glfw3.h>
#include <string>

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
		int create(int w, int h, std::string &title);
		int create(int w, int h, std::string &&title);

		//--------------------------------------------------------------------------------------
		//      Method:  Window :: isShouldClose
		// Description:  Returns true if window is to be closed otherwise returns false
		//--------------------------------------------------------------------------------------
		bool isShouldClose() const;

		//--------------------------------------------------------------------------------------
		//      Method:  Window :: setShouldClose
		// Description:  Sets the ShouldClose flag to the specified value
		//--------------------------------------------------------------------------------------
		void setShouldClose(int value);

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
	private:
		GLFWwindow *m_window;
		WinSize     m_size;
		WinPos      m_pos;

}; // -----  end of class Window  ----- 
}; // namespace eto
