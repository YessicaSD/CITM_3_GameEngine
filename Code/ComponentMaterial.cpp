#include "ComponentMaterial.h"
#include "AssetMesh.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#include "imgui/imgui.h"

CLASS_DEFINITION(Component, ComponentMaterial)

ComponentMaterial::ComponentMaterial(GameObject * gameobject, ComponentMesh* mesh) : Component(gameobject)
{
	name = "Material";
	if(mesh)
		this->component_mesh = mesh;
}

void ComponentMaterial::SetTexture(Texture * texture)
{
	if (component_mesh->mesh && component_mesh->mesh->UVCoord)
	{
		if (texture)
		{
			this->texture = texture;
		}
	}
}

void ComponentMaterial::DisableGLModes()
{
	if (texture)
	{
		glFlush();
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

void ComponentMaterial::RenderTexture()
{
	if (texture)
	{
		
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		AssetMesh * mesh = component_mesh->mesh;
		glBindTexture(GL_TEXTURE_2D, texture->buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uv);
		glTexCoordPointer(mesh->uv_num_components, GL_FLOAT, 0, (void*)0);
	}
	
}

void ComponentMaterial::PropertiesEditor()
{
	ImGuiStyle& style = ImGui::GetStyle();
	if (ImGui::CollapsingHeader(name.c_str()))
	{
		if (texture)
		{
			
			ImGui::Image((void*)(intptr_t)texture->buffer_id, ImVec2(100, 100));
			
			float child_height = ImGui::GetTextLineHeight() + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
			int i = 0;
			ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)i), ImVec2(-100, child_height), true, child_flags);
			ImGui::Text("Path: %s", texture->path);
			ImGui::EndChild();
			
		}
	}





}

