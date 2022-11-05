#include "SandBox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <chrono>


static const uint32_t s_mapWidth =31 ;
static const char* s_mapTiles = 
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWSSSSWWWWWWWWWWWWW"
"WWWWWWWWWWSSSSDDDDDSRRRRWWWWWWW"
"WWWWWWWSSDDDDDDDDDDDDDDRRWWWWWW"
"WWWWWWSSSDDDDDDDDDDDDDDDRRWWWWW"
"WWWWSSSSSDDDDDDTDDDDDDDWWWWWWWW"
"WWSSSWWSSDDDDDWWWDDDDRWWWWWWWWW"
"WWWWWWSSDDTDDDWWWDDDDRRWWWWWWWW"
"WWWWSSDDDDDDDDDDDDSSSRRWWWWWWWW"
"WWWWWWWSSDDDDDDDTDDCWWWWWWWWWWW"
"WWWWWWWSSDDDDDDDDDDDDDWWWWWWWWW"
"WWWWWWWSSSSSSSSSSSSSSSWWWWWWWWW"
"WWWWWWWWWWWWSSSSSSRWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWRSSRRRWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
;


SandBox2D::SandBox2D()
	:Layer("SandBox2D"), m_cameraController(1280.f / 720.f, true)
{}

void SandBox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_boardTexture = Hazel::Texture2D::Create("Assets/Texture/99.png");
	m_quancifangTexture= Hazel::Texture2D::Create("Assets/Texture/quancifang.png");
	m_spriteSheet= Hazel::Texture2D::Create("Assets/Texture/game/RPGpack_sheet_2X.png");
	m_spriteHair = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 7,3 }, { 128,128 });
	m_spriteTree = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 2,1 }, { 128,128 }, {1,2});
	//m_boardTexture->Bind();

	//set particle 
	m_particle.LifeTiem = 1.0f;
	m_particle.colorBegin = { 1.0f,0.9f,0.5f,1.0f };
	m_particle.colorEnd = { 1.0f,0.4f,0.2f,1.0f };
	m_particle.SizeBeging = 0.5f, m_particle.SizeVariation = 0.3f, m_particle.SizeEnd = 0.f;
	m_particle.Velocity = { 0.f,0.f };
	m_particle.VelocityVariation = { 3.0f,1.f };
	m_particle.Position = { 0.0f,0.f };

	m_cameraController.SetZoomLevel(5.f);

	//Init the maptexture
	m_mapHeight = (uint32_t)strlen(s_mapTiles)/s_mapWidth;
	m_mapWidth= s_mapWidth;

	m_TextureMap['W'] = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 11,11 }, { 128,128 });
	m_TextureMap['S'] = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 6,11 }, { 128,128 });
	m_TextureMap['D'] = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 1,11 }, { 128,128 });
	m_TextureMap['T'] = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 2,1 }, { 128,128 }, {1,2});
	m_TextureMap['R'] = Hazel::SubTexture2D::CreateFromCroods(m_spriteSheet, { 5,9 }, { 128,128 });
}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	auto stats = Hazel::Renderer2D::GetStats();

	ImGui::Begin("2DLayer Grid Control");
	
	ImGui::Text("Renderer2D test - stats:");
	ImGui::Text("DrawCall:%d",stats.DrawCalls);
	ImGui::Text("quadCount: %d",stats.quadCount);
	ImGui::Text("Vertex:%d",stats.GetTotalVertexCount());
	ImGui::Text("Indeics:%d",stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Grid Color", glm::value_ptr(m_flatColor));

	for(auto& result: m_result)
	{	


		char label[50];
		std::strcpy(label, "%.3fms ");
		std::strcat(label, result.Name); //cat 追加到结尾  "Name &.3fms"
		ImGui::Text(label,result.Time);
	
	}
	m_result.clear();

	ImGui::End();
}

