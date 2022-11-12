#include "hzpch.h"
#include "Scene.h"
#include "Component.h"
#include "Entity.h"

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
		delete m_mainCamera;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		//render
		Camera* main_Camera = nullptr;
		glm::mat4* main_Camrea_transformMat = nullptr;
		{

			auto view = m_registry.view<CameraComponent,TransformComponent>();
			for (auto entity : view)
			{
				auto&[camera,trans] = view.get<CameraComponent, TransformComponent>(entity);
				if (camera.Primary)
				{

					m_mainCamera = new Entity{entity,this};
					main_Camera = &camera.camera;
					main_Camrea_transformMat = &trans.Transform;
					break;
				}
				
			}
		}

		if (main_Camera)
		{

			Renderer2D::BeginScene(main_Camera->GetProjection(),*main_Camrea_transformMat);

			auto group = m_registry.group<TransformComponent, SpriteRendererComponent>();

			for (auto entity : group)
			{
				auto& [transC, spriteC] = m_registry.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad((const glm::mat4&)transC, (const glm::vec4&)spriteC);
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
