#pragma once
#include "glm/glm.hpp"
#include "Hazel/Scene/SceneCamera.h"

#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel
{
	class EditorCamera:public Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0, Orthographic
		};

	public:
		EditorCamera() ;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);


		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);


		inline float GetDistance() { return m_distance; }

		inline void  SetDistance(float distance) { m_distance =distance; }
		//CHANGED
		inline void SetViewPortSize(float width, float height) {
			m_viewportWidth = width; m_viewportHeight = height; UpdateProjection();
		}

		const glm::mat4& GetViewMatrix() const { return m_viewMartix; }
		glm::mat4 GetViewProjectionMatrix() const { return m_projection * m_viewMartix; }

		//local xyz
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetForwardDirection()const;

		const  glm::vec3& GetPosition() const { return m_position; }
		
		glm::quat GetOrientation()const; //for local rotation interface
		float GetPitch() const { return m_pitch; }
		float GetYaw() const { return m_yaw; }
		
		bool& IfLockRotation() { return m_ifLockRotation; };
	private:
		void UpdateProjection();
		void UpdateView() ;

		bool OnMouseScroll(MouseScorllEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotation(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float>PanSpeed()const;
		float RotationSpeed()const;
		float ZoomSpeed()const;

		void RecalculateProjection();


	public:

		//@return: fov in radians
		inline float GetFOV() { return m_fov; }
		inline float GetNearClip() { return m_nearClip; }
		inline float GetFarClip() { return m_farClip; }


		//it shouldn't be used by user directly
		void SetProjectionType(ProjectionType type) { m_projectionType = type; }


		//@in put: fov in radiance
		void SetProjection(float fov, float NearClip, float FarClip) { m_fov = fov; m_farClip = FarClip; m_nearClip = NearClip; };
		
		inline void SetFOV(float FOV) { m_fov = FOV; RecalculateProjection(); }
		inline void SetFarClip(float zfar) { m_farClip = zfar; RecalculateProjection(); }
		inline void SetNearClip(float znear) { m_nearClip = znear; RecalculateProjection(); }


		


	private:
		/////////////////////
		ProjectionType m_projectionType = ProjectionType::Orthographic;




		/////////////
		float m_aspect = 1.778f;


		float m_fov = 90.f;
		float m_farClip= 1000.f;
		float m_nearClip = 0.01f;


		glm::mat4 m_viewMartix;
		glm::vec3 m_position = {0.f,0.f,5.f};
		glm::vec3 m_focalPoint = {0.f,0.f,0.f};
		float m_yaw=0.f; //roll Yaw Pitch 
		float m_pitch=0.f;
	

		float m_viewportWidth = 1280.f;
		float m_viewportHeight = 720.f;
		
		float m_distance=5.f;//change camera position 0.0

		glm::vec2 m_initialMousePosition = {0.f,0.f}; //for drag

		bool m_ifLockRotation = false;


	};

}

