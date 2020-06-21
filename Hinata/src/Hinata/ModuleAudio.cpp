#include "ModuleAudio.h"

#include "../Bass/include/bass.h"
#include "../Bass/include/bassenc.h"
#include "../Bass/include/bassenc_ogg.h"
#include "JSONFile.h"

#pragma comment( lib, "src/Bass/libx86/bass.lib" )
#pragma comment( lib, "src/Bass/libx86/bassenc.lib" )
#pragma comment( lib, "src/Bass/libx86/bassenc_ogg.lib" )

ModuleAudio::ModuleAudio(const char * name):Module(true, name)
{
}

bool ModuleAudio::Init(JSONFile * module_file)
{
	LOG("Loading Audio Module");

	if (BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL) != TRUE)
	{
		LOG("BASS_Init() error num: %i", BASS_ErrorGetCode());
		this->Disable();
		return true;
	}
	else
	{
		LOG("Init Bass successfully, Now you are using Bass %s", BASSVERSIONTEXT);
		BASS_DEVICEINFO info;
		for (int device = 0; BASS_GetDeviceInfo(device, &info); ++device)
		{
			if (info.flags & BASS_DEVICE_ENABLED) // device is enabled
				LOG("Audio device detected: %s", info.name);
			++number_of_devices;
		}
			
	}

	general_volume = BASS_GetVolume();
	if (general_volume == -1)
	{
		LOG("BASS_GetVolum error: %i", BASS_ErrorGetCode());
		this->Disable();
	}
	return true;
}

void ModuleAudio::SetVolume(float value)
{
	value = (value < 0.0f) ? 0.0f : (value > 1.0f) ? 1.0f : value;
	general_volume = value;
	BASS_SetVolume(value);
}

bool ModuleAudio::SaveConfiguration(JSONFile * module_file)
{
	module_file->SaveNumber("general volume", general_volume);

	return true;
}

bool ModuleAudio::LoadConfiguration(JSONFile * module_file)
{
	general_volume = module_file->LoadNumber("general volume");
	//SetVolume(general_volume);
	return true;
}
