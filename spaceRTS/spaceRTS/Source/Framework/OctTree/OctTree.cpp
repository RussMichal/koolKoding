#include "MEpch.h"
#include "OctTree.h"
#include "IntersectionRecord.h"
#include "BoundingFrustum.h"
#include "Framework\Drawling\GameObject.h"

namespace MYENGINE
{
	//static members need to be global
	bool OctTree::m_treeBuilt = false;
	bool OctTree::m_treeReady = false;
	std::queue<GameObject*> OctTree::m_pendingInsertion;

	OctTree::OctTree(BoundingBox region, std::vector<GameObject*> objList)
		: m_region(region)
		, m_objects(objList)
		, m_curLife(-1)
		, m_childNode(std::vector<OctTree*>(8))

	{
		m_treeBuilt = false;
		m_treeReady = false;
	}

	OctTree::OctTree()
		: m_region(glm::vec3(0,0,0), glm::vec3(0,0,0))
		, m_curLife(-1)
		, m_childNode(std::vector<OctTree*>(8))
	{
		m_treeBuilt = false;
		m_treeReady = false;
	}

	/// 
	/// Creates an octTree with a suggestion for the bounding region containing the items. 
	/// 

	///The suggested dimensions for the bounding region. 
	///Note: if items are outside this region, the region will be automatically resized. 
	OctTree::OctTree(BoundingBox region)
		: m_region(region)
		, m_curLife(-1)
		, m_childNode(std::vector<OctTree*>(8))
	{
		m_treeBuilt = false;
		m_treeReady = false;
	}
    std::vector<IntersectionRecord> OctTree::GetIntersectionNotOnTeam(const BaseBoundingObject* BBO, const unsigned int& isNotTeam, const unsigned int& type) const
    {
        ColitionFilter cf;
        cf.isNotOnTeam = isNotTeam;
        cf.isType = type;
        return GetIntersection(BBO, cf);
    }

    std::vector<IntersectionRecord> OctTree::GetIntersectionInPlayer(const BaseBoundingObject* BBO, const unsigned int& isPlayerNum, const unsigned int& type) const
    {
        ColitionFilter cf;
        cf.isPlayerNumber = isPlayerNum;
        cf.isType = type;
        return GetIntersection(BBO, cf);
    }

	/// <summary>
/// Gives you a list of all intersection records which intersect or are contained within the given frustum area
/// </summary>
/// <param name="frustum">The containing frustum to check for intersection/containment with</param>
/// <returns>A list of intersection records with collisions</returns>
	std::vector<IntersectionRecord> OctTree::GetIntersection(const BaseBoundingObject* BBO, const ColitionFilter& cf) const
	{
		if (!m_treeBuilt)
		{
			return std::vector<IntersectionRecord>();
		}

		if (m_objects.size() == 0 && HasChildren() == false)   //terminator for any recursion
		{
			return std::vector<IntersectionRecord>();
		}

		std::vector<IntersectionRecord> ret;

		//test each object in the list for intersection
        for (GameObject* obj : m_objects)
        {

            //skip any objects which don't meet our type criteria
            if ((int)(cf.isType & (int)obj->type) == 0 ||
                (int)(cf.isPlayerNumber & (int)obj->objMainPlayer.playerNum) == 0 ||
                (int)(cf.isOnTeam & (int)obj->objMainPlayer.team) == 0 ||

                (int)(cf.isNotType & (int)obj->type) != 0 ||
                (int)(cf.isNotPlayerNumber & (int)obj->objMainPlayer.playerNum) != 0 ||
                (int)(cf.isNotOnTeam & (int)obj->objMainPlayer.team) != 0)
            {
                continue;
            }

			//test for intersection
			IntersectionRecord* ir = obj->Intersects(BBO);//might need to set a intersection bool in ir so we can put this on the heap
			if (ir != nullptr)//ir != null
				ret.push_back(*ir);
		}

		//test each object in the list for intersection
		for (int a = 0; a < 8; a++)
		{
			if (m_childNode[a] != nullptr)
			{
				ContainmentType objectContained = BBO->Contains(m_childNode[a]->m_region);
				if (objectContained == ContainmentType::INTERSECTS || objectContained == ContainmentType::CONTAINS)
				{
					std::vector<IntersectionRecord> hitList = m_childNode[a]->GetIntersection(BBO, cf);
					if (hitList.size() != 0)//hitList != null
					{
						ret.insert(ret.end(), hitList.begin(), hitList.end());
					}
				}
			}
		}
		return ret;
	}

