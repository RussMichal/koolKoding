#pragma once
#include "MEpch.h"
#include "Vender\glm\glm.hpp"
//#include "IntersectionRecord.h"
#include "Framework\Drawling\GameObject.h"

namespace MYENGINE
{

	Builder::Builder()
		:Distance(std::numeric_limits<double>::max())
	{

	}

	Builder::Builder(IntersectionRecord copy)
		: Position(copy.GetPosition())
		, Normal(copy.GetNormal())
		, Object1(copy.GetGameObject())
		, Object2(copy.GetOtherGameObject())
		, hitRay(copy.GetRay())
		, Distance(copy.Distance())
	{

	}

	IntersectionRecord* Builder::Build()
	{
		return new IntersectionRecord(Position, Normal, Object1, Object2, hitRay, Distance);//might need to move to heap
	}


	#pragma region Constructors
	IntersectionRecord::IntersectionRecord(glm::vec3 pos, glm::vec3 normal, GameObject* obj1, GameObject* obj2, Ray r, double dist)
	{
		m_position = pos;
		m_normal = normal;
		m_intersectedObject1 = obj1;
		m_intersectedObject2 = obj2;
		m_ray = r;
		m_distance = dist;
	}
	#pragma endregion

	#pragma region Accessors
	// <summary>
	// This is the exact point in 3D space which has an intersection.
	// </summary>
	glm::vec3 IntersectionRecord::GetPosition() const { return m_position; }

	// <summary>
	// This is the normal of the surface at the point of intersection
	// </summary>
	glm::vec3 IntersectionRecord::GetNormal() const { return m_normal; }

	// <summary>
	// This is the ray which caused the intersection
	// </summary>
	Ray IntersectionRecord::GetRay() const { return m_ray; }

	// <summary>
	// This is the object which is being intersected
	// </summary>
	GameObject* IntersectionRecord::GetGameObject() const { return m_intersectedObject1; }

	// <summary>
	// This is the other object being intersected (may be null, as in the case of a ray-object intersection)
	// </summary>
	GameObject* IntersectionRecord::GetOtherGameObject() const { return m_intersectedObject2; }

	// <summary>
	// This is the distance from the ray to the intersection point. 
	// You'll usually want to use the nearest collision point if you get multiple intersections.
	// </summary>
	double IntersectionRecord::Distance() const { return m_distance; }

	#pragma endregion

	#pragma Overrides
	std::string IntersectionRecord::ToString() const //override
	{
		return "Hit: Object1";// +m_intersectedObject1.ToString();
	}

	int IntersectionRecord::GetHashCode() //override
	{
		//return base->GetHashCode();
		return 0;
	}
	// <summary>
	// check the object identities between the two intersection records. If they match in either order, we have a duplicate.
	// </summary>
	// <param name="otherRecord">the other record to compare against</param>
	// <returns>true if the records are an intersection for the same pair of objects, false otherwise.</returns>
	bool IntersectionRecord::Equals(IntersectionRecord* otherRecord) //override
	{
		IntersectionRecord o = (IntersectionRecord)*otherRecord;
		//
		//return (m_intersectedObject1 != null && m_intersectedObject2 != null && m_intersectedObject1.ID == m_intersectedObject2.ID);
		if (otherRecord == nullptr)
			return false;
		if (o.m_intersectedObject1->ID == m_intersectedObject1->ID && o.m_intersectedObject2->ID == m_intersectedObject2->ID)
			return true;
		if (o.m_intersectedObject1->ID == m_intersectedObject2->ID && o.m_intersectedObject2->ID == m_intersectedObject1->ID)
			return true;
		return false;
	}
	#pragma endregion

}