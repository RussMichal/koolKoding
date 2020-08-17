#pragma once
#include "MEpch.h"//windows.h min max
#include "Vender\glm\glm.hpp"
#include "BaseBoundingObject.h"
#include "BoundingBox.h"
#include "Framework/Drawling/Object.h"

namespace MYENGINE
{
	struct BoundingSphere:BaseBoundingObject
	{
	public:
		glm::vec3 m_center;
		float m_radius;
        std::shared_ptr<Object> bs_Graphic;

		BoundingSphere(const glm::vec3& center, const float& radius);
		BoundingSphere();
		
        void SetGraphic(const unsigned int& numberOfSides, const float& radius, const glm::vec4& color);

		ContainmentType Contains(const BoundingSphere& innerSphere) const override;
		ContainmentType Contains(const BoundingBox& innerBox) const override;

		void Rotate(glm::quat Rotation);
		void Scale(const float& scale);
		void Translate(const glm::vec3& pos);
        
	private: 
        const std::vector<float> GetCircleVertices(const unsigned int& numberOfSides, const float& radius) const;
        const std::vector<unsigned int> GetCircleIndies(const unsigned int& numVertices) const;

		glm::vec3 m_originalCenter;
		float m_originalRadius;
	};
}