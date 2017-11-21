#include <core/Input.hpp>

using namespace eto;

Input& Input::get_instance()
{
	static Input gInput;
	return gInput;
}

Input::Input()
	: m_window(nullptr), next_callback_id(0)
{
}

void Input::set_window(const Window &w)
{
	m_window = w.m_window;
	setup_callbacks();
}

void Input::setup_callbacks()
{	
	glfwSetWindowUserPointer(m_window, this);
	// some dark magic from https://stackoverflow.com/questions/7676971/
	#define generic_callback(function_name)\
		[](GLFWwindow *window, auto... args) {\
			auto ptr = static_cast<Input*>(glfwGetWindowUserPointer(window));\
			ptr->function_name(args...);\
		}

	glfwSetKeyCallback(m_window, generic_callback(on_key));
	glfwSetMouseButtonCallback(m_window, generic_callback(on_mouse_button));
	glfwSetCursorPosCallback(m_window, generic_callback(on_cursor_position));

	#undef generic_callback
}

void Input::on_key(int key, int scancode, int action, int mods)
{
	GLFWevent event;
	event.type = GLFWevent::Type::Key;
	GLFWevent::KeyEvent &e = event.data.key;
	e.key = static_cast<input::Key>(key);
	e.action = static_cast<input::Action>(action);
	e.modifier = mods;
	e.scancode = scancode;
	notify(event.type, event);
}

void Input::on_mouse_button(int button, int action, int mods)
{
	GLFWevent event;
	event.type = GLFWevent::Type::MouseButton;
	GLFWevent::MouseButtonEvent &e = event.data.mouseButton;
	e.button = static_cast<input::MouseButton>(button);
	e.action = static_cast<input::Action>(action);
	e.modifier = mods;
	notify(event.type, event);
}

void Input::on_cursor_position(double x, double y)
{
	GLFWevent event;
	event.type = GLFWevent::Type::CursorPosition;
	GLFWevent::CursorPositionEvent &e = event.data.cursorPos;
	e.x = x;
	e.y = y;
	notify(event.type, event);
}

Input::CallbackId Input::add_callback(GLFWevent::Type type, CallbackType callback)
{
	CallbackId new_id = next_callback_id++;
	m_observers[type].push_back({new_id, callback});
	return new_id;
}

void Input::remove_callback(GLFWevent::Type type, CallbackId id)
{
	auto it = m_observers.find(type);
	if (it == m_observers.end())
		return;
	auto &vec = it->second;
	vec.erase(std::remove_if(vec.begin(), vec.end(), 
			[id](const pair<CallbackId, CallbackType> &i)	 {
				 return i.first == id;
			}));
}

void Input::notify(GLFWevent::Type t, const GLFWevent &e)
{
	auto it = m_observers.find(t);
	if (it != m_observers.end())
		for (auto const &i : it->second)
			i.second(e);
}

bool Input::is_key_press(input::Key key) const
{
	return glfwGetKey(m_window, key) == input::Press;
}

bool Input::is_key_release(input::Key key) const
{
	return glfwGetKey(m_window, key) == input::Release;
}

bool Input::is_key_held(input::Key key) const
{
	return glfwGetKey(m_window, key) == input::Repeat;
}

GLFWevent::CursorPositionEvent Input::get_cursor_position() const
{
	GLFWevent::CursorPositionEvent e;
	glfwGetCursorPos(m_window, &e.x, &e.y);
	return e;
}
