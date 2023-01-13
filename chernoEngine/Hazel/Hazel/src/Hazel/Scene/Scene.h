#pragma once
#include "entt.hpp"

#include "Hazel/Core/Timestep.h"
#include "Hazel/Core/UUID.h"
#include "Hazel/Renderer/EditorCamera.h"

class b2World;

namespace Hazel //100% 包装 不会让用户接触到除了hazel之外的api
{
	class Entity;// 前向声明 解决头文件锁(在定义之前无法调用其成员)

	class Scene //scene 是 rigister的包装器, entity 是 conpoment的包装器
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		/**
		* @含有默认的transformComponent与Tag, brief Creates a new entity or recycles a destroyed one.
		* @return Entity.
		*/
		Entity CreateEntity(const std::string& tag = std::string("Entity"));
		Entity CreateEntityWithUUID(UUID uuid, const std::string& tag = std::string("Entity"));
		Entity GetPrimaryCamera();
		
		void OnViewportResize(const uint32_t & width, const uint32_t& height) ;

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts,EditorCamera& camera);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void DestroyEntity(Entity entity);

		void DuplicateEntity(Entity entity); //复制实体

	private:
		template <typename T>
		void OnComponentAdded(Entity entity, T& component);


	private:

		entt::registry m_registry; //实体注册表 我们entity的容器,我们对entity做的一切操作都要经过他 https://github.com/skypjack/entt
		Entity* m_mainCamera;
		uint32_t m_viewportWidth =0 ;
		uint32_t m_viewportHeight =0 ;

		b2World* m_physicsWorld =nullptr;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel; //that means SceneHierarchyPanel can use scene's private numbers

	};


}

