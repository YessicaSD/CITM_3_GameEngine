#ifndef __RENDER_TEXTURE_H__
#define __RENDER_TEXTURE_H__

typedef unsigned int uint;
struct ImVec2;

//A class which uses a frame buffer object to render to a texture
//https://learnopengl.com/Advanced-OpenGL/Framebuffers
class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();

	void GenerateFrameBuffer();

	void StartRender(ImVec2 size);
	void EndRender();
	//TODO: Delete buffers

private:
	//GenerateFrameBuffer
	void GenerateDepthBuffer();
	void GenerateRenderTexture();
	//StartRender
	void PrepareCamera(ImVec2& size);
	void PrepareCameraPosition();
	void PrepareCameraFrustum(ImVec2& size);
	void PrepareDepthBuffer(ImVec2& size);
	void PrepareTextureBuffer(ImVec2& size);

public:
	uint frame_buffer = 0u;
	uint render_texture = 0u;
	uint depth_render_buffer = 0u;
	uint stencil_buffer = 0u;

	ImVec2* panel_size = nullptr;
	float clear_color[4];
};

#endif
