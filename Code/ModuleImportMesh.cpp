#include "ModuleImportMesh.h"
#include "Timer.h"
#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleResourceManager.h"
#include "Assimp/include/mesh.h"
#include "par/par_shapes.h"
#include "ResourceBone.h"

ModuleImportMesh::ModuleImportMesh(const char *name) : Module(true, name)
{
}

ResourceMesh * ModuleImportMesh::ImportAssimpMesh(aiMesh * assimp_mesh, UID uid, const char * asset_path)
{
	Timer import_timer;

	ResourceMesh *resource_mesh = App->resource_manager->CreateResource<ResourceMesh>(uid);
	resource_mesh->asset_source = asset_path;
	//INFO: We can only do this cast because we know that aiVector3D is 3 consecutive floats
	resource_mesh->ImportVertices(assimp_mesh->mNumVertices, (const float *)assimp_mesh->mVertices);
	resource_mesh->CreateBoundingBox();
	resource_mesh->ImportVerticesNormals(assimp_mesh);
	resource_mesh->ImportFaces(assimp_mesh);
	resource_mesh->CalculateFaceNormals();
	resource_mesh->ImportUVs(assimp_mesh);
	if (assimp_mesh->HasBones())
	{
		for (uint i = 0u; i < assimp_mesh->mNumBones; ++i)
		{
			aiBone bone = (*assimp_mesh->mBones[i]);
			ResourceBone* resource_bone = App->resource_manager->CreateResource<ResourceBone>();
			
		}
	}

	resource_mesh->SaveFileData();

	LOG("Success importing mesh with uid: %llu in: %i ms.", resource_mesh->GetUID(), import_timer.Read());

	return resource_mesh;
}

//ResourceMesh * ModuleImportMesh::ImportParShapeMesh(par_shapes_mesh * mesh)
//{
//	ResourceMesh *resource_mesh = App->resource_manager->CreateResource<ResourceMesh>();
//
//	resource_mesh->ImportVertices(mesh->npoints, mesh->points);
//	resource_mesh->CreateBoundingBox();
//
//	//TODO: Get vertices normals
//	resource_mesh->ImportFaces(mesh->ntriangles, mesh->triangles);
//	resource_mesh->CalculateFaceNormals();
//	//TODO: Load uv coomponent (2D or 3D)
//	resource_mesh->ImportUVs(mesh->tcoords);
//
//	resource_mesh->SaveFileData();
//
//	return resource_mesh;
//}
