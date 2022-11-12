#include "hzpch.h"
#include "Entity.h"

namespace Hazel
{


	Entity::Entity(const entt::entity& handle, Scene* scene)
		:m_entityHandle(handle),m_scene(scene)
	{
		
	}

	Entity::Entity(Scene* scene)
	{
		m_entityHandle= scene->m_registry.create();
	}

}

