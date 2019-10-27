#include "PanelConfiguration.h"

#include "imgui/imgui.h"
#include "Application.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>

#include "ModuleAudio.h"
#include "ModuleHardware.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"

//RULES: Every variable you add here should also be added in the Save and Load method of the corresponding module

void ModuleWindow::DrawConfigurationUi()
{
	if (ImGui::SliderFloat("Brightness", &brightness, 0.2f, 1.0f))
	{
		SetBrightness(brightness);
	}

	float width = GetWindowWidth();
	float height = GetWindowHeight();
	GetMaxWindowSize(max_width, max_height);
	bool changed_width  = ImGui::SliderFloat("Width",  &width,  min_width,  max_width);
	bool changed_height = ImGui::SliderFloat("Height", &height, min_height, max_height);
	if (changed_width || changed_height)
	{
		SetWindowSize(width, height);
	}

	ImGui::Text("Window options");
	const char * window_modes[] = {
		"No fullscreen",
		"Fullscreen",
		"Fullscreen des<ktop" };

	if (ImGui::Combo("Window mode", &current_window_mode, window_modes, IM_ARRAYSIZE(window_modes)))
	{
		SetFullscreenMode();
	}

	if (ImGui::Checkbox("Resizable", &App->window->resizable))
	{
		SDL_SetWindowResizable(App->window->window, App->window->resizable ? SDL_TRUE : SDL_FALSE);
	}
	if (ImGui::Checkbox("Borderless", &App->window->borderless))
	{
		SDL_SetWindowBordered(App->window->window, App->window->borderless ? SDL_FALSE : SDL_TRUE);
	}
	if (ImGui::Checkbox("Vsync", &App->window->vsync))
	{
		if (SDL_GL_SetSwapInterval(App->window->vsync ? 1 : 0) == -1)
			//1 for updates synchronized with the vertical retrace
			//0 for immediate updates
		{
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}
	}
}

void ModuleRenderer3D::DrawConfigurationUi()
{
	ImGui::Text("Change background color:");

	static float default_color_background[3] = { 3 / 255.F,19 / 255.F,29 / 255.F };
	if (ImGui::ColorPicker3("MyColor##4", background_col))
	{
		glClearColor(background_col[0], background_col[1], background_col[2], 1);
	}
	if (ImGui::Button("Reset"))
	{
		glClearColor(default_color_background[0], default_color_background[1], default_color_background[2], 1);
	}

	ImGui::Text("Render options");

	//Render options

	PanelConfiguration::RenderOption("Depth test", &depth_test, GL_DEPTH_TEST);
	PanelConfiguration::RenderOption("Cull faces", &cull_face, GL_CULL_FACE);
	PanelConfiguration::RenderOption("Lighting", &lighting, GL_LIGHTING);
	PanelConfiguration::RenderOption("Color materials", &color_material, GL_COLOR_MATERIAL);
	PanelConfiguration::RenderOption("Texture 2D", &texture_2d, GL_TEXTURE_2D);
}

void ModuleCamera3D::DrawConfigurationUi()
{
	ImGui::InputFloat("Camera speed", &camera_move_speed);
	ImGui::InputFloat("Roate speed", &camera_rotate_speed);
}

void ModuleInput::DrawConfigurationUi()
{
	int mouse_x = App->input->GetMouseX();
	int mouse_y = App->input->GetMouseY();
	ImGui::Text("Mouse Position:");
	ImGui::SameLine();
	ImGui::Text("%i,%i", mouse_x, mouse_y);

	int mouse_motion_x = App->input->GetMouseMotionX();
	int mouse_motion_y = App->input->GetMouseMotionY();
	ImGui::Text("Mouse Motion:");
	ImGui::SameLine();
	ImGui::Text("%i,%i", mouse_x, mouse_y);

	int mouse_wheel = App->input->GetMouseWheel();
	ImGui::Text("Mouse Wheel:");
	ImGui::SameLine();
	ImGui::Text("%i", mouse_wheel);

	ImGui::Separator();

	ImGui::BeginChild("Input Log");
	
	ImGui::TextUnformatted(input_log_buffer.begin());
	static bool scroll_input_log = false;
	if (scroll_input_log)
	{
		ImGui::SetScrollHere(1.0f);
	}
	scroll_input_log = false;
	
	ImGui::EndChild();
}

void ModuleAudio::DrawConfigurationUi()
{
	if (ImGui::SliderFloat("General volume", &general_volume, 0.0f, 1.0f))
	{
		SetVolume(general_volume);
	}
}
void ModuleHardware::DrawConfigurationUi()
{
	ImGui::Text("SDL Version: %s", info.sdl_version);
	ImGui::Separator();

	ImGui::Text("CPUs: %u (Cache: %ukb)", info.cpu_cores, info.cachekb);

	ImGui::Text("System RAM: %.1fGb", info.ram_gb);
	ImGui::Text("Caps: %s%s%s%s%s%s",
		info.rdtsc ? "RDTSC," : "",
		info.alti_vec ? "AltiVec," : "",
		info.mmx ? "MMX," : "",
		info.now3d ? "3DNow," : "",
		info.sse ? "SSE," : "",
		info.sse2 ? "SSE2," : "");
	ImGui::Text("", "%s%s%s%s%s",
		info.sse3 ? "SSE3," : "",
		info.sse41 ? "SSE41," : "",
		info.sse42 ? "SSE42," : "",
		info.avx ? "AVX," : "",
		info.avx2 ? "AVX2" : "");


	ImGui::Separator();
	ImGui::Text("VRAM Budget: ");
	ImGui::SameLine();
	int aux_memory = 0;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &aux_memory);
	ImGui::Text( "%i Mb", aux_memory / 1024);

	ImGui::Text("VRAM Usage: ");
	ImGui::SameLine();
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &aux_memory);
	ImGui::Text( "%i Mb", aux_memory / 1024);

	ImGui::Text("VRAM Available: ");
	ImGui::SameLine();
	glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &aux_memory);
	ImGui::Text( "%i Mb", aux_memory / 1024);

	ImGui::Text("VRAM Reserved: ");
	ImGui::SameLine();
	glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &aux_memory);
	ImGui::Text( "%i Mb", aux_memory / 1024);
}
