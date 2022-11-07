#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <chrono>




namespace Hazel
{

	static const uint32_t s_mapWidth = 31;
	static const char* s_mapTiles =
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWSSSSWWWWWWWWWWWWW"
		"WWWWWWWWWWSSSSDDDDDSRRRRWWWWWWW"
		"WWWWWWWSSDDDDDDDDDDDDDDRRWWWWWW"
		"WWWWWWSSSDDDDDDDDDDDDDDDRRWWWWW"
		"WWWWSSSSSDDDDDDTDDDDDDDWWWWWWWW"
		"WWSSSWWSSDDDDDWWWDDDDRWWWWWWWWW"
		"WWWWWWSSDDTDDDWWWDDDDRRWWWWWWWW"
		"WWWWSSDDDDDDDDDDDDSSSRRWWWWWWWW"
		"WWWWWWWSSDDDDDDDTDDCWWWWWWWWWWW"
		"WWWWWWWSSDDDDDDDDDDDDDWWWWWWWWW"
		"WWWWWWWSSSSSSSSSSSSSSSWWWWWWWWW"
		"WWWWWWWWWWWWSSSSSSRWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWRSSRRRWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
		;

	EditorLayer::EditorLayer()
		:Layer("SandBox2D"), m_cameraController(1280.f / 720.f, true)
	{}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		m_boardTexture = Hazel::Texture2D::Create("Assets/Texture/99.png");
		m_quancifangTexture = Hazel::Texture2D::Create("Assets/Texture/quancifang.png");
		m_spriteSheet = Hazel::Texture2D::Create("Assets/Texture/game/RPGpack_sheet_2X.png");
		m_spriteHair = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 7,3 }, { 128,128 });
		m_spriteTree = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 2,1 }, { 128,128 }, { 1,2 });
		//m_boardTexture->Bind();

		//set particle 
		m_particle.LifeTiem = 1.0f;
		m_particle.colorBegin = { 1.0f,0.9f,0.5f,1.0f };
		m_particle.colorEnd = { 1.0f,0.4f,0.2f,1.0f };
		m_particle.SizeBeging = 0.5f, m_particle.SizeVariation = 0.3f, m_particle.SizeEnd = 0.f;
		m_particle.Velocity = { 0.f,0.f };
		m_particle.VelocityVariation = { 3.0f,1.f };
		m_particle.Position = { 0.0f,0.f };

		m_cameraController.SetZoomLevel(5.f);

		//Init the maptexture
		m_mapHeight = (uint32_t)strlen(s_mapTiles) / s_mapWidth;
		m_mapWidth = s_mapWidth;

		m_TextureMap['W'] = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 11,11 }, { 128,128 });
		m_TextureMap['S'] = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 6,11 }, { 128,128 });
		m_TextureMap['D'] = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 1,11 }, { 128,128 });
		m_TextureMap['T'] = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 2,1 }, { 128,128 }, { 1,2 });
		m_TextureMap['R'] = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 5,9 }, { 128,128 });


		Hazel::FrameBufferSpecification spec{ 1080,720 };
		m_FrameBuffer = Hazel::FrameBuffer::Create(spec);
	}

	void EditorLayer::OnDetach()
	{

	}


	//ImGui Dock 
	static void ShowDockingDisabledMessage()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
		ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
		ImGui::SameLine(0.0f, 0.0f);
		if (ImGui::SmallButton("click here"))
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	void EditorLayer::OnImGuiRender()
	{
		HZ_PROFILE_FUNCTION();

		auto stats = Hazel::Renderer2D::GetStats();


		////////////////////////////////////
		//Dock Part
		//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		///////////////////////////////////

		static bool dockSpace = true;

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;


		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}


		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpace, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
			ShowDockingDisabledMessage();
		}


		//a menu for option about dockSpace ,unnecessary
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("file"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				//ImGui::MenuItem("Padding", NULL, &opt_padding);
				//ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
					Hazel::Application::Get().Close();
				ImGui::Separator();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (0)
		{
			ImGui::Begin("Status view port");

			ImGui::Text("Renderer2D test - stats:");
			ImGui::Text("DrawCall:%d", stats.DrawCalls);
			ImGui::Text("quadCount: %d", stats.quadCount);
			ImGui::Text("Vertex:%d", stats.GetTotalVertexCount());
			ImGui::Text("Indeics:%d", stats.GetTotalIndexCount());
			ImGui::ColorEdit4("Grid Color", glm::value_ptr(m_flatColor));
			ImGui::Image((ImTextureID)m_FrameBuffer->GetColorAttachMentRendererID(), { 1080.f,720.f }, { 0,1 }, { 1,0 }); //.... 所以都是用gl的id吗...  
		}
		else
		{
			ImGui::Begin("Status view port");

			ImGui::Text("Renderer2D test - stats:");
			ImGui::Text("DrawCall:%d", stats.DrawCalls);
			ImGui::Text("quadCount: %d", stats.quadCount);
			ImGui::Text("Vertex:%d", stats.GetTotalVertexCount());
			ImGui::Text("Indeics:%d", stats.GetTotalIndexCount());
			ImGui::ColorEdit4("Grid Color", glm::value_ptr(m_flatColor));
			ImGui::Image((ImTextureID)m_quancifangTexture->GetRendererID(), { 1080.f,720.f }, { 0,1 }, { 1,0 }); //ImGui是从左上角开始绘制的....设置下uv
		}
		ImGui::End();


		ImGui::End();
	}

	void EditorLayer::OnUpdate(Hazel::Timestep deltaime)
	{


		HZ_PROFILE_FUNCTION();

		{
			HZ_PROFILE_SCOPE("CameraController::OnUpdate");
			m_cameraController.OnUpdate(deltaime);
		}

		Hazel::Renderer2D::ResetStats();

		{


			HZ_PROFILE_SCOPE("Renderer Prepare ");
			m_FrameBuffer->Bind(); //every frame;

			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
			RenderCommand::Clear(); //↓	//why i can't see the layer before ? you clear it
		}

		static float rotation = 0.f;

		{

			//rotation += deltaime;

			HZ_PROFILE_SCOPE("Render Draw");

			Renderer2D::BeginScene(m_cameraController.GetCamera());//开始调度,设置场景参数 //提交场景的相机 ,注意 是"提交" , vp
			Renderer2D::DrawQuad({ 1.f,0.2f }, { 0.3f,1.5f }, { 1.f,0.2f,0.4f,1.f });
			Renderer2D::DrawQuad({ -1.f,-1.f,-0.1f }, { 1.f,1.f }, { 0.3f,0.5f,0.9f,1.0f });
			Renderer2D::DrawQuad({ -5.f,-5.f,-0.2f }, { 10.f,10.f }, m_boardTexture, 10.f);
			//Hazel::Renderer2D::DrawQuad({ -1.f,1.f,0.1f }, { 1.f,1.f },m_quancifangTexture);
			//Hazel::Renderer2D::DrawRotatedQuad({ 0.5f,0.2f }, { 2.f,2.f }, glm::degrees(rotation),m_quancifangTexture);

			for (float x = -5.0f; x < 5.f; x += 0.5f)
			{
				for (float y = -5.0f; y < 5.f; y += 0.5f)
				{
					Hazel::Renderer2D::DrawQuad({ x ,y ,-0.1f }, { 0.5f,0.5f }, { 0.2f,(x + 5.f) / 10.f,(y + 5.f) / 10.f,0.5f });
				}
			}
			Hazel::Renderer2D::EndScene();

			m_FrameBuffer->UnBind();
		}

		if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Hazel::Input::GetMousePos();
			auto width = Hazel::Application::Get().GetWindow().GetWidth();
			auto height = Hazel::Application::Get().GetWindow().GetHeight();

			auto bounds = m_cameraController.GetBound();
			auto pos = m_cameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = -(y / height) * bounds.GetHeight() + bounds.GetHeight() * 0.5f;//y轴向下;
			m_particle.Position = { x + pos.x,y + pos.y };
			for (int i = 0; i < 5; i++) //emit 5 time?
			{
				m_particleSystem.Emit(m_particle);
			}

		}



		m_particleSystem.OnUpdate(deltaime);
		m_particleSystem.OnRender(m_cameraController.GetCamera());



