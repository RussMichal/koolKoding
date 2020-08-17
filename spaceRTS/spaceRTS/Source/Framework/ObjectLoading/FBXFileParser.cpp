#include "MEpch.h"
#include "FBXFileParser.h"

namespace MYENGINE
{
	FBXFileParser::FBXFileParser(const std::string& file)
	{
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> textureCoord;
		std::vector<float> indices;

		std::vector<float> retVertices;
		std::vector<float> retIndices;

		std::string pathToFBXFiles = "..\\res\\FBXModels\\spaceShip\\";
		std::ifstream stream(pathToFBXFiles + file);

		std::string line;
		std::streampos prevPos;
		unsigned int VerticesInPolygon = 0;

		if (stream.fail())
		{
			std::cout << "couldnt read FBX file" << std::endl;
			//ASSERT(false);
		}

		while (getline(stream, line) && !stream.fail())
		{
			if (line.find("Vertices: ") != std::string::npos)
			{
				vertices = LoadData(stream, line, DataType::Vertices);
			}

			if (line.find("PolygonVertexIndex: ") != std::string::npos)
			{
				RemoveAlphabeticChar(line);
				VerticesInPolygon = GetVerticesInPolygon(line);
				indices = LoadData(stream, line, DataType::Indices);
			}

			if (line.find("LayerElementNormal") != std::string::npos)
			{
				MappingInformationType normalMappingInfo;
				ReferenceInformationType normalReferenceInfo;
				GetMappingAndReferanceInfo(stream, normalMappingInfo, normalReferenceInfo);

				if (line.find("Normals:") != std::string::npos)
				{
					switch (normalReferenceInfo)
					{
					case ReferenceInformationType::Direct:
						switch (normalMappingInfo)
						{
						case MappingInformationType::ByPolygon:
							normals = LoadData(stream, line, DataType::Vertices);
							FacesToTriangles(normals, VerticesInPolygon);
							IndicesToTriangles(indices, VerticesInPolygon);
							DataToVertex(normals, indices, vertices.size(), DataType::Faces);
							//to triangles->compute vertexnormals
							break;
						case MappingInformationType::ByPolygonVertex:
							normals = LoadData(stream, line, DataType::Vertices);
							NormalsToTriangles(normals, VerticesInPolygon);
							IndicesToTriangles(indices, VerticesInPolygon);
							DataToVertex(normals, indices, vertices.size(), DataType::Vertices);
							//to triangles->avrage
							break;
						case MappingInformationType::ByVertex:
							normals = LoadData(stream, line, DataType::Vertices);
							break;
						case MappingInformationType::ByEdge:
							//avrage edges into vertices
							break;
						case MappingInformationType::AllSame:
							normals.resize(vertices.size());//set size
							RemoveAlphabeticChar(line);
							std::fill(normals.begin(), normals.end(), stof(line));//fill with value
							break;
						}
					}

				}
			}

			if (line.find("LayerElementUV:") != std::string::npos)
			{
				while (getline(stream, line) && !stream.fail())
				{
					if (line.find("UV: ") != std::string::npos)
					{
						//textureCoord = LoadData(stream);
					}
				}
			}

			prevPos = stream.tellg();
		}
	}
	std::vector<float> FBXFileParser::LoadData(std::ifstream& stream, std::string& line, const DataType& loadType)
	{
		std::vector<float> data;
		RemoveAlphabeticChar(line);

		do
		{
			//end of data
			if (line.find(":") != std::string::npos)
			{
				return data;
				break;
			}
			else if (!HasNumbers(line))
			{
				continue;
			}
			std::vector<float> newData = Split(line, ',', loadType);
			unsigned int newDataSize = newData.size();

			data.reserve(data.size() + newDataSize); // preallocate memory
			data.insert(data.end(), newData.begin(), newData.end()); //combined vectors
		} while (getline(stream, line) && !stream.fail());

		return data;
	}

