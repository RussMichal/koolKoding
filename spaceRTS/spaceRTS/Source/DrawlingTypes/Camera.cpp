#include "MEpch.h"
#include "Camera.h"

namespace MYENGINE
{
	Camera::Camera()
		: m_view(glm::mat4(1.0f))
		, m_pos(glm::vec3(0.0f,0.0f, 200.0f))
	{
	}

	Camera::~Camera()
	{

	}
	void Camera::Identity()
	{
		m_view = glm::mat4(1.0f);
		m_pos = glm::vec3(0.0f, 0.0f, 200.0f);
	}
	void Camera::Translate(const glm::vec3& v)
	{
		m_view = glm::translate<float>(m_view, v);
		m_pos += v;
	}

	void Camera::Rotate(const float& angle, const glm::vec3& v)
	{
		m_view = glm::rotate<float>(m_view, glm::radians(angle), v);
		glm::vec4 r = glm::rotate<float>(glm::mat4(1.0f), glm::radians(angle), v)*glm::vec4(m_pos.x, m_pos.y, m_pos.z, 1.0f);
		m_pos = glm::vec3(r.x, r.y, r.z);
	}

	void Camera::Scale(const glm::vec3& v)
	{
		m_view = glm::scale<float>(m_view, v);
	}

	//this function will over ride the current view
	void Camera::LookAt(const glm::vec3& pos, const glm::vec3& laPoint, const glm::vec3& up)
	{
		m_view = glm::lookAt<float>(pos, laPoint, up);
	}

	glm::mat4 Camera::GetView()
	{
		return glm::inverse(m_view);
	}

	glm::mat4 Camera::GetNonInverseView()
	{
		return m_view;
	}

	glm::vec3 Camera::GetPos()
	{
		return m_pos;
	}

}