#pragma once
#include <vector>
#include <unordered_map>
#include"Framework/ObjectLoading/ParsingBase.h"
#include "vender/glm/glm.hpp"
#include "Framework/Drawling/Laser.h"
#include "AssetEnums.h"

namespace MYENGINE
{
    struct WeaponInfo
    {
        WeaponType weaponType;
        unsigned int range;
        unsigned int damage;
        unsigned int attackSpeed;
    };
	struct MYENGINE_API AssetProperties
	{
		std::string name;
		ObjectType type;

		std::vector<float> vertexData;
		std::vector<unsigned int> indices;
		std::string texture = "";

		float moveSpeed = 0.0;
        unsigned int health;

		std::vector<WeaponInfo> weapons;
	};
	class AssetLoader:public ParsingBase
	{
	public:
		AssetLoader();

		void LoadAssetCSV();

		std::unordered_map<Asset, AssetProperties> Assets;
	private:
		void LoadAssetPropertes(const std::list<std::string>& group, const std::unordered_map<std::string, std::string>& AssetElements, AssetProperties& assetProp);

		const std::unordered_map<std::string, Asset> strToAsset = {
			{"Triangle",Asset::Triangle},
			{"Square",Asset::Square},
			{"OGCommander",Asset::OGCommander}
		};
		const std::unordered_map<std::string, WeaponType> strToWeaponType = {
			{"Laser",WeaponType::Laser},
			{"Bullet",WeaponType::Bullet},
			{"AOEProjectile",WeaponType::AOEProjectile}
		};
		
		const std::unordered_map<std::string, ObjectType> strToObjType = {
			{"Unit",ObjectType::Unit},
			{"Building",ObjectType::Building},
			{"Commander",ObjectType::Commander}
		};

		const std::unordered_map<ObjectType, std::string> objTypeToStr = {
			{ObjectType::Unit, "Unit"},
			{ObjectType::Building, "Building"},
			{ObjectType::Commander, "Commander"}
		};
		
	};

}