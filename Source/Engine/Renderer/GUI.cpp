#include "GUI.h"
#include "Renderer.h"
#include "Framework/Engine.h"
#include <glad/include/glad/glad.h>

namespace nc
{
	bool Gui::Initialize()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		const char* glsl_version = "#version 430";
		ImGui_ImplOpenGL3_Init(glsl_version);
		ImGui_ImplSDL2_InitForOpenGL(ENGINE.GetSystem<Renderer>()->m_window, ENGINE.GetSystem<Renderer>()->m_context);

		ImGui::StyleColorsDark();

		return true;
	}

	void Gui::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void Gui::Update()
	{
		//
	}

	void Gui::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void Gui::EndFrame()
	{
		ImGui::EndFrame();
	}

	void Gui::Draw()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Gui::ProcessEvent(SDL_Event& event)
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
	}
}