#include <graphcis/core/Renderable.hpp>

using namespace eto;

Renderable::Renderable(SPtr<Model> model)
	: m_model(model), m_transform(1.0f)
{
	ETO_ASSERT(m_model != nullptr);
}

void Renderable::translate(const glm::vec3 &v)
{
	m_transform = glm::translate(m_transform, v);
}

void Renderable::rotate(float degrees, const glm::vec3 &v)
{
	m_transform = glm::rotate(m_transform, glm::radians(degrees), v);
}

void Renderable::scale(const glm::vec3 &v)
{
	m_transform = glm::scale(m_transform, v);
}

void Renderable::draw()
{
	if (! m_model->isLoaded())
		return;
	m_model->m_shader->use();
	m_model->m_shader->setMat4f("model", m_transform);
	std::string name; // very bad aproach
	for (auto it: m_model->m_meshes)
	{
		uint numDiff = 1, numSpec = 1;
		for (uint i = 0; i < it->m_material.textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			auto t = it->m_material.textures[i];
			switch (t.first) 
			{
				case Material::TexDiffuse:
					name = {"texture_diffuse" + std::to_string(numDiff)};
					++numDiff;
					break;
				case Material::TexSpecular:
					name = {"texture_specular" + std::to_string(numDiff)};
					++numSpec;
					break;
			}
			m_model->m_shader->setInt(name.c_str(), i);
			glBindTexture(t.second->m_tp.type, t.second->m_handle.id);
		}
		glActiveTexture(GL_TEXTURE0);

		m_model->m_shader->setVec3f("color_ambient", it->m_material.color_ambient);
		m_model->m_shader->setVec3f("color_diffuse", it->m_material.color_diffuse);
		m_model->m_shader->setVec3f("color_specular", it->m_material.color_specular);

		GLint prevVao = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevVao);
		glBindVertexArray(it->m_vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glDrawElements(GL_TRIANGLES, it->m_numIndices, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(prevVao);
	}
}
