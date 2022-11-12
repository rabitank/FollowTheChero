#include "hzpch.h"
#include "Camera.h"


#include "glm/gtc/matrix_transform.hpp"


namespace Hazel
{

	PerspectiveCamera::PerspectiveCamera(float left, float right, float bottom, float top, float znear,float zfar) //这四个,是视场大小,暂且只提供2D平面面积,但我们的投影矩阵包括z向的剪切
		: m_ProjectionMartix(glm::perspective((top-bottom)/zfar,(right-left)/(top-bottom),znear,zfar)),m_pos(glm::vec3(0.0f, 0.0f, 0.0f)), m_rotation(0.0f)
	{
		HZ_PROFILE_FUNCTION();

		m_ViewMartix = glm::lookAt(m_direction,m_pos,m_up);
		m_ViewProjectionMartix = m_ProjectionMartix * m_ViewMartix; //
	}

	PerspectiveCamera::~PerspectiveCamera()
	{

	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		HZ_PROFILE_FUNCTION();

		auto up = glm::rotate(glm::mat4(1.f), glm::radians(m_rotation), m_direction) * glm::vec4(m_up, 1.f);
		m_up = {up.x,up.y,up.z};
		m_ViewMartix = glm::lookAt(m_direction, m_pos, m_up);//做逆变换
		m_ViewProjectionMartix = m_ProjectionMartix * m_ViewMartix;


	}

	void PerspectiveCamera::SetProjection(float fovy, float aspect, float znear,float zfar)
	{
		HZ_PROFILE_FUNCTION();

		m_ProjectionMartix = glm::perspective(fovy,aspect,znear,zfar);
		m_ViewProjectionMartix = m_ProjectionMartix * m_ViewMartix;
	}

	void PerspectiveCamera::SetProjection(float left, float right, float bottom, float top, float znear, float zfar)
	{
		m_ProjectionMartix=glm::perspective((top - bottom) / zfar, (right - left) / (top - bottom), znear, zfar);
		m_ViewProjectionMartix = m_ProjectionMartix * m_ViewMartix;
	}

}