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
	//INFO: The number of elements on the ranges array must be the same as in the ranges array of SaveFileData()
	uint header[1];
	CopyToMemory(header, cursor, sizeof(header));
	nodes.reserve(header[0u]);

	uint name_bytes = NODE_NAME_SIZE * sizeof(char);
	

	//Load nodes
	for (uint i = 0u; i < header[0u]; ++i)
	{
		ResourceModelNode node;
		nodes[i]->name = new char[NODE_NAME_SIZE];
		//INFO: Clear the name
		//memset(nodes[i]->name, 0, name_bytes);
		CopyToMemory(nodes[i]->name, cursor, name_bytes);
		for (uint i = 0u; i < 16u; ++i)
		{
			//reinterpret_cast<float*>(&node.transform)[i];

		}
	}

	return false;
}
