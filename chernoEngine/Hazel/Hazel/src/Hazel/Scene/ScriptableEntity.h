#pragma once
#include "Entity.h"

namespace Hazel
{

	class  ScriptableEntity
	{
	public:
		ScriptableEntity() =default;
		virtual ~ScriptableEntity() =default;


		//不要忘记! 编译时需要明确定义,要么 =0,要么给定义.这里和cherno不一样..加了接口..确保不会没有这三个函数
		template<typename T>
		T& GetComponent() {

			HZ_CORE_ASSERT(m_entity.HasComponent<T>(), "Entity already has component");
			return m_entity.GetComponent<T>();
		}
		
	protected:
		virtual void OnCreate() {};
		virtual void OnUpdate(Timestep ts) {};
		virtual void OnDestroy() {};



	private:

		Entity m_entity;
		friend class Scene;


	};
}



