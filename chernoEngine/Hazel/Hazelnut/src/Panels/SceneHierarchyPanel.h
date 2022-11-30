#pragma once
#include "Hazel/Core/Base.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Entity.h"

namespace Hazel
{

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(const Ref<Scene>& scene);
		SceneHierarchyPanel() =default;

		void SetContext(const Ref<Scene>& scene) ;
		void OnImGuiRender();
		Entity GetSelectedEntity() const{ return m_selectionContext; };
		void SetSelectedEntity(Entity& entity) { m_selectionContext = entity; }

	private: 

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private: 
		Ref<Scene> m_context;

		Entity m_selectionContext;

	
	};


}
