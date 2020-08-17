#pragma once
#include <string>
#include <vector>
namespace MYENGINE
{

	class ParsingBase
	{
	public:
		std::vector<float> GetData(const std::vector<float>& vertices, const std::vector<float>& texCoord, const std::vector<float>& normals);

	protected:
		ParsingBase();

		template <typename T>
		std::vector<T> Split(const std::string& s, char delimiter)
		{
			std::vector<T> tokens;
			std::string token;
			//std::string splitString = s;
			//RemoveAlphabeticChar(splitString);
			std::istringstream tokenStream(s);
			while (std::getline(tokenStream, token, delimiter))
			{
				//float curNum = stof(token);
				T curNum = convert_to<T>(token);
				tokens.push_back(curNum);
			}
			return tokens;
		}

		void RemoveAlphabeticChar(std::string& strToTrim);
		bool HasNumbers(const std::string& strToCheck);
		std::vector<unsigned int> ToTriangles(std::vector<unsigned int> in_indices);


	private:
		template <typename T>
		T convert_to(const std::string &str)
		{
			std::istringstream ss(str);
			T num;
			ss >> num;
			return num;
		}
	};
}