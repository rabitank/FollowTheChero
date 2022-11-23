#include "SceneHierarchyPanel.h"
#include "Hazel/Scene/Component.h"
#include <glm/gtc/type_ptr.hpp>


#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h> //imgui internal function? -> for PushMultiItemsWidths
namespace Hazel
{



	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{

		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context	)
	{
		m_context = context;
		m_selectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		//parent relation?
		ImGui::Begin("SceneHierarchy");

		m_context->m_registry.each([&](auto entityID) {
			Entity entity{ entityID,m_context.get()};
			DrawEntityNode(entity);

			
		});



		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) //if we click the empty part on the panel,means we don't choose any entity
			m_selectionContext = {};

		//if right-click
		if (ImGui::BeginPopupContextWindow(0,1,false)) //弹出上下文窗口 (右键菜单)
		{
			if (ImGui::MenuItem("Create Empty Entity"))//创建菜单元素并
				m_context->CreateEntity("Empty entity");
			ImGui::EndPopup();	


		}

		ImGui::End();

		ImGui::Begin("Proporities");
		{
			if(m_selectionContext)
			{
				DrawComponents(m_selectionContext);
			}
		}

		ImGui::End();
	}



	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{

		auto& tc = entity.GetComponent<TagComponent>().Tag;
		// flag is the setting flags of this node status in tree;
		ImGuiTreeNodeFlags flags=  (m_selectionContext == entity ?ImGuiTreeNodeFlags_Selected : 0)| ImGuiTreeNodeFlags_OpenOnArrow;//选中状态显示 点击箭头打开
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth; //make the tree node can be choose by the whole line
		//利用entityID 生成对应指针,使其为该树根节点的标识符(prt_ID) -> 我们可以跟踪entity 通过点击节点
		//返回 是否点击箭头的指针
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity,flags,tc.c_str());

		if (ImGui::IsItemClicked())
		{
			m_selectionContext = entity;
		}

		bool deleteEntity = false;
		if(ImGui::BeginPopupContextItem())
		{ 
			if (ImGui::MenuItem("Delete  Entity"))
				deleteEntity = true; //delete entity later

			ImGui::EndPopup();

		}


		if (opened) //
		{
			ImGuiTreeNodeFlags flags = (m_selectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)((uint32_t)entity+1000), flags, tc.c_str());
			if (opened)
			{
				ImGui::TreePop();//弹出子树

			}
			ImGui::TreePop();//弹出子树
		}

		if (deleteEntity) 
		{
			m_context->DestroyEntity(entity);
			if (m_selectionContext == entity)//for safe ; if the entity is being showed on ComponentWinodw,we need to stop show it
				m_selectionContext = {};
		}

	}


	static void DrawVec3Control(const std::string& label, glm::vec3& vec3, float resetValue =0.f,float speed =0.1f,float columnWidth =100.f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0]; //你可以搞个字体库,让他可读一点

		ImGui::PushID(label.c_str()); //create an new IDnamespace/IDscope



		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));//推送格式设置 item间距00

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y*2.f;// 行高计算 GImGui :global pointer imguicontext*
		ImVec2 buttonSize = { lineHeight + 3.f,lineHeight };

		//do you see how the popStyle work?
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.6f,0.1f,0.15f,1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f,0.2f,0.2f,1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f,0.1f,0.15f,1.f });


		ImGui::PushFont(boldFont); //粗体设置
		if (ImGui::Button("X", buttonSize))
			vec3.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &vec3.x,speed,0.f,0.f,"%.2f"); //##X is they identification code. that mians who has the same identification code in same IDspace has the same behavior
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.5f,0.15f,1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.8f,0.2f,1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.75f,0.15f,1.f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			vec3.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &vec3.y,speed,0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.2f,0.5f,1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f,0.35f,0.9f,1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f,0.3f,0.8f,1.f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			vec3.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &vec3.z, speed, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopID(); //pop now IDscope
		ImGui::PopStyleVar();

		ImGui::Columns(1);

	}

	//we will pass the function that actuall we need call to draw TheComponent into this function 
	//a bit like CallBack()
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uifunction)
	{


		if (entity.HasComponent<T>())
		{


			ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen
				| ImGuiTreeNodeFlags_AllowItemOverlap
				| ImGuiTreeNodeFlags_Framed
				| ImGuiTreeNodeFlags_SpanAvailWidth
				| ImGuiTreeNodeFlags_FramePadding;

			auto& comp = entity.GetComponent<T>();

			//button + style
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4,4 });

			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();//GetContentRegionAvail() is get the content size of current render position 

			//if removeComponent
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f); //level space for scrollbar


			if (ImGui::Button("+", { lineHeight,lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");

			}


			bool removeComponent = false;

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			//Draw

			if (open)
			{
				uifunction(comp);
				ImGui::TreePop();
			}

			//remove
			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{



		if (entity.HasComponent<TagComponent>())
		{
			auto& comp = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));//memorySet
			strcpy_s(buffer, sizeof(buffer), comp.c_str());//str copy
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				comp = std::string(buffer);
			}

		}
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camer"))
			{
				m_selectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("SpriteRender"))
			{
				m_selectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();

		}
		ImGui::PopItemWidth();



		
		DrawComponent<TransformComponent>("Transform", entity, [](auto& comp)
			{
				DrawVec3Control("Position:", comp.Translation, 0.f);
				glm::vec3 rotation = glm::degrees(comp.Rotation);
				DrawVec3Control("Rotation:", rotation, 0.f, 1.f);
				comp.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale:", comp.Scale, 1.f);

			});

		DrawComponent<CameraComponent>("CameraComponent", entity, [](auto& comp)
			{
				auto& compCamera = comp.Camera;

				const char* projectionTypeStrings[] = { "Perspective","Othrographic" };
				const char* currentProjectionType = projectionTypeStrings[(int)compCamera.GetProjectionType()];

				ImGui::Checkbox("Main(Primary)Camera", &comp.Primary);

				if (ImGui::BeginCombo("Projection", currentProjectionType))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionType == projectionTypeStrings[i]; //获得是否选中状态

						if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) //绘制可选项同时检测选择点击
						{
							currentProjectionType = projectionTypeStrings[i]; //响应: 设置相机类型
							compCamera.SetProjectionType(SceneCamera::ProjectionType(i));
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus(); //设为复选框默认选中的东西并显示
					}
					ImGui::EndCombo();
				};


				if (compCamera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					auto Size = compCamera.GetOrthographicSize();
					auto zNear = compCamera.GetOthrographicNearClip();
					auto zFar = compCamera.GetOthrographicFarClip();

					if (ImGui::DragFloat("OrthoSize:", &Size))
						compCamera.SetOrthographicSize(Size);
					if (ImGui::DragFloat("OrthoClipFar:", &zFar))
						compCamera.SetOthroFarClip(zFar);
					if (ImGui::DragFloat("OrthoNearClip:", &zNear))
						compCamera.SetOrthoNearClip(zNear);

				}

				if (compCamera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{

					auto verticalFov = glm::degrees(compCamera.GetPerspectFOV());
					auto zFar = compCamera.GetPerspectiveFarClip();
					auto zNear = compCamera.GetPerspectiveNearClip();

					if (ImGui::DragFloat("PerspectSize:", &verticalFov))
						compCamera.SetPerspectFOV(glm::radians(verticalFov));
					if (ImGui::DragFloat("PerspectClipFar:", &zFar))
						compCamera.SetPerspectFarClip(zFar);
					if (ImGui::DragFloat("PerspectNearClip:", &zNear))
						compCamera.SetPerspectNearClip(zNear);
				}
				ImGui::Checkbox("FixedAspectRatio:", &comp.FixedAspectRatio);
			});

		DrawComponent<SpriteRendererComponent>("Sprite Render",entity,[](auto& compnent) 
		{
			ImGui::ColorEdit4("color:", glm::value_ptr(compnent.Color));
		});




