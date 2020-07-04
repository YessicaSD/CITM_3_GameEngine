#include "ResourceShader.h"

RESOURCE_DEFINITION(Resource, ResourceShader);

void ResourceShader::Bind()
{
	glUseProgram(shaderProgram);
}

void ResourceShader::Unbind()
{
	glUseProgram(0);
}

void ResourceShader::UploadUniformFloat4(const std::string& name, const float4& values)
{
	GLint location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform4f(location,values.x, values.y, values.z, values.w);
}

void ResourceShader::UploadUniformMat4(const std::string& name, const float4x4& values)
{
	GLint location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, values.ptr());
}
