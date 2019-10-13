#include "Application.h"
#include "ModuleImport.h"
#include "ModuleRenderer3D.h"

#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#include "GameObject.h"
#include "ComponentMesh.h"
#include "AssetMesh.h"

bool ModuleImport::Start()
{
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	return true;
}

bool ModuleImport::LoadMesh(const char * path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		AssimpScene* assimp_scene = new AssimpScene();
		assimp_scenes.push_back(assimp_scene);

		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* assimp_mesh = scene->mMeshes[i];
			AssetMesh * asset_mesh = new AssetMesh();
			assimp_scene->assimp_meshes.push_back(asset_mesh);

			//1 Create meshes
			LoadVertices(asset_mesh, assimp_mesh);
			LoadFaces(assimp_mesh, asset_mesh);
			assimp_scene->assimp_meshes.push_back(asset_mesh);

			//2 associate meshes
			CreateGameObjectsFromNodes(scene->mRootNode, &App->scene->root_gameobject.transform, assimp_scene);
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", path);
	}
	
	return true;
}

void ModuleImport::LoadFaces(aiMesh * assimp_mesh, AssetMesh * &mesh_component)
{
	if (assimp_mesh->HasFaces())
	{
		if (assimp_mesh->HasNormals())
		{
			mesh_component->normals = new aiVector3D[assimp_mesh->mNumVertices];
			memcpy(mesh_component->normals, assimp_mesh->mNormals, sizeof(aiVector3D) * assimp_mesh->mNumVertices);
		}


		mesh_component->num_indices = assimp_mesh->mNumFaces * 3;
		mesh_component->indices = new uint[mesh_component->num_indices]; // assume each face is a triangle
		for (uint i = 0; i < assimp_mesh->mNumFaces; ++i)
		{
			if (assimp_mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&mesh_component->indices[i * 3], assimp_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}

		mesh_component->numFaces = assimp_mesh->mNumFaces;
		mesh_component->faces_normals = new float3[assimp_mesh->mNumFaces];
		mesh_component->face_middle_point = new float3[assimp_mesh->mNumFaces];

		for (uint i = 0; i < mesh_component->num_indices; i += 3)
		{
			uint index = mesh_component->indices[i];
			float3 vertex1 = { mesh_component->vertices[index * 3], mesh_component->vertices[index * 3 + 1] , mesh_component->vertices[index * 3 + 2] };
			index = mesh_component->indices[i + 1];

			float3 vertex2 = { mesh_component->vertices[index * 3], mesh_component->vertices[index * 3 + 1] , mesh_component->vertices[index * 3 + 2] };
			index = mesh_component->indices[i + 2];
			float3 vertex3 = { mesh_component->vertices[index * 3], mesh_component->vertices[index * 3 + 1] , mesh_component->vertices[index * 3 + 2] };

			float3 vector1 = vertex2 - vertex1;
			float3 vector2 = vertex3 - vertex1;

			mesh_component->faces_normals[i / 3] = Cross(vector1, vector2);
			mesh_component->faces_normals[i / 3].Normalize();
			mesh_component->face_middle_point[i / 3] = { (vertex1.x + vertex2.x + vertex3.x) / 3, (vertex1.y + vertex2.y + vertex3.y) / 3, (vertex1.z + vertex2.z + vertex3.z) / 3 };
		}
		glGenBuffers(1, &mesh_component->id_indice);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_component->id_indice);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*assimp_mesh->mNumFaces * 3, mesh_component->indices, GL_STATIC_DRAW);
	}
}

void ModuleImport::LoadVertices(AssetMesh * component_mesh, aiMesh * assimp_mesh)
{
	component_mesh->num_vertices = assimp_mesh->mNumVertices;
	component_mesh->vertices = new float[component_mesh->num_vertices * 3];
	memcpy(component_mesh->vertices, assimp_mesh->mVertices, sizeof(float) * component_mesh->num_vertices * 3);
	LOG("New mesh with %d vertices", component_mesh->num_vertices);

	glGenBuffers(1, &component_mesh->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, component_mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * component_mesh->num_vertices * 3, component_mesh->vertices, GL_STATIC_DRAW);
}

void ModuleImport::CreateGameObjectsFromNodes(aiNode * node, ComponentTransform * parent, AssimpScene * assimp_scene)
{
	GameObject * new_gameobject = new GameObject(std::string(node->mName.C_Str()), parent);

	//new_gameobject->transform.local_matrix.Set(
	//	node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4,
	//	node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4,
	//	node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4,
	//	node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4);

	////TODO: Calculate global matrix after that, don't set it directly to the local matrix of the fbx node
	//new_gameobject->transform.global_matrix.Set(
	//	node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4,
	//	node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4,
	//	node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4,
	//	node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4);

	//TODO: Search if there is a better way to convert from aiMatrix4x4 to math::float4x4 (both are arrays with 16 positions at the end)

	if (node->mNumMeshes > 0u)
	{
		//Load the meshes of this GameObject
		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			ComponentMesh * component_mesh = new_gameobject->CreateComponent<ComponentMesh>();
			component_mesh->mesh = assimp_scene->assimp_meshes[node->mMeshes[i]];
		}
	}

	for (int i = 0 ; i < node->mNumChildren; ++i)
	{
		CreateGameObjectsFromNodes(node->mChildren[i], &new_gameobject->transform, assimp_scene);
	}
}

bool ModuleImport::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}




