#include "MEpch.h"
#include "PrimitiveObject.h"

namespace MYENGINE
{
	PrimitiveObject::PrimitiveObject(const std::vector<float> vertices, const std::vector<unsigned int> indies, const std::string& texture, const glm::vec4& color)
		: m_mainObject(vertices, indies, texture, color)
		, m_position(0.0f, 0.0f, 0.0f)
		, m_scale(glm::vec3(1.0f, 1.0f, 1.0f))
		, m_orientation(glm::vec3(0.0f, 0.0f, 0.0f))
	{

	}
	PrimitiveObject::PrimitiveObject(const AssetProperties& assetData, const glm::vec4& color)
		: PrimitiveObject(assetData.vertexData, assetData.indices, assetData.texture, color)
	{

	}

	PrimitiveObject::~PrimitiveObject()
	{

	}
	glm::vec3 PrimitiveObject::GetPosition() const
	{
		return m_position;
	}
	glm::vec3 PrimitiveObject::GetScale() const
	{
		return m_scale;
	}

	glm::quat PrimitiveObject::GetOrientation() const
	{
		return m_orientation;
	}

	void PrimitiveObject::SetPosition(glm::vec3 pos)
	{
		m_position = pos;
	}

	void PrimitiveObject::SetScale(glm::vec3 scale)
	{
		m_scale = scale;
	}

	void PrimitiveObject::SetOrientation(glm::quat orin)
	{
		m_orientation = orin;
	}
}