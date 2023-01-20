#include "hzpch.h"
#include "SceneSerializer.h"
#include "Hazel/Core/Base.h"
#include <fstream>	
#include <yaml-cpp/yaml.h>

#include "Entity.h"
#include "Component.h"


namespace YAML
{
	template<> //in YAML: 这是convert模板的特化 -> as<>()
	struct convert<glm::vec2>
	{

		static Node encode(const glm::vec2& rhs) //data to node
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs) // node to data type
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<> //in YAML: 这是convert模板的特化 -> as<>()
	struct convert<glm::vec3>
	{

		static Node encode(const glm::vec3& rhs) //data to node
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) // node to data type
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};


	template<> //in YAML: 这是convert模板的特化 -> as<>()
	struct convert<glm::vec4>
	{

		static Node encode(const glm::vec4& rhs) //data to node
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) // node to data type
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}


namespace Hazel
{

	static std::string RigidBody2Dtype_2_String(const Rigidbody2DComponent::RigidBodyType& type )
	{
		switch (type)
		{
		case Rigidbody2DComponent::RigidBodyType::Static:
			return "Static";
		case Rigidbody2DComponent::RigidBodyType::Dynamic:
			return "Dynamic";
		case Rigidbody2DComponent::RigidBodyType::Kinematic:
			return "Kinematic";
		}
		HZ_CORE_ASSERT(false, "Unkonw RigidBodyType");
		return "Static";
	}

