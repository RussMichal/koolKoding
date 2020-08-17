#pragma once
#include "MEpch.h"//windows.h min max
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "Vender\glm\glm.hpp"

namespace MYENGINE
{
	BoundingSphere::BoundingSphere(const glm::vec3& center, const float& radius)
		: BaseBoundingObject(BoundingType::Sphere)
        , m_center(center)
		, m_radius(radius)
		, m_originalCenter(center)
		, m_originalRadius(radius){}
	BoundingSphere::BoundingSphere()
        : BaseBoundingObject(BoundingType::Sphere)
        , m_center(glm::vec3(0))
		, m_radius(0)
		, m_originalCenter(glm::vec3(0))
		, m_originalRadius(0){}

    void BoundingSphere::SetGraphic(const unsigned int& numberOfSides, const float& radius, const glm::vec4& color)
    {
        const std::vector<float> vertices = GetCircleVertices(numberOfSides, radius);
        const std::vector<unsigned int> indices = GetCircleIndies(vertices.size()/8);
        bs_Graphic = std::make_shared<Object>(vertices, indices, "",color);
    }


    //assumes a vertice is x,y,z, u,v, nx,ny,nz
    const std::vector<float> BoundingSphere::GetCircleVertices(const unsigned int& numberOfSides, const float& radius) const
    {
        std::vector<float> circleVertices;
        float doublePi = 2.0f * 3.14159265358979323846;
        int numberOfVertices = numberOfSides + 1;

        for (int i = 0; i < numberOfVertices; i++)
        {
            //pos
            circleVertices.push_back(0.0f + (radius * cos(i * doublePi / numberOfSides)));
            circleVertices.push_back(0.0f + (radius * sin(i * doublePi / numberOfSides)));
            circleVertices.push_back(0.0f);

            //tex coord
            circleVertices.push_back(0.0f);
            circleVertices.push_back(0.0f);

            //normal
            circleVertices.push_back(0.0f);
            circleVertices.push_back(0.0f);
            circleVertices.push_back(1.0f);
        }

        return circleVertices;
    }

    const std::vector<unsigned int> BoundingSphere::GetCircleIndies(const unsigned int& numVertices) const
    {
        std::vector<unsigned int> circleIndies;
        for (unsigned int i = 0; i < numVertices; i++)
        {
            circleIndies.push_back(i);
        }
        circleIndies.push_back(0);

        return circleIndies;
    }

	ContainmentType BoundingSphere::Contains(const BoundingSphere& innerSphere) const
	{
		float distSquared = sqrt(pow((innerSphere.m_center.x - this->m_center.x), 2) + pow((innerSphere.m_center.y - this->m_center.y), 2) + pow((innerSphere.m_center.z - this->m_center.z), 2));
		if (distSquared<abs(this->m_radius-innerSphere.m_radius))
		{
			return ContainmentType::CONTAINS;
		}
		//leave this to else?
		else if (abs(distSquared- this->m_radius) < innerSphere.m_radius)
		{
			return ContainmentType::INTERSECTS;
		}
		else if (distSquared > this->m_radius + innerSphere.m_radius)
		{
			return ContainmentType::DISJOINT;
		}
		else
		{
			return ContainmentType::INTERSECTS;
		}
	}

	ContainmentType BoundingSphere::Contains(const BoundingBox& innerBox) const
	{
		//intersects
		float dist_squared = m_radius*m_radius;

		if (m_center.x < innerBox.min.x)
		{
			dist_squared -= pow(m_center.x - innerBox.min.x, 2);
		}
		else if (m_center.x > innerBox.max.x)
		{
			dist_squared -= pow(m_center.x - innerBox.max.x, 2);
		}
		if (m_center.y < innerBox.min.y)
		{
			dist_squared -= pow(m_center.y - innerBox.min.y, 2);
		}
		else if (m_center.y > innerBox.max.y)
		{
			dist_squared -= pow(m_center.y - innerBox.max.y, 2);
		}
		if (m_center.z < innerBox.min.z)
		{
			dist_squared -= pow(m_center.z - innerBox.min.z, 2);
		}
		else if (m_center.z > innerBox.max.z)
		{
			dist_squared -= pow(m_center.z - innerBox.max.z, 2);
		}

		if (dist_squared < 0)
		{
			return ContainmentType::DISJOINT;
		}

		//contianed
		if (m_center.x - m_radius >= innerBox.min.x &&
			m_center.x + m_radius <= innerBox.max.x &&
			m_center.y - m_radius >= innerBox.min.y &&
			m_center.y + m_radius <= innerBox.max.y &&
			m_center.z - m_radius >= innerBox.min.z &&
			m_center.z + m_radius <= innerBox.max.z)
		{
			return ContainmentType::CONTAINS;
		}
		else
		{
			return ContainmentType::INTERSECTS;
		}
	}

	void BoundingSphere::Rotate(glm::quat Rotation)
	{

	}

	void BoundingSphere::Scale(const float& scale)
	{
		m_radius = m_originalRadius * scale;
	}

	void BoundingSphere::Translate(const glm::vec3& pos)
	{
		m_center = m_originalCenter + pos;
	}
}