#include "hzpch.h"
#include "EditorCamera.h"

#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseButtonCodes.h"
#include "Hazel/Core/Input.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>



namespace Hazel
{


	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		:m_fov(fov), m_aspect(aspectRatio), m_nearClip(nearClip), m_farClip(farClip),Camera(glm::perspective(glm::radians(m_fov),m_aspect,m_nearClip,m_farClip))
	{
		UpdateView();

	}


	//CHANGED
	EditorCamera::EditorCamera()
	{
		UpdateView();
		UpdateProjection();
	}

	void EditorCamera::UpdateProjection()
	{
		m_aspect = m_viewportWidth / m_viewportHeight;
		m_projection = glm::perspective(glm::radians(m_fov), m_aspect, m_nearClip, m_farClip);
	}

	void EditorCamera::UpdateView()
	{
		//m_ya
		
		m_yaw = m_ifLockRotation ?  0.f: m_yaw, m_pitch = m_ifLockRotation ? 0.f: m_pitch;//lock camera rotation

		m_position = CalculatePosition();

		glm::quat orientation = GetOrientation();//for rotation to orientation;
		m_viewMartix = glm::translate(glm::mat4(1.f),m_position) * glm::toMat4(orientation);
		m_viewMartix = glm::inverse(m_viewMartix);
	}

	bool EditorCamera::OnMouseScroll(MouseScorllEvent& e)
	{
		if (!Input::IsKeyPressed((int)Key::Left_control))
		{
			float delta = e.GetYOffset() * 0.1f;
			MouseZoom(delta);
			UpdateView();
				
		};

		return false;
	}

	std::pair<float, float> EditorCamera::PanSpeed() const // -> move with Mouse
	{
		float x = std::min(m_viewportWidth / 1000.f, 2.4f);
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_viewportWidth / 1000.f, 2.4f);
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor,yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_distance * 0.4f; //camera to focalPoint distance?
		distance = std::max(distance, 0.f);
		float speed = distance * distance; 

		speed = std::min(speed, 100.f);
		return speed;

	}



	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_focalPoint += -GetRightDirection() * delta.x * xSpeed * m_distance;
		m_focalPoint += GetUpDirection() * delta.y * ySpeed *m_distance;

	}

	void EditorCamera::MouseRotation(const glm::vec2& delta) 
	{
		float yawSign = GetUpDirection().y < 0 ? -1.f : 1.f;
		//it should be rotated about the foclaPoint
		m_yaw += yawSign * delta.x * RotationSpeed();
		m_pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_distance -= delta * ZoomSpeed();
		if (m_distance < 1.f)
		{
			m_focalPoint += GetForwardDirection();
			m_distance = 1.f;
		}
	}


	void EditorCamera::OnUpdate(Timestep ts) //make it like blender!!
	{
		bool leftshift = Input::IsKeyPressed((int)Key::Left_shift);
		bool leftctrl= Input::IsKeyPressed((int)Key::Left_control);
		bool leftalt= Input::IsKeyPressed((int)Key::Left_alt);

		if (leftalt ||leftctrl ||leftshift || Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
		{
			const glm::vec2 mouse{ Input::GetMouseX(),Input::GetMouseY() }; 
			glm::vec2 delta = (mouse - m_initialMousePosition) * 0.003f ;
			m_initialMousePosition = mouse; //got m_initalMousePositon shuold after pressed key and before pressed mouseButton

			

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle)&&leftshift)
			{
				MousePan(delta);
			}
			else if(!leftshift && !leftctrl&& Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
			{
				//FIX: More smooth;
				if (abs(delta.x) > 0.2f || abs(delta.y) > 0.2f) //no Jump
				{
					return;
				}
				MouseRotation(delta);
			}
			else if (leftalt) //4deriction othroGraphicView
			{

			}
			else if (leftctrl&& Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
			{
				MouseZoom(delta.y);
			}
		}
		
		
		
		UpdateView();


	}



	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.DisPatch<MouseScorllEvent>(BIND_EVENT_CALLFN(EditorCamera::OnMouseScroll));
	}

	glm::quat EditorCamera::GetOrientation()const
	{
		return glm::quat(glm::vec3(-m_pitch,-m_yaw,0.f)); //use Orientation to make an DiifrotateQuat (对齐旋转四元数) but no roll?
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_focalPoint - m_distance*GetForwardDirection() ;
	}

	glm::vec3 EditorCamera::GetRightDirection()const
	{
		//view https://www.qiujiawei.com/understanding-quaternions/ to figout how Quaternion work 
		return glm::rotate( GetOrientation(), glm::vec3(1.f, 0.f, 0.f)); //use Quaternions make global x rotate to local x;(just dot mulity as quaternion) 
	}

	glm::vec3 EditorCamera::GetUpDirection()const
	{
		return glm::rotate( GetOrientation(), glm::vec3(0.f, 1.f, 0.f)); //use Quaternions make global y rotate to local y;(just dot mulity as quaternion) 

	}

	//attention: local z positive direction is opposite to the viewface direction
	glm::vec3 EditorCamera::GetForwardDirection()const
	{
		return glm::rotate( GetOrientation(), glm::vec3(0.f, 0.f, -1.f)); //use Quaternions make global z rotate to local z;(just dot mulity as quaternion) 
	}


	void EditorCamera::RecalculateProjection()//for safe
	{

// 		switch (m_projectionType)
// 		{
// 		case ProjectionType::Perspective:
// 			m_projection = glm::perspective(m_fov, m_aspect, m_nearClip,m_farClip); //hen ... like a prespectiveCamera
// 
// 			break;
// 		case ProjectionType::Orthographic:
// 			float size= glm::tan(glm::radians(m_fov))* m_farClip;
// 			float orthoLeft = -0.5f * m_aspect * size;
// 			float orthoRight = 0.5f * m_aspect * size;
// 			float orthoBottom = -0.5f * size;
// 			float orthoUp = 0.5f * size;
// 			m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoUp, m_nearClip, m_farClip); //hen ... like a prespectiveCamera
// 			break;
// 
// 		}

	}

}