	static  Rigidbody2DComponent::RigidBodyType RigidBody2Dtype_from_String(const std::string& str)
	{
		if (str == "Static")
			return Rigidbody2DComponent::RigidBodyType::Static;

		if (str == "Dynamic")
			return Rigidbody2DComponent::RigidBodyType::Dynamic;

		if (str == "Kinematic")
			return Rigidbody2DComponent::RigidBodyType::Kinematic;
		HZ_CORE_ASSERT(false, "Unkonw rigidBodyType String");
		return Rigidbody2DComponent::RigidBodyType::Static;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec4)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec4.x << vec4.y << vec4.z << vec4.w << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec3)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec3.x << vec3.y << vec3.z << YAML::EndSeq;
		return out;
	}	
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& vec2)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec2.x << vec2.y  << YAML::EndSeq;
		return out;
	}


	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		HZ_CORE_ASSERT(entity.HasComponent<IDComponent>());

		out << YAML::BeginMap;

		out << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<IDComponent>().ID; //TODO:we want an entity has a globalunique ID	


		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;

			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;

		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent"; //empty value
			out << YAML::BeginMap;

			auto& comp = entity.GetComponent<TransformComponent>();

			out << YAML::Key << "Translation" << YAML::Value << comp.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << comp.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << comp.Scale;
			out << YAML::EndMap;

		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent"; //empty value
			out << YAML::BeginMap;

			auto& comp = entity.GetComponent<CameraComponent>();
			auto& camera = comp.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOthrographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOthrographicFarClip();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << comp.Primary;
			out << YAML::Key << "FixedAspecRatio" << YAML::Value << comp.FixedAspectRatio;

			out << YAML::EndMap;

		};		
		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "Rigidbody2DComponent"; //empty value
			out << YAML::BeginMap;

			auto& comp = entity.GetComponent<Rigidbody2DComponent>();

			out << YAML::Key << "BodyType" << YAML::Value << RigidBody2Dtype_2_String(comp.BodyType);
			out << YAML::Key << "FixedRotation" << YAML::Value << comp.FixedRotation;
			out << YAML::EndMap;
		};

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent"; //empty value
			out << YAML::BeginMap;

			auto& comp = entity.GetComponent<BoxCollider2DComponent>();

			out << YAML::Key << "Offset" << YAML::Value << comp.Offset;
			out << YAML::Key << "Size" << YAML::Value << comp.Size;
			out << YAML::Key << "Density" << YAML::Value << comp.Density;
			out << YAML::Key << "Friction" << YAML::Value << comp.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << comp.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << comp.RestitutionThreshold;

			out << YAML::EndMap;
		};

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& comp = entity.GetComponent<SpriteRendererComponent>();

			out << YAML::Key << "Color" << YAML::Value << comp.Color;
			out << YAML::EndMap;

		}	
		
		if (entity.HasComponent<CircleRendererComponent>())
		{
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap;

			auto& comp = entity.GetComponent<CircleRendererComponent>();

			out << YAML::Key << "Color" << YAML::Value << comp.Color;
			out << YAML::Key << "Thickness" << YAML::Value << comp.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << comp.Fade;
			out << YAML::EndMap;

		}
				
		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap;

			auto& comp = entity.GetComponent<CircleCollider2DComponent>();

			out << YAML::Key << "Offset" << YAML::Value << comp.Offset;
			out << YAML::Key << "Radius" << YAML::Value << comp.Radius;

			out << YAML::Key << "Density" << YAML::Value << comp.Density;
			out << YAML::Key << "Friction" << YAML::Value << comp.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << comp.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << comp.RestitutionThreshold;

			out << YAML::EndMap;

		}

		out << YAML::EndMap;



	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:m_scene(scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene";
		out << YAML::Value << "Untitled";
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq; //beginsequence

		m_scene->m_registry.each([&](auto entityid)
			{
				Entity entity{ entityid,m_scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);


			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		//not implemented

		HZ_CORE_ASSERT(false,"");

	}

	bool SceneSerializer::DeSerialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data= YAML::Load(strStream);
		if (!data["Scene"])
		{
			return false;
		}
		std::string sceneName = data["Scene"].as<std::string>();
		HZ_CORE_TRACE("Deserializer Scene {0}!", sceneName);

		YAML::Node entities = data["Entities"];
		if (entities)
		{
			for (auto& entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();
				std::string name = "entity";

				auto& tagcomponent = entity["TagComponent"];
				if(tagcomponent)
					name = tagcomponent["Tag"].as<std::string>();

				HZ_CORE_TRACE("Deserialized entity with ID = {0} ,name ={1}", uuid, name);

				Entity& deserializedEntity = m_scene->CreateEntityWithUUID(uuid,name);

				auto  transformCompnent = entity["TransformComponent"];
				if (transformCompnent)
				{

					//TODO : GetOrCreateComponent<>();
					auto& Transcomp = deserializedEntity.GetComponent<TransformComponent>();
					Transcomp.Translation = transformCompnent["Translation"].as<glm::vec3>();
					Transcomp.Rotation= transformCompnent["Rotation"].as<glm::vec3>();
					Transcomp.Scale= transformCompnent["Scale"].as<glm::vec3>();

				}

				auto  cameraCompnent = entity["CameraComponent"];
				if (cameraCompnent)
				{

					//TODO : GetOrCreateComponent<>();
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					auto& camera = cameraCompnent["Camera"];
					//这他妈几个Set要重算投影矩阵好几次啊
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)camera["ProjectionType"].as<int>());
					cc.Camera.SetPerspectFOV(camera["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectNearClip(camera["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectFarClip(camera["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(camera["OrthographicSize"].as<float>());
					cc.Camera.SetOrthoNearClip(camera["OrthographicNear"].as<float>());
					cc.Camera.SetOthroFarClip(camera["OrthographicFar"].as<float>());

					

					cc.Primary = cameraCompnent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraCompnent["FixedAspecRatio"].as<bool>();

				}

				auto  spriteCompnent = entity["SpriteRendererComponent"];
				if (spriteCompnent)
				{
					auto& sc = deserializedEntity.AddComponent<SpriteRendererComponent>();
					sc.Color = spriteCompnent["Color"].as<glm::vec4>();
				}
				
				auto  circleCompnent = entity["CircleRendererComponent"];
				if (circleCompnent)
				{
					auto& cc = deserializedEntity.AddComponent<CircleRendererComponent>();
					cc.Color = circleCompnent["Color"].as<glm::vec4>();
					cc.Thickness= circleCompnent["Thickness"].as<float>();
					cc.Fade = circleCompnent["Fade"].as<float>();
				}

				auto  rigidbody2DComponent = entity["Rigidbody2DComponent"];
				if (rigidbody2DComponent)
				{
					auto& rc = deserializedEntity.AddComponent<Rigidbody2DComponent>();
					rc.BodyType =  RigidBody2Dtype_from_String(rigidbody2DComponent["BodyType"].as<std::string>());
					rc.FixedRotation= rigidbody2DComponent["FixedRotation"].as<bool>();
				}
				auto  boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& bc = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc.Density= boxCollider2DComponent["Density"].as<float>();
					bc.Friction= boxCollider2DComponent["Friction"].as<float>();
					bc.Restitution= boxCollider2DComponent["Restitution"].as<float>();
					bc.RestitutionThreshold= boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}

				auto  circleCollider2DComponent = entity["CircleCollider2DComponent"];
				if (circleCollider2DComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					cc.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
					cc.Radius= circleCollider2DComponent["Radius"].as<float>();
					cc.Density = circleCollider2DComponent["Density"].as<float>();
					cc.Friction = circleCollider2DComponent["Friction"].as<float>();
					cc.Restitution = circleCollider2DComponent["Restitution"].as<float>();
					cc.RestitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();
				}

			}
		

		}
		return true;
	}

	bool SceneSerializer::DeSerializeRuntime(const std::string& filepath)
	{
		//not implemented

		HZ_CORE_ASSERT(false,"");
		return false;
	}

}
