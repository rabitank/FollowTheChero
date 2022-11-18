#pragma once
#include "Scene.h"

namespace Hazel
{
	class SceneSerializer //take a scene and serializer it
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath); //serialize to yaml
		void SerializeRuntime(const std::string& filepath); //serialize to binary

		bool DeSerialize(const std::string& filepath); //serialize to yaml
		bool DeSerializeRuntime(const std::string& filepath); //serialize to binary

	private:
		Ref<Scene> m_scene;
			

	};

}

