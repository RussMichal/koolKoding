#include "MEpch.h"
#include "ParsingBase.h"

namespace MYENGINE
{
	ParsingBase::ParsingBase()
	{

	}

	//template <typename T>
	//std::vector<T> ParsingBase::Split(const std::string& s, char delimiter)
	//{
	//	std::vector<float> tokens;
	//	std::string token;
	//	//std::string splitString = s;
	//	//RemoveAlphabeticChar(splitString);
	//	std::istringstream tokenStream(s);
	//	while (std::getline(tokenStream, token, delimiter))
	//	{
	//		//float curNum = stof(token);
	//		T curNum = convert_to<T>(token);
	//		tokens.push_back(curNum);
	//	}
	//	return tokens;
	//}

	//template <typename T> 
	//T ParsingBase::convert_to(const std::string &str)
	//{
	//	std::istringstream ss(str);
	//	T num;
	//	ss >> num;
	//	return num;
	//}

	void ParsingBase::RemoveAlphabeticChar(std::string& strToTrim)
	{
		for (unsigned int i = 0; i < strToTrim.size(); ++i)
		{
			if (!((strToTrim[i] >= '0' && strToTrim[i] <= '9') ||
				strToTrim[i] == '.' ||
				strToTrim[i] == ',' ||
				strToTrim[i] == '-'))
			{
				strToTrim.erase(strToTrim.begin() + i);
				i--;
			}
		}
	}
	bool ParsingBase::HasNumbers(const std::string& strToCheck)
	{
		return strToCheck.find('0') != std::string::npos ||
			strToCheck.find('1') != std::string::npos ||
			strToCheck.find('2') != std::string::npos ||
			strToCheck.find('3') != std::string::npos ||
			strToCheck.find('4') != std::string::npos ||
			strToCheck.find('5') != std::string::npos ||
			strToCheck.find('6') != std::string::npos ||
			strToCheck.find('7') != std::string::npos ||
			strToCheck.find('8') != std::string::npos ||
			strToCheck.find('9') != std::string::npos;
	}

	std::vector<unsigned int> ParsingBase::ToTriangles(std::vector<unsigned int> in_indices)
	{
		std::vector<unsigned int> newIndices;
		for (int i = 0; i < 3; i++)
		{
			newIndices.push_back(in_indices[i]);
		}
		for (int i = 3; i < in_indices.size(); i++)
		{
			unsigned int last = newIndices.back();
			newIndices.push_back(in_indices[0]);
			newIndices.push_back(last);
			newIndices.push_back(in_indices[i]);
		}
		return newIndices;
	}

	std::vector<float> ParsingBase::GetData(const std::vector<float>& vertices, const std::vector<float>& texCoord, const std::vector<float>& normals)
	{
		unsigned int verticesSize = vertices.size();
		unsigned int texCoordSize = texCoord.size();
		std::vector<float> data(verticesSize * 2 + texCoordSize);
		unsigned int counter=0;

		for (unsigned int i = 0, j = 0; i < verticesSize; i+=3, j+=2)
		{
			data[counter++]= vertices[i + 0];
			data[counter++]= vertices[i + 1];
			data[counter++] = vertices[i + 2];
			if (texCoord[j + 0] == -2)
			{
				data[counter++] = 0.0f;
			}
			else
			{
				data[counter++] = texCoord[j + 0];
			}
			if (texCoord[j + 1] == -2)
			{
				data[counter++] = 0.0f;
			}
			else
			{
				data[counter++] = texCoord[j + 1];
			}
			//data[counter++] = texCoord[j + 0];
			//data[counter++] = texCoord[j + 1];

			data[counter++] = normals[i + 0];
			data[counter++] = normals[i + 1];
			data[counter++] = normals[i + 2];
		}

		int counterz = 0;
		for (int i = 0; i < texCoord.size(); i++)
		{
			if (texCoord[i] == -2)
			{
				counterz++;
			}
		}
		return data;
	}
}