#pragma once

#include "Hazel/Renderer/Camera.h"

namespace Hazel
{
	class SceneCamera:public Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0, Orthographic
		};

	public:
		SceneCamera();
		virtual ~SceneCamera() =default;


		inline ProjectionType GetProjectionType() const { return m_projectionType;}

		inline float GetOrthographicSize() { return m_OrthographicSize; }
		inline float GetOthrographicNearClip() { return m_orthoZNear; }
		inline float GetOthrographicFarClip() { return m_orthoZFar; }

		//@return: fov in radians
		inline float GetPerspectFOV() { return m_perspectiveFov;}
		inline float GetPerspectiveNearClip() { return m_perspectiveZNear; }
		inline float GetPerspectiveFarClip() { return m_perspectiveZFar; }

		
		//it shouldn't be used by user directly
		void SetProjectionType(ProjectionType type);

		void SetViewPortSize(uint32_t width,uint32_t height) ;

		void SetOrthographic(float size, float nearClip, float FarClip) ;
		inline void SetOrthographicSize(float orthoCameraSize) { m_OrthographicSize = orthoCameraSize; RecalculateProjection(); }
		inline void SetOthroFarClip(float zfar) {m_orthoZFar = zfar; RecalculateProjection();}
		inline void SetOrthoNearClip(float znear) {m_orthoZNear = znear; RecalculateProjection();}

		//@in put: fov in radiance
		void SetPerspective(float fov, float NearClip, float FarClip) ;
		inline void SetPerspectFOV(float FOV) { m_perspectiveFov= FOV ; RecalculateProjection(); }
		inline void SetPerspectFarClip(float zfar) {m_perspectiveZFar = zfar; RecalculateProjection();}
		inline void SetPerspectNearClip(float znear) {m_perspectiveZNear = znear; RecalculateProjection();}



	private:

		void RecalculateProjection();
	private:
		ProjectionType m_projectionType = ProjectionType::Orthographic;

		float m_aspect = 1.f;

		float m_OrthographicSize= 10.f;
		float m_orthoZFar= 20.f;
		float m_orthoZNear= -1.f;

		float m_perspectiveFov = glm::radians(90.f);
		float m_perspectiveZFar= 1000.f;
		float m_perspectiveZNear= 0.001f;

		
	};

}


