#include "Cube.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>

/*
						@ + + + + + + + + + + + @
						+\                      +\
						+ \                     + \
						+  \                    +  \
						+   \                   +   \
						+    @ + + + + + + + + +++ + @
						+    +                  +    +
						+    +                  +    +
						+    +                  +    +
						+    +                  +    +
						+    +                  +    +
						+    +                  +    +
						@ + +++ + + + + + + + + @    +
						 \   +                   \   +
						  \  +                    \  +
						   \ +                     \ +
						    \+                      \+
						     @ + + + + + + + + + + + @  
							 
	 */
		
Cube::Cube(float width, float height, float depth, float3	position)
{
	OpenBuffers();
	Set(width, height, depth, position);

}

Cube::Cube()
{
	OpenBuffers();
	Set(1, 1, 1, float3(0, 0, 0));

}

//void Cube::Draw_DirectMode()
//{
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glBindBuffer(GL_ARRAY_BUFFER, vetex_buf_id);
//	glVertexPointer(3, GL_FLOAT, 0, NULL);
//	// … draw other buffers
//	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
//	glDisableClientState(GL_VERTEX_ARRAY);
//
//}

void Cube::Draw(const float * matrix)
{
	if (matrix)
	{
		glPushMatrix();
		glMultMatrixf(matrix);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vetex_buf_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf_id);
	glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, NULL);
	if (matrix)
	{
		glPopMatrix();
	}
}

void Cube::Set(float width, float height, float depth, float3 position)
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

	glBindBuffer(GL_ARRAY_BUFFER, vetex_buf_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NUM_VERTICES * 3, vertices, GL_STATIC_DRAW);

	uint indices[NUM_INDICES] = { 0,2,1, 0,3,2, 1,2,5,2,6,5 ,2,3,6,3,7,6,   4,5,6,6,7,4,	0,4,3,4,7,3,	0,1,5,5,4,0 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * NUM_INDICES, indices, GL_STATIC_DRAW);
}

void Cube::OpenBuffers()
{
	//Opening buffer for vertexs
	glGenBuffers(1, (GLuint*) & (vetex_buf_id));
	
	//Opening buffer for indeces
	glGenBuffers(1, (GLuint*) & (indices_buf_id));
	
}
