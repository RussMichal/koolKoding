#include "MEpch.h"
#include "BaseBoundingObject.h"

namespace MYENGINE
{
    BaseBoundingObject::BaseBoundingObject(const BoundingType& i_boundingObj)
        : boundingObj(i_boundingObj)
    {}
    ContainmentType BaseBoundingObject::Contains(const BoundingBox & innerBox) const
    {
        return ContainmentType::DISJOINT;
    }
    ContainmentType BaseBoundingObject::Contains(const BoundingSphere & innerSphere) const
    {
        return ContainmentType::DISJOINT;
    }
}