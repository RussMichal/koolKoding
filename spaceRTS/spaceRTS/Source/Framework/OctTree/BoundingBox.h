#pragma once
#include "MEpch.h"//windows.h min max
#include "Vender\glm\glm.hpp"
#include "Vender\glm\gtc\quaternion.hpp"
#include "BaseBoundingObject.h"
#include "Core.h"

namespace MYENGINE
{
	struct MYENGINE_API BoundingBox:BaseBoundingObject
	{
	public:
		glm::vec3 min;
		glm::vec3 max;

		BoundingBox(glm::vec3 minPoint, glm::vec3 maxPoint);
		BoundingBox();
		
		ContainmentType Contains(const BoundingBox& innerBox) const override;
		ContainmentType Contains(const BoundingSphere& innerSphere) const override;

		void Rotate(const glm::quat& Rotation);
		void Scale(const glm::vec3& scale);
		void Translate(const glm::vec3& pos);

	private: 
		glm::vec3 originalMin;
		glm::vec3 originalMax;

		glm::vec3 m_scale;
		glm::quat m_rotation;
		glm::vec3 m_position;

		void setMinMax();
	};
}