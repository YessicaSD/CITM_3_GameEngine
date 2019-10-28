#include"RenderTexture.h"
#include "glew/include/GL/glew.h"
#include "Globals.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleGui.h"
#include "PanelScene.h"

RenderTexture::RenderTexture()
{
	clear_color[0] = 0;
	clear_color[1] = 0;
	clear_color[2] = 0;
	clear_color[3] = 1;

	panel_size = new ImVec2();
}

RenderTexture::~RenderTexture()
{
	delete panel_size;
}

void RenderTexture::GenerateFrameBuffer()
{
	//Generate frame buffer
	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

	GenerateDepthBuffer();
	GenerateRenderTexture();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("[Error] creating screen buffer");
	}

	//Reset framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::GenerateDepthBuffer()
{
	//Generate depth render buffer
	glGenRenderbuffers(1, &depth_render_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer);
	
	//Attach to frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_render_buffer);
	
	//Reset depth render buffer
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderTexture::GenerateRenderTexture()
{
	//Generate render texture
	glGenTextures(1, &render_texture);
	glBindTexture(GL_TEXTURE_2D, render_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	//Attach to frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_texture, 0);
	
	//Reset render texture
	glBindTexture(GL_TEXTURE_2D, 0);
}


//TODO: If this is updated in ModuleInput->PreUpdate we maybe should change the viewport size
void RenderTexture::StartRender(ImVec2 size)
{
	PrepareCamera(size);
	PrepareDepthBuffer(size);
	PrepareTextureBuffer(size);

	//Set camera
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	//Set fame buffer object
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Stencil
	glStencilFunc(GL_ALWAYS, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void RenderTexture::PrepareCamera(ImVec2& size)
{
	glViewport(0, 0, size.x, size.y);

	glMatrixMode(GL_PROJECTION);
	App->renderer3D->projection_matrix = perspective(60.0f, size.x / size.y, App->renderer3D->camera_near, App->renderer3D->camera_far);
	glLoadMatrixf(&App->renderer3D->projection_matrix);

	//Reset camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void RenderTexture::PrepareDepthBuffer(ImVec2& size)
{
	glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);

	//Reset buffer
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderTexture::PrepareTextureBuffer(ImVec2& size)
{
	glBindTexture(GL_TEXTURE_2D, render_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	//Reset buffer
	glBindTexture(GL_TEXTURE_2D, 0);
}


void RenderTexture::EndRender()
{
	//Stencil
	glStencilFunc(GL_ALWAYS, 1, 0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	
	//TODO: Reset to default clear color
	//glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}