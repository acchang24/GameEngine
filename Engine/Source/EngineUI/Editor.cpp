#include "Editor.h"
#include <iostream>
#include "imgui.h"

Editor::Editor() :
	mIsVisible(false)
{
}

Editor::~Editor()
{
	std::cout << "Deleted editor\n";
}

void Editor::SetEditorUI()
{
	if (mIsVisible)
	{
		ImGui::ShowDemoWindow(&mIsVisible);

		// TODO: Render all editor panels
	}
}
