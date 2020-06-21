#include "PanelImport.h"
#include "../imgui/imgui.h"
#include "../Assimp/include/postprocess.h"
#include "Application.h"
#include "ModuleGui.h"
#include "PanelAssets.h"

PanelImport::PanelImport(std::string name, bool active, std::vector<SDL_Scancode> shortcuts) :
	Panel(name, active, shortcuts)
{
}

void PanelImport::Draw()
{
	ImGui::Begin("Import");
	//switch() type of the selected object in panel assets
	//switch (App->gui->panel_assets->selected_asset.type)
	//{

	//}
	DrawModel();
	ImGui::End();
}

void PanelImport::DrawModel()
{
	//TODO: Preview model image with the settings?

	//TODO: Access the import options from somewhere (maybe load them from the meta once we select it?)
	//TODO: Add presets (realtime max quality, etc.)
	ImGui::Checkbox("Calculate tangent space", &model_aux.calcTangentSpace);
	Tooltip("<hr>Calculates the tangents and bitangents for the imported meshes. Does nothing if a mesh does not have normals.You might want this post processing step to be executed if you plan to use tangent space calculations such as normal mapping  applied to the meshes.There's an importer property, <tt>#AI_CONFIG_PP_CT_MAX_SMOOTHING_ANGLE< / tt>, which allows you to specify a maximum smoothing angle for the algorithm.However, usually you'll want to leave it at the default value.");
	ImGui::Checkbox("Join identical vertices", &model_aux.joinIdenticalVertices);
	ImGui::Checkbox("Make left handed", &model_aux.makeLeftHanded);
	//ImGui::Checkbox();
	//ImGui::Checkbox();

	if (ImGui::Button("Reimport"))
	{
	}
}

void PanelImport::DrawTexture()
{
}

void PanelImport::Tooltip(const char* desc) const
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
