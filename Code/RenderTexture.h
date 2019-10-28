#ifndef __RENDER_TEXTURE_H__
#define __RENDER_TEXTURE_H__

typedef unsigned int uint;
struct ImVec2;

//A classe which uses frame buffer objects for rendering to a texture
class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();

	void GenFramebuffer();
	void StartRenderingToTexture(ImVec2 size);
	void EndRenderingToTexture();

private:
	void PrepareCamera(ImVec2& size);
	void PrepareDepthBuffer(ImVec2& size);
	void PrepareTextureBuffer(ImVec2& size);

public:
	uint frame_buffer = 0u;
	uint render_texture = 0u;
	uint depth_render_buffer = 0u;
	uint stencil_buffer = 0u;

	ImVec2* panel_size = nullptr;
};

#endif