	void OctTree::UpdateTree() //complete & tested 
	{
		if (!m_treeBuilt)
		{
            m_objects.clear();
			while (m_pendingInsertion.size() != 0)
			{
				m_objects.push_back(m_pendingInsertion.front());
				m_pendingInsertion.pop();
			}
			DeleteTree();
			BuildTree();
			m_treeBuilt = true;
		}
		else
		{
			while (m_pendingInsertion.size() != 0)
			{
				Insert(m_pendingInsertion.front());
				m_pendingInsertion.pop();
			}
		}
		m_treeReady = true;
	}

	// <summary>
	// This finds the smallest enclosing cube with power of 2 dimensions for the given bounding box
	// </summary>
	// <param name="region">The bounding box to cubify</param>
	// <returns>A cubified version of the input parameter.</returns>
	BoundingBox OctTree::FindEnclosingCube(BoundingBox region)
	{
		//we can't guarantee that all bounding regions will be relative to the origin, so to keep the math
		//simple, we're going to translate the existing region to be oriented off the origin and remember the translation.
		//find the min offset from (0,0,0) and translate by it for a short while
		glm::vec3 offset = glm::vec3(0) - region.min;
		region.min += offset;
		region.max += offset;

		//A 3D rectangle has a length, height, and width. Of those three dimensions, we want to find the largest dimension.
		//the largest dimension will be the minimum dimensions of the cube we're creating.
		int highX = (int)ceil(std::max(std::max(region.max.x, region.max.y), region.max.z));

		//see if our cube dimension is already at a power of 2. If it is, we don't have to do any work.
		for (int bit = 0; bit < 32; bit++)
		{
			if (highX == 1 << bit)
			{
				region.max = glm::vec3((float)highX);

				region.min -= offset;
				region.max -= offset;
				return region;//BoundingBox(region.min, region.max);
			}
		}

		//We have a cube with non-power of two dimensions. We want to find the next highest power of two.
		//example: 63 -> 64; 65 -> 128;
		unsigned int x = GetHighestBit(highX) << 1;

		region.max = glm::vec3((float)highX);

		region.min -= offset;
		region.max -= offset;

		return region;//new BoundingBox(region.min, region.max);
	}

	unsigned int OctTree::GetHighestBit(int n)
	{
		//bytes
		for (int i = sizeof(int); i <0 ; i--)
		{
			//bits
			for (int j = 4; j > 0; j--)
			{
				if ((n | BIT(i*4+j)) != 0)
				{
					return BIT(i);
				}
			}
		}
		return 0;
	}

