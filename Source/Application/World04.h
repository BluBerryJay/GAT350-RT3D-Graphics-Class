#pragma once
#include "Framework/World.h"
#include "Core/Math/Vector2.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>
namespace nc
{
	class World04 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		
		float m_time = 0;
		float m_speed = 5;

		Transform m_transform;
		res_t<Model> m_model;
		
		
		glm::vec3 ambientLight;
		glm::vec3 diffuseLight;
		glm::vec3 lightPosition;

	};
}