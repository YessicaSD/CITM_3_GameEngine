#ifndef PANEL_ASSETS_H_
#define PANEL_ASSETS_H_

#include "Panel.h"

class PanelAssets : public Panel
{
public:

	PanelAssets(const char* name, bool state);
	void Draw();
};




#endif // !PANEL_ASSETS_H_
