#pragma once

#include "Hazel/Renderer/Texture.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Core/UUID.h"

#include "SceneCamera.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp" // for qurt and toMat  需要启用glm实验功能

namespace Hazel
{


	struct Component  //store all information we need to create an specific entity
	{

	};

	// 使用UUID来作为entity句柄
	struct IDComponent:Component
	{
		UUID ID;
		IDComponent() = default;
		IDComponent(UUID id) { ID = id; };
		IDComponent(const IDComponent& other) = default;
	};

	struct TagComponent :Component
	{
		std::string Tag;
		TagComponent() = default;
		TagComponent(const TagComponent& another) = default;
		TagComponent(const std::string& tag)
			:Tag(tag) {};

	};

	//@use: the Transform[3] is its Position
	struct TransformComponent:Component //use degree, like Blender!!
	{

		glm::vec3 Translation = glm::vec3(0.f);
		glm::vec3 Rotation= glm::vec3(0.f);
		glm::vec3 Scale = glm::vec3(1.f);

	

		TransformComponent() = default;
		TransformComponent(const TransformComponent& trans) = default;
		TransformComponent(const glm::vec3& translation)
			:Translation(translation) {};
		glm::mat4 GetTransform() const
		{
			glm::mat4 rotationMat = glm::toMat4(glm::quat(Rotation));
			glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), Scale);
			return glm::translate(glm::mat4(1.f), Translation) * rotationMat * scaleMat;

		}

		operator glm::mat4 () { return GetTransform(); }   //他面对 const &的传参似乎分不清用const &还是 &的重载....
		operator const glm::mat4 () const{ return GetTransform(); } //重载了转换函数.之后就能直接转换了...无论显式隐式..估计是 重载了()?? 
	
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{1.f,1.f,1.f,1.f};
		Ref<Texture2D> Texture;
		float TilingFactor = 1.f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& another) = default;
		SpriteRendererComponent(const glm::vec4& o_color)
			:Color(o_color) {};

		operator glm::vec4& () { return Color; }   
		operator const glm::vec4& () const { return Color; } 
	};

	struct CircleRendererComponent
	{
		glm::vec4 Color{ 1.f,1.f,1.f,1.f };
		Ref<Texture2D> Texture;
		//Use transform to control a circlr 
		//float Radius = 0.5f; // for a width 1m circle 
		float Thickness =1.0f;
		float Fade = 0.002f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent& another) = default;
		CircleRendererComponent(const glm::vec4& o_color,float thickness,float fade)
			:Color(o_color),Thickness(thickness),Fade(fade)
			{};

		operator glm::vec4& () { return Color; }
		operator const glm::vec4& () const { return Color; }
	};

	struct MeshComponent
	{

	};



	struct CameraComponent
	{


		SceneCamera Camera;
		bool Primary = false;//当前活动摄像机/主摄像机;
		bool FixedAspectRatio = false;//是否需要修复横纵比(不随视口变化而变化)



		CameraComponent() = default;
		CameraComponent(const CameraComponent& another) = default;
		

	};
	class ScriptableEntity; // 前向声明
	struct NativeScripComponent //interface/slot of ScrptableEntity Component
	{
	public:
		
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();//函数指针 , 比std::function更简洁?
		void (*DestroyScript)(NativeScripComponent*); //模拟 成员函数?
		template<typename T>
		void Bind() 
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); }; //初始化 Instance
			DestroyScript= [](NativeScripComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };//& 引用捕获

			//转发: 
			//instance 没用成员&Instance , 而是选择再调用时手动输入 .Instance
		}

	};

	//physics component;
	struct Rigidbody2DComponent
	{
		enum class RigidBodyType
		{
			Static=0, Dynamic,Kinematic
			//Kinematic 动力学(符合动力学的->关节/骨骼一类) 
			//IK, -> b2Body 里没有IK类型		
		};

		RigidBodyType BodyType = RigidBodyType::Static;
		bool FixedRotation = false; // no rotation

		//Storage for runtime
		void* RuntimeBody = nullptr; //trace a RigidBody in Runtime and delete after stop

		Rigidbody2DComponent() = default;	
		Rigidbody2DComponent(const Rigidbody2DComponent& other) = default; //copy srtuct
		

	};
	struct BoxCollider2DComponent:Component
	{
		//shape?
		glm::vec2 Offset = { 0.f,0.f };
		glm::vec2 Size= { 0.5f,0.5f };

		//it should belong to physics material
		float Density = 1.f; //密度
		float Friction = 0.5f;//摩擦系数
		float Restitution = 0.0f; //恢复系数(弹性)
		float RestitutionThreshold = 0.1f; //恢复阈值(柔性)



		//Storage for runtime
		void* RuntimeFixture= nullptr; //trace a RigidBody in Runtime and delete after stop

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent& other) = default; //copy srtuct


	};

}


