#pragma once
#include "Vender\glm\glm.hpp"
#include "Vender\glm\gtc\quaternion.hpp"

#include "Core.h"

namespace glm
{
    inline bool operator >(const glm::vec3& lhs, const glm::vec3& rhs)
    {
        return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z;
    }

    inline bool operator <(const glm::vec3& lhs, const glm::vec3& rhs)
    {
        return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
    }
}

namespace MYENGINE
{
    enum ContainmentType
    {
        DISJOINT,
        INTERSECTS,
        CONTAINS
    };
    enum BoundingType
    {
        Sphere,
        Box,
        Frustum,
        RayType
    };
    struct BoundingBox;
    struct BoundingSphere;
    struct MYENGINE_API BaseBoundingObject
    {
    public:
        BaseBoundingObject(const BoundingType& i_boundingObj);

        virtual ContainmentType Contains(const BoundingBox& innerBox) const;
        virtual ContainmentType Contains(const BoundingSphere& innerSphere) const;
        BoundingType boundingObj;
    };
}