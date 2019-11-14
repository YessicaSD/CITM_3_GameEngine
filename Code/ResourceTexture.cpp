#include "ResourceTexture.h"
#include "glew/include/GL/glew.h"
#include "DevIL/include/il.h"

ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::SaveFileData()
{
	//TODO: Save resource texture (see ResourceMesh as an example)
	return true;
}

bool ResourceTexture::LoadFileData()
{
	//TODO: Finsih this function. Use ResourceMesh as an example.
	return true;
}

bool ResourceTexture::ReleaseData()
{
	glDeleteTextures(1, &buffer_id);
	//TODO: Finish this function
	return true;
}
