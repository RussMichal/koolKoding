#include "MEpch.h"
#include "GameObject.h"
#include "Object.h"

namespace MYENGINE
{
    //static members need to be global
    unsigned int GameObject::objsCreated = 0;
    std::list<unsigned int> GameObject::availableIDs;

    GameObject::GameObject(const std::vector<float> vertices, const std::vector<unsigned int> indies, const std::string& texture, const glm::vec4& color)
        : PrimitiveObject(vertices, indies, texture, color)
        , m_isSelected()
        , objMainPlayer(PlayerNumber::Player1,Team::Team1,glm::vec4(1.0f,0.0f,0.0f,1.0f))
    {
        //ConstructObj(vertices);
    }
    GameObject::GameObject(AssetProperties assetData, Player mainPlayer)
        : PrimitiveObject(assetData.vertexData, assetData.indices, assetData.texture, mainPlayer.color)
        , m_isSelected()
        , m_moveSpeed(assetData.moveSpeed)
        , health(assetData.health)
        //, m_weapons(assetData.weapons)
        , type(assetData.type)
        , objMainPlayer(mainPlayer)
	{
		ConstructObj(assetData.vertexData, assetData.weapons, mainPlayer);
	}
	void GameObject::ConstructObj(std::vector<float> vertexData, const std::vector<WeaponInfo>& wiVector, const Player& mainPlayer)
	{
		if (availableIDs.size() > 0)
		{
			ID = availableIDs.front();
			availableIDs.erase(availableIDs.begin());
		}
		else
		{
			ID = objsCreated++;
		}
        SetBoundingObjects(vertexData);
        m_weapons = std::make_unique<std::vector<WeaponBase*>>();
        for (WeaponInfo wi : wiVector)
        {
            if (wi.weaponType == WeaponType::Laser)
            {
                class Laser* weap = new class Laser(wi.range,
                                                   wi.damage,
                                                   wi.attackSpeed,
                                                   ID,
                                                   mainPlayer);
                m_weapons->push_back(weap);
            }
        }
	}

	GameObject::~GameObject()
	{
		availableIDs.push_back(ID);
	}

