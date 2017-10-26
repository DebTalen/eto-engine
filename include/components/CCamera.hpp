#ifndef ETO_CCAMERA_HPP
#define ETO_CCAMERA_HPP

#include <graphcis/core/Camera.hpp>
#include <core/Component.hpp>
#include <memory>

namespace eto
{


/**
 *  @brief  Wraps a Camera as a Component.
 */
class CCamera : public Component
{
public:
	CCamera(const vec2 &size, const vec3 &position = vec3(0.0f)) ;

	~CCamera();

	/** @copydoc Camera::getViewMatrix */
	glm::mat4 getViewMatrix() const { return m_camera->getViewMatrix(); }

	/** @copydoc Camera::getProjectionMatrix */
	glm::mat4 getProjectionMatrix() const { return m_camera->getProjectionMatrix(); }

	/** @copydoc Camera::move */
	void move(const vec3 &vec) { m_camera->move(vec); }

	/** @copydoc Camera::rotate */
	void rotate(const vec3 &rotation) { m_camera->rotate(rotation); }

	/** @copydoc Camera::getPos */
	vec3 getPos() const { return m_camera->getPos(); } 

	/** @copydoc Camera::setPos */
	void setPos(const vec3 &pos) { m_camera->setPos(pos); }

	/** @copydoc Camera::getRotate */
	vec3 getRotate() const { return m_camera->getRotate(); }

	/** @copydoc Camera::setRotate */
	void setRotate(const vec3 &rotation) { m_camera->setRotate(rotation); }

	/** @copydoc Camera::getTarget */
	vec3 getTarget() const { return m_camera->getTarget(); }

private:
	bool m_active;
	std::unique_ptr<Camera> m_camera;
};

}

#endif
