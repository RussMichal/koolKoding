#pragma once
#include <functional>
#include <vector>
#include <memory>

#include "Core.h"
#include "Framework\Drawling\Renderer.h"
#include "DrawlingTypes\Camera.h"
#include "Framework\Log.h"
#include "Events\ApplicationEvent.h"
#include "Events\MouseEvent.h"
#include "Framework\OctTree\OctTree.h"
#include "Vender\glm\glm.hpp"
#include "Framework\ObjectLoading\\AssetLoader.h"

namespace MYENGINE
{
	namespace Scene
	{
		class MYENGINE_API Scene
		{
		public:
			Scene() {}
			virtual ~Scene() {}
			virtual void OnEngineUpdate(float deltaTime) {}
			virtual void OnUpdate(float deltaTime) {}
			virtual void OnRender() {}
			virtual void OnImGuiRender() {}
			virtual bool OnWindowResizeEvent(WindowResizeEvent& e) { return false; }
			virtual bool OnMouseButtonPress(MouseButtonPressedEvent& e) { return false; }
			virtual bool OnMouseButtonRelease(MouseButtonReleasedEvent& e) { return false; }

		};

		template class MYENGINE_API std::allocator<Object*>;
		template class MYENGINE_API std::vector<Object*>;
		template struct MYENGINE_API glm::mat<4, 4, float, glm::qualifier::defaultp>;
		template class MYENGINE_API std::unique_ptr<glm::mat4>;

		class MYENGINE_API SceneSteppingStone : public Scene
		{
		public:
			SceneSteppingStone(Renderer::ShaderType sh,
				const float cc0,
				const float cc1,
				const float cc2,
				const float cc3);
			~SceneSteppingStone();

			void OnEngineUpdate(float deltaTime) override;
			bool OnWindowResizeEvent(WindowResizeEvent& e) override;
			bool OnMouseButtonPress(MouseButtonPressedEvent& e) override;
			bool OnMouseButtonRelease(MouseButtonReleasedEvent& e) override;

		protected:
			std::vector<GameObject*> m_objects;
			Renderer m_renderer;
			Camera m_cam;
			OctTree m_octTree;
			AssetLoader al;

			unsigned int m_windowHeight;

			std::unique_ptr<glm::mat4> m_projection;
		private:
			void LeftClick(double x, double y);
			void LeftRelease(double x, double y);
			void RightRelease(double x, double y);
			glm::vec3 CalculateMouseWorldPointScene(float x, float y);
			glm::vec3 ToWorldCoords(glm::vec4 eyeCoords);
			glm::vec4 ToEyeCoords(glm::vec4 clipCoords);
			glm::vec2 GetNormalisedDeviceCoordinates(float mouseX, float mouseY);
			glm::vec3 PointOnXYPlane(glm::vec3 rayFar, glm::vec3 rayNear);

			glm::vec3 m_mouseLeftPressPos;
			bool m_leftMouseDown;
		};
		class SceneMenu :public Scene
		{
		public:

			SceneMenu(Scene*& currSence);
			virtual ~SceneMenu();

			void OnImGuiRender() override;

			template<typename T>
			void RegisterScene(const std::string& name)
			{
				ME_CORE_TRACE("Register Scene " + name);

				m_scenes.push_back(std::make_pair(name, []() {return new T(); }));
			}

		private:
			Scene*& m_curScene;
			std::vector<std::pair<std::string, std::function<Scene*()>>> m_scenes;
		};
	}
}