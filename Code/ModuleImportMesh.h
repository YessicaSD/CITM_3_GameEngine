#ifndef __MODULE_IMPORT_MESH_H__
#define __MODULE_IMPORT_MESH_H__

#include "Module.h"
#include <vector>

class ResourceMesh;
struct aiMesh;

class ModuleImportMesh : public Module
{
public:
	ModuleImportMesh(const char * name);

	ResourceMesh * ImportAssimpMesh(aiMesh *assimp_mesh, UID uid, std::vector<UID> & bones_uid, const char * asset_path);
	//ResourceMesh * ImportParShapeMesh(par_shapes_mesh *mesh);
};

#endif