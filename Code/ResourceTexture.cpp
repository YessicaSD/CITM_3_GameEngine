#include "ResourceTexture.h"
#include "glew/include/GL/glew.h"
#include "Application.h"
#include "ModuleFileSystem.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

RESOURCE_DEFINITION(Resource, ResourceTexture)

ResourceTexture::~ResourceTexture()
{
}

ResourceTexture::ResourceTexture() : Resource()
{
}

bool ResourceTexture::SaveFileData()
{
	//Create path
	uint path_size = 250u;
	char * path = new char[path_size];
	App->file_system->CreatePath(path, path_size, RESOURCES_TEXTURES_FOLDER, uid, TEXTURE_EXTENSION);

	//Save the image as .dds (quicker format)
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	ILuint size = size = ilSaveL(IL_DDS, NULL, 0);
	if (size > 0)
	{
		//INFO: It loads the data of the image we've just Bind using BindImage at ModuleTextures::ImportTexture()
		ILubyte * data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			App->file_system->SaveFile(data, size, &path);
		}
		RELEASE_ARRAY(data);
	}

	RELEASE_ARRAY(path);

	return true;
}

//Load the dds we createXd in SaveFileData
bool ResourceTexture::LoadFileData()
{
	Timer load_timer;

	//Create path
	uint path_size = 250u;
	char * custom_format_path = new char[path_size];
	App->file_system->CreatePath(custom_format_path, path_size, RESOURCES_TEXTURES_FOLDER, uid, TEXTURE_EXTENSION);

	//Import data from path first
	uint image_id = 0u;
	ilGenImages(1, &image_id);
	ilBindImage(image_id);
	//ilutRenderer(ILUT_OPENGL);

	if (ilLoadImage(custom_format_path) == IL_TRUE)
	{
		buffer_id = ilutGLBindTexImage();

		height = ilGetInteger(IL_IMAGE_HEIGHT);
		width = ilGetInteger(IL_IMAGE_WIDTH);
		size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);

		height = ilGetInteger(IL_IMAGE_HEIGHT);
		width = ilGetInteger(IL_IMAGE_WIDTH);
		size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
		//Get the data?
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, buffer_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, 0x8072, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		LOG("Success loading texture from: %s in: %i ms.", custom_format_path, load_timer.Read());
	}
	else
	{
		auto error = ilGetError();
		LOG("Error loadig texture with path: %s. Error: %s", custom_format_path, ilGetString(error));
	}
	//free(lump);

	return true;
}

bool ResourceTexture::ReleaseData()
{
	ilDeleteImages(1, &buffer_id);
	buffer_id = 0u;

	return true;
}
