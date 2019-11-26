#include "CubeLine.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>

void CubeLine::OpenBuffers()
{
	//Opening buffer for vertexs
	glGenBuffers(1, (GLuint*) & (vetex_buf_id));
	glBindBuffer(GL_ARRAY_BUFFER, vetex_buf_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NUM_VERTICES * 3, nullptr, GL_DYNAMIC_DRAW);
	//Opening buffer for indeces
	glGenBuffers(1, (GLuint*) & (indices_buf_id));
}

void CubeLine::SetIndices()
{
	uint indices[NUM_INDICES] = {
	0,1, 0,1,  4,5,	0,4,//Front
	3,2, 2,0, 5,1, 1,3,
	7,6, 6,2, 2,3, 3,7,
	6,4, 2,0, 7,5, 3,1 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * NUM_INDICES, indices, GL_STATIC_DRAW);
}

CubeLine::CubeLine(float width, float height, float depth, float3 position)
{
	OpenBuffers();
	SetIndices();
	Set(width, height, depth, position);
}

CubeLine::CubeLine()
{
	OpenBuffers();
	SetIndices();
	Set(1, 1, 1, float3(0,0,0));
}

CubeLine::~CubeLine()
{
	CleanUp();
}

void CubeLine::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vetex_buf_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf_id);
	glDrawElements(GL_LINES, NUM_INDICES, GL_UNSIGNED_INT, NULL);
}

void CubeLine::Set(float width, float height, float depth, float3 position)
{
	float half_width = 0.5f*width;
	float half_height = 0.5f*height;
	float half_depth = 0.5f*depth;

	float vertices[8 * 3] = { -half_width + position.x, -half_height + position.y, -half_depth + position.z, // 0, 0 , 0
							   -half_width + position.x, -half_height + position.y, half_depth + position.z, //0,0,1
							   half_width + position.x, -half_height + position.y, half_depth + position.z, // 1, 0, 1
							   half_width + position.x, -half_height + position.y, -half_depth + position.z, // 1,0,0
								-half_width + position.x, half_height + position.y, -half_depth + position.z, // 0, 1, 0
								-half_width + position.x, half_height + position.y, half_depth + position.z, // 0 1 1
								half_width + position.x, half_height + position.y, half_depth + position.z, // 1,1,1
								half_width + position.x, half_height + position.y, -half_depth + position.z }; //1,1,0

	
}

void CubeLine::SetVetices(float * vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, vetex_buf_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * NUM_VERTICES * 3, vertices);
}

void CubeLine::CleanUp()
{
	glDeleteBuffers(1, &vetex_buf_id);
	glDeleteBuffers(1, &indices_buf_id);
}
