#include "Hazel.h"


//-------------entry point	-------------
#include "Hazel/Core/EntryPoint.h"
//---------------------------------------

//app 对引擎启动细节不感兴趣, 引擎入口(main)应该由引擎定义, app只需要通过实现来控制
//没有正确的程序入口点 是 dll没有更新,lib提供的函数入口与旧dll不对应引起的

#include "imGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SandBox2D.h"


class ExampleLayer :public Hazel::Layer //layer也许需要持有 delta,或者application传参delta?, layer同时还有GUi的更新部分..双面啊..
{
public:
	ExampleLayer()
		:Layer("Example"), m_cameraController(1280.f/720.f,true)
	{
		m_VertexArray = (Hazel::VertexArray::Create());// they are the same type , so just equal!
		m_VertexArray->Bind();

		float vertices[7 * 3] =
		{
			-0.5f,0.5f,0.0f,0.7f,0.8f,0.1f,1.0f, //虽然提前写了顶点色但是不影响我们只读位置
			0.5f,0.5f,0.0f, 0.7f,0.3f,0.1f,1.0f,
			0.0f,-0.5f,0.0f,0.7f,0.3f,0.1f,1.0f,
		};

		Hazel::Ref<Hazel::VertexBuffer> TrangleVertexBuffer;
		TrangleVertexBuffer = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));


		Hazel::BufferLayout Layout = //initializer_list 能满足 元素调用构造函数的列表,特性,特性
		{
			{Hazel::ShaderDataType::Float3 ,"a_Position"},//并不需要同名...,名字只是增强可读
			{Hazel::ShaderDataType::Float4 ,"a_Color"}
		};

		TrangleVertexBuffer->SetLayout(Layout); //又拿到AddVertexBuffer了	// 注意,这里我将绑定布局指针放到了SetLayout里,和cherno有区别
		m_VertexArray->AddVertexBuffer(TrangleVertexBuffer);//所以,在AddVB里应用布局,需要VB含有布局不然报错
		TrangleVertexBuffer->Bind();



		uint32_t indices[3] = { 0,1,2 };

		Hazel::Ref<Hazel::IndexBuffer> TrangleIndexBuffer;
		TrangleIndexBuffer = Hazel::IndexBuffer::Create(indices, 3); //3是因为Cherno的强迫症 T_T
		m_VertexArray->SetIndexBuffer(TrangleIndexBuffer);	//很多地方加了Bind啊....可能会经常重复bind
		TrangleIndexBuffer->Bind();


		m_flatVA =(Hazel::VertexArray::Create());
		float SquraVertics[4 * 5] =
		{
			-0.5f,-0.5f,0.0f,0.0f,0.0f,
			0.5f,-0.5f,0.0f,1.0f,0.0f,
			0.5f,0.5f,0.0f,1.0f,1.0f,
			-0.5f,0.5f,0.0f,0.0f,1.0f

		};
		uint32_t SquareIndics[6] = { 0,1,2,2,3,0 };

		Hazel::Ref<Hazel::VertexBuffer> SquraVB = Hazel::Ref<Hazel::VertexBuffer>(Hazel::VertexBuffer::Create(SquraVertics, sizeof(SquraVertics)));
		Hazel::BufferLayout SquareLayout = //initializer_list 能满足 元素调用构造函数的列表,特性,特性
		{
			{Hazel::ShaderDataType::Float3 ,"a_Position"},
			{Hazel::ShaderDataType::Float2 ,"a_UVCoord"}
		};
		SquraVB->SetLayout(SquareLayout);
		m_flatVA->AddVertexBuffer(SquraVB);
		Hazel::Ref<Hazel::IndexBuffer> SquraIB = Hazel::Ref<Hazel::IndexBuffer>(Hazel::IndexBuffer::Create(SquareIndics, sizeof(SquareIndics) / sizeof(uint32_t)));
		m_flatVA->SetIndexBuffer(SquraIB);


		//the shader which just use filename , their name from filename

		m_shader=m_shaderLib.Load("Assets/shader/base.glsl"); //小技巧 std::string str = R"(可以任意换行,咋写都行 )"
		m_flatShader= m_shaderLib.Load("Assets/shader/flat.glsl"); //小技巧 std::string str = R"(可以任意换行,咋写都行 )"
		m_textureShader=m_shaderLib.Load("Assets/shader/texture.glsl"); //小技巧 std::string str = R"(可以任意换行,咋写都行 )"


		m_squareTexture = Hazel::Texture2D::Create("Assets/Texture/quancifang.png");
		m_chernoLogo = Hazel::Texture2D::Create("Assets/Texture/theChernoLogo.png");

		m_shaderLib.Get("texture")->Bind();
		m_shaderLib.Get("texture")->SetInt("u_Texture", 0);//use 0.num slot

		m_flatShader->Bind();
		m_flatShader->SetFloat3("u_color", m_flatColor);


	}
	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Grid Control");
		ImGui::ColorEdit3("Grid Color", glm::value_ptr(m_flatColor));
		ImGui::End();
	}

	void OnUpdate(Hazel::Timestep deltaime) override
	{


		Hazel::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Hazel::RenderCommand::Clear(); //↓	


		m_cameraController.OnUpdate(deltaime);
		Hazel::Renderer::BeginScene(m_cameraController.GetCamera());//开始调度,设置场景参数 //提交场景的相机 ,注意 是"提交" , vp矩阵不是引用
		//2D type
		
		glm::mat4 scale = glm::scale(glm::mat4(1.0f),glm::vec3(0.1f));

		//Hazel::Materialref* material = new Hazel::Material(Shader);
		//Hazel::MaterialInstanceRef* mi = new Hazel::MaterialInstance(Shader);
		//
		//mi->set("uniform",data);
		//squareMesh->Setmaterial(mi);//以及渲染序列与批渲染
		//构建数据存储区


		for (int j = 0; j < 20; j++)
		{
			for (int i = 0; i < 20; i ++)
			{
				glm::vec3 pos = glm::vec3(i * 0.11f,j*0.11f,0.0f);
				glm::mat4 tra = glm::translate(glm::mat4(1.0f), pos)*scale;
		
				Hazel::Renderer::Submit(m_flatShader, m_flatVA,tra);//提交shader,material,mesh,作为渲染内容
			}
		}
		m_chernoLogo->Bind();
		Hazel::Renderer::Submit(m_textureShader, m_flatVA, glm::scale( glm::mat4(1.0f), glm::vec3(1.5f)));//提交shader,material,mesh,作为渲染内容

		m_squareTexture->Bind();
		Hazel::Renderer::Submit(m_textureShader, m_flatVA, glm::scale( glm::mat4(1.0f), glm::vec3(1.5f)));//提交shader,material,mesh,作为渲染内容

		//Hazel::Renderer::Submit(m_flatShader, m_VertexArray);
	
		Hazel::Renderer::EndScene();//一些清理工作


	}

	void OnEvent(Hazel::Event& e) override //调度的解决判断在application 的Onevent ,不用管
	{

		m_cameraController.OnEvent(e);

		if (e.GetEventType() == Hazel::EventType::WindowResize) //  窗口自适应
		{
			auto& wre =  (Hazel::WindowsResizeEvent&)e;

			//float zoom = width / 1280.f;
			//m_cameraController.SetZoomLevel(zoom);

		}
	}

