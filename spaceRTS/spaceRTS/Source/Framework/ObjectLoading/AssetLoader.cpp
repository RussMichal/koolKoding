#include "MEpch.h"
#include "AssetLoader.h"
#include "OBJParser.h"
#include <filesystem>

namespace MYENGINE
{
	AssetLoader::AssetLoader()
	{
		//LoadAssetCSV();

		std::string pathToAssets = "..\\res\\assets\\";
		for (const auto & AssetTypeFolder : std::experimental::filesystem::directory_iterator(pathToAssets))
		{
			if (std::experimental::filesystem::is_directory(AssetTypeFolder))
			{
				for (const auto & AssetFolder : std::experimental::filesystem::directory_iterator(AssetTypeFolder))
				{
					AssetProperties assetProp;
					std::ifstream stream(AssetFolder.path().string() + "\\AssetProperties.xml");
					std::string line;

					if (stream.fail())
					{
						std::cout << "couldnt read Asset file" << std::endl;
						//ASSERT(false);
						continue;
					}

					std::unordered_map<std::string, std::string> AssetElements;
					std::list<std::string> groupNodes;

					while (getline(stream, line) && !stream.fail())
					{
						std::size_t found1 = line.find("=");
						if (found1 != std::string::npos)
						{
							std::vector<std::string> splitLine = Split<std::string>(line, '=');
							AssetElements[splitLine[0]] = splitLine[1];
						}

						found1 = line.find("<");
						std::size_t found2 = line.find(">");
						std::size_t found3 = line.find("</");

						//start group
						if (found1 != std::string::npos &&
							found2 != std::string::npos &&
							found3 == std::string::npos)
						{
							line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
							groupNodes.push_back(line.substr(1, line.size() - 2));
						}

						//end group
						else if (found1 != std::string::npos &&
							found2 != std::string::npos &&
							found3 != std::string::npos)
						{
							LoadAssetPropertes(groupNodes, AssetElements, assetProp);
							groupNodes.pop_back();
							AssetElements.clear();
						}
					}
					Assets[strToAsset.at(assetProp.name)] = assetProp; 
				}
			}
		}
	}

	void AssetLoader::LoadAssetPropertes(const std::list<std::string>& group, const std::unordered_map<std::string, std::string>& AssetElements, AssetProperties& assetProp)
	{
		//std::list<std::string>::const_reverse_iterator  it = group.rbegin();

		if(group.front() == "General")
		{
			assetProp.name = AssetElements.at("Name");
			assetProp.type = strToObjType.at(AssetElements.at("AssetType"));
		}
		else if (group.front() == "Art")
		{
			if (AssetElements.at("Model").find(".obj"))
			{
				OBJParser OBJModel(objTypeToStr.at(assetProp.type) + "s//" + assetProp.name + "//" + AssetElements.at("Model"));
				assetProp.vertexData = OBJModel.vertexData;
				assetProp.indices = OBJModel.vertexIndices;
			}
			assetProp.texture = objTypeToStr.at(assetProp.type) + "s//" + assetProp.name + "//" + AssetElements.at("Texture");
		}
		else if (group.front() == "Stats")
		{
            assetProp.moveSpeed = stof(AssetElements.at("MovementSpeed"));
            assetProp.health = stoi(AssetElements.at("Health"));
        }
		else if (group.front() == "Weapons" && AssetElements.size()>0)
		{
            if (group.back() == "Laser")
            {
                
                WeaponInfo wi;
                wi.weaponType  = strToWeaponType.at("Laser");
                wi.range       = stoi(AssetElements.at("Range"));
                wi.damage      = stoi(AssetElements.at("Damage"));
                wi.attackSpeed = stof(AssetElements.at("AttackSpeed"));
                assetProp.weapons.push_back(wi);
            }
		}
	}

	//int AssetLoader::hashString(const std::string& s)
	//{
	//	int i = 0;
	//	for (char c : s)
	//	{
	//		i += c;
	//	}
	//	return i;
	//}
	void AssetLoader::LoadAssetCSV()
	{
		std::string pathToAssetFile = "..\\res\\assets\\AssetFile.csv";
		std::ifstream stream(pathToAssetFile);

		std::string line;
		if (stream.fail())
		{
			std::cout << "couldnt read Asset file" << std::endl;
			//ASSERT(false);
		}

		//skip first line. it is the names of the fields
		getline(stream, line);

		while (getline(stream, line) && !stream.fail())
		{
			std::vector<std::string> AssetElements = Split<std::string>(line, ',');
			AssetProperties assetProp;

			if (AssetElements[2].find(".obj"))
			{
				OBJParser OBJModel(AssetElements[1] + "s//" + AssetElements[0] + "//" + AssetElements[2]);
				assetProp.vertexData = OBJModel.vertexData;
				assetProp.indices = OBJModel.vertexIndices;
			}
			assetProp.texture = AssetElements[1] + "s//" + AssetElements[0] + "//" + AssetElements[3];
			assetProp.moveSpeed = stof(AssetElements[11]);

			std::string s = AssetElements[1];
			ObjectType ot = strToObjType.at(s);
			assetProp.type = ot;

			s = AssetElements[0];
			Asset a = strToAsset.at(s);
			Assets[a] = assetProp;
		}
	}
}