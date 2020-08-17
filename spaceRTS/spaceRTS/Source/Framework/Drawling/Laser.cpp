#include "MEpch.h"
#include "Laser.h"
#include "GameObject.h"

namespace MYENGINE
{
    Laser::Laser(const unsigned int&  i_range,
        const unsigned int& i_damage,
        const unsigned int& i_attackSpeed,
        const unsigned int& i_ID,
        const Player& i_player)
        : WeaponBase(i_range, i_damage, i_attackSpeed, i_ID, i_player)
    {
        const std::vector<float> vertices{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
        const std::vector<unsigned int> indices{ 1, 2 };
        graphic = std::make_unique<Object>(vertices, indices, "", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), GL_LINE_STRIP);
    }

    Laser::~Laser()
    {

    }
    void Laser::UpdateWeaponType(const std::vector<IntersectionRecord>& TargetsInRange, const float& dt)
    {

        if (TargetsInRange.size() > 0 && TargetsInRange[0].GetGameObject()->ID != m_objectID)
        {
            PerformAction(*TargetsInRange[0].GetGameObject());
            UpdateGraphic(TargetsInRange[0].GetGameObject()->GetPosition(),dt);
        }
        else if (TargetsInRange.size() > 1)
        {
            PerformAction(*TargetsInRange[1].GetGameObject());
            UpdateGraphic(TargetsInRange[1].GetGameObject()->GetPosition(),dt);
        }
        else
        {
            UpdateGraphic(glm::vec3(0.0f), dt);
        }
    }

    //action is to deal damage
    void Laser::PerformAction(GameObject& target)
    {
        if (m_attackTimer > attackSpeed)
        {
            m_graphicTimer = 0;
            m_attackTimer  = 0;

            target.health -= damage;
        }
    }

    void Laser::UpdateGraphic(const glm::vec3 & targetPos, const float& dt)
    {
        if (m_graphicTimer == 0.0f)
        {
            m_targetPos = targetPos;
        }

        m_graphicTimer += m_graphicTimer > m_laserTime ? 0 : dt;

        if (m_graphicTimer < m_laserTime)
        {
            const std::vector<float> vertices{ rangeSphere.m_center.x,rangeSphere.m_center.y,rangeSphere.m_center.z, 0.0f,0.0f,  0.0f, 0.0f, 1.0f,  m_targetPos.x,m_targetPos.y,m_targetPos.z, 0.0f,0.0f,  0.0f, 0.0f, 1.0f, };
            const std::vector<unsigned int> indices{ 0, 1 };
            graphic = std::make_unique<Object>(vertices, indices, "", m_owningPlayer.color, GL_LINE_STRIP);
            drawGraphic = true;
        }
        else
        {
            drawGraphic = false;
        }


    }
}