	std::vector<float> FBXFileParser::Split(const std::string& s, char delimiter, const DataType& loadType)
	{
		std::vector<float> tokens;
		std::string token;
		//std::string splitString = s;
		//RemoveAlphabeticChar(splitString);
		std::istringstream tokenStream(s);
		while (std::getline(tokenStream, token, delimiter))
		{
			float curNum = stof(token);
			switch (loadType)
			{
			case DataType::Vertices:
				tokens.push_back(curNum);
				break;
			case DataType::Indices:
				if (curNum < 0)
				{
					curNum = abs(curNum) - 1;
				}
				tokens.push_back(curNum);
				break;
			}
		}
		return tokens;
	}

	void FBXFileParser::GetMappingAndReferanceInfo(std::ifstream& stream, MappingInformationType& mappingInfo, ReferenceInformationType& referenceInfo)
	{
		mappingInfo = MappingInformationType::Invalid;
		referenceInfo = ReferenceInformationType::Invalid;
		std::string line;
		while (getline(stream, line) && !stream.fail() && (mappingInfo == MappingInformationType::Invalid || referenceInfo == ReferenceInformationType::Invalid))
		{
			if (line.find("MappingInformationType:") != std::string::npos)
			{
				if (line.find("ByPolygonVertex") != std::string::npos)
				{
					mappingInfo = MappingInformationType::ByPolygonVertex;
				}
				else if (line.find("ByPolygon") != std::string::npos)
				{
					mappingInfo = MappingInformationType::ByPolygon;
				}
				else if (line.find("ByVertex") != std::string::npos ||
					line.find("ByVertice") != std::string::npos)
				{
					mappingInfo = MappingInformationType::ByVertex;
				}
				else if (line.find("ByEdge") != std::string::npos)
				{
					mappingInfo = MappingInformationType::ByEdge;
				}
				else if (line.find("AllSame") != std::string::npos)
				{
					mappingInfo = MappingInformationType::AllSame;
				}
			}

			if (line.find("ReferenceInformationType:") != std::string::npos)
			{
				if (line.find("Direct") != std::string::npos)
				{
					referenceInfo = ReferenceInformationType::Direct;
				}
				else if (line.find("IndexToDirect:") != std::string::npos ||
					line.find("Index") != std::string::npos)
				{
					referenceInfo = ReferenceInformationType::IndexToDirect;
				}
			}
		}
	}
	void FBXFileParser::NormalsToTriangles(std::vector<float>& data, const unsigned int verticesInPolygon)
	{
		if (verticesInPolygon == 3)
		{
			return;
		}

		std::vector<float> retData(data.size() / verticesInPolygon * ((verticesInPolygon - 2) * 3));
		for (unsigned int i = 0; i < data.size(); i += verticesInPolygon * 3)//loop through faces/polygons
		{
			for (unsigned int j = 0; j < (verticesInPolygon - 2); j++)//loop through triangles in face/polygon
			{
				retData[i + j * 9 + 0] = data[i + 0 + 0];
				retData[i + j * 9 + 1] = data[i + 0 + 1];
				retData[i + j * 9 + 2] = data[i + 0 + 2];

				retData[i + j * 9 + 3] = data[i + j * 3 + 3];
				retData[i + j * 9 + 4] = data[i + j * 3 + 4];
				retData[i + j * 9 + 5] = data[i + j * 3 + 5];

				retData[i + j * 9 + 6] = data[i + j * 3 + 6];
				retData[i + j * 9 + 7] = data[i + j * 3 + 7];
				retData[i + j * 9 + 8] = data[i + j * 3 + 8];
			}
		}

		data = retData;
	}

