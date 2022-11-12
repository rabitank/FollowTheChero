#pragma once
#include "Hazel/Renderer/Camera.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/ApplicationEvent.h"
namespace Hazel
{

		struct  PerspectiveCameraBounds
		{
			float Left, Right;
			float Bottom, Top;
			float zNear, zFar;

			float GetWidth() { return Right - Left; }
			float GetHeight() { return Top - Bottom; }
			float GetDepth() { return zFar- zNear; }
		};


		class PerspectiveCameraController //it just help user control the camera ,and reduce the camera contains
		{
		public:
			PerspectiveCameraController(float fovy,float aspectratio, bool rotation = false);//aspectratio * 2 units  //横纵比成2个单位
			void OnUpdate(Timestep ts);
			void OnEvent(Event& event);

			PerspectiveCamera& GetCamera() { return m_PerspectiveCamera; };
			const PerspectiveCamera& GetCamera() const { return m_PerspectiveCamera; };


			inline const PerspectiveCameraBounds& GetBound() const { return m_bounds; };

			void OnResiz(float width, float height);
		private:


			bool onMouseScrolled(MouseScorllEvent& event);
			bool onWindowResized(WindowsResizeEvent& event);

			void CalculateView();

		private:
			//又说定义顺序跟初始化顺须一样了..


			//begin data
			float m_fovy;
			float m_aspectratio;
			float m_znear = 1.0f;
			float m_zfar = 10.0f;
			float m_zoomLevel= 1.f;

			//temp data
			PerspectiveCameraBounds m_bounds;
			bool m_rotation;


			PerspectiveCamera m_PerspectiveCamera;

			glm::vec3 m_CameraPosition = { 0.f,0.f,0.f };
			
			float m_CameraRotation = 0.f;
			float m_CameraSpeed = 1.0f;
			float m_CameraRotationSpeed = 90.0f;

			float m_zoomStepLength = 0.2f;

		};

}

