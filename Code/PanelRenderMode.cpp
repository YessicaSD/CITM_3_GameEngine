#include "PanelRenderMode.h"
#include "imgui\imgui.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>
PanelRenderMode::PanelRenderMode(std::string name, bool active):Panel(name, active)
{


}

void PanelRenderMode::Draw()
{
	ImGui::Begin(name.c_str(), &active);
	static int style_idx = -1;
	if (ImGui::Combo("Mode", &style_idx, "Default\0Wireframe\0"))
	{
		switch (style_idx)
		{
		case 0: 
		{
			labelState = "Default";
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		
			break;
		case 1:
		{
			labelState = "Wireframe";
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
			
			break;
		
		}
	}
	ImGui::End();
}

