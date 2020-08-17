#include "MEpch.h"

#include "Scene.h"

namespace MYENGINE
{
	namespace Scene
	{
		glm::vec3 SceneSteppingStone::CalculateMouseWorldPointScene(float x, float y) {
			glm::vec2 normalizedCoords = GetNormalisedDeviceCoordinates(x, y);
			glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
			glm::vec4 eyeCoords = ToEyeCoords(clipCoords);
			glm::vec3 worldRayNear = ToWorldCoords(eyeCoords);

			clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, 1.0f, 1.0f);
			eyeCoords = ToEyeCoords(clipCoords);
			glm::vec3 worldRayFar = ToWorldCoords(eyeCoords);

			return PointOnXYPlane(worldRayFar, worldRayNear);
		}

		glm::vec2 SceneSteppingStone::GetNormalisedDeviceCoordinates(float mouseX, float mouseY) {
			int viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			int width = viewport[2] - viewport[0];
			int height = viewport[3] - viewport[1];

			float x = (2.0f * mouseX) / width - 1.0f;
			float y = (2.0f * mouseY) / height - 1.0f;
			return glm::vec2(x, -y);
		}

		glm::vec4 SceneSteppingStone::ToEyeCoords(glm::vec4 clipCoords) {
			glm::mat4 invertedProjection = glm::inverse(*m_projection);
			glm::vec4 eyeCoords = invertedProjection * clipCoords;
			return glm::vec4(eyeCoords.x, eyeCoords.y, eyeCoords.z, eyeCoords.w);
		}

		glm::vec3 SceneSteppingStone::ToWorldCoords(glm::vec4 eyeCoords) {
			glm::mat4 invertedView = m_cam.GetNonInverseView();
			glm::vec4 rayWorld = invertedView * eyeCoords;
			glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z)/ rayWorld.w;
			return mouseRay;
		}

		glm::vec3 SceneSteppingStone::PointOnXYPlane(glm::vec3 rayFar, glm::vec3 rayNear)
		{
			glm::vec3 planeP = glm::vec3(1.0f,1.0,0.0);
			glm::vec3 planeN = glm::vec3(0.0f, 0.0f, 1.0f);

			glm::vec3 rayPos = rayNear;

			glm::vec3 rayDir = glm::normalize(rayFar-rayNear);

			// assuming vectors are all normalized
			float denom = glm::dot(planeN, rayDir);
			if (abs(denom) > 1e-6) {
				glm::vec3 planePMinusRayP = planeP - rayPos;
				float t = glm::dot(planePMinusRayP, planeN) / denom;
				
				if (t >= 0)
				{
					return rayPos + rayDir*t;
				}
			}

			return glm::vec3(NAN,NAN,NAN);
		}
	}
}