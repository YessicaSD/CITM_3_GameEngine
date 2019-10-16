#include "ModuleTexture.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#pragma comment (lib, "DevIL/lib/DevIL.lib")
#pragma comment (lib, "DevIL/lib/ILU.lib")
#pragma comment (lib, "DevIL/lib/ILUT.lib")

#include "Texture.h"



bool ModuleTexture::Init()
{
	//Initialize DevIL libraries
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return true;
}

Texture* ModuleTexture::LoadTexture(const char* path)
{
	Texture* new_texture = nullptr;

	std::map<std::string, Texture*>::iterator iter= textures.find(path);
	if (iter!= textures.end())
	{
		return new_texture;
	}
	if (path != nullptr && path != "")
	{
		uint devil_id = 0;
		ilGenImages(1, &devil_id);
		ilBindImage(devil_id);
		ilutRenderer(ILUT_OPENGL);

		if (!ilLoad(IL_PNG, path))
		{
			auto error = ilGetError();
			LOG("Failed to load texture with path: %s. Error: %s", path, ilGetString(error));
			return new_texture;
		}
		else
		{
			new_texture = new Texture();
			new_texture->buffer_id = ilutGLBindTexImage();
			new_texture->height  = ilGetInteger(IL_IMAGE_HEIGHT);
			new_texture->width = ilGetInteger(IL_IMAGE_WIDTH);

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
		LOG("there is no path")
	}
	

	return new_texture;
}

bool ModuleTexture::CleanUp()
{
	for (std::map<std::string, Texture*>::iterator iter = textures.begin(); iter!= textures.end();++iter)
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
