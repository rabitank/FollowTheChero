#pragma once
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel

{
	class OrthographicCameraController //it just help user control the camera ,and reduce the camera contains
	{
	public:
		OrthographicCameraController(float aspectratio, bool rotation = false);//aspectratio * 2 units  //横纵比成2个单位
		void OnUpdate(Timestep ts);
		void OnEvent(Event& event);

		OrthographicCamera& GetCamera() { return m_orthographicCamera; };
		const OrthographicCamera& GetCamera() const  { return m_orthographicCamera; };

		void SetZoomLevel(float level) { m_zoomLevel = level; };
		float GetZoomLevel() {return m_zoomLevel ; };

	private:

		bool onMouseScrolled(MouseScorllEvent& event);
		bool onMouseReleased(MouseButtonPressedEvent& event);
		bool onMousePressed(MouseButtonPressedEvent& event);

		bool onWindowResized(WindowsResizeEvent& event);


	private:
		//又说定义顺序跟初始化顺须一样了..
		float m_aspectratio;
		float m_zoomLevel = 1.0f;
		bool m_rotation;
		OrthographicCamera m_orthographicCamera;

		glm::vec3 m_CameraPosition = {0.f,0.f,0.f};
		float m_CameraRotation =0.f;
		float m_CameraSpeed = 1.0f;
		float m_CameraRotationSpeed =90.0f;
		float m_zoomStepLength =0.2f;

	};

}

