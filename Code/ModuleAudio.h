#ifndef MODULE_AUDIO_H_
#define MODULE_AUDIO_H_
#include "Module.h"
class ModuleAudio : public Module
{
private:
	uint number_of_devices = 0;
	float general_volume = 0.0f;
public:
	ModuleAudio(const char* name);
	bool Init(JSONFile * module_file);
	void SetVolume(float value);
	bool SaveConfiguration(JSONFile * module_file) override;
	bool LoadConfiguration(JSONFile * module_file) override;
	void DrawConfigurationUi() override;
};
#endif // !MODULE_AUDIO_H_
