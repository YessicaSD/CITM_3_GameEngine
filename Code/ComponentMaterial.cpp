#include "ComponentMaterial.h"
#include "glew\include\GL\glew.h"
#include "ResourceMesh.h"
#include <gl\GL.h>

#include "imgui/imgui.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleResourceManager.h"

CLASS_DEFINITION(Component, ComponentMaterial)

ComponentMaterial::ComponentMaterial(GameObject *gameobject, ComponentMesh *mesh) : Component(gameobject)
{
	name = "Material";
	if (mesh != nullptr)
	{
		this->component_mesh = mesh;
	}
}

ComponentMaterial::ComponentMaterial(GameObject *gameobject) : Component(gameobject)
{
}

void ComponentMaterial::CleanUp()
{
	if (texture != nullptr)
	{
		texture->StopUsingResource();
		texture = nullptr;
	}
}

bool ComponentMaterial::SetTexture(ResourceTexture *texture)
{
	bool ret = false;

	if (this->texture != nullptr)
	{
		this->texture->StopUsingResource();
	}

	if (component_mesh->mesh != nullptr && component_mesh->mesh->uv_coord != nullptr && texture != nullptr)
	{
		this->texture = texture;
		this->texture->StartUsingResource();
		ret = true;
	}
	else
	{
		LOG("Can't set the texture to this GameObject");
	}

	return ret;
}

void ComponentMaterial::DisableGLModes()
{
	if (texture != nullptr)
	{
		glFlush();
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void ComponentMaterial::RenderTexture()
{
	if (texture != nullptr)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture->buffer_id);
	}
}

void ComponentMaterial::PropertiesEditor()
{
	ImGuiStyle &style = ImGui::GetStyle();
	if (ImGui::CollapsingHeader(name.c_str()))
	{
		if (texture != nullptr)
			ImGui::Image((void *)(intptr_t)texture->buffer_id, ImVec2(100, 100));
		{
			float child_height = ImGui::GetTextLineHeight() + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
			int i = 0;
			ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild(ImGui::GetID((void *)(intptr_t)i), ImVec2(-100, child_height), true, child_flags);
			ImGui::EndChild();
		}
	}
}

void ComponentMaterial::SetMeshComponent(ComponentMesh *component_mesh)
{
	this->component_mesh = component_mesh;
}

void ComponentMaterial::OnSave(JSONFile * file)
{
	JSONFile material_file = file->AddSection("Material");
	if (texture != nullptr)
	{
		material_file.SaveUID("Resource material UID", texture->GetUID());
	}
}

void ComponentMaterial::OnLoad(JSONFile * file)
{
	UID resource_uid = file->LoadUID("Resource material UID");
	ResourceTexture * texture = (ResourceTexture*)App->resource_manager->GetResource(resource_uid);
	if (texture != nullptr)
	{
		SetTexture(texture);
	}
}