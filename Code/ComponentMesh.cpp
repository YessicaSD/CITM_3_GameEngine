#include "ComponentMesh.h"
#include "Globals.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#include "GameObject.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleImport.h"

#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "MathGeoLib/include/Geometry/Triangle.h"

#include "ComponentMaterial.h"
#include "imgui/imgui.h"
#include "ResourceMesh.h"

CLASS_DEFINITION(Component, ComponentMesh)

ComponentMesh::ComponentMesh(GameObject *gameobject) : Component(gameobject)
{
	name = "Mesh";

	fill_color[0] = fill_color[1] = fill_color[2] = fill_color[3] = 1.f;
	line_color[0] = line_color[1] = line_color[2] = line_color[3] = 1.f;
	point_color[0] = point_color[1] = point_color[2] = point_color[3] = 1.f;
	material = gameobject->CreateComponent<ComponentMaterial>();
	material->SetMeshComponent(this);
}

ComponentMesh::~ComponentMesh()
{
	gameobject->RemoveComponent<ComponentMaterial>();
}

void ComponentMesh::OnPostUpdate()
{
	if (gameobject->transform->IsSelected())
	{
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	if (mesh->uv_coord != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	glPushMatrix();
	glMultMatrixf((const GLfloat *)&gameobject->transform->GetGlobalMatrix().Transposed());
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

	if (mesh->uv_coord != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uv);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uv);
		glTexCoordPointer(mesh->uv_dimensions, GL_FLOAT, 0, NULL);
	}

	if (render_mode.fill)
	{
		if (mesh->vertex_normals != nullptr)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex_normals);
			glNormalPointer(GL_FLOAT, 0, NULL);
		}
		if (material != nullptr)
		{
			material->RenderTexture();
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4f(fill_color[0], fill_color[1], fill_color[2], fill_color[3]);
		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		
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
	if (gameobject->transform->IsSelected())
	{
		DrawOutline();
	}

	//glDisableClienState(GL_VERTEX_ARRAY);//TODO: Activate this
	material->DisableGLModes();

	if (mesh->uv_coord != nullptr)
	{
		glDisable(GL_TEXTURE_COORD_ARRAY);
	}
	glPopMatrix();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ComponentMesh::DrawVertexNormal()
{
	glColor3f(0.2f, 1.f, 0.25f);
	float lenght = 2;
	if (mesh->vertex_normals != nullptr)
	{
		for (uint i = 0; i < mesh->num_vertices; ++i)
		{
			glBegin(GL_LINES);
			glVertex3f(mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z);
			glVertex3f(mesh->vertices[i].x + mesh->vertex_normals[i].x * lenght, mesh->vertices[i].y + mesh->vertex_normals[i].y * lenght, mesh->vertices[i].z + mesh->vertex_normals[i].z * lenght);
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
		glVertex3f(mesh->face_middle_point[i].x + mesh->faces_normals[i].x * lenght, mesh->face_middle_point[i].y + mesh->faces_normals[i].y * lenght, mesh->face_middle_point[i].z + mesh->faces_normals[i].z * lenght);
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
		if (render_mode.fill)
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
	gameobject->RemoveComponent<ComponentMaterial>();

	if (mesh != nullptr)
	{
		mesh->StopUsingResource();
		mesh = nullptr;
	}
	//TODO: Go to the gameobject and remove this component
}

void ComponentMesh::DrawOutline()
{
	if (glIsEnabled(GL_STENCIL_TEST) == GL_TRUE)
	{
		bool light = false;
		if (light = glIsEnabled(GL_LIGHTING))
		{
			glDisable(GL_LIGHTING);
		}

		glColor3f(1.f,1.f, 1.f);
		glLineWidth(5.f);

		glStencilFunc(GL_NOTEQUAL, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glPolygonMode(GL_FRONT, GL_LINE);

		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indice);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);

		glDisable(GL_STENCIL_TEST);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisableClientState(GL_VERTEX_ARRAY);

		glLineWidth(1);

		if (light)
		{
			glEnable(GL_LIGHTING);
		}
	}

}

bool ComponentMesh::Intersect(LineSegment * ray, RaycastHit& hit)
{
	bool ret = false;
	hit.transform = nullptr;
	LineSegment local_ray((*ray));
	local_ray.Transform(gameobject->transform->GetGlobalMatrix().Inverted());

	for (int i = 0; i < mesh->num_indices;i+=3 )
	{
		Triangle tri(mesh->vertices[mesh->indices[i]], mesh->vertices[mesh->indices[i + 1]], mesh->vertices[mesh->indices[i + 2]]);
		RaycastHit new_hit(gameobject->transform);
		if (local_ray.Intersects(tri, &new_hit.distance, &new_hit.hit_point))
		{
			if (hit.transform == nullptr || new_hit.distance < hit.distance)
			{
				hit = new_hit;
				ret = true;
			}
		}

	}
	return ret;
}

bool ComponentMesh::SetMesh(ResourceMesh * mesh)
{
	bool ret = false;

	if (this->mesh != nullptr)
	{
		this->mesh->StopUsingResource();
	}

	if (mesh != nullptr)
	{
		this->mesh = mesh;
		mesh->StartUsingResource();
		ret = true;
	}
	else
	{
		LOG("Can't set the mesh to this GameObject.");
	}

	return ret;
}
