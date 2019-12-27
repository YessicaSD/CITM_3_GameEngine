#include  "ComponentSkinnedMesh.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#include "MathGeoLib/include/Math/float3.h"
#include "imgui/imgui.h"

#include "GameObject.h"
#include "ResourceMesh.h"
#include "ResourceBone.h"
#include "Globals.h"

CLASS_DEFINITION(Component, ComponentSkinnedMesh)

void ComponentSkinnedMesh::OpenBuffer()
{
	if (id_vertex_buffer != 0u)
	{
		glDeleteBuffers(1, &id_vertex_buffer);
		id_vertex_buffer = 0u;
	}
	if (vertices != nullptr)
	{
		glGenBuffers(1, &id_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, id_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, vertices, GL_DYNAMIC_DRAW);
	}
}

ComponentSkinnedMesh::ComponentSkinnedMesh(GameObject * parent):Component(parent)
{
	name = "Skinned Mesh";
};

void ComponentSkinnedMesh::PropertiesEditor()
{
	if (CollapsigHeaderWithCheckbox())
	{

	}
}

void ComponentSkinnedMesh::SetMesh(ResourceMesh * mesh)
{
	if (mesh != nullptr)
	{
		if (this->mesh != nullptr)
		{
			this->mesh->StartUsingResource();
			RELEASE_ARRAY(vertices);
		}
		this->mesh = mesh;
		this->mesh->StartUsingResource();
		vertices = new math::float3[this->mesh->num_vertices];
		memcpy(vertices, this->mesh->vertices, sizeof(float3)*this->mesh->num_vertices);
		OpenBuffer();
	}
	else
	{
		LOG("%s: Couldn't set mesh", name.c_str());
	}
}

void ComponentSkinnedMesh::OnUpdate(float dt)
{
	if (mesh != nullptr)
	{
		memset(vertices, 0, sizeof(float3)*this->mesh->num_vertices);
		for (uint i = 0; i < mesh->num_bones; ++i)
		{
			std::string bone_name  = mesh->bones[i]->GetName();
			std::map<std::string, ComponentTransform*>::iterator iter = bones.find(bone_name);
			ComponentTransform* bone = nullptr;
			if (iter != bones.end())
			{
				bone = (*iter).second;
			}
			else if (root_object != nullptr)
			{
				bone = root_object->Find(bone_name.c_str());
				if (bone != nullptr)
				{
					bones[bone_name] = bone;
				}
			}
			if (bone != nullptr)
			{
				ResourceBone* resource_bone = nullptr;
				for (uint i = 0; i < mesh->num_bones; ++i)
				{
					if (strcmp(mesh->bones[i]->GetName(), bone_name.c_str()) == 0)
					{
						resource_bone = mesh->bones[i];
						break;
					}
				}

				float4x4 trans = gameobject->transform->GetLocalMatrix().Inverted() * bone->GetGlobalMatrix();
				trans = trans *  resource_bone->offset_matrix;

				if (resource_bone != nullptr)
				{
					for (uint i = 0; i < resource_bone->num_weights; ++i)
					{
						float3 original = mesh->vertices[resource_bone->weights[i].vertex_id];
						float3 vertex = trans.TransformPos(original);
						vertices[resource_bone->weights[i].vertex_id] += vertex * resource_bone->weights[i].weigth;
					}
				}
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, id_vertex_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * mesh->num_vertices * 3, vertices);
	}
}

void ComponentSkinnedMesh::OnPostUpdate()
{
	if (mesh == nullptr)
	{
		LOG("Error: Invalid mesh.");
		return;
	}
	//if (gameobject->transform->IsSelected())
	//{
	//	glEnable(GL_STENCIL_TEST);
	//	glStencilFunc(GL_ALWAYS, 1, -1);
	//	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//}

	if (mesh->uv_coord != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	glPushMatrix();
	glMultMatrixf((const GLfloat *)&gameobject->transform->GetGlobalMatrix().Transposed());
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indice);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	if (mesh->uv_coord != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uv);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}
	if (mesh->vertex_normals != nullptr)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);
	
	//if (gameobject->transform->IsSelected())
	//{
	//	DrawOutline();
	//}
	//Disable all ============================
	glDisableClientState(GL_VERTEX_ARRAY);
	if (mesh->vertex_normals != nullptr)
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	if (mesh->uv_coord != nullptr)
	{
		glDisable(GL_TEXTURE_COORD_ARRAY);
	}
	glPopMatrix();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ComponentSkinnedMesh::CleanUp()
{
}
