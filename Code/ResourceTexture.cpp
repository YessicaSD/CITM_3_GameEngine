#include "ResourceTexture.h"
#include "glew/include/GL/glew.h"
#include "DevIL/include/il.h"

//void Texture::GenerateTexture()
//{
//	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	//glBindTexture(GL_TEXTURE_2D, buffer_id);
//
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//	//glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
//	//	0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
//
//	//glBindTexture(GL_TEXTURE_2D, 0);
//
//	
//
//
//}
//
//void Texture::Render()
//{
//}

ResourceTexture::~ResourceTexture()
{
	if (buffer_id > 0)
	{
		ilDeleteImages(1, (ILuint*)&buffer_id);
	}
}

bool ResourceTexture::GenerateWriteData()
{
	//TODO: Save resource texture (see ResourceMesh as an example)
	return true;
}
