#include <components/CFpsCamera.hpp>

using namespace eto;
  
CFpsCamera::CFpsCamera(const Entity &parent)
		: m_firstMouse(true)
{
	m_camera = parent.getComponent<CCamera>();
	m_velocity = 0.20;
	m_sens = 0.05;
	Input &input = Input::getInstance();
	input.addCallback(GLFWevent::Type::CursorPosition, [&](const GLFWevent &e) {
			  processMouse(e.data.cursorPos.x, e.data.cursorPos.y);
			  });

	input.addCallback(GLFWevent::Key, [&](const GLFWevent &e) {
			  processKeyboard(static_cast<CameraMovement>(e.data.key.key));
			  });
}

CFpsCamera::~CFpsCamera()
{
	Input &input = Input::getInstance();
	input.removeCallback(GLFWevent::Type::CursorPosition, [&](const GLFWevent &e) {
			     processMouse(e.data.cursorPos.x, e.data.cursorPos.y);
			     });

	input.removeCallback(GLFWevent::Key, [&](const GLFWevent &e) {
			     processKeyboard(static_cast<CameraMovement>(e.data.key.key));
			     });
}

void CFpsCamera::processKeyboard(CameraMovement direction)
{
	auto c = m_camera;
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
	//	_pos->y = 0; for fixed floor
}

void CFpsCamera::processMouse(float x, float y)
{
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
	m_camera->rotate({xOffset, yOffset, 0});
}


