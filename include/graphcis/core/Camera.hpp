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
	Camera(const vec2 &size, const vec3 &position = vec3(0.0f));

	/**
	 *  @brief  Returns view matrix 
	 *
	 *  View matrix is already multiplied with the projection matrix
	 *  @return 4x4 matrix
	 */
	glm::mat4 get_view_matrix() const;

	/**
	 *  @brief  Returns projection matrix
	 *
	 *  Camera is unsing the perspective projection
	 *  @return 4x4 matrix
	 */
	glm::mat4 get_projection_matrix() const;

	/**
	 *  @brief  Adds specified vector to the camera position
	 *  @param  vec Values added to each axis
	 */
	void move(const vec3 &vec);

	/**
	 *  @brief  Adds specified rotations to the camera rotation
	 *  @param  rotation Angles to each axis in degrees.
	 */
	void rotate(const vec3 &rotation);

	/**
	 *  @brief  Returns the camera position
	 *  @return The camera coordinates in three-dimensional space
	 */
	vec3 get_pos() const { return m_position; }

	/**
	 *  @brief  Sets the camera position.
	 *  @param  pos The position to be set
	 */
	void set_pos(const vec3 &pos);

	/**
	 *  @brief  Returns the camera rotation
	 *  @return The camera rotation in degrees
	 */
	vec3 get_rotate() const { return m_rotation; }

	/**
	 *  @brief  Sets the camera rotation
	 *  @param  rotation The angles of each axis in degrees
	 */
	void set_rotate(const vec3 &rotation);

	/**
	 *  @brief  Returns the camera direction
	 */
	vec3 get_target() const { return m_direction; }
private:
	vec3 m_position;
       	vec3 m_direction;
	vec3 m_up;
       	vec3 m_right;
	vec3 m_world_up;
	vec3 m_rotation;
	glm::mat4 m_projection;

	float m_fov;
	float m_aspect_ratio;
	float m_near;
	float m_far;

	void update_camera();
};

}

#endif 
