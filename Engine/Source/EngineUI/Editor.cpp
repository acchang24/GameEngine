#include "Editor.h"
#include "imgui.h"

Editor::Editor() :
	mIsVisible(false)
{
}

Editor::~Editor()
{
}

void Editor::SetEditorUI()
{
	if (mIsVisible)
	{
		ImGui::ShowDemoWindow(&mIsVisible);

		// TODO: Render all editor panels
	}
}
