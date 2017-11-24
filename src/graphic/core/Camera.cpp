#include <graphcis/core/Camera.hpp>

using namespace eto;

Camera::Camera(const vec2 &size, const vec3 &position)
	: position(position), direction(0.0f), world_up(0, 1, 0), rotation(0.0f) 
{
	aspect_ratio = size.x / size.y;
	near = 0.01;
	far  = 1000;
	fov  = 45.0;

	projection = glm::perspective(glm::radians(fov), aspect_ratio, near, far);
	update_camera();
}

glm::mat4 Camera::get_view_matrix() const
{
	return projection * glm::lookAt(position, position + direction, vec3(0, 1, 0));
}


void Camera::update_camera() 
{
	direction = glm::normalize( vec3 ( 
				 cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y)),
				 sin(glm::radians(rotation.y)),
				 sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y))
				 ));
	right = glm::normalize(glm::cross(direction, world_up));
	up    = glm::normalize(glm::cross(right, direction));
}

