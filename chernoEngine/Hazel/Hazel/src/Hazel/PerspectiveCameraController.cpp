#include "hzpch.h"
#include "PerspectiveCameraController.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
namespace Hazel
{
	PerspectiveCameraController::PerspectiveCameraController(float fovy,float aspectratio, bool rotation)
		:m_bounds({ -m_aspectratio * m_zfar*m_zoomLevel,m_aspectratio * m_zfar* m_zoomLevel,-fovy*m_zfar* m_zoomLevel,fovy*m_zfar* m_zoomLevel}),
		m_aspectratio(aspectratio),m_fovy(fovy), m_rotation(rotation), 
		m_PerspectiveCamera(m_bounds.Left, m_bounds.Right, m_bounds.Bottom, m_bounds.Top,m_bounds.zNear,m_bounds.zFar)
	{

	}


	void PerspectiveCameraController::OnUpdate(Timestep deltaime)
	{

		HZ_PROFILE_FUNCTION();

		//control ↓ . move
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

			m_PerspectiveCamera.SetRotation(m_CameraRotation);
		}


		m_PerspectiveCamera.SetPosition(m_CameraPosition);

		m_CameraSpeed = m_znear;

	}

	void PerspectiveCameraController::OnEvent(Event& event)
	{
		HZ_PROFILE_FUNCTION();


		EventDispatcher dispatcher(event);
		dispatcher.DisPatch<MouseScorllEvent>(BIND_EVENT_CALLFN(PerspectiveCameraController::onMouseScrolled));
		dispatcher.DisPatch<WindowsResizeEvent>(BIND_EVENT_CALLFN(PerspectiveCameraController::onWindowResized));

	}


	void PerspectiveCameraController::CalculateView()
	{
		m_bounds = { -m_aspectratio* m_zfar*m_zoomLevel,m_aspectratio * m_zfar*m_zoomLevel,-m_fovy * m_zfar * m_zoomLevel,m_fovy * m_zfar * m_zoomLevel };
		m_PerspectiveCamera.SetProjection(m_bounds.Left, m_bounds.Right, m_bounds.Bottom, m_bounds.Top,m_bounds.zNear,m_bounds.zFar);

	}

	bool PerspectiveCameraController::onMouseScrolled(MouseScorllEvent& event)
	{

		HZ_PROFILE_FUNCTION();

		m_zoomLevel -= m_zoomStepLength * event.GetYOffset();
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		CalculateView();
		return false;

	}

	//好处只改变宽高比 而不会改变尺寸
	void PerspectiveCameraController::OnResiz(float width, float height)
	{
		m_aspectratio = width / height;
		CalculateView();
	}

	bool PerspectiveCameraController::onWindowResized(WindowsResizeEvent& event)
	{
		HZ_PROFILE_FUNCTION();

		OnResiz((float)event.GetWidth(), (float)event.GetHeight());
		return false;
	}

}