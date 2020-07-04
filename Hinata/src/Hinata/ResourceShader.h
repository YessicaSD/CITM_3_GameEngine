#pragma once
#include "Resource.h"
#include "../glew/include/GL/glew.h"
#include "../MathGeoLib/include/Math/float4.h"
#include "../MathGeoLib/include/Math/float4x4.h"

class ResourceShader :
	public Resource
{
	RESOURCE_DECLARATION(ResourceShader);
public:
	void Bind();
	void Unbind();

	void UploadUniformFloat4(const std::string& name, const float4& values);
	void UploadUniformMat4(const std::string& name, const float4x4& values);
	

	GLuint shaderProgram;
};

