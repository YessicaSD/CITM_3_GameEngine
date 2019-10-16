#include "PanelAssets.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "Texture.h"
PanelAssets::PanelAssets(const char* name, bool state):Panel(name, state)
{
}
void PanelAssets::Draw()
{
	for (std::map<const char*, Texture*>::iterator iter = App->texture->textures.begin();
		iter!= App->texture->textures.end();
		++iter)
	{
		
		ImGui::Image((void*)(intptr_t)(*iter).second->buffer_id, ImVec2(100, 100));
		ImGui::SameLine();
	}
	//ImGui::Begin(name.c_str());
	//ImGui::End();
}
