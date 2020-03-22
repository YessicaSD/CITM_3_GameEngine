#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"
#include "Globals.h"

#pragma comment(lib, "DevIL/lib/DevIL.lib")
#pragma comment(lib, "DevIL/lib/ILU.lib")
#pragma comment(lib, "DevIL/lib/ILUT.lib")

#include "ModuleImportTexture.h"
#include "ResourceTexture.h"
#include "Application.h"
#include "ModuleResourceManager.h"
#define IMAGE_WIDTH 100
//#define checkImageWidth 512
//#define checkImageHeight 512
//static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

ModuleImportTexture::ModuleImportTexture(const char *name) : Module(true, name)
{
}

bool ModuleImportTexture::Init(JSONFile *module_file)
{
	//Initialize DevIL libraries
	LOG("Initializing DevIl libraries");
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	return true;
}

//Saves the texture as DDS in the Library folder (faster to use)
ResourceTexture* ModuleImportTexture::ImportTexture(const char * asset_path, UID forced_uid)
{
	Timer import_timer;

	ResourceTexture* resource_texture = App->resource_manager->CreateResource<ResourceTexture>(forced_uid);
	resource_texture->asset_source = asset_path;

	//Import data from path first
	uint image_id = 0u;
	ilGenImages(1, &image_id);
	ilBindImage(image_id);

	resource_texture->mini_image_buffer = ilGenImage();

	if (ilLoadImage(asset_path) == IL_TRUE)
	{
		ilBindImage(resource_texture->mini_image_buffer);
		ilCopyImage(image_id);

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		resource_texture->SaveFileData();
		SaveTextureMeta(resource_texture, asset_path);

		int div_result =  ImageInfo.Width / IMAGE_WIDTH;
		iluScale(ImageInfo.Width/ div_result, ImageInfo.Height/ div_result, ImageInfo.Depth/ div_result);

		resource_texture->mini_image_buffer = ilutGLBindTexImage();
		//Get the data?
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, resource_texture->mini_image_buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, 0x8072, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		ilDeleteImages(1, &image_id);
		ilBindImage(0);
		LOG("Success importing texture from: %s in: %i ms", asset_path, import_timer.Read());
	}
	else
	{
		auto error = ilGetError();
		LOG("Failed to load texture with path: %s. Error: %s", asset_path, ilGetString(error));
	}
	//free(lump);

	return resource_texture;
}

void ModuleImportTexture::SaveTextureMeta(ResourceTexture * resource_texture, const char * asset_path)
{
	//INFO Create .meta
	JSONFile meta_file;
	meta_file.CreateJSONFile();
	App->resource_manager->SaveUID(&meta_file, resource_texture->uid);
	resource_texture->SaveModifiedDate(&meta_file, asset_path);
	//TODO: Add import options
	meta_file.SaveFile(std::string(asset_path) + std::string(".") + std::string(META_EXTENSION));
	meta_file.CloseFile();
}

//void ModuleTexture::ImportCheckerTexture()
//{
//	ResourceTexture* new_texture = App->resource_manager->CreateResource<ResourceTexture>();
//	new_texture->asset_source = "checker texture";
//
//	int i, j, c;
//
//	for (i = 0; i < checkImageHeight; i++)
//	{
//		for (j = 0; j < checkImageWidth; j++)
//		{
//			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
//			checkImage[i][j][0] = (GLubyte)c;
//			checkImage[i][j][1] = (GLubyte)c;
//			checkImage[i][j][2] = (GLubyte)c;
//			checkImage[i][j][3] = (GLubyte)255;
//		}
//	}
//
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//	uint buffer = 0;
//
//	glGenTextures(1, (uint *)&((*new_texture).buffer_id));
//	glBindTexture(GL_TEXTURE_2D, ((*new_texture).buffer_id));
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
//					GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
//					GL_NEAREST);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
//				 checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
//				 checkImage);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	//Save as dds
//}
