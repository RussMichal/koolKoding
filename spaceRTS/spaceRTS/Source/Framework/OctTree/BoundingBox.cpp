#pragma once
#include "MEpch.h"//windows.h min max
//#include "BoundingBox.h"
#include "BoundingSphere.h"

namespace MYENGINE
{
	BoundingBox::BoundingBox(glm::vec3 minPoint, glm::vec3 maxPoint)
		: BaseBoundingObject(BoundingType::Box)
        , min(minPoint)
		, max(maxPoint)
		, originalMin(minPoint)
		, originalMax(maxPoint)
		, m_scale(glm::vec3(1))
		, m_rotation(glm::quat(glm::vec3(0, 0, 0)))
		, m_position(glm::vec3(0)){}
	BoundingBox::BoundingBox()
	    : BaseBoundingObject(BoundingType::Box)
        , min(glm::vec3(0))
		, max(glm::vec3(0))
		, originalMin(glm::vec3(0))
		, originalMax(glm::vec3(0))
		, m_scale(glm::vec3(1))
		, m_rotation(glm::quat(glm::vec3(0, 0, 0)))
		, m_position(glm::vec3(0)) {}
		
	ContainmentType BoundingBox::Contains(const BoundingBox& innerBox) const
	{
		if (innerBox.min > this->min &&
			innerBox.max < this->max)
		{
			return ContainmentType::CONTAINS;
		}
		// If one rectangle is on left side of other 
		else if (innerBox.min.x > this->max.x || innerBox.max.x > this->min.x)
		{
			return ContainmentType::DISJOINT;
		}

		// If one rectangle is above other 
		else if (innerBox.min.y < this->max.y || innerBox.max.y < this->min.y)
		{
			return ContainmentType::DISJOINT;
		}
		else
		{
			return ContainmentType::INTERSECTS;
		}
	}

	ContainmentType BoundingBox::Contains(const BoundingSphere& innerSphere) const
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

	void BoundingBox::Rotate(const glm::quat& Rotation)
	{
		m_rotation = Rotation;
		setMinMax();
	}

	void BoundingBox::Scale(const glm::vec3& scale)
	{
		m_scale = scale;
		setMinMax();
	}

	void BoundingBox::Translate(const glm::vec3& pos)
	{
		m_position = pos;
		setMinMax();
	}

	void BoundingBox::setMinMax()
	{
		std::vector<glm::vec3> points;
		points.push_back(m_rotation * originalMin);
		points.push_back(m_rotation * glm::vec3(originalMax.x, originalMin.y, originalMin.z));
		points.push_back(m_rotation * glm::vec3(originalMin.x, originalMax.y, originalMin.z));
		points.push_back(m_rotation * glm::vec3(originalMax.x, originalMax.y, originalMin.z));
		points.push_back(m_rotation * originalMax);
		points.push_back(m_rotation * glm::vec3(originalMax.x, originalMin.y, originalMax.z));
		points.push_back(m_rotation * glm::vec3(originalMin.x, originalMax.y, originalMax.z));
		points.push_back(m_rotation * glm::vec3(originalMax.x, originalMax.y, originalMax.z));//*/

		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float minZ = std::numeric_limits<float>::max();

		float maxX = std::numeric_limits<float>::min();
		float maxY = std::numeric_limits<float>::min();
		float maxZ = std::numeric_limits<float>::min();

		for (glm::vec3 point : points)
		{
			minX = std::min(point.x, minX);
			maxX = std::max(point.x, maxX);

			minY = std::min(point.y, minY);
			maxY = std::max(point.y, maxY);

			minZ = std::min(point.z, minZ);
			maxZ = std::max(point.z, maxZ);
		}

		glm::vec3 rotMin = glm::vec3(minX, minY, minZ);
		glm::vec3 rotMax = glm::vec3(maxX, maxY, maxZ);
		
		min = rotMin * m_scale + m_position;
		max = rotMax * m_scale + m_position;
	}
}