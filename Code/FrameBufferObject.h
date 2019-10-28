#ifndef __FRAME_BUFFER_OBJECT_H__
#define __FRAME_BUFFER_OBJECT_H__

typedef unsigned int uint;
struct ImVec2;

//Used for rendering to a texture
class FrameBufferObject
{
public:
	FrameBufferObject();
	~FrameBufferObject();

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
