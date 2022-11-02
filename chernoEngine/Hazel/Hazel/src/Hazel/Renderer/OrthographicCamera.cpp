#include "hzpch.h"
#include "OrthographicCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Hazel
{

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) //这四个,是视场大小,暂且只提供2D平面面积,但我们的投影矩阵包括z向的剪切
		:m_ProjectionMartix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_pos(glm::vec3(0.0f,0.0f,0.0f)),m_rotation(0.0f)
	{
		HZ_PROFILE_FUNCTION();

		m_ViewMartix = glm::mat4(1.0f);
		m_ViewProjectionMartix = m_ProjectionMartix*m_ViewMartix; //
	}

	OrthographicCamera::~OrthographicCamera()
	{

	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		HZ_PROFILE_FUNCTION();	
		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_pos);  //不懂自己看函数原型去!
		//transform =  glm::rotate(transform, m_rotation, { 0.0f,0.0f,1.0f });
		//等价 ...先旋转还是先平移这里无区别,transform 是变换 ,切不可当作相机位置的一种描述
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_pos)* glm::rotate(glm::mat4(1.0f),glm::radians(m_rotation), { 0.0f,0.0f,1.0f }); 
		//m_rot是角度,这里转为弧度

		m_ViewMartix = glm::inverse(transform);//做逆变换
		m_ViewProjectionMartix = m_ProjectionMartix * m_ViewMartix;


	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		HZ_PROFILE_FUNCTION();

		m_ProjectionMartix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f); 
		m_ViewProjectionMartix = m_ProjectionMartix * m_ViewMartix;
	}

}