#if 0
		if (selectedE.HasComponent<CameraComponent>())
		{

			//if removeComponent
			bool open = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "CameraComponent");

			ImGui::SameLine();

			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("ComponentSettings");

			}

			bool removeComponent = false;

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;
				ImGui::EndPopup();
			}


			if (open)
			{

				auto& comp = selectedE.GetComponent<CameraComponent>();
				auto& compCamera = comp.Camera;

				const char* projectionTypeStrings[] = {"Perspective","Othrographic" };
				const char* currentProjectionType = projectionTypeStrings[(int)compCamera.GetProjectionType()];

				ImGui::Checkbox("Main(Primary)Camera", &comp.Primary);

				if (ImGui::BeginCombo("Projection",currentProjectionType))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionType == projectionTypeStrings[i]; //获得是否选中状态
						
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) //绘制可选项同时检测选择点击
						{
							currentProjectionType = projectionTypeStrings[i]; //响应: 设置相机类型
							compCamera.SetProjectionType( SceneCamera::ProjectionType(i) );
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus(); //设为复选框默认选中的东西并显示
					}
					ImGui::EndCombo();
				};


				if (compCamera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					auto Size= compCamera.GetOrthographicSize();
					auto zNear = compCamera.GetOthrographicNearClip();
					auto zFar = compCamera.GetOthrographicFarClip();

					if(ImGui::DragFloat("OrthoSize:", &Size))
						compCamera.SetOrthographicSize(Size);
					if(ImGui::DragFloat("OrthoClipFar:", &zFar))
						compCamera.SetOthroFarClip(zFar);
					if(ImGui::DragFloat("OrthoNearClip:", &zNear))
						compCamera.SetOrthoNearClip(zNear);

				}

				if (compCamera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{

					auto verticalFov = glm::degrees(compCamera.GetPerspectFOV());
					auto zFar = compCamera.GetPerspectiveFarClip();
					auto zNear = compCamera.GetPerspectiveNearClip();

					if (ImGui::DragFloat("PerspectSize:", &verticalFov))
						compCamera.SetPerspectFOV(glm::radians(verticalFov));
					if (ImGui::DragFloat("PerspectClipFar:", &zFar))
						compCamera.SetPerspectFarClip(zFar);
					if (ImGui::DragFloat("PerspectNearClip:", &zNear))
						compCamera.SetPerspectNearClip(zNear);
				}
				ImGui::Checkbox("FixedAspectRatio:", &comp.FixedAspectRatio);

				ImGui::TreePop(); //必须要这条 才会显示tree

				//remove
				if (removeComponent)
					selectedE.RemoveComponent<CameraComponent>();
			}
		}

#endif
		//TODO : we should auto draw anyComponent, but...not now
		//if (selectedE.HasComponent<Component>())
		//{
		//}
	}

}