#pragma once
#include "MEpch.h"//windows.h min max
#include "Vender\glm\glm.hpp"
#include "BaseBoundingObject.h"
//#include "BoundingBox.h"
#include "BoundingSphere.h"

namespace MYENGINE
{
	struct BoundingFrustum : BaseBoundingObject
	{
	public:
		glm::vec3 min;
		glm::vec3 max;

		BoundingFrustum(glm::vec3 minPoint, glm::vec3 maxPoint);
		BoundingFrustum();

		//2D only
		ContainmentType Contains(const BoundingBox& innerBox) const override;
		ContainmentType Contains(const BoundingSphere& innerSphere) const override;
	};
}