#include <core/Window.hpp>
#include <cstdlib>

using namespace eto;

namespace eto {
	namespace glfw {
		bool isActive = glfwInit();
		bool isAtExit = std::atexit(glfwTerminate);
		bool GetInit() {
			return isActive;
		}
	}
}

Window::Window() 
	:m_window(nullptr), m_size{0, 0}, m_pos{0, 0}
{
}

/* int Window::create(int w, int h, std::string &title)
{
	if (m_window) 
		return 0;
	setWinHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	setWinHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	setWinHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	m_window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
	if (m_window == NULL)
		return 0;
	m_size.x = w;
	m_size.y = h;
	prepareCallbacks();
	return 1;
} */

int Window::create(int w, int h, const std::string &title)
{
	if (m_window) 
		return 0;
	setWinHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	setWinHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	setWinHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	m_window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
	if (m_window == NULL)
		return 0;
	m_size.x = w;
	m_size.y = h;
	prepareCallbacks();
	return 1;
}

Window::~Window() 
{
	if (m_window) 
		glfwDestroyWindow(m_window);
}

bool Window::shouldClose() const 
{
	return glfwWindowShouldClose(m_window);
}

void Window::setShouldClose(int value) 
{
	glfwSetWindowShouldClose(m_window, value);
}

void Window::pollEvents() 
{
	glfwPollEvents();
}

void Window::setWinHint(int code, int value)
{
	glfwWindowHint(code, value);
}

void Window::setPos(WinPos position)
{
	m_pos = position;
	glfwSetWindowPos(m_window, m_pos.x, m_pos.y);
}

void Window::setPos(int x, int y)
{
	m_pos.x = x;
	m_pos.y = y;
	glfwSetWindowPos(m_window, x, y);
}

void Window::setSize(WinSize size)
{
	if (size.x > 1 && size.y > 1)
	{
		m_size = size;
		glfwSetWindowSize(m_window, size.x, size.y);
	}
}

Window::WinPos Window::getPos() const 
{
	return m_pos;
}

void Window::setSize(int w, int h)
{
	if (w > 1 && h > 1)
	{
		m_size.x = w;
		m_size.y = h;
		glfwSetWindowSize(m_window, w, h);
	}
}

Window::WinSize Window::getSize() const 
{
	return m_size;
}

void Window::addEvent(GLFWevent &event)
{
	if (m_eventQueue.size() > 100) // change constant value to variable
		m_eventQueue.pop();
	m_eventQueue.push(event);
}

bool Window::getEvent(GLFWevent &event) 
{
	if (m_eventQueue.empty())
		return 0;
	event = m_eventQueue.front();
	m_eventQueue.pop();
	return 1;
}

void Window::prepareCallbacks() 
{
	glfwSetWindowUserPointer(m_window, this);

	// some dark magic from https://stackoverflow.com/questions/7676971/
	#define genericCallback(functionName)\
		[](GLFWwindow *window, auto... args) {\
			auto ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));\
			ptr->functionName(args...);\
		}

	glfwSetKeyCallback(m_window, genericCallback(onKey));
	glfwSetMouseButtonCallback(m_window, genericCallback(onMouseButton));
	glfwSetCursorPosCallback(m_window, genericCallback(onCursorPosition));
}

void Window::onKey(int key, int scancode, int action, int mods)
{
	GLFWevent event;
	event.type = GLFWevent::Type::Key;
	event.key.key = static_cast<Input::Key>(key);
	event.key.action = static_cast<Input::Action>(action);
	event.key.modifier = mods;
	event.key.scancode = scancode;
	addEvent(event);
}

void Window::onMouseButton(int button, int action, int mods)
{
	GLFWevent event;
	event.type = GLFWevent::Type::MouseButton;
	event.mouseButton.button = static_cast<Input::MouseButton>(button);
	event.mouseButton.action = static_cast<Input::Action>(action);
	event.mouseButton.modifier = mods;
	addEvent(event);
}

void Window::onCursorPosition(double x, double y)
{
	GLFWevent event;
	event.type = GLFWevent::Type::CursorPosition;
	event.cursorPos.x = x;
	event.cursorPos.y = y;
	addEvent(event);
}
