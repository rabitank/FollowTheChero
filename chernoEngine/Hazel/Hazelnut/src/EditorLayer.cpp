#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include "Hazel/Scene/SceneSerializer.h"
#include "Hazel/Utils/PlantformUtils.h"

#include "ImGuizmo.h"
#include "Hazel/Math/Math.h"


namespace Hazel
{

	extern const std::filesystem::path g_AssetsDirectory ;//外部声明的文件路径常量: contentBrowserPanel 里

#if 0
	m_TextureMap['W'] = SubTexture2D::CreateFromCroods(m_spriteSheet, { 11,11 }, { 128,128 });
	m_TextureMap['S'] = SubTexture2D::CreateFromCroods(m_spriteSheet, { 6,11 }, { 128,128 });
	m_TextureMap['D'] = SubTexture2D::CreateFromCroods(m_spriteSheet, { 1,11 }, { 128,128 });
	m_TextureMap['T'] = SubTexture2D::CreateFromCroods(m_spriteSheet, { 2,1 }, { 128,128 }, { 1,2 });
	m_TextureMap['R'] = SubTexture2D::CreateFromCroods(m_spriteSheet, { 5,9 }, { 128,128 });
#endif
	EditorLayer::EditorLayer()
		:Layer("SandBox2D")//, m_cameraController(1280.f / 720.f, true)
	{}




	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();
		FrameBufferSpecification spec{ 1080,720 };
		spec.Attachments = { { FrameBufferTextureFormat::RGBA8},{ FrameBufferTextureFormat::RED_INTEGER},{FrameBufferTextureFormat::DEPTH } };
		m_frameBuffer = Hazel::FrameBuffer::Create(spec);

		//FrameBufferSpecification IDspec{ 1080,720 };
		//IDspec.Attachments = { {FrameBufferTextureFormat::DEPTH_STENCIL} };
		//m_frameBuffer = Hazel::FrameBuffer::Create(IDspec);



		//Create a simple entitiy
		m_activeScene = CreateRef<Scene>();
		m_editScene = m_activeScene;

		m_editorCamera = EditorCamera(30.f, 1.778f, 0.1f, 1000.f);


		m_IconPlay= Texture2D::Create("Resources/Icon/playbuttonIcon.png");
		m_IconStop= Texture2D::Create("Resources/Icon/stopbuttonIcon.png");


#if 0
		//Init the maptexture
		//m_mapHeight = (uint32_t)strlen(s_mapTiles) / s_mapWidth;
		//m_mapWidth = s_mapWidth;
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

#endif

#if 0
		Entity squareEntity = m_activeScene->CreateEntity("SquareA");
		squareEntity.AddComponent< SpriteRendererComponent>(glm::vec4(0.6f, 0.2f, 0.3f, 1.f));

		Entity square2Entity = m_activeScene->CreateEntity("SquareB");
		square2Entity.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.7f, 0.4f, 1.f));
		square2Entity.GetComponent<TransformComponent>().Translation= glm::vec3(1.f,1.f,0.2f);

		//Create two simple camera
		m_cameraEntity = m_activeScene->CreateEntity("CamreaA");
		m_cameraEntity.AddComponent<CameraComponent>().Primary=true;
		m_secondCamera = m_activeScene->CreateEntity("CamreaB");
		m_secondCamera.AddComponent<CameraComponent>().Primary=true; //in fact , the latter entity will be poped earlier



		//it shoule be someKind Entity
		class CameraController:public ScriptableEntity
		{
		public:
			CameraController() {};

			virtual void OnCreate() override
			{
				//GetComponent<TransformComponent>();
				std::cout << "CameraController : OnCreate! "<< std::endl;
				
				auto& transform = GetComponent<TransformComponent>().Translation;
				
				transform[0] = rand() % 10 - 5.f;
			}

			virtual void OnUpdate(Timestep ts ) override
			{

				auto& transform = GetComponent<TransformComponent>().Translation;
				if (Input::IsKeyPressed(HZ_KEY_A))
					transform[0] -= m_cameraSpeed * ts;
				if (Input::IsKeyPressed(HZ_KEY_D))
					transform[0] += m_cameraSpeed * ts;
				if (Input::IsKeyPressed(HZ_KEY_W))
					transform[1] += m_cameraSpeed * ts;
				if (Input::IsKeyPressed(HZ_KEY_S))
					transform[1] -= m_cameraSpeed * ts;

			}

			virtual void OnDestroy() override
			{

				std::cout << "CameraController :OnDestroy! "<< std::endl;
			}
		private:
			float m_cameraSpeed = 1.f;

		};

		m_secondCamera.AddComponent<NativeScripComponent>().Bind<CameraController>();
		m_cameraEntity.AddComponent<NativeScripComponent>().Bind<CameraController>();

