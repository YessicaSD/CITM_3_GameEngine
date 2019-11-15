#include "ResourceTexture.h"
#include "glew/include/GL/glew.h"
#include "DevIL/include/il.h"
#include "Application.h"
#include "ModuleFileSystem.h"

ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::SaveFileData()
{
	//Create path
	uint path_size = 250u;
	char * path = new char[path_size];
	App->file_system->CreatePath(path, path_size, RESOURCES_TEXTURES_FOLDER, "texture", uid, "dds");

	//Save the image as .dds (quicker format)
	ILuint size;
	ILubyte * data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, NULL, 0);
	if (size > 0)
	{
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			App->file_system->SaveFile(data, size, &path);
		}
		RELEASE_ARRAY(data);
	}

	RELEASE_ARRAY(path);

	return true;
}

bool ResourceTexture::LoadFileData()
{
	//Load the dds we created in SaveFileData

	return true;
}

bool ResourceTexture::ReleaseData()
{
	glDeleteTextures(1, &buffer_id);
	//TODO: Finish this function
	return true;
}
