#ifndef ETO_RENDERER_HPP
#define ETO_RENDERER_HPP

#include <graphcis/core/Light.hpp>
#include <graphcis/core/Renderable.hpp>
#include <graphcis/core/Camera.hpp>
#include <core/EntityManager.hpp>

namespace eto
{

class Renderer 
{
private:
	EntityManager *m_manager;
public:
	Renderer(EntityManager &manager)
		: m_manager(&manager)
	{
	}

	void render()
	{
		for (auto e : m_manager->with<Renderable>())
		{
			auto &r = e.get<Renderable>();

			auto model = r.model;
			auto shader = r.model->get_shader_program();
			shader->use();
			for (auto c : m_manager->with<Camera>())
			{
				auto &camera = c.get<Camera>();
				shader->set_mat4f("view", camera.get_view_matrix());
				shader->set_vec3f("view_position", camera.position);
			}

			// aplying lights
			for(auto l : m_manager->with<Light>())
			{
				auto &lt = l.get<Light>();
				shader->set_vec3f("light.ambient", lt.color.ambient);
				shader->set_vec3f("light.diffuse", lt.color.diffuse);
				shader->set_vec3f("light.specular", lt.color.specular);
				shader->set_vec3f("light.position", lt.position);
				shader->set_vec3f("light.direction", lt.direction);
				shader->set_int("light.type", static_cast<int>(lt.type));
				if (lt.type == Light::Point || lt.type == Light::Spot)
				{
					shader->set_float("light.constant", lt.att.constant);
					shader->set_float("light.linear", lt.att.linear);
					shader->set_float("light.quadratic", lt.att.quadratic);
					if (lt.type == Light::Spot) 
					{
						shader->set_float("light.inner_cur_off", lt.inner_cut_off);
						shader->set_float("light.outer_cur_off", lt.outer_cut_off);
					}
				}
			}

			shader->set_mat4f("model", r.transform);
			std::string name; // very bad aproach
			for (auto it: model->m_meshes)
			{
				// aplying textures
				uint num_diff = 1, num_spec = 1;
				for (uint i = 0; i < it->m_material.textures.size(); ++i)
				{
					glActiveTexture(GL_TEXTURE0 + i);
					auto t = it->m_material.textures[i];
					switch (t.first) 
					{
						case Material::TexDiffuse:
							name = {"texture_diffuse" + std::to_string(num_diff)};
							++num_diff;
							break;
						case Material::TexSpecular:
							name = {"texture_specular" + std::to_string(num_diff)};
							++num_spec;
							break;
					}
					shader->set_int(name.c_str(), i);
					glBindTexture(t.second->m_tp.type, t.second->m_handle.id);
				}
				glActiveTexture(GL_TEXTURE0);

				shader->set_vec3f("color_ambient", it->m_material.color_ambient);
				shader->set_vec3f("color_diffuse", it->m_material.color_diffuse);
				shader->set_vec3f("color_specular", it->m_material.color_specular);

				GLint prev_vao = 0;
				glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prev_vao);
				glBindVertexArray(it->m_vao);
				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glEnableVertexAttribArray(2);
				glDrawElements(GL_TRIANGLES, it->m_num_indices, GL_UNSIGNED_INT, 0);
				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(2);
				glBindVertexArray(prev_vao);
			}
		}
	}
};

}

#endif 