#endif

		//m_sceneHierarchyPanel.SetContext(m_activeScene);

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

		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;


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
					Application::Get().Close();

				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("SaveAs...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{

					OpenScene();
				}
				ImGui::Separator();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		//Stats 
		{
			ImGui::Begin("Render Stats");

			std::string name = "None";
			if (m_hoveredEntity)
				name = m_hoveredEntity.GetComponent<TagComponent>().Tag;
			ImGui::Text("Hovered Entity:%s", name.c_str());

			ImGui::Text("Renderer2D test - stats:");
			ImGui::Text("DrawCall:%d", stats.DrawCalls);
			ImGui::Text("quadCount: %d", stats.quadCount);
			ImGui::Text("Vertex:%d", stats.GetTotalVertexCount());
			ImGui::Text("Indeics:%d", stats.GetTotalIndexCount());


			ImGui::End();
		}
		//Setting
		{
			ImGui::Begin("Settings");
			ImGui::Checkbox("Visual Physics Collider", &m_showPhysicsColliders);
			bool& iflock = m_editorCamera.IfLockRotation();
			ImGui::Checkbox("Lock EditorCamer Rotation", &iflock);

			ImGui::End();
		}
		//sceneHierarchyPanel
		{
			m_sceneHierarchyPanel.OnImGuiRender();

		}

		//contentBrowserPanel
		{

			m_contentBrowserPanel.OnImGuiRender();
		}

		//ViewPort
		{
			HZ_PROFILE_SCOPE("ViewPortRender:");
			//make port no board
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
			ImGui::Begin("ViewPort");

			auto viewPortOffset = ImGui::GetWindowPos(); //Include Tab bar

			ImVec2 minBound = ImGui::GetWindowContentRegionMin();
			ImVec2 maxBound = ImGui::GetWindowContentRegionMax(); //the viewPortImage rightButtom

			m_viewPortBounds[0] = { minBound.x + viewPortOffset.x,minBound.y + viewPortOffset.y };
			m_viewPortBounds[1] = { maxBound.x + viewPortOffset.x,maxBound.y + viewPortOffset.y };


			// IsWindow...() 查询的是所在上下文的窗口的状态
			m_viewPortIsFocus = ImGui::IsWindowFocused();
			m_viewPortIsHover = ImGui::IsWindowHovered();
			//Application::Get().GetImGuiLayer()->SetBlockEvent(!m_viewPortIsFocus || !m_viewPortIsHover);
			Application::Get().GetImGuiLayer()->SetBlockEvent( !m_viewPortIsHover);




			// view Port Resize: 
			ImVec2 viewPortPanelSize = ImGui::GetContentRegionAvail();
			//ViewPort这panel的内容渲染大小...我们需要framebuffer跟随它改变以减少不必要的buffer写入

			//viewPortResize framebuffer 移动到 update
			m_ViewPortSize = { viewPortPanelSize.x,viewPortPanelSize.y };


			//show what rendered
			uint64_t textureID = m_frameBuffer->GetColorAttachMentRendererID();
			ImGui::Image(reinterpret_cast<void*>(textureID), { m_ViewPortSize.x,m_ViewPortSize.y }, { 0,1 }, { 1,0 }); //.... 所以都是用gl的id吗...  


			//Drag
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data; //wchar_t 是windows常用的utf-16格式
					OpenScene(std::filesystem::path (g_AssetsDirectory)/path);
				}
				ImGui::EndDragDropTarget();

			}


			//Gizmos  //it should just be used in EditorTime 
			Entity selectedentity = m_sceneHierarchyPanel.GetSelectedEntity();
			if (m_sceneState == SceneState::Edit && selectedentity)
			{
				ImGuizmo::SetDrawlist();
				float windowWidth = ImGui::GetWindowWidth();
				float windowHeight = ImGui::GetWindowHeight();

				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);//设置矩形坐标 左上右下(画布覆盖整个ViewPort?)

				//Camera


				const glm::mat4& cameraProjectMat = m_editorCamera.GetProjection();
				glm::mat4 cameraViewMat = m_editorCamera.GetViewMatrix();

				//Entity
				auto& transcomp = selectedentity.GetComponent<TransformComponent>();
				glm::mat4 transform = transcomp.GetTransform();


				//snap / capture .. rotation/tanslate with grids
				bool snap = Input::IsKeyPressed((int)Keycode::Left_control);
				glm::vec4 snapvalue = { 0.5f,15.f,0.5f,0.5f };
				if (m_gizmoMod != -1)
				{
					ImGuizmo::Manipulate(glm::value_ptr(cameraViewMat), glm::value_ptr(cameraProjectMat),
						(ImGuizmo::OPERATION)m_gizmoMod, ImGuizmo::MODE::WORLD, glm::value_ptr(transform), nullptr, snap ? &snapvalue[(int)m_gizmoMod] : nullptr);

				}

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 translation, rotation, scale;
					Hazel::Math::DecomposeTransform(transform, translation, rotation, scale);
					glm::vec3 deletaRotation = rotation - transcomp.Rotation; //for no lock

					transcomp.Translation = translation;
					transcomp.Rotation += deletaRotation;
					transcomp.Scale = scale;
				};
