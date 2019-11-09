
#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#pragma comment (lib, "DevIL/lib/DevIL.lib")
#pragma comment (lib, "DevIL/lib/ILU.lib")
#pragma comment (lib, "DevIL/lib/ILUT.lib")

#include "ModuleTexture.h"
#include "AssetTexture.h"

#define checkImageWidth 512
#define checkImageHeight 512
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

ModuleTexture::ModuleTexture(const char * name) : Module(true, name)
{}

bool ModuleTexture::Init(JSONFile * module_file)
{
	//Initialize DevIL libraries
	LOG("Initializing DevIl libraries");
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	makeCheckTexture();
	return true;
}

//Saves the texture as DDS in the Library folder (faster to use)
bool ModuleTexture::ImportTexture(const char * path)
{
	ILuint size;
	ILubyte * data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, NULL, 0);
	if (size > 0)
	{
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			//TODO: Finish this function
		}
	}
	return true;
}

AssetTexture* ModuleTexture::LoadTexture(const char* path)
{
	AssetTexture* new_texture = nullptr;

	std::map<std::string, AssetTexture*>::iterator iter= textures.find(path);
	if (iter!= textures.end())
	{
		return (*iter).second;
	}
	if (path != nullptr && path != "")
	{
		uint devil_id = 0;
		ilGenImages(1, &devil_id);
		ilBindImage(devil_id);
		//ilutRenderer(ILUT_OPENGL);

		if (!ilLoadImage(path))
		{
			auto error = ilGetError();
			LOG("Failed to load texture with path: %s. Error: %s", path, ilGetString(error));
			return new_texture;
		}
		else
		{
			new_texture = new AssetTexture();
			new_texture->path = path;
			new_texture->buffer_id = ilutGLBindTexImage();
			new_texture->height  = ilGetInteger(IL_IMAGE_HEIGHT);
			new_texture->width = ilGetInteger(IL_IMAGE_WIDTH);
			new_texture->size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glBindTexture(GL_TEXTURE_2D, new_texture->buffer_id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, 0x8072, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			textures[path] = new_texture;

			glBindTexture(GL_TEXTURE_2D, 0);

		}
		//free(lump);
		ilDeleteImages(1, &devil_id);
		
		
	}
	else
	{
		LOG("Invalid path");
	}
	

	return new_texture;
}

bool ModuleTexture::CleanUp()
{
	for (std::map<std::string, AssetTexture*>::iterator iter = textures.begin(); iter!= textures.end();++iter)
	{
		if (((*iter).second))
		{
			delete ((*iter).second);
			(*iter).second = nullptr;
		}
	}
	textures.clear();
	return true;
}

void ModuleTexture::makeCheckTexture()
{
	AssetTexture* new_texture = new AssetTexture();

	int i, j, c;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	uint buffer = 0;
	
	glGenTextures(1, (uint*)&((*new_texture).buffer_id));
	glBindTexture(GL_TEXTURE_2D, ((*new_texture).buffer_id));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
		checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	textures["checkTexture"] = new_texture;
}
