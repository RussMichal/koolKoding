#pragma once

#include "Vender\glm\glm.hpp"
#include "Vender\glm\gtc\matrix_transform.hpp"
#include "Core.h"

namespace MYENGINE
{
	template struct MYENGINE_API glm::mat<4, 4, float, glm::qualifier::defaultp>;

	
	class MYENGINE_API Camera
	{
	public:
		Camera();
		~Camera();

		void Identity();
		void Translate(const glm::vec3& v);
		void Rotate(const float& angle, const glm::vec3& v);
		void Scale(const glm::vec3& v);
		void LookAt(const glm::vec3& pos, const glm::vec3& laPoint, const glm::vec3& up);

		glm::mat4 GetView();
		glm::mat4 GetNonInverseView();

		glm::vec3 GetPos();
	private:
		glm::mat4 m_view;

		//might not need cam pos. here for now. could remove later
		glm::vec3 m_pos;
	};
}