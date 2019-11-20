#ifndef PANEL_ASSETS_H_
#define PANEL_ASSETS_H_

#include "Panel.h"
#include <string>

struct Dir;

class PanelAssets : public Panel
{

public:
	float image_size = 100;
	PanelAssets(std::string name, bool state, std::vector<SDL_Scancode> shortcuts = {});
	void Draw();

	//TODO: Remove this function
	void FillAssetTreeRecursive(Dir * dir);

	void DeleteTreeRecursive(Dir * dir);

	void DisplayFolderAssetsRecursive(Dir * dir);

	friend class ModuleGui;
};




#endif // !PANEL_ASSETS_H_
