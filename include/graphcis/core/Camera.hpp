#ifndef ETO_CAMERA_HPP
#define ETO_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec2;
using glm::vec3;

namespace eto
{

/**
 *  @brief  Camera is used to oriento bjects to a certain point of view
 */
class Camera
{
public:
	/**
	 *  @brief  Creates the Camera object
	 *
	 *  @param  size The width and height of the view that are used as the aspect ratio
	 *  @param  position Camera position in three-dimensional space
	 */
	Camera(const vec2 &size = vec2(800, 600), const vec3 &position = vec3(0.0f));

	/**
	 *  @brief  Returns view matrix 
	 *
	 *  View matrix is already multiplied with the projection matrix
	 *  @return 4x4 matrix
	 */
	glm::mat4 get_view_matrix() const;


	/** Use whenever some of members changed*/
	void update_camera();

	vec3 position;
       	vec3 direction;
	vec3 up;
       	vec3 right;
	vec3 world_up;
	vec3 rotation;
	glm::mat4 projection;

	float fov;
	float aspect_ratio;
	float near;
	float far;

};

}

#endif 
