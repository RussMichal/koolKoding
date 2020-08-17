#include "MEpch.h"
#include "WeaponBase.h"
#include "GameObject.h"

namespace MYENGINE
{

    WeaponBase::WeaponBase(const unsigned int& i_range, const unsigned int& i_damage, const unsigned int& i_attackSpeed, const unsigned int& i_ID, const Player& i_player)
        : range(i_range)
        , damage(i_damage)
        , attackSpeed(i_attackSpeed)
        , m_objectID(i_ID)
        , rangeSphere(glm::vec3(0.0f, 0.0f, 0.0f), range)
        , drawGraphic(false)
        , m_owningPlayer(i_player)
    {
        rangeSphere.SetGraphic(36, range, glm::vec4(1.0, 1.0, 0.0, 1.0));
    }
    WeaponBase::~WeaponBase()
    {
    }

    void WeaponBase::Translate(const glm::vec3& pos)
    {
        rangeSphere.Translate(pos);
    }
    void WeaponBase::Scale(const float& scale)
    {
        rangeSphere.Scale(scale);
    }

    BoundingSphere WeaponBase::GetRangeSphere()
    {
        return rangeSphere;
    }

    void WeaponBase::UpdateWeapon(const OctTree& octTree, const float& dt)
    {
        
        m_attackTimer += m_attackTimer>attackSpeed ? 0 : dt;
        std::vector<IntersectionRecord> targetIrs = TargetInRange(octTree);

        UpdateWeaponType(targetIrs, dt);
    }
    std::vector<IntersectionRecord> WeaponBase::TargetInRange(const OctTree& octTree)
    {
        std::vector<IntersectionRecord> irs = octTree.GetIntersectionNotOnTeam(&rangeSphere, m_owningPlayer.team);

        return irs;
    }
}