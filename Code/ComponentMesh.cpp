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

ComponentMesh::ComponentMesh(GameObject * gameobject) : Component(gameobject)
{
}

void ComponentMesh::OnPostUpdate()
{
	//ACTIVE TEXTURE MODE
	//glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPushMatrix();
	glMultMatrixf((const GLfloat *)&gameobject->transform.global_matrix[0]);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indice);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

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



	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

	//glDisableClienState(GL_VERTEX_ARRAY);//TODO: Activate this
	glPopMatrix();

	//Multiple modes

	//	if (App->scene->view_mode["default"])
	//{
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	cube[1]->Draw();
	//
	//	glEnableClientState(GL_VERTEX_ARRAY);
	//	glBindBuffer(GL_ARRAY_BUFFER, sphere_v_id);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_indice_id);
	//	glVertexPointer(3, GL_FLOAT, 0, NULL);
	//
	//	glDrawElements(GL_TRIANGLES, sphereInfo->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	//}
	//
	//if (view_mode["wireframe"])
	//{
	//	glColor3f(0, 0, 1);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	cube[1]->Draw();
	//
	//	glEnableClientState(GL_VERTEX_ARRAY);
	//	glBindBuffer(GL_ARRAY_BUFFER, sphere_v_id);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_indice_id);
	//	glVertexPointer(3, GL_FLOAT, 0, NULL);
	//
	//	glDrawElements(GL_TRIANGLES, sphereInfo->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	//
	//	glColor3f(1, 1, 1);
	//}
	//
	//if (view_mode["vertex"])
	//{
	//	glColor3f(1, 0, 0);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	//	glPointSize(3);
	//	cube[1]->Draw();
	//
	//	glEnableClientState(GL_VERTEX_ARRAY);
	//	glBindBuffer(GL_ARRAY_BUFFER, sphere_v_id);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_indice_id);
	//	glVertexPointer(3, GL_FLOAT, 0, NULL);
	//
	//	glDrawElements(GL_TRIANGLES, sphereInfo->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	//	glColor3f(1, 1, 1);
	//}
	//
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ComponentMesh::DrawVertexNormal()
{
	glColor3f(0.2f, 1.f, 0.25f);
	uint j = 0;
	float lenght = 2;
	for (uint i = 0; i < mesh->num_vertices*3; i+=3)
	{

		glBegin(GL_LINES);
		glVertex3f(mesh->vertices[i], mesh->vertices[i+1], mesh->vertices[i+2]);
		glVertex3f(mesh->vertices[i] + mesh->normals[j].x*lenght, mesh->vertices[i+1] + mesh->normals[j].y *lenght, mesh->vertices[i+2] + mesh->normals[j].z*lenght);
		++j;
		glEnd();
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