	//assumes a vertice is x,y,z, u,v, nx,ny,nz
	void GameObject::SetBoundingObjects(const std::vector<float> vertices)
	{
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float minZ = std::numeric_limits<float>::max();

		float maxX = -std::numeric_limits<float>::max();
		float maxY = -std::numeric_limits<float>::max();
		float maxZ = -std::numeric_limits<float>::max();

		float maxDist = -std::numeric_limits<float>::max();
		float extraSpace = 0.0f;
		for (unsigned int i = 0; i < vertices.size(); i+=8)
		{
			minX = std::min(vertices[i], minX);
			maxX = std::max(vertices[i], maxX);

			minY = std::min(vertices[i + 1], minY);
			maxY = std::max(vertices[i + 1], maxY);

			minZ = std::min(vertices[i + 2], minZ);
			maxZ = std::max(vertices[i + 2], maxZ);

			maxDist = std::max((vertices[i + 1] * vertices[i + 1]) + (vertices[i] * vertices[i]), maxDist);
		}
		const glm::vec3 center = glm::vec3((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2);
		maxDist = sqrt(maxDist) + extraSpace;

		m_bb = BoundingBox(glm::vec3(minX, minY, minZ), glm::vec3(maxX, maxY, maxZ));
		m_bs = BoundingSphere(center, maxDist);

		//const std::vector<float> selectCircleVertices = GetCircleVertices(36, maxDist);
		//const std::vector<unsigned int> selectCircleIndies = GetCircleIndies(selectCircleVertices.size()/8);
		//m_selectObjectCircle = std::make_unique<Object>(selectCircleVertices, selectCircleIndies, "", glm::vec4(0.0, 1.0, 0.0, 1.0));
        m_bs.SetGraphic(36, maxDist, glm::vec4(0.0, 1.0, 0.0, 1.0));

		const std::vector<float> selectBoxVertices = GetBoxVertices(minX,minY,minZ,maxX,maxY,maxZ);
		const std::vector<unsigned int> selectBoxIndies = GetBoxIndies();
		m_selectObjectBox = std::make_unique<Object>(selectBoxVertices, selectBoxIndies, "", glm::vec4(0.2, 0.0, 1.0, 1.0), GL_LINE_STRIP);
		m_selectObjectBoxOrig = std::make_unique<Object>(selectBoxVertices, selectBoxIndies, "", glm::vec4(0.2, 0.4, 1.0, 1.0), GL_LINE_STRIP);
	}

	#pragma region Intersection
    IntersectionRecord* GameObject::Intersects(const BaseBoundingObject* BBO)
    {
        switch (BBO->boundingObj)
        {
        case BoundingType::Box:
                return Intersects(*(BoundingBox*)(void*)BBO);
                break;
        case BoundingType::Frustum:
            return Intersects(*(BoundingFrustum*)(void*)BBO);
            break;
        case BoundingType::Sphere:
            return Intersects(*(BoundingSphere*)(void*)BBO);
            break;
        }
    }
	// <summary>
	// Tells you if the bounding regions for this object [intersect or are contained within] the bounding frustum
	// </summary>
	// <param name="intersectionFrustum">The frustum to do bounds checking against</param>
	// <returns>An intersection record containing any intersection information, or null if there isn't any
	// </returns>
	IntersectionRecord* GameObject::Intersects(BoundingFrustum intersectionFrustum)
	{
		if (m_bb.max - m_bb.min != glm::vec3(0))
		{
			ContainmentType ct = intersectionFrustum.Contains(m_bb);
			if (ct != ContainmentType::DISJOINT)
			{

				Builder buildIR = Builder();
				buildIR.Object1 = this;

				return buildIR.Build();
				//return new IntersectionRecord.Builder() { Object1 = this }.Build();
				//return new IntersectionRecord(this);
			}
		}
		else if (m_bs.m_radius != 0.0f)
		{
			ContainmentType ct = intersectionFrustum.Contains(m_bs);
			if (ct != ContainmentType::DISJOINT)
			{
				Builder buildIR = Builder();
				buildIR.Object1 = this;

				return buildIR.Build();
				//return new IntersectionRecord.Builder() { Object1 = this }.Build();
			}
		}
		else
		{
			throw std::invalid_argument("no bounding area for this object!");
		}

		return nullptr;
	}

	IntersectionRecord* GameObject::Intersects(Ray intersectionRay)
	{

		float f = NAN;
		if (m_bb.min != m_bb.max)
		{
			//f = m_bb.Intersects(intersectionRay);
		}
		else if (m_bs.m_radius != 0)
		{
			//f = m_bs.Intersects(intersectionRay);
		}

		if (f != NAN)
		{
			/*return new IntersectionRecord.Builder()
			{
				Distance = f.Value,
					Object1 = this,
					hitRay = intersectionRay,
					Position = intersectionRay.Position + (intersectionRay.Direction * f.Value)
			}
			.Build();*/
		}
		return nullptr;
	}

	// <summary>
	// Coarse collision check: Tells you if this object intersects with the given intersection sphere.
	// </summary>
	// <param name="intersectionSphere">The intersection sphere to check against</param>
	// <returns>An intersection record containing this object</returns>
	// <remarks>You'll want to override this for granular collision detection</remarks>
	IntersectionRecord* GameObject::Intersects(BoundingSphere intersectionSphere)
	{
		if (m_bb.max != m_bb.min && m_bb.Contains(intersectionSphere) != ContainmentType::DISJOINT)
		{
			Builder buildIR = Builder();
			buildIR.Object1 = this;

			return buildIR.Build();
			//return new IntersectionRecord.Builder() { Object1 = this }.Build();
		}

		else if (m_bs.m_radius != 0.0f && m_bs.Contains(intersectionSphere) != ContainmentType::DISJOINT)
		{
			Builder buildIR = Builder();
			buildIR.Object1 = this;

			return buildIR.Build();
			//return new IntersectionRecord.Builder() { Object1 = this }.Build();
		}	
		return nullptr;
	}

	/// <summary>
	/// Coarse collision check: Tells you if this object intersects with the given intersection box.
	/// </summary>
	/// <param name="intersectionBox">The intersection box to check against</param>
	/// <returns>An intersection record containing this object</returns>
	/// <remarks>You'll want to override this for granular collision detection</remarks>
	IntersectionRecord* GameObject::Intersects(BoundingBox intersectionBox)
	{
		if (m_bb.max != m_bb.min)
		{
			ContainmentType ct = m_bb.Contains(intersectionBox);
			if (ct != ContainmentType::DISJOINT)
			{
				Builder buildIR = Builder();
				buildIR.Object1 = this;

				return buildIR.Build();
				//return new IntersectionRecord.Builder() { Object1 = this }.Build();
			}
		}
		else if (m_bs.m_radius != 0.0f)
		{
			if (m_bs.Contains(intersectionBox) != ContainmentType::DISJOINT)
			{
				Builder buildIR = Builder();
				buildIR.Object1 = this;

				return buildIR.Build();
				//return new IntersectionRecord.Builder() { Object1 = this }.Build();
			}
		}

		return nullptr;
	}

	/// <summary>
	/// Tests for intersection with this object against the other object
	/// </summary>
	/// <param name="otherObj">The other object to test for intersection against</param>
	/// <returns>Null if there isn't an intersection, an intersection record if there is a hit.</returns>
	IntersectionRecord* GameObject::Intersects(GameObject* otherObj)
	{
		IntersectionRecord* ir;

		if (otherObj->m_bb.min != otherObj->m_bb.max)
		{
			ir = Intersects(otherObj->m_bb);
			Builder buildIR = Builder();
			buildIR.Object1 = this;
			buildIR.Object2 = otherObj;

			return ir;
			//return buildIR.Build();			
		}
		else if (otherObj->m_bs.m_radius != 0.0f)
		{
			ir = Intersects(otherObj->m_bs);
			Builder buildIR = Builder();
			buildIR.Object1 = this;
			buildIR.Object2 = otherObj;

			return ir;
			//return buildIR.Build();
		}
		else
		{
			return nullptr;
		}

		//if (ir != nullptr)
		//{
			//ir.PhysicalObject = this;
			//ir.OtherPhysicalObject = otherObj;
		//}

		//return new IntersectionRecord.Builder() { Object1 = this, Object2 = otherObj }.Build();
	}

	void GameObject::HandleIntersection(IntersectionRecord& ir)
	{

	}
	#pragma endregion

	//assumes a vertice is x,y,z, u,v, nx,ny,nz
	const std::vector<float> GameObject::GetCircleVertices(const unsigned int& numberOfSides, const float& radius) const
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

	const std::vector<unsigned int> GameObject::GetCircleIndies(const unsigned int& numVertices) const
	{
		std::vector<unsigned int> circleIndies;
		for (unsigned int i = 0; i < numVertices; i++)
		{
			circleIndies.push_back(i);
		}
		circleIndies.push_back(0);

		return circleIndies;
	}

	//assumes a vertice is x,y,z, u,v, nx,ny,nz
	const std::vector<float> GameObject::GetBoxVertices(const float& minX, const float& minY, const float& minZ,
														const float& maxX, const float& maxY, const float& maxZ) const
	{
		const std::vector<float> boxVertices =
		{
			minX, minY, minZ,  0.0f,0.0f,  0.0f, 0.0f, 1.0f,
			maxX, minY, minZ,  0.0f,0.0f,  0.0f, 0.0f, 1.0f,
			maxX, maxY, minZ,  0.0f,0.0f,  0.0f, 0.0f, 1.0f,
			minX, maxY, minZ,  0.0f,0.0f,  0.0f, 0.0f, 1.0f,

			minX, minY, maxZ,  0.0f,0.0f,  0.0f, 0.0f, 1.0f,
			maxX, minY, maxZ,  0.0f,0.0f,  0.0f, 0.0f, 1.0f,
			maxX, maxY, maxZ,  0.0f,0.0f,  0.0f, 0.0f, 1.0f,
			minX, maxY, maxZ,  0.0f,0.0f,  0.0f, 0.0f, 1.0f,
		};
		return boxVertices;
	}

	const std::vector<unsigned int> GameObject::GetBoxIndies() const
	{
		const std::vector<unsigned int> boxIndies =
		{
			0,1,2,3,0,
			4,5,6,7,4,
			5,1,2,6,7,3
		};
		return boxIndies;
	}

	glm::vec3 GameObject::GetPosition() const
	{
		return m_position;
	}
	glm::vec3 GameObject::GetScale() const
	{
		return m_scale;
	}

	glm::quat GameObject::GetOrientation() const
	{
		return m_orientation;
	}

    std::vector<WeaponBase*> GameObject::GetWeapons() const
    {
        return *m_weapons;
    }

    void GameObject::SetPositionAndDestination(glm::vec3 pos)
    {
        destination = pos;
        SetPosition(pos);
    }

	void GameObject::SetPosition(glm::vec3 pos)
	{
		m_position = pos;
		m_bb.Translate(pos);
        m_bs.Translate(pos);
        for (WeaponBase* weap : *m_weapons)
        {
            weap->Translate(pos);
        }

		const std::vector<float> selectBoxVertices = GetBoxVertices(m_bb.min.x, m_bb.min.y, m_bb.min.z, m_bb.max.x, m_bb.max.y, m_bb.max.z);
		const std::vector<unsigned int> selectBoxIndies = GetBoxIndies();
		m_selectObjectBox = std::make_unique<Object>(selectBoxVertices, selectBoxIndies, "", glm::vec4(0.2, 0.0, 1.0, 1.0), GL_LINE_STRIP);
	}

	void GameObject::SetScale(glm::vec3 scale)
	{
        const float maxScaleFactor = std::max(std::max(scale.x, scale.y), scale.z);
        
        m_scale = scale;
		m_bb.Scale(scale);
        m_bs.Scale(maxScaleFactor);
        for (WeaponBase* weap : *m_weapons)
        {
            //weap->Scale(maxScaleFactor);
        }

		const std::vector<float> selectBoxVertices = GetBoxVertices(m_bb.min.x, m_bb.min.y, m_bb.min.z, m_bb.max.x, m_bb.max.y, m_bb.max.z);
		const std::vector<unsigned int> selectBoxIndies = GetBoxIndies();
		m_selectObjectBox = std::make_unique<Object>(selectBoxVertices, selectBoxIndies, "", glm::vec4(0.2, 0.0, 1.0, 1.0), GL_LINE_STRIP);
	}

	void GameObject::SetOrientation(glm::quat orin)
	{
		m_orientation = orin;
		m_bb.Rotate(orin);
		//std::vector<float> minmax = m_bb.RotateOrig(orin);
		
		//glm::vec3 min = m_bb.min / m_scale;
		//glm::vec3 max = m_bb.max / m_scale;
	
		//min = min - m_position;
		//max = max - m_position;

		//const std::vector<float> selectBoxVertices = GetBoxVertices(minmax[0], minmax[1], minmax[2], minmax[3], minmax[4], minmax[5]);
		const std::vector<float> selectBoxVertices = GetBoxVertices(m_bb.min.x, m_bb.min.y, m_bb.min.z, m_bb.max.x, m_bb.max.y, m_bb.max.z);
		const std::vector<unsigned int> selectBoxIndies = GetBoxIndies();
		m_selectObjectBox = std::make_unique<Object>(selectBoxVertices, selectBoxIndies, "", glm::vec4(0.2, 0.0, 1.0, 1.0), GL_LINE_STRIP);
	}

    void GameObject::UpdateGameObject(const OctTree& octTree, const float& deltaTime)
    {
        Move(deltaTime);
        UpdateWeapons(octTree, deltaTime);
    }
	void GameObject::Move(const float& deltaTime)
	{
		float dt = deltaTime;
        dt *= 60;
			//const float speed = 0.5;

			//glm::vec3 dir =  glm::normalize(pos - m_position);
		float angle = -1 * (glm::radians(90.0f) - glm::eulerAngles(m_orientation).z);
		//angle in rads
		//float angle = -1*(glm::radians(90.0f)-atan2(pos.y, pos.x));
		//float degAngle = glm::degrees(angle);
		//SetOrientation(glm::quat(glm::vec3(0.0f, 0.0f, angle)));
        glm::vec3 diff = m_position - destination;
        float dist = glm::dot(diff, diff);
        if (dist > pow(m_moveSpeed* dt, 2))
        {
            float x = -m_moveSpeed * dt * cos(angle);
            float y = -m_moveSpeed * dt * sin(angle);
            SetPosition(m_position + glm::vec3(x, y, 0));
        }
	}
    void GameObject::UpdateWeapons(const OctTree& octTree, const float& dt)
    {
        for (WeaponBase* weapon : *m_weapons)
        {
            weapon->UpdateWeapon(octTree, dt);
        }
    }
    
}