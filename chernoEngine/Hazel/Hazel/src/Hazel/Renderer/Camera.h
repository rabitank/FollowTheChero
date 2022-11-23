#pragma once
#include "glm/glm.hpp"

namespace Hazel
{

	class Camera
	{
	public:
		Camera() =default	;
		Camera(const glm::mat4& projectionMat) 
			:m_projection(projectionMat)
		{ };
		virtual ~Camera() {};

		const glm::mat4& GetProjection() const { return m_projection; }

	protected:

		glm::mat4 m_projection = glm::mat4(1.f);

	};





	class  PerspectiveCamera
	{

	public:
		PerspectiveCamera(float left, float right, float bottom, float top, float zfar, float znear);
		void SetProjection(float fovy, float aspect, float znear, float zfar);
		void SetProjection(float left, float right, float bottom, float top, float znear, float zfar);
		~PerspectiveCamera();

		inline void SetPosition(const glm::vec3& pos) { m_pos = pos; RecalculateViewMatrix(); }
		inline void SetRotation(const float& rot) { m_rotation = rot; RecalculateViewMatrix(); };

		inline const glm::vec3& GetPosition() const { return m_pos; };
		inline const float& GetRotation() const { return m_rotation; };

		inline const glm::mat4& GetProjectionMat() const { return m_ProjectionMartix; };
		inline const glm::mat4& GetViewMat() const { return m_ViewMartix; };
		inline const glm::mat4& GetViewProjectionMat() const { return m_ViewProjectionMartix; };



	private:
		void RecalculateViewMatrix(); //for updata ViewM
	private:

		glm::mat4 m_ProjectionMartix;
		glm::mat4 m_ViewMartix;
		glm::mat4 m_ViewProjectionMartix; // you may use it consquently 

		glm::vec3 m_pos;
		glm::vec3 m_direction = glm::vec3(0.f,0.f,-1.f);
		glm::vec3 m_up = glm::vec3(0.f,1.f,0.f);
		float m_rotation = 0.0f;//there is a 2D camera , so  rotation about direction
	};


}


