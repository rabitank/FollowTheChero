#include "hzpch.h"
#include "SceneCamera.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Hazel
{

	SceneCamera::SceneCamera()
	{
		m_projectionType = ProjectionType::Orthographic;

		RecalculateProjection();
			
	}

	void SceneCamera::SetOrthographic(float size, float NearClip, float FarClip)
	{
		m_OrthographicSize = size; m_orthoZFar = FarClip; m_orthoZNear = NearClip;
		RecalculateProjection();

	}

	void SceneCamera::SetPerspective(float fov, float NearClip, float FarClip)
	{
		m_perspectiveFov = fov; m_perspectiveZNear= NearClip; m_perspectiveZFar= FarClip;
		RecalculateProjection();

	}

	void SceneCamera::SetViewPortSize(uint32_t width, uint32_t height)
	{
		
		m_aspect = (float)width / (float)height;
		
		RecalculateProjection();

	}

	void SceneCamera::SetProjectionType(ProjectionType type)
	{
		m_projectionType = type;
		RecalculateProjection();

	}

	void SceneCamera::RecalculateProjection()
	{

		switch (m_projectionType)
		{
		case Hazel::SceneCamera::ProjectionType::Perspective:
			
			m_projection = glm::perspective(m_perspectiveFov,m_aspect,m_perspectiveZNear,m_perspectiveZFar); //hen ... like a prespectiveCamera

			break;
		case Hazel::SceneCamera::ProjectionType::Orthographic:
			float orthoLeft = -0.5f * m_aspect * m_OrthographicSize;
			float orthoRight = 0.5f * m_aspect * m_OrthographicSize;
			float orthoBottom = -0.5f * m_OrthographicSize;
			float orthoUp = 0.5f * m_OrthographicSize;
			m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoUp, m_orthoZNear, m_orthoZFar); //hen ... like a prespectiveCamera
			break;

		}



	}

}