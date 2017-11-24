#ifndef ETO_FPS_CAMERA_SYSTEM
#define ETO_FPS_CAMERA_SYSTEM

#include <core/Input.hpp>
#include <core/EntityManager.hpp>
#include <graphcis/core/Camera.hpp>

namespace eto 
{
class FPSCameraSystem 
{
public:
	FPSCameraSystem(EntityManager &manager)
		: m_manager(&manager)
	{
		m_velocity = 0.25;
		m_sens= 0.05;
		first = true;
	}

	void update()
	{
		for (auto e: m_manager->with<Camera>())
		{
			auto &cam = e.get<Camera>();
			process_mouse(cam);
			process_keyboard(cam);
		}
	}

	float get_velocity() const { return m_velocity; }
	void set_velocity(float v) { m_velocity = v; }

	float get_sensitivity() const { return m_sens; }
	void set_sensitivity(float v) { m_sens = v; } 
private:

	void process_keyboard(Camera &c)
	{
		Input &input = Input::get_instance();
		if (input.is_key_press(input::Key::W))
		{
			c.position += (c.direction * m_velocity);
		}
		if (input.is_key_press(input::Key::S))
		{
			c.position -= (c.direction * m_velocity);
		}
		if (input.is_key_press(input::Key::A))
		{
			c.position -= c.right * m_velocity;
		}
		if (input.is_key_press(input::Key::D))
		{
			c.position += c.right * m_velocity;
		}
		if (input.is_key_press(input::Key::Space))
		{
			c.position += (c.world_up * m_velocity);
		}
		if (input.is_key_press(input::Key::C))
		{
			c.position -= (c.world_up * m_velocity);
		}
		c.update_camera();
	}

	void process_mouse(Camera &c)
	{
		auto pos = Input::get_instance().get_cursor_position();
		if (first)
		{
			last_x = pos.x;
			last_y = pos.y;
			first = 0;
		}
		float x_offset = pos.x - last_x;
		float y_offset = last_y - pos.y;
		last_x = pos.x;
		last_y = pos.y;

		x_offset *= m_sens;
		y_offset *= m_sens;
		c.rotation += vec3(x_offset, y_offset, 0);
		if (c.rotation.y > 89.0f)
			c.rotation.y = 89.0f;
		if (c.rotation.y < -89.0f)
			c.rotation.y = -89.0f;
		c.update_camera();
	}

	EntityManager *m_manager;
	float m_velocity;
	float m_sens;
	bool  first;
	float last_x, last_y;
};
}

#endif 
