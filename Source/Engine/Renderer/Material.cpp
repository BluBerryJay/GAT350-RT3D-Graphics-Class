#include "Material.h"
#include "Program.h"
#include "Texture.h"
#include "Core/Core.h"

namespace nc
{
	bool Material::Create(std::string filename, ...)
	{
		// load program json document
		rapidjson::Document document;
		bool success = Json::Load(filename, document);
		if (!success)
		{
			INFO_LOG("Could not load program file (%s)." << filename);
			return false;
		}

		// read the program name
		std::string program;
		READ_DATA(document, program);
		// get program resource
		m_program = GET_RESOURCE(Program, program);
		if (m_program) {
			std::cout << "Console Log M_Program";
		}

		// read the textures name
		std::vector<std::string> textures;
		READ_DATA(document, textures);
		for (auto texture : textures)
		{
			// get texture resource
			m_textures.push_back(GET_RESOURCE(Texture, texture));
		}
		//READ_DATA(document, color);
		//READ_DATA(document, position);
		READ_DATA(document, diffuse);
		READ_DATA(document, specular);
		READ_DATA(document, shininess);
		READ_DATA(document, tiling);
		READ_DATA(document, offset);
		std::cout << "================" << "\n" << "Material Created" << "\n" << "================" << std::endl;
		return true; 
	}

	void Material::Bind()
	{
		m_program->Use();
		m_program->SetUniform("material.diffuse", diffuse);
		//m_program->SetUniform("light.color", color);
		//m_program->SetUniform("light.position", position);
		m_program->SetUniform("material.specular", specular);
		m_program->SetUniform("material.shininess", shininess);
		m_program->SetUniform("material.offset", offset);
		m_program->SetUniform("material.tiling", tiling);

		for (size_t i = 0; i < m_textures.size(); i++)
		{
			m_textures[i]->SetActive(GL_TEXTURE0 + (int)i);
			m_textures[i]->Bind();
			
		}
		std::cout << "================" << "\n" << "Material Binded!" << "\n" << "================" << std::endl;
	}
	void Material::ProcessGui()
	{
		ImGui::Begin("Material");
		//ImGui::ColorEdit3("Color", glm::value_ptr(color));
		ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuse));
		ImGui::ColorEdit3("Specular", glm::value_ptr(specular));
		//ImGui::ColorEdit3("Position", glm::value_ptr(position));
		ImGui::DragFloat("Shininess", &shininess, 0.1f, 2.0f, 200.0f);
		ImGui::DragFloat2("Tiling", glm::value_ptr(tiling));
		ImGui::DragFloat2("Offset", glm::value_ptr(offset));
		ImGui::End();
		std::cout << "================" << "\n" << "Material::ProcessGui() Called" << "\n" << "================" << std::endl;
	}
}