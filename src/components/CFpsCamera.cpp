#include <components/CFpsCamera.hpp>

using namespace eto;
  
CFpsCamera::CFpsCamera(const Entity &parent)
		: m_active(true), m_firstMouse(true)
{
	m_camera = parent.getComponent<CCamera>(); // should ckeck nullptr
	m_velocity = 0.20;
	m_sens = 0.05;
	Input &input = Input::getInstance();
	m_mouseCallbackId = input.addCallback(GLFWevent::CursorPosition, [&](const GLFWevent &e) {
				processMouse(e.data.cursorPos.x, e.data.cursorPos.y);
			  });

	m_keyCallbackId = input.addCallback(GLFWevent::Key, [&](const GLFWevent &e) {
				processKeyboard(static_cast<CameraMovement>(e.data.key.key));
			  });
}

CFpsCamera::~CFpsCamera()
{
	unregisterCallbacks();
}

void CFpsCamera::unregisterCallbacks()
{
	if(m_active)
	{
		Input &input = Input::getInstance();
		input.removeCallback(GLFWevent::CursorPosition, m_mouseCallbackId);
		input.removeCallback(GLFWevent::Key, m_keyCallbackId);
		m_active = false;
	}
}

void CFpsCamera::processKeyboard(CameraMovement direction)
{
	if (m_camera.expired())	{
		unregisterCallbacks();
		return;
	}
	auto c = m_camera.lock();
	switch (direction)
	{
		case FORWARD:
			c->move(c->getTarget() * m_velocity); break;
		case BACKWARD:
			c->move(-(c->getTarget() * m_velocity)); break;
		case LEFT:
			c->move(glm::normalize(glm::cross(c->getTarget(), vec3(0,-1, 0))) * m_velocity); break;
		case RIGHT:
			c->move(glm::normalize(glm::cross(c->getTarget(), vec3(0, 1, 0))) * m_velocity); break;
		case UPWARD:
			c->move(vec3(0, 1, 0) * m_velocity); break;
		case DOWNWARD:
			c->move(vec3(0,-1, 0) * m_velocity); break;
	}
}

void CFpsCamera::processMouse(float x, float y)
{
	if (m_camera.expired())	{
		unregisterCallbacks();
		return;
	}
	if (m_firstMouse)
	{
		m_lastX = x;
		m_lastY = y;
		m_firstMouse = 0;
	}
	float xOffset = x - m_lastX;
	float yOffset = m_lastY - y;
	m_lastX = x;
	m_lastY = y;

	xOffset *= m_sens;
	yOffset *= m_sens;
	m_camera.lock()->rotate({xOffset, yOffset, 0});
}

