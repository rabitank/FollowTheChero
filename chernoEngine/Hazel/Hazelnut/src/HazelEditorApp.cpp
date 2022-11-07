#include "Hazel.h"


//-------------entry point	-------------
#include "Hazel/Core/EntryPoint.h"
//---------------------------------------


#include "imGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"



namespace Hazel
{
	class Hazelnut :public Application //Hazel µÄ±à¼­Æ÷½çÃæ!!!!!
	{
	public:
		Hazelnut(const std::string&  windowName=  "Hazel Editor")
			:Application(windowName)
		{

			PushLayer(new EditorLayer());// ExampleLayer* -->Layer* 

		}
		~Hazelnut()
		{

		}


	};

	// a box about how the game work would be send/trans to engine's entry point

	Application* CreateApplication()
	{
		return new Hazelnut();

	}
}
