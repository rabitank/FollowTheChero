#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel
{

	OrthographicCameraController::OrthographicCameraController(float aspectratio,bool rotation)
		:m_aspectratio(aspectratio),m_rotation(rotation),m_orthographicCamera(-m_aspectratio*m_zoomLevel,m_aspectratio*m_zoomLevel,-m_zoomLevel,m_zoomLevel)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep deltaime)
	{

		HZ_PROFILE_FUNCTION();

		//control ¡ý . move
		if (Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition.x -= deltaime * m_CameraSpeed;
		else if (Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition.x += deltaime * m_CameraSpeed;
		if (Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition.y += deltaime * m_CameraSpeed;
		else if (Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition.y -= deltaime * m_CameraSpeed;

		if (m_rotation)
		{

		if (Input::IsKeyPressed(HZ_KEY_Q))
			m_CameraRotation += deltaime * m_CameraRotationSpeed;
		else if (Input::IsKeyPressed(HZ_KEY_E))
			m_CameraRotation -= deltaime * m_CameraRotationSpeed;

		m_orthographicCamera.SetRotation(m_CameraRotation);
		}


		m_orthographicCamera.SetPosition(m_CameraPosition);

		m_CameraSpeed = m_zoomLevel;

	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		HZ_PROFILE_FUNCTION();


		EventDispatcher dispatcher(event);
		dispatcher.DisPatch<MouseScorllEvent>(BIND_EVENT_CALLFN(OrthographicCameraController::onMouseScrolled));
		dispatcher.DisPatch<WindowsResizeEvent>(BIND_EVENT_CALLFN(OrthographicCameraController::onWindowResized));

	}

	bool OrthographicCameraController::onMouseScrolled(MouseScorllEvent& event)
	{

		HZ_PROFILE_FUNCTION();

		m_zoomLevel -=  m_zoomStepLength*event.GetYOffset();
		m_orthographicCamera.SetProjection(-m_aspectratio * m_zoomLevel, m_aspectratio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		return false;	

	}

	bool OrthographicCameraController::onWindowResized(WindowsResizeEvent& event)
	{
		HZ_PROFILE_FUNCTION();

		m_aspectratio = (float) event.GetWidth() / (float)event.GetHeight();
		m_orthographicCamera.SetProjection(-m_aspectratio * m_zoomLevel, m_aspectratio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}


	bool OrthographicCameraController::onMouseReleased(MouseButtonPressedEvent& event)
	{

		return false;
	}

	bool OrthographicCameraController::onMousePressed(MouseButtonPressedEvent& event)
	{
		return false;

	}
}