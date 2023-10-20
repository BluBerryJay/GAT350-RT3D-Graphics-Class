#include "World04.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>

#define INTERLEAVE

namespace nc
{
    bool World04::Initialize()
    {
        auto material = GET_RESOURCE(Material, "Materials/grid.mtrl");
        /*m_program->Use();
        m_program = GET_RESOURCE(Program, "Shaders/unlit_texture.prog");
        m_texture = GET_RESOURCE(Texture, "Textures/Llama.png");
        m_texture->Bind();
        m_texture->SetActive(GL_TEXTURE0);*/

        
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("Models/cube.obj", glm::vec3{ 0 }, glm::vec3{ -90, 0, 0 }, glm::vec3{ 1 });
        // vertex data
        
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
        ImGui::DragFloat3("Position", &m_transform.position[0]);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::DragFloat3("Scale", &m_transform.scale[0]);
        ImGui::End();
       
        ImGui::Begin("Light");
        ImGui::DragFloat3("Position", glm::value_ptr(lightPosition), 0.1f);
        ImGui::ColorEdit3("Diffuse light", glm::value_ptr(diffuseLight));
        ImGui::ColorEdit3("Ambient Light", glm::value_ptr(ambientLight));
        ImGui::End();
        

        auto material = m_model->GetMaterial();
        material->ProcessGui();
        material->Bind();

        
        material->GetProgram()->SetUniform("light.position", lightPosition);
        material->GetProgram()->SetUniform("ambientLight", ambientLight);
        material->GetProgram()->SetUniform("light.color", diffuseLight);
        
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