	void OctTree::BuildTree() //complete & tested
	{
		//terminate the recursion if we're a leaf node
		if (m_objects.size() <= 1)
			return;

		glm::vec3 dimensions = m_region.max - m_region.min;

		if (dimensions == glm::vec3(0))
		{
			FindEnclosingCube(m_region);
			dimensions = m_region.max - m_region.min;
		}

		//Check to see if the dimensions of the box are greater than the minimum dimensions
		if (dimensions.x <= MIN_SIZE && dimensions.y <= MIN_SIZE && dimensions.z <= MIN_SIZE)
		{
			return;
		}

		glm::vec3 half = dimensions / 2.0f;
		glm::vec3 center = m_region.min + half;

		//Create subdivided regions for each octant
		BoundingBox octant[8];
		octant[0] = BoundingBox(m_region.min, center);
		octant[1] = BoundingBox(glm::vec3(center.x, m_region.min.y, m_region.min.z), glm::vec3(m_region.max.x, center.y, center.z));
		octant[2] = BoundingBox(glm::vec3(center.x, m_region.min.y, center.z), glm::vec3(m_region.max.x, center.y, m_region.max.z));
		octant[3] = BoundingBox(glm::vec3(m_region.min.x, m_region.min.y, center.z), glm::vec3(center.x, center.y, m_region.max.z));
		octant[4] = BoundingBox(glm::vec3(m_region.min.x, center.y, m_region.min.z), glm::vec3(center.x, m_region.max.y, center.z));
		octant[5] = BoundingBox(glm::vec3(center.x, center.y, m_region.min.z), glm::vec3(m_region.max.x, m_region.max.y, center.z));
		octant[6] = BoundingBox(center, m_region.max);
		octant[7] = BoundingBox(glm::vec3(m_region.min.x, center.y, center.z), glm::vec3(center.x, m_region.max.y, m_region.max.z));

		//This will contain all of our objects which fit within each respective octant.
		std::vector<std::vector<GameObject*>> octList(8);

		//this list contains all of the objects which got moved down the tree and can be delisted from this node.
		std::vector<unsigned int> delist;

		for(unsigned int i=0; i< m_objects.size(); i++)
		{
			GameObject* obj = m_objects[i];

			if (obj->m_bb.min != obj->m_bb.max)
			{
				for (unsigned int a = 0; a < 8; a++)
				{
					if (octant[a].Contains(obj->m_bb) == ContainmentType::CONTAINS)
					{
						octList[a].push_back(obj);
						m_objects.erase(m_objects.begin()+i);
						i--;
						delist.push_back(i);
						break;
					}
				}
			}

			//if no bounding box use bounding sphere
			/*else if (obj.BoundingSphere.Radius != 0)
			{
				for (int a = 0; a < 8; a++)
				{
					if (octant[a].Contains(obj.BoundingSphere) == ContainmentType::CONTAINS)
					{
						octList[a].push_back(obj);
						i--;
						delist.push_back(obj);
						break;
					}
				}
			}*/
		}

		//delist every moved object from this node.
		//for(unsigned int removeIndex : delist)
			//m_objects.erase(m_objects.begin() + removeIndex);

		//for pointer to vec. no null trees this way... but objects are on stack!
		//{
		//	m_childNode = &(std::vector<OctTree>());
		//	m_childNode->resize(8);
		//	int x = 8;
		//}
		//std::vector<OctTree>& m_childNodeRef = *m_childNode;

		//Create child nodes where there are items contained in the bounding region
		for (int a = 0; a < 8; a++)
		{
			if (octList[a].size() != 0)
			{
				m_childNode[a] = CreateNode(octant[a], octList[a]);//need to move tree to the heap so it does not get deallocated when function ends?
				m_activeNodes |= (unsigned char)(1 << a);//unsigned char==byte??
				m_childNode[a]->BuildTree();
			}
		}

		m_treeBuilt = true;
		m_treeReady = true;
	}

	OctTree* OctTree::CreateNode(BoundingBox region, std::vector<GameObject*> objList) //complete & tested
	{
		if (objList.size() == 0)
			return new OctTree();//need to be null?
		OctTree* ret = new OctTree(region, objList);
		ret->_parent = this;
		return ret;
	}

	//should just use the above create node... or at least call it...
	OctTree* OctTree::CreateNode(BoundingBox region, GameObject* Item)
	{
		std::vector<GameObject*> objList(1); //sacrifice potential CPU time for a smaller memory footprint
		objList.push_back(Item);
		OctTree* ret = new OctTree(region, objList);
		ret->_parent = this;
		return ret;
	}
	bool OctTree::HasChildren() const 
	{
		return m_activeNodes != 0;
	}

	void OctTree::DeleteTree()
	{
		for (OctTree*& child : m_childNode)
		{
			if (child != nullptr)
			{
                child->DeleteTree();
				delete child;
				child = nullptr;
			}
		}
	}

	void OctTree::Update(std::time_t time)
	{
	}


	bool OctTree::Insert(GameObject * Item)
	{
		return false;
	}
}