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
	bool Init(JSON_Object* config);
	void SetVolume(float value);
	bool SaveConfiguration(JSON_Object * module_obj) override;
	bool LoadConfiguration(JSON_Object * module_obj) override;
	void DrawConfigurationUi() override;
};
#endif // !MODULE_AUDIO_H_
