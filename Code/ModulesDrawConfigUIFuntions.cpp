#include "imgui/imgui.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "PanelConfiguration.h"
#include "ModuleAudio.h"
#include "ModuleHardware.h"
#include "glew\include\GL\glew.h"
#include <gl\GL.h>

void ModuleWindow::DrawConfigurationUi()
{
	static float brightness = 100;
	if (ImGui::SliderFloat("Brightness", &brightness, 0.2f, 1.0f))
	{
		SetBrightness(brightness);
	}
	static float max_width, max_height, min_width = 640, min_height = 480;
	static float width = GetWindowWidth(),
		height = GetWindowHeight();
	GetMaxWindowSize(max_width, max_height);


	if (ImGui::SliderFloat("Width", &width, min_width, max_width))
	{
		SetWidth(width);
	}
	if (ImGui::SliderFloat("Height", &height, min_height, max_height))
	{
		
SetHeight(height);
	}
}

void ModuleRenderer3D::DrawConfigurationUi()
{
	
		ImGui::Text("Change background color:");

		static float default_color_background[3] = { 3 / 255.F,19 / 255.F,29 / 255.F };
		static float col1[3] = { 1.0f,0.0f,0.2f };
		if (ImGui::ColorPicker4("MyColor##4", col1))
		{
			glClearColor(col1[0], col1[1], col1[2], 1);
		}
		if (ImGui::Button("Reset"))
		{
			glClearColor(default_color_background[0], default_color_background[1], default_color_background[2], 1);

		}

		ImGui::Text("Window options");
		const char * window_modes[] = {
			"No fullscreen",
			"Fullscreen",
			"Fullscreen desktop" };

		static int current_window_mode = 0;
		if (ImGui::Combo("Window mode", &current_window_mode, window_modes, IM_ARRAYSIZE(window_modes)))
		{
			Uint32 change_mode = 0;
			if (current_window_mode == 0)
			{
				change_mode = 0;
			}
			else if (current_window_mode == 1)
			{
				change_mode = SDL_WINDOW_FULLSCREEN;
			}
			else if (current_window_mode == 2)
			{
				change_mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
			}
			SDL_SetWindowFullscreen(App->window->window, change_mode);
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

		ImGui::Text("Render options");

		//Render options

		PanelConfiguration::RenderOption("Depth test", &depth_test, GL_DEPTH_TEST);
		PanelConfiguration::RenderOption("Cull faces", &cull_face, GL_CULL_FACE);
		PanelConfiguration::RenderOption("Lighting", &lighting, GL_LIGHTING);
		PanelConfiguration::RenderOption("Color materials", &color_material, GL_COLOR_MATERIAL);
		PanelConfiguration::RenderOption("Texture 2D", &texture_2d, GL_TEXTURE_2D);

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
	if (ImGui::SliderFloat("General volum", &general_volume, 0.0f, 1.0f))
	{
		SetVolum(general_volume);
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
	ImGui::Text("GPU: vendor %u device %u", info.gpu_vendor, info.gpu_device);
	ImGui::Text("Brand: %s", info.gpu_brand);
	ImGui::Text("VRAM Budget: %.1f Mb", info.vram_mb_budget);
	ImGui::Text("VRAM Usage: %.1f Mb", info.vram_mb_usage);
	ImGui::Text("VRAM Available: %.1f Mb", info.vram_mb_available);
	ImGui::Text("VRAM Reserved: %.1f Mb", info.vram_mb_reserved);
}
