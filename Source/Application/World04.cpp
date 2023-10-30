#include "World04.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/color_space.hpp>

#define INTERLEAVE

namespace nc
{
    bool World04::Initialize()
    {
       auto material = GET_RESOURCE(Material, "Materials/grid.mtrl");
                
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("Models/plane.obj");
        // vertex data
        for (int i = 0; i < 3; i++) 
        {
            m_lights[i].type = light_t::eType::Point;
            m_lights[i].position = glm::vec3{ randomf(-5, 5), randomf(1,8), randomf(-5, 5)};
            m_lights[i].color = glm::vec3{ 1 };
            m_lights[i].range = 5;
            m_lights[i].innerAngle = 10.0f;
            m_lights[i].outerAngle = 30.0f;
            m_lights[i].direction = glm::vec3{0, -1, 0};
            m_lights[i].intensity = 1;

        }
        
        
        std::cout << "================" << "\n" << "Initialized World04" << "\n" << "================" << std::endl;
        return true;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();
        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0], 0.1f);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::DragFloat3("Scale", &m_transform.scale[0], 0.1f);
        ImGui::End();
       
        ImGui::Begin("Light");
        const char* types[] = {"Point", "Directional", "Spot"};
        ImGui::Combo("Type", (int*)(&m_lights[m_selected].type), types, 3);

        if (m_lights[m_selected].type != light_t::Directional) ImGui::DragFloat3("Position", glm::value_ptr(m_lights[m_selected].position), 0.1f);
        if (m_lights[m_selected].type != light_t::Point) ImGui::DragFloat3("Direction", glm::value_ptr(m_lights[m_selected].direction), 0.1f);
        if (m_lights[m_selected].type != light_t::Spot) {
            
            ImGui::DragFloat3("Inner Angle", &m_lights[m_selected].innerAngle, 1, 0, m_lights[m_selected].outerAngle);
            ImGui::DragFloat3("Outer Angle", &m_lights[m_selected].outerAngle, 1, m_lights[m_selected].innerAngle, 90);
        }
        ImGui::DragFloat3("Color", glm::value_ptr(m_lights[m_selected].color));
        ImGui::DragFloat("Intensity", &m_lights[m_selected].intensity, 0.1f, 0, 10);
        if (m_lights[m_selected].type != light_t::Directional) ImGui::DragFloat("Range", &m_lights[m_selected].range, 0.1f, 0.1f, 50.0f);
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(m_ambientColor));
        ImGui::End();
        

        auto material = m_model->GetMaterial();
        material->ProcessGui();
        material->Bind();

        for (int i = 0; i < 3; i++) {
            std::string name = "lights[" + std::to_string(i); +"]";
            material->GetProgram()->SetUniform(name + ".type", m_lights[i].type);
            material->GetProgram()->SetUniform(name + ".position", m_lights[i].position);
            material->GetProgram()->SetUniform(name + ".direction", glm::normalize(m_lights[i].direction));
            material->GetProgram()->SetUniform(name + ".innerAngle", glm::radians(m_lights[i].innerAngle));
            material->GetProgram()->SetUniform(name + ".outerAngle", glm::radians(m_lights[i].outerAngle));
            material->GetProgram()->SetUniform(name + ".intensity", m_lights[i].intensity);
            material->GetProgram()->SetUniform(name + ".color", m_lights[i].color);
            material->GetProgram()->SetUniform(name + ".range", m_lights[i].range);
        }
        
        material->GetProgram()->SetUniform("ambientLight", m_ambientColor);
        
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * -dt : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * +dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * -dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * +dt : 0;
        m_time += dt;
        
        // model matrix
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());
       
        // view matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3}, glm::vec3{0, 0, 0}, glm::vec3{ 0, 1, 0 });
        material->GetProgram()->SetUniform("view", view);
        
        // projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)ENGINE.GetSystem<Renderer>()->GetWidth() / ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);
      
        ENGINE.GetSystem<Gui>()->EndFrame();

        
       
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.SetColor(0, 0, 0, 0);
        renderer.BeginFrame();
        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_model->Draw(GL_TRIANGLES);
       
        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
