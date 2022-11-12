#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <chrono>




namespace Hazel
{
#if 0
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


	m_TextureMap['W'] = SubTexture2D::CreateFromCroods(m_spriteSheet, { 11,11 }, { 128,128 });
	m_TextureMap['S'] = SubTexture2D::CreateFromCroods(m_spriteSheet, { 6,11 }, { 128,128 });
	m_TextureMap['D'] = SubTexture2D::CreateFromCroods(m_spriteSheet, { 1,11 }, { 128,128 });
	m_TextureMap['T'] = SubTexture2D::CreateFromCroods(m_spriteSheet, { 2,1 }, { 128,128 }, { 1,2 });
	m_TextureMap['R'] = SubTexture2D::CreateFromCroods(m_spriteSheet, { 5,9 }, { 128,128 });
#endif
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
		//m_mapHeight = (uint32_t)strlen(s_mapTiles) / s_mapWidth;
		//m_mapWidth = s_mapWidth;



		FrameBufferSpecification spec{ 1080,720 };
		m_frameBuffer = Hazel::FrameBuffer::Create(spec);

		
		//Create a simple entitiy
		m_activeScene = CreateRef<Scene>();
		Entity squareEntity = m_activeScene->CreateEntity("Square");
		bool hasTrans = squareEntity.HasComponent<TransformComponent>();
		squareEntity.AddComponent< SpriteRendererComponent>(glm::vec4(0.6f, 0.2f, 0.3f, 1.f));
		m_quadEntity = squareEntity;

		//Create two simple camera
		m_cameraEntity = m_activeScene->CreateEntity("Camrea");
		m_cameraEntity.AddComponent<CameraComponent>(glm::ortho(-16.f,16.f,-9.f,9.f)).Primary=true;
		m_secondCamera = m_activeScene->CreateEntity("SecondCamrea");
		m_secondCamera.AddComponent<CameraComponent>(glm::ortho(-1.f,1.f,-1.f,1.f)).Primary=true; //in fact , the latter entity will be poped earlier

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

		//Setting
		{
			ImGui::Begin("Setting");

			ImGui::Text("Renderer2D test - stats:");
			ImGui::Text("DrawCall:%d", stats.DrawCalls);
			ImGui::Text("quadCount: %d", stats.quadCount);
			ImGui::Text("Vertex:%d", stats.GetTotalVertexCount());
			ImGui::Text("Indeics:%d", stats.GetTotalIndexCount());


			//TODO: abstract
			if (m_quadEntity)
			{
				ImGui::Separator();
				ImGui::Text("%s", m_quadEntity.GetComponent<TagComponent>().Tag.c_str()); //记得.., 要的是 c风格字符
				ImGui::ColorEdit4("quad color",glm::value_ptr(m_quadEntity.GetComponent<SpriteRendererComponent>().Color));
				ImGui::Separator();
				
				
			}
			
			glm::mat4& cameraTransMat = m_activeScene->GetMainCamera()->GetComponent<TransformComponent>().Transform;
			ImGui::Text("%s", m_cameraEntity.GetComponent<TagComponent>().Tag.c_str()); //记得.., 要的是 c风格字符
			ImGui::DragFloat3("camera Position x", glm::value_ptr(cameraTransMat[3]));

			if(ImGui::Checkbox("Camer A:",&m_primaryCamera))//对勾小盒子..返回是否点击..引用绑定m_primaryCamera
			{
				m_secondCamera.GetComponent<CameraComponent>().Primary = !m_primaryCamera;
				m_cameraEntity.GetComponent<CameraComponent>().Primary = m_primaryCamera;
				ImGui::Separator();
			}

			ImGui::End();
		}


		//ViewPort
		{

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0}); //make port no board

			ImGui::Begin("ViewPort");

			// IsWindow...() 查询的是所在上下文的窗口的状态

			m_viewPortIsFocus=ImGui::IsWindowFocused();
			m_viewPortIsHover = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->SetBlockEvent(!m_viewPortIsFocus || !m_viewPortIsHover);

			ImVec2 viewPortPanelSize = ImGui::GetContentRegionAvail();//ViewPort这panel的内容渲染大小...我们需要framebuffer跟随它改变以减少不必要的buffer写入
			if (m_ViewPortSize != *(glm::vec2*)&viewPortPanelSize && viewPortPanelSize.x>0&&viewPortPanelSize.y>0)
			{
				m_ViewPortSize = { viewPortPanelSize.x,viewPortPanelSize.y };
				m_frameBuffer->ReSize((uint32_t)viewPortPanelSize.x, (uint32_t)viewPortPanelSize.y);


				m_cameraController.OnResiz(m_ViewPortSize.x, m_ViewPortSize.y); //相机跟着视口,glviewPort一起变化就不会引起形变了..
			}

			ImGui::Image((ImTextureID)m_frameBuffer->GetColorAttachMentRendererID(), { m_ViewPortSize.x,m_ViewPortSize.y}, { 0,1 }, { 1,0 }); //.... 所以都是用gl的id吗...  
			ImGui::End();

			ImGui::PopStyleVar();
		}

		//dockSpace's end
		ImGui::End();



	}

	void EditorLayer::OnUpdate(Hazel::Timestep deltaime)
	{


		HZ_PROFILE_FUNCTION();

		{
			HZ_PROFILE_SCOPE("CameraController::OnUpdate");
			if (m_viewPortIsFocus)//没聚焦就关掉位置刷新
			{
				m_cameraController.OnUpdate(deltaime); //也减少了资源耗费对吧
			}
		}

		{
			HZ_PROFILE_SCOPE("Renderer Prepare ");
			Renderer2D::ResetStats();
			m_frameBuffer->Bind(); //every frame;

			//Clear 必须
			RenderCommand::SetClearColor({ 0.1f,0.1,0.1f,1.f });
			RenderCommand::Clear();
		}


		{
			HZ_PROFILE_SCOPE("Render Draw");
			m_activeScene->OnUpdate(deltaime);
		}
		m_frameBuffer->UnBind();


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
		if (m_viewPortIsFocus)
		{
			m_cameraController.OnEvent(e); //没聚焦就不要管滚轮对吧
		}

	}

}
