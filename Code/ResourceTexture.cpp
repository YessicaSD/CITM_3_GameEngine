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
	////Create path
	//char * data = nullptr;
	//uint path_size = 250u;
	//char * path = new char[path_size];
	//App->file_system->CreatePath(path, path_size, RESOURCES_TEXTURES_FOLDER, "texture", uid, "hinata_texture");

	//ResourceTexture* new_texture = nullptr;

	//uint image_id = 0u;
	//ilGenImages(1, &image_id);
	//ilBindImage(image_id);
	////ilutRenderer(ILUT_OPENGL);

	//if (ilLoadImage(path) == IL_TRUE)
	//{
	//	new_texture = App->resource_manager->CreateNewResource<ResourceTexture>();
	//	new_texture->path = path;
	//	new_texture->buffer_id = ilutGLBindTexImage();
	//	new_texture->height = ilGetInteger(IL_IMAGE_HEIGHT);
	//	new_texture->width = ilGetInteger(IL_IMAGE_WIDTH);
	//	new_texture->size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
	//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//	glBindTexture(GL_TEXTURE_2D, new_texture->buffer_id);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, 0x8072, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//	textures[path] = new_texture;

	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}
	//else
	//{
	//	auto error = ilGetError();
	//	LOG("Failed to load texture with path: %s. Error: %s", path, ilGetString(error));
	//	return new_texture;
	//}
	////free(lump);
	//ilDeleteImages(1, &image_id);

	return true;
}

bool ResourceTexture::ReleaseData()
{
	glDeleteTextures(1, &buffer_id);
	//TODO: Finish this function
	return true;
}
