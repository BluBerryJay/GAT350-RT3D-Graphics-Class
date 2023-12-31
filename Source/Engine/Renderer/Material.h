#pragma once
#include "Framework/Resource/Resource.h"
#include "GUI.h"
#include <glm/glm/glm.hpp>
#include <vector>
#include <memory>

namespace nc
{
	class Texture;
	class Program;

	class Material : public Resource
	{
	public:
		virtual bool Create(std::string filename, ...) override;

		void Bind();

		res_t<Program> GetProgram() { return m_program; }
		void ProcessGui();
	public:
		glm::vec3 diffuse;
		glm::vec3 color;
		glm::vec3 position;
		glm::vec3 specular;
		float shininess;
		glm::vec2 tiling;
		glm::vec2 offset;
	private:
		res_t<Program> m_program;
		std::vector<res_t<Texture>> m_textures;
	};
}