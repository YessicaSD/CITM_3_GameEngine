#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "ModuleImporter.h"

#include <stdio.h>//to use memcpy

bool ModuleImporter::Start()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	return true;
}

bool ModuleImporter::LoadFile(std::string full_path)
{
	bool ret = true;

	const aiScene * scene = aiImportFile(full_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh * assimp_mesh = scene->mMeshes[i];
			CustomMesh custom_mesh;
			// Copy vertices
			custom_mesh.num_vertices = assimp_mesh->mNumVertices;
			custom_mesh.vertices = new float[custom_mesh.num_vertices * 3];
			memcpy(custom_mesh.vertices, assimp_mesh->mVertices, sizeof(float) * custom_mesh.num_vertices * 3);
			LOG("New mesh with %d vertices", custom_mesh.num_vertices);
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", full_path);
		ret = false;
	}

	return ret;
}


bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}
