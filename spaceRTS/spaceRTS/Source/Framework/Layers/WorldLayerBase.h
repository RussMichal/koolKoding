#pragma once
#include <functional>
#include <vector>
#include <memory>

#include "Core.h"
#include "Framework\Drawling\Renderer.h"
#include "DrawlingTypes\Camera.h"
#include "Framework\Log.h"
#include "Events\ApplicationEvent.h"
#include "Platform/Windows/WindowsInput.h"
#include "Events\MouseEvent.h"
#include "Framework\OctTree\OctTree.h"
#include "Vender\glm\glm.hpp"
#include "Framework\ObjectLoading\\AssetLoader.h"
#include "Framework/Layers/Layer.h"

namespace MYENGINE
{
	class MYENGINE_API WorldLayerBase : public Layer
	{
	public:
		WorldLayerBase(Renderer::ShaderType sh,
            Player mainPlayer,
			const float cc0,
			const float cc1,
			const float cc2,
			const float cc3);
		~WorldLayerBase();

		void OnEngineUpdate(const float& deltaTime) override;

		bool OnWindowResizeEvent(const WindowResizeEvent& e);
		bool OnMouseButtonPress(const MouseButtonPressedEvent& e);
		bool OnMouseButtonRelease(const MouseButtonReleasedEvent& e);

	protected:
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		std::vector<GameObject*> m_objects;
		std::vector<PrimitiveObject*> m_primitiveObjects;
		Renderer m_renderer;
		Camera m_cam;
		OctTree m_octTree;
		AssetLoader al;
		WindowsInput inputPulling;
        Player m_mainPlayer;
		unsigned int m_windowHeight;

		std::unique_ptr<glm::mat4> m_projection;
	private:
		void OnEvent(Event& event) override;
		void LeftClick(const MouseButtonPressedEvent& e);
		void LeftRelease(const MouseButtonReleasedEvent& e);
		void RightRelease(const MouseButtonReleasedEvent& e);
		glm::vec3 CalculateMouseWorldPoint(float x, float y);
		glm::vec3 ToWorldCoords(glm::vec4 eyeCoords);
		glm::vec4 ToEyeCoords(glm::vec4 clipCoords);
		glm::vec2 GetNormalisedDeviceCoordinates(float mouseX, float mouseY);
		glm::vec3 PointOnXYPlane(glm::vec3 rayFar, glm::vec3 rayNear);

        //number of players
		glm::vec3 m_mouseLeftPressPos[PlayerNumber::numPlayers];
	};
}