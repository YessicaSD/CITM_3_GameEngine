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
	float vertices[8 * 3] = { 0.f,0.f,0.f,  0.f,0.f,1.f,  1.f,0.f ,1.f,    1.f,0.f ,0.f ,   0.f ,1.f,0.f ,  0.f ,1.f,1.f,  1.f,1.f,1.f,  1.f,1.f,0.f };
	glBindBuffer(GL_ARRAY_BUFFER, vetex_buf_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NUM_VERTICES * 3, vertices, GL_STATIC_DRAW);
	
	uint indices[NUM_INDICES] = { 0,2,1, 0,3,2, 1,2,5,2,6,5 ,2,3,6,3,7,6,   4,5,6,6,7,4,	0,4,3,4,7,3,	0,1,5,5,4,0 };
	glGenBuffers(1, (GLuint*) & (indices_buf_id));
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * NUM_INDICES, indices, GL_STATIC_DRAW);
	
}

void Cube::Draw_DirectMode()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vetex_buf_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void Cube::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vetex_buf_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … draw other buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf_id);
	glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, NULL);
}

void Cube::Set(float width, float height, float depth, float3 position)
{
	float vertices[8 * 3] = { 0.f + position.x,0.f + position.y,0.f + position.z,  0.f + position.x,0.f + position.y,1.f*depth + position.z,  1.f*width + position.x,0.f + position.y,1.f*depth + position.z,    1.f*width + position.x,0.f + position.y,0.f + position.z,   0.f + position.x,1.f*height + position.y,0.f + position.z,  0.f + position.x,1.f*height + position.y,1.f*depth + position.z,  1.f*width + position.x,1.f*height + position.y,1.f*depth + position.z,  1.f*width + position.x,1.f*height + position.y,0.f + position.z };
	glBindBuffer(GL_ARRAY_BUFFER, vetex_buf_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NUM_VERTICES * 3, vertices, GL_STATIC_DRAW);

	uint indices[NUM_INDICES] = { 0,2,1, 0,3,2, 1,2,5,2,6,5 ,2,3,6,3,7,6,   4,5,6,6,7,4,	0,4,3,4,7,3,	0,1,5,5,4,0 };
	glGenBuffers(1, (GLuint*) & (indices_buf_id));
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * NUM_INDICES, indices, GL_STATIC_DRAW);
}

void Cube::OpenBuffers()
{
	//Opening buffer for vertexs
	glGenBuffers(1, (GLuint*) & (vetex_buf_id));
	
	//Opening buffer for indeces
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf_id);
}