private:

	Hazel::Ref<Hazel::VertexArray> m_VertexArray;  // VB,IB现在由 VA持有
	Hazel::Ref<Hazel::VertexArray> m_flatVA; //for test

	Hazel::Ref<Hazel::Shader> m_shader; //换shared_ptr了...大逃杀模式...
	Hazel::Ref<Hazel::Shader> m_flatShader,m_textureShader;
	Hazel::ShaderLibrary m_shaderLib; //MOVE : Renderer


	Hazel::Ref<Hazel::Texture2D> m_squareTexture,m_chernoLogo;
	
	
	Hazel::OrthographicCameraController m_cameraController; //cherno把position提出来了?? 


	glm::vec3 m_flatColor = { 0.2,0.3f,0.8f };

};




class SandBox:public Hazel::Application
{
public:
	SandBox()
	{
	//	PushOverLayer(new Hazel::ImGuiLayer()); //重复推了ImGUILayer!!!尼玛!!!
		//PushLayer(new ExampleLayer());// ExampleLayer* -->Layer* 
		PushLayer(new SandBox2D());// ExampleLayer* -->Layer* 
	
	}
	~SandBox()
	{

	}

	
};

// a box about how the game work would be send/trans to engine's entry point

Hazel::Application* Hazel::CreateApplication()
{
	return new SandBox;	

}