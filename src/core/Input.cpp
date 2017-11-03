#include <core/Input.hpp>

using namespace eto;

Input& Input::getInstance()
{
	static Input gInput;
	return gInput;
}

Input::Input()
	: m_window(nullptr), nextCallbackId(0)
{
}

void Input::setWindow(const Window &w)
{
	m_window = w.m_window;
	setupCallbacks();
}

void Input::setupCallbacks()
{	
	glfwSetWindowUserPointer(m_window, this);
	// some dark magic from https://stackoverflow.com/questions/7676971/
	#define genericCallback(functionName)\
		[](GLFWwindow *window, auto... args) {\
			auto ptr = static_cast<Input*>(glfwGetWindowUserPointer(window));\
			ptr->functionName(args...);\
		}

	glfwSetKeyCallback(m_window, genericCallback(onKey));
	glfwSetMouseButtonCallback(m_window, genericCallback(onMouseButton));
	glfwSetCursorPosCallback(m_window, genericCallback(onCursorPosition));

	#undef genericCallback
}

void Input::onKey(int key, int scancode, int action, int mods)
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

void Input::onMouseButton(int button, int action, int mods)
{
	GLFWevent event;
	event.type = GLFWevent::Type::MouseButton;
	GLFWevent::MouseButtonEvent &e = event.data.mouseButton;
	e.button = static_cast<input::MouseButton>(button);
	e.action = static_cast<input::Action>(action);
	e.modifier = mods;
	notify(event.type, event);
}

void Input::onCursorPosition(double x, double y)
{
	GLFWevent event;
	event.type = GLFWevent::Type::CursorPosition;
	GLFWevent::CursorPositionEvent &e = event.data.cursorPos;
	e.x = x;
	e.y = y;
	notify(event.type, event);
}

Input::CallbackId Input::addCallback(GLFWevent::Type type, CallbackType callback)
{
	CallbackId newId = nextCallbackId++;
	m_observers[type].push_back({newId, callback});
	return newId;
}

void Input::removeCallback(GLFWevent::Type type, CallbackId id)
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

bool Input::isKeyPress(input::Key key) const
{
	return glfwGetKey(m_window, key) == input::Press;
}

bool Input::isKeyRelease(input::Key key) const
{
	return glfwGetKey(m_window, key) == input::Release;
}

bool Input::isKeyHeld(input::Key key) const
{
	return glfwGetKey(m_window, key) == input::Repeat;
}

GLFWevent::CursorPositionEvent Input::getCursorPosition() const
{
	GLFWevent::CursorPositionEvent e;
	glfwGetCursorPos(m_window, &e.x, &e.y);
	return e;
}
