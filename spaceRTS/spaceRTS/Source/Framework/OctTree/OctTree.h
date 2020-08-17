#pragma once
#include "Vender\glm\glm.hpp"
#include "Framework\OctTree\BoundingBox.h"
#include "IntersectionRecord.h"
#include "BoundingFrustum.h"
#include "Framework/ObjectLoading/AssetEnums.h"

#include <vector>
#include <queue>
namespace MYENGINE
{
    struct GameObject;
	class OctTree
	{

    //must define colitionfilter before using it
    private:
        struct ColitionFilter
        {
            unsigned int isPlayerNumber=63;
            unsigned int isNotPlayerNumber=0;
            unsigned int isOnTeam = 63;
            unsigned int isNotOnTeam=0;
            unsigned int isType = 63;
            unsigned int isNotType=0;
        };

	public:
		// Creates an octTree with a suggestion for the bounding region containing the items.  

		//The suggested dimensions for the bounding region. 
		//Note: if items are outside this region, the region will be automatically resized
		OctTree(BoundingBox region);
		OctTree();

        std::vector<IntersectionRecord> GetIntersectionInPlayer(const BaseBoundingObject* BBO, const unsigned int& isPlayerNum, const unsigned int& type = ObjectType::ALL) const;
        std::vector<IntersectionRecord> GetIntersectionNotOnTeam(const BaseBoundingObject* BBO, const unsigned int& isNotTeam, const unsigned int& type = ObjectType::ALL) const;

		std::vector<IntersectionRecord> GetIntersection(const BaseBoundingObject* BBO, const ColitionFilter& cf) const;

		void Update(std::time_t time);
		static BoundingBox FindEnclosingCube(BoundingBox region);

		BoundingBox m_region;
		std::vector<GameObject*> m_objects;

		// These are items which we're waiting to insert into the data structure. 
		// We want to accrue as many objects in here as possible before we inject them into the tree. This is slightly more cache friendly. 
		static std::queue<GameObject*> m_pendingInsertion;

		// These are all of the possible child octants for this node in the tree. 
		std::vector<OctTree*> m_childNode;

		// This is a bitmask indicating which child nodes are actively being used. 
		// It adds slightly more complexity, but is faster for performance since there is only one comparison instead of 8. 
		//byte??
		unsigned char m_activeNodes = 0;

		// this is how many frames we'll wait before deleting an empty tree branch. Note that this is not a constant. The maximum lifespan doubles
		// every time a node is reused, until it hits a hard coded constant of 64 
		int m_maxLifespan = 8; // 
		int m_curLife = -1; //this is a countdown time showing how much time we have left to live 

		// A reference to the parent node is nice to have when we're trying to do a tree update. 
		OctTree* _parent;
		static bool m_treeReady; //the tree has a few objects which need to be inserted before it is complete 
		static bool m_treeBuilt; //there is no pre-existing tree yet. 

		// Processes all pending insertions by inserting them into the tree. 
		// Consider deprecating this? 
		void UpdateTree(); //complete & tested 

	private:
		// The minumum size for enclosing region is a 1x1x1 cube. 
		int MIN_SIZE = 1;

		/*Note: we want to avoid allocating memory for as long as possible since there can be lots of nodes.*/
		// Creates an oct tree which encloses the given region and contains the provided objects. 


		//The bounding region for the oct tree. 
		//The list of objects contained within the bounding region 
		OctTree(BoundingBox region, std::vector<GameObject*> objList);

		OctTree* CreateNode(BoundingBox region, std::vector<GameObject*> objList); //complete & tested
		OctTree* CreateNode(BoundingBox region, GameObject* Item);
		bool HasChildren() const;
		void DeleteTree();

		static unsigned int GetHighestBit(int n);

		// Naively builds an oct tree from scratch.
		void BuildTree(); //complete & tested

		// <summary>
		// A tree has already been created, so we're going to try to insert an item into the tree without rebuilding the whole thing
		// </summary>
		// <typeparam name="T">A physical object</typeparam>
		// <param name="Item">The physical object to insert into the tree</param>
		bool Insert(GameObject* Item);

	};
}