#include "MEpch.h"

#include "Drawling\Renderer.h"
#include "Shaders.h"

namespace MYENGINE
{
	Shaders::Shaders(const std::string & filepath) :
		m_filePath(filepath),
		m_renderID(0)
	{
		ShaderStrings shaderProgramSource = ParseShaders(filepath);
		m_renderID = CreateShaders(shaderProgramSource);
	}

	Shaders::~Shaders()
	{
		GLCall(glDeleteProgram(m_renderID));
	}

	void Shaders::bind() const
	{
		GLCall(glUseProgram(m_renderID));
	}

	void Shaders::unbind() const
	{
		GLCall(glUseProgram(0));
	}

	void Shaders::SetUniform1i(const std::string name, int v0)
	{
		GLCall(glUniform1i(GetUniformLocation(name), v0));
	}

	void Shaders::SetUniform1f(const std::string name, float v0)
	{
		GLCall(glUniform1f(GetUniformLocation(name), v0));
	}

	void Shaders::SetUniform3f(const std::string name, float v0, float v1, float v2)
	{
		GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
	}

	void Shaders::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3)
	{
		GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
	}

	void Shaders::SetUniformMat4f(const std::string name, glm::mat4 mat)
	{
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
	}

	int Shaders::GetUniformLocation(const std::string & name)
	{
		if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
		{
			return m_uniformLocationCache[name];
		}

		GLCall(int location = glGetUniformLocation(m_renderID, name.c_str()));

		if (location == -1)
		{
			std::cout << "Warning unifrom " << name << "(-1) doesn't exist" << std::endl;
			ASSERT((location != -1));//if unused uniform will be -1. is ok in this case.
		}

		return location;
	}



	Shaders::ShaderStrings Shaders::ParseShaders(const std::string file)
	{
		std::ifstream stream(file);

		enum class ShaderType
		{
			NONE = -1,
			Vertex = 0,
			FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;

		if (stream.fail())
		{
			std::cout << "couldnt read shader file" << std::endl;
			ASSERT(false);
		}

		while (getline(stream, line) && !stream.fail())
		{
			if (line.find("//") != std::string::npos)
			{
				continue;
			}

			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = ShaderType::Vertex;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;
				}
			}
			else
			{
				ss[(int)type] << line << '\n';
			}
		}

		return { ss[0].str(), ss[1].str() };
	}

	unsigned int Shaders::CompileShader(int type, const char * shaderSource)
	{
		// Build and compile our shader program
		// Vertex shader
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);

		// Check for compile time errors
		int success;
		char infoLog[512];

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(shader, length, &length, message);
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << ((type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED fixed size\n" << infoLog << std::endl;
			std::cout << "ERROR::SHADER::" << ((type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED flexible size\n" << message << std::endl;
		}

		return shader;
	}

	int Shaders::CreateShaders(ShaderStrings shaders)
	{
		const char* vertexShaderSource = shaders.vertex.c_str();
		const char* fragmentShaderSource = shaders.fragment.c_str();


		// Build and compile our shader program
		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		// Link shaders
		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// Check for linking errors
		int success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

		if (!success)
		{
			int length;
			glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(shaderProgram, length, &length, message);
			glGetProgramInfoLog(shaderProgram, length, NULL, message);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << message << std::endl;
		}

		glDeleteProgram(vertexShader);
		glDeleteProgram(fragmentShader);

		glUseProgram(shaderProgram);

		return shaderProgram;
	}
}