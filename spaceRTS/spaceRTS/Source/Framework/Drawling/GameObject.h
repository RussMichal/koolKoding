#pragma once

#include "Core.h"
#include "Framework\Drawling\Object.h"
#include "Framework\Drawling\PrimitiveObject.h"
//#include "Framework\OctTree\BoundingBox.h"
//#include "Framework\OctTree\BoundingSphere.h"
#include "Framework\OctTree\BoundingFrustum.h"
#include "Framework\OctTree\IntersectionRecord.h"
#include "Framework\ObjectLoading\AssetLoader.h"
#include "Framework\Drawling\Player.h"
#include <memory>

namespace MYENGINE
{
	class MYENGINE_API GameObject : public PrimitiveObject
	{
	public:
		GameObject(const std::vector<float> vertices, const std::vector<unsigned int> indies, const std::string& texture, const glm::vec4& color);
		GameObject(AssetProperties assetData, Player mainPlayer);
		~GameObject();

		bool m_isSelected[PlayerNumber::numPlayers];
        glm::vec3 destination;

		std::unique_ptr <Object> m_selectObjectCircle;
		std::unique_ptr <Object> m_selectObjectBox;
		std::unique_ptr <Object> m_selectObjectBoxOrig;

		BoundingBox m_bb;
		BoundingSphere m_bs;

        IntersectionRecord* Intersects(const BaseBoundingObject* BBO);


		glm::vec3 GetPosition() const;
		glm::vec3 GetScale() const;
		glm::quat GetOrientation() const;
        std::vector<WeaponBase*> GetWeapons() const;
		unsigned int ID;
        unsigned int health;
		ObjectType type;
        Player objMainPlayer;

        void SetPosition(glm::vec3 pos) override;
        void SetPositionAndDestination(glm::vec3 pos);
        void SetScale(glm::vec3 scale) override;
		void SetOrientation(glm::quat orin) override;

        void UpdateGameObject(const OctTree& octTree, const float& deltaTime);

	private:
		void ConstructObj(std::vector<float> vertexData, const std::vector<WeaponInfo>& wiVector, const Player& mainPlayer);

        virtual IntersectionRecord* Intersects(BoundingFrustum intersectionFrustum);
        virtual IntersectionRecord* Intersects(Ray intersectionRay);
        virtual IntersectionRecord* Intersects(BoundingSphere intersectionSphere);
        virtual IntersectionRecord* Intersects(BoundingBox intersectionBox);
        virtual IntersectionRecord* Intersects(GameObject* otherObj);
        virtual void HandleIntersection(IntersectionRecord& ir);

		float m_moveSpeed;
        std::unique_ptr <std::vector<WeaponBase*>> m_weapons;

        float m_time;

        void SetBoundingObjects(const std::vector<float> vertices);
		const std::vector<float> GetCircleVertices(const unsigned int& numberOfSides, const float& radius) const;
		const std::vector<unsigned int> GetCircleIndies(const unsigned int& numVertices) const;
		const std::vector<float> GetBoxVertices( const float& minX, const float& minY, const float& minZ, 
												 const float& maxX, const float& maxY, const float& maxZ ) const;
		const std::vector<unsigned int> GetBoxIndies() const;

        void Move(const float& deltaTime);
        void UpdateWeapons(const OctTree& octTree, const float& dt);

		static unsigned int objsCreated;
		static std::list<unsigned int> availableIDs;
	};
}