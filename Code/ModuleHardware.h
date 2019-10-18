#ifndef MODULE_HARDWARE_H_
#define MODULE_HARDWARE_H_

#include "Module.h"
typedef unsigned int uint;
class ModuleHardware : public Module
{
private:
	struct hardware_info
	{
		char sdl_version[25] = "";
		float ram_gb = 0.0f;
		uint cpu_cores = 0;
		uint cachekb = 0;
		bool rdtsc = false;
		bool altivec = false;
		bool now3d = false;
		bool mmx = false;
		bool sse = false;
		bool sse2 = false;
		bool sse3 = false;
		bool sse41 = false;
		bool sse42 = false;
		bool avx = false;
		bool avx2 = false;
		uint gpu_vendor = 0;
		uint gpu_device = 0;
		char gpu_brand[250] = "";
		float vram_mb_budget = 0.f;
		float vram_mb_usage = 0.f;
		float vram_mb_available = 0.f;
		float vram_mb_reserved = 0.f;
	} info;

	
public:
	ModuleHardware(const char* name);
	update_status PreUpdate() override;
	void DrawConfigurationUi();
};
#endif // !MODULE_HARDWARE_H_

