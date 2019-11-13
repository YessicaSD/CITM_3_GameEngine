#include "ResourceModel.h"
#include "Application.h"
#include "ModuleFileSystem.h"

bool ResourceModel::SaveFileData()
{
	char * data;
	char * cursor;

	uint name_bytes = NODE_NAME_SIZE * sizeof(char);

	for (auto iter = nodes.begin(); iter != nodes.end(); ++iter)
	{
		memcpy((*iter)->name, cursor, name_bytes);
	}

	uint path_size = 250u;
	char * path = new char[path_size];
	App->file_system->CreatePath(path, path_size, RESOURCES_MODEL_FOLDER, "model", uid, "hinata_model");

	return false;
}

bool ResourceModel::LoadFileData(char * data)
{
	char * cursor = data;

	//Load header
	//TODO: If it's only one variable that it's on the header, make a single variable instead of an array
	//Called num_nodes (it's more descriptive)
	uint header[] = { nodes.size };
	uint header_bytes = sizeof(header);
	memcpy(header, cursor, header_bytes);
	nodes.reserve(header[0u]);
	cursor += header_bytes;

	uint name_bytes = NODE_NAME_SIZE * sizeof(char);

	//Load nodes
	for (uint i = 0u; i < header[0u]; ++i)
	{
		//TODO: Maybe clear the name before?
	}

	return false;
}
