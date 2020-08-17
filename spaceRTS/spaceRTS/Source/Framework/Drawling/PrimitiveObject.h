#pragma once
#include "Framework\Drawling\Object.h"
#include "Framework\ObjectLoading\AssetLoader.h"

namespace MYENGINE
{
	class PrimitiveObject
	{
	public:
		PrimitiveObject(const std::vector<float> vertices, const std::vector<unsigned int> indies, const std::string& texture, const glm::vec4& color);
		PrimitiveObject(const AssetProperties& assetData, const glm::vec4& color);

		~PrimitiveObject();

		Object m_mainObject;

		glm::vec3 GetPosition() const;
		glm::vec3 GetScale() const;
		glm::quat GetOrientation() const;

		virtual void SetPosition(glm::vec3 pos);
		virtual void SetScale(glm::vec3 scale);
		virtual void SetOrientation(glm::quat orin);
	protected:
		glm::vec3 m_position;
		glm::vec3 m_scale;
		glm::quat m_orientation;
	private:
	};
}