#ifndef ETO_CFPSCAMERA_HPP
#define ETO_CFPSCAMERA_HPP

#include <core/Input.hpp>
#include <core/Entity.hpp>
#include <components/CCamera.hpp>

namespace eto
{

class CFpsCamera : public Component
{
public:
	CFpsCamera(const Entity &parent);

	~CFpsCamera();
	
	float getSensitivity() const { return m_sens; }

	void setSensitivity(float val) { m_sens = val; }

	float getVelocity() const { return m_velocity; }

	void setVelocity(float val) { m_velocity = val; }
private:
	std::shared_ptr<CCamera> m_camera;
	bool  m_firstMouse = true;
	float m_lastX;
       	float m_lastY;
	float m_sens;
	float m_velocity;

	enum CameraMovement {
		FORWARD  = input::Key::W,
		BACKWARD = input::Key::S,
		UPWARD   = input::Key::Space,
		DOWNWARD = input::Key::C,
		RIGHT    = input::Key::D,
		LEFT 	 = input::Key::A
	};
	void processKeyboard(CameraMovement direction);
	void processMouse(float x, float y);
};

}

#endif 
