#include "MEpch.h"
#include "OBJParser.h"
#include "glm/glm.hpp"
namespace MYENGINE
{
	OBJParser::OBJParser(const std::string & fileName)
	{
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> textureCoord;
		std::vector<unsigned int> indices;

		std::ifstream stream("..\\res\\assets\\" + fileName);

		std::string line;
		std::streampos beforeFaces;
		unsigned int VerticesInPolygon = 0;

		if (stream.fail())
		{
			std::cout << "couldnt read OBJ file" << std::endl;
			//ASSERT(false);
		}

		while (getline(stream, line) && !stream.fail())
		{
			//erase is [begin, end);
			if (line.find("v ") != std::string::npos)
			{
				line.erase(line.begin(),line.begin()+2);
				InsertData(vertices,line);
			}
			else if (line.find("vt ") != std::string::npos)
			{
				line.erase(line.begin(), line.begin() + 3);
				InsertData(textureCoord, line);
				beforeFaces = stream.tellg();
			}
			else if (line.find("f ") != std::string::npos)
			{
				break;
			}
		}

		//std::unordered_set<vAndvt> addedVertices;
		std::vector<vAndvt> addedVertices;
		unsigned int origVerticesSize = vertices.size();
		std::vector<float> outputTextureCoord(origVerticesSize/3*2,-2);
		stream.seekg(beforeFaces);
		int facesNum = 0, quads=0, tri =0;
		while (getline(stream, line) && !stream.fail())
		{

			if (line.find("f ") != std::string::npos)
			{
				facesNum++;
				line.erase(line.begin(), line.begin() + 2);
				std::vector<std::string> curFaceValues = Split<std::string>(line, ' ');
				std::vector<unsigned int> curFace(curFaceValues.size());

				if (curFaceValues.size()==4)
				{
					quads++;
				}
				else if (curFaceValues.size() == 3)
				{
					tri++;
				}
				for (int i=0; i< curFaceValues.size(); i++)
				{
					std::vector<unsigned int> curVertexValues = Split<unsigned int>(curFaceValues[i], '/');
					unsigned int index = curVertexValues[0] - 1;
					float u = textureCoord[(curVertexValues[1] - 1) * 2 + 0];
					float v = textureCoord[(curVertexValues[1] - 1) * 2 + 1];
					float x = vertices[index * 3 + 0];
					float y = vertices[index * 3 + 1];
					float z = vertices[index * 3 + 2];
					vAndvt vertex(x, y, z, u, v);

					if (outputTextureCoord[index * 2 + 0] == -2)
					{
						outputTextureCoord[index * 2 + 0] = u;
						outputTextureCoord[index * 2 + 1] = v;
					}
					else if(outputTextureCoord[index * 2 + 0] != u ||
							outputTextureCoord[index * 2 + 1] != v)
					{
						//std::unordered_set<vAndvt>::iterator it = addedVertices.find(vertex);
						std::vector<vAndvt>::iterator it = std::find(addedVertices.begin(), addedVertices.end(), vertex);
						if (it == addedVertices.end())
						{
							outputTextureCoord.push_back(u);
							outputTextureCoord.push_back(v);

							vertices.push_back(x);
							vertices.push_back(y);
							vertices.push_back(z);
							//addedVertices.insert(vertex);
							addedVertices.push_back(vertex);
							index = vertices.size()/3-1;
						}
						else
						{
							unsigned int dist = std::distance(addedVertices.begin(), it);
							index = origVerticesSize/3+dist;
						}
					}//*/
					curFace[i] = index;
				}
				curFace = ToTriangles(curFace);
				indices.reserve(indices.size() + curFace.size()); // preallocate memory
				indices.insert(indices.end(), curFace.begin(), curFace.end()); //combined vectors
			}
		}
		normals = LoadNormals(vertices, indices);

		vertexIndices = indices;
		vertexData = GetData(vertices,outputTextureCoord,normals);
	}

	template<typename T>
	void OBJParser::InsertData(std::vector<T>& data, const std::string& line)
	{
		std::vector<T> newData = Split<T>(line, ' ');
		data.reserve(data.size() + newData.size()); // preallocate memory
		data.insert(data.end(), newData.begin(), newData.end()); //combined vectors
	}

	std::vector<float> OBJParser::LoadNormals(std::vector<float> vertices, std::vector<unsigned int> indices)
	{
		std::vector<float> faceNormals(vertices.size());

		for (int i = 0; i < indices.size(); i+=3)
		{
			glm::vec3 U(vertices[indices[i + 1] * 3 + 0] - vertices[indices[i] * 3 + 0],
						vertices[indices[i + 1] * 3 + 1] - vertices[indices[i] * 3 + 1],
						vertices[indices[i + 1] * 3 + 2] - vertices[indices[i] * 3 + 2]);

			glm::vec3 V(vertices[indices[i + 2] * 3 + 0] - vertices[indices[i] * 3 + 0],
						vertices[indices[i + 2] * 3 + 1] - vertices[indices[i] * 3 + 1],
						vertices[indices[i + 2] * 3 + 2] - vertices[indices[i] * 3 + 2]);

			glm::vec3 N = glm::cross(U, V);

			faceNormals[indices[i] * 3 + 0] += N.x;
			faceNormals[indices[i] * 3 + 1] += N.y;
			faceNormals[indices[i] * 3 + 2] += N.z;

			faceNormals[indices[i+1] * 3 + 0] += N.x;
			faceNormals[indices[i+1] * 3 + 1] += N.y;
			faceNormals[indices[i+1] * 3 + 2] += N.z;

			faceNormals[indices[i+2] * 3 + 0] += N.x;
			faceNormals[indices[i+2] * 3 + 1] += N.y;
			faceNormals[indices[i+2] * 3 + 2] += N.z;
		}

		for (int i = 0; i < faceNormals.size(); i+=3)
		{
			glm::vec3 v = {
				faceNormals[i],
				faceNormals[i+1],
				faceNormals[i+2]
			};
			if (abs(v.x + v.y + v.z) > 0.0f)
			{
				v = glm::normalize(v);
			}
			faceNormals[i + 0] = v.x;
			faceNormals[i + 1] = v.y;
			faceNormals[i + 2] = v.z;

			//faceNormals[i + 0] = 0.0;
			//faceNormals[i + 1] = 0.0;
			//faceNormals[i + 2] = 1.0;
		}
		return faceNormals;
	}
}