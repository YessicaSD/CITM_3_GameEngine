#ifndef __PANEL_PROPERTIES_H__
#define __PANEL_PROPERTIES_H__

#include "Panel.h"
#include <string>
#include "ComponentTransform.h"
#include "ModuleResourceManager.h"

class ResourceTexture;
class PanelProperties : public Panel
{
private:
	ComponentTransform* selected_transform = nullptr;
	AssetFile selected_asset;
	bool asset_is_selected = false;
	ResourceTexture* resource_selected = nullptr;
public:
	PanelProperties(std::string name, bool state, std::vector<SDL_Scancode> shortcuts = {});
	void Draw() override;
	const ComponentTransform* GetSelecteTransform();

	void SetFileSelected(AssetFile selected_asset);

	friend class PanelHierarchy;
	friend class ModuleGui;
};



#endif // !PANEL_PROPERTIES_H_

