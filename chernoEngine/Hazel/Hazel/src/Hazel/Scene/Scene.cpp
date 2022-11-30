#include "hzpch.h"
#include "Scene.h"

#include "Entity.h"
#include "Component.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "glm/glm.hpp"

namespace Hazel
{
	//包装啊.......... entt真的很完美了

	Scene::Scene()
	{


	}


	Scene::~Scene()
	{
		//delete m_mainCamera;
	}


	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{


		Renderer2D::BeginScene(camera);

		auto group = m_registry.group<TransformComponent, SpriteRendererComponent>();

		for (auto entity : group) //按大纲从上到下绘制
		{
			auto& [transC, spriteC] = m_registry.get<TransformComponent, SpriteRendererComponent>(entity);
			//Renderer2D::DrawQuad((const glm::mat4&)transC, (const glm::vec4&)spriteC,20);
			//似乎DrawQuad设置entityID 暂时不能传递到colorAttach1
			Renderer2D::DrawSprite((const glm::mat4&)transC, spriteC, (int)entity);
		}

		Renderer2D::EndScene();

	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		//Update Script ... you should bind an specific NativeScriptcomponent before it enter Scene's Update
		{
			m_registry.view<NativeScripComponent>().each([=](auto entity ,NativeScripComponent& nsc)  // auto entity ,&nsc : view 
			{
				if(!nsc.Instance)
				{ 
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_entity = { entity,this };
					nsc.Instance->OnCreate();	
				}

				
				nsc.Instance->OnUpdate(ts);	
			
			}); // nsc: native script component


		}


		//render
		Camera* main_Camera = nullptr;
		TransformComponent* main_Camrea_transform = nullptr;
		{

			auto view = m_registry.view<CameraComponent,TransformComponent>();
			for (auto entity : view)
			{
				auto[camera,trans] = view.get<CameraComponent, TransformComponent>(entity); //ok.. view.get返回引用元组,我们直接auto[] 就行.这种引用并不是因为auto 而是[]的结构化绑定
				if (camera.Primary)	
				{

					main_Camera = & camera.Camera;
					main_Camrea_transform = &trans;
					break;
				}
				
			}
		}

		if (main_Camera)
		{

			Renderer2D::BeginScene(main_Camera->GetProjection(),*main_Camrea_transform);

			auto group = m_registry.group<TransformComponent, SpriteRendererComponent>();

			for (auto entity : group)
			{
				auto& [transC, spriteC] = m_registry.get<TransformComponent, SpriteRendererComponent>(entity);
				HZ_CORE_TRACE("Renderer2D::DrawQuad ,entityID: {0}", (int)entity);
				Renderer2D::DrawQuad((const glm::mat4&)transC, (const glm::vec4&)spriteC, (int)entity);

			}

			Renderer2D::EndScene();
		}


	}

	Entity Scene::CreateEntity(const std::string& tag)
	{



		Entity e{ m_registry.create(),this };
		e.AddComponent<TransformComponent>();
		e.AddComponent<TagComponent>(tag);
		return e;
	}

	void Scene::OnViewportResize(const uint32_t& width, const uint32_t& height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;

		//fix our camera aspectratio
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);//但是对于单个组件的get方法..返回仍需要& 承接...自己去看变量类型啊!
			if (!camera.FixedAspectRatio) //camera size change with viewPort -> change ProjectionMatrix
			{
				camera.Camera.SetViewPortSize(width,height);
			}

		}

	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_registry.destroy(entity);

	}


	template <typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false); //静态断言 -> 无对应特化模板 ->中断
	}
	//模板特化
	template <>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	Entity Scene::GetPrimaryCamera()
	{
		auto view = m_registry.view<CameraComponent>();
		if(view.empty())
		{
			return {entt::null,this};
		}
		for (auto entity : view)
		{
			auto& camera= view.get<CameraComponent>(entity); //ok.. view.get返回引用元组,我们直接auto[] 就行.这种引用并不是因为auto 而是[]的结构化绑定
			if (camera.Primary)
			{
				return {entity,this};
			}

		}
		return { entt::null,this };
	}


	template <>
	void Hazel::Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
		
	}

	template <>
	void Hazel::Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewPortSize(m_viewportWidth, m_viewportHeight);
	}
	template <>
	void Hazel::Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
		
	}

	template <>
	void Hazel::Scene::OnComponentAdded<NativeScripComponent>(Entity entity, NativeScripComponent& component)
	{

	}



}


// example:
// 		//不懂自己看wiki
// 		transform trans;
// 		Stmath((const glm::mat4&)trans);
// 
// 		entt::entity entity =   m_registry.create(); //entity is std::uint32_t
// 		m_registry.emplace<transform>(entity, glm::mat4(1.f));//为entity配置组件
// 		m_registry.remove<transform>(entity);
// 
// 		m_registry.get<transform>(entity);
// 
// 		//请使用 any_of / all_of 代替 has() 来探查 实体是否具有集合中的所有组件或其中任何一个组件
// 		m_registry.any_of<transform>(entity);
// 		//对于实体没有的组件 get会出错 :auto vec2 = m_registry.get<glm::vec2>(entity);
// 
// 		auto view = m_registry.view<transform>(); //探查所有含有模板内组件的实体并返回 view: 一个entitys的可迭代对象,entity索引的该种组件表(get)
// 		for (entt::entity entit : view)
// 		{
// 			auto& trasn = m_registry.get<transform>(entit);
// 			auto& trasn = m_registry.get<TagComponent>(entit);

// 			auto& trasn = view.get<transform>(entit);
// 		}
// 
// 		//这个东西...有点问题现在
// 		//m_registry.on_construct<transform>().connect<&CallMePls>(); // 监听/响应transform组件创建事件 connect<&func>() 并链接回调函数	
// 
// 		auto group = m_registry.group<transform>(entt::get<MeshCom>); //group (非拥有组) 返回含有组件的完整实体 类似引用切片... 但是优化了..迭代它开销不大
// 		for (auto entit : group)
// 		{
// 			auto& [as, sb] = group.get<transform,MeshCom>(entit); //返回tuple ... 像返回几个返几个
// 		}
