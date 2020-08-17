#pragma once

#include <string>
#include <unordered_map>

#include "Vender\glm\glm.hpp"
#include "Vender\glm\gtc\matrix_transform.hpp"

namespace MYENGINE
{
	class Shaders
	{
	private:
		std::string m_filePath;
		unsigned m_renderID;

		struct ShaderStrings {
			std::string vertex;
			std::string fragment;
		};

		ShaderStrings ParseShaders(const std::string file);
		unsigned int CompileShader(int type, const char * shaderSource);
		int CreateShaders(ShaderStrings shaders);

		//cashing unifroms
		std::unordered_map<std::string, int> m_uniformLocationCache;
	public:
		Shaders(const std::string& filepath);
		~Shaders();

		void bind() const;
		void unbind() const;

		//set uniforms
		void SetUniform1i(const std::string name, int v0);
		void SetUniform1f(const std::string name, float v0);
		void SetUniform3f(const std::string name, float v0, float v1, float v2);
		void SetUniform4f(const std::string name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string name, glm::mat4 mat);
		int GetUniformLocation(const std::string& name);
	};
}