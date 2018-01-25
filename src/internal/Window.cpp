#include <internal/Window.hpp>
#include <cstdlib>

using namespace eto::internal;

namespace eto {
	namespace glfw {
		bool is_active = glfwInit();
		bool is_at_exit = std::atexit(glfwTerminate);
		bool get_init() {
			return is_active;
		}
	}
}

Window::Window() 
	:m_window(nullptr), m_size{0, 0}, m_pos{0, 0}
{
}

int Window::create(int w, int h, const std::string &title, bool fullscreen)
{
	if (m_window) 
		return 0;
	set_win_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	set_win_hint(GLFW_CONTEXT_VERSION_MINOR, 3);
	set_win_hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	if (fullscreen)
		m_window = glfwCreateWindow(w, h, title.c_str(), glfwGetPrimaryMonitor(), NULL);
	else
		m_window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);
	if (m_window == NULL)
		return 0;
	m_size.x = w;
	m_size.y = h;
	return 1;
}

Window::~Window() 
{
	if (m_window) 
		glfwDestroyWindow(m_window);
}

bool Window::should_close() const 
{
	return glfwWindowShouldClose(m_window);
}

void Window::set_should_close(int value) 
{
	glfwSetWindowShouldClose(m_window, value);
}

void Window::poll_events() 
{
	glfwPollEvents();
}

void Window::set_win_hint(int code, int value)
{
	glfwWindowHint(code, value);
}

void Window::set_input_mode(int code, int value)
{
	glfwSetInputMode(m_window, code, value);
}

void Window::set_pos(WinPos position)
{
	m_pos = position;
	glfwSetWindowPos(m_window, m_pos.x, m_pos.y);
}

void Window::set_pos(int x, int y)
{
	m_pos.x = x;
	m_pos.y = y;
	glfwSetWindowPos(m_window, x, y);
}

void Window::set_size(WinSize size)
{
	if (size.x > 1 && size.y > 1)
	{
		m_size = size;
		glfwSetWindowSize(m_window, size.x, size.y);
	}
}

Window::WinPos Window::get_pos() const 
{
	return m_pos;
}

void Window::set_size(int w, int h)
{
	if (w > 1 && h > 1)
	{
		m_size.x = w;
		m_size.y = h;
		glfwSetWindowSize(m_window, w, h);
	}
}

Window::WinSize Window::get_size() const 
{
	return m_size;
}
