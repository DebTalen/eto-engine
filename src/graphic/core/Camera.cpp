#include <graphcis/core/Camera.hpp>

using namespace eto;

Camera::Camera(const vec2 &size, const vec3 &position)
	: m_position(position), m_direction(0.0f), m_rotation(0.0f)
{
	m_aspect_ratio = size.x / size.y;
	m_near = 0.01;
	m_far  = 1000;
	m_fov  = 45.0;

	m_projection = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);
	update_camera();
}

glm::mat4 Camera::get_view_matrix() const
{
	return get_projection_matrix() * glm::lookAt(m_position, m_position + m_direction, vec3(0, 1, 0));
}

glm::mat4 Camera::get_projection_matrix() const
{
	return m_projection;
}

void Camera::set_pos(const vec3& pos) 
{
	m_position = pos;
	update_camera();
}

void Camera::set_rotate(const vec3 &rotation) 
{
	m_rotation = rotation;
	update_camera();
}

void Camera::update_camera() 
{
	m_direction = glm::normalize( vec3 ( 
				 cos(glm::radians(m_rotation.x)) * cos(glm::radians(m_rotation.y)),
				 sin(glm::radians(m_rotation.y)),
				 sin(glm::radians(m_rotation.x)) * cos(glm::radians(m_rotation.y))
				 ));
	m_right = glm::normalize(glm::cross(m_direction, m_world_up));
	m_up    = glm::normalize(glm::cross(m_right, m_direction));
}

void Camera::move(const vec3 &vec)
{
	m_position += vec;
	update_camera();
}

void Camera::rotate(const vec3 &rotation)
{
	m_rotation += rotation;
	if (m_rotation.y > 89.0f)
		m_rotation.y = 89.0f;
	if (m_rotation.y < -89.0f)
		m_rotation.y = -89.0f;
	update_camera();
}
