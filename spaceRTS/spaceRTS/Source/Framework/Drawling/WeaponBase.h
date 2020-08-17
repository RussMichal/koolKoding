#pragma once
#include "Framework/OctTree/OctTree.h"
#include "Vender/glm/glm.hpp"
#include "Player.h"
#include "Core.h"

namespace MYENGINE
{
    class MYENGINE_API WeaponBase
    {
    public:
        WeaponBase(const unsigned int&  i_range,
               const unsigned int& i_damage,
               const unsigned int& i_attackSpeed,
               const unsigned int& i_ID,
               const Player& i_player);
        ~WeaponBase();

        void Translate(const glm::vec3& pos);
        void Scale(const float& scale);

        BoundingSphere GetRangeSphere();

        void UpdateWeapon(const OctTree& octTree, const float& dt);

        unsigned int range;
        unsigned int damage;
        unsigned int attackSpeed;
        BoundingSphere rangeSphere;
        bool drawGraphic;
        std::unique_ptr<Object> graphic;


    protected:
        virtual void UpdateWeaponType(const std::vector<IntersectionRecord>& TargetsInRange, const float& dt) = 0;

        float m_attackTimer;
        const unsigned int m_objectID;
        Player m_owningPlayer;

    private:
        std::vector<IntersectionRecord> TargetInRange(const OctTree& octTree);
        virtual void PerformAction(GameObject& target) = 0;
        
    };
}