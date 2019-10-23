#include "ComponentMesh.h"
#include "Globals.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>
#include "AssetMesh.h"

#include "GameObject.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleImport.h"
#include "MathGeoLib/include/Math/float4.h"
ComponentMesh::ComponentMesh(GameObject * gameobject) : Component(gameobject)
{
	name = "Mesh";

	fill_color[0] = fill_color[1] = fill_color[2] = fill_color[3] = 1.f;
	line_color[0] = line_color[1] = line_color[2] = line_color[3] = 1.f;
	point_color[0] = point_color[1] = point_color[2] = point_color[3] = 1.f;
}

ComponentMesh::~ComponentMesh()
{
	CleanUp();
}

void ComponentMesh::OnPostUpdate()
{
	//ACTIVE TEXTURE MODE
	//glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPushMatrix();
	glMultMatrixf((const GLfloat *)&gameobject->transform->global_matrix[0]);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indice);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (render_mode.vertex_normals)
	{
		DrawVertexNormal();
	}
	if (render_mode.face_normals)
	{
		DrawNormals();
	}

	if (render_mode.fill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4f(fill_color[0], fill_color[1], fill_color[2], fill_color[3]);
		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);
	}

	if (render_mode.wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(line_width);
		glColor4f(line_color[0], line_color[1], line_color[2], line_color[3]);
		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);
	}

	if (render_mode.point)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(point_size);
		glColor4f(point_color[0], point_color[1], point_color[2], point_color[3]);
		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);
	}
	

	//UV
	//if (mesh->UVCoord != nullptr)
	//{
	//	if (App->import->lenna_img_id)
	//	{
	//		glBindTexture(GL_TEXTURE_2D, App->import->lenna_img_id->buffer_id);

	//	}
	//	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uv);
	//	glTexCoordPointer(mesh->uv_num_components, GL_FLOAT, 0, (void*)0);
	//}

	//glDisableClienState(GL_VERTEX_ARRAY);//TODO: Activate this
	glPopMatrix();
}

void ComponentMesh::DrawVertexNormal()
{
	glColor3f(0.2f, 1.f, 0.25f);
	uint j = 0;
	float lenght = 2;
	if (mesh->vertex_normals != nullptr)
	{
		for (uint i = 0; i < mesh->num_vertices * 3; i += 3)
		{
			glBegin(GL_LINES);
			glVertex3f(mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2]);
			glVertex3f(mesh->vertices[i] + mesh->vertex_normals[j].x*lenght, mesh->vertices[i + 1] + mesh->vertex_normals[j].y *lenght, mesh->vertices[i + 2] + mesh->vertex_normals[j].z*lenght);
			++j;
			glEnd();
		}
	}

	glColor3f(1, 1, 1);
}

void ComponentMesh::DrawNormals()
{
	glColor3f(0, 0, 1);
	float lenght = 2;
	for (uint i = 0; i < mesh->num_faces; ++i)
	{
		glBegin(GL_LINES);
		glVertex3f(mesh->face_middle_point[i].x, mesh->face_middle_point[i].y, mesh->face_middle_point[i].z);
		glVertex3f(mesh->face_middle_point[i].x + mesh->faces_normals[i].x*lenght, mesh->face_middle_point[i].y + mesh->faces_normals[i].y *lenght, mesh->face_middle_point[i].z + mesh->faces_normals[i].z*lenght);
		glEnd();
	}
	glColor3f(1, 1, 1);
}

void ComponentMesh::PropertiesEditor()
{
	if (CollapsigHeaderWithCheckbox())
	{
		ImGui::Text("Render options");

		ImGui::Checkbox("View fill", &render_mode.fill);
		if(render_mode.fill)
		{
			ImGui::ColorPicker4("Fill color", fill_color);
		}

		ImGui::Checkbox("View wireframe", &render_mode.wireframe);
		if (render_mode.wireframe)
		{
			ImGui::InputFloat("Wireframe width", &line_width);
			ImGui::ColorPicker4("Wireframe color", line_color);
		}

		ImGui::Checkbox("View point", &render_mode.point);
		if (render_mode.point)
		{
			ImGui::InputFloat("Point size", &point_size);
			ImGui::ColorPicker4("Point color", point_color);
		}

		ImGui::Text("View normals");
		ImGui::Checkbox("View points normals", &render_mode.vertex_normals);
		ImGui::Checkbox("View faces normals", &render_mode.face_normals);


	}
}

void ComponentMesh::CleanUp()
{
	if (mesh)
	{
		delete mesh;
		mesh = nullptr;
	}
	if (global_face_normals)
	{
		delete global_face_normals;
		global_face_normals = nullptr;
	}
	if (global_vertex_normals)
	{
		delete global_vertex_normals;
		global_vertex_normals = nullptr;
	}
}

void ComponentMesh::UpdateVertexAndNormals()
{
	if(mesh->vertices)
	{
		if(!global_vertex)
		{
			global_vertex = new float3[mesh->num_vertices];
		}
		for (uint i = 0; i < mesh->num_vertices * 3; i += 3)
		{
			float4 vertex4 = { mesh->vertices[i],mesh->vertices[i + 1], mesh->vertices[i + 2],1 };
			vertex4 = vertex4 * gameobject->transform->global_matrix;
			global_vertex[i / 3] = { vertex4.x, vertex4.y, vertex4.z };
		}
	}
	else
	{
		return;
	}
	if (mesh->faces_normals)
	{
		if (!global_face_normals)
		{
			global_face_normals = new float3[mesh->num_faces];
		}

		for (uint i = 0; i < mesh->num_faces; ++i)
		{
			float4 vertex4 = { mesh->faces_normals[i].x,mesh->faces_normals[i].y, mesh->faces_normals[i].z,1 };
			vertex4 = vertex4 * gameobject->transform->global_matrix;
			global_face_normals[i] = { vertex4.x,vertex4.y, vertex4.z };
		}
	}
	if (mesh->vertex_normals)
	{
		if (!global_vertex_normals)
		{
			global_vertex_normals = new float3[mesh->num_vertices];
		}
		for (uint i = 0; i < mesh->num_vertices; ++i)
		{

		}
	}
}
