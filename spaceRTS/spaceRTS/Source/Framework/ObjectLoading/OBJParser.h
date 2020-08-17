#pragma once
#include <string>
#include <vector>
#include "ParsingBase.h"

namespace MYENGINE
{
	class OBJParser: public ParsingBase
	{
	public:
		OBJParser(const std::string& fileName);
		struct vAndvt
		{
			float x, y, z;
			float u, v;

			bool operator==(const vAndvt &lhs) const
			{
				return (x == lhs.x
					&& y == lhs.y
					&& z == lhs.z
					&& u == lhs.u
					&& v == lhs.v);
			}

			vAndvt(const float& ix,
				const float& iy,
				const float& iz,
				const float& iu,
				const float& iv)
				: x(ix)
				, y(iy)
				, z(iz)
				, u(iu)
				, v(iv) {}
		};

		std::vector<unsigned int> vertexIndices;
		std::vector<float> vertexData;
	private:
		template<typename T>
		void InsertData(std::vector<T>& data, const std::string& line);
		std::vector<float> LoadNormals(std::vector<float> vertices, std::vector<unsigned int> indices);



	};

}

//template<class T> struct hash;
template<> struct std::hash<MYENGINE::OBJParser::vAndvt>
{
	std::size_t operator()(const MYENGINE::OBJParser::vAndvt& v) const
	{

		// and bit shifting:
		std::size_t seed = 5;//5elements in struct
		seed ^= std::hash<float>()(v.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= std::hash<float>()(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= std::hash<float>()(v.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= std::hash<float>()(v.u) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= std::hash<float>()(v.v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

		return seed;
	}
};