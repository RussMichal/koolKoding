#pragma once

#include <string>
#include <vector>
#include "ParsingBase.h"
namespace MYENGINE
{
	class FBXFileParser : public ParsingBase
	{
	public:
		FBXFileParser(const std::string& file);
		//~FBXFileParser();
		enum class DataType
		{
			Vertices,
			Indices,
			Faces,
			texCoord
		};
		enum class MappingInformationType
		{
			Invalid = -1,
			ByPolygon,
			ByPolygonVertex,
			ByVertex,
			ByEdge,
			AllSame
		};
		enum class ReferenceInformationType
		{
			Invalid = -1,
			Direct,
			IndexToDirect
		};

		std::vector<float> LoadData(std::ifstream& stream, std::string& line, const DataType& loadType);
		std::vector<float> Split(const std::string& s, char delimiter, const DataType& loadType);

		void IndicesToTriangles(std::vector<float>& data, const unsigned int verticesInPolygon);
		void NormalsToTriangles(std::vector<float>& data, const unsigned int verticesInPolygon);
		void TexCoordToTriangles(std::vector<float>& data, const unsigned int verticesInPolygon);
		void FacesToTriangles(std::vector<float>& data, const unsigned int verticesInPolygon);

		void DataToVertex(std::vector<float>& data, const std::vector<float>& indicies, unsigned int verticesSize, const DataType& dataType);

		void GetMappingAndReferanceInfo(std::ifstream& stream, MappingInformationType& mappingInfo, ReferenceInformationType& referenceInfo);
		unsigned int GetVerticesInPolygon(const std::string& line);
	};
}