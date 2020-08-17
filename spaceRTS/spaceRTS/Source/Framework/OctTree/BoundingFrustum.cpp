#pragma once
#include "MEpch.h"//windows.h min max
#include "BoundingFrustum.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

namespace MYENGINE
{
	BoundingFrustum::BoundingFrustum(glm::vec3 minPoint, glm::vec3 maxPoint)
		: BaseBoundingObject(BoundingType::Frustum)
        , min(minPoint)
		, max(maxPoint) {}
	BoundingFrustum::BoundingFrustum()
		: BaseBoundingObject(BoundingType::Frustum)
        , min(glm::vec3(0))
		, max(glm::vec3(0)) {}

	//2D only?
	ContainmentType BoundingFrustum::Contains(const BoundingBox& innerBox) const
	{
		if (innerBox.min.x > this->min.x &&
			innerBox.min.y > this->min.y &&
			innerBox.max.x < this->max.x &&
			innerBox.max.y < this->max.y)
		{
			return ContainmentType::CONTAINS;
		}
		else if ( this->min.x > innerBox.min.x  &&
			  	  this->min.y > innerBox.min.y  &&
				  this->max.x < innerBox.max.x  &&
				  this->max.y < innerBox.max.y)
		{
			return ContainmentType::INTERSECTS;
		}
		else if ((this->min.x <= innerBox.max.x && this->max.x >= innerBox.min.x) &&
				 (this->min.y <= innerBox.max.y && this->max.y >= innerBox.min.y))// &&
				 //(this->min.z <= innerBox.max.z && this->max.z >= innerBox.min.z))
		{
			return ContainmentType::INTERSECTS;
		}
		else
		{
			return ContainmentType::DISJOINT;
		}
	}

	//2D only?
	ContainmentType BoundingFrustum::Contains(const BoundingSphere& innerSphere) const
	{
		//intersects
		float dist_squared = innerSphere.m_radius*innerSphere.m_radius;

		if (innerSphere.m_center.x < min.x)
		{
			dist_squared -= pow(innerSphere.m_center.x - min.x, 2);
		}
		else if (innerSphere.m_center.x > max.x)
		{
			dist_squared -= pow(innerSphere.m_center.x - max.x, 2);
		}
		if (innerSphere.m_center.y < min.y)
		{
			dist_squared -= pow(innerSphere.m_center.y - min.y, 2);
		}
		else if (innerSphere.m_center.y > max.y)
		{
			dist_squared -= pow(innerSphere.m_center.y - max.y, 2);
		}
		if (innerSphere.m_center.z < min.z)
		{
			dist_squared -= pow(innerSphere.m_center.z - min.z, 2);
		}
		else if (innerSphere.m_center.z > max.z)
		{
			dist_squared -= pow(innerSphere.m_center.z - max.z, 2);
		}

		if (dist_squared < 0)
		{
			return ContainmentType::DISJOINT;
		}

		//contianed
		if (innerSphere.m_center.x - innerSphere.m_radius >= min.x &&
			innerSphere.m_center.x + innerSphere.m_radius <= max.x &&
			innerSphere.m_center.y - innerSphere.m_radius >= min.y &&
			innerSphere.m_center.y + innerSphere.m_radius <= max.y &&
			innerSphere.m_center.z - innerSphere.m_radius >= min.z &&
			innerSphere.m_center.z + innerSphere.m_radius <= max.z)
		{
			return ContainmentType::CONTAINS;
		}
		else
		{
			return ContainmentType::INTERSECTS;
		}
	}
}