	void FBXFileParser::TexCoordToTriangles(std::vector<float>& data, const unsigned int verticesInPolygon)
	{
		if (verticesInPolygon == 3)
		{
			return;
		}

		std::vector<float> retData(data.size() / verticesInPolygon * ((verticesInPolygon - 2) * 3));
		for (unsigned int i = 0; i < data.size(); i += verticesInPolygon * 2)//loop through faces/polygons
		{
			for (unsigned int j = 0; j < (verticesInPolygon - 2); j++)//loop through triangles in face/polygon
			{
				retData[i + j * 6 + 0] = data[i + 0 + 0];
				retData[i + j * 6 + 1] = data[i + 0 + 1];

				retData[i + j * 6 + 3] = data[i + j * 2 + 2];
				retData[i + j * 6 + 4] = data[i + j * 2 + 3];

				retData[i + j * 6 + 6] = data[i + j * 2 + 4];
				retData[i + j * 6 + 7] = data[i + j * 2 + 5];
			}
		}

		data = retData;
	}

	void FBXFileParser::IndicesToTriangles(std::vector<float>& data, const unsigned int verticesInPolygon)
	{
		if (verticesInPolygon == 3)
		{
			return;
		}

		std::vector<float> retData(data.size() / verticesInPolygon * ((verticesInPolygon - 2) * 3));
		for (unsigned int i = 0; i < data.size(); i += verticesInPolygon)
		{
			for (unsigned int j = 0; j < (verticesInPolygon - 2); j++)
			{
				retData[i + j * 3 + 0] = data[i + 0 + 0];
				retData[i + j * 3 + 1] = data[i + j + 1];
				retData[i + j * 3 + 2] = data[i + j + 2];
			}
		}

		data = retData;
	}

	void FBXFileParser::FacesToTriangles(std::vector<float>& data, const unsigned int verticesInPolygon)
	{
		if (verticesInPolygon == 3)
		{
			return;
		}

		std::vector<float> retData(data.size()*(verticesInPolygon - 2));
		for (unsigned int i = 0; i < data.size(); i++)
		{
			for (unsigned int j = 0; j < verticesInPolygon - 2; j++)
			{
				retData.push_back(data[i]);
			}
		}

		data = retData;
	}

	void FBXFileParser::DataToVertex(std::vector<float>& data, const std::vector<float>& indicies, unsigned int verticesSize, const DataType& dataType)
	{
		int retDataSize = 0;
		if (dataType == DataType::texCoord)
		{
			retDataSize = verticesSize / 3 * 2;
		}
		else
		{
			retDataSize = verticesSize;
		}
		std::vector<float> retData(retDataSize);
		std::fill(retData.begin(), retData.end(), 0.0f);

		std::vector<float> counts(verticesSize / 3);
		std::fill(counts.begin(), counts.end(), 0.0f);

		for (unsigned int i = 0; i < indicies.size(); i++)
		{
			switch (dataType)
			{
			case DataType::Indices:
			case DataType::Vertices:
				retData[indicies[i] * 3 + 0] += data[i * 3 + 0];
				retData[indicies[i] * 3 + 1] += data[i * 3 + 1];
				retData[indicies[i] * 3 + 2] += data[i * 3 + 2];
				break;
			case DataType::Faces:
				retData[indicies[i] * 3 + 0] += data[i / 3 + 0];
				retData[indicies[i] * 3 + 1] += data[i / 3 + 1];
				retData[indicies[i] * 3 + 2] += data[i / 3 + 2];
				break;
			case DataType::texCoord:
				retData[indicies[i] * 2 + 0] += data[i * 2 + 0];
				retData[indicies[i] * 2 + 1] += data[i * 2 + 1];
				break;
			}

			counts[indicies[i]]++;
		}

		for (unsigned int i = 0; i < counts.size(); i++)
		{
			retData[i * 3 + 0] /= counts[i];
			retData[i * 3 + 1] /= counts[i];
			retData[i * 3 + 2] /= counts[i];
		}

		data = retData;
	}

	unsigned int FBXFileParser::GetVerticesInPolygon(const std::string& line)
	{
		unsigned int count = 1;
		std::string token;
		std::istringstream tokenStream(line);
		while (std::getline(tokenStream, token, ','))
		{
			if (stof(token) < 0)
			{
				break;
			}
			count++;
		}

		return count;
	}

}