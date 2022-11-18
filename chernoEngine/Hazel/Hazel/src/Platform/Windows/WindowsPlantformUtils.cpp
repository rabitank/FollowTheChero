#include "hzpch.h"
#include "Hazel/Utils/PlantformUtils.h"

#include "Hazel/Core/Application.h"
#include <commdlg.h> //? for a bunch of spersific types about File? 通用对话框的头文件，包括文件对话框，颜色对话框，打印对话框。 ->较底层的c头 , QT一类的软件以及把他集成成一行了
//我们用它来唤起一个文件管理器框窗口选择文件

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h> //for get glfwGetWin32Monitor


namespace Hazel
{
	
	std::string  PlantformUtils::OpenFile(const char* filter) //filter格式: "文本文件(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0\0"
	{
		OPENFILENAMEA ofn; // 公共对话框结构。
		CHAR szFile[260] = { 0 }; // 用来保存获取文件名称的缓冲区。 

		ZeroMemory(&ofn, sizeof(OPENFILENAME)); // 初始化选择文件对话框
		ofn.lStructSize = sizeof(OPENFILENAME); //指定这个结构的大小，以字节为单位。
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile; //打开的文件的全路径
		ofn.nMaxFile = sizeof(szFile);//指定lpstrFile缓冲的大小，以TCHARs为单位
		ofn.lpstrFilter = filter; //打开文件类型过滤器
		ofn.nFilterIndex = 1; //指定在文件类型控件中当前选择的过滤器的索引

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; //位标记的设置，你可以使用来初始化对话框
		if(GetOpenFileNameA(&ofn) == true) // 显示"打开选择文件对话框"。  
		{
			return ofn.lpstrFile;

		}
		return std::string();

		//OPENFLIENAME 解释 https://blog.csdn.net/kevinfan2011/article/details/84982588

// 		open.lpstrFile[0] = '\0'; //第一个字符串是过滤器描述的显示字符串
// 		open.lpstrFileTitle = NULL; // 指向接收选择的文件的文件名和扩展名的缓冲（不带路径信息）。这个成员可以是NULL。
// 		open.nMaxFileTitle = 0;  //指定lpstrFileTitle缓冲的大小，以TCHARs为单位
// 		open.lpstrInitialDir = NULL;  //指向以空字符结束的字符串，可以在这个字符串中指定初始目录。
// 		open.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;//位标记的设置，你可以使用来初始化对话框
// 		if (GetOpenFileName(&open))  // 显示打开选择文件对话框。  
// 		{
// 			path = file;
// 			printf(path + "\n");
// 		}
// 		system("pause");


	}
	
	std::string  PlantformUtils::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn; 
		CHAR szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAME)); // 初始化选择文件对话框
		ofn.lStructSize = sizeof(OPENFILENAME); //指定这个结构的大小，以字节为单位。
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile; 
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter; 
		ofn.nFilterIndex = 1; 

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; //: 路径必须存在,文件必须存在,不能操作文件
		if (GetSaveFileNameA(&ofn) == true) // 显示"保存文件对话框"。  
		{
			return ofn.lpstrFile;

		}
		return std::string();
	}



}