#if 0
				//RunTimeGzmos
				//auto cameraEntity = m_activeScene->GetPrimaryCamera();
				if (cameraEntity && m_gizmoMod != -1)
				{
					const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
					const glm::mat4& cameraProjectMat = camera.GetProjection();
					glm::mat4 cameraViewMat = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

					//Entity
					auto& transcomp = selectedentity.GetComponent<TransformComponent>();
					glm::mat4 transform = transcomp.GetTransform();


					//snap / capture .. rotation/tanslate with grids
					bool snap = Input::IsKeyPressed((int)Keycode::Left_control);
					glm::vec4 snapvalue = { 0.5f,15.f,0.5f,0.5f };


					ImGuizmo::Manipulate(glm::value_ptr(cameraViewMat), glm::value_ptr(cameraProjectMat),
						(ImGuizmo::OPERATION)m_gizmoMod, ImGuizmo::MODE::WORLD, glm::value_ptr(transform), nullptr, snap ? &snapvalue[(int)m_gizmoMod] : nullptr);


					if (ImGuizmo::IsUsing())
					{
						glm::vec3 translation, rotation, scale;
						Hazel::Math::DecomposeTransform(transform, translation, rotation, scale);
						glm::vec3 deletaRotation = rotation - transcomp.Rotation; //for no lock

						transcomp.Translation = translation;
						transcomp.Rotation += deletaRotation;
						transcomp.Scale = scale;
					}
#endif

			}

		}


		ImGui::End();
		ImGui::PopStyleVar();
		//dockSpace's end
		ImGui::End();

		UI_Toolbar();
	}


	void EditorLayer::UI_Toolbar()
	{

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,2 });//垂直向两个像素的填充
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0,0 });//item无间距 
		ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { buttonHovered.x,buttonHovered.y,buttonHovered.z,0.5f });
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { buttonActive.x,buttonActive.y,buttonActive.z ,0.5f });

		ImGui::Begin("##toolsbar",nullptr, ImGuiWindowFlags_NoDecoration| ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_NoScrollWithMouse);
		Ref<Texture2D> Icon = m_sceneState == SceneState::Edit ? m_IconPlay : m_IconStop;

		float size = ImGui::GetWindowHeight() - 4.0;
		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x *0.5f)- size*0.5); //this line start (offset)-> jump to useable window middle to draw
		//无标题,id:toolsbar,无关闭按钮(p_open),无装饰
		if (ImGui::ImageButton((ImTextureID)Icon->GetRendererID(), ImVec2(size,size), { 0.f,0.f }, {1.f,1.f},0))
		{
			if (m_sceneState == SceneState::Edit)
				OnScenePlay();
			else if(m_sceneState == SceneState::Play)
				OnSceneStop	();

		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);

		ImGui::End();

	}



	void EditorLayer::OnUpdate(Hazel::Timestep deltaime)
	{

		HZ_PROFILE_FUNCTION();



		if (auto spec = m_frameBuffer->GetSpecification(); m_ViewPortSize.x > 0.f && m_ViewPortSize.y > 0.f &&
			(spec.width != m_ViewPortSize.x || spec.height != m_ViewPortSize.y))
		{
			m_editorCamera.SetViewPortSize(m_ViewPortSize.x, m_ViewPortSize.y);
			m_frameBuffer->ReSize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			m_activeScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		}



		

		{
			HZ_PROFILE_SCOPE("Renderer Prepare ");
			Renderer2D::ResetStats();

			m_frameBuffer->Bind(); //every frame;
			//Clear 必须
			RenderCommand::SetClearColor({ 0.1f,0.1,0.1f,1.f });
			RenderCommand::Clear();

			//Clear	entityID Attachment to -1
			m_frameBuffer->ClearColorAttachment(1, -1);

				
		}

		switch (m_sceneState)
		{
			case SceneState::Edit:
			{
				{
					HZ_PROFILE_SCOPE("CameraController::OnUpdate");
					if (m_viewPortIsFocus)//没聚焦就关掉位置刷新
					{
						//m_cameraController.OnUpdate(deltaime); //也减少了资源耗费对吧
						m_editorCamera.OnUpdate(deltaime);
					}
				}

				{
					HZ_PROFILE_SCOPE("Render Draw");
					//CHANGED:
					m_editScene->OnUpdateEditor(deltaime, m_editorCamera);
				}

				break;
			}
			case SceneState::Play:
			{
				{
					HZ_PROFILE_SCOPE("Render Draw");
					//CHANGED:
					m_activeScene->OnUpdateRuntime(deltaime);
				}
				break;

			}



		}



		//calculate the relative Position
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_viewPortBounds[0].x; 
		my -= m_viewPortBounds[0].y;
		glm::vec2 viewPortSize = m_viewPortBounds[1] - m_viewPortBounds[0];

		//updown y to alien with openglCoord
		my = viewPortSize.y - my;

		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX > 0 && mouseY > 0 && mouseX < viewPortSize.x && mouseY < viewPortSize.y)
		{
			//跟渲染有关, m_hoveredEntity的初始化问题 ,不知道为什么没有渲染时ID有初始化默认值.
			int piexlID= m_frameBuffer->ReadPixel(1, mouseX, mouseY);
			m_hoveredEntity = piexlID == -1  ? Entity() : Entity((entt::entity)piexlID, m_activeScene.get());
		}
		else
		{
			m_hoveredEntity = Entity();
		}

		OnOverlayRender();

		m_frameBuffer->UnBind();



	}

	void EditorLayer::OnOverlayRender()
	{


		if (m_sceneState == SceneState::Play)
		{
			auto PlayCamera = m_activeScene->GetPrimaryCamera();
			Renderer2D::BeginScene(PlayCamera.GetComponent<CameraComponent>().Camera, PlayCamera.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_editorCamera);

		}
		//啊?? 
		if (m_showPhysicsColliders)
		{
			//Circle Colliders
			{
				auto view = m_activeScene->GetAllEntityWith<TransformComponent, CircleCollider2DComponent>();
				for (auto e : view)
				{
					auto [tc, cc] = view.get<TransformComponent, CircleCollider2DComponent>(e); // view只含有部分componennt,view的get只能获得当初view指定的component
					glm::vec3 translation = tc.Translation + glm::vec3(cc.Offset, 0.001f);
					glm::mat4 transformMat = glm::translate(glm::mat4(1.f), translation) * glm::scale(glm::mat4(1.f), glm::vec3(tc.Scale.x * cc.Radius * 2));
					Renderer2D::DrawCircle(transformMat, glm::vec4(0.0f, 1.f, 0.0f, 1.f), 0.03f);

				}

			}		
			//Box colliders
			{
				auto view = m_activeScene->GetAllEntityWith<TransformComponent, BoxCollider2DComponent>();
				for (auto e : view)
				{
					auto [tc, bc] = view.get<TransformComponent, BoxCollider2DComponent>(e); // view只含有部分componennt,view的get只能获得当初view指定的component
					glm::vec3 translation = tc.Translation + glm::vec3(bc.Offset, 0.001f);
					glm::mat4 transformMat = glm::translate(glm::mat4(1.f), translation)
						* glm::rotate(glm::mat4(1.f), tc.Rotation.z, glm::vec3(0.f, 0.f, 1.f))
						* glm::scale(glm::mat4(1.f), glm::vec3(tc.Scale.x * bc.Size.x * 2, tc.Scale.y * bc.Size.y * 2, 0.f));
					
					Renderer2D::DrawRect(transformMat, glm::vec4(0.f, 1.f, 0.0f, 1.f));

				}

			}
		}


		Renderer2D::EndScene();
	}


	void EditorLayer::OnEvent(Hazel::Event& e)
	{
		m_editorCamera.OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.DisPatch<KeyPressedEvent>(BIND_EVENT_CALLFN(EditorLayer::OnKeyPressed));
		dispatcher.DisPatch<MouseButtonPressedEvent>(BIND_EVENT_CALLFN(EditorLayer::OnMousebuttomPressed));

	}



	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{

		if (e.GetRepeatcount() > 0) //if you want pressed an hotKeies ,you might have pressed it for a while;
			return false;
		
		bool CtrlPressed = Input::IsKeyPressed((int)Key::Left_control) || Input::IsKeyPressed((int)Key::Right_control);
		bool ShiftPressed = Input::IsKeyPressed((int)Key::Left_shift) || Input::IsKeyPressed((int)Key::Right_shift);
		switch (e.GetKeyCode())
		{
		case Key::N:
			{
				if (CtrlPressed)
					NewScene();
				break;
			}
		case Key::S:
			{

				
				if (CtrlPressed)
				{

					if (ShiftPressed)
					{
						SaveSceneAs();
						break;
					}
					else
					{
						SaveScene();
						break;
					}
				}
	
				m_gizmoMod = m_gizmoMod == ImGuizmo::OPERATION::SCALE? -1 : ImGuizmo::OPERATION::SCALE;
				break;
			}
		case Key::O:
		{
			if (CtrlPressed)
				OpenScene();
			break;
		}
		case Key::D:
		{
			if (CtrlPressed)
				OnDuplicateEntity();
			break;
		}

		case Key::G:
		{
			m_gizmoMod = m_gizmoMod == ImGuizmo::OPERATION::TRANSLATE ? -1 : ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
	
		case Key::R:
		{
			m_gizmoMod = m_gizmoMod == ImGuizmo::OPERATION::ROTATE? -1 : ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case Key::Tab:
		{
			m_gizmoMod = m_gizmoMod == ImGuizmo::OPERATION::BOUNDS? -1 : ImGuizmo::OPERATION::BOUNDS;
			break;
		}


		}

		return false;
	}

	bool EditorLayer::OnMousebuttomPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButtonCode() == Mouse::ButtonLeft)
			if(m_viewPortIsHover && !ImGuizmo::IsOver())
				m_sceneHierarchyPanel.SetSelectedEntity(m_hoveredEntity);
		return false;
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string path = PlantformUtils::SaveFile("场景文件(*.hazel)\0*.hazel\0"); //Showtext\0filter\0
		if (!path.empty())
		{
			SerializeScene(m_editScene,path);
			m_editorScenePath = path;
		}

	}

	void EditorLayer::SaveScene()
	{
		if (!m_editorScenePath.empty())
		{
			std::string path = m_editorScenePath.string();
			SerializeScene(m_editScene, path);
		}
		else
		{
			SaveSceneAs();
		}
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& filePath)
	{
		SceneSerializer sceneSerializer(scene);
		sceneSerializer.Serialize(filePath.string());
	}


	void EditorLayer::OpenScene()
	{
		std::string path = PlantformUtils::OpenFile("场景文件(*.hazel)\0*.hazel\0");
		if (!path.empty())
		{
			OpenScene(path);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_sceneState != SceneState::Edit)
			OnSceneStop();
		if (path.extension() != ".hazel")
		{
			HZ_CORE_WARN("Could not load {0} - not a scene file(.hazel)", path.string());
			return;
		}

		Ref<Scene> newScene = CreateRef<Scene>();
		newScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		//should Resize before Serializer
		SceneSerializer sceneSerializer(newScene);
		if (sceneSerializer.DeSerialize(path.string()))
		{
			m_editScene = newScene;
			m_activeScene = m_editScene;
			m_sceneHierarchyPanel.SetContext(m_editScene);

			m_editorScenePath = path;

		};


	}

	void EditorLayer::NewScene()
	{
		if (m_sceneState != SceneState::Edit)
			OnSceneStop();
		m_editScene = CreateRef<Scene>();
		m_editScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_activeScene = m_editScene;
		m_sceneHierarchyPanel.SetContext(m_activeScene);
		
		m_editorScenePath = std::filesystem::path(); //empty
	

		//m_activeScene = CreateRef<Scene>();
		//m_activeScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		//m_sceneHierarchyPanel.SetContext(m_activeScene);
	}

	void EditorLayer::OnScenePlay()
	{
		m_sceneState = SceneState::Play;
		m_activeScene =Scene::Copy(m_editScene);
		m_sceneHierarchyPanel.SetContext(m_activeScene);

		m_activeScene->OnRuntimeStart();

	}

	void EditorLayer::OnSceneStop()
	{
		m_sceneState = SceneState::Edit;
		m_activeScene->OnRuntimeStop();
		//Ref<>是共享指针,再点击play引用到新的复制eidtscene时,旧的activescene中断引用指向的scene会自动被回收的.

		m_activeScene = m_editScene;
		m_sceneHierarchyPanel.SetContext(m_editScene);


	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (m_sceneState == SceneState::Play)
			return;

		Entity BDEntity = m_sceneHierarchyPanel.GetSelectedEntity();
		if (BDEntity)
		{
			m_editScene->DuplicateEntity(BDEntity);
			return;
		}
		HZ_CORE_INFO("Please choose a Entity before Duplicate");
		return;
	}


}