void SandBox2D::OnUpdate(Hazel::Timestep deltaime)
{


	HZ_PROFILE_FUNCTION();
	
	{
		HZ_PROFILE_SCOPE("CameraController::OnUpdate");
		m_cameraController.OnUpdate(deltaime);
	}

	Hazel::Renderer2D::ResetStats();
	
	{
		HZ_PROFILE_SCOPE("Renderer Prepare ");
	Hazel::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	Hazel::RenderCommand::Clear(); //↓	//why i can't see the layer before ? you clear it
	}

	static float rotation = 0.f;

	{

		//rotation += deltaime;

		HZ_PROFILE_SCOPE("Render Draw");

		Hazel::Renderer2D::BeginScene(m_cameraController.GetCamera());//开始调度,设置场景参数 //提交场景的相机 ,注意 是"提交" , vp矩阵不是引用
		
		Hazel::Renderer2D::DrawQuad({ 1.f,0.2f }, { 0.3f,1.5f }, { 1.f,0.2f,0.4f,1.f });
		Hazel::Renderer2D::DrawQuad({ -1.f,-1.f,-0.1f }, { 1.f,1.f },{0.3f,0.5f,0.9f,1.0f});
		Hazel::Renderer2D::DrawQuad({ -5.f,-5.f,-0.2f }, { 10.f,10.f },m_boardTexture,10.f);
		//Hazel::Renderer2D::DrawQuad({ -1.f,1.f,0.1f }, { 1.f,1.f },m_quancifangTexture);
		//Hazel::Renderer2D::DrawRotatedQuad({ 0.5f,0.2f }, { 2.f,2.f }, glm::degrees(rotation),m_quancifangTexture);

		for (float x = -5.0f; x < 5.f; x+=0.5f)
		{
			for (float y = -5.0f; y < 5.f; y+=0.5f)
			{
				Hazel::Renderer2D::DrawQuad({ x ,y ,-0.1f }, { 0.5f,0.5f }, {0.2f,(x + 5.f) / 10.f,(y + 5.f) / 10.f,0.5f});
			}
		}
		Hazel::Renderer2D::EndScene();
	
		
	}
	
	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Hazel::Input::GetMousePos();
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();

		auto bounds = m_cameraController.GetBound();
		auto pos = m_cameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = -(y / height) * bounds.GetHeight()+ bounds.GetHeight() * 0.5f;//y轴向下;
		m_particle.Position = { x + pos.x,y + pos.y };
		for (int i = 0; i < 5; i++) //emit 5 time?
		{
			m_particleSystem.Emit(m_particle);
		}

	}



	m_particleSystem.OnUpdate(deltaime);
	m_particleSystem.OnRender(m_cameraController.GetCamera());

#if 0
	for (int y = 0; y < (int)m_mapHeight; y++)
	{
		for (int x = 0; x <(int) m_mapWidth; x++)
		{
			char tileType = s_mapTiles[y * m_mapWidth + x];
			Hazel::Ref<Hazel::SubTexture2D> tileTexture;
			if (m_TextureMap.find(tileType) == m_TextureMap.end())
			{
				tileTexture = m_spriteHair;
				HZ_ERROR("didn,t find tile type:{0}", tileType);
			}
			else
			{

				tileTexture= m_TextureMap[tileType]; //小心....unordedmap就算没有也会给你返回个空的..这回导致一些难以识别的崩溃 //所以加个判别
			}
			Hazel::Renderer2D::DrawQuad({ x - m_mapWidth/2.f,(-y+m_mapHeight-1)-m_mapHeight/2.f,0.2f },{1.f,1.f}, tileTexture);
		}

	}

	Hazel::Renderer2D::BeginScene(m_cameraController.GetCamera());
	Hazel::Renderer2D::DrawQuad({ 0.f,0.0f,0.1f }, { 1.f,1.f }, m_spriteHair);
	Hazel::Renderer2D::DrawQuad({ 1.f,0.0f,0.1f }, { 1.f,2.f }, m_spriteTree);

	Hazel::Renderer2D::EndScene();

#endif


}

void SandBox2D::OnEvent(Hazel::Event& e)
{
	m_cameraController.OnEvent(e);

}
