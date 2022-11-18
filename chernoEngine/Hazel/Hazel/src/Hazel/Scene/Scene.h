#pragma once
#include "entt.hpp"

#include "Hazel/Core/Timestep.h"


namespace Hazel //100% 包装 不会让用户接触到除了hazel之外的api
{
	class Entity;// 前向声明 解决头文件锁(在定义之前无法调用其成员)

	class Scene //scene 是 rigister的包装器, entity 是 conpoment的包装器
	{
	public:
		Scene();
		~Scene();


		/**
		* @含有默认的transformComponent与Tag, brief Creates a new entity or recycles a destroyed one.
		* @return Entity.
		*/
		Entity CreateEntity(const std::string& tag = std::string("Entity"));
		inline Entity* GetMainCamera(){ return m_mainCamera; };
		
		void OnViewportResize(const uint32_t & width, const uint32_t& height) ;

		void OnUpdate(Timestep ts);

		void DestroyEntity(Entity entity);

	private:
		template <typename T>
		void OnComponentAdded(Entity entity, T& component);


	private:

		entt::registry m_registry; //实体注册表 我们entity的容器,我们对entity做的一切操作都要经过他 https://github.com/skypjack/entt
		Entity* m_mainCamera;
		uint32_t m_viewportWidth =0 ;
		uint32_t m_viewportHeight =0 ;


		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel; //that means SceneHierarchyPanel can use scene's private numbers

	};


}