#if 0
		for (int y = 0; y < (int)m_mapHeight; y++)
		{
			for (int x = 0; x < (int)m_mapWidth; x++)
			{
				char tileType = s_mapTiles[y * m_mapWidth + x];
				Hazel::Ref<Hazel::SubTexture2D> tileTexture;
				if (m_TextureMap.find(tileType) == m_TextureMap.end())
				{
					tileTexture = m_spriteHair;
					HZ_ERROR("didn,t find tile type:{0}", tileType);
				}
				else
				{

					tileTexture = m_TextureMap[tileType]; //小心....unordedmap就算没有也会给你返回个空的..这回导致一些难以识别的崩溃 //所以加个判别
				}
				Hazel::Renderer2D::DrawQuad({ x - m_mapWidth / 2.f,(-y + m_mapHeight - 1) - m_mapHeight / 2.f,0.2f }, { 1.f,1.f }, tileTexture);
			}

		}

		Hazel::Renderer2D::BeginScene(m_cameraController.GetCamera());
		Hazel::Renderer2D::DrawQuad({ 0.f,0.0f,0.1f }, { 1.f,1.f }, m_spriteHair);
		Hazel::Renderer2D::DrawQuad({ 1.f,0.0f,0.1f }, { 1.f,2.f }, m_spriteTree);

		Hazel::Renderer2D::EndScene();

#endif


	}

	void EditorLayer::OnEvent(Hazel::Event& e)
	{
		m_cameraController.OnEvent(e);

	}

}
