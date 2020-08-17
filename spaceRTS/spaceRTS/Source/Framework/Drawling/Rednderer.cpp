#include "MEpch.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "Renderer.h"
#include "GLCall.h"

namespace MYENGINE
{
	Renderer::Renderer(const ShaderType shaderID, float  cc1, float cc2, float cc3, float cc4)
		: clearColor{ cc1, cc2, cc3, cc4 }
	{
        //TODO: anti aliesing setting set in init window also
        if (true)
        {
            GLCall(glEnable(GL_MULTISAMPLE));
        }
        else
        {
            GLCall(glDisable(GL_MULTISAMPLE));
        }

		switch (shaderID)
		{
		case ShaderType::Basic:
			m_shader = std::make_unique<Shaders>("..//res//shaders//Basic.shader");
			break;
		}
	}
	void Renderer::SetBlend()
	{
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));
        GLCall(glEnable(GL_DEPTH_TEST));
	}
	void Renderer::SetDrawBuffers(const unsigned int buffers) const
	{
		GLCall(glDrawBuffer(buffers));
	}

	void Renderer::Clear() const
	{
		GLCall(glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	Shaders Renderer::GetShader(ShaderType shaderID)
	{
		switch (shaderID)
		{
		case ShaderType::Basic:
			return *m_shader;
			break;
		}
		return *m_shader;
	}

	void Renderer::SetShader(ShaderType shaderID)
	{
		switch (shaderID)
		{
		case ShaderType::Basic:
			m_shader = std::make_unique<Shaders>("..//res//shaders//Basic.shader");//"..//res//shaders//Basic.shader"
			break;
		}
	}
	void Renderer::SetConstUniforms(glm::vec3 lightPos, float lightPower, float gamma)
	{
		m_shader->SetUniform3f("u_lightPos", lightPos[0], lightPos[1], lightPos[2]);
		m_shader->SetUniform1f("u_lightPower", lightPower);
		m_shader->SetUniform1f("u_screenGamma", gamma);
	}

	void Renderer::SetObjectUniforms(const Object& obj)
	{
		m_shader->SetUniform1i("u_texture", obj.GetTextureID());
		m_shader->SetUniform4f("u_color", obj.m_color[0], obj.m_color[1], obj.m_color[2], obj.m_color[3]);

		m_shader->SetUniform1f("shininess", obj.m_shininess);
	}

	void Renderer::SetObjectMVP(const PrimitiveObject& obj, const glm::mat4& projectionView, const ModelMode& modelUsage)
	{
		glm::mat4 ident(1.0f);
        glm::mat4 model;
        switch (modelUsage)
        {
        case ModelMode::None:
            model = ident;
            break;
        case ModelMode::All:
            model = glm::translate<float>(ident, obj.GetPosition());
            model *= glm::mat4_cast(obj.GetOrientation());
            model = glm::scale<float>(model, obj.GetScale());
            break;
        case ModelMode::NoScale:
            model = glm::translate<float>(ident, obj.GetPosition());
            model *= glm::mat4_cast(obj.GetOrientation());
            break;
        }


		m_shader->SetUniformMat4f("u_MVP", projectionView*model);
		m_shader->SetUniformMat4f("u_NM", GetNormalMatrix(model));//need view??
	}

	glm::mat4 Renderer::GetNormalMatrix(glm::mat4 mv)
	{
		return glm::transpose(glm::inverse(mv));
	}

	void Renderer::Draw(const GameObject& obj, const glm::mat4& projectionView, const PlayerNumber& mainPlayer)
	{
		// Draw our first triangle
		//glUseProgram(shaderProgram);
		obj.m_mainObject.Bind();
		m_shader->bind();

		SetObjectMVP(obj, projectionView);
		SetObjectUniforms(obj.m_mainObject);
		GLCall(glDrawElements(obj.m_mainObject.m_drawMode, obj.m_mainObject.GetElements(), GL_UNSIGNED_INT, nullptr));

		if (obj.m_isSelected[mainPlayer])
		{
			//obj.m_selectObjectCircle->Bind();
			//SetObjectUniforms(*obj.m_selectObjectCircle);
			//GLCall(glDrawElements(obj.m_selectObjectCircle->m_drawMode, obj.m_selectObjectCircle->GetElements(), GL_UNSIGNED_INT, nullptr));
            obj.m_bs.bs_Graphic->Bind();
            SetObjectUniforms(*obj.m_bs.bs_Graphic);
            GLCall(glDrawElements(obj.m_bs.bs_Graphic->m_drawMode, obj.m_bs.bs_Graphic->GetElements(), GL_UNSIGNED_INT, nullptr));

            obj.m_selectObjectBoxOrig->Bind();
            SetObjectUniforms(*obj.m_selectObjectBoxOrig);
            GLCall(glDrawElements(obj.m_selectObjectBoxOrig->m_drawMode, obj.m_selectObjectBoxOrig->GetElements(), GL_UNSIGNED_INT, nullptr));



			SetObjectMVP(obj, projectionView, ModelMode::None);//no mvp for bb bc it is rotated to stay axis aligned when object is rotated
			obj.m_selectObjectBox->Bind();
			SetObjectUniforms(*obj.m_selectObjectBox);
			GLCall(glDrawElements(obj.m_selectObjectBox->m_drawMode, obj.m_selectObjectBox->GetElements(), GL_UNSIGNED_INT, nullptr));
		}

        for (WeaponBase* weap : obj.GetWeapons())
        {
            if (obj.m_isSelected)
            {
                SetObjectMVP(obj, projectionView, ModelMode::NoScale);
                weap->rangeSphere.bs_Graphic->Bind();
                SetObjectUniforms(*weap->rangeSphere.bs_Graphic);
                GLCall(glDrawElements(weap->rangeSphere.bs_Graphic->m_drawMode, weap->rangeSphere.bs_Graphic->GetElements(), GL_UNSIGNED_INT, nullptr));
            }
            if (weap->drawGraphic)
            {
                SetObjectMVP(obj, projectionView, ModelMode::None);
                weap->graphic->Bind();
                SetObjectUniforms(*weap->graphic);
                GLCall(glDrawElements(weap->graphic->m_drawMode, weap->graphic->GetElements(), GL_UNSIGNED_INT, nullptr));
            }
        }
		//performace waste??
		//va.Unbind();
		//ib.Unbind();
		//sh.unbind();
	}

	void Renderer::Draw(const PrimitiveObject& obj, const glm::mat4& projectionView, const PlayerNumber& mainPlayer)
	{
		obj.m_mainObject.Bind();
		m_shader->bind();

		SetObjectMVP(obj, projectionView);
		SetObjectUniforms(obj.m_mainObject);
		GLCall(glDrawElements(obj.m_mainObject.m_drawMode, obj.m_mainObject.GetElements(), GL_UNSIGNED_INT, nullptr));
	}
}