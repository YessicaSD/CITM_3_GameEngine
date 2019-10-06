#ifndef __MODULE_IMPORTER_H__
#define __MODULE_IMPORTER_H__

#include "Module.h"

struct CustomMesh
{
	uint id_index = 0u;
	uint num_index = 0u;
	uint * index = nullptr;

	uint id_vertices = 0u;
	uint num_vertices = 0u;
	float * vertices = nullptr;
};

class ModuleImporter : public Module
{
public:
	bool Start() override;
	bool LoadFile(std::string full_path);
	bool CleanUp() override;
};

#endif
