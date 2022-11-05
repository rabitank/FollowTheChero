#pragma once

#include "glm/glm.hpp"


namespace Hazel
{


	class OrthographicCamera //正交相机
	{
	public:
		OrthographicCamera( float left,float right,float bottom,float top);
		void SetProjection(float left, float right, float bottom, float top) ;
		~OrthographicCamera();

		inline void SetPosition(const glm::vec3& pos) { m_pos = pos; RecalculateViewMatrix(); }
		inline void SetRotation(const float& rot) {m_rotation = rot; RecalculateViewMatrix();	};

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
		float m_rotation = 0.0f;//there is a 2D camera , so  rotation about z 
	};

}


