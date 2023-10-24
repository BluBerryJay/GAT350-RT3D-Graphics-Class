#include "Engine.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc
{
	bool Engine::Initialize()
	{
		// create systems
		m_systems.push_back(std::move(std::make_unique<Renderer>()));
		m_systems.push_back(std::move(std::make_unique<InputSystem>()));



		// initialize systems
		for (auto& system : m_systems)
		{
			system->Initialize();
		}



		GetSystem<Renderer>()->CreateWindow("My Name Is Deez", 1280, 720);
		m_systems.push_back(std::move(std::make_unique<Gui>()));
		GetSystem<Gui>()->Initialize();



		std::cout << "================" << "\n" << "Initialized Engine" << "\n" << "================" << std::endl;
		return true;
	}

	void Engine::Shutdown()
	{
		// shutdown systems
		for (auto& system : m_systems)
		{
			system->Shutdown();
		}

		m_systems.clear();
	}

	void Engine::Update()
	{
		// update sdl events
		SDL_Event event;
		SDL_PollEvent(&event);
		GetSystem<Gui>()->ProcessEvent(event);
		// update time
		m_time.Tick();

		// update systems
		for (auto& system : m_systems)
		{
			system->Update();
		}
		std::cout << "================" << "\n" << "Updated Engine" << "\n" << "================" << std::endl;
		m_quit = GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_ESCAPE);
	}
}
