#pragma once
#include "WeaponBase.h"

namespace MYENGINE
{
    class Laser : public WeaponBase
    {
    public:
        Laser(const unsigned int&  i_range,
            const unsigned int& i_damage,
            const unsigned int& i_attackSpeed,
            const unsigned int& i_ID,
            const Player& i_player);
        ~Laser();

    private:
        void PerformAction(GameObject& target) override;
        void UpdateWeaponType(const std::vector<IntersectionRecord>& TargetsInRange, const float& dt) override;

        void UpdateGraphic(const glm::vec3& targetPos, const float& dt);

        float m_graphicTimer;
        const float m_laserTime = 0.7f;
        glm::vec3 m_targetPos;
    };
}