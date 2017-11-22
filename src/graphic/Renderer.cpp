#include <graphcis/Renderer.hpp>
/*
using namespace eto;


// most naive implementation by now

Renderer& Renderer::get_instance()
{
	static Renderer renderer{};
	return renderer;
}

Renderer::Renderer()
{

}

void Renderer::add_cLight(shared_ptr<CLight> pCLight)
{
	m_lights.push_back(pCLight);
}
void Renderer::remove_cLight(shared_ptr<CLight> pCLight)
{
	m_lights.erase(std::remove_if(m_lights.begin(), m_lights.end(), 
				      [&](auto const& i) {
				      		return i == pCLight;
				      }));
}

void Renderer::add_cRenderable(shared_ptr<CRenderable> pRend)
{
	m_rends.push_back(pRend);
}

void Renderer::remove_cRenderable(shared_ptr<CRenderable> pRend)
{
	m_rends.erase(std::remove_if(m_rends.begin(), m_rends.end(), 
				     [&](auto const& i) {
				     		return i == pRend;
				     }));
}

void Renderer::render()
{
	for(shared_ptr<CRenderable> &r : m_rends)
	{
		if (! r->is_active()) 
			continue;

		shared_ptr<Model> model = r->m_rend.m_model;
		shared_ptr<ShaderProgram> shader = r->m_rend.m_model->get_shader_program();
		shader->use();

		// aplying lights
		for(shared_ptr<CLight> &l : m_lights)
		{
			if (! l->is_active())
				continue;

			auto &lt = l->m_light;
			shader->set_vec3f("light.ambient", lt.m_color.ambient);
			shader->set_vec3f("light.diffuse", lt.m_color.diffuse);
			shader->set_vec3f("light.specular", lt.m_color.specular);
			shader->set_vec3f("light.position", lt.m_position);
			shader->set_vec3f("light.direction", lt.m_direction);
			shader->set_int("light.type", static_cast<int>(lt.m_type));
			if (lt.m_type == Light::Point || lt.m_type == Light::Spot)
			{
				shader->set_float("light.constant", lt.m_att.constant);
				shader->set_float("light.linear", lt.m_att.linear);
				shader->set_float("light.quadratic", lt.m_att.quadratic);
				if (lt.m_type == Light::Spot) 
				{
					shader->set_float("light.inner_cur_off", lt.m_inner_cut_off);
					shader->set_float("light.outer_cur_off", lt.m_outer_cut_off);
				}
			}
		}

		shader->set_mat4f("model", r->m_rend.m_transform);
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
*/
