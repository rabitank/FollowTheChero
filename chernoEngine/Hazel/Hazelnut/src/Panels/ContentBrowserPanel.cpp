#include "ContentBrowserPanel.h"
#include "hzpch.h"
#include "ImGui/imgui.h"


namespace Hazel
{ 
	//std::string 目前没有constexpr的初始化和转换
	//有其他目录到时候再说
	extern const std::filesystem::path g_AssetsDirectory = "Assets";

	ContentBrowserPanel::ContentBrowserPanel()
	{
		m_directoryIcon = Texture2D::Create("Assets/Texture/ContentIcon/FoloerIcon.png");
		m_fileIcon = Texture2D::Create("Assets/Texture/ContentIcon/fileIcon.png");
		m_currentDirectory = g_AssetsDirectory;
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		
		ImGui::Begin("ContentBrowserPanel");
		//return
		if (m_currentDirectory != std::filesystem::path(g_AssetsDirectory))
		{
			if (ImGui::Button("<-"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
			}
		}

		//style,cell icom panel
		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			//DrawFileIcon
			//这里添加不同ID域作为限制,避免了下面情况的发生
			ImGui::PushID(filenameString.c_str());//use c_str  to ID -> like scope in ImGui
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_directoryIcon : m_fileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			//cherno 的拖拽,资源ID由拖拽对象提供,由于imageButton的image用的是同一张图(file.png),它的纹理ID相同,所以指向最后一个imageButton,总打开同一个场景,
			//ImageButtonEx()会重新指定一个新的ID,但是Cherno没用
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			//拖拽资源
			//但是我不需要 0v0		
			if (ImGui::BeginDragDropSource()) 
			{
				auto relativePath = std::filesystem::relative(path,g_AssetsDirectory);
				const wchar_t* itemPath = relativePath.c_str();
				//load dragsource to imgui wait to be loaded in dragtarget
				//wclen return sizeof wchar(no Null)
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t),ImGuiCond_Once); //我们实际上载的是文件路径

				//size_t dByte = sizeof(wchar_t) * wcslen(itemPath) + 1;
				//char* showPath=new char[dByte];
				//wcstombs_s(nullptr, showPath, dByte, itemPath,_TRUNCATE);
				//HZ_CORE_INFO("BeginDragDropSource Upload itemPath {0}", showPath);
				ImGui::EndDragDropSource();
			}
			
			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_currentDirectory /= path.filename(); // m_currentDirectory = m_currentDirectory/path.filename 

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		// TODO: status bar
		ImGui::End();
	

	}

}

