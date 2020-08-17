#pragma once
#include "Vender\glm\glm.hpp"

namespace MYENGINE
{
	class GameObject;
	struct Ray
	{
		glm::vec3 position;
		glm::vec3 direction;
	};

	class IntersectionRecord
	{
	private:
		glm::vec3 m_position;
		glm::vec3 m_normal;

		Ray m_ray;

		GameObject* m_intersectedObject1;
		GameObject* m_intersectedObject2;

		double m_distance;
	public:

		#pragma region Constructors
		IntersectionRecord(glm::vec3 pos, glm::vec3 normal, GameObject* obj1, GameObject* obj2, Ray r, double dist);
		#pragma endregion

		#pragma region Accessors
		// <summary>
		// This is the exact point in 3D space which has an intersection.
		// </summary>
		glm::vec3 GetPosition() const;

		// <summary>
		// This is the normal of the surface at the point of intersection
		// </summary>
		glm::vec3 GetNormal() const;

		// <summary>
		// This is the ray which caused the intersection
		// </summary>
		Ray GetRay() const;

		// <summary>
		// This is the object which is being intersected
		// </summary>
		GameObject* GetGameObject() const;

		// <summary>
		// This is the other object being intersected (may be null, as in the case of a ray-object intersection)
		// </summary>
		GameObject* GetOtherGameObject() const;

		// <summary>
		// This is the distance from the ray to the intersection point. 
		// You'll usually want to use the nearest collision point if you get multiple intersections.
		// </summary>
		double Distance() const;

		#pragma endregion

		#pragma region Overrides
		std::string ToString() const; //override

		int GetHashCode(); //override

		// <summary>
		// check the object identities between the two intersection records. If they match in either order, we have a duplicate.
		// </summary>
		// <param name="otherRecord">the other record to compare against</param>
		// <returns>true if the records are an intersection for the same pair of objects, false otherwise.</returns>
		bool Equals(IntersectionRecord* otherRecord); //override
		#pragma endregion
	};

	class Builder
	{
	public:
		glm::vec3 Position;
		glm::vec3 Normal;
		GameObject* Object1;
		GameObject* Object2;
		Ray hitRay;
		double Distance;

		Builder();

		Builder(IntersectionRecord copy);

		IntersectionRecord* Build();
	};
}