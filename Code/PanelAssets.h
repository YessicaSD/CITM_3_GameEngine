#ifndef PANEL_ASSETS_H_
#define PANEL_ASSETS_H_

#include "Panel.h"
#include <string>

struct AssetDir;
struct AssetFile;

class PanelAssets : public Panel
{

public:
	float image_size = 100;
	PanelAssets(std::string name, bool state, std::vector<SDL_Scancode> shortcuts = {});
	void Draw();

	void DisplayFolderAssetsRecursive(AssetDir * dir);

	void DragAsset(AssetFile * asset);

	void DropObject(AssetFile * asset);

private:
	friend class ModuleGui;
	friend class PanelProperties;
};




#endif // !PANEL_ASSETS